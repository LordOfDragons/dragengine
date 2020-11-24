/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

gdeVAOLight::gdeVAOLight( gdeViewActiveObject &view, gdeOCLight *oclight ) :
pView( view ),
pOCLight( oclight ),
pLight( NULL ),
pDebugDrawer( NULL ),
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
	
	const decVector &position = pOCLight->GetPosition();
	const decQuaternion orientation( decQuaternion::CreateFromEuler(
		pOCLight->GetRotation() * DEG2RAD ) );
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
		
	}catch( const deException &e ){
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
		pDebugDrawer->FreeReference();
	}
	
	if( pOCLight ){
		pOCLight->FreeReference();
	}
}



void gdeVAOLight::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// create debug drawer
	pDebugDrawer = engine.GetDebugDrawerManager()->CreateDebugDrawer();
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
	
	pLight = engine.GetLightManager()->CreateLight();
	pLight->SetType( pOCLight->GetType() );
	pLight->SetHintMovement( pOCLight->GetHintMovement() );
	pLight->SetHintParameter( pOCLight->GetHintParameter() );
	pLight->SetColor( pOCLight->GetColor() );
	pLight->SetIntensity( pOCLight->GetIntensity() );
	pLight->SetRange( pOCLight->GetRange() );
	pLight->SetAmbientRatio( pOCLight->GetAmbientRatio() );
	pLight->SetHalfIntensityDistance( pOCLight->GetHalfIntensityDistance() );
	pLight->SetSpotAngle( pOCLight->GetSpotAngle() * DEG2RAD );
	pLight->SetSpotRatio( pOCLight->GetSpotRatio() );
	pLight->SetSpotSmoothness( pOCLight->GetSpotSmoothness() );
	pLight->SetSpotExponent( pOCLight->GetSpotExponent() );
	pLight->SetCastShadows( pOCLight->GetCastShadows() );
	pLight->SetHintLightImportance( pOCLight->GetHintLightImportance() );
	pLight->SetHintShadowImportance( pOCLight->GetHintShadowImportance() );
	pLight->SetPosition( pOCLight->GetPosition() );
	pLight->SetOrientation( decQuaternion::CreateFromEuler( pOCLight->GetRotation() * DEG2RAD ) );
	
	// light skin
	deSkinReference skin;
	if( ! pOCLight->GetLightSkinPath().IsEmpty() ){
		try{
			skin.TakeOver( engine.GetSkinManager()->LoadSkin( vfs, pOCLight->GetLightSkinPath(), "/" ) );
			
		}catch( const deException &e ){
			skin = environment.GetErrorSkin();
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
	deWorld &world = *pView.GetGameDefinition()->GetWorld();
	
	if( pLight ){
		world.RemoveLight( pLight );
		pLight->FreeReference();
		pLight = NULL;
	}
}
