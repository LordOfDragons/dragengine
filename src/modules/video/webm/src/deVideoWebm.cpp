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

#include <webm/webm_parser.h>

#include "deVideoWebm.h"
#include "dewmInfos.h"
#include "dewmWebmCallbackInfos.h"
#include "dewmWebmReader.h"
#include "dewmVideoDecoder.h"
#include "dewmVideoAudioDecoder.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/video/deVideo.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *WebmCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *WebmCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deVideoWebm( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deVideoWebm
////////////////////////

// Constructor, destructor
////////////////////////////

deVideoWebm::deVideoWebm( deLoadableModule &loadableModule ) :
deBaseVideoModule( loadableModule ){
}

deVideoWebm::~deVideoWebm(){
}



// Management
///////////////

void deVideoWebm::InitLoadVideo( decBaseFileReader &reader, deBaseVideoInfo &info ){
	dewmInfos webmInfo;
	dewmWebmCallbackInfos webmCallback( *this, webmInfo );
	dewmWebmReader webmReader( reader );
	webm::WebmParser parser;
	DEASSERT_TRUE( parser.Feed( &webmCallback, &webmReader ).ok() )
	
	DEASSERT_TRUE( webmInfo.GetVideoCodec() != dewmInfos::evcUnsupported )
	
	info.SetWidth( webmInfo.GetWidth() );
	info.SetHeight( webmInfo.GetHeight() );
	info.SetComponentCount( webmInfo.GetComponentCount() );
	info.SetFrameCount( webmInfo.GetFrameCount() );
	info.SetFrameRate( webmInfo.GetFrameRate() );
	info.SetColorConversionMatrix( webmInfo.GetColorConversionMatrix() );
	
	if( webmInfo.GetAudioCodec() != dewmInfos::eacUnsupported ){
		info.SetBytesPerSample( webmInfo.GetBytesPerSample() );
		info.SetSampleRate( webmInfo.GetSampleRate() );
		info.SetSampleCount( webmInfo.GetSampleCount() );
		info.SetChannelCount( webmInfo.GetChannelCount() );
	}
	
	LogInfoFormat( "InitLoadVideo(%s): size=%dx%d components=%d frames=%d frameRate=%g"
		" bps=%d channels=%d sampleRate=%d samples=%d", reader.GetFilename(),
		webmInfo.GetWidth(), webmInfo.GetHeight(), webmInfo.GetComponentCount(),
		webmInfo.GetFrameCount(), webmInfo.GetFrameRate(), webmInfo.GetBytesPerSample(),
		webmInfo.GetChannelCount(), webmInfo.GetSampleRate(), webmInfo.GetSampleCount() );
}

void deVideoWebm::SaveVideo( decBaseFileWriter &reader, const deVideo &video ){
	// not supported yet
}

deBaseVideoDecoder *deVideoWebm::CreateDecoder( decBaseFileReader *reader ){
	return new dewmVideoDecoder( *this, reader );
}

deBaseVideoAudioDecoder *deVideoWebm::CreateAudioDecoder( decBaseFileReader *reader ){
	return new dewmVideoAudioDecoder( *this, reader );
}
