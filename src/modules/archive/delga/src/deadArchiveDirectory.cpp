
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

bool deadArchiveDirectory::HasDirectoryNamed(const char *filename) const{
	return pDirectories.HasMatching([&](const deadArchiveDirectory &dir){
		return dir.GetFilename() == filename;
	});
}

deadArchiveDirectory *deadArchiveDirectory::GetDirectoryNamed(const char *filename) const{
	return pDirectories.FindOrDefault([&](const deadArchiveDirectory &dir){
		return dir.GetFilename() == filename;
	});
}

deadArchiveDirectory *deadArchiveDirectory::GetOrAddDirectoryNamed(const char *filename){
	deadArchiveDirectory * const findDirectory = GetDirectoryNamed(filename);
	if(findDirectory){
		return findDirectory;
	}
	
	const deadArchiveDirectory::Ref directory(deadArchiveDirectory::Ref::New(pModule, filename));
	pDirectories.Add(directory);
	return directory;
}

deadArchiveDirectory *deadArchiveDirectory::GetDirectoryByPath(const decPath &path){
	const int count = path.GetComponentCount();
	if(count == 0){
		return nullptr;
		
	}else if(count == 1){
		return GetDirectoryNamed(path.GetComponentAt(0));
	}
	
	deadArchiveDirectory *directory = this;
	int i;
	
	for(i=0; i<count; i++){
		directory = directory->GetDirectoryNamed(path.GetComponentAt(i));
		if(!directory){
			return nullptr;
		}
	}
	
	return directory;
}

void deadArchiveDirectory::AddDirectory(deadArchiveDirectory *directory){
	DEASSERT_NOTNULL(directory)
	DEASSERT_FALSE(HasDirectoryNamed(directory->GetFilename()))
	DEASSERT_FALSE(HasFileNamed(directory->GetFilename()))
	
	pDirectories.AddOrThrow(directory);
}



// Files
//////////

bool deadArchiveDirectory::HasFileNamed(const char *filename) const{
	return pFiles.HasMatching([&](const deadArchiveFile &file){
		return file.GetFilename() == filename;
	});
}

deadArchiveFile *deadArchiveDirectory::GetFileNamed(const char *filename) const{
	return pFiles.FindOrDefault([&](const deadArchiveFile &file){
		return file.GetFilename() == filename;
	});
}

deadArchiveFile *deadArchiveDirectory::GetFileByPath(const decPath &path) const{
	const int count = path.GetComponentCount();
	if(count == 0){
		return nullptr;
		
	}else if(count == 1){
		return GetFileNamed(path.GetComponentAt(0));
	}
	
	const deadArchiveDirectory *directory = this;
	int i;
	
	for(i=0; i<count-1; i++){
		directory = directory->GetDirectoryNamed(path.GetComponentAt(i));
		if(!directory){
			return nullptr;
		}
	}
	
	return directory->GetFileNamed(path.GetComponentAt(count - 1));
}

void deadArchiveDirectory::AddFile(deadArchiveFile *file){
	DEASSERT_NOTNULL(file)
	DEASSERT_FALSE(HasDirectoryNamed(file->GetFilename()))
	DEASSERT_FALSE(HasFileNamed(file->GetFilename()))
	
	pFiles.AddOrThrow(file);
}
