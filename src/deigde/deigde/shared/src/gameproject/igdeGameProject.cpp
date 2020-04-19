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

#include "igdeGameProject.h"
#include "../environment/igdeEnvironment.h"
#include "../gamedefinition/igdeGameDefinition.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"



// Class igdeGameProject
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeGameProject::igdeGameProject( igdeEnvironment &environment ) :
pEnvironment( environment ),
pChanged( false ),

pName( "New Project" ),
pPathData( "data" ),
pPathCache( "cache" ),
pPathLocal( "local" ),

pProjectGameDefinition( NULL ),
pXMLEClassGameDefinition( NULL ),
pFoundGameDefinition( NULL ),
pGameDefinition( NULL )
{
	pGameDefinition = new igdeGameDefinition( environment );
	pXMLEClassGameDefinition = new igdeGameDefinition( environment );
	pFoundGameDefinition = new igdeGameDefinition( environment );
}

igdeGameProject::~igdeGameProject(){
	pCleanUp();
}



// Management
///////////////

void igdeGameProject::SetFilePath( const char *path ){
	if( pPathFile == path ){
		return;
	}
	
	decPath pathDirectory;
	
	pPathFile = path;
	
	pathDirectory.SetFromNative( path );
	if( pathDirectory.GetComponentCount() > 1 ){
		pathDirectory.RemoveLastComponent();
		
	}else{
		pathDirectory.SetFromUnix( "/" );
	}
	
	pPathDirectory = pathDirectory.GetPathNative();
}

void igdeGameProject::SetName( const char *name ){
	pName = name;
}

void igdeGameProject::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGameProject::SetPathData( const char *path ){
	pPathData = path;
}

void igdeGameProject::SetPathCache( const char *path ){
	pPathCache = path;
}

void igdeGameProject::SetPathLocal( const char *path ){
	pPathLocal = path;
}



void igdeGameProject::SetPathProjectGameDefinition( const char *path ){
	pPathProjectGameDefinition = path;
}

void igdeGameProject::SetProjectGameDefinition( igdeGameDefinition *gameDefinition ){
	if( pProjectGameDefinition == gameDefinition ){
		return;
	}
	
	if( pProjectGameDefinition ){
		pProjectGameDefinition->FreeReference();
	}
	
	pProjectGameDefinition = gameDefinition;
	
	if( gameDefinition ){
		gameDefinition->AddReference();
	}
}



void igdeGameProject::MergeGameDefinitions(){
	if( ! pProjectGameDefinition ){
		DETHROW( deeInvalidParam );
	}
	
	decTimer timer;
	
	const int baseGameDefCount = pBaseGameDefinitionList.GetCount();
	igdeGameDefinition *merged = NULL;
	int i;
	
	try{
		// merge game definition
		merged = new igdeGameDefinition( pEnvironment );
		
		merged->UpdateWith( *pEnvironment.GetGameDefinition() );
		for( i=0; i<baseGameDefCount; i++ ){
			merged->UpdateWith( *pBaseGameDefinitionList.GetAt( i ) );
		}
		merged->UpdateWith( *pProjectGameDefinition );
		
		merged->SetFilename( pProjectGameDefinition->GetFilename() );
		merged->SetID( pProjectGameDefinition->GetID() );
		merged->SetDescription( pProjectGameDefinition->GetDescription() );
		merged->SetBasePath( pProjectGameDefinition->GetBasePath() );
		
		merged->ResolveInheritClasses();
		merged->UpdateWithElementClasses( *pXMLEClassGameDefinition );
		merged->UpdateWithFound( *pFoundGameDefinition );
		
		merged->UpdateTags();
		
		// replace game definition
		if( pGameDefinition ){
			pGameDefinition->FreeReference();
		}
		pGameDefinition = merged;
		
	}catch( const deException & ){
		if( merged ){
			merged->FreeReference();
		}
		throw;
	}
	
	pEnvironment.GetLogger()->LogInfoFormat( LOGSOURCE,
		"Merged game definition in %.1fs", timer.GetElapsedTime() );
}



void igdeGameProject::SetScriptModule( const char* moduleName ){
	pScriptModule = moduleName;
}

void igdeGameProject::SetScriptModuleVersion( const char *version ){
	pScriptModuleVersion = version;
}



void igdeGameProject::SetChanged( bool changed ){
	if( changed == pChanged ){
		return;
	}
	
	pChanged = changed;
	NotifyStateChanged();
}



void igdeGameProject::NotifyStateChanged(){
}

void igdeGameProject::NotifyUndoChanged(){
}



// Private Functions
//////////////////////

void igdeGameProject::pCleanUp(){
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
	if( pFoundGameDefinition ){
		pFoundGameDefinition->FreeReference();
	}
	if( pXMLEClassGameDefinition ){
		pXMLEClassGameDefinition->FreeReference();
	}
	if( pProjectGameDefinition ){
		pProjectGameDefinition->FreeReference();
	}
	pBaseGameDefinitionList.RemoveAll();
}
