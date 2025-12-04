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

#include "deRLTaskReadAnimation.h"
#include "../deResourceLoader.h"
#include "../../animation/deAnimation.h"
#include "../../animation/deAnimationManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReader.h"
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
	
	decBaseFileReader::Ref reader;
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
