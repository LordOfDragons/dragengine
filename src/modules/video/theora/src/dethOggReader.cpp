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
#include "deVideoTheora.h"

#include <dragengine/resources/video/deVideo.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>



// Class dethOggReader
////////////////////////

// Constructor, destructor
////////////////////////////

dethOggReader::dethOggReader( deVideoTheora &module, decBaseFileReader &reader ) :
pModule( module ),

pReader( reader ),
pStream( NULL ),

pCurFrame( 0 )
{
	if( ogg_sync_init( &pSyncState ) != 0 ){
		DETHROW( deeInvalidParam );
	}
}

dethOggReader::~dethOggReader(){
	if( pStream ){
		delete pStream;
	}
	ogg_sync_clear( &pSyncState );
}



// Management
///////////////

void dethOggReader::ReadStreamHeaders( dethInfos &infos ){
	ogg_int64_t maxGranulePos = 0;
	ogg_int64_t pageGranulePos;
	ogg_page page;
	int serial;
	
	while( /* ! infos.GetHeaderFinished() && */ ReadPage( page ) ){
		serial = ogg_page_serialno( &page );
		
		if( ogg_page_bos( &page ) ){
			if( ! pStream ){
				pStream = new dethStreamReader( this, serial );
				//pModule.LogInfoFormat( "Stream %i: Testing for Theora", serial );
			}
			
		}else{
			if( ! pStream ) break;
			//pModule.LogInfoFormat( "Stream %i: additional page", serial );
		}
		
		if( pStream && serial == pStream->GetSerial() ){
			pageGranulePos = ogg_page_granulepos( &page );
			if( pageGranulePos > maxGranulePos ){
				maxGranulePos = pageGranulePos;
			}
			
			if( ! infos.GetHeaderFinished() ){
				pStream->AddPage( page );
				
				if( ! pStream->ReadTheoraHeader( infos ) ){
					//pModule.LogWarnFormat( "Stream %i: Not a Theora stream, ignoring it", serial );
					delete pStream;
					pStream = NULL;
				}
			}
		}
	}
	
	if( infos.GetHeaderFinished() ){
		const th_info &tinfo = infos.GetInfo();
		decColorMatrix3 colorMatrix;
		
		infos.SetWidth( tinfo.pic_width );
		infos.SetHeight( tinfo.pic_height );
		infos.SetFrameCount( pStream ? pStream->GranuleToFrame( maxGranulePos ) + 1 : 0 );
		infos.SetFrameRate( tinfo.fps_numerator / tinfo.fps_denominator );
		
		DefaultColorConversionMatrix( colorMatrix );
		infos.SetColorConversionMatrix( colorMatrix );
		
		switch( tinfo.pixel_fmt ){
		case TH_PF_420:
		case TH_PF_422:
		case TH_PF_444:
			infos.SetComponentCount( 3 );
			break;
			
		default:
			pModule.LogErrorFormat( "Unsupported Pixel Format %i", tinfo.pixel_fmt );
		}
		
		/*
		pModule.LogInfo( "header finished." );
		
		pModule.LogInfoFormat( "frame_width: %i", tinfo.frame_width );
		pModule.LogInfoFormat( "frame_height: %i", tinfo.frame_height );
		pModule.LogInfoFormat( "pic_width: %i", tinfo.pic_width );
		pModule.LogInfoFormat( "pic_height: %i", tinfo.pic_height );
		pModule.LogInfoFormat( "pic_x: %i", tinfo.pic_x );
		pModule.LogInfoFormat( "pic_y: %i", tinfo.pic_y );
		pModule.LogInfoFormat( "colorspace: %i", tinfo.colorspace );
		pModule.LogInfoFormat( "pixel_fmt: %i", tinfo.pixel_fmt );
		pModule.LogInfoFormat( "target_bitrate: %i", tinfo.target_bitrate );
		pModule.LogInfoFormat( "quality: %i", tinfo.quality );
		pModule.LogInfoFormat( "keyframe_granule_shift: %i", tinfo.keyframe_granule_shift );
		pModule.LogInfoFormat( "version_major: %i", tinfo.version_major );
		pModule.LogInfoFormat( "version_minor: %i", tinfo.version_minor );
		pModule.LogInfoFormat( "version_subminor: %i", tinfo.version_subminor );
		pModule.LogInfoFormat( "fps_numerator: %i", tinfo.fps_numerator );
		pModule.LogInfoFormat( "fps_denominator: %i", tinfo.fps_denominator );
		pModule.LogInfoFormat( "aspect_numerator: %i", tinfo.aspect_numerator );
		pModule.LogInfoFormat( "aspect_denominator: %i", tinfo.aspect_denominator );
		
		const th_comment &tcomment = infos.GetComment();
		pModule.LogInfoFormat( "comments: %i", tcomment.comments );
		int c;
		for( c=0; c<tcomment.comments; c++ ){
			pModule.LogInfoFormat( "- comment %i: %s", c, tcomment.user_comments[ c ] );
		}
		pModule.LogInfoFormat( "vendor: %s", tcomment.vendor );
		*/
	}
}

