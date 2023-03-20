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
pPixelFormat( deVideo::epf444 ),
pFrameCount( 0 ),
pFrameRate( 30 ),

pBytesPerSample( 8 ),
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

void deBaseVideoInfo::SetPixelFormat( deVideo::ePixelFormat pixelFormat ){
	pPixelFormat = pixelFormat;
}

void deBaseVideoInfo::SetFrameCount( int frameCount ){
	DEASSERT_TRUE( frameCount >= 0 )
	pFrameCount = frameCount;
}

void deBaseVideoInfo::SetFrameRate( int frameRate ){
	DEASSERT_TRUE( frameRate >= 1 )
	pFrameRate = frameRate;
}

void deBaseVideoInfo::SetColorConversionMatrix( const decColorMatrix3 &matrix ){
	pColorConversionMatrix = matrix;
}



void deBaseVideoInfo::SetBytesPerSample( int bytesPerSample ){
	DEASSERT_TRUE( bytesPerSample >= 8 )
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
