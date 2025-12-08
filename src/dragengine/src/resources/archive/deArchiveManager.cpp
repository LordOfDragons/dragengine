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

#include "deArchive.h"
#include "deArchiveContainer.h"
#include "deArchiveManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/file/decPath.h"
#include "../../errortracing/deErrorTrace.h"
#include "../../errortracing/deErrorTracePoint.h"
#include "../../errortracing/deErrorTraceValue.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/archive/deBaseArchiveModule.h"
#include "../../systems/modules/archive/deBaseArchiveContainer.h"



// Class deArchiveManager
///////////////////////////

// Constructor, destructor
////////////////////////////

deArchiveManager::deArchiveManager(deEngine *engine) :
deFileResourceManager(engine, ertArchive),
pContainerRoot(NULL),
pContainerTail(NULL),
pContainerCount(0)
{
	SetLoggingName("archive");
}

deArchiveManager::~deArchiveManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deArchiveManager::GetArchiveCount() const{
	return pArchives.GetCount();
}

deArchive *deArchiveManager::GetRootArchive() const{
	return (deArchive*)pArchives.GetRoot();
}

deArchive *deArchiveManager::OpenArchive(const char *filename, const char *basePath){
	return OpenArchive(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deArchive *deArchiveManager::OpenArchive(deVirtualFileSystem *vfs, const char *filename,
const char *basePath){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	decBaseFileReader::Ref fileReader = NULL;
	deBaseArchiveContainer *peer = NULL;
	deArchive::Ref findArchive;
	deArchive::Ref archive = NULL;
	decPath path;
	
	try{
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		findArchive = (deArchive*)pArchives.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findArchive && findArchive->GetModificationTime() != modificationTime){
			LogInfoFormat("Archive '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findArchive->MarkOutdated();
			findArchive = NULL;
		}
		
		if(findArchive){
			archive = findArchive;
			
		}else{
			deBaseArchiveModule * const module = (deBaseArchiveModule*)GetModuleSystem()->
				GetModuleAbleToLoad(deModuleSystem::emtArchive, path.GetPathUnix());
			
			fileReader = OpenFileForReading(*vfs, path.GetPathUnix());
			
			peer = module->CreateContainer(fileReader);
			fileReader = NULL;
			
			archive.TakeOver(new deArchive(this, vfs, path.GetPathUnix(), modificationTime));
			archive->SetPeerContainer(peer);
			peer = NULL;
			
			pArchives.Add(archive);
		}
		
	}catch(const deException &){
		LogErrorFormat("Open archive '%s' (base path '%s') failed", filename,
			basePath ? basePath : "");
		if(peer){
			delete peer;
		}
		throw;
	}
	
	return archive;
}

deArchiveContainer *deArchiveManager::CreateContainer(const decPath &rootPath,
deArchive *archive, const decPath &archivePath){
	if(!archive){
		DETHROW(deeInvalidParam);
	}
	
	deArchiveContainer *container = new deArchiveContainer(rootPath, archive, archivePath);
	
	if(pContainerTail){
		pContainerTail->SetLLManagerNext(container);
		container->SetLLManagerPrev(pContainerTail);
		pContainerTail = container;
		
	}else{ // it can never happen that root is non-NULL if tail is NULL
		pContainerRoot = container;
		pContainerTail = container;
	}
	
	pContainerCount++;
	
	return container;
}



void deArchiveManager::ReleaseLeakingResources(){
	// containers
	if(pContainerCount > 0){
		LogWarnFormat("%i leaking archive containers", pContainerCount);
		
		while(pContainerRoot){
			LogWarnFormat("- %s", pContainerRoot->GetArchive()->GetFilename().IsEmpty()
				? "<temporary>" : pContainerRoot->GetArchive()->GetFilename().GetString());
			pContainerRoot->MarkLeaking();
			pContainerRoot= pContainerRoot->GetLLManagerNext();
		}
		
		pContainerTail = NULL;
		pContainerCount = 0;
	}
	
	// archives
	const int count = GetArchiveCount();
	
	if(count > 0){
		deArchive::Ref archive = (deArchive*)pArchives.GetRoot();
		
		LogWarnFormat("%i leaking archives", count);
		
		while(archive){
			LogWarnFormat("- %s", archive->GetFilename().IsEmpty()
				? "<temporary>" : archive->GetFilename().GetString());
			archive->SetPeerContainer(NULL);  // prevent crash
			archive = (deArchive*)archive->GetLLManagerNext();
		}
		
		pArchives.RemoveAll(); // we do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deArchiveManager::RemoveResource(deResource *resource){
	pArchives.RemoveIfPresent(resource);
}

void deArchiveManager::RemoveContainer(deArchiveContainer *container){
	if(!container){
		DETHROW(deeInvalidParam);
	}
	
	if(container != pContainerRoot && !container->GetLLManagerNext()
	&& !container->GetLLManagerPrev()){
		return;
	}
	
	if(pContainerCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	if(container == pContainerTail){
		if(container->GetLLManagerNext()){
			DETHROW(deeInvalidParam);
		}
		
		pContainerTail = pContainerTail->GetLLManagerPrev();
		if(pContainerTail){
			pContainerTail->SetLLManagerNext(NULL);
		}
		
	}else{
		if(!container->GetLLManagerNext()){
			DETHROW(deeInvalidParam);
		}
		
		container->GetLLManagerNext()->SetLLManagerPrev(container->GetLLManagerPrev());
	}
	
	if(container == pContainerRoot){
		if(container->GetLLManagerPrev()){
			DETHROW(deeInvalidParam);
		}
		
		pContainerRoot = pContainerRoot->GetLLManagerNext();
		if(pContainerRoot){
			pContainerRoot->SetLLManagerPrev(NULL);
		}
		
	}else{
		if(!container->GetLLManagerPrev()){
			DETHROW(deeInvalidParam);
		}
		
		container->GetLLManagerPrev()->SetLLManagerNext(container->GetLLManagerNext());
	}
	
	container->SetLLManagerNext(NULL);
	container->SetLLManagerPrev(NULL);
	pContainerCount--;
	
	container->MarkLeaking();
}
