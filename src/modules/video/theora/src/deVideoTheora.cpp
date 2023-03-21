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
#include <ogg/ogg.h>
#include <theora/codec.h>

#include "deVideoTheora.h"
#include "dethInfos.h"
#include "dethVideoDecoder.h"
#include "dethVideoAudioDecoder.h"
#include "dethOggReader.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/video/deVideo.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *TheoraCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *TheoraCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deVideoTheora( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deVideoTheora
////////////////////////

// Constructor, destructor
////////////////////////////

deVideoTheora::deVideoTheora( deLoadableModule &loadableModule ) :
deBaseVideoModule( loadableModule ){
}

deVideoTheora::~deVideoTheora(){
}



// Management
///////////////

void deVideoTheora::InitLoadVideo( decBaseFileReader &reader, deBaseVideoInfo &info ){
	dethOggReader oggReader( *this, reader );
	dethInfos oggInfo;
	
	oggReader.ReadStreamHeaders( oggInfo );
	if( ! oggInfo.GetHeaderFinished() ){
		DETHROW_INFO( deeReadFile, reader.GetFilename() );
	}
	
	info.SetWidth( oggInfo.GetWidth() );
	info.SetHeight( oggInfo.GetHeight() );
	info.SetComponentCount( oggInfo.GetComponentCount() );
	info.SetFrameCount( oggInfo.GetFrameCount() );
	info.SetFrameRate( oggInfo.GetFrameRate() );
	info.SetColorConversionMatrix( oggInfo.GetColorConversionMatrix() );
	
	reader.SetPosition( 0 );
	dethVideoAudioDecoder audioDecoder( *this, reader );
	info.SetBytesPerSample( audioDecoder.GetBytesPerSample() );
	info.SetSampleRate( audioDecoder.GetSampleRate() );
	info.SetSampleCount( audioDecoder.GetSampleCount() );
	info.SetChannelCount( audioDecoder.GetChannelCount() );
	
	/*
	LogInfoFormat( "InitLoadVideo(%s): size=%dx%d format=%d frames=%d frameRate=%g"
		" bps=%d channels=%d sampleRate=%d samples=%d", reader.GetFilename(),
		oggInfo.GetWidth(), oggInfo.GetHeight(), oggInfo.GetPixelFormat(), oggInfo.GetFrameCount(),
		oggInfo.GetFrameRate(), audioDecoder.GetBytesPerSample(), audioDecoder.GetChannelCount(),
		audioDecoder.GetSampleRate(), audioDecoder.GetSampleCount() );
	*/
}

void deVideoTheora::SaveVideo( decBaseFileWriter &reader, const deVideo &video ){
	// not supported yet
}

deBaseVideoDecoder *deVideoTheora::CreateDecoder( decBaseFileReader *reader ){
	return new dethVideoDecoder( *this, reader );
}

deBaseVideoAudioDecoder *deVideoTheora::CreateAudioDecoder( decBaseFileReader *reader ){
	dethVideoAudioDecoder * const decoder = new dethVideoAudioDecoder( *this, *reader );
	if( decoder->GetSampleCount() > 0 ){
		return decoder;
		
	}else{
		delete decoder;
		return NULL;
	}
}
