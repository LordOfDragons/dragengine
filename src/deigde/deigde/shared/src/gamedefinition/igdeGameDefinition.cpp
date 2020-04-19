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

#include "igdeGameDefinition.h"
#include "igdeGDCategory.h"
#include "igdeXMLElementClass.h"
#include "class/igdeGDClass.h"
#include "class/igdeGDClassManager.h"
#include "skin/igdeGDSkinManager.h"
#include "sky/igdeGDSkyManager.h"
#include "../environment/igdeEnvironment.h"
#include "../engine/igdeEngineController.h"
#include "../gui/filedialog/igdeFilePattern.h"
#include "../gui/filedialog/igdeFilePatternList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>



// Class igdeGameDefinition
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGameDefinition::igdeGameDefinition( igdeEnvironment &environment ) :
pEnvironment( environment ),

pVFSPath( "/" ),

pSkinManager( NULL ),
pClassManager( NULL ),
pSkyManager( NULL ),

pDefaultModel( NULL ),
pDefaultSkin( NULL )
{
	try{
		pSkinManager = new igdeGDSkinManager;
		pClassManager = new igdeGDClassManager;
		pSkyManager = new igdeGDSkyManager;
		
	}catch( const deException &e ){
		pCleanUp();
		throw;
	}
}

igdeGameDefinition::~igdeGameDefinition(){
	pCleanUp();
}



// Management
///////////////

void igdeGameDefinition::SetFilename( const char *filename ){
	pFilename = filename;
}

void igdeGameDefinition::SetID( const char *id ){
	pID = id;
}

void igdeGameDefinition::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGameDefinition::SetBasePath( const char *path ){
	pBasePath = path;
}

void igdeGameDefinition::SetVFSPath( const char *path ){
	pVFSPath = path;
}

void igdeGameDefinition::SetScriptModule( const char *identifier ){
	pScriptModule = identifier;
}



void igdeGameDefinition::UpdateTags(){
	pClassManager->UpdateTags();
}

void igdeGameDefinition::ResolveInheritClasses(){
	pClassManager->ResolveInheritClasses();
}



void igdeGameDefinition::UpdateWith( const igdeGameDefinition &gameDefinition ){
	pFilename = gameDefinition.pFilename;
	pID = gameDefinition.pID;
	pDescription = gameDefinition.pDescription;
	pBasePath = gameDefinition.pBasePath;
	pVFSPath = gameDefinition.pVFSPath;
	
	pSkinManager->UpdateWith( *gameDefinition.pSkinManager );
	pClassManager->UpdateWith( *gameDefinition.pClassManager );
	pSkyManager->UpdateWith( *gameDefinition.pSkyManager );
	pParticleEmitterManager.UpdateWith( gameDefinition.pParticleEmitterManager );
	
	pListWorldProperties.SetToDeepCopyFrom( gameDefinition.pListWorldProperties );
	pListDecalProperties.SetToDeepCopyFrom( gameDefinition.pListDecalProperties );
}

void igdeGameDefinition::UpdateWithElementClasses( const igdeGameDefinition &gameDefinition ){
	pClassManager->UpdateWithElementClasses( *gameDefinition.pClassManager );
}

void igdeGameDefinition::UpdateWithFound( const igdeGameDefinition &gameDefinition ){
	pSkinManager->UpdateWithFound( *gameDefinition.pSkinManager );
	pSkyManager->UpdateWithFound( *gameDefinition.pSkyManager );
}



// Engine Objects
///////////////////

void igdeGameDefinition::SetDefaultModel( deModel *model ){
	if( pDefaultModel ){
		pDefaultModel->FreeReference();
		pDefaultModel = NULL;
	}
	
	pDefaultModel = model;
	
	if( model ){
		model->AddReference();
	}
}

void igdeGameDefinition::SetDefaultSkin( deSkin *skin ){
	if( pDefaultSkin ){
		pDefaultSkin->FreeReference();
		pDefaultSkin = NULL;
	}
	
	pDefaultSkin = skin;
	
	if( skin ){
		skin->AddReference();
	}
}

void igdeGameDefinition::UpdateEngineObjects(){
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	
	// free old engine objects
	if( pDefaultModel ){
		pDefaultModel->FreeReference();
		pDefaultModel = NULL;
	}
	if( pDefaultSkin ){
		pDefaultSkin->FreeReference();
		pDefaultSkin = NULL;
	}
	
	// load default engine objects
	try{
		pDefaultModel = engine.GetModelManager()->LoadModel( "/igde/models/box/box.demodel", "/" );
		pDefaultSkin = engine.GetSkinManager()->LoadSkin( "/igde/models/box/materials/problem.deskin", "/" );
		
	}catch( const deException &e ){
		e.PrintError();
		throw;
	}
}