int dethOggReader::ReadFromFile( char *buffer, int size ){
	int remaining = pReader.GetLength() - pReader.GetPosition();
	
	if( size > remaining ){
		size = remaining;
	}
	
	if( size > 0 ){
		pReader.Read( buffer, size );
	}
	
	return size;
}

bool dethOggReader::ReadPage( ogg_page &page ){
	int bufferSize = 4096;
	char *buffer = NULL;
	int totalBytes = 0;
	int readBytes;
	
	try{
		while( ogg_sync_pageout( &pSyncState, &page ) != 1 ){
			buffer = ogg_sync_buffer( &pSyncState, bufferSize );
			if( ! buffer ){
				DETHROW( deeOutOfMemory );
			}
			
			readBytes = ReadFromFile( buffer, bufferSize );
			if( readBytes == 0 ){
				return false; // end of file
			}
			
			if( ogg_sync_wrote( &pSyncState, readBytes ) != 0 ){
				DETHROW( deeOutOfMemory );
			}
			
			totalBytes += readBytes;
		}
		
	}catch( const deException & ){
		//e.PrintError();
		throw;
	}
	
	return true;
}

bool dethOggReader::ReadPage( ogg_page &page, int serial ){
	while( ReadPage( page ) ){
		if( ogg_page_serialno( &page ) == serial ){
			return true;
		}
	}
	
	return false;
}

void dethOggReader::Rewind(){
	if( pStream ){
		int serial = pStream->GetSerial();
		ogg_int64_t granulePacket;
		bool keepRunning = true;
		ogg_packet packet;
		ogg_page page;
		
		//pModule.LogInfo( "Rewind" );
		// reset the sync and stream states
		ogg_sync_reset( &pSyncState );
		pStream->Reset();
		
		// rewind to the beginning of the file. an optimization would be to store
		// the file position of the first page that contained video data
		pReader.SetPosition( 0 );
		
		// seek until the first page is found which contains data packets
		// of the right stream. once found add it to the decoder
		while( keepRunning && ReadPage( page ) ){
			if( ogg_page_serialno( &page ) == serial ){
				pStream->AddPage( page );
				
				while( pStream->PeekPacket( packet ) == 1 ){
					if( th_packet_isheader( &packet ) == 0 ){
						pStream->AddPacketToDecoder( packet, granulePacket );
						keepRunning = false;
						break;
					}
					
					pStream->SubmitPacket( packet );
				}
			}
		}
		
		// reset the granule position
		pStream->SetGranulePosition( pStream->FrameToGranule( 0, 0 ) );
	}
	
	pCurFrame = 0;
}

