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

#include "deRLTaskReadVideo.h"
#include "../deResourceLoader.h"
#include "../../video/deVideo.h"
#include "../../video/deVideoManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/video/deBaseVideoInfo.h"
#include "../../../systems/modules/video/deBaseVideoModule.h"



// Class deRLTaskReadVideo
////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadVideo::deRLTaskReadVideo( deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deVideo *video ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertVideo ),
pSucceeded( false )
{
	LogCreateEnter();
	
	// if already loaded set finished
	if( video ){
		pVideo = video;
		SetResource( video );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pVideo.TakeOver( new deVideo( engine.GetVideoManager(), vfs, path, 0 ) );
	
	LogCreateExit();
}

deRLTaskReadVideo::~deRLTaskReadVideo(){
}



// Management
///////////////

void deRLTaskReadVideo::Run(){
	LogRunEnter();
	
	deBaseVideoModule * const module = ( deBaseVideoModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtVideo, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReader::Ref reader(decBaseFileReader::Ref::New(
		 GetVFS()->OpenFileForReading( vfsPath ) ));
	
	deBaseVideoInfo videoInfo;
	module->InitLoadVideo( reader, videoInfo );
	reader = NULL;
	
	pVideo->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pVideo->SetAsynchron( true );
	pVideo->FinalizeConstruction( videoInfo.GetWidth(), videoInfo.GetHeight(),
		videoInfo.GetComponentCount(), videoInfo.GetBitCount(), videoInfo.GetFrameRate(),
		videoInfo.GetFrameCount(), videoInfo.GetColorConversionMatrix(),
		videoInfo.GetBytesPerSample(), videoInfo.GetSampleCount(),
		videoInfo.GetSampleRate(), videoInfo.GetChannelCount() );
	
	// create peers. modules can request to load the video data if small enough
	GetEngine().GetGraphicSystem()->LoadVideo( pVideo );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadVideo::Finished(){
	LogFinishedEnter();
	
	if( ! pSucceeded ){
		SetState( esFailed );
		pVideo = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deVideoManager &videoManager = *GetEngine().GetVideoManager();
	deVideo * const checkVideo = videoManager.GetVideoWith( GetPath() );
	
	if( checkVideo ){
		SetResource( checkVideo );
		
	}else{
		pVideo->SetAsynchron( false );
		videoManager.AddLoadedVideo( pVideo );
		SetResource( pVideo );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadVideo::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Video-Read";
}
