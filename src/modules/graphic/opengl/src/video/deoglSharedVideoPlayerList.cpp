/* 
 * Drag[en]gine OpenGL Graphic Module
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
