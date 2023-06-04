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
#include "dewmVorbisStream.h"
#include "dewmAudioTrackCallback.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class dewmVorbisStream
///////////////////////////

// Constructor, destructor
////////////////////////////

dewmVorbisStream::dewmVorbisStream( dewmAudioTrackCallback &callback ) :
pCallback( callback ),
pBytesPerSample( 2 ),
pSampleRate( 44100 ),
pChannelCount( 1 ),
pBufferSampleSize( 2 ),
pInfoInited( false ),
pContextInited( false ),
pBlockInited( false ),
pFillUpSample( nullptr )
{
	memset( &pPacket, 0, sizeof( pPacket ) );
}

dewmVorbisStream::~dewmVorbisStream(){
	if( pBlockInited ){
		vorbis_block_clear( &pBlock );
	}
	if( pContextInited ){
		vorbis_dsp_clear( &pContext );
	}
	if( pInfoInited ){
		vorbis_info_clear( &pInfo );
	}
	if( pFillUpSample ){
		delete [] pFillUpSample;
	}
}



// Management
///////////////

bool dewmVorbisStream::OpenTrack( const webm::TrackEntry &track ){
	if( track.track_type.value() != webm::TrackType::kAudio ){
		pCallback.GetModule().LogErrorFormat( "VorbisStream.OpenTrack: track type is not audio" );
		return false;
	}
	if( pInfoInited ){
		pCallback.GetModule().LogErrorFormat( "VorbisStream.OpenTrack: info already inited" );
		return false;
	}
	
	if( track.codec_id.value() != "A_VORBIS" ){
		pCallback.GetModule().LogErrorFormat( "VorbisStream.OpenTrack: codec is not vorbis" );
		return false;
	}
	
	// build ogg headers to start decoding. uses codec private data. no idea where one is supposed
	// to find this information. got this code from https://github.com/zaps166/libsimplewebm
	if( ! track.codec_private.is_present() ){
		pCallback.GetModule().LogErrorFormat( "VorbisStream.OpenTrack: missing codec private data" );
		return false;
	}
	
	const std::vector<unsigned char> codecPrivateData( track.codec_private.value() );
	if( codecPrivateData.size() < 3 ){
		pCallback.GetModule().LogErrorFormat(
			"VorbisStream.OpenTrack: codec private size mismatch (%d)",
			( int )codecPrivateData.size() );
		return false;
	}
	if( codecPrivateData[ 0 ] != 2 ){
		pCallback.GetModule().LogErrorFormat(
			"VorbisStream.OpenTrack: value mismatch (0 => %d)",
			codecPrivateData[ 0 ] );
		return false;
	}
	
	// calculate three headers sizes
	size_t headerSize[ 3 ] = { 0, 0, 0 };
	size_t offset = 1;
	int i;
	
	for( i=0; i<2; i++ ){
		while( true ){
			if( offset >= codecPrivateData.size() ){
				pCallback.GetModule().LogErrorFormat(
					"VorbisStream.OpenTrack: offset mismatch (%d)", ( int )offset );
				return false;
			}
			
			headerSize[ i ] += codecPrivateData[ offset ];
			if( codecPrivateData[ offset++ ] < 0xff ){
				break;
			}
		}
	}
	
	headerSize[ 2 ] = codecPrivateData.size() - ( headerSize[ 0 ] + headerSize[ 1 ] + offset );
	
	if( headerSize[ 0 ] + headerSize[ 1 ] + headerSize[ 2 ] + offset != codecPrivateData.size() ){
		pCallback.GetModule().LogErrorFormat(
			"VorbisStream.OpenTrack: header size mismatch (%d, %d, %d, %d => %d)",
			( int )headerSize[ 0 ], ( int )headerSize[ 1 ], ( int )headerSize[ 2 ],
			( int )offset, ( int )codecPrivateData.size() );
		return false;
	}
	
	ogg_packet op[ 3 ];
	memset( op, 0, sizeof( op ) );
	
	op[ 0 ].packet = ( unsigned char* )( codecPrivateData.data() + offset );
	op[ 0 ].bytes = ( long )headerSize[ 0 ];
	op[ 0 ].b_o_s = 1;
	
	op[ 1 ].packet = ( unsigned char* )( codecPrivateData.data() + offset + headerSize[ 0 ] );
	op[ 1 ].bytes = ( long )headerSize[ 1 ];
	
	op[ 2 ].packet = ( unsigned char* )( codecPrivateData.data()
		+ offset + headerSize[ 0 ] + headerSize[ 1 ] );
	op[ 2 ].bytes = ( long )headerSize[ 2 ];
	
	vorbis_info_init( &pInfo );
	pInfoInited = true;
	
	// upload three Vorbis headers into libvorbis
	vorbis_comment comment;
	vorbis_comment_init( &comment );
	
	for( i=0; i<3; i++ ){
		if( vorbis_synthesis_headerin( &pInfo, &comment, &op[ i ] ) ){
			vorbis_comment_clear( &comment );
			pCallback.GetModule().LogErrorFormat( "VorbisStream.OpenTrack: headerin failed" );
			return false;
		}
	}
	
	vorbis_comment_clear( &comment );
	
	if( vorbis_synthesis_init( &pContext, &pInfo ) ){
		pCallback.GetModule().LogErrorFormat( "VorbisStream.OpenTrack: init failed" );
		return false;
	}
	pContextInited = true;
	
	if( vorbis_block_init( &pContext, &pBlock ) ){
		pCallback.GetModule().LogErrorFormat( "VorbisStream.OpenTrack: block init failed" );
		return false;
	}
	pBlockInited = true;
	
	pBytesPerSample = 2;
	pSampleRate = pInfo.rate;
	pChannelCount = pInfo.channels;
	
	pBufferSampleSize = pBytesPerSample * pChannelCount;
	
	return true;
}

