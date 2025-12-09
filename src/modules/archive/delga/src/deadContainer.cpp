
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
#include "deadContainer.h"
#include "deadArchiveDirectory.h"
#include "deadArchiveFile.h"
#include "deadContextUnpack.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decWeakFileReader.h>
#include <dragengine/common/file/decWeakFileWriter.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/filesystem/deContainerFileSearch.h>
#include <dragengine/systems/modules/archive/deBaseArchiveContainer.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deadContainer
////////////////////////

// Constructor, destructor
////////////////////////////

deadContainer::deadContainer(deArchiveDelga &module, decBaseFileReader &reader) :
deBaseArchiveContainer(&reader),
pModule(module),
pFilename(reader.GetFilename())
{
	deadContextUnpack *context = nullptr;
	
	try{
		context = AcquireContextUnpack();
		pArchiveDirectory = context->ReadFileTable();
		ReleaseContextUnpack(context);
		
	}catch(const deException &){
		if(context){
			ReleaseContextUnpack(context);
		}
		pCleanUp();
		throw;
	}
}

deadContainer::~deadContainer(){
	pCleanUp();
}



// Management
///////////////

deadContextUnpack *deadContainer::AcquireContextUnpack(){
	// NOTE it would be optimal if the file position inside the compressed archive data stream
	//      is known as input for this function. in this case the free contexts can be searched
	//      for one where the last read block contains the data already. this would avoid a
	//      seek and block read. right now this is simply ignored
	const deMutexGuard guard(pMutex);
	deadContextUnpack *context;
	
	if(pContextsUnpackFree.GetCount() > 0){
		const int index = pContextsUnpackFree.GetCount() - 1;
		context = (deadContextUnpack*)pContextsUnpackFree.GetAt(index);
		pContextsUnpackFree.RemoveFrom(index);
		
	}else{
		pModule.LogInfoFormat("Archive %s: Create unpacking context (%i contexts)",
			pFilename.GetString(), pContextsUnpack.GetCount() + 1);
		
		context = new deadContextUnpack(pModule, this);
		pContextsUnpack.Add(context);
	}
	
	return context;
}

void deadContainer::ReleaseContextUnpack(deadContextUnpack *context){
	DEASSERT_NOTNULL(context)
	
	context->CloseFile(); // make sure it is closed just in case
	
	const deMutexGuard guard(pMutex);
	pContextsUnpackFree.Add(context);
}



bool deadContainer::ExistsFile(const decPath &path){
	const deMutexGuard guard(pMutex);
	return pArchiveDirectory->GetFileByPath(path) || pArchiveDirectory->GetDirectoryByPath(path);
}

bool deadContainer::CanReadFile(const decPath &path){
	const deMutexGuard guard(pMutex);
	return pArchiveDirectory->GetFileByPath(path) != nullptr;
}

bool deadContainer::CanWriteFile(const decPath &){
	return false;
}

bool deadContainer::CanDeleteFile(const decPath &){
	return false;
}

decBaseFileReader::Ref deadContainer::OpenFileForReading(const decPath &path){
	const deadArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	if(!file){
		DETHROW(deeFileNotFound);
	}
	
	deadContextUnpack * const context = AcquireContextUnpack();
	
	try{
		return decBaseFileReader::Ref::New(context->OpenFileForReading(*file).Pointer());
		
	}catch(const deException &){
		ReleaseContextUnpack(context);
		throw;
	}
}

decBaseFileWriter::Ref deadContainer::OpenFileForWriting(const decPath &){
	// not supported for the time being
	DETHROW(deeInvalidParam);
}

void deadContainer::DeleteFile(const decPath &){
	DETHROW(deeInvalidParam);
}

void deadContainer::TouchFile(const decPath &){
	DETHROW(deeInvalidParam);
}

void deadContainer::SearchFiles(const decPath &directory, deContainerFileSearch &searcher){
	const deMutexGuard guard(pMutex);
	
	deadArchiveDirectory *adir = pArchiveDirectory;
	if(directory.GetComponentCount() > 0){
		adir = adir->GetDirectoryByPath(directory);
	}
	if(!adir){
		return;
	}
	
	const int directoryCount = adir->GetDirectoryCount();
	const int fileCount = adir->GetFileCount();
	int i;
	
	for(i=0; i<directoryCount; i++){
		searcher.Add(adir->GetDirectoryAt(i)->GetFilename(), deVFSContainer::eftDirectory);
	}
	for(i=0; i<fileCount; i++){
		searcher.Add(adir->GetFileAt(i)->GetFilename(), deVFSContainer::eftRegularFile);
	}
}

deVFSContainer::eFileTypes deadContainer::GetFileType(const decPath &path){
	const deMutexGuard guard(pMutex);
	
	if(pArchiveDirectory->GetFileByPath(path)){
		return deVFSContainer::eftRegularFile;
		
	}else if(pArchiveDirectory->GetDirectoryByPath(path)){
		return deVFSContainer::eftDirectory;
		
	}else{
		DETHROW(deeFileNotFound);
	}
}

uint64_t deadContainer::GetFileSize(const decPath &path){
	const deMutexGuard guard(pMutex);
	
	const deadArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	if(!file){
		DETHROW(deeFileNotFound);
	}
	return file->GetFileSize();
}

TIME_SYSTEM deadContainer::GetFileModificationTime(const decPath &path){
	const deMutexGuard guard(pMutex);
	
	const deadArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	if(!file){
		DETHROW(deeFileNotFound);
	}
	return file->GetModificationTime();
}



// Private Functions
//////////////////////

void deadContainer::pCleanUp(){
	const int count = pContextsUnpack.GetCount();
	int i;
	for(i=0; i<count; i++){
		delete (deadContextUnpack*)pContextsUnpack.GetAt(i);
	}
}
