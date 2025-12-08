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

#include "deSkin.h"
#include "deSkinBuilder.h"
#include "deSkinManager.h"
#include "deSkinTexture.h"
#include "property/deSkinPropertyImage.h"
#include "property/deSkinPropertyVideo.h"
#include "property/deSkinPropertyVisitorLoad.h"
#include "../image/deImage.h"
#include "../image/deImageManager.h"
#include "../loader/deResourceLoader.h"
#include "../video/deVideoManager.h"
#include "../video/deVideo.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/skin/deBaseSkinModule.h"



// definitions
#define SKIN_NO_SKIN	"<no_skin>"



// Class deSkinManager
////////////////////////

// Constructor, destructor
////////////////////////////

deSkinManager::deSkinManager(deEngine *engine) : deFileResourceManager(engine, ertSkin){
	SetLoggingName("skin");
}

deSkinManager::~deSkinManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSkinManager::GetSkinCount() const{
	return pSkins.GetCount();
}

deSkin *deSkinManager::GetRootSkin() const{
	return (deSkin*)pSkins.GetRoot();
}

deSkin *deSkinManager::GetSkinWith(const char *filename) const{
	return GetSkinWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deSkin *deSkinManager::GetSkinWith(deVirtualFileSystem *vfs, const char *filename) const{
	deSkin * const skin = (deSkin*)pSkins.GetWithFilename(vfs, filename);
	return skin && !skin->GetOutdated() ? skin : NULL;
}

deSkin *deSkinManager::CreateSkin(const char *filename, deSkinBuilder &builder){
	return CreateSkin(GetEngine()->GetVirtualFileSystem(), filename, builder);
}

deSkin *deSkinManager::CreateSkin(deVirtualFileSystem *vfs, const char *filename, deSkinBuilder &builder){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	
	deSkin::Ref skin = NULL;
	deSkin::Ref findSkin;
	
	try{
		// check if the skin with this filename already exists. skins with a zero
		// length file name are considered unique and match no filename
		if(strlen(filename) > 0){
			findSkin = (deSkin*)pSkins.GetWithFilename(vfs, filename);
			if(findSkin && !findSkin->GetOutdated()){
				DETHROW(deeInvalidParam);
			}
		}
		
		// create skin using the builder
		skin.TakeOver(new deSkin(this, vfs, filename, decDateTime::GetSystemTime()));
		builder.BuildSkin(skin);
		
		// load into graphic system
		GetGraphicSystem()->LoadSkin(skin);
		GetAudioSystem()->LoadSkin(skin);
		GetPhysicsSystem()->LoadSkin(skin);
		
		// add skination
		pSkins.Add(skin);
		
	}catch(const deException &){
		LogErrorFormat("Creating skin '%s' failed", filename);
		throw;
	}
	
	return skin;
}

deSkin *deSkinManager::LoadSkin(const char *filename, const char *basePath){
	return LoadSkin(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deSkin *deSkinManager::LoadSkin(deVirtualFileSystem *vfs, const char *filename, const char *basePath){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	decBaseFileReader::Ref fileReader = NULL;
	deBaseSkinModule *module;
	decPath path;
	deSkin::Ref skin = NULL;
	deSkin::Ref findSkin;
	
	try{
		// locate file
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			//return LoadDefault();
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		// check if the skin with this filename already exists
		findSkin = (deSkin*)pSkins.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findSkin && findSkin->GetModificationTime() != modificationTime){
			LogInfoFormat("Skin '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findSkin->MarkOutdated();
			findSkin = NULL;
		}
		
		if(findSkin){
			skin = findSkin;
			
		}else{
			// find the module able to handle this model file
			module = (deBaseSkinModule*)GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtSkin, path.GetPathUnix());
			
			// load the file with it
			fileReader = OpenFileForReading(*vfs, path.GetPathUnix());
			
			skin.TakeOver(new deSkin(this, vfs, path.GetPathUnix(), modificationTime));
			skin->SetAsynchron(false);
			
			module->LoadSkin(*fileReader, *skin);
			fileReader = NULL;
			
			// load resources in properties
			LoadPropertyResources(*skin);
			
			// load into graphic system
			GetGraphicSystem()->LoadSkin(skin);
			GetAudioSystem()->LoadSkin(skin);
			GetPhysicsSystem()->LoadSkin(skin);
			
			// add model
			pSkins.Add(skin);
//			LogInfoFormat( "Loading '%s' succeeded", path->GetPath() );
		}
		
	}catch(const deException &){
		LogErrorFormat("Loading skin '%s' (base path '%s') failed", filename, basePath ? basePath : "");
		throw;
	}
	
	if(skin){
		skin->SetAsynchron(false); // just to make sure it is marked async no matter who might have messed with it
	}
	return skin;
}

deSkin *deSkinManager::LoadDefault(){
	deSkin::Ref skin = NULL;
	deSkin::Ref findSkin = NULL;
	deSkinTexture * skinTex = NULL;
	deSkinPropertyImage * propDiff = NULL;
	deImage::Ref imgNoTex;
	
	try{
		findSkin = (deSkin*)pSkins.GetWithFilename(GetEngine()->GetVirtualFileSystem(), SKIN_NO_SKIN);
		
		if(findSkin){
			if(!findSkin->GetPeerGraphic()){
				GetGraphicSystem()->LoadSkin(findSkin);
			}
			skin = findSkin;
			
		}else{
			// fetch default image
			imgNoTex.TakeOver(GetImageManager()->LoadDefault());
			
			// load skin
			skin.TakeOver(new deSkin(this, GetEngine()->GetVirtualFileSystem(),
				SKIN_NO_SKIN, decDateTime::GetSystemTime()));
			skinTex = new deSkinTexture("skin");
			propDiff = new deSkinPropertyImage("color");
			
			propDiff->SetImage(imgNoTex); // attention!+1 reference
			
			skinTex->AddProperty(propDiff);
			propDiff = NULL;
			
			skin->AddTexture(skinTex);
			skinTex = NULL;
			
			// load into graphic system
			GetGraphicSystem()->LoadSkin(skin);
			GetAudioSystem()->LoadSkin(skin);
			GetPhysicsSystem()->LoadSkin(skin);
			
			// add skin
			pSkins.Add(skin);
		}
		
	}catch(const deException &){
		if(propDiff){
			delete propDiff;
		}
		if(skinTex){
			delete skinTex;
		}
		throw;
	}
	
	return skin;
}



void deSkinManager::AddLoadedSkin(deSkin *skin){
	if(!skin){
		DETHROW(deeInvalidParam);
	}
	
	pSkins.Add(skin);
}



void deSkinManager::LoadPropertyResources(deSkin &skin){
	const int textureCount = skin.GetTextureCount();
	int i, j;
	
	decPath basePath;
	basePath.SetFromUnix(skin.GetFilename());
	basePath.RemoveLastComponent();
	basePath.SetPrefix("/");
	
	const decString strBasePath(basePath.GetPathUnix());
	deSkinPropertyVisitorLoad visitor(*GetEngine(), skin.GetVirtualFileSystem(), strBasePath);
	
	for(i=0; i<textureCount; i++){
		const deSkinTexture &texture = *skin.GetTextureAt(i);
		const int propertyCount = texture.GetPropertyCount();
		
		for(j=0; j<propertyCount; j++){
			texture.GetPropertyAt(j)->Visit(visitor);
		}
	}
}



void deSkinManager::ReleaseLeakingResources(){
	const int count = GetSkinCount();
	
	if(count > 0){
		deSkin::Ref skin = (deSkin*)pSkins.GetRoot();
		
		LogWarnFormat("%i leaking skins", count);
		
		while(skin){
			LogWarnFormat("- %s", skin->GetFilename().GetString());
			skin = (deSkin*)skin->GetLLManagerNext();
		}
		
		pSkins.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSkinManager::SystemGraphicLoad(){
	deSkin::Ref skin = (deSkin*)pSkins.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(skin){
		if(!skin->GetPeerGraphic()){
			grasys.LoadSkin(skin);
		}
		
		skin = (deSkin*)skin->GetLLManagerNext();
	}
}

void deSkinManager::SystemGraphicUnload(){
	deSkin::Ref skin = (deSkin*)pSkins.GetRoot();
	
	while(skin){
		skin->SetPeerGraphic(NULL);
		skin = (deSkin*)skin->GetLLManagerNext();
	}
}

void deSkinManager::SystemAudioLoad(){
	deSkin::Ref skin = (deSkin*)pSkins.GetRoot();
	deAudioSystem &audsys = *GetAudioSystem();
	
	while(skin){
		if(!skin->GetPeerAudio()){
			audsys.LoadSkin(skin);
		}
		
		skin = (deSkin*)skin->GetLLManagerNext();
	}
}

void deSkinManager::SystemAudioUnload(){
	deSkin::Ref skin = (deSkin*)pSkins.GetRoot();
	
	while(skin){
		skin->SetPeerAudio(NULL);
		skin = (deSkin*)skin->GetLLManagerNext();
	}
}

void deSkinManager::SystemPhysicsLoad(){
	deSkin::Ref skin = (deSkin*)pSkins.GetRoot();
	dePhysicsSystem &physys = *GetPhysicsSystem();
	
	while(skin){
		if(!skin->GetPeerPhysics()){
			physys.LoadSkin(skin);
		}
		
		skin = (deSkin*)skin->GetLLManagerNext();
	}
}

void deSkinManager::SystemPhysicsUnload(){
	deSkin::Ref skin = (deSkin*)pSkins.GetRoot();
	
	while(skin){
		skin->SetPeerPhysics(NULL);
		skin = (deSkin*)skin->GetLLManagerNext();
	}
}

void deSkinManager::RemoveResource(deResource *resource){
	pSkins.RemoveIfPresent(resource);
}
