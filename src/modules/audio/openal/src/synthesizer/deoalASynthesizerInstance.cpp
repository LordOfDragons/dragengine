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
#include <string.h>

#include "deoalASynthesizerInstance.h"
#include "deoalSynthesizerInstance.h"
#include "../deoalBasics.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoalASynthesizerInstance
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalASynthesizerInstance::deoalASynthesizerInstance( deoalAudioThread &audioThread,
	deSynthesizerInstance &instance ) :
pAudioThread( audioThread ),
pBytesPerSample( 1 ),
pChannelCount( 1 ),
pSampleRate( 11025 ),
pSampleCount( instance.GetSampleCount() ),
pInstance( &instance )
{
	deSynthesizer * const synthesizer = instance.GetSynthesizer();
	if( synthesizer ){
		pBytesPerSample = synthesizer->GetBytesPerSample();
		pChannelCount = synthesizer->GetChannelCount();
		pSampleRate = synthesizer->GetSampleRate();
	}
	
	LEAK_CHECK_CREATE( audioThread, SynthesizerInstance );
}

deoalASynthesizerInstance::~deoalASynthesizerInstance(){
	LEAK_CHECK_FREE( pAudioThread, SynthesizerInstance );
	
	pCleanUp();
}



// Management
///////////////

void deoalASynthesizerInstance::UpdateParameters(){
	// WARNING Called by main thread during synchronization
	deMutexGuard guard( pMutex );
	
	pBytesPerSample = 1;
	pChannelCount = 1;
	pSampleRate = 11025;
	pSampleCount = 0;
	
	pAdvanceUpdateTracker();
	
	if( ! pInstance ){
		return;
	}
	
	pSampleCount = pInstance->GetSampleCount();
	
	const deSynthesizer * const synthesizer = pInstance->GetSynthesizer();
	if( synthesizer ){
		pBytesPerSample = synthesizer->GetBytesPerSample();
		pChannelCount = synthesizer->GetChannelCount();
		pSampleRate = synthesizer->GetSampleRate();
	}
}

void deoalASynthesizerInstance::DropInstance(){
	// WARNING Called by main thread during synchronization
	deMutexGuard guard( pMutex );
	pInstance = NULL;
	pAdvanceUpdateTracker();
}

void deoalASynthesizerInstance::Reset(){
	deMutexGuard guard( pMutex );
	if( pInstance ){
		pInstance->Reset();
	}
}

void deoalASynthesizerInstance::GenerateSound( void *buffer, int bufferSize, int offset, int samples ){
	deMutexGuard guard( pMutex );
	if( pInstance ){
		pInstance->GenerateSound( buffer, bufferSize, offset, samples );
	}
}



// Private Functions
//////////////////////

void deoalASynthesizerInstance::pCleanUp(){
}

void deoalASynthesizerInstance::pAdvanceUpdateTracker(){
	pUpdateTracker++;
	if( pUpdateTracker == 0 ){
		pUpdateTracker = 1;
	}
}
