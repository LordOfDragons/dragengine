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

#include "deVideo.h"
#include "deVideoManager.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicVideo.h"



// Class deVideo
//////////////////

// Constructor, destructor
////////////////////////////

deVideo::deVideo( deVideoManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime, int width, int height, ePixelFormat pixelFormat,
	int frameRate, int frameCount, const decColorMatrix3 &colorConversionMatrix,
	int bytesPerSample, int sampleCount, int sampleRate, int channelCount ) :
deFileResource( manager, vfs, filename, modificationTime ),
pWidth( width ),
pHeight( height ),
pPixelFormat( pixelFormat ),
pFrameCount( frameCount ),
pFrameRate( frameRate ),
pColorConversionMatrix( colorConversionMatrix ),
pBytesPerSample( bytesPerSample ),
pSampleCount( sampleCount ),
pSampleRate( sampleRate ),
pChannelCount( channelCount ),
pPeerGraphic( NULL )
{
	if( width < 1 || height < 1 || pixelFormat < epf444 || pixelFormat > epf4444
	|| frameRate < 0 || frameCount < 0 || bytesPerSample < 0 || sampleCount < 0
	|| sampleRate < 0 || channelCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pPlayTime = ( float )( frameCount / frameRate )
		+ ( float )( frameCount % frameRate ) / ( float )frameRate;
}

deVideo::deVideo( deVideoManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),
pWidth( 0 ),
pHeight( 0 ),
pPixelFormat( epf444 ),
pFrameCount( 0 ),
pFrameRate( 0 ),
pPlayTime( 0.0f ),
pBytesPerSample( 0 ),
pSampleCount( 0 ),
pSampleRate( 0 ),
pChannelCount( 0 ),
pPeerGraphic( NULL ){
}

deVideo::~deVideo(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
}



// System Peers
/////////////////

void deVideo::SetPeerGraphic( deBaseGraphicVideo *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}



// Special
////////////

void deVideo::FinalizeConstruction( int width, int height, ePixelFormat pixelFormat,
int frameRate, int frameCount, const decColorMatrix3 &colorConversionMatrix,
int bytesPerSample, int sampleCount, int sampleRate, int channelCount ){
	if( width < 1 || height < 1 || pixelFormat < epf444 || pixelFormat > epf4444
	|| frameRate < 0 || frameCount < 0 || bytesPerSample < 0 || sampleCount < 0
	|| sampleRate < 0 || channelCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pWidth = width;
	pHeight = height;
	pPixelFormat = pixelFormat;
	pFrameCount = frameCount;
	pFrameRate = frameRate;
	pColorConversionMatrix = colorConversionMatrix;
	pPlayTime = ( float )( frameCount / frameRate )
		+ ( float )( frameCount % frameRate ) / ( float )frameRate;
	pBytesPerSample = bytesPerSample;
	pSampleCount = sampleCount;
	pSampleRate = sampleRate;
	pChannelCount = channelCount;
}
