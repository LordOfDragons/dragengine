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

#include "deRLTaskReadAnimation.h"
#include "../deResourceLoader.h"
#include "../../animation/deAnimation.h"
#include "../../animation/deAnimationManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deAnimatorSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/animation/deBaseAnimationModule.h"



// Class deRLTaskReadAnimation
////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadAnimation::deRLTaskReadAnimation( deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deAnimation *animation ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertAnimation ),
pSucceeded( false )
{
	LogCreateEnter();
	// if already loaded set finished
	if( animation ){
		pAnimation = animation;
		SetResource( animation );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pAnimation.TakeOver( new deAnimation( engine.GetAnimationManager(), vfs, path, 0 ) );
	LogCreateExit();
}

deRLTaskReadAnimation::~deRLTaskReadAnimation(){
}



// Management
///////////////

void deRLTaskReadAnimation::Run(){
	LogRunEnter();
	deBaseAnimationModule * const module = ( deBaseAnimationModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtAnimation, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
	pAnimation->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pAnimation->SetAsynchron( true );
	module->LoadAnimation( reader, pAnimation );
	reader = NULL;
	
	GetEngine().GetAnimatorSystem()->LoadAnimation( pAnimation );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadAnimation::Finished(){
	LogFinishedEnter();
	if( ! pSucceeded ){
		SetState( esFailed );
		pAnimation = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deAnimationManager &animationManager = *GetEngine().GetAnimationManager();
	deAnimation * const checkAnimation = animationManager.GetAnimationWith( GetPath() );
	
	if( checkAnimation ){
		SetResource( checkAnimation );
		
	}else{
		pAnimation->SetAsynchron( false );
		animationManager.AddLoadedAnimation( pAnimation );
		SetResource( pAnimation );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadAnimation::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Anim-Read";
}
