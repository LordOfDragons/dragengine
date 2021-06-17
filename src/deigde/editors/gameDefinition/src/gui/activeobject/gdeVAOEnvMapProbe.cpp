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

#include "gdeVAOEnvMapProbe.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/probe/deEnvMapProbeManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOEnvMapProbe
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOEnvMapProbe::gdeVAOEnvMapProbe( gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix, gdeOCEnvMapProbe *ocenvMapProbe ) :
gdeVAOSubObject( view, objectClass, propertyPrefix ),
pOCEnvMapProbe( ocenvMapProbe ),
pDDSCenter( NULL ),
pDDSReflection( NULL ),
pDDSInfluence( NULL ),
pDDSMask( NULL )
{
	if( ! ocenvMapProbe ){
		DETHROW( deeInvalidParam );
	}
	
	pOCEnvMapProbe->AddReference();
	
	try{
		pCreateDebugDrawer();
		pCreateEnvMapProbe();
		pUpdateDDShapes();
		pUpdateDDShapeColor();
		UpdateDDVisibility();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeVAOEnvMapProbe::~gdeVAOEnvMapProbe(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOEnvMapProbe::RebuildResources(){
	pReleaseResources();
	
	pCreateEnvMapProbe();
	pUpdateDDShapes();
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAOEnvMapProbe::SelectedObjectChanged(){
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAOEnvMapProbe::UpdateDDVisibility(){
	const gdeGameDefinition &gameDefinition = *pView.GetGameDefinition();
	
	pDebugDrawer->SetVisible( pView.GetShowEnvMapProbes()
		|| ( gameDefinition.GetSelectedObjectType() == gdeGameDefinition::eotOCEnvMapProbe
			&& gameDefinition.GetActiveOCEnvMapProbe() == pOCEnvMapProbe ) );
}



// Private functions
//////////////////////

void gdeVAOEnvMapProbe::pCleanUp(){
	pReleaseResources();
	
	if( pDDSMask ){
		delete pDDSMask;
	}
	if( pDDSReflection ){
		delete pDDSReflection;
	}
	if( pDDSInfluence ){
		delete pDDSInfluence;
	}
	if( pDDSCenter ){
		delete pDDSCenter;
	}
	if( pDebugDrawer ){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer( pDebugDrawer );
		pDebugDrawer = NULL;
	}
	
	if( pOCEnvMapProbe ){
		pOCEnvMapProbe->FreeReference();
	}
}



void gdeVAOEnvMapProbe::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// create debug drawer
	pDebugDrawer.TakeOver( engine.GetDebugDrawerManager()->CreateDebugDrawer() );
	pDebugDrawer->SetXRay( true );
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer( pDebugDrawer );
	
	// create center shape
	pDDSCenter = new igdeWDebugDrawerShape;
	pDDSCenter->AddSphereShape( 0.05f, decVector() );
	pDDSCenter->SetParentDebugDrawer( pDebugDrawer );
	
	// create reflection shape
	pDDSReflection = new igdeWDebugDrawerShape;
	pDDSReflection->SetParentDebugDrawer( pDebugDrawer );
	
	// create influence shape
	pDDSInfluence = new igdeWDebugDrawerShape;
	pDDSInfluence->SetParentDebugDrawer( pDebugDrawer );
	
	// create mask shape
	pDDSMask = new igdeWDebugDrawerShape;
	pDDSMask->SetParentDebugDrawer( pDebugDrawer );
}

void gdeVAOEnvMapProbe::pCreateEnvMapProbe(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	pEnvMapProbe.TakeOver( engine.GetEnvMapProbeManager()->CreateEnvMapProbe() );
	pEnvMapProbe->SetPosition( pOCEnvMapProbe->GetPosition() );
	pEnvMapProbe->SetOrientation( decQuaternion::CreateFromEuler(
		pOCEnvMapProbe->GetRotation() * DEG2RAD ) );
	pEnvMapProbe->SetScaling( pOCEnvMapProbe->GetScaling() );
	
	if( pOCEnvMapProbe->GetShapeReflection() ){
		pEnvMapProbe->SetShapeReflection( pOCEnvMapProbe->GetShapeReflection()->Copy() );
	}
	
	pEnvMapProbe->GetShapeListInfluence() = pOCEnvMapProbe->GetShapeListInfluence();
	pEnvMapProbe->NotifyShapeListInfluenceChanged();
	
	pEnvMapProbe->GetShapeListReflectionMask() = pOCEnvMapProbe->GetShapeListReflectionMask();
	pEnvMapProbe->NotifyShapeReflectionChanged();
	
	pEnvMapProbe->SetInfluenceBorderSize( pOCEnvMapProbe->GetInfluenceBorderSize() );
	pEnvMapProbe->SetInfluencePriority( pOCEnvMapProbe->GetInfluencePriority() );
	
	pView.GetGameDefinition()->GetWorld()->AddEnvMapProbe( pEnvMapProbe );
}

void gdeVAOEnvMapProbe::pUpdateDDShapes(){
	const decVector &position = pOCEnvMapProbe->GetPosition();
	const decQuaternion orientation( decQuaternion::CreateFromEuler(
		pOCEnvMapProbe->GetRotation() * DEG2RAD ) );
	
	// center
	pDDSCenter->SetPosition( position );
	pDDSCenter->SetOrientation( orientation );
	
	// reflection
	pDDSReflection->SetPosition( position );
	pDDSReflection->SetOrientation( orientation );
	
	pDDSReflection->RemoveAllShapes();
	if( pOCEnvMapProbe->GetShapeReflection() ){
		pDDSReflection->AddShape( pOCEnvMapProbe->GetShapeReflection()->Copy() );
	}
	
	// influence
	pDDSInfluence->SetPosition( position );
	pDDSInfluence->SetOrientation( orientation );
	
	pDDSInfluence->RemoveAllShapes();
	pDDSInfluence->AddShapes( pOCEnvMapProbe->GetShapeListInfluence() );
	
	// mask
	pDDSMask->SetPosition( position );
	pDDSMask->SetOrientation( orientation );
	
	pDDSMask->RemoveAllShapes();
	pDDSMask->AddShapes( pOCEnvMapProbe->GetShapeListReflectionMask() );
}

void gdeVAOEnvMapProbe::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if( pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCEnvMapProbe
	&& pView.GetGameDefinition()->GetActiveOCEnvMapProbe() == pOCEnvMapProbe ){
		pDDSCenter->SetEdgeColor( decColor( config.GetColorEnvMapProbeActive(), 1.0f ) );
		pDDSCenter->SetFillColor( config.GetColorEnvMapProbeActive() );
		
		pDDSReflection->SetEdgeColor( decColor( config.GetColorEnvMapProbeActiveReflection(), 1.0f ) );
		pDDSReflection->SetFillColor( config.GetColorEnvMapProbeActiveReflection() );
		
		pDDSInfluence->SetEdgeColor( decColor( config.GetColorEnvMapProbeActiveInfluence(), 1.0f ) );
		pDDSInfluence->SetFillColor( config.GetColorEnvMapProbeActiveInfluence() );
		
		pDDSMask->SetEdgeColor( decColor( config.GetColorEnvMapProbeActiveMask(), 1.0f ) );
		pDDSMask->SetFillColor( config.GetColorEnvMapProbeActiveMask() );
		
	}else{
		pDDSCenter->SetEdgeColor( decColor( config.GetColorEnvMapProbe(), 0.25f ) );
		pDDSCenter->SetFillColor( config.GetColorEnvMapProbe() );
		
		pDDSReflection->SetEdgeColor( decColor( config.GetColorEnvMapProbeReflection(), 0.25f ) );
		pDDSReflection->SetFillColor( config.GetColorEnvMapProbeReflection() );
		
		pDDSInfluence->SetEdgeColor( decColor( config.GetColorEnvMapProbeInfluence(), 0.25f ) );
		pDDSInfluence->SetFillColor( config.GetColorEnvMapProbeInfluence() );
		
		pDDSMask->SetEdgeColor( decColor( config.GetColorEnvMapProbeMask(), 0.25f ) );
		pDDSMask->SetFillColor( config.GetColorEnvMapProbeMask() );
	}
}



void gdeVAOEnvMapProbe::pReleaseResources(){
	deWorld &world = *pView.GetGameDefinition()->GetWorld();
	
	if( pEnvMapProbe ){
		world.RemoveEnvMapProbe( pEnvMapProbe );
		pEnvMapProbe = NULL;
	}
	
	pDDSCenter->RemoveAllShapes();
	pDDSReflection->RemoveAllShapes();
	pDDSInfluence->RemoveAllShapes();
	pDDSMask->RemoveAllShapes();
}
