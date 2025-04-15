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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class dewebmModuleInternal : public deInternalModule{
public:
	dewebmModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("WebM");
		SetDescription("Handles videos in the WebM format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtVideo);
		SetDirectoryName("webm");
		GetPatternList().Add(".webm");
		SetDefaultExtension(".webm");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(WebmCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *dewebmRegisterInternalModule(deModuleSystem *system){
	return new dewebmModuleInternal(system);
}
#endif
