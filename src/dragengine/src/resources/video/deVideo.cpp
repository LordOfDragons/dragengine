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
	
	pPlayTime = ( float )frameCount / frameRate;
	
	// this rounds up to a full second... why has this been here?
	//pPlayTime += fmodf( ( float )frameCount, frameRate ) / frameRate;
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
