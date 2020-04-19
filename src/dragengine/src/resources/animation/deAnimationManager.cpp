/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

deAnimationManager::deAnimationManager( deEngine *engine ) :
deFileResourceManager( engine, ertAnimation ){
	SetLoggingName( "animation" );
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
	return ( deAnimation* )pAnimations.GetRoot();
}

deAnimation * deAnimationManager::GetAnimationWith( const char *filename ) const{
	return GetAnimationWith( GetEngine()->GetVirtualFileSystem(), filename );
}

deAnimation *deAnimationManager::GetAnimationWith( deVirtualFileSystem *vfs, const char *filename ) const{
	deAnimation * const animation = ( deAnimation* )pAnimations.GetWithFilename( vfs, filename );
	return animation && ! animation->GetOutdated() ? animation : NULL;
}

deAnimation *deAnimationManager::CreateAnimation( const char *filename, deAnimationBuilder &builder ){
	return CreateAnimation( GetEngine()->GetVirtualFileSystem(), filename, builder );
}

deAnimation *deAnimationManager::CreateAnimation( deVirtualFileSystem *vfs,
const char *filename, deAnimationBuilder &builder ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	deAnimation *anim=NULL, *findAnim;
	
	try{
		// check if the animation with this filename already exists
		findAnim = ( deAnimation* )pAnimations.GetWithFilename( vfs, filename );
		if( findAnim && ! findAnim->GetOutdated() ){
			DETHROW( deeInvalidParam );
		}
		
		// create animation using the builder
		anim = new deAnimation( this, vfs, filename, decDateTime::GetSystemTime() );
		builder.BuildAnimation( anim );
		
		// load system peers
		GetAnimatorSystem()->LoadAnimation( anim );
		
		// add animation
		pAnimations.Add( anim );
		
	}catch( const deException & ){
		LogErrorFormat( "Creating of animation '%s' failed", filename );
		if( anim ){
			anim->FreeReference();
		}
		throw;
	}
	
	return anim;
}

deAnimation *deAnimationManager::LoadAnimation( const char *filename, const char *basePath ){
	return LoadAnimation( GetEngine()->GetVirtualFileSystem(), filename, basePath );
}

deAnimation *deAnimationManager::LoadAnimation( deVirtualFileSystem *vfs,
const char *filename, const char *basePath ){
	decBaseFileReader *fileReader=NULL;
	deBaseAnimationModule *module;
	deAnimation *anim=NULL, *findAnim;
	decPath path;
	try{
		// locate file
		if( ! FindFileForReading( path, *vfs, filename, basePath ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime( path );
		
		// check if the animation with this filename already exists and is not outdated
		findAnim = ( deAnimation* )pAnimations.GetWithFilename( vfs, path.GetPathUnix() );
		
		if( findAnim && findAnim->GetModificationTime() != modificationTime ){
			LogInfoFormat( "Animation '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "" );
			findAnim->MarkOutdated();
			findAnim = NULL;
		}
		
		if( findAnim ){
			findAnim->AddReference();
			anim = findAnim;
			
		}else{
			// find the module able to handle this animation file
			module = ( deBaseAnimationModule* )GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtAnimation, path.GetPathUnix() );
			// load the file with it
			fileReader = OpenFileForReading( *vfs, path.GetPathUnix() );
			anim = new deAnimation( this, vfs, path.GetPathUnix(), modificationTime );
			anim->SetAsynchron( false );
			module->LoadAnimation( *fileReader, *anim );
			fileReader->FreeReference(); fileReader = NULL;
			
			// load system peers
			GetAnimatorSystem()->LoadAnimation( anim );
			
			// add animation
			pAnimations.Add( anim );
			//LogInfoFormat( "Loading '%s' succeeded", path->GetPathUnix() );
		}
		
	}catch( const deException & ){
		LogErrorFormat( "Loading animation '%s' (base path '%s') failed", filename, basePath ? basePath : "" );
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( anim ){
			anim->FreeReference();
		}
		throw;
	}
	
	if( anim ) anim->SetAsynchron( false );
	return anim;
}

void deAnimationManager::AddLoadedAnimation( deAnimation *animation ){
	if( ! animation ){
		DETHROW( deeInvalidParam );
	}
	
	pAnimations.Add( animation );
}



void deAnimationManager::ReleaseLeakingResources(){
	const int count = GetAnimationCount();
	
	if( count > 0 ){
		deAnimation *animation = ( deAnimation* )pAnimations.GetRoot();
		
		LogWarnFormat( "%i leaking animations", count );
		
		while( animation ){
			LogWarnFormat( "- %s", animation->GetFilename().GetString() );
			animation = ( deAnimation* )animation->GetLLManagerNext();
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
	deAnimation *animation = ( deAnimation* )pAnimations.GetRoot();
	deAnimatorSystem &aniSys = *GetAnimatorSystem();
	
	while( animation ){
		if( ! animation->GetPeerAnimator() ){
			aniSys.LoadAnimation( animation );
		}
		animation = ( deAnimation* )animation->GetLLManagerNext();
	}
}

void deAnimationManager::SystemAnimatorUnload(){
	deAnimation *animation = ( deAnimation* )pAnimations.GetRoot();
	
	while( animation ){
		animation->SetPeerAnimator( NULL );
		animation = ( deAnimation* )animation->GetLLManagerNext();
	}
}



void deAnimationManager::RemoveResource( deResource *resource ){
	pAnimations.RemoveIfPresent( resource );
}
