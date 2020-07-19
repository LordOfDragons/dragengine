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

#include "igdeLSGameProject.h"
#include "igdeLoadSaveSystem.h"
#include "../gui/igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../../shared/src/gameproject/igdeXMLGameProject.h"

#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gamedefinition/igdeXMLGameDefinition.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"



// Class igdeLSGameProject
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLSGameProject::igdeLSGameProject( igdeLoadSaveSystem *lssys ){
	if( ! lssys ){
		DETHROW( deeInvalidParam );
	}
	
	pLoadSaveSystem = lssys;
	
	pName = "Drag[en]gine Game Project";
	pPattern = "*.degp";
	pDefaultExtension = ".degp";
}

igdeLSGameProject::~igdeLSGameProject(){
}



// Management
///////////////

void igdeLSGameProject::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
}

void igdeLSGameProject::SetPattern( const char *pattern ){
	if( ! pattern ){
		DETHROW( deeInvalidParam );
	}
	
	pPattern = pattern;
}

void igdeLSGameProject::SetDefaultExtension( const char *extension ){
	if( ! extension ){
		DETHROW( deeInvalidParam );
	}
	
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLSGameProject::Load( const char *filename, igdeGameProject *project, decBaseFileReader *file ){
	if( ! project || ! file ){
		DETHROW( deeInvalidParam );
	}
	
	igdeWindowMain &windowMain = *pLoadSaveSystem->GetWindowMain();
	igdeEnvironment &environment = windowMain.GetEnvironment();
	deLogger * const logger = windowMain.GetLogger();
	
	// load game project file
	igdeXMLGameProject xmlGameProject( logger );
	
	xmlGameProject.Load( *file, *project );
	project->SetFilePath( filename );
	
	// locate base game definitions
	const igdeGameDefinitionList &sharedGameDefs = windowMain.GetSharedGameDefinitions();
	const decStringList &baseGameDefs = project->GetBaseGameDefinitionIDList();
	const int baseGameDefCount = baseGameDefs.GetCount();
	int i;
	
	for( i=0; i<baseGameDefCount; i++ ){
		const decString &id = baseGameDefs.GetAt( i );
		igdeGameDefinition * const baseGameDef = sharedGameDefs.GetWithID( id );
		if( baseGameDef ){
			project->GetBaseGameDefinitionList().Add( baseGameDef );
			
		}else{
			logger->LogWarnFormat( LOGSOURCE, "Can not find base game definition '%s', skipped",
				id.GetString() );
		}
	}
	
	// load the project game definition specified
	decDiskFileReader *gameDefReader = NULL;
	igdeGameDefinition *gameDef = NULL;
	decPath path;
	
	try{
		path.SetFromNative( project->GetDirectoryPath() );
		path.AddUnixPath( project->GetPathProjectGameDefinition() );
		gameDefReader = new decDiskFileReader( path.GetPathNative() );
		
		gameDef = new igdeGameDefinition( environment );
		gameDef->SetFilename( path.GetPathNative() );
		
		igdeXMLGameDefinition( environment, logger ).Load( *gameDefReader, *gameDef );
		
		project->SetProjectGameDefinition( gameDef );
		
		gameDefReader->FreeReference();
		gameDefReader = NULL;
		
		gameDef->FreeReference();
		gameDef = NULL;
		
	}catch( const deException & ){
		if( gameDefReader ){
			gameDefReader->FreeReference();
		}
		if( gameDef ){
			gameDef->FreeReference();
		}
		throw;
	}
}

void igdeLSGameProject::Save( igdeGameProject *project, decBaseFileWriter *file ){
	if( ! project || ! file ){
		DETHROW( deeInvalidParam );
	}
	
	igdeXMLGameProject xmlGameProject( pLoadSaveSystem->GetWindowMain()->GetLogger() );
	
	xmlGameProject.Save( *file, *project );
}
