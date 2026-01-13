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
#include <string.h>


#include "deContainerFileSearch.h"
#include "deVFSMemoryFiles.h"
#include "../common/file/decPath.h"
#include "../common/file/decMemoryFile.h"
#include "../common/file/decMemoryFileReader.h"
#include "../common/exceptions.h"



// Class deVFSMemoryFiles
///////////////////////////

// Constructor, destructor
////////////////////////////

deVFSMemoryFiles::deVFSMemoryFiles(const decPath &rootPath) :
deVFSContainer(rootPath),
pDirtyDirectories(){
}

deVFSMemoryFiles::~deVFSMemoryFiles(){
}



// Management
///////////////

bool deVFSMemoryFiles::ExistsFile(const decPath &path){
	return HasMemoryFileWith(path.GetPathUnix());
}

bool deVFSMemoryFiles::CanReadFile(const decPath &path){
	return HasMemoryFileWith(path.GetPathUnix());
}

bool deVFSMemoryFiles::CanWriteFile(const decPath &path){
	return false; // writing to memory files is not allowed
}

bool deVFSMemoryFiles::CanDeleteFile(const decPath &path){
	return false; // deleting memory files is not allowed
}

decBaseFileReader::Ref deVFSMemoryFiles::OpenFileForReading(const decPath &path){
	const int index = IndexOfMemoryFileWith(path.GetPathUnix());
	if(index == -1){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	return decMemoryFileReader::Ref::New(pFiles.GetAt(index));
}

decBaseFileWriter::Ref deVFSMemoryFiles::OpenFileForWriting(const decPath &path){
	DETHROW(deeInvalidAction);
}

void deVFSMemoryFiles::DeleteFile(const decPath &path){
	DETHROW(deeInvalidAction);
}

void deVFSMemoryFiles::TouchFile(const decPath &path){
	DETHROW(deeInvalidAction);
}

void deVFSMemoryFiles::SearchFiles(const decPath &directory, deContainerFileSearch &searcher){
	if(pDirtyDirectories){
		pRebuildDirectories();
		pDirtyDirectories = false;
	}
	
	const int directoryComponentCount = directory.GetComponentCount();
	
	const int countDirectories = pDirectories.GetCount();
	int i, j;
	for(i=0; i<countDirectories; i++){
		const decPath &child = pDirectories.GetAt(i);
		if(child.GetComponentCount() <= countDirectories){
			continue;
		}
		
		for(j=0; j<directoryComponentCount; j++){
			if(child.GetComponentAt(j) != directory.GetComponentAt(j)){
				break;
			}
		}
		
		if(j == directoryComponentCount){
			searcher.Add(child.GetComponentAt(j), deVFSContainer::eftDirectory);
		}
	}
	
	decPath pattern(directory);
	pattern.AddComponent("*");
	const decString patternString(pattern.GetPathUnix());
	const int countFiles = pFiles.GetCount();
	decPath pathFile;
	
	for(i=0; i<countFiles; i++){
		const decMemoryFile &memoryFile = *pFiles.GetAt(i);
		if(!memoryFile.GetFilename().MatchesPattern(patternString)){
			continue;
		}
		
		pathFile.SetFromUnix(memoryFile.GetFilename());
		searcher.Add(pathFile.GetLastComponent(), deVFSContainer::eftRegularFile);
	}
}

deVFSContainer::eFileTypes deVFSMemoryFiles::GetFileType(const decPath &path){
	if(!HasMemoryFileWith(path.GetPathUnix())){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	return eftRegularFile;
}

uint64_t deVFSMemoryFiles::GetFileSize(const decPath &path){
	const int index = IndexOfMemoryFileWith(path.GetPathUnix());
	if(index == -1){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	return (uint64_t)pFiles.GetAt(index)->GetLength();
}

TIME_SYSTEM deVFSMemoryFiles::GetFileModificationTime(const decPath &path){
	const int index = IndexOfMemoryFileWith(path.GetPathUnix());
	if(index == -1){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	return pFiles.GetAt(index)->GetModificationTime();
}



// Memory Files
/////////////////

int deVFSMemoryFiles::IndexOfMemoryFileWith(const char *path) const{
	return pFiles.IndexOfMatching([&](const decMemoryFile &file){
		return file.GetFilename() == path;
	});
}

bool deVFSMemoryFiles::HasMemoryFileWith(const char *path) const{
	if(!path){
		DETHROW(deeInvalidParam);
	}
	
	return pFiles.HasMatching([&](const decMemoryFile &file){
		return file.GetFilename() == path;
	});
}

void deVFSMemoryFiles::AddMemoryFile(decMemoryFile *memoryFile){
	if(!memoryFile || HasMemoryFileWith(memoryFile->GetFilename())){
		DETHROW(deeInvalidParam);
	}
	
	pFiles.Add(memoryFile);
	pDirtyDirectories = true;
}

void deVFSMemoryFiles::RemoveMemoryFile(decMemoryFile *memoryFile){
	const int index = pFiles.IndexOf(memoryFile);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pFiles.RemoveFrom(index);
	pDirtyDirectories = true;
}

void deVFSMemoryFiles::RemoveAllMemoryFiles(){
	pFiles.RemoveAll();
	pDirectories.RemoveAll();
	pDirtyDirectories = false;
}



// Private Functions
//////////////////////

void deVFSMemoryFiles::pRebuildDirectories(){
	pDirectories.RemoveAll();
	
	pFiles.Visit([&](const decMemoryFile &file){
		decPath path(decPath::CreatePathUnix(file.GetFilename()));
		path.RemoveLastComponent();
		
		while(path.GetComponentCount() > 0){
			pDirectories.Add(path);
			path.RemoveLastComponent();
		}
	});
}
