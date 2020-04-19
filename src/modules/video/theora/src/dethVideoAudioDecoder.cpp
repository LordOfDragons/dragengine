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

#include "deVideoTheora.h"
#include "dethInfos.h"
#include "dethVideoAudioDecoder.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/resources/image/deImage.h>



// Class dethVideoAudioDecoder
////////////////////////////////

// Constructor, destructor
////////////////////////////

dethVideoAudioDecoder::dethVideoAudioDecoder( deVideoTheora &module, decBaseFileReader &reader ) :
deBaseVideoAudioDecoder( &reader ),
pModule( module ),
pSerial( 0 ),
pDecoderContextInited( false ),
pDecoderBlockInited( false ),
pFileSize( reader.GetLength() ),
pFileRemaining( pFileSize - reader.GetPosition() ),
pBytesPerSample( 0 ),
pSampleCount( 0 ),
pSampleRate( 0 ),
pChannelCount( 0 ),
pBufferSampleSize( 0 ),
pCurSample( 0 )
{
	// WARNING info and comment struct have to stay alive while the decoder context and
	//         decoder block is alive. not doing so will segfault. thanks to yet another
	//         important information not mentioned in the official documentation U_U
	vorbis_info_init( &pInfo );
	vorbis_comment_init( &pComment );
	
	try{
		if( ogg_sync_init( &pSyncState ) ){
			DETHROW( deeInvalidAction );
		}
		
		pFindAudioStream();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dethVideoAudioDecoder::~dethVideoAudioDecoder(){
	pCleanUp();
}



// Management
///////////////

int dethVideoAudioDecoder::GetPosition(){
	return pCurSample;
}

void dethVideoAudioDecoder::SetPosition( int position ){
	pSeek( position );
}

int dethVideoAudioDecoder::ReadSamples( void *buffer, int size ){
	int16_t *ptrBuffer = ( int16_t* )buffer;
	float **samples = NULL;
	int bytesWritten = 0;
	
	size -= size % pBufferSampleSize;
	
	while( bytesWritten < size ){
		int sampleCount = vorbis_synthesis_pcmout( &pDecoderContext, &samples );
		if( sampleCount == 0 ){
			bool keepGoing = false;
			ogg_packet packet;
			ogg_page page;
			
			while( true ){
				if( ogg_stream_packetout( &pStreamState, &packet ) == 1 ){
					if( vorbis_synthesis( &pDecoderBlock, &packet ) ){
						continue; // not data packet (for example header) or bad packet. read on
					}
					if( vorbis_synthesis_blockin( &pDecoderContext, &pDecoderBlock ) ){
						DETHROW( deeInvalidAction );
					}
					keepGoing = true;
					break;
					
				}else if( pReadPage( page ) == 0 ){
					if( ogg_page_serialno( &page ) != pSerial ){
						continue;
					}
					if( ogg_stream_pagein( &pStreamState, &page ) ){
						DETHROW( deeInvalidAction );
					}
					
				}else{
					break;
				}
			}
			
			if( keepGoing ){
				continue;
				
			}else{
				break;
			}
		}
		
		const int bufferSampleLimit = ( size - bytesWritten ) / pBufferSampleSize;
		if( sampleCount > bufferSampleLimit ){
			sampleCount = bufferSampleLimit;
		}
		
		int i, j;
		for( i=0; i<sampleCount; i++ ){
			for( j=0; j<pChannelCount; j++ ){
				*( ptrBuffer++ ) = ( int16_t )decMath::clamp(
					( int )( samples[ j ][ i ] * 32767 ), -32768, 32767 );
			}
		}
		
		if( vorbis_synthesis_read( &pDecoderContext, sampleCount ) ){
			DETHROW( deeInvalidAction );
		}
		bytesWritten += sampleCount * pBufferSampleSize;
		pCurSample = decMath::min( pCurSample + sampleCount, pSampleCount );
	}
	
	return bytesWritten;
}



// Private Functions
//////////////////////

void dethVideoAudioDecoder::pCleanUp(){
	if( pDecoderBlockInited ){
		vorbis_block_clear( &pDecoderBlock );
	}
	if( pDecoderContextInited ){
		vorbis_dsp_clear( &pDecoderContext );
	}
	if( pSerial ){
		ogg_stream_clear( &pStreamState );
	}
	
	vorbis_info_clear( &pInfo );
	vorbis_comment_clear( &pComment );
	ogg_sync_clear( &pSyncState );
}

void dethVideoAudioDecoder::pFindAudioStream(){
	// NOTE finding stream is a bit involved (to avoid segfaults and such). multiple headers
	//      appear before the data packets appear. the way vorbis_synthesis_headerin is
	//      designed we have to decide on the first header we see if this is a vorbis stream
	//      or not. then we have to continue parsing headers until all headers appeared
	//      otherwise vorbis_synthesis_init segfaults. so the problematic part here is that
	//      the return value OV_ENOTVORBIS has a double meaning which is annoying. for the
	//      first encountered package this means "not a vorbis stream, ignore it". for any
	//      following header though it means "not more a header but data stream, all fine".
	//      one time it is an error the other time it is not. speaking about design mistakes.
	ogg_int64_t maxGranulePos = 0;
	bool firstHeaderSeen = false;
	bool headerFinished = false;
	ogg_packet packet;
	ogg_page page;
	
	while( pReadPage( page ) ){
		const int serial = ogg_page_serialno( &page );
		
		if( ogg_page_bos( &page ) ){
			// first page in stream
			if( ! pSerial ){
				if( ogg_stream_init( &pStreamState, serial ) ){
					DETHROW( deeInvalidAction );
				}
				pSerial = serial;
				firstHeaderSeen = false;
				//pModule.LogInfoFormat( "Stream %d: Testing for Vorbis", serial );
			}
			
		}else if( ! pSerial ){
			//break;
			continue;
		}
		
		if( serial == pSerial ){
			const ogg_int64_t pageGranulePos = ogg_page_granulepos( &page );
			if( pageGranulePos > maxGranulePos ){
				maxGranulePos = pageGranulePos;
			}
			
			if( ! headerFinished ){
				if( ogg_stream_pagein( &pStreamState, &page ) ){
					DETHROW( deeInvalidAction );
				}
				if( ogg_stream_packetout( &pStreamState, &packet ) != 1 ){
					continue; // more data needed
				}
				
				// check if this is a vorbis header. be careful about the double-meaning code
				const int result = vorbis_synthesis_headerin( &pInfo, &pComment, &packet );
				switch( result ){
				case 0: // this is a vorbis header
					firstHeaderSeen = true;
					break;
					
				case OV_ENOTVORBIS:
					// WARNING double-meaning code!
					if( firstHeaderSeen ){
						// first data package encountered
						headerFinished = true;
						
					}else{
						// first header is not a vorbis header. seems to be not a vorbis stream
						//pModule.LogInfoFormat( "Stream %d: Not a Vorbis stream, ignoring it", serial );
						ogg_stream_clear( &pStreamState );
						pSerial = 0;
					}
					break;
					
				default:
					// errors
					//pModule.LogWarnFormat( "Stream %d: Not a Vorbis stream, ignoring it", serial );
					ogg_stream_clear( &pStreamState );
					pSerial = 0;
				}
			}
		}
	}
	
	if( ! headerFinished ){
		if( pSerial ){
			ogg_stream_clear( &pStreamState );
			pSerial = 0;
		}
		return;
	}
	
	if( vorbis_synthesis_init( &pDecoderContext, &pInfo ) ){
		DETHROW( deeInvalidAction );
	}
	pDecoderContextInited = true;
	
	if( vorbis_block_init( &pDecoderContext, &pDecoderBlock ) ){
		DETHROW( deeInvalidAction );
	}
	pDecoderBlockInited = true;
	
	if( pInfo.channels < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pBytesPerSample = 2;
	pSampleRate = pInfo.rate;
	pChannelCount = pInfo.channels;
	
	const double playtime = vorbis_granule_time( &pDecoderContext, maxGranulePos );
	pSampleCount = ( int )( playtime * pInfo.rate + 0.5f );
	
	pBufferSampleSize = pBytesPerSample * pChannelCount;
	
	pRewind();
}

int dethVideoAudioDecoder::pReadFromFile( char *buffer, int size ){
	decBaseFileReader &reader = *GetFile();
	if( size > pFileRemaining ){
		size = pFileRemaining;
	}
	if( size > 0 ){
		reader.Read( buffer, size );
		pFileRemaining -= size;
	}
	return size;
}

bool dethVideoAudioDecoder::pReadPage( ogg_page &page ){
	const int bufferSize = 4096;
	
	while( ogg_sync_pageout( &pSyncState, &page ) != 1 ){
		char * const buffer = ogg_sync_buffer( &pSyncState, bufferSize );
		if( ! buffer ){
			DETHROW( deeOutOfMemory );
		}
		
		const int readBytes = pReadFromFile( buffer, bufferSize );
		if( readBytes == 0 ){
			return false; // end of file
		}
		
		if( ogg_sync_wrote( &pSyncState, readBytes ) ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	return true;
}

void dethVideoAudioDecoder::pRewind(){
	ogg_sync_reset( &pSyncState );
	ogg_stream_reset( &pStreamState );
	
	// rewind to the beginning of the file. an optimization would be to store
	// the file position of the first page that contained video data
	GetFile()->SetPosition( 0 );
	pFileRemaining = pFileSize;
	
	// seek until the first page is found which contains data packets
	// of the right stream. once found add it to the decoder
	bool keepRunning = true;
	ogg_packet packet;
	ogg_page page;
	
	while( keepRunning && pReadPage( page ) ){
		if( ogg_page_serialno( &page ) != pSerial ){
			continue;
		}
		
		if( ogg_stream_pagein( &pStreamState, &page ) != 0 ){
			DETHROW( deeOutOfMemory );
		}
		
		while( ogg_stream_packetpeek( &pStreamState, &packet ) == 1 ){
			if( vorbis_synthesis_idheader( &packet ) == 1 ){
				vorbis_synthesis_trackonly( &pDecoderBlock, &packet );
				break;
			}
			ogg_stream_packetout( &pStreamState, &packet );
		}
	}
	
	vorbis_synthesis_restart( &pDecoderContext );
	pCurSample = 0;
}

void dethVideoAudioDecoder::pSeek( int position ){
	// if the seek goes backwards restart from the beginning of the file
	if( position < pCurSample ){
		pRewind();
	}
	
	// read audio until the right time is found
	float **samples = NULL;
	
	while( pCurSample < position ){
		int sampleCount = vorbis_synthesis_pcmout( &pDecoderContext, &samples );
		if( sampleCount == 0 ){
			bool keepGoing = false;
			ogg_packet packet;
			ogg_page page;
			
			while( true ){
				if( ogg_stream_packetout( &pStreamState, &packet ) == 1 ){
					if( vorbis_synthesis( &pDecoderBlock, &packet ) ){
						continue; // not data packet (for example header) or bad packet. read on
					}
					if( vorbis_synthesis_blockin( &pDecoderContext, &pDecoderBlock ) ){
						DETHROW( deeInvalidAction );
					}
					keepGoing = true;
					break;
					
				}else if( pReadPage( page ) == 0 ){
					if( ogg_page_serialno( &page ) != pSerial ){
						continue;
					}
					if( ogg_stream_pagein( &pStreamState, &page ) ){
						DETHROW( deeInvalidAction );
					}
					
				}else{
					break;
				}
			}
			
			if( keepGoing ){
				continue;
				
			}else{
				break;
			}
		}
		
		const int remaining = position - pCurSample;
		if( sampleCount > remaining ){
			sampleCount = remaining;
		}
		
		if( vorbis_synthesis_read( &pDecoderContext, sampleCount ) ){
			DETHROW( deeInvalidAction );
		}
		pCurSample += sampleCount;
	}
	
	vorbis_synthesis_restart( &pDecoderContext );
}
