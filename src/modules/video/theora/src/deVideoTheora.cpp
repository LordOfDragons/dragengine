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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class dethModuleInternal : public deInternalModule{
public:
	dethModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("Theora");
		SetDescription("Handles videos in the theora format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtVideo);
		SetDirectoryName("theora");
		GetPatternList().Add(".ogv");
		SetDefaultExtension(".ogv");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(TheoraCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *dethRegisterInternalModule(deModuleSystem *system){
	return new dethModuleInternal(system);
}
#endif
