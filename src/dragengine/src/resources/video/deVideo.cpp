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
	TIME_SYSTEM modificationTime, int width, int height, int componentCount, int bitCount,
	float frameRate, int frameCount, const decColorMatrix3 &colorConversionMatrix,
	int bytesPerSample, int sampleCount, int sampleRate, int channelCount ) :
deFileResource( manager, vfs, filename, modificationTime ),
pWidth( width ),
pHeight( height ),
pComponentCount( componentCount ),
pBitCount( bitCount ),
pFrameCount( frameCount ),
pFrameRate( frameRate ),
pColorConversionMatrix( colorConversionMatrix ),
pBytesPerSample( bytesPerSample ),
pSampleCount( sampleCount ),
pSampleRate( sampleRate ),
pChannelCount( channelCount ),
pPeerGraphic( NULL )
{
	DEASSERT_TRUE( width >= 1 )
	DEASSERT_TRUE( height >= 1 )
	DEASSERT_TRUE( componentCount >= 1 && componentCount <= 4 )
	DEASSERT_TRUE( bitCount == 8 )
	DEASSERT_TRUE( frameRate > 0.0f )
	DEASSERT_TRUE( frameCount >= 0 )
	DEASSERT_TRUE( bytesPerSample == 1 || bytesPerSample == 2 )
	DEASSERT_TRUE( sampleCount >= 0 )
	DEASSERT_TRUE( sampleRate >= 8000 )
	DEASSERT_TRUE( channelCount >= 1 )
	
	pPlayTime = ( float )frameCount / frameRate + fmodf( ( float )frameCount, frameRate ) / frameRate;
}

deVideo::deVideo( deVideoManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),
pWidth( 1 ),
pHeight( 1 ),
pComponentCount( 3 ),
pBitCount( 8 ),
pFrameCount( 0 ),
pFrameRate( 30.0f ),
pPlayTime( 0.0f ),
pBytesPerSample( 2 ),
pSampleCount( 0 ),
pSampleRate( 44100 ),
pChannelCount( 1 ),
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

void deVideo::FinalizeConstruction( int width, int height, int componentCount,
int bitCount, float frameRate, int frameCount, const decColorMatrix3 &colorConversionMatrix,
int bytesPerSample, int sampleCount, int sampleRate, int channelCount ){
	DEASSERT_TRUE( width >= 1 )
	DEASSERT_TRUE( height >= 1 )
	DEASSERT_TRUE( componentCount >= 1 && componentCount <= 4 )
	DEASSERT_TRUE( bitCount == 8 )
	DEASSERT_TRUE( frameRate > 0.0f )
	DEASSERT_TRUE( frameCount >= 0 )
	DEASSERT_TRUE( bytesPerSample == 1 || bytesPerSample == 2 )
	DEASSERT_TRUE( sampleCount >= 0 )
	DEASSERT_TRUE( sampleRate >= 8000 )
	DEASSERT_TRUE( channelCount >= 1 )
	
	pWidth = width;
	pHeight = height;
	pComponentCount = componentCount;
	pFrameCount = frameCount;
	pFrameRate = frameRate;
	pColorConversionMatrix = colorConversionMatrix;
	pPlayTime = ( float )frameCount / frameRate + fmodf( ( float )frameCount, frameRate ) / frameRate;
	pBytesPerSample = bytesPerSample;
	pSampleCount = sampleCount;
	pSampleRate = sampleRate;
	pChannelCount = channelCount;
}
