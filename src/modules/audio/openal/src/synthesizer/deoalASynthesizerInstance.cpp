/* 
 * Drag[en]gine OpenAL Audio Module
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
