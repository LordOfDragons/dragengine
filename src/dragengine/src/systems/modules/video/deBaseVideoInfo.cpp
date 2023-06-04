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
