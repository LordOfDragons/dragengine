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
#include "deAnimation.h"
#include "deAnimationBuilder.h"
#include "deAnimationManager.h"
#include "../../deEngine.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/exceptions.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/deAnimatorSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/animation/deBaseAnimationModule.h"


// Class deAnimationManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimationManager::deAnimationManager(deEngine *engine) :
deFileResourceManager(engine, ertAnimation){
	SetLoggingName("animation");
}

deAnimationManager::~deAnimationManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deAnimationManager::GetAnimationCount() const{
	return pAnimations.GetCount();
}

deAnimation *deAnimationManager::GetRootAnimation() const{
	return (deAnimation*)pAnimations.GetRoot();
}

deAnimation * deAnimationManager::GetAnimationWith(const char *filename) const{
	return GetAnimationWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deAnimation *deAnimationManager::GetAnimationWith(deVirtualFileSystem *vfs, const char *filename) const{
	deAnimation * const animation = (deAnimation*)pAnimations.GetWithFilename(vfs, filename);
	return animation && !animation->GetOutdated() ? animation : NULL;
}

deAnimation::Ref deAnimationManager::CreateAnimation(const char *filename, deAnimationBuilder &builder){
	return CreateAnimation(GetEngine()->GetVirtualFileSystem(), filename, builder);
}

deAnimation::Ref deAnimationManager::CreateAnimation(deVirtualFileSystem *vfs,
const char *filename, deAnimationBuilder &builder){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	deAnimation::Ref anim;
	
	try{
		// check if animation with filename already exists. check is only done if
		// filename is not empty in which case an unnamed animation is created
		if(filename[0] != '\0'){
			deAnimation * const findAnim = (deAnimation*)pAnimations.GetWithFilename(vfs, filename);
			if(findAnim && !findAnim->GetOutdated()){
				DETHROW(deeInvalidParam);
			}
		}
		
		// create animation using the builder
		anim.TakeOver(new deAnimation(this, vfs, filename, decDateTime::GetSystemTime()));
		builder.BuildAnimation(anim);
		
		// load system peers
		GetAnimatorSystem()->LoadAnimation(anim);
		
		// add animation
		pAnimations.Add(anim);
		
	}catch(const deException &){
		LogErrorFormat("Creating of animation '%s' failed", filename);
		throw;
	}
	
	return anim;
}

deAnimation::Ref deAnimationManager::LoadAnimation(const char *filename, const char *basePath){
	return LoadAnimation(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deAnimation::Ref deAnimationManager::LoadAnimation(deVirtualFileSystem *vfs,
const char *filename, const char *basePath){
	deBaseAnimationModule *module;
	deAnimation::Ref anim;
	decPath path;
	try{
		// locate file
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		// check if the animation with this filename already exists and is not outdated
		deAnimation *findAnim = (deAnimation*)pAnimations.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findAnim && findAnim->GetModificationTime() != modificationTime){
			LogInfoFormat("Animation '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findAnim->MarkOutdated();
			findAnim = NULL;
		}
		
		if(findAnim){
			anim = findAnim;
			
		}else{
			// find the module able to handle this animation file
			module = (deBaseAnimationModule*)GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtAnimation, path.GetPathUnix());
			// load the file with it
			anim.TakeOver(new deAnimation(this, vfs, path.GetPathUnix(), modificationTime));
			anim->SetAsynchron(false);
			module->LoadAnimation(OpenFileForReading(*vfs, path.GetPathUnix()), *anim);
			
			// load system peers
			GetAnimatorSystem()->LoadAnimation(anim);
			
			// add animation
			pAnimations.Add(anim);
			//LogInfoFormat( "Loading '%s' succeeded", path->GetPathUnix() );
		}
		
	}catch(const deException &){
		LogErrorFormat("Loading animation '%s' (base path '%s') failed", filename, basePath ? basePath : "");
		throw;
	}
	
	if(anim) anim->SetAsynchron(false);
	return anim;
}

void deAnimationManager::AddLoadedAnimation(deAnimation *animation){
	if(!animation){
		DETHROW(deeInvalidParam);
	}
	
	pAnimations.Add(animation);
}

void deAnimationManager::SaveAnimation(const deAnimation &animation, const char *filename){
	SaveAnimation(animation, *GetEngine()->GetVirtualFileSystem(), filename);
}

void deAnimationManager::SaveAnimation(const deAnimation &animation,
deVirtualFileSystem &vfs, const char *filename){
	deBaseAnimationModule &module = *((deBaseAnimationModule*)
		GetModuleSystem()->GetModuleAbleToLoad(deModuleSystem::emtAnimation, filename));
	
	module.SaveAnimation(vfs.OpenFileForWriting(decPath::CreatePathUnix(filename)), animation);
}



void deAnimationManager::ReleaseLeakingResources(){
	const int count = GetAnimationCount();
	
	if(count > 0){
		deAnimation *animation = (deAnimation*)pAnimations.GetRoot();
		
		LogWarnFormat("%i leaking animations", count);
		
		while(animation){
			LogWarnFormat("- %s", animation->GetFilename().GetString());
			animation = (deAnimation*)animation->GetLLManagerNext();
		}
		
		pAnimations.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deAnimationManager::SystemGraphicLoad(){
	/*
	deGraphicSystem *graSys = GetGraphicSystem();
	deAnimation *anim;
	for(int i=0; i<pAnims.GetResourceCount(); i++){
		anim = (deAnimation*)pAnims.GetResourceAt(i);
		if(anim->GetGraphicAnimation()) continue;
		graSys->LoadSkin(anim);
	}
	*/
}

void deAnimationManager::SystemGraphicUnload(){
	/*
	for(int i=0; i<pAnims.GetResourceCount(); i++){
		((deAnimation*)pAnims.GetResourceAt(i))->SetGraphicAnimation(NULL);
	}
	*/
}

void deAnimationManager::SystemAnimatorLoad(){
	deAnimation *animation = (deAnimation*)pAnimations.GetRoot();
	deAnimatorSystem &aniSys = *GetAnimatorSystem();
	
	while(animation){
		if(!animation->GetPeerAnimator()){
			aniSys.LoadAnimation(animation);
		}
		animation = (deAnimation*)animation->GetLLManagerNext();
	}
}

void deAnimationManager::SystemAnimatorUnload(){
	deAnimation *animation = (deAnimation*)pAnimations.GetRoot();
	
	while(animation){
		animation->SetPeerAnimator(NULL);
		animation = (deAnimation*)animation->GetLLManagerNext();
	}
}



void deAnimationManager::RemoveResource(deResource *resource){
	pAnimations.RemoveIfPresent(resource);
}
