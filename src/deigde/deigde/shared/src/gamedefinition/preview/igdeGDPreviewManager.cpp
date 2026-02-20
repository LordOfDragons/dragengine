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

#include "igdeGDPreviewListener.h"
#include "igdeGDPreviewManager.h"
#include "creators/igdeGDPCObjectClass.h"
#include "creators/igdeGDPCSkin.h"
#include "creators/igdeGDPCSky.h"
#include "../igdeGameDefinition.h"
#include "../class/igdeGDClass.h"
#include "../skin/igdeGDSkin.h"
#include "../sky/igdeGDSky.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/igdeEngineController.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"



// Class igdeGDPreviewManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPreviewManager::igdeGDPreviewManager(igdeEnvironment &environment) :
pEnvironment(environment),
pPathCache("/igde/cache/preview"),
pImageSize(256), // large=128, medium=96, small=64
pHasCreators(false){
}

igdeGDPreviewManager::~igdeGDPreviewManager(){
	OnBeforeEngineStop();
}



// Management
///////////////

void igdeGDPreviewManager::SetPathCache(const char *path){
	pPathCache = path;
}

void igdeGDPreviewManager::SetDirectoryObjectClass(const char *directory){
	pDirObjectClass = directory;
}

void igdeGDPreviewManager::SetDirectorySkin(const char *directory){
	pDirSkin = directory;
}

void igdeGDPreviewManager::SetDirectorySky(const char *directory){
	pDirSky = directory;
}



void igdeGDPreviewManager::OnAfterEngineStart(){
	DEASSERT_NULL(pImageCreating)
	DEASSERT_NULL(pImageFailed)
	
	deImageManager &imgmgr = *pEnvironment.GetEngineController()->GetEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemIGDE();
	
	try{
		pImageCreating = imgmgr.LoadImage(vfs, "/data/data/models/previewBuilder/rendering.png", "/");
		pImageFailed = imgmgr.LoadImage(vfs, "/data/data/models/previewBuilder/failed.png", "/");
		
	}catch(const deException &){
		OnBeforeEngineStop();
		throw;
	}
}

void igdeGDPreviewManager::OnBeforeEngineStop(){
	AbortAllCreators();
}

void igdeGDPreviewManager::Update(){
	if(!pHasCreators){
		return;
	}
	
	// update object classes
	pCreatorsObjectClass.RemoveIf([&](igdeGDPCObjectClass &creator){
		if(!creator.FinishCreation()){
			return false;
		}
		
		if(creator.GetImage()){
			creator.GetGDClass()->SetPreviewImage(creator.GetImage());
			pSaveToFile(creator.GetImage(), pDirObjectClass, creator.GetGDClass()->GetName());
			
		}else{
			creator.GetGDClass()->SetPreviewImage(pImageFailed);
		}
		return true;
	});
	
	// update skins
	pCreatorsSkin.RemoveIf([&](igdeGDPCSkin &creator){
		if(!creator.FinishCreation()){
			return false;
		}
		
		if(creator.GetImage()){
			creator.GetGDSkin()->SetPreviewImage(creator.GetImage());
			pSaveToFile(creator.GetImage(), pDirSkin, creator.GetGDSkin()->GetName());
			
		}else{
			creator.GetGDSkin()->SetPreviewImage(pImageFailed);
		}
		return true;
	});
	
	// update skies
	pCreatorsSky.RemoveIf([&](igdeGDPCSky &creator){
		if(!creator.FinishCreation()){
			return false;
		}
		
		if(creator.GetImage()){
			creator.GetGDSky()->SetPreviewImage(creator.GetImage());
			pSaveToFile(creator.GetImage(), pDirSky, creator.GetGDSky()->GetName());
			
		}else{
			creator.GetGDSky()->SetPreviewImage(pImageFailed);
		}
		return true;
	});
	
	// check if still some creators are left
	pHasCreators = pCreatorsObjectClass.IsNotEmpty()
		|| pCreatorsSkin.IsNotEmpty()
		|| pCreatorsSky.IsNotEmpty();
}

void igdeGDPreviewManager::AbortAllCreators(){
	if(!pHasCreators){
		return;
	}
	
	// update object classes
	pCreatorsObjectClass.Visit([&](igdeGDPCObjectClass &c){
		c.AbortCreation();
	});
	pCreatorsObjectClass.RemoveAll();
	
	// update skins
	pCreatorsSkin.Visit([&](igdeGDPCSkin &c){
		c.AbortCreation();
	});
	pCreatorsSkin.RemoveAll();
	
	// update skies
	pCreatorsSky.Visit([&](igdeGDPCSky &c){
		c.AbortCreation();
	});
	pCreatorsSky.RemoveAll();
	
	pHasCreators = false;
}



deImage *igdeGDPreviewManager::GetPreviewObjectClass(igdeGDClass *gdclass) const{
	DEASSERT_NOTNULL(gdclass)
	
	if(gdclass->GetPreviewImage()){
		return gdclass->GetPreviewImage();
	}
	
	deImage::Ref image;
	pLoadFromFile(image, pDirObjectClass, gdclass->GetName());
	gdclass->SetPreviewImage(image);
	return image;
}

void igdeGDPreviewManager::CreatePreviewObjectClass(igdeGDClass *gdclass, igdeGDPreviewListener *listener){
	DEASSERT_NOTNULL(gdclass)
	DEASSERT_NOTNULL(listener)
	
	igdeGDPCObjectClass * const found = pCreatorsObjectClass.FindOrDefault([&](const igdeGDPCObjectClass &c){
		return c.GetGDClass() == gdclass;
	});
	if(found){
		found->AddListener(listener);
		return;
	}
	
	const igdeGDPCObjectClass::Ref creator(igdeGDPCObjectClass::Ref::New(
		pEnvironment, gdclass, decPoint(pImageSize, pImageSize)));
	creator->AddListener(listener);
	creator->BeginCreation();
	pCreatorsObjectClass.Add(creator);
	
	pHasCreators = true;
}

