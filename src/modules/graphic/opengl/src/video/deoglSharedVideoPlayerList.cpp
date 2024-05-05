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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglSharedVideoPlayer.h"
#include "deoglSharedVideoPlayerList.h"
#include "deoglVideoPlayer.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoPlayerManager.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoglSharedVideoPlayerList
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedVideoPlayerList::deoglSharedVideoPlayerList( deGraphicOpenGl &ogl ) :
pOgl( ogl ){
}

deoglSharedVideoPlayerList::~deoglSharedVideoPlayerList(){
	while( pList.GetCount() > 0 ){
		delete ( deoglSharedVideoPlayer* )pList.GetAt( pList.GetCount() - 1 );
		pList.RemoveFrom( pList.GetCount() - 1 );
	}
}



// Management
///////////////

int deoglSharedVideoPlayerList::GetCount() const{
	return pList.GetCount();
}

deoglSharedVideoPlayer *deoglSharedVideoPlayerList::GetVideoPlayerFor(
deVideo *video, int currentFrame, float playbackSpeed ){
	if( ! video ){
		DETHROW( deeInvalidParam );
	}
	
	// find matching video
	const int count = pList.GetCount();
	int i;
	
	/*
	pOgl.LogInfoFormat( "Get shared video player: video=%s frame=%i speed=%g",
		video->GetFilename(), currentFrame, playbackSpeed );
	for( i=0; i<count; i++ ){
		deoglSharedVideoPlayer * const sharedVideoPlayer = ( deoglSharedVideoPlayer* )pList.GetAt( i );
		const deVideoPlayer &engPlayer = *sharedVideoPlayer->GetVideoPlayer();
		const deoglVideoPlayer &player = *( ( deoglVideoPlayer* )engPlayer.GetPeerGraphic() );
		pOgl.LogInfoFormat( "- video=%s frame=%i speed=%g usage=%i",
			engPlayer.GetVideo()->GetFilename(), player.GetCurrentFrame(), engPlayer.GetPlaySpeed(),
			sharedVideoPlayer->GetUsageCount() );
	}
	*/
	
	for( i=0; i<count; i++ ){
		deoglSharedVideoPlayer * const sharedVideoPlayer = ( deoglSharedVideoPlayer* )pList.GetAt( i );
		const deVideoPlayer &engPlayer = *sharedVideoPlayer->GetVideoPlayer();
		const deoglVideoPlayer &player = *( ( deoglVideoPlayer* )engPlayer.GetPeerGraphic() );
		
		if( engPlayer.GetVideo() == video
		&& player.GetCurrentFrame() == currentFrame
		&& fabsf( engPlayer.GetPlaySpeed() - playbackSpeed ) <= 0.001f ){
			sharedVideoPlayer->AddUsage();
			//pOgl.LogInfoFormat( "Use shared video player: video=%s frame=%i speed=%g usage=%i",
			//	video->GetFilename(), currentFrame, playbackSpeed, sharedVideoPlayer->GetUsageCount() );
			return sharedVideoPlayer;
		}
	}
	
	// no match found. add a new shared video player
	deoglSharedVideoPlayer *sharedVideoPlayer = NULL;
	deVideoPlayer *videoPlayer = NULL;
	
	try{
		videoPlayer = pOgl.GetGameEngine()->GetVideoPlayerManager()->CreateVideoPlayer();
		videoPlayer->SetVideo( video );
		videoPlayer->SetLooping( true );
		videoPlayer->SetPlaySpeed( playbackSpeed );
		videoPlayer->SetPlayPosition( ( float )currentFrame / ( float )video->GetFrameRate() );
		
		sharedVideoPlayer = new deoglSharedVideoPlayer( pOgl, *this, videoPlayer );
		pList.Add( sharedVideoPlayer );
		
		videoPlayer->Play();
		videoPlayer->FreeReference();
		
	}catch( const deException & ){
		if( sharedVideoPlayer ){
			delete sharedVideoPlayer;
		}
		if( videoPlayer ){
			videoPlayer->FreeReference();
		}
		throw;
	}
	
	//pOgl.LogInfoFormat( "New shared video player: video=%s frame=%i speed=%g",
	//	video->GetFilename(), currentFrame, playbackSpeed );
	return sharedVideoPlayer;
}



void deoglSharedVideoPlayerList::UpdateAll( float elapsed ){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglSharedVideoPlayer* )pList.GetAt( i ) )->Update( elapsed );
	}
}

void deoglSharedVideoPlayerList::SyncAllToRender(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglSharedVideoPlayer* )pList.GetAt( i ) )->SyncToRender();
	}
}



void deoglSharedVideoPlayerList::RemoveSharedVideoPlayer( deoglSharedVideoPlayer *sharedVideoPlayer ){
	pList.RemoveFrom( pList.IndexOf( sharedVideoPlayer ) );
}
