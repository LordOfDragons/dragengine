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

#include <stdlib.h>

#include "dealRemoveDirTree.h"
#include "common/exceptions.h"
#include "common/file/decPath.h"
#include "filesystem/dePathList.h"
#include "filesystem/deCollectDirectorySearchVisitor.h"
#include "filesystem/deVFSDiskDirectory.h"
#include "filesystem/deVFSContainer.h"
#include "filesystem/deCollectFileSearchVisitor.h"
#include "filesystem/deVirtualFileSystem.h"
#include "logger/deLogger.h"



// Definitions
////////////////

#define LOGSOURCE "DEAndroidLauncher"



// Class dealRemoveDirTree
////////////////////////////

// Constructor, destructor
////////////////////////////

dealRemoveDirTree::dealRemoveDirTree(deLogger &logger) :
pLogger(logger){
}

dealRemoveDirTree::~dealRemoveDirTree(){
}



// Management
///////////////

void dealRemoveDirTree::Remove(const char *path){
	pLogger.LogInfoFormat(LOGSOURCE, "RemoveDirTree '%s'", path);
	
	// split path into parent directory and directory name
	decPath diskPath;
	diskPath.SetFromNative(path);
	
	const decString dirName(diskPath.GetLastComponent());
	diskPath.RemoveLastComponent();
	
	// create container for base directory
	deVirtualFileSystem::Ref vfs((deVirtualFileSystem::Ref::NewWith()));
	
	deVFSDiskDirectory::Ref container((deVFSDiskDirectory::Ref::NewWith(diskPath)));
	container->SetReadOnly(false);
	vfs->AddContainer(container);
	
	// delete the directory recursively if present
	decPath deletePath;
	deletePath.AddComponent(dirName);
	
	if(vfs->ExistsFile(deletePath)){
		pRemove(vfs, deletePath);
	}
}



// Private Functions
//////////////////////

void dealRemoveDirTree::pRemove(deVirtualFileSystem &vfs, const decPath &deletePath){
	//pLogger.LogInfoFormat( LOGSOURCE, "Process '%s'", deletePath.GetPathUnix().GetString() );
	// delete directories
	deCollectDirectorySearchVisitor collectDirectories;
	vfs.SearchFiles(deletePath, collectDirectories);
	const dePathList &directories = collectDirectories.GetDirectories();
	const int directoryCount = directories.GetCount();
	int i;
	
	for(i=0; i<directoryCount; i++){
		pRemove(vfs, directories.GetAt(i));
	}
	
	// delete all files
	deCollectFileSearchVisitor collectFiles;
	vfs.SearchFiles(deletePath, collectFiles);
	const dePathList &files = collectFiles.GetFiles();
	const int fileCount = directories.GetCount();
	
	for(i=0; i<fileCount; i++){
		//pLogger.LogInfoFormat( LOGSOURCE, "Delete File '%s'", files.GetAt( i ).GetPathUnix().GetString() );
		vfs.DeleteFile(files.GetAt(i));
	}
	
	// delete directory itself. this is not done since android seems to delete directories
	// automatically if they become empty
	//pLogger.LogInfoFormat( LOGSOURCE, "Delete Directory '%s'", deletePath.GetPathUnix().GetString() );
	//container.DeleteFile( deletePath );
}
