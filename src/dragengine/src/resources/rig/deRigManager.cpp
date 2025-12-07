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
#include "deRig.h"
#include "deRigBuilder.h"
#include "deRigManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/rig/deBaseRigModule.h"



// Class deRigManager
///////////////////////

// Constructor, destructor
////////////////////////////

deRigManager::deRigManager(deEngine *engine) : deFileResourceManager(engine, ertRig){
	SetLoggingName("rig");
}

deRigManager::~deRigManager(){
	ReleaseLeakingResources();
}

// management
int deRigManager::GetRigCount() const{
	return pRigs.GetCount();
}

deRig *deRigManager::GetRootRig() const{
	return (deRig*)pRigs.GetRoot();
}

deRig *deRigManager::GetRigWith(const char *filename) const{
	return GetRigWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deRig *deRigManager::GetRigWith(deVirtualFileSystem *vfs, const char *filename) const{
	deRig * const rig = (deRig*)pRigs.GetWithFilename(vfs, filename);
	return rig && ! rig->GetOutdated() ? rig : NULL;
}

deRig *deRigManager::CreateRig(const char *filename, deRigBuilder &builder){
	return CreateRig(GetEngine()->GetVirtualFileSystem(), filename, builder);
}

deRig *deRigManager::CreateRig(deVirtualFileSystem *vfs, const char *filename, deRigBuilder &builder){
	if(! vfs || ! filename){
		DETHROW(deeInvalidParam);
	}
	deRig *rig = NULL, *findRig;
	
	try{
		// check if the rigation with this filename already exists. this check
		// is only done if the filename is not empty in which case an unnamed
		// rig is created
		if(filename[0] != '\0'){
			findRig = (deRig*)pRigs.GetWithFilename(vfs, filename);
			if(findRig && ! findRig->GetOutdated()){
				DETHROW(deeInvalidParam);
			}
		}
		
		// create rigation using the builder
		rig = new deRig(this, vfs, filename, decDateTime::GetSystemTime());
		builder.BuildRig(rig);
		
		// check rig
		if(! rig->Verify()){
			DETHROW(deeInvalidParam);
		}
		
		// prepare rig
		rig->Prepare();
		
		// load system peers
		GetPhysicsSystem()->LoadRig(rig);
		
		// add rigation
		pRigs.Add(rig);
		
	}catch(const deException &e){
		if(rig){
			rig->FreeReference();
		}
		LogErrorFormat("Creating rig '%s' failed", filename);
		LogException(e);
		throw;
	}
	
	return rig;
}

deRig *deRigManager::LoadRig(const char *filename, const char *basePath){
	return LoadRig(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deRig *deRigManager::LoadRig(deVirtualFileSystem *vfs, const char *filename, const char *basePath){
	if(! vfs || ! filename || filename[0] == '\0' || ! basePath){
		DETHROW(deeInvalidParam);
	}
	
	decBaseFileReader *fileReader=NULL;
	deBaseRigModule *module;
	deRig *rig=NULL, *findRig;
	decPath path;
	
	try{
		// locate file
		if(! FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		// check if the rig with this filename already exists
		findRig = (deRig*)pRigs.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findRig && findRig->GetModificationTime() != modificationTime){
			LogInfoFormat("Rig '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findRig->MarkOutdated();
			findRig = NULL;
		}
		
		if(findRig){
			findRig->AddReference();
			rig = findRig;
			
		}else{
			// find the module able to handle this rig file
			module = (deBaseRigModule*)GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtRig, path.GetPathUnix());
			// load the file with it
			fileReader = OpenFileForReading(*vfs, path.GetPathUnix());
			rig = new deRig(this, vfs, path.GetPathUnix(), modificationTime);
			rig->SetAsynchron(false);
			module->LoadRig(*fileReader, *rig);
			fileReader->FreeReference(); fileReader = NULL;
			// check rig
			if(! rig->Verify()) DETHROW(deeInvalidParam);
			// prepare rig
			rig->Prepare();
			// load system peers
			GetPhysicsSystem()->LoadRig(rig);
			// add rig
			pRigs.Add(rig);
//			LogInfoFormat( "Loading '%s' succeeded", path->GetPath() );
		}
		
	}catch(const deException &){
		LogErrorFormat("Loading rig '%s' (base path '%s') failed", filename, basePath ? basePath : "");
		if(fileReader){
			fileReader->FreeReference();
		}
		if(rig){
			rig->FreeReference();
		}
		throw;
	}
	
	if(rig) rig->SetAsynchron(false);
	return rig;
}

void deRigManager::AddLoadedRig(deRig *rig){
	if(! rig){
		DETHROW(deeInvalidParam);
	}
	
	pRigs.Add(rig);
}



void deRigManager::ReleaseLeakingResources(){
	const int count = GetRigCount();
	
	if(count > 0){
		deRig *rig = (deRig*)pRigs.GetRoot();
		
		LogWarnFormat("%i leaking rigs", count);
		
		while(rig){
			LogWarnFormat("- %s", rig->GetFilename().GetString());
			rig = (deRig*)rig->GetLLManagerNext();
		}
		
		pRigs.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deRigManager::SystemPhysicsLoad(){
	deRig *rig = (deRig*)pRigs.GetRoot();
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	
	while(rig){
		if(! rig->GetPeerPhysics()){
			phySys.LoadRig(rig);
		}
		
		rig = (deRig*)rig->GetLLManagerNext();
	}
}

void deRigManager::SystemPhysicsUnload(){
	deRig *rig = (deRig*)pRigs.GetRoot();
	
	while(rig){
		rig->SetPeerPhysics(NULL);
		rig = (deRig*)rig->GetLLManagerNext();
	}
}

void deRigManager::RemoveResource(deResource *resource){
	pRigs.RemoveIfPresent(resource);
}
