
/* 
 * Drag[en]gine DELGA Archive Module
 *
 * Copyright (C) 2017, Plüss Roland ( roland@rptd.ch )
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

#include "deArchiveDelga.h"
#include "deadArchiveFile.h"
#include "deadArchiveDirectory.h"

#include <dragengine/common/exceptions.h>



// Class deadArchiveDirectory
///////////////////////////////

// Constructor, destructor
////////////////////////////

deadArchiveDirectory::deadArchiveDirectory(deArchiveDelga &module, const char *filename) :
pModule(module),
pFilename(filename){
}

deadArchiveDirectory::~deadArchiveDirectory(){
}



// Directories
///////////////

int deadArchiveDirectory::GetDirectoryCount() const{
	return pDirectories.GetCount();
}

deadArchiveDirectory *deadArchiveDirectory::GetDirectoryAt(int index) const{
	return (deadArchiveDirectory*)pDirectories.GetAt(index);
}

bool deadArchiveDirectory::HasDirectoryNamed(const char *filename) const{
	const int count = pDirectories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deadArchiveDirectory * const directory = (deadArchiveDirectory*)pDirectories.GetAt(i);
		if(directory->GetFilename() == filename){
			return true;
		}
	}
	
	return false;
}

deadArchiveDirectory *deadArchiveDirectory::GetDirectoryNamed(const char *filename) const{
	const int count = pDirectories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deadArchiveDirectory * const directory = (deadArchiveDirectory*)pDirectories.GetAt(i);
		if(directory->GetFilename() == filename){
			return directory;
		}
	}
	
	return NULL;
}

deadArchiveDirectory *deadArchiveDirectory::GetOrAddDirectoryNamed(const char *filename){
	deadArchiveDirectory::Ref directory = GetDirectoryNamed(filename);
	if(directory){
		return directory;
	}
	
	try{
		directory.TakeOver(new deadArchiveDirectory(pModule, filename));
		pDirectories.Add(directory);
	}catch(const deException &){
		throw;
	}
	return directory;
}

deadArchiveDirectory *deadArchiveDirectory::GetDirectoryByPath(const decPath &path){
	const int count = path.GetComponentCount();
	if(count == 0){
		return NULL;
		
	}else if(count == 1){
		return GetDirectoryNamed(path.GetComponentAt(0));
	}
	
	deadArchiveDirectory::Ref directory = this;
	int i;
	
	for(i=0; i<count; i++){
		directory = directory->GetDirectoryNamed(path.GetComponentAt(i));
		if(!directory){
			return NULL;
		}
	}
	
	return directory;
}

void deadArchiveDirectory::AddDirectory(deadArchiveDirectory *directory){
	if(!directory){
		DETHROW(deeInvalidParam);
	}
	if(HasDirectoryNamed(directory->GetFilename())){
		DETHROW(deeInvalidParam);
	}
	if(HasFileNamed(directory->GetFilename())){
		DETHROW(deeInvalidParam);
	}
	
	pDirectories.Add(directory);
}



// Files
//////////

int deadArchiveDirectory::GetFileCount() const{
	return pFiles.GetCount();
}

deadArchiveFile *deadArchiveDirectory::GetFileAt(int index) const{
	return (deadArchiveFile*)pFiles.GetAt(index);
}

bool deadArchiveDirectory::HasFileNamed(const char *filename) const{
	const int count = pFiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deadArchiveFile * const file = (deadArchiveFile*)pFiles.GetAt(i);
		if(file->GetFilename() == filename){
			return true;
		}
	}
	
	return false;
}

deadArchiveFile *deadArchiveDirectory::GetFileNamed(const char *filename) const{
	const int count = pFiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deadArchiveFile * const file = (deadArchiveFile*)pFiles.GetAt(i);
		if(file->GetFilename() == filename){
			return file;
		}
	}
	
	return NULL;
}

deadArchiveFile *deadArchiveDirectory::GetFileByPath(const decPath &path) const{
	const int count = path.GetComponentCount();
	if(count == 0){
		return NULL;
		
	}else if(count == 1){
		return GetFileNamed(path.GetComponentAt(0));
	}
	
	const deadArchiveDirectory::Ref directory = this;
	int i;
	
	for(i=0; i<count-1; i++){
		directory = directory->GetDirectoryNamed(path.GetComponentAt(i));
		if(!directory){
			return NULL;
		}
	}
	
	return directory->GetFileNamed(path.GetComponentAt(count - 1));
}

void deadArchiveDirectory::AddFile(deadArchiveFile *file){
	if(!file){
		DETHROW(deeInvalidParam);
	}
	if(HasDirectoryNamed(file->GetFilename())){
		DETHROW(deeInvalidParam);
	}
	if(HasFileNamed(file->GetFilename())){
		DETHROW(deeInvalidParam);
	}
	
	pFiles.Add(file);
}
