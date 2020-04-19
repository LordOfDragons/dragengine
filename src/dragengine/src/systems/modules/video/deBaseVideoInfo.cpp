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

pBytesPerSample( 0 ),
pSampleCount( 0 ),
pSampleRate( 0 ),
pChannelCount( 0 ){
}

deBaseVideoInfo::~deBaseVideoInfo(){
}



// Management
///////////////

void deBaseVideoInfo::SetWidth( int width ){
	if( width < 1 ){
		DETHROW( deeInvalidParam );
	}
	pWidth = width;
}

void deBaseVideoInfo::SetHeight( int height ){
	if( height < 1 ){
		DETHROW( deeInvalidParam );
	}
	pHeight = height;
}

void deBaseVideoInfo::SetPixelFormat( deVideo::ePixelFormat pixelFormat ){
	if( pixelFormat < deVideo::epf444 || pixelFormat > deVideo::epf4444 ){
		DETHROW( deeInvalidParam );
	}
	pPixelFormat = pixelFormat;
}

void deBaseVideoInfo::SetFrameCount( int frameCount ){
	if( frameCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	pFrameCount = frameCount;
}

void deBaseVideoInfo::SetFrameRate( int frameRate ){
	if( frameRate < 1 ){
		DETHROW( deeInvalidParam );
	}
	pFrameRate = frameRate;
}

void deBaseVideoInfo::SetColorConversionMatrix( const decColorMatrix3 &matrix ){
	pColorConversionMatrix = matrix;
}



void deBaseVideoInfo::SetBytesPerSample( int bytesPerSample ){
	if( bytesPerSample < 0 ){
		DETHROW( deeInvalidParam );
	}
	pBytesPerSample = bytesPerSample;
}

void deBaseVideoInfo::SetSampleCount( int sampleCount ){
	if( sampleCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	pSampleCount = sampleCount;
}

void deBaseVideoInfo::SetSampleRate( int sampleRate ){
	if( sampleRate < 0 ){
		DETHROW( deeInvalidParam );
	}
	pSampleRate = sampleRate;
}

void deBaseVideoInfo::SetChannelCount( int channelCount ){
	if( channelCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	pChannelCount = channelCount;
}
