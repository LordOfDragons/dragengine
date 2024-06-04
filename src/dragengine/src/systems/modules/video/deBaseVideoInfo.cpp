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

#include "deBaseVideoInfo.h"
#include "../../../common/exceptions.h"
#include "../../../resources/video/deVideo.h"



// Class deBaseVideoInfo
//////////////////////////

deBaseVideoInfo::deBaseVideoInfo() :
pWidth( 1 ),
pHeight( 1 ),
pComponentCount( 3 ),
pBitCount( 8 ),
pFrameCount( 0 ),
pFrameRate( 30.0f ),

pBytesPerSample( 2 ),
pSampleCount( 0 ),
pSampleRate( 44100 ),
pChannelCount( 1 ){
}

deBaseVideoInfo::~deBaseVideoInfo(){
}



// Management
///////////////

void deBaseVideoInfo::SetWidth( int width ){
	DEASSERT_TRUE( width >= 1 )
	pWidth = width;
}

void deBaseVideoInfo::SetHeight( int height ){
	DEASSERT_TRUE( height >= 1 )
	pHeight = height;
}

void deBaseVideoInfo::SetComponentCount( int componentCount ){
	DEASSERT_TRUE( componentCount >= 1 && componentCount <= 4 )
	pComponentCount = componentCount;
}

void deBaseVideoInfo::SetBitCount( int bitCount ){
	DEASSERT_TRUE( bitCount == 8 )
	pBitCount = bitCount;
}

void deBaseVideoInfo::SetFrameCount( int frameCount ){
	DEASSERT_TRUE( frameCount >= 0 )
	pFrameCount = frameCount;
}

void deBaseVideoInfo::SetFrameRate( float frameRate ){
	DEASSERT_TRUE( frameRate > 0.0f )
	pFrameRate = frameRate;
}

void deBaseVideoInfo::SetColorConversionMatrix( const decColorMatrix3 &matrix ){
	pColorConversionMatrix = matrix;
}



void deBaseVideoInfo::SetBytesPerSample( int bytesPerSample ){
	DEASSERT_TRUE( bytesPerSample == 1 || bytesPerSample == 2 )
	pBytesPerSample = bytesPerSample;
}

void deBaseVideoInfo::SetSampleCount( int sampleCount ){
	DEASSERT_TRUE( sampleCount >= 0 )
	pSampleCount = sampleCount;
}

void deBaseVideoInfo::SetSampleRate( int sampleRate ){
	DEASSERT_TRUE( sampleRate >= 8000 )
	pSampleRate = sampleRate;
}

void deBaseVideoInfo::SetChannelCount( int channelCount ){
	DEASSERT_TRUE( channelCount >= 1 )
	pChannelCount = channelCount;
}
