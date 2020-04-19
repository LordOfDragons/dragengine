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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglVideo.h"
#include "deoglRVideo.h"
#include "../deGraphicOpenGl.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoglVideo
/////////////////////

// Constructor, destructor
////////////////////////////

deoglVideo::deoglVideo( deGraphicOpenGl &ogl, deVideo &video ) :
pOgl( ogl ),
pVideo( video ),
pRVideo( NULL ),

pCachedFrameCount( 0 ),
pCacheFrames( false )
{
	pDetermineCacheParams();
	pRVideo = new deoglRVideo( ogl.GetRenderThread(), pVideo.GetWidth(), pVideo.GetHeight(), pCachedFrameCount );
}

deoglVideo::~deoglVideo(){
	if( pRVideo ){
		pRVideo->FreeReference();
	}
}



// Management
///////////////

bool deoglVideo::AllFramesAreCached() const{
	return pRVideo->GetFrameCountToCache() == 0;
}

bool deoglVideo::IsFrameReady( int frame ) const{
	if( ! pCacheFrames ){
		return false;
	}
	return pRVideo->GetTexture( frame ) != NULL;
}

bool deoglVideo::CanCacheFrame( int frame ) const{
	if( ! pCacheFrames ){
		return false;
	}
	if( pRVideo->GetUpdateFrame() != -1 ){
		return false;
	}
	return pRVideo->GetTexture( frame ) == NULL;
}

deoglPixelBuffer *deoglVideo::CacheFrame( int frame, deoglPixelBuffer *pixelBuffer ){
	if( ! pCacheFrames ){
		DETHROW( deeInvalidParam );
	}
	return pRVideo->SetPixelBuffer( frame, pixelBuffer );
}

deoglTexture *deoglVideo::GetCachedFrameTexture( int frame ) const{
	if( ! pCacheFrames ){
		return NULL;
	}
	return pRVideo->GetTexture( frame );
}



// Private functions
//////////////////////

void deoglVideo::pDetermineCacheParams(){
	// check if the video is small enough to warrant keeping all frames in memory.
	// what he sweet spot is for this threshold is difficult to say. certainly needs
	// a user tunable parameter since users of GPUs with larger RAM can decide to
	// speed things up by caching larger videos. for the time being we use a
	// total size threshold of 24MB uncompressed. Compressed this yields a total
	// memory consumption of 6MB.
	const int frameCount = pVideo.GetFrameCount();
	
	const int width = pVideo.GetWidth();
	const int height = pVideo.GetHeight();
	const int frameBytes = width * height * 3;
	const int thresholdFrames = 24000000 / frameBytes;
	
	pCacheFrames = ( frameCount <= thresholdFrames );
	
	/*
	pOgl.LogInfoFormat( "Video '%s': size=%ix%i frames=%i frameBytes=%i"
		" thresholdFrames=%i cacheFrames=%d", pVideo.GetFilename().GetString(), width,
		height, frameCount, frameBytes, thresholdFrames, pCacheFrames );
	*/
	
	if( pCacheFrames ){
		pCachedFrameCount = frameCount;
	}
}
