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
