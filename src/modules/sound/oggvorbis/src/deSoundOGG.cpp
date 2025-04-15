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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deoggModuleInternal : public deInternalModule{
public:
	deoggModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("OggVorbis");
		SetDescription("Handles sounds in ogg vorbis format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtSound);
		SetDirectoryName("oggvorbis");
		GetPatternList().Add(".ogg");
		SetDefaultExtension(".ogg");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(OGGCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

DE_FUNC_USED deInternalModule *deoggRegisterInternalModule(deModuleSystem *system){
	return new deoggModuleInternal(system);
}
#endif
