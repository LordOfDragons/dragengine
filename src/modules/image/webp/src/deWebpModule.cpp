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

#include <webp/decode.h>
#include <webp/encode.h>

#include "deWebpModule.h"
#include "deWebpImageInfo.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/image/deImage.h>


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *WebpCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
#endif


// Entry Point
////////////////

deBaseModule *WebpCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deWebpModule( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deWebpModule
///////////////////////

// Constructor, destructor
////////////////////////////

deWebpModule::deWebpModule( deLoadableModule &loadableModule ) :
deBaseImageModule( loadableModule ){
}

deWebpModule::~deWebpModule(){
}



// Loading, Saving
////////////////////

deBaseImageInfo *deWebpModule::InitLoadImage( decBaseFileReader &file ){
	return new deWebpImageInfo( file );
}

void deWebpModule::LoadImage( decBaseFileReader&, deImage &image, deBaseImageInfo &infos ){
	deWebpImageInfo &webpInfo = ( deWebpImageInfo& )infos;
	uint8_t *readTarget = nullptr;
	sRGBA8 *bufferRGBA = nullptr;
	sRGB8 *bufferRGB = nullptr;
	uint8_t *result = nullptr;
	int i;
	
	try{
		const int pixelCount = image.GetWidth() * image.GetHeight();
		
		if( webpInfo.GetHasAlpha() ){
			if( webpInfo.GetIsGrayscale() ){
				bufferRGBA = new sRGBA8[ pixelCount ];
				readTarget = ( uint8_t* )bufferRGBA;
				
			}else{
				readTarget = ( uint8_t* )image.GetDataRGBA8();
			}
			
			result = WebPDecodeRGBAInto( ( const uint8_t* )webpInfo.GetData().GetPointer(),
				webpInfo.GetData().GetLength(), readTarget, pixelCount * 4, image.GetWidth() * 4 );
			
		}else{
			if( webpInfo.GetIsGrayscale() ){
				bufferRGB = new sRGB8[ pixelCount ];
				readTarget = ( uint8_t* )bufferRGB;
				
			}else{
				readTarget = ( uint8_t* )image.GetDataRGB8();
			}
			
			result = WebPDecodeRGBInto( ( const uint8_t* )webpInfo.GetData().GetPointer(),
				webpInfo.GetData().GetLength(), readTarget, pixelCount * 3, image.GetWidth() * 3 );
		}
		
		if( ! result ){
			DETHROW( deeInvalidFileFormat );
		}
		
		if( bufferRGBA ){
			sGrayscaleAlpha8 * const dest = image.GetDataGrayscaleAlpha8();
			
			for( i=0; i<pixelCount; i++ ){
				dest[ i ].value = bufferRGBA[ i ].red;
				dest[ i ].alpha = bufferRGBA[ i ].alpha;
			}
			
			delete [] bufferRGBA;
			bufferRGBA = nullptr;
		}
		
		if( bufferRGB ){
			sGrayscale8 * const dest = image.GetDataGrayscale8();
			
			for( i=0; i<pixelCount; i++ ){
				dest[ i ].value = bufferRGB[ i ].red;
			}
			
			delete [] bufferRGB;
			bufferRGB = nullptr;
		}
		
	}catch( const deException & ){
		if( bufferRGBA ){
			delete [] bufferRGBA;
		}
		if( bufferRGB ){
			delete [] bufferRGB;
		}
		throw;
	}
}

void deWebpModule::SaveImage( decBaseFileWriter &file, const deImage &image ){
	if( image.GetBitCount() != 8 ){
		DETHROW_INFO( deeInvalidParam, "Only 8-Bit supporeted" );
	}
	
	if( image.GetComponentCount() < 3 ){
		DETHROW_INFO( deeInvalidParam, "Only 3 or 4 components supported" );
	}
	
	size_t size = 0;
	uint8_t *output = nullptr;
	const float quality = 95.0f;
	
	try{
		if( image.GetComponentCount() == 4 ){
			// WebPEncodeLosslessRGB
			size = WebPEncodeRGBA( ( const uint8_t* )image.GetDataRGBA8(), image.GetWidth(),
				image.GetHeight(), image.GetWidth() * 4, quality, &output );
			
		}else{
			size = WebPEncodeRGB( ( const uint8_t* )image.GetDataRGB8(), image.GetWidth(),
				image.GetHeight(), image.GetWidth() * 3, quality, &output );
		}
		
		if( size == 0 || ! output ){
			DETHROW( deeWriteFile );
		}
		
		file.Write( output, ( int )size );
		
		WebPFree( output );
		
	}catch( const deException & ){
		if( output ){
			WebPFree( output );
		}
		throw;
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deWebpModuleInternal : public deInternalModule{
public:
	deWebpModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("WebP");
		SetDescription("Handles images saved in the WebP format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtImage);
		SetDirectoryName("webp");
		GetPatternList().Add(".webp");
		SetDefaultExtension(".webp");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(WebpCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deWebpRegisterInternalModule(deModuleSystem *system){
	return new deWebpModuleInternal(system);
}
#endif
