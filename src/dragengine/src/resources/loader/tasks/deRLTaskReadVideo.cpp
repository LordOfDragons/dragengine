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

#include "deRLTaskReadVideo.h"
#include "../deResourceLoader.h"
#include "../../video/deVideo.h"
#include "../../video/deVideoManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
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
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
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
