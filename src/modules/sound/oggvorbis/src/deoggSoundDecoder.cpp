/* 
 * Drag[en]gine OGG Vorbis Sound Module
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

#include "deoggSoundDecoder.h"
#include "deSoundOGG.h"

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>



// Callbacks
//////////////

static size_t fOggDecodeRead( void *ptr, size_t size, size_t nmemb, void *datasource ){
	decBaseFileReader *reader = ( decBaseFileReader* )datasource;
	int position = reader->GetPosition();
	int readSize = ( int )( size * nmemb );
	int remaining = reader->GetLength() - position;
	
	if( readSize > remaining ) readSize = remaining;
	
	reader->Read( ptr, readSize );
	
	return readSize;
}

static int fOggDecodeSeek( void *datasource, ogg_int64_t offset, int whence ){
	decBaseFileReader *reader = ( decBaseFileReader* )datasource;
	
	if( whence == SEEK_SET ){
		reader->SetPosition( ( int )offset );
		
	}else if( whence == SEEK_CUR ){
		reader->MovePosition( ( int )offset );
		
	}else if( whence == SEEK_END ){
		reader->SetPositionEnd( ( int )offset );
	}
	
	return 0;
}

static int fOggDecodeClose( void *datasource ){
	return 0;
}

static long fOggDecodeTell( void *datasource ){
	decBaseFileReader *reader = ( decBaseFileReader* )datasource;
	
	return reader->GetPosition();
}



// Class deoggSoundDecoder
////////////////////////////

// Constructor, destructor
////////////////////////////

deoggSoundDecoder::deoggSoundDecoder( deSoundOGG *module, decBaseFileReader *file ) :
deBaseSoundDecoder( file ){
	ov_callbacks callbacks;
	vorbis_info *fileInfos;
	bool isOpen = false;
	int sampleCount;
	
	pModule = module;
	
	try{
		// setup the callbacks
		callbacks.read_func = fOggDecodeRead;
		callbacks.seek_func = fOggDecodeSeek;
		callbacks.close_func = fOggDecodeClose;
		callbacks.tell_func = fOggDecodeTell;
		
		// open the file
		if( ov_open_callbacks( file, &pOggFile, NULL, 0, callbacks ) < 0 ){
			module->LogError( "Invalid OGG file." );
			DETHROW( deeInvalidParam );
		}
		isOpen = true;
		
		// fetch the file properties
		fileInfos = ov_info( &pOggFile, -1 );
		if( ! fileInfos ){
			module->LogError( "Could not get infos from OGG file." );
			DETHROW( deeInvalidParam );
		}
		
		if( fileInfos->channels < 1 ){
			module->LogErrorFormat( "Invalid channel count %i.", fileInfos->channels );
			DETHROW( deeInvalidParam );
		}
		
		sampleCount = ( int )ov_pcm_total( &pOggFile, -1 );
		if( sampleCount == OV_EINVAL ){
			module->LogError( "Could not determine the number of samples in the sound file." );
			DETHROW( deeInvalidParam );
		}
		
		// store informations
		pBytesPerSample = 2;
		pSampleRate = fileInfos->rate;
		pChannelCount = fileInfos->channels;
		pSampleCount = sampleCount;
		
		if( pBytesPerSample == 1 ){
			pSigned = 0;
		}else{
			pSigned = 1;
		}
		
	}catch( const deException &e ){
		if( isOpen ) ov_clear( &pOggFile );
		e.PrintError();
		throw;
	}
}

deoggSoundDecoder::~deoggSoundDecoder(){
	ov_clear( &pOggFile );
}



// Management
///////////////

int deoggSoundDecoder::GetPosition(){
	return ( int )ov_pcm_tell( &pOggFile );
}

void deoggSoundDecoder::SetPosition( int position ){
	if( pSampleCount == 0 ){
		return;
	}
	ov_pcm_seek( &pOggFile, decMath::clamp( position, 0, pSampleCount ) );
}

int deoggSoundDecoder::ReadSamples( void *buffer, int size ){
	if( ! buffer || size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size == 0 ){
		return 0;
	}
	
	char *samples = ( char* )buffer;
	int totalReadBytes = 0;
	
	// read the file
	/*
	long ov_read(OggVorbis_File *vf, char *buffer, int length, int bigendianp, int word,
	int sgned, int *bitstream);
	
	bigendianp: Specifies big or little endian byte packing. 0 for little endian,
	            1 for b ig endian. Typical value is 0.
	word: Specifies word size. Possible arguments are 1 for 8-bit samples, or 2 or
	      16-bit samples. Typical value is 2.
	sgned: Signed or unsigned data. 0 for unsigned, 1 for signed. Typically 1.
	*/
	while( size > 0 ){
		const long readBytes = ov_read( &pOggFile, samples, size, 0, pBytesPerSample, pSigned, &pSection );
		if( readBytes == 0 ){
			size = 0;
			
		// ( readBytes < 0 ) => error in stream, can be ignored safely ( really? )
		}else if( readBytes > 0 ){
			samples += readBytes;
			size -= readBytes;
			totalReadBytes += readBytes;
		}
	}
	
	return totalReadBytes;
}
