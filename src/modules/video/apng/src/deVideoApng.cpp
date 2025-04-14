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

#include "deVideoApng.h"
#include "deapngDecoder.h"
#include "deapngReader.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/systems/modules/video/deBaseVideoInfo.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *APNGCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *APNGCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deVideoApng( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deVideoApng
//////////////////////

// Constructor, destructor
////////////////////////////

deVideoApng::deVideoApng( deLoadableModule &loadableModule ) :
deBaseVideoModule( loadableModule ){
}

deVideoApng::~deVideoApng(){
}



// Management
///////////////

void deVideoApng::InitLoadVideo( decBaseFileReader &reader, deBaseVideoInfo &info ){
	deapngReader apngReader( *this, &reader );
	
	info.SetWidth( apngReader.GetWidth() );
	info.SetHeight( apngReader.GetHeight() );
	info.SetComponentCount( apngReader.GetComponentCount() );
	info.SetFrameCount( apngReader.GetFrameCount() );
	info.SetFrameRate( apngReader.GetFrameRate() );
}

void deVideoApng::SaveVideo( decBaseFileWriter &reader, const deVideo &video ){
	// not supported yet
}

deBaseVideoDecoder *deVideoApng::CreateDecoder( decBaseFileReader *reader ){
	if( ! reader ){
		DETHROW( deeInvalidParam );
	}
	return new deapngDecoder( *this, reader );
}

deBaseVideoAudioDecoder *deVideoApng::CreateAudioDecoder( decBaseFileReader* ){
	return NULL;
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deapngModuleInternal : public deInternalModule{
public:
	deapngModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("APNG");
		SetDescription("Handles videos in the Animated PNG format.");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtVideo);
		SetDirectoryName("apng");
		GetPatternList().Add(".apng");
		SetDefaultExtension(".apng");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(APNGCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deapngRegisterInternalModule(deModuleSystem *system){
	return new deapngModuleInternal(system);
}
#endif