void igdeGameDefinition::FindClasses( deVirtualFileSystem &vfs, igdeGDClassManager &found ){
	deLogger &logger = *pEnvironment.GetLogger();
	decTimer timer;
	igdeXMLElementClass loader( &logger );
	
	found.RemoveAll();
	
	const decStringSet &pathList = pClassManager->GetAutoFindPath();
	const int pathCount = pathList.GetCount();
	int i;
	for( i=0; i<pathCount; i++ ){
		const decString &path = pathList.GetAt( i );
		logger.LogInfoFormat( "IGDE", "Load XML Element Classes: %s", path.GetString() );
		loader.LoadElementClasses( found, vfs, decPath::CreatePathUnix( path ) );
	}
	logger.LogInfoFormat( "IGDE", "Load XML Element Classes done: %.1fs (%d found)",
		timer.GetElapsedTime(), found.GetCount() );
	
	// DEBUG
	/*
	const igdeGDClassManager &gdClasses = *gameProject.GetXMLEClassGameDefinition()->GetClassManager();
	const int count = gdClasses.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const igdeGDClass &gdClass = *gdClasses.GetAt( i );
		GetLogger()->LogInfoFormat( LOGSOURCE, "XMLEClass: %s%s", gdClass.GetName().GetString(),
			gameProject.GetProjectGameDefinition()->GetClassManager()->HasNamed( gdClass.GetName() )
				? "(existing in project)" : "" );
	}
	*/
	// DEBUG
}

void igdeGameDefinition::FindSkins( deVirtualFileSystem &vfs, igdeGDSkinManager &found ){
	const igdeFilePatternList &patterns = *pEnvironment.GetOpenFilePatternList( igdeEnvironment::efpltSkin );
	const decStringSet &pathList = pSkinManager->GetAutoFindPath();
	deLogger &logger = *pEnvironment.GetLogger();
	const int pathCount = pathList.GetCount();
	decTimer timer;
	int i, j;
	
	for( i=0; i<pathCount; i++ ){
		const decString &path = pathList.GetAt( i );
		logger.LogInfoFormat( "IGDE", "Find Skins: %s", path.GetString() );
		
		const int patternCount = patterns.GetFilePatternCount();
		const decPath searchPath( decPath::CreatePathUnix( path ) );
		
		for( j=0; j<patternCount; j++ ){
			const igdeFilePattern &pattern = *patterns.GetFilePatternAt( j );
			found.FindAndAddSkins( vfs, searchPath, pattern.GetPattern() );
		}
	}
	logger.LogInfoFormat( "IGDE", "Find Skins done: %.1fs (%d found)", timer.GetElapsedTime(), found.GetSkinCount() );
}

void igdeGameDefinition::FindSkies( deVirtualFileSystem &vfs, igdeGDSkyManager &found ){
	const igdeFilePatternList &patterns = *pEnvironment.GetOpenFilePatternList( igdeEnvironment::efpltSky );
	const decStringSet &pathList = pSkyManager->GetAutoFindPath();
	deLogger &logger = *pEnvironment.GetLogger();
	const int pathCount = pathList.GetCount();
	decTimer timer;
	int i, j;
	
	for( i=0; i<pathCount; i++ ){
		const decString &path = pathList.GetAt( i );
		logger.LogInfoFormat( "IGDE", "Find Skies: %s", path.GetString() );
		
		const int patternCount = patterns.GetFilePatternCount();
		const decPath searchPath( decPath::CreatePathUnix( path ) );
		
		for( j=0; j<patternCount; j++ ){
			const igdeFilePattern &pattern = *patterns.GetFilePatternAt( j );
			found.FindAndAddSkies( vfs, searchPath, pattern.GetPattern() );
		}
	}
	logger.LogInfoFormat( "IGDE", "Find Skies done: %.1fs (%d found)",
		timer.GetElapsedTime(), found.GetSkyList().GetCount() );
}




// Private Functions
//////////////////////

void igdeGameDefinition::pCleanUp(){
	if( pSkyManager ){
		delete pSkyManager;
	}
	if( pClassManager ){
		delete pClassManager;
	}
	if( pSkinManager ){
		delete pSkinManager;
	}
	pParticleEmitterManager.RemoveAllEmitters();
	
	if( pDefaultModel ){
		pDefaultModel->FreeReference();
	}
	if( pDefaultSkin ){
		pDefaultSkin->FreeReference();
	}
}
