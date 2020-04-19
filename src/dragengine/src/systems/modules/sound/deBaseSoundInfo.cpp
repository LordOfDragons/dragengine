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

#include "deBaseSoundInfo.h"
#include "../../../common/exceptions.h"



// Class deBaseSoundInfo
//////////////////////////

deBaseSoundInfo::deBaseSoundInfo() :
pBytesPerSample( 1 ),
pSampleCount( 0 ),
pSampleRate( 44100 ),
pChannelCount( 1 ){
}

deBaseSoundInfo::~deBaseSoundInfo(){
}



// Management
///////////////

void deBaseSoundInfo::SetBytesPerSample( int bytesPerSample ){
	if( bytesPerSample < 1 ){
		DETHROW( deeInvalidParam );
	}
	pBytesPerSample = bytesPerSample;
}

void deBaseSoundInfo::SetSampleCount( int sampleCount ){
	if( sampleCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	pSampleCount = sampleCount;
}

void deBaseSoundInfo::SetSampleRate( int sampleRate ){
	if( sampleRate < 1 ){
		DETHROW( deeInvalidParam );
	}
	pSampleRate = sampleRate;
}

void deBaseSoundInfo::SetChannelCount( int channelCount ){
	if( channelCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	pChannelCount = channelCount;
}