void igdeGDPreviewManager::ClearPreviewObjectClass(igdeGDClass *gdclass){
	DEASSERT_NOTNULL(gdclass)
	
	gdclass->SetPreviewImage(nullptr);
	
	pDeleteFile(pDirObjectClass, gdclass->GetName());
}



deImage *igdeGDPreviewManager::GetPreviewSkin(igdeGDSkin *gdskin) const{
	DEASSERT_NOTNULL(gdskin)
	
	if(gdskin->GetPreviewImage()){
		return gdskin->GetPreviewImage();
	}
	
	deImage::Ref image;
	pLoadFromFile(image, pDirSkin, gdskin->GetName());
	gdskin->SetPreviewImage(image);
	return image;
}

void igdeGDPreviewManager::CreatePreviewSkin(igdeGDSkin *gdskin, igdeGDPreviewListener *listener){
	DEASSERT_NOTNULL(gdskin)
	DEASSERT_NOTNULL(listener)
	
	igdeGDPCSkin * const found = pCreatorsSkin.FindOrDefault([&](const igdeGDPCSkin &c){
		return c.GetGDSkin() == gdskin;
	});
	if(found){
		found->AddListener(listener);
		return;
	}
	
	const igdeGDPCSkin::Ref creator(igdeGDPCSkin::Ref::New(
		pEnvironment, gdskin, decPoint(pImageSize, pImageSize)));
	creator->AddListener(listener);
	creator->BeginCreation();
	pCreatorsSkin.Add(creator);
	
	pHasCreators = true;
}

void igdeGDPreviewManager::ClearPreviewSkin(igdeGDSkin *gdskin){
	DEASSERT_NOTNULL(gdskin)
	
	gdskin->SetPreviewImage(nullptr);
	
	pDeleteFile(pDirSkin, gdskin->GetName());
}



deImage *igdeGDPreviewManager::GetPreviewSky(igdeGDSky *gdsky) const{
	DEASSERT_NOTNULL(gdsky)
	
	if(gdsky->GetPreviewImage()){
		return gdsky->GetPreviewImage();
	}
	
	deImage::Ref image;
	pLoadFromFile(image, pDirSky, gdsky->GetName());
	gdsky->SetPreviewImage(image);
	return image;
}

void igdeGDPreviewManager::CreatePreviewSky(igdeGDSky *gdsky, igdeGDPreviewListener *listener){
	DEASSERT_NOTNULL(gdsky)
	DEASSERT_NOTNULL(listener)
	
	igdeGDPCSky * const found = pCreatorsSky.FindOrDefault([&](const igdeGDPCSky &c){
		return c.GetGDSky() == gdsky;
	});
	if(found){
		found->AddListener(listener);
		return;
	}
	
	const igdeGDPCSky::Ref creator(igdeGDPCSky::Ref::New(
		pEnvironment, gdsky, decPoint(pImageSize, pImageSize)));
	creator->AddListener(listener);
	creator->BeginCreation();
	pCreatorsSky.Add(creator);
	
	pHasCreators = true;
}

void igdeGDPreviewManager::ClearPreviewSky(igdeGDSky *gdsky){
	DEASSERT_NOTNULL(gdsky)
	
	gdsky->SetPreviewImage(nullptr);
	
	pDeleteFile(pDirSky, gdsky->GetName());
}



// Private Functions
//////////////////////

void igdeGDPreviewManager::pLoadFromFile(deImage::Ref &image, const decString &typedir, const decString &filename) const{
	image = nullptr;
	
	decPath path;
	path.SetFromUnix(pPathCache);
	path.AddComponent(typedir);
	path.AddComponent(filename + ".png");
	
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemGame();
	if(!vfs->ExistsFile(path)){
		return;
	}
	
	try{
		image = pEnvironment.GetEngineController()->GetEngine()->GetImageManager()->LoadImage(vfs, path.GetPathUnix(), "/");
		if(image->GetBitCount() != 8){
			image = nullptr;
		}
		
	}catch(const deException &e){
		e.PrintError();
	}
}

void igdeGDPreviewManager::pSaveToFile(deImage *image, const decString &typedir, const decString &filename) const{
	DEASSERT_NOTNULL(image)
	
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemGame();
	decPath path;
	path.SetFromUnix(pPathCache);
	path.AddComponent(typedir);
	path.AddComponent(filename + ".png");
	
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	try{
		engine.GetImageManager()->SaveImage(vfs, image, path.GetPathUnix());
		
	}catch(const deException &){
		pEnvironment.GetLogger()->LogWarnFormat(LOGSOURCE,
			"Can not save preview image to cache file '%s'", path.GetPathUnix().GetString());
	}
}

void igdeGDPreviewManager::pDeleteFile(const decString &typedir, const decString &filename) const{
	decPath path;
	path.SetFromUnix(pPathCache);
	path.AddComponent(typedir);
	path.AddComponent(filename + ".png");
	
	deVirtualFileSystem * const vfs = pEnvironment.GetFileSystemGame();
	if(!vfs->ExistsFile(path)){
		return;
	}
	
	try{
		vfs->DeleteFile(path);
		
	}catch(const deException &e){
		e.PrintError();
	}
}
