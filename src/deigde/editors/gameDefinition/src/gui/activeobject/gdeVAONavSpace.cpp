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

#include "gdeVAONavSpace.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/loadsave/igdeLoadSaveNavSpace.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAONavSpace
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAONavSpace::gdeVAONavSpace( gdeViewActiveObject &view, gdeOCNavigationSpace *ocnavspace ) :
pView( view ),
pOCNavSpace( ocnavspace ),
pDDSpace( NULL ),
pDDBlocker( NULL ),
pDDSSpace( NULL ),
pDDSBlocker( NULL )
{
	if( ! ocnavspace ){
		DETHROW( deeInvalidParam );
	}
	ocnavspace->AddReference();
	
	try{
		pCreateDebugDrawer();
		pBuildDDSSpace();
		pBuildDDSBlocker();
		pUpdateDDShapeColor();
		UpdateDDVisibility();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeVAONavSpace::~gdeVAONavSpace(){
	pCleanUp();
}



// Management
///////////////

void gdeVAONavSpace::RebuildResources(){
	pBuildDDSSpace();
	pBuildDDSBlocker();
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAONavSpace::SelectedObjectChanged(){
	pUpdateDDShapeColor();
	UpdateDDVisibility();
}

void gdeVAONavSpace::UpdateDDVisibility(){
	const gdeGameDefinition &gameDefinition = *pView.GetGameDefinition();
	
	pDDBlocker->SetVisible( pView.GetShowNavBlockers()
		|| ( gameDefinition.GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationSpace
			&& gameDefinition.GetActiveOCNavigationSpace() == pOCNavSpace ) );
}



// Private functions
//////////////////////

void gdeVAONavSpace::pCleanUp(){
	if( pDDSSpace ){
		pDDSSpace->SetParentDebugDrawer( NULL );
		delete pDDSSpace;
	}
	if( pDDSBlocker ){
		pDDSBlocker->SetParentDebugDrawer( NULL );
		delete pDDSBlocker;
	}
	if( pDDSpace ){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer( pDDSpace );
		pDDSpace->FreeReference();
	}
	if( pDDBlocker ){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer( pDDBlocker );
		pDDBlocker->FreeReference();
	}
	
	if( pOCNavSpace ){
		pOCNavSpace->FreeReference();
	}
}



void gdeVAONavSpace::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// debug drawer
	pDDSpace = engine.GetDebugDrawerManager()->CreateDebugDrawer();
	pDDSpace->SetXRay( true );
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer( pDDSpace );
	
	pDDBlocker = engine.GetDebugDrawerManager()->CreateDebugDrawer();
	pDDBlocker->SetXRay( false );
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer( pDDBlocker );
	
	// space
	pDDSSpace = new igdeWDebugDrawerShape;
	pDDSSpace->SetParentDebugDrawer( pDDSpace );
	
	// blocker
	pDDSBlocker = new igdeWDebugDrawerShape;
	pDDSBlocker->SetParentDebugDrawer( pDDBlocker );
}

void gdeVAONavSpace::pBuildDDSSpace(){
	pDDSSpace->RemoveAllFaces();
	
	if( pOCNavSpace->GetPath().IsEmpty() ){
		return;
	}
	
	// load navigation space
	deVirtualFileSystem * const vfs = pView.GetGameDefinition()->GetPreviewVFS();
	const decPath path( decPath::CreatePathUnix( pOCNavSpace->GetPath() ) );
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	igdeLoadSaveNavSpace loader( &environment, "gdeVAONavSpace" );
	deNavigationSpace *navspace = NULL;
	decBaseFileReader *reader = NULL;
	
	try{
		navspace = engine.GetNavigationSpaceManager()->CreateNavigationSpace();
		reader = vfs->OpenFileForReading( path );
		loader.Load( *navspace, *reader );
		reader->FreeReference();
		reader = NULL;
		
	}catch( const deException &e ){
		if( navspace ){
			navspace->FreeReference();
			navspace = NULL;
		}
		environment.GetLogger()->LogException( LOGSOURCE, e );
		return;
	}
	
	// create debug drawer shape
	try{
		pDDSSpace->SetPosition( pOCNavSpace->GetPosition() );
		pDDSSpace->SetOrientation( decQuaternion::CreateFromEuler(
			pOCNavSpace->GetRotation() * DEG2RAD ) );
		pDDSSpace->AddNavSpaceFaces( *navspace );
		
	}catch( const deException &e ){
		if( navspace ){
			navspace->FreeReference();
			navspace = NULL;
		}
		throw;
	}
	
	navspace->FreeReference();
	navspace = NULL;
}

void gdeVAONavSpace::pBuildDDSBlocker(){
	pDDSBlocker->RemoveAllShapes();
	
	const decShapeList &blockerShape = pOCNavSpace->GetBlockerShapeList();
	if( blockerShape.GetCount() == 0 ){
		return;
	}
	
	pDDSBlocker->SetPosition( pOCNavSpace->GetPosition() );
	pDDSBlocker->SetOrientation( decQuaternion::CreateFromEuler(
		pOCNavSpace->GetRotation() * DEG2RAD ) );
	pDDSBlocker->AddShapes( blockerShape );
}

void gdeVAONavSpace::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if( pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationSpace
	&& pView.GetGameDefinition()->GetActiveOCNavigationSpace() == pOCNavSpace ){
		pDDSSpace->SetEdgeColor( decColor( config.GetColorNavigationSpaceActive(), 1.0f ) );
		pDDSSpace->SetFillColor( config.GetColorNavigationSpaceActive() );
		
		pDDSBlocker->SetEdgeColor( decColor( config.GetColorNavigationBlockerActive(), 1.0f ) );
		pDDSBlocker->SetFillColor( config.GetColorNavigationBlockerActive() );
		
	}else{
		pDDSSpace->SetEdgeColor( decColor( config.GetColorNavigationSpace(), 1.0f ) );
		pDDSSpace->SetFillColor( config.GetColorNavigationSpace() );
		
		pDDSBlocker->SetEdgeColor( decColor( config.GetColorNavigationBlocker(), 1.0f ) );
		pDDSBlocker->SetFillColor( config.GetColorNavigationBlocker() );
	}
}
