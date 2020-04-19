/* 
 * Drag[en]gine GUI Launcher
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

#include "deglPatch.h"
#include "deglPatchXML.h"
#include "deglPatchManager.h"
#include "../../deglLauncher.h"
#include "../../config/deglConfiguration.h"
#include "../../engine/deglEngine.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveReference.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/resources/archive/deArchiveManager.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class deglPatchManager
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglPatchManager::deglPatchManager( deglLauncher &launcher ) :
pLauncher( launcher ){
}

deglPatchManager::~deglPatchManager(){
}



// Management
///////////////

void deglPatchManager::LoadPatchList( deglEngineInstance &instance ){
	pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Loading patch list" );
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	const decPath pathDisk( decPath::CreatePathNative( pLauncher.GetConfiguration()->GetPathGames() ) );
	container.TakeOver( new deVFSDiskDirectory( pathRoot, pathDisk ) );
	vfs->AddContainer( container );
	
	pPatches.RemoveAll();
	pScanPatchDefFiles( instance, vfs, pathDisk, pathRoot );
}

void deglPatchManager::LoadPatchFromDisk( deglEngineInstance &instance, const decString &path, deglPatchList &list ){
	deLogger &logger = *pLauncher.GetLogger();
	deglPatchXML patchXML( &logger, LOGSOURCE );
	
	logger.LogInfoFormat( LOGSOURCE, "Reading patch file '%s'", path.GetString() );
	
	if( path.EndsWith( ".delga" ) ){
		deglPatchList delgaPatches;
		pLauncher.GetEngine()->ReadDelgaPatchDefs( instance, path, delgaPatches );
		
		const int count = delgaPatches.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			list.Add( delgaPatches.GetAt( i ) );
		}
		
	}else{
		decBaseFileReaderReference reader;
		reader.TakeOver( new decDiskFileReader( path ) );
		
		deObjectReference refPatch;
		deglPatch &patch = ( deglPatch& )( deObject& )refPatch;
		
		patchXML.ReadFromFile( reader, patch );
		
		if( ! decPath::IsNativePathAbsolute( patch.GetPatchDirectory() ) ){
			decPath baseDir( decPath::CreatePathNative( path ) );
			baseDir.RemoveLastComponent();
			baseDir.AddUnixPath( patch.GetPatchDirectory() );
			patch.SetPatchDirectory( baseDir.GetPathNative() );
		}
		
		list.Add( &patch );
	}
}



// Private Functions
//////////////////////

void deglPatchManager::pScanPatchDefFiles( deglEngineInstance &instance,
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

void deglPatchManager::pProcessFoundFiles( deglEngineInstance &instance, const decPath &path ){
	deglPatchList list;
	try{
		LoadPatchFromDisk( instance, path.GetPathNative(), list );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
		return;
	}
	
	const int count = list.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deglPatch * const patch = list.GetAt( i );
		if( pPatches.HasWithID( patch->GetIdentifier() ) ){
			pLauncher.GetLogger()->LogWarnFormat( LOGSOURCE, "Ignore duplicate game patch '%s'",
				patch->GetIdentifier().ToHexString( false ).GetString() );
			continue;
		}
		
		pPatches.Add( patch );
	}
}
