/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delPatch.h"
#include "delPatchXML.h"
#include "delPatchManager.h"
#include "../delGame.h"
#include "../delGameManager.h"
#include "../../delLauncher.h"
#include "../../engine/delEngine.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/resources/archive/deArchiveManager.h>



// Class delPatchManager
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

delPatchManager::delPatchManager( delLauncher &launcher ) :
pLauncher( launcher ){
}

delPatchManager::~delPatchManager(){
	Clear();
}



// Management
///////////////

void delPatchManager::LoadPatches( delEngineInstance &instance ){
	pLauncher.GetLogger()->LogInfo( pLauncher.GetLogSource(), "Loading patch list" );
	
	deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	decPath pathDisk;
	
	pPatches.RemoveAll();
	
	// load patches from known delga directories
	const delGameList &games = pLauncher.GetGameManager().GetGames();
	const int gameCount = games.GetCount();
	decPath pathSearch;
	int i;
	
	for( i=0; i<gameCount; i++ ){
		const delGame &game = *games.GetAt( i );
		if( game.GetDelgaFile().IsEmpty() ){
			continue;
		}
		
		vfs->RemoveAllContainers();
		pathDisk.SetFromNative( game.GetDelgaFile() );
		pathDisk.RemoveLastComponent();
		
		vfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathRoot, pathDisk ) ) );
		pScanPatchDefFiles( instance, vfs, pathDisk, pathRoot );
	}
	
	// load patches from install games directory (deprecated)
	vfs->RemoveAllContainers();
	pathDisk.SetFromNative( pLauncher.GetPathGames() );
	vfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathRoot, pathDisk ) ) );
	pScanPatchDefFiles( instance, vfs, pathDisk, pathRoot );
}

void delPatchManager::LoadPatchFromDisk( delEngineInstance &instance, const decString &path, delPatchList &list ){
	deLogger &logger = *pLauncher.GetLogger();
	delPatchXML patchXML( &logger, pLauncher.GetLogSource() );
	
	logger.LogInfoFormat( pLauncher.GetLogSource(), "Reading patch file '%s'", path.GetString() );
	
	if( path.EndsWith( ".delga" ) ){
		delPatchList delgaPatches;
		pLauncher.GetEngine().ReadDelgaPatchDefs( instance, path, delgaPatches );
		
		const int count = delgaPatches.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			list.Add( delgaPatches.GetAt( i ) );
		}
		
	}else{
		const decDiskFileReader::Ref reader( decDiskFileReader::Ref::New( new decDiskFileReader( path ) ) );
		const delPatch::Ref patch( delPatch::Ref::New( new delPatch ) );
		
		patchXML.ReadFromFile( reader, patch );
		
		if( ! decPath::IsNativePathAbsolute( patch->GetPatchDirectory() ) ){
			decPath baseDir( decPath::CreatePathNative( path ) );
			baseDir.RemoveLastComponent();
			baseDir.AddUnixPath( patch->GetPatchDirectory() );
			patch->SetPatchDirectory( baseDir.GetPathNative() );
		}
		
		list.Add( patch );
	}
}

void delPatchManager::Clear(){
	pPatches.RemoveAll();
}



// Protected Functions
////////////////////////

void delPatchManager::pScanPatchDefFiles( delEngineInstance &instance,
deVirtualFileSystem &vfs, const decPath &baseDir, const decPath &directory ){
	deCollectFileSearchVisitor collect;
	collect.AddPattern( "*.depatch" );
	collect.AddPattern( "*.delga" );
	collect.SetRecursive( true );
	vfs.SearchFiles( directory, collect );
	
	const dePathList &result = collect.GetFiles();
	const int count = result.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pProcessFoundFiles( instance, baseDir + result.GetAt( i ) );
	}
}

void delPatchManager::pProcessFoundFiles( delEngineInstance &instance, const decPath &path ){
	delPatchList list;
	try{
		LoadPatchFromDisk( instance, path.GetPathNative(), list );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
		return;
	}
	
	const int count = list.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delPatch * const patch = list.GetAt( i );
		if( pPatches.HasWithID( patch->GetIdentifier() ) ){
// 			pLauncher.GetLogger()->LogWarnFormat( pLauncher.GetLogSource(), "Ignore duplicate game patch '%s'",
// 				patch->GetIdentifier().ToHexString( false ).GetString() );
			continue;
		}
		
		pPatches.Add( patch );
	}
}