void dewmVorbisStream::CopySamples(){
	float **samples = nullptr;
	const int synthSamplecount = vorbis_synthesis_pcmout( &pContext, &samples );
	if( synthSamplecount == 0 ){
		// no more samples in context
		return;
	}
	
	// samples are present in the context. copy them over to the buffer until it is full
	const int bufferSampleCount = pCallback.GetResSize() - pCallback.GetResPosition();
	const int copiedSampleCount = decMath::min( synthSamplecount, bufferSampleCount );
	
	if( pCallback.GetResBuffer() ){
		int16_t *ptrBuffer = ( int16_t* )( pCallback.GetResBuffer()
			+ pCallback.GetResPosition() * pBufferSampleSize );
		int i, j;
		
		for( i=0; i<copiedSampleCount; i++ ){
			for( j=0; j<pChannelCount; j++ ){
				*( ptrBuffer++ ) = ( int16_t )decMath::clamp(
					( int )( samples[ j ][ i ] * 32767 ), -32768, 32767 );
			}
		}
	}
	
	pCallback.AdvanceResPosition( copiedSampleCount );
	DEASSERT_TRUE( vorbis_synthesis_read( &pContext, copiedSampleCount ) == 0 )
}

void dewmVorbisStream::LoadFrameData( std::uint64_t frameSize ){
	pPacket.packet = ( unsigned char* )pCallback.GetFrameBuffer();
	pPacket.bytes = ( long )frameSize;
	
	if( vorbis_synthesis( &pBlock, &pPacket ) ){
		return; // corrupt package. ignore it
	}
	if( vorbis_synthesis_blockin( &pContext, &pBlock ) ){
		return; // corrupt package. ignore it
	}
}

void dewmVorbisStream::Rewind(){
	DEASSERT_TRUE( vorbis_synthesis_restart( &pContext ) == 0 )
}

void dewmVorbisStream::FillUpBuffer(){
	if( ! pFillUpSample ){
		pFillUpSample = new uint8_t[ pBufferSampleSize ];
	}
	
	uint8_t *ptrBuffer = pCallback.GetResBuffer() + pCallback.GetResPosition() * pBufferSampleSize;
	
	if( pCallback.GetResPosition() > 0 ){
		memcpy( pFillUpSample, ptrBuffer - pBufferSampleSize, pBufferSampleSize );
		
	}else{
		memset( pFillUpSample, 0, pBufferSampleSize );
	}
	
	int i;
	for( i=pCallback.GetResPosition(); i<pCallback.GetResSize(); i++ ){
		memcpy( ptrBuffer, pFillUpSample, pBufferSampleSize );
		ptrBuffer += pBufferSampleSize;
	}
}
