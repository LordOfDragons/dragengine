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
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "deSoundOGG.h"
#include "deoggSoundDecoder.h"

#include <dragengine/resources/sound/deSound.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/systems/modules/sound/deBaseSoundInfo.h>



// Export Definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *OGGCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Function
///////////////////

deBaseModule *OGGCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deSoundOGG( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Callbacks
//////////////

static size_t fOggRead( void *ptr, size_t size, size_t nmemb, void *datasource ){
	decBaseFileReader *reader = ( decBaseFileReader* )datasource;
	int position = reader->GetPosition();
	int readSize = ( int )( size * nmemb );
	int remaining = reader->GetLength() - position;
	
	if( readSize > remaining ) readSize = remaining;
	
	reader->Read( ptr, readSize );
	
	return readSize;
}

static int fOggSeek( void *datasource, ogg_int64_t offset, int whence ){
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

static int fOggClose( void *datasource ){
	return 0;
}

static long fOggTell( void *datasource ){
	decBaseFileReader *reader = ( decBaseFileReader* )datasource;
	
	return reader->GetPosition();
}



// Class deSoundOGG
/////////////////////

// Constructor, destructor
////////////////////////////

deSoundOGG::deSoundOGG( deLoadableModule &loadableModule ) :
deBaseSoundModule( loadableModule ){
}

deSoundOGG::~deSoundOGG(){
}



// Management
///////////////

void deSoundOGG::InitLoadSound( decBaseFileReader &file, deBaseSoundInfo &info ){
	bool needsCloseFile = false;
	OggVorbis_File oggFile;
	ov_callbacks callbacks;
	vorbis_info *fileInfos;
	int sampleCount;
	
	try{
		// setup the callbacks
		callbacks.read_func = fOggRead;
		callbacks.seek_func = fOggSeek;
		callbacks.close_func = fOggClose;
		callbacks.tell_func = fOggTell;
		
		// open the file
		if( ov_open_callbacks( &file, &oggFile, NULL, 0, callbacks ) < 0 ){
			LogError( "Invalid OGG file." );
			DETHROW( deeInvalidParam );
		}
		needsCloseFile = true;
		
		// fetch the file properties
		fileInfos = ov_info( &oggFile, -1 );
		if( ! fileInfos ){
			LogError( "Could not get infos from OGG file." );
			DETHROW( deeInvalidParam );
		}
		
		if( fileInfos->channels < 1 ){
			LogErrorFormat( "Invalid channel count %i.", fileInfos->channels );
			DETHROW( deeInvalidParam );
		}
		
		sampleCount = ( int )ov_pcm_total( &oggFile, -1 );
		if( sampleCount == OV_EINVAL ){
			LogError( "Could not determine the number of samples in the sound file." );
			DETHROW( deeInvalidParam );
		}
		
		// store information
		info.SetBytesPerSample( 2 );
		info.SetSampleRate( fileInfos->rate );
		info.SetSampleCount( sampleCount );
		info.SetChannelCount( fileInfos->channels );
		info.SetIsCompressed( true );
		
		// close the file
		ov_clear( &oggFile );
		
	}catch( const deException & ){
		if( needsCloseFile ){
			ov_clear( &oggFile );
		}
		throw;
	}
}

void deSoundOGG::SaveSound( decBaseFileWriter &file, const deSound &sound ){
	// not supported yet
}

deBaseSoundDecoder *deSoundOGG::CreateDecoder( decBaseFileReader *reader ){
	return new deoggSoundDecoder( this, reader );
}
