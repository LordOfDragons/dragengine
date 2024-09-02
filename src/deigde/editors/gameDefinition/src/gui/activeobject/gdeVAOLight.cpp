/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gdeVAOLight.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/light/gdeOCLight.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOLight
//////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOLight::gdeVAOLight( gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix, gdeOCLight *oclight ) :
gdeVAOSubObject( view, objectClass, propertyPrefix ),
pOCLight( oclight ),
pDDSCenter( NULL ),
pDDSCoordSystem( NULL )
{
	if( ! oclight ){
		DETHROW( deeInvalidParam );
	}
	
	pOCLight->AddReference();
	
	try{
		pCreateDebugDrawer();
		pCreateLight();
		pUpdateDDShapes();
		pUpdateDDShapeColor();
		
		AttachResources();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeVAOLight::~gdeVAOLight(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOLight::Update( float elapsed ){
}

void gdeVAOLight::RebuildResources(){
	DetachResources();
	pReleaseResources();
	
	pCreateLight();
	AttachResources();
	
	pUpdateDDShapes();
	pUpdateDDShapeColor();
}

void gdeVAOLight::AttachResources(){
	if( ! pLight ){
		return;
	}
	
	deCollider * const attachCollider = pView.GetAttachComponentCollider();
	if( ! attachCollider ){
		return;
	}
	
	const decVector position( PropertyVector( pOCLight->GetPropertyName( gdeOCLight::epAttachPosition ), pOCLight->GetPosition() ) );
	const decQuaternion orientation( PropertyQuaternion( pOCLight->GetPropertyName( gdeOCLight::epAttachRotation ), pOCLight->GetRotation() ) );
	const decString &bone = pOCLight->GetBoneName();
	
	deColliderAttachment *attachment = NULL;
	try{
		// attach light
		attachment = new deColliderAttachment( pLight );
		attachment->SetPosition( position );
		attachment->SetOrientation( orientation );
		
		if( bone.IsEmpty() ){
			attachment->SetAttachType( deColliderAttachment::eatStatic );
			
		}else{
			attachment->SetTrackBone( bone );
			attachment->SetAttachType( deColliderAttachment::eatBone );
		}
		
		attachCollider->AddAttachment( attachment );
		attachment = NULL;
		
		// attach debug drawer
		attachment = new deColliderAttachment( pDebugDrawer );
		attachment->SetPosition( position );
		attachment->SetOrientation( orientation );
		
		if( bone.IsEmpty() ){
			attachment->SetAttachType( deColliderAttachment::eatStatic );
			
		}else{
			attachment->SetTrackBone( bone );
			attachment->SetAttachType( deColliderAttachment::eatBone );
		}
		
		attachCollider->AddAttachment( attachment );
		attachment = NULL;
		
	}catch( const deException & ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
}

void gdeVAOLight::DetachResources(){
	if( ! pLight ){
		return;
	}
	
	deCollider * const attachCollider = pView.GetAttachComponentCollider();
	if( ! attachCollider ){
		return;
	}
	
	deColliderAttachment *attachment = NULL;
	attachment = attachCollider->GetAttachmentWith( pLight );
	if( attachment ){
		attachCollider->RemoveAttachment( attachment );
	}
	
	attachment = attachCollider->GetAttachmentWith( pDebugDrawer );
	if( attachment ){
		attachCollider->RemoveAttachment( attachment );
	}
}

void gdeVAOLight::SelectedObjectChanged(){
	pUpdateDDShapeColor();
}

void gdeVAOLight::ShadowIgnoreComponent( deComponent *component ){
	if( pLight && ! pLight->HasShadowIgnoreComponent( component ) ){
		pLight->AddShadowIgnoreComponent( component );
	}
}



// Private functions
//////////////////////

void gdeVAOLight::pCleanUp(){
	DetachResources();
	pReleaseResources();
	
	if( pDDSCoordSystem ){
		delete pDDSCoordSystem;
	}
	if( pDDSCenter ){
		delete pDDSCenter;
	}
	if( pDebugDrawer ){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer( pDebugDrawer );
		pDebugDrawer = NULL;
	}
	
	if( pOCLight ){
		pOCLight->FreeReference();
	}
}



void gdeVAOLight::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// create debug drawer
	pDebugDrawer.TakeOver( engine.GetDebugDrawerManager()->CreateDebugDrawer() );
	pDebugDrawer->SetXRay( true );
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer( pDebugDrawer );
	
	// create center shape
	pDDSCenter = new igdeWDebugDrawerShape;
	pDDSCenter->AddSphereShape( 0.05f, decVector() );
	pDDSCenter->SetParentDebugDrawer( pDebugDrawer );
	
	// create coordinate system shape
	pDDSCoordSystem = new igdeWCoordSysArrows;
	pDDSCoordSystem->SetArrowLength( 0.2f );
	pDDSCoordSystem->SetArrowSize( 0.01f );
	pDDSCoordSystem->SetParentDebugDrawer( pDebugDrawer );
}

void gdeVAOLight::pCreateLight(){
	deVirtualFileSystem * const vfs = pView.GetGameDefinition()->GetPreviewVFS();
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	pLight.TakeOver( engine.GetLightManager()->CreateLight() );
	
	decString typeName;
	switch( pOCLight->GetType() ){
	case deLight::eltSpot:
		typeName = "spot";
		break;
		
	case deLight::eltProjector:
		typeName = "projector";
		break;
		
	case deLight::eltPoint:
	default:
		typeName = "point";
		break;
	}
	
	typeName = PropertyString( pOCLight->GetPropertyName( gdeOCLight::epType ), typeName );
	
	if( typeName == "spot" ){
		pLight->SetType( deLight::eltSpot );
		
	}else if( typeName == "projector" ){
		pLight->SetType( deLight::eltProjector );
		
	}else{
		pLight->SetType( deLight::eltPoint );
	}
	
	pLight->SetHintMovement( pOCLight->GetHintMovement() );
	pLight->SetHintParameter( pOCLight->GetHintParameter() );
	pLight->SetColor( PropertyColor( pOCLight->GetPropertyName( gdeOCLight::epColor ), pOCLight->GetColor() ) );
	pLight->SetIntensity( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epIntensity ), pOCLight->GetIntensity() ) );
	pLight->SetRange( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epRange ), pOCLight->GetRange() ) );
	pLight->SetAmbientRatio( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epAmbientRatio ), pOCLight->GetAmbientRatio() ) );
	pLight->SetHalfIntensityDistance( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epHalfIntDist ), pOCLight->GetHalfIntensityDistance() ) );
	pLight->SetSpotAngle( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epSpotAngle ), pOCLight->GetSpotAngle() ) * DEG2RAD );
	pLight->SetSpotRatio( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epSpotRatio ), pOCLight->GetSpotRatio() ) );
	pLight->SetSpotSmoothness( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epSpotSmoothness ), pOCLight->GetSpotSmoothness() ) );
	pLight->SetSpotExponent( PropertyFloat( pOCLight->GetPropertyName( gdeOCLight::epSpotExponent ), pOCLight->GetSpotExponent() ) );
	pLight->SetCastShadows( PropertyBool( pOCLight->GetPropertyName( gdeOCLight::epCastShadows ), pOCLight->GetCastShadows() ) );
	pLight->SetHintLightImportance( PropertyInt( pOCLight->GetPropertyName( gdeOCLight::epHintLightImportance ), pOCLight->GetHintLightImportance() ) );
	pLight->SetHintShadowImportance( PropertyInt( pOCLight->GetPropertyName( gdeOCLight::epHintShadowImportance ), pOCLight->GetHintShadowImportance() ) );
	
	// light skin
	decString path( PropertyString( pOCLight->GetPropertyName( gdeOCLight::epLightSkin ), pOCLight->GetLightSkinPath() ) );
	deSkinReference skin;
	if( ! path.IsEmpty() ){
		try{
			skin.TakeOver( engine.GetSkinManager()->LoadSkin( vfs, path, "/" ) );
			
		}catch( const deException & ){
			skin = environment.GetStockSkin( igdeEnvironment::essError );
		}
	}
	
	try{
		pLight->SetLightSkin( skin );
		
	}catch( const deException &e ){
		environment.GetLogger()->LogException( LOGSOURCE, e );
	}
	
	pView.GetGameDefinition()->GetWorld()->AddLight( pLight );
}

void gdeVAOLight::pUpdateDDShapes(){
}

void gdeVAOLight::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if( pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCLight
	&& pView.GetGameDefinition()->GetActiveOCLight() == pOCLight ){
		pDDSCenter->SetEdgeColor( decColor( config.GetColorLightActive(), 1.0f ) );
		pDDSCenter->SetFillColor( config.GetColorLightActive() );
		
	}else{
		pDDSCenter->SetEdgeColor( decColor( config.GetColorLight(), 0.25f ) );
		pDDSCenter->SetFillColor( config.GetColorLight() );
	}
}



void gdeVAOLight::pReleaseResources(){
	if( pLight ){
		pView.GetGameDefinition()->GetWorld()->RemoveLight( pLight );
		pLight = NULL;
	}
}
