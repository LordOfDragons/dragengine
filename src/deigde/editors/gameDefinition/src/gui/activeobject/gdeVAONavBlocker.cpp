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

#include "gdeVAONavBlocker.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAONavBlocker
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAONavBlocker::gdeVAONavBlocker( gdeViewActiveObject &view,
gdeOCNavigationBlocker *ocnavblocker ) :
pView( view ),
pOCNavBlocker( ocnavblocker ),
pDebugDrawer( NULL ),
pDDSBlocker( NULL )
{
	if( ! ocnavblocker ){
		DETHROW( deeInvalidParam );
	}
	ocnavblocker->AddReference();
	
	try{
		pCreateDebugDrawer();
		pBuildDDSBlocker();
		pUpdateDDShapeColor();
		UpdateDDVisibility();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeVAONavBlocker::~gdeVAONavBlocker(){
	pCleanUp();
}



// Management
///////////////

void gdeVAONavBlocker::RebuildResources(){
	pBuildDDSBlocker();
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAONavBlocker::SelectedObjectChanged(){
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAONavBlocker::UpdateDDVisibility(){
	const gdeGameDefinition &gameDefinition = *pView.GetGameDefinition();
	
	pDebugDrawer->SetVisible( pView.GetShowNavBlockers()
		|| ( gameDefinition.GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationBlocker
			&& gameDefinition.GetActiveOCNavigationBlocker() == pOCNavBlocker ) );
}



// Private functions
//////////////////////

void gdeVAONavBlocker::pCleanUp(){
	if( pDDSBlocker ){
		pDDSBlocker->SetParentDebugDrawer( NULL );
		delete pDDSBlocker;
	}
	if( pDebugDrawer ){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer( pDebugDrawer );
		pDebugDrawer->FreeReference();
	}
	
	if( pOCNavBlocker ){
		pOCNavBlocker->FreeReference();
	}
}



void gdeVAONavBlocker::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// debug drawer
	pDebugDrawer = engine.GetDebugDrawerManager()->CreateDebugDrawer();
	pDebugDrawer->SetXRay( false );
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer( pDebugDrawer );
	
	// blocker
	pDDSBlocker = new igdeWDebugDrawerShape;
	pDDSBlocker->SetParentDebugDrawer( pDebugDrawer );
}

void gdeVAONavBlocker::pBuildDDSBlocker(){
	pDDSBlocker->RemoveAllShapes();
	
	const decShapeList &blockerShape = pOCNavBlocker->GetShapeList();
	if( blockerShape.GetCount() == 0 ){
		return;
	}
	
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	pDDSBlocker->SetEdgeColor( decColor( config.GetColorNavigationBlocker(), 1.0f ) );
	pDDSBlocker->SetFillColor( config.GetColorNavigationBlocker() );
	pDDSBlocker->SetPosition( pOCNavBlocker->GetPosition() );
	pDDSBlocker->SetOrientation( decQuaternion::CreateFromEuler(
		pOCNavBlocker->GetRotation() * DEG2RAD ) );
	pDDSBlocker->AddShapes( blockerShape );
}

void gdeVAONavBlocker::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if( pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationBlocker
	&& pView.GetGameDefinition()->GetActiveOCNavigationBlocker() == pOCNavBlocker ){
		pDDSBlocker->SetEdgeColor( decColor( config.GetColorNavigationBlockerActive(), 1.0f ) );
		pDDSBlocker->SetFillColor( config.GetColorNavigationBlockerActive() );
		
	}else{
		pDDSBlocker->SetEdgeColor( decColor( config.GetColorNavigationBlocker(), 1.0f ) );
		pDDSBlocker->SetFillColor( config.GetColorNavigationBlocker() );
	}
}