void dethOggReader::SeekFrame( int frame ){
	if( pStream ){
		int serial = pStream->GetSerial();
		ogg_int64_t granulePacket;
		ogg_packet packet;
		ogg_page page;
		int result;
		
		//pModule.LogInfoFormat( "Decoder: seek=%i current=%i", frame, pCurFrame );
		
		// if the seek goes backwards restart from the beginning of the file
		if( frame < pCurFrame ){
			Rewind();
		}
		
		// read packets until we find the one with the matching frame number
		while( pCurFrame < frame ){
			if( pStream->ReadPacket( packet ) == 1 ){
				result = pStream->AddPacketToDecoder( packet, granulePacket );
				
				if( result == 0 || result == TH_DUPFRAME ){
					pCurFrame++;
				}
				
				//pModule.LogInfoFormat( "Decoder: packet: granule=%i iframe=%i pframe=%i frame=%i curFrame=%i",
				//	( int )granulePacket, pStream->GranuleToIFrame( granulePacket ),
				//	pStream->GranuleToPFrame( granulePacket ), pStream->GranuleToFrame( granulePacket ), pCurFrame );
				
				//if( pStream->GranuleToFrame( granulePacket ) >= frame ) break;
				
			}else{
				if( ! ReadPage( page, serial ) ) break;
				pStream->AddPage( page );
			}
		}
	}
}



void dethOggReader::DefaultColorConversionMatrix( decColorMatrix3 &matrix ){
	// offsets: 16, 128, 128
	// exculsions: 219, 224, 224
	// kr = 0.299
	// kb = 0.114
	// 
	// Y' = (Y'-offY)/excY
	// Pb = (Cb-offCb)/excCb
	// Pr = (Cr-offCr)/excCr
	// 
	// R' = Y' + c1*Pr
	// G' = Y' + c2*Pb + c3*Pr
	// B' = Y' + c4*Pb
	// 
	// c1 = 2(1-kr)
	// c2 = -2(1-kb)kb / (1-kb-kr)
	// c3 = -2(1-kr)kr / (1-kb-kr)
	// c4 = 2(1-kb)
	// 
	// | R |   | a11 a12 a13 a14 |   | Y' |
	// | G | = | a21 a22 a23 a24 | * | Cb |
	// | B |   | a31 a32 a33 a34 |   | Cr |
	// 
	// a11 = 1/excY
	// a12 = 0
	// a13 = c1/excCr
	// a14 = -offY/excY - c1*offCr/excCr
	// a21 = 1/excY
	// a22 = c2/excCb
	// a23 = c3/excCr
	// a24 = -offY/excY - c2*offCb/excCb - c3*offCr/excCr
	// a31 = 1/excY
	// a32 = c4/excCb
	// a33 = 0
	// a34 = -offY/excY - c4*offCb/excCb
	// 
	// important note for creating the color conversation matrix. the game engine
	// expects the matrix to transform from [0..1] to [0..1] hence both the input
	// YCbCr values and the output RGB values have to be in the range from 0 to 1.
	
	float offY = 16.0f / 255.0f;
	float offCb = 128.0f / 255.0f;
	float offCr = 128.0f / 255.0f;
	
	float excY = 219.0f / 255.0f;
	float excCb = 224.0f / 255.0f;
	float excCr = 224.0f / 255.0f;
	
	float kr = 0.299f;
	float kb = 0.114f;
	
	float c1 = 2.0f * ( 1.0f - kr );
	float c2 = -2.0f * ( 1.0f - kb ) * kb / ( 1.0f - kb - kr );
	float c3 = -2.0f * ( 1.0f - kr ) * kr / ( 1.0f - kb - kr );
	float c4 = 2.0f * ( 1.0f - kb );
	
	// calculate matrix from parameters
	matrix.a11 = 1.0f / excY;
	matrix.a12 = 0.0f;
	matrix.a13 = c1 / excCr;
	matrix.a14 = -offY / excY - c1 * offCr / excCr;
	
	matrix.a21 = matrix.a11;
	matrix.a22 = c2 / excCb;
	matrix.a23 = c3 / excCr;
	matrix.a24 = -offY / excY - c2 * offCb / excCb - c3 * offCr / excCr;
	
	matrix.a31 = matrix.a11;
	matrix.a32 = c4 / excCb;
	matrix.a33 = 0.0f;
	matrix.a34 = -offY / excY - c4 * offCb / excCb;
}
