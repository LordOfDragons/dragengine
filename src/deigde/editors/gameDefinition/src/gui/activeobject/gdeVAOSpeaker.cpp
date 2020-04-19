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

#include "gdeVAOSpeaker.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/speaker/gdeOCSpeaker.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOSpeaker
//////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOSpeaker::gdeVAOSpeaker( gdeViewActiveObject &view, gdeOCSpeaker *ocspeaker ) :
pView( view ),
pOCSpeaker( ocspeaker ),
pDebugDrawer( NULL ),
pDDSCenter( NULL ),
pDDSCoordSystem( NULL )
{
	if( ! ocspeaker ){
		DETHROW( deeInvalidParam );
	}
	
	pOCSpeaker->AddReference();
	
	try{
		pCreateDebugDrawer();
		pUpdateDDShapes();
		pUpdateDDShapeColor();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeVAOSpeaker::~gdeVAOSpeaker(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOSpeaker::RebuildResources(){
	pReleaseResources();
	
	pUpdateDDShapes();
	pUpdateDDShapeColor();
}

void gdeVAOSpeaker::SelectedObjectChanged(){
	pUpdateDDShapeColor();
}



// Private functions
//////////////////////

void gdeVAOSpeaker::pCleanUp(){
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
	
	if( pOCSpeaker ){
		pOCSpeaker->FreeReference();
	}
}



void gdeVAOSpeaker::pCreateDebugDrawer(){
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

void gdeVAOSpeaker::pUpdateDDShapes(){
	const decVector &position = pOCSpeaker->GetPosition();
	const decQuaternion orientation( decQuaternion::CreateFromEuler(
		pOCSpeaker->GetRotation() * DEG2RAD ) );
	
	pDDSCenter->SetPosition( position );
	pDDSCenter->SetOrientation( orientation );
	
	pDDSCoordSystem->SetPosition( position );
	pDDSCoordSystem->SetOrientation( orientation );
}

void gdeVAOSpeaker::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if( pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCSpeaker
	&& pView.GetGameDefinition()->GetActiveOCSpeaker() == pOCSpeaker ){
		pDDSCenter->SetEdgeColor( decColor( config.GetColorSpeakerActive(), 1.0f ) );
		pDDSCenter->SetFillColor( config.GetColorSpeakerActive() );
		
	}else{
		pDDSCenter->SetEdgeColor( decColor( config.GetColorSpeaker(), 0.25f ) );
		pDDSCenter->SetFillColor( config.GetColorSpeaker() );
	}
}



void gdeVAOSpeaker::pReleaseResources(){
}
