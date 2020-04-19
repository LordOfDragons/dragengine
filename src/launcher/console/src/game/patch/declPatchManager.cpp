/* 
 * Drag[en]gine Console Launcher
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

#include "declPatch.h"
#include "declPatchXML.h"
#include "declPatchManager.h"
#include "../../declLauncher.h"
#include "../../config/declConfiguration.h"
#include "../../engine/declEngine.h"

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



// Class declPatchManager
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

declPatchManager::declPatchManager( declLauncher &launcher ) :
pLauncher( launcher ){
}

declPatchManager::~declPatchManager(){
}



// Management
///////////////

void declPatchManager::LoadPatchList(){
	pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Loading patch list" );
	
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	const decPath pathDisk( decPath::CreatePathNative( pLauncher.GetConfiguration()->GetPathGames() ) );
	container.TakeOver( new deVFSDiskDirectory( pathRoot, pathDisk ) );
	vfs->AddContainer( container );
	
	pScanPatchDefFiles( vfs, pathDisk, pathRoot );
}

void declPatchManager::LoadPatchFromDisk( const decString &path, declPatchList &list ){
	deLogger &logger = *pLauncher.GetLogger();
	declPatchXML patchXML( &logger, LOGSOURCE );
	decBaseFileReaderReference reader;
	deObjectReference refPatch;
	
	logger.LogInfoFormat( LOGSOURCE, "Reading game patch file '%s'", path.GetString() );
	
	decPath absPath;
	if( decPath::IsNativePathAbsolute( path ) ){
		absPath.SetFromNative( path );
		
	}else{
		absPath.SetWorkingDirectory();
		absPath.AddNativePath( path );
	}
	
	if( absPath.GetLastComponent().EndsWith( ".delga" ) ){
		deArchiveManager &amgr = *pLauncher.GetEngine()->GetEngine()->GetArchiveManager();
		decPath pathRoot( decPath::CreatePathUnix( "/" ) );
		
		deVirtualFileSystemReference delgaVfs;
		delgaVfs.TakeOver( new deVirtualFileSystem );
		
		decPath pathDelgaDir( absPath );
		const decString delgaFilename( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( new deVFSDiskDirectory( pathDelgaDir ) );
		
		deArchiveReference delgaArchive;
		delgaArchive.TakeOver( amgr.OpenArchive( delgaVfs, delgaFilename, "/" ) );
		
		deVirtualFileSystemReference vfs;
		vfs.TakeOver( new deVirtualFileSystem );
		
		deVFSContainerReference container;
		container.TakeOver( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) );
		vfs->AddContainer( container );
		
		deCollectFileSearchVisitor collect( "*.depatch", true );
		vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collect );
		const dePathList &files = collect.GetFiles();
		if( files.GetCount() == 0 ){
			return;
		}
		
		const int count = files.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			const decPath &file = files.GetAt( i );
			
			refPatch.TakeOver( new declPatch );
			declPatch &patch = ( declPatch& )( deObject& )refPatch;
			
			reader.TakeOver( vfs->OpenFileForReading( file ) );
			patchXML.ReadFromFile( reader, patch );
			
			if( ! decPath::IsNativePathAbsolute( patch.GetPatchDirectory() ) ){
				decPath baseDir( file );
				baseDir.RemoveLastComponent();
				baseDir.AddUnixPath( patch.GetPatchDirectory() );
				patch.SetPatchDirectory( baseDir.GetPathNative() );
			}
			
			patch.SetDelgaFile( absPath.GetPathNative() );
			list.Add( &patch );
		}
		
	}else{
		decBaseFileReaderReference reader;
		reader.TakeOver( new decDiskFileReader( absPath.GetPathNative() ) );
		
		refPatch.TakeOver( new declPatch );
		declPatch &patch = ( declPatch& )( deObject& )refPatch;
		
		patchXML.ReadFromFile( reader, patch );
		
		list.Add( &patch );
	}
}



// Private Functions
//////////////////////

void declPatchManager::pScanPatchDefFiles( deVirtualFileSystem &vfs,
const decPath &baseDir, const decPath &directory ){
	deCollectFileSearchVisitor collect;
	collect.AddPattern( "*.depatch" );
	collect.AddPattern( "*.delga" );
	collect.SetRecursive( true );
	vfs.SearchFiles( directory, collect );
	
	const dePathList &result = collect.GetFiles();
	const int count = result.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pProcessFoundFiles( baseDir + result.GetAt( i ) );
	}
}

void declPatchManager::pProcessFoundFiles( const decPath &path ){
	declPatchList list;
	try{
		LoadPatchFromDisk( path.GetPathNative(), list );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
		return;
	}
	
	const int count = list.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		declPatch * const patch = list.GetAt( i );
		if( pPatches.HasWithID( patch->GetIdentifier() ) ){
			pLauncher.GetLogger()->LogWarnFormat( LOGSOURCE, "Ignore duplicate game patch '%s'",
				patch->GetIdentifier().ToHexString( false ).GetString() );
			continue;
		}
		
		pPatches.Add( patch );
	}
}
