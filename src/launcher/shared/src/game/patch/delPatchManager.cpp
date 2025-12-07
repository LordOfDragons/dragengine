/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	
	deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::NewWith() );
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
		
		vfs->AddContainer( deVFSDiskDirectory::Ref::NewWith(pathRoot, pathDisk) );
		pScanPatchDefFiles( instance, vfs, pathDisk, pathRoot, pPatches );
	}
	
	// load patches from install games directory (deprecated)
	vfs->RemoveAllContainers();
	pathDisk.SetFromNative( pLauncher.GetPathGames() );
	vfs->AddContainer( deVFSDiskDirectory::Ref::NewWith(pathRoot, pathDisk) );
	pScanPatchDefFiles( instance, vfs, pathDisk, pathRoot, pPatches );
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
		const decDiskFileReader::Ref reader( decDiskFileReader::Ref::NewWith(path) );
		const delPatch::Ref patch( delPatch::Ref::NewWith() );
		
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

void delPatchManager::LoadPatchesFromDisk( delEngineInstance &instance, const decString &baseDir, delPatchList &list ){
	deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::NewWith() );
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	const decPath pathDisk( decPath::CreatePathNative( baseDir ) );
	
	vfs->AddContainer( deVFSDiskDirectory::Ref::NewWith(pathRoot, pathDisk, true) );
	pScanPatchDefFiles( instance, vfs, pathDisk, pathRoot, list );
}

void delPatchManager::Clear(){
	pPatches.RemoveAll();
}



// Protected Functions
////////////////////////

void delPatchManager::pScanPatchDefFiles( delEngineInstance &instance, deVirtualFileSystem &vfs,
const decPath &baseDir, const decPath &directory, delPatchList &list ){
	deCollectFileSearchVisitor collect;
	collect.AddPattern( "*.depatch" );
	collect.AddPattern( "*.delga" );
	collect.SetRecursive( true );
	vfs.SearchFiles( directory, collect );
	
	const dePathList &result = collect.GetFiles();
	const int count = result.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pProcessFoundFiles( instance, baseDir + result.GetAt( i ), list );
	}
}

void delPatchManager::pProcessFoundFiles( delEngineInstance &instance,
const decPath &path, delPatchList &list ){
	delPatchList subList;
	try{
		LoadPatchFromDisk( instance, path.GetPathNative(), subList );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
		return;
	}
	
	const int count = subList.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delPatch * const patch = subList.GetAt( i );
		if( ! list.HasWithID( patch->GetIdentifier() ) ){
			list.Add( patch );
		}
	}
}
