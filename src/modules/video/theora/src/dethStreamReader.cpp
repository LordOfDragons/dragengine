/* 
 * Drag[en]gine Theora Video Module
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
#include <theora/codec.h>
#include <theora/theoradec.h>

#include "dethInfos.h"
#include "dethOggReader.h"
#include "dethStreamReader.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define DETH_VERSION( major, minor, subminor ) ( ( ( int )( major ) << 16 ) | ( ( int )( minor ) << 8 ) | ( int )( subminor ) )



// Class dethStreamReader
///////////////////////////

// Constructor, destructor
////////////////////////////

dethStreamReader::dethStreamReader( dethOggReader *reader, int serial ){
	if( ! reader ) DETHROW( deeInvalidParam );
	
	pReader = reader;
	pSerial = serial;
	pDecoderContext = NULL;
	pFrameOffset = 0;
	
	try{
		if( ogg_stream_init( &pStreamState, serial ) != 0 ){
			DETHROW( deeInvalidParam );
		}
		
	}catch( const deException & ){
		throw;
	}
}

dethStreamReader::~dethStreamReader(){
	if( pDecoderContext ) th_decode_free( pDecoderContext );
	ogg_stream_clear( &pStreamState );
}



// Management
///////////////

int dethStreamReader::GranuleToFrame( ogg_int64_t granule ) const{
	int iframe = ( int )( granule >> pGranuleShift );
	int pframe = ( int )( granule - ( ( ogg_int64_t )iframe << pGranuleShift ) );
	
	return iframe + pframe - pFrameOffset;
}

int dethStreamReader::GranuleToIFrame( ogg_int64_t granule ) const{
	return ( int )( granule >> pGranuleShift );
}

int dethStreamReader::GranuleToPFrame( ogg_int64_t granule ) const{
	int iframe = ( int )( granule >> pGranuleShift );
	
	return ( int )( granule - ( ( ogg_int64_t )iframe << pGranuleShift ) );
}

ogg_int64_t dethStreamReader::FrameToGranule( int iframe, int pframe ) const{
	return ( ( ogg_int64_t )iframe << pGranuleShift ) | ( ogg_int64_t )( pframe + pFrameOffset );
}

void dethStreamReader::SetGranulePosition( ogg_int64_t granule ){
	th_decode_ctl( pDecoderContext, TH_DECCTL_SET_GRANPOS, &granule, sizeof( granule ) );
}



void dethStreamReader::AddPage( ogg_page &page ){
	if( ogg_stream_pagein( &pStreamState, &page ) != 0 ) DETHROW( deeOutOfMemory );
}

int dethStreamReader::AddPacketToDecoder( ogg_packet &packet, ogg_int64_t &granule ){
	return th_decode_packetin( pDecoderContext, &packet, &granule );
}

int dethStreamReader::ReadPacket( ogg_packet &packet ){
	return ogg_stream_packetout( &pStreamState, &packet );    
}

int dethStreamReader::PeekPacket( ogg_packet &packet ){
	return ogg_stream_packetpeek( &pStreamState, &packet );    
}

void dethStreamReader::SubmitPacket( ogg_packet &packet ){
	ogg_stream_packetout( &pStreamState, &packet );
}

bool dethStreamReader::ReadTheoraHeader( dethInfos &infos ){
	if( ! infos.GetHeaderFinished() ){
		th_info &tinfo = infos.GetInfo();
		th_comment &tcomment = infos.GetComment();
		th_setup_info *tsinfo = infos.GetSetupInfo();
		ogg_packet packet;
		int result;
		
		while( ReadPacket( packet ) == 1 ){
			result = th_decode_headerin( &tinfo, &tcomment, &tsinfo, &packet );
			infos.SetSetupInfo( tsinfo );
			
			if( result == TH_ENOTFORMAT ){ // not a theora header
				return false;
			}
			
			if( result == 0 ){ // a data packet
				infos.SetHeaderFinished( true );
				
				pGranuleShift = tinfo.keyframe_granule_shift;
				pVersion = DETH_VERSION( tinfo.version_major, tinfo.version_minor, tinfo.version_subminor );
				
				// 3.2.0 streams store the frame index in the granule position.
				// 3.2.1 and later store the frame count so granule2frame requires an offset by 1
				if( pVersion >= DETH_VERSION( 3, 2, 1 ) ){
					pFrameOffset = 1;
				}
				
				pDecoderContext = th_decode_alloc( &tinfo, tsinfo );
				if( ! pDecoderContext ) DETHROW( deeOutOfMemory );
				
				th_setup_free( tsinfo );
				infos.SetSetupInfo( NULL );
				
				break;
			}
		}
	}
	
	return true;
}

void dethStreamReader::Reset(){
	ogg_stream_reset( &pStreamState );
}

bool dethStreamReader::GetDecodedFrame( th_ycbcr_buffer &buffer ){
	if( pDecoderContext ){
		return th_decode_ycbcr_out( pDecoderContext, buffer ) == 0;
	}
	
	return false;
}
