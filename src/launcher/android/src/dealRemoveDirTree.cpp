/* 
 * Drag[en]gine Android Launcher
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

#include <stdlib.h>

#include "dealRemoveDirTree.h"
#include "common/exceptions.h"
#include "common/file/decPath.h"
#include "filesystem/dePathList.h"
#include "filesystem/deCollectDirectorySearchVisitor.h"
#include "filesystem/deVFSDiskDirectory.h"
#include "filesystem/deVFSContainerReference.h"
#include "filesystem/deCollectFileSearchVisitor.h"
#include "filesystem/deVirtualFileSystem.h"
#include "filesystem/deVirtualFileSystemReference.h"
#include "logger/deLogger.h"



// Definitions
////////////////

#define LOGSOURCE "DEAndroidLauncher"



// Class dealRemoveDirTree
////////////////////////////

// Constructor, destructor
////////////////////////////

dealRemoveDirTree::dealRemoveDirTree( deLogger &logger ) :
pLogger( logger ){
}

dealRemoveDirTree::~dealRemoveDirTree(){
}



// Management
///////////////

void dealRemoveDirTree::Remove( const char *path ){
	pLogger.LogInfoFormat( LOGSOURCE, "RemoveDirTree '%s'", path );
	
	// split path into parent directory and directory name
	decPath diskPath;
	diskPath.SetFromNative( path );
	
	const decString dirName( diskPath.GetLastComponent() );
	diskPath.RemoveLastComponent();
	
	// create container for base directory
	deVirtualFileSystemReference vfs;
	vfs.TakeOver( new deVirtualFileSystem );
	
	deVFSContainerReference container;
	container.TakeOver( new deVFSDiskDirectory( diskPath ) );
	( ( deVFSDiskDirectory& )( deVFSContainer& )container ).SetReadOnly( false );
	vfs->AddContainer( container );
	
	// delete the directory recursively if present
	decPath deletePath;
	deletePath.AddComponent( dirName );
	
	if( vfs->ExistsFile( deletePath ) ){
		pRemove( vfs, deletePath );
	}
}



// Private Functions
//////////////////////

void dealRemoveDirTree::pRemove( deVirtualFileSystem &vfs, const decPath &deletePath ){
	//pLogger.LogInfoFormat( LOGSOURCE, "Process '%s'", deletePath.GetPathUnix().GetString() );
	// delete directories
	deCollectFileSearchVisitor collectDirectories;
	vfs.SearchFiles( deletePath, collectDirectories );
	const dePathList &directories = collectDirectories.GetDirectories();
	const int directoryCount = directories.GetCount();
	int i;
	
	for( i=0; i<directoryCount; i++ ){
		pRemove( vfs, directories.GetAt( i ) );
	}
	
	// delete all files
	deCollectFileSearchVisitor collectFiles;
	vfs.SearchFiles( deletePath, collectFiles );
	const dePathList &files = collectFiles.GetFiles();
	const int fileCount = directories.GetCount();
	
	for( i=0; i<fileCount; i++ ){
		//pLogger.LogInfoFormat( LOGSOURCE, "Delete File '%s'", files.GetAt( i ).GetPathUnix().GetString() );
		vfs.DeleteFile( files.GetAt( i ) );
	}
	
	// delete directory itself. this is not done since android seems to delete directories
	// automatically if they become empty
	//pLogger.LogInfoFormat( LOGSOURCE, "Delete Directory '%s'", deletePath.GetPathUnix().GetString() );
	//container.DeleteFile( deletePath );
}
