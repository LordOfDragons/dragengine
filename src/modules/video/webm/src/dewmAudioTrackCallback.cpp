/* 
 * Drag[en]gine WebM Video Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include <string.h>

#include "deVideoWebm.h"
#include "dewmAudioTrackCallback.h"
#include "dewmVorbisStream.h"
#include "dewmInfos.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class dewmAudioTrackCallback
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dewmAudioTrackCallback::dewmAudioTrackCallback( deVideoWebm &module ) :
dewmTrackCallback( module ),
pResBuffer( nullptr ),
pResSize( 0 ),
pResPosition( 0 ),
pSampleSize( 1 ),
pStreamVorbis( nullptr ){
}

dewmAudioTrackCallback::~dewmAudioTrackCallback(){
	if( pStreamVorbis ){
		delete pStreamVorbis;
	}
}



// Management
///////////////

void dewmAudioTrackCallback::SetResBuffer( void *buffer, int samples ){
	DEASSERT_TRUE( ! buffer || IsStreamOpen() )
	
	pResBuffer = ( uint8_t* )buffer;
	pResSize = samples;
	pResPosition = 0;
}

bool dewmAudioTrackCallback::IsStreamOpen(){
	return pStreamVorbis;
}

void dewmAudioTrackCallback::Rewind(){
	DEASSERT_TRUE( IsStreamOpen() )
	
	if( pStreamVorbis ){
		pStreamVorbis->Rewind();
	}
}

bool dewmAudioTrackCallback::OpenTrack( const webm::TrackEntry &track ){
	return pOpenTrack( track );
}

void dewmAudioTrackCallback::UpdateInfos( dewmInfos &infos ){
	DEASSERT_TRUE( IsStreamOpen() )
	
	if( pStreamVorbis ){
		infos.SetBytesPerSample( pStreamVorbis->GetBytesPerSample() );
		infos.SetSampleRate( pStreamVorbis->GetSampleRate() );
		infos.SetChannelCount( pStreamVorbis->GetChannelCount() );
	}
}

void dewmAudioTrackCallback::AdvanceResPosition( int bytes ){
	pResPosition += bytes;
}



// Protected Functions
////////////////////////

bool dewmAudioTrackCallback::pOpenTrack( const webm::TrackEntry &track ){
	if( track.track_type.value() != webm::TrackType::kAudio ){
		return false;
	}
	
	if( pStreamVorbis ){
		return false;
	}
	
	if( track.codec_id.value() == "A_VORBIS" ){
		pStreamVorbis = new dewmVorbisStream( *this );
		if( pStreamVorbis->OpenTrack( track ) ){
			pSampleSize = pStreamVorbis->GetBufferSampleSize();
			return true;
		}
		
		delete pStreamVorbis;
		pStreamVorbis = nullptr;
	}
	
	return false;
}

void dewmAudioTrackCallback::pProcessFrame( webm::Reader &reader, std::uint64_t &bytes_remaining ){
	// copy remaining samples in context until buffer is full
	pCopySamples();
	
	if( pResPosition == pResSize ){
		// found enough remaining samples to fill the buffer. the data from this frame is
		// not required. by not touching bytes_remaining this frame will be process again
		// the next time the parser is run
		return;
	}
	
	// we need more samples. load frame data and copy samples until buffer is full.
	// after this call bytes_remaining will be 0 and the next frame will be process
	pLoadFrameData( reader, bytes_remaining );
	pCopySamples();
	
	// if the buffer is not full we need more frames to complete it otherwise we can stop
	SetNeedMoreFrames( pResPosition < pResSize );
}

void dewmAudioTrackCallback::pEndSegment(){
	// we have a really annoying problem here. the webm format has no way to store the
	// total count of samples nor a reliable way to calculate this. it is thus possible
	// we are asked for more samples at the end of the video than we can provide.
	// in this case we fill up with last sample written
	/*
	GetModule().LogInfoFormat("pEndSegment %d %d", pResPosition, pResSize);
	if( pResPosition < pResSize ){
		if( pStreamVorbis ){
			pStreamVorbis->FillUpBuffer();
			pResPosition = pResSize;
		}
	}
	*/
}



// Private Functions
//////////////////////

void dewmAudioTrackCallback::pCopySamples(){
	if( pStreamVorbis ){
		pStreamVorbis->CopySamples();
	}
}

void dewmAudioTrackCallback::pLoadFrameData( webm::Reader &reader, std::uint64_t &bytes_remaining ){
	if( bytes_remaining == 0 ){
		// frame consumed
		return;
	}
	
	// read frame data and load it. bytes_remaining will become 0 after reading
	// the frame so if we know the next time the frame has been consumed
	const uint64_t frameSize = bytes_remaining;
	pReadFrameData( reader, bytes_remaining );
	
	if( pStreamVorbis ){
		pStreamVorbis->LoadFrameData( frameSize );
	}
}
