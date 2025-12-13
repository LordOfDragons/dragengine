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

#include "deOcclusionMesh.h"
#include "deOcclusionMeshManager.h"
#include "deOcclusionMeshBuilder.h"

#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decPath.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/occlusionmesh/deBaseOcclusionMeshModule.h"
#include "../../systems/modules/graphic/deBaseGraphicOcclusionMesh.h"



// Class deOcclusionMeshManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deOcclusionMeshManager::deOcclusionMeshManager(deEngine *engine) :
deFileResourceManager(engine, ertOcclusionMesh){
	SetLoggingName("occlusion mesh");
}

deOcclusionMeshManager::~deOcclusionMeshManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deOcclusionMeshManager::GetOcclusionMeshCount() const{
	return pMeshes.GetCount();
}

deOcclusionMesh *deOcclusionMeshManager::GetRootOcclusionMesh() const{
	return (deOcclusionMesh*)pMeshes.GetRoot();
}

deOcclusionMesh *deOcclusionMeshManager::GetOcclusionMeshWith(const char *filename) const{
	return GetOcclusionMeshWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deOcclusionMesh *deOcclusionMeshManager::GetOcclusionMeshWith(
deVirtualFileSystem *vfs, const char *filename) const{
	deOcclusionMesh * const occmesh = (deOcclusionMesh*)pMeshes.GetWithFilename(vfs, filename);
	return occmesh && !occmesh->GetOutdated() ? occmesh : NULL;
}

deOcclusionMesh::Ref deOcclusionMeshManager::CreateOcclusionMesh(
const char *filename, deOcclusionMeshBuilder &builder){
	return CreateOcclusionMesh(GetEngine()->GetVirtualFileSystem(), filename, builder);
}

deOcclusionMesh::Ref deOcclusionMeshManager::CreateOcclusionMesh(deVirtualFileSystem *vfs,
const char *filename, deOcclusionMeshBuilder &builder){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	deOcclusionMesh::Ref occmesh;
	
	try{
		// check if an occlusion mesh with this filename already exists. this check is only done if
		// the filename is not empty in which case an unnamed occlusion mesh is created
		if(filename[0] != '\0'){
			deOcclusionMesh * const findOccMesh = (deOcclusionMesh*)pMeshes.GetWithFilename(vfs, filename);
			if(findOccMesh && !findOccMesh->GetOutdated()){
				DETHROW(deeInvalidParam);
			}
		}
		
		// create occlusion mesh using the builder
		occmesh = deOcclusionMesh::Ref::New(this, vfs, filename, decDateTime::GetSystemTime());
		builder.BuildOcclusionMesh(occmesh);
		
		// prepare and check occlusion mesh
		if(!occmesh->Verify()){
			DETHROW(deeInvalidParam);
		}
		occmesh->Prepare();
		
		// load system peers
		GetGraphicSystem()->LoadOcclusionMesh(occmesh);
		
		// add occlusion mesh
		pMeshes.Add(occmesh);
		
	}catch(const deException &e){
		LogErrorFormat("Creating occlusion mesh '%s' failed", filename);
		LogException(e);
		throw;
	}
	
	return occmesh;
}

deOcclusionMesh::Ref deOcclusionMeshManager::LoadOcclusionMesh(const char *filename, const char *basePath){
	return LoadOcclusionMesh(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deOcclusionMesh::Ref deOcclusionMeshManager::LoadOcclusionMesh(deVirtualFileSystem *vfs,
const char *filename, const char *basePath){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	deOcclusionMesh::Ref occmesh;
	deBaseOcclusionMeshModule *module;
	decPath path;
	
	try{
		// locate file
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		// check if the model with this filename already exists
		deOcclusionMesh *findOccMesh = (deOcclusionMesh*)
			pMeshes.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findOccMesh && findOccMesh->GetModificationTime() != modificationTime){
			LogInfoFormat("Occlusion Mesh '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findOccMesh->MarkOutdated();
			findOccMesh = NULL;
		}
		
		if(findOccMesh){
			occmesh = findOccMesh;
			
		}else{
			// find the module able to handle this model file
			module = (deBaseOcclusionMeshModule*)GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtOcclusionMesh, path.GetPathUnix());
			
			// load the file with it
			occmesh = deOcclusionMesh::Ref::New(this, vfs, path.GetPathUnix(), modificationTime);
			
			occmesh->SetAsynchron(false);
			module->LoadOcclusionMesh(OpenFileForReading(*vfs, path.GetPathUnix()), *occmesh);
			
			// prepare and check model
			if(!occmesh->Verify()){
				DETHROW(deeInvalidParam);
			}
			occmesh->Prepare();
			
			// load system peers
			GetGraphicSystem()->LoadOcclusionMesh(occmesh);
			
			// add model
			pMeshes.Add(occmesh);
		}
		
	}catch(const deException &e){
		LogErrorFormat("Loading occlusion mesh '%s' (base path '%s') failed", filename, basePath ? basePath : "");
		LogException(e);
		throw;
	}
	
	if(occmesh){
		occmesh->SetAsynchron(false);
	}
	
	return occmesh;
}



void deOcclusionMeshManager::AddLoadedOcclusionMesh(deOcclusionMesh *occmesh){
	if(!occmesh){
		DETHROW(deeInvalidParam);
	}
	
	pMeshes.Add(occmesh);
}



void deOcclusionMeshManager::ReleaseLeakingResources(){
	const int count = GetOcclusionMeshCount();
	
	if(count > 0){
		deOcclusionMesh *occlusionMesh = (deOcclusionMesh*)pMeshes.GetRoot();
		int unnamedCount = 0;
		
		LogWarnFormat("%i leaking occlusion meshes", count);
		
		while(occlusionMesh){
			if(occlusionMesh->GetFilename().IsEmpty()){
				unnamedCount++;
				
			}else{
				LogWarnFormat("- %s", occlusionMesh->GetFilename().GetString());
			}
			
			occlusionMesh = (deOcclusionMesh*)occlusionMesh->GetLLManagerNext();
		}
		
		if(unnamedCount > 0){
			LogWarnFormat("%i unnamed occlusion meshes", unnamedCount);
		}
		
		pMeshes.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deOcclusionMeshManager::SystemGraphicLoad(){
	deOcclusionMesh *occmesh = (deOcclusionMesh*)pMeshes.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(occmesh){
		if(!occmesh->GetPeerGraphic()){
			grasys.LoadOcclusionMesh(occmesh);
		}
		
		occmesh = (deOcclusionMesh*)occmesh->GetLLManagerNext();
	}
}

void deOcclusionMeshManager::SystemGraphicUnload(){
	deOcclusionMesh *occmesh = (deOcclusionMesh*)pMeshes.GetRoot();
	
	while(occmesh){
		occmesh->SetPeerGraphic(NULL);
		occmesh = (deOcclusionMesh*)occmesh->GetLLManagerNext();
	}
}



void deOcclusionMeshManager::RemoveResource(deResource *resource){
	pMeshes.RemoveIfPresent(resource);
}
