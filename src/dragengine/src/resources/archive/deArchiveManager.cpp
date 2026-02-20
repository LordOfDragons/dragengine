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
deFileResourceManager(engine, ertArchive)
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

deArchive::Ref deArchiveManager::OpenArchive(const char *filename, const char *basePath){
	return OpenArchive(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deArchive::Ref deArchiveManager::OpenArchive(deVirtualFileSystem *vfs, const char *filename,
const char *basePath){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	deBaseArchiveContainer *peer = nullptr;
	deArchive::Ref archive;
	decPath path;
	
	try{
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		deArchive *findArchive = (deArchive*)pArchives.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findArchive && findArchive->GetModificationTime() != modificationTime){
			LogInfoFormat("Archive '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findArchive->MarkOutdated();
			findArchive = nullptr;
		}
		
		if(findArchive){
			archive = findArchive;
			
		}else{
			deBaseArchiveModule * const module = (deBaseArchiveModule*)GetModuleSystem()->
				GetModuleAbleToLoad(deModuleSystem::emtArchive, path.GetPathUnix());
			
			peer = module->CreateContainer(OpenFileForReading(*vfs, path.GetPathUnix()));
			
			archive = deArchive::Ref::New(this, vfs, path.GetPathUnix(), modificationTime);
			archive->SetPeerContainer(peer);
			peer = nullptr;
			
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

deArchiveContainer::Ref deArchiveManager::CreateContainer(const decPath &rootPath,
deArchive *archive, const decPath &archivePath){
	if(!archive){
		DETHROW(deeInvalidParam);
	}
	
	auto container = deArchiveContainer::Ref::New(rootPath, archive, archivePath);
	
	pContainers.Add(&container->GetLLManager());
	
	return container;
}


void deArchiveManager::ReleaseLeakingResources(){
	// containers
	const int containerCount = pContainers.GetCount();
	if(containerCount > 0){
		LogWarnFormat("%i leaking archive containers", containerCount);
		
		pContainers.Visit([&](deArchiveContainer *container){
			LogWarnFormat("- %s", container->GetArchive()->GetFilename().IsEmpty()
				? "<temporary>" : container->GetArchive()->GetFilename().GetString());
			container->MarkLeaking();
		});
		
		pContainers.RemoveAll();
	}
	
	// archives
	const int count = GetArchiveCount();
	if(count > 0){
		LogWarnFormat("%i leaking archives", count);
		
		pArchives.GetResources().Visit([&](deResource *r){
			deArchive *archive = static_cast<deArchive*>(r);
			LogWarnFormat("- %s", archive->GetFilename().IsEmpty()
				? "<temporary>" : archive->GetFilename().GetString());
			archive->SetPeerContainer(nullptr);  // prevent crash
		});
		
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
	
	if(container->GetLLManager().GetList() != &pContainers){
		return;
	}
	
	pContainers.Remove(&container->GetLLManager());
	container->MarkLeaking();
}
