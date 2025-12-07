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

#include "deArchive.h"
#include "deArchiveContainer.h"
#include "deArchiveManager.h"
#include "../../common/exceptions.h"
#include "../../filesystem/dePathList.h"
#include "../../filesystem/deContainerFileSearch.h"
#include "../../systems/modules/archive/deBaseArchiveContainer.h"



// Class deArchiveContainer
/////////////////////////////

// Constructor, destructor
////////////////////////////

deArchiveContainer::deArchiveContainer(const decPath &rootPath, deArchive *archive,
const decPath &archivePath) :
deVFSContainer(rootPath),
pArchivePath(archivePath),
pArchive(NULL),
pLLManagerPrev(NULL),
pLLManagerNext(NULL)
{
	if(!archive){
		DETHROW(deeInvalidParam);
	}
	
	pArchive = archive;
	archive->AddReference();
}

deArchiveContainer::~deArchiveContainer(){
	if(pArchive){
		if(pArchive->GetResourceManager()){
			((deArchiveManager*)pArchive->GetResourceManager())->RemoveContainer(this);
		}
	}
}



// Management
///////////////

bool deArchiveContainer::ExistsFile(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		return false;
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->ExistsFile(path);
		
	}else{
		return pArchive->GetPeerContainer()->ExistsFile(pArchivePath + path);
	}
}

bool deArchiveContainer::CanReadFile(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		return false;
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->CanReadFile(path);
		
	}else{
		return pArchive->GetPeerContainer()->CanReadFile(pArchivePath + path);
	}
}

bool deArchiveContainer::CanWriteFile(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		return false;
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->CanWriteFile(path);
		
	}else{
		return pArchive->GetPeerContainer()->CanWriteFile(pArchivePath + path);
	}
}

bool deArchiveContainer::CanDeleteFile(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		return false;
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->CanDeleteFile(path);
		
	}else{
		return pArchive->GetPeerContainer()->CanDeleteFile(pArchivePath + path);
	}
}

decBaseFileReader *deArchiveContainer::OpenFileForReading(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->OpenFileForReading(path);
		
	}else{
		return pArchive->GetPeerContainer()->OpenFileForReading(pArchivePath + path);
	}
}

decBaseFileWriter *deArchiveContainer::OpenFileForWriting(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->OpenFileForWriting(path);
		
	}else{
		return pArchive->GetPeerContainer()->OpenFileForWriting(pArchivePath + path);
	}
}

void deArchiveContainer::DeleteFile(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		pArchive->GetPeerContainer()->DeleteFile(path);
		
	}else{
		pArchive->GetPeerContainer()->DeleteFile(pArchivePath + path);
	}
}

void deArchiveContainer::TouchFile(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		pArchive->GetPeerContainer()->TouchFile(path);
		
	}else{
		pArchive->GetPeerContainer()->TouchFile(pArchivePath + path);
	}
}

void deArchiveContainer::SearchFiles(const decPath &directory, deContainerFileSearch &searcher){
	if(!pArchive->GetPeerContainer()){
		return;
	}
	
	if(pArchivePath.GetComponentCount() > 0){
		pArchive->GetPeerContainer()->SearchFiles(pArchivePath + directory, searcher);
		
	}else{
		pArchive->GetPeerContainer()->SearchFiles(directory, searcher);
	}
	
}

deVFSContainer::eFileTypes deArchiveContainer::GetFileType(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->GetFileType(path);
		
	}else{
		return pArchive->GetPeerContainer()->GetFileType(pArchivePath + path);
	}
}

uint64_t deArchiveContainer::GetFileSize(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->GetFileSize(path);
		
	}else{
		return pArchive->GetPeerContainer()->GetFileSize(pArchivePath + path);
	}
}

TIME_SYSTEM deArchiveContainer::GetFileModificationTime(const decPath &path){
	if(!pArchive->GetPeerContainer()){
		DETHROW_INFO(deeFileNotFound, path.GetPathUnix());
	}
	
	if(pArchivePath.GetComponentCount() == 0){
		return pArchive->GetPeerContainer()->GetFileModificationTime(path);
		
	}else{
		return pArchive->GetPeerContainer()->GetFileModificationTime(pArchivePath + path);
	}
}



// Resource manager linked list
/////////////////////////////////

void deArchiveContainer::SetLLManagerNext(deArchiveContainer *resource){
	pLLManagerNext = resource;
}

void deArchiveContainer::SetLLManagerPrev(deArchiveContainer *resource){
	pLLManagerPrev = resource;
}

void deArchiveContainer::MarkLeaking(){
	pLLManagerNext = NULL;
	pLLManagerPrev = NULL;
}
