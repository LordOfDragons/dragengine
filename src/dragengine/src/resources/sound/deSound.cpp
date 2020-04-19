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

#include "deSound.h"
#include "deSoundManager.h"
#include "../../systems/modules/audio/deBaseAudioSound.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSound.h"
#include "../../common/exceptions.h"



// Class deSound
//////////////////

// Constructor, destructor
////////////////////////////

deSound::deSound( deSoundManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime, int bytesPerSample, int sampleRate, int sampleCount,
	int channelCount ) :
deFileResource( manager, vfs, filename, modificationTime ),

pBytesPerSample( bytesPerSample ),
pSampleCount( sampleCount ),
pChannelCount( channelCount ),
pSampleRate( sampleRate ),

pPeerAudio( NULL ),
pPeerSynthesizer( NULL )
{
	if( bytesPerSample < 1 || sampleCount < 0 || sampleRate < 1 || channelCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pPlayTime = ( float )( sampleCount / sampleRate )
		+ ( float )( sampleCount % sampleRate ) / ( float )sampleRate;
}

deSound::deSound( deSoundManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),

pBytesPerSample( 0 ),
pSampleCount( 0 ),
pChannelCount( 0 ),
pSampleRate( 0 ),
pPlayTime( 0.0f ),

pPeerAudio( NULL ),
pPeerSynthesizer( NULL ){
}

deSound::~deSound(){
	SetPeerAudio( NULL ),
	SetPeerSynthesizer( NULL );
}



// Management
///////////////



// System Peers
/////////////////

void deSound::SetPeerAudio( deBaseAudioSound *peer ){
	if( peer == pPeerAudio ){
		return;
	}
	
	if( pPeerAudio ){
		delete pPeerAudio;
	}
	
	pPeerAudio = peer;
}

void deSound::SetPeerSynthesizer( deBaseSynthesizerSound *peer ){
	if( peer == pPeerSynthesizer ){
		return;
	}
	
	if( pPeerSynthesizer ){
		delete pPeerSynthesizer;
	}
	
	pPeerSynthesizer = peer;
}



// Special
////////////

void deSound::FinalizeConstruction( int bytesPerSample, int sampleRate,
int sampleCount, int channelCount ){
	if( bytesPerSample < 1 || sampleCount < 0 || sampleRate < 1 || channelCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pBytesPerSample = bytesPerSample;
	pSampleCount = sampleCount;
	pChannelCount = channelCount;
	pSampleRate = sampleRate;
	
	pPlayTime = ( float )( sampleCount / sampleRate )
		+ ( float )( sampleCount % sampleRate ) / ( float )sampleRate;
}
