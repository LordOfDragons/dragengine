/* 
 * Drag[en]gine IGDE
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeTaskSyncGameDefinition.h"
#include "igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../module/igdeEditorModuleManager.h"
#include "../module/igdeEditorModuleDefinition.h"

#include <deigde/module/igdeEditorModule.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/igdeXMLGameDefinition.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decPath.h>



// Class igdeTaskSyncGameDefinition
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTaskSyncGameDefinition::igdeTaskSyncGameDefinition( igdeWindowMain &windowMain ) :
pWindowMain( windowMain ),
pOldProjectGameDef( NULL ),
pOldGameDef( NULL ),
pState( esReloadProjectGameDef ),
pTaskIndex( 0 ),
pLastProgress( -1.0f ),
pBaseProgress( 0.0f ),
pProgressFactor( 0.0f ),
pReloadXMLElementClasses( false )
{
	try{
		SetMessage( "Reloading Project Game Definition" );
		
		windowMain.SetProgressVisible( true );
		windowMain.SetProgress( 0.0f );
		windowMain.SetProgressText( GetMessage() );
		
		// NOTE looks like a bug in FOX but disable() seems to have no effect. No idea how
		//      to solve this problem
		windowMain.SetEnabled( false );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeTaskSyncGameDefinition::~igdeTaskSyncGameDefinition(){
	pWindowMain.SetEnabled( true );
	
	pWindowMain.SetProgressVisible( false);
	pWindowMain.SetProgress( 0.0f );
	pWindowMain.SetProgressText( "" );
	
	pCleanUp();
}



// Management
///////////////

void igdeTaskSyncGameDefinition::SetReloadXMLElementClasses( bool reloadXMLElementClasses ){
	pReloadXMLElementClasses = reloadXMLElementClasses;
}

bool igdeTaskSyncGameDefinition::Step(){
	switch( pState ){
	case esReloadProjectGameDef:{
		igdeGameProject &project = *pWindowMain.GetGameProject();
		
		pOldProjectGameDef = project.GetProjectGameDefinition();
		pOldProjectGameDef->AddReference();
		
		pOldGameDef = project.GetGameDefinition();
		pOldGameDef->AddReference();
		
		if( ! pReloadXMLElementClasses ){
			pLoadProjectGameDefinition();
		}
		
		project.MergeGameDefinitions();
		project.GetGameDefinition()->UpdateEngineObjects();
		
		pCreateEditorTasks();
		
		if( pEditorTasks.GetCount() == 0 ){
			pState = esFinished;
			return false;
		}
		
		pProgressFactor = 1.0f / ( float )( pEditorTasks.GetCount() + 1 );
		pBaseProgress = pProgressFactor;
		pUpdateProgress( true );
		
		pState = esEditorUpdateTasks;
		break;
		}
		
	case esEditorUpdateTasks:{
		// step 2..2+N: process module update tasks
		const int taskCount = pEditorTasks.GetCount();
		if( pTaskIndex == taskCount ){
			pState = esFinished;
			return false;
		}
		
		igdeStepableTask &activeTask = *( ( igdeStepableTask* )pEditorTasks.GetAt( pTaskIndex ) );
		if( activeTask.Step() ){
			pUpdateProgress( false );
			break;
		}
		
		delete ( igdeStepableTask* )pEditorTasks.GetAt( pTaskIndex );
		pEditorTasks.SetAt( pTaskIndex, NULL );
		
		pTaskIndex++;
		if( pTaskIndex == taskCount ){
			pState = esFinished;
			return false;
		}
		
		pBaseProgress = ( float )( pTaskIndex + 1 ) / ( float )( taskCount + 1 );
		pUpdateProgress( true );
		break;
		}
		
	case esFinished:
	default:
		return false;
	}
	
	pWindowMain.SetProgress( GetProgress() );
	pWindowMain.SetProgressText( GetMessage() );
	
	return true;
}



// Private Functions
//////////////////////

void igdeTaskSyncGameDefinition::pCleanUp(){
	const int taskCount = pEditorTasks.GetCount();
	
	while( pTaskIndex<taskCount ){
		delete ( igdeStepableTask* )pEditorTasks.GetAt( pTaskIndex++ );
	}
	
	if( pOldGameDef ){
		pOldGameDef->FreeReference();
	}
	if( pOldProjectGameDef ){
		pOldProjectGameDef->FreeReference();
	}
}

void igdeTaskSyncGameDefinition::pUpdateProgress( bool force ){
	const igdeStepableTask &task = *( ( const igdeStepableTask* )pEditorTasks.GetAt( pTaskIndex ) );
	
	if( force || fabsf( task.GetProgress() - pLastProgress ) > 0.1f ){
		pLastProgress = task.GetProgress();
		SetProgress( pBaseProgress + pProgressFactor * pLastProgress );
	}
	
	if( force || task.GetMessage() != pLastMessage ){
		pLastMessage = task.GetMessage();
		SetMessage( pLastMessage );
	}
}

void igdeTaskSyncGameDefinition::pLoadProjectGameDefinition(){
	igdeGameProject &project = *pWindowMain.GetGameProject();
	igdeGameDefinition *gamedef = NULL;
	decDiskFileReader *reader = NULL;
	decPath path;
	
	try{
		path.SetFromNative( project.GetDirectoryPath() );
		path.AddUnixPath( project.GetPathProjectGameDefinition() );
		reader = new decDiskFileReader( path.GetPathNative() );
		
		gamedef = new igdeGameDefinition( pWindowMain.GetEnvironment() );
		gamedef->SetFilename( path.GetPathNative() );
		
		igdeXMLGameDefinition( pWindowMain.GetEnvironment(), pWindowMain.GetLogger() ).Load( *reader, *gamedef );
		
		project.SetProjectGameDefinition( gamedef );
		
		reader->FreeReference();
		gamedef->FreeReference();
		
	}catch( const deException & ){
		if( reader ){
			reader->FreeReference();
		}
		if( gamedef ){
			gamedef->FreeReference();
		}
		throw;
	}
}

void igdeTaskSyncGameDefinition::pCreateEditorTasks(){
	const igdeEditorModuleManager &moduleManager = pWindowMain.GetModuleManager();
	const int editorCount = moduleManager.GetModuleCount();
	igdeStepableTask *editorTask = NULL;
	int i;
	
	try{
		for( i=0; i<editorCount; i++ ){
			const igdeEditorModuleDefinition &editorDefinition = *moduleManager.GetModuleAt( i );
			if( ! editorDefinition.IsModuleRunning() ){
				continue;
			}
			
			igdeEditorModule * const editor = editorDefinition.GetModule();
			if( ! editor ){
				continue;
			}
			
			editorTask = editor->OnGameDefinitionChanged();
			if( ! editorTask ){
				continue;
			}
			
			pEditorTasks.Add( editorTask );
			editorTask = NULL;
		}
		
	}catch( const deException & ){
		if( editorTask ){
			delete editorTask;
		}
		throw;
	}
}
