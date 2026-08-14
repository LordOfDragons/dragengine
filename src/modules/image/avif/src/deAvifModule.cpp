/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <avif/avif.h>
#include <cstring>

#include "deAvifModule.h"
#include "deAvifImageInfo.h"
#include "deAvifIO.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/image/deImage.h>


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *AvifCreateModule(deLoadableModule *loadableModule);
#ifdef __cplusplus
}
#endif
#endif


// Entry Point
////////////////

deBaseModule *AvifCreateModule(deLoadableModule *loadableModule){
	try{
		return new deAvifModule(*loadableModule);
		
	}catch(const deException &){
		return nullptr;
	}
}


// Class deAvifModule
///////////////////////

// Constructor, destructor
////////////////////////////

deAvifModule::deAvifModule(deLoadableModule &loadableModule) :
deBaseImageModule(loadableModule){
}

deAvifModule::~deAvifModule() = default;


// Loading, Saving
////////////////////

deBaseImageInfo *deAvifModule::InitLoadImage(decBaseFileReader &file){
	return new deAvifImageInfo(file);
}

void deAvifModule::LoadImage(decBaseFileReader &file, deImage &image, deBaseImageInfo &infos){
	const int componentCount = infos.GetComponentCount();
	const int bitCount = infos.GetBitCount();
	const int height = infos.GetHeight();
	const int width = infos.GetWidth();
	avifImage *decodedImage = nullptr;
	bool isDataAllocated = false;
	avifRGBImage rgbImage{};
	
	deAvifIO ioHandler(file);
	
	auto decoder = avifDecoderCreate();
	DEASSERT_NOTNULL(decoder)
	
	try{
		int y, x;
		
		avifDecoderSetIO(decoder, &ioHandler.GetIO());
		
		decodedImage = avifImageCreateEmpty();
		DEASSERT_NOTNULL(decodedImage)
		
		auto result = avifDecoderRead(decoder, decodedImage);
		if(result != AVIF_RESULT_OK){
			DETHROW_INFO(deeInvalidFileFormat, avifResultToString(result));
		}
		
		// create RGB image for conversion
		avifRGBImageSetDefaults(&rgbImage, decodedImage);
		
		if(componentCount == 1){
			rgbImage.format = AVIF_RGB_FORMAT_GRAY;
			
		}else if(componentCount == 2){
			rgbImage.format = AVIF_RGB_FORMAT_GRAYA;
			
		}else if(componentCount == 3){
			rgbImage.format = AVIF_RGB_FORMAT_RGB;
			
		}else if(componentCount == 4){
			rgbImage.format = AVIF_RGB_FORMAT_RGBA;
		}
		rgbImage.depth = bitCount;
		
		// allocate RGB buffer for conversion
		result = avifRGBImageAllocatePixels(&rgbImage);
		if(result != AVIF_RESULT_OK){
			DETHROW(deeOutOfMemory);
		}
		isDataAllocated = true;
		
		result = avifImageYUVToRGB(decodedImage, &rgbImage);
		if(result != AVIF_RESULT_OK){
			DETHROW_INFO(deeInvalidFileFormat, avifResultToString(result));
		}
		
		const int rgbRowBytes = (int)rgbImage.rowBytes;
		const auto rgbData = rgbImage.pixels;
		
		// copy pixels to image
		if(bitCount == 16){
			if(componentCount == 1){
				auto dest = image.GetDataGrayscale16();
				for(y=0; y<height; y++){
					auto srcRow = reinterpret_cast<const uint16_t*>(rgbData + rgbRowBytes * y);
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = (unsigned short)srcRow[x];
					}
				}
				
			}else if(componentCount == 2){
				auto dest = image.GetDataGrayscaleAlpha16();
				for(y=0; y<height; y++){
					auto srcRow = reinterpret_cast<const uint16_t*>(rgbData + rgbRowBytes * y);
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = (unsigned short)*(srcRow++);
						destRow[x].alpha = (unsigned short)*(srcRow++);
					}
				}
				
			}else if(componentCount == 3){
				auto dest = image.GetDataRGB16();
				for(y=0; y<height; y++){
					auto srcRow = reinterpret_cast<const uint16_t*>(rgbData + rgbRowBytes * y);
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].red = (unsigned short)*(srcRow++);
						destRow[x].green = (unsigned short)*(srcRow++);
						destRow[x].blue = (unsigned short)*(srcRow++);
					}
				}
				
			}else if(componentCount == 4){
				auto dest = image.GetDataRGBA16();
				for(y=0; y<height; y++){
					auto srcRow = reinterpret_cast<const uint16_t*>(rgbData + rgbRowBytes * y);
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].red = (unsigned short)*(srcRow++);
						destRow[x].green = (unsigned short)*(srcRow++);
						destRow[x].blue = (unsigned short)*(srcRow++);
						destRow[x].alpha = (unsigned short)*(srcRow++);
					}
				}
			}
			
		}else{
			if(componentCount == 1){
				auto dest = image.GetDataGrayscale8();
				for(y=0; y<height; y++){
					auto srcRow = rgbData + rgbRowBytes * y;
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = srcRow[x];
					}
				}
				
			}else if(componentCount == 2){
				auto dest = image.GetDataGrayscaleAlpha8();
				for(y=0; y<height; y++){
					auto srcRow = rgbData + rgbRowBytes * y;
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = *(srcRow++);
						destRow[x].alpha = *(srcRow++);
					}
				}
				
			}else if(componentCount == 3){
				auto dest = image.GetDataRGB8();
				for(y=0; y<height; y++){
					auto srcRow = rgbData + rgbRowBytes * y;
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].red = *(srcRow++);
						destRow[x].green = *(srcRow++);
						destRow[x].blue = *(srcRow++);
					}
				}
				
			}else if(componentCount == 4){
				auto dest = image.GetDataRGBA8();
				for(y=0; y<height; y++){
					auto srcRow = rgbData + rgbRowBytes * y;
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].red = *(srcRow++);
						destRow[x].green = *(srcRow++);
						destRow[x].blue = *(srcRow++);
						destRow[x].alpha = *(srcRow++);
					}
				}
			}
		}
		
	}catch(const deException &){
		if(isDataAllocated){
			avifRGBImageFreePixels(&rgbImage);
		}
		if(decodedImage){
			avifImageDestroy(decodedImage);
		}
		avifDecoderDestroy(decoder);
		throw;
	}
	
	avifRGBImageFreePixels(&rgbImage);
	avifImageDestroy(decodedImage);
	avifDecoderDestroy(decoder);
}

void deAvifModule::SaveImage(decBaseFileWriter &file, const deImage &image){
	if(image.GetBitCount() != 8 && image.GetBitCount() != 16){
		DETHROW_INFO(deeInvalidParam, "Only 8-Bit or 16-Bit supported");
	}
	
	auto encoder = avifEncoderCreate();
	if(!encoder){
		DETHROW(deeOutOfMemory);
	}
	
	avifImage *encodeImage = nullptr;
	const int componentCount = image.GetComponentCount();
	const int bitCount = image.GetBitCount();
	const int height = image.GetHeight();
	const int width = image.GetWidth();
	
	avifRGBImage rgbImage{};
	bool isDataAllocated = false;
	
	avifRWData output = AVIF_DATA_EMPTY;
	
	try{
		encodeImage = avifImageCreate(width, height, bitCount, AVIF_PIXEL_FORMAT_YUV420);
		DEASSERT_NOTNULL(encodeImage)
		
		// create encode RGB image
		avifRGBImageSetDefaults(&rgbImage, encodeImage);
		
		switch(componentCount){
		case 1:
			rgbImage.format = AVIF_RGB_FORMAT_GRAY;
			break;
			
		case 2:
			rgbImage.format = AVIF_RGB_FORMAT_GRAYA;
			break;
			
		case 3:
			rgbImage.format = AVIF_RGB_FORMAT_RGB;
			break;
			
		case 4:
			rgbImage.format = AVIF_RGB_FORMAT_RGBA;
			break;
		}
		rgbImage.depth = bitCount;
		
		if(avifRGBImageAllocatePixels(&rgbImage) != AVIF_RESULT_OK){
			DETHROW(deeOutOfMemory);
		}
		isDataAllocated = true;
		
		// write pixels to encode image
		if(bitCount == 16){
			if(componentCount == 1){
				const auto src = image.GetDataGrayscale16();
				for(int y=0; y<height; y++){
					auto destRow = reinterpret_cast<uint16_t*>(
						rgbImage.pixels + rgbImage.rowBytes * y);
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						destRow[x] = (uint16_t)srcRow[x].value;
					}
				}
				
			}else if(componentCount == 2){
				const auto src = image.GetDataGrayscaleAlpha16();
				for(int y=0; y<height; y++){
					auto destRow = reinterpret_cast<uint16_t*>(
						rgbImage.pixels + rgbImage.rowBytes * y);
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						*(destRow++) = (uint16_t)srcRow[x].value;
						*(destRow++) = (uint16_t)srcRow[x].alpha;
					}
				}
				
			}else if(componentCount == 3){
				const auto src = image.GetDataRGB16();
				for(int y=0; y<height; y++){
					auto destRow = reinterpret_cast<uint16_t*>(
						rgbImage.pixels + rgbImage.rowBytes * y);
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						*(destRow++) = (uint16_t)srcRow[x].red;
						*(destRow++) = (uint16_t)srcRow[x].green;
						*(destRow++) = (uint16_t)srcRow[x].blue;
					}
				}
				
			}else if(componentCount == 4){
				const auto src = image.GetDataRGBA16();
				for(int y=0; y<height; y++){
					auto destRow = reinterpret_cast<uint16_t*>(
						rgbImage.pixels + rgbImage.rowBytes * y);
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						*(destRow++) = (uint16_t)srcRow[x].red;
						*(destRow++) = (uint16_t)srcRow[x].green;
						*(destRow++) = (uint16_t)srcRow[x].blue;
						*(destRow++) = (uint16_t)srcRow[x].alpha;
					}
				}
			}
			
		}else{
			if(componentCount == 1){
				const auto src = image.GetDataGrayscale8();
				for(int y=0; y<height; y++){
					auto destRow = rgbImage.pixels + rgbImage.rowBytes * y;
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						destRow[x] = (uint8_t)srcRow[x].value;
					}
				}
				
			}else if(componentCount == 2){
				const auto src = image.GetDataGrayscaleAlpha8();
				for(int y=0; y<height; y++){
					auto destRow = rgbImage.pixels + rgbImage.rowBytes * y;
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						*(destRow++) = (uint8_t)srcRow[x].value;
						*(destRow++) = (uint8_t)srcRow[x].alpha;
					}
				}
				
			}else if(componentCount == 3){
				const auto src = image.GetDataRGB8();
				for(int y=0; y<height; y++){
					auto destRow = rgbImage.pixels + rgbImage.rowBytes * y;
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						*(destRow++) = (uint8_t)srcRow[x].red;
						*(destRow++) = (uint8_t)srcRow[x].green;
						*(destRow++) = (uint8_t)srcRow[x].blue;
					}
				}
				
			}else if(componentCount == 4){
				const auto src = image.GetDataRGBA8();
				for(int y=0; y<height; y++){
					auto destRow = rgbImage.pixels + rgbImage.rowBytes * y;
					auto srcRow = src + width * y;
					
					for(int x=0; x<width; x++){
						*(destRow++) = (uint8_t)srcRow[x].red;
						*(destRow++) = (uint8_t)srcRow[x].green;
						*(destRow++) = (uint8_t)srcRow[x].blue;
						*(destRow++) = (uint8_t)srcRow[x].alpha;
					}
				}
			}
		}
		
		// convert RGB to YUV
		auto result = avifImageRGBToYUV(encodeImage, &rgbImage);
		if(result != AVIF_RESULT_OK){
			DETHROW_INFO(deeWriteFile, avifResultToString(result));
		}
		
		// encode image
		encoder->quality = 80;
		encoder->qualityAlpha = 80;
		encoder->speed = 6; // 0-10, higher is faster
		
		result = avifEncoderWrite(encoder, encodeImage, &output);
		if(result != AVIF_RESULT_OK){
			DETHROW_INFO(deeWriteFile, avifResultToString(result));
		}
		
		file.Write(output.data, (int)output.size);
		
	}catch(const deException &){
		if(output.data){
			avifRWDataFree(&output);
		}
		if(isDataAllocated){
			avifRGBImageFreePixels(&rgbImage);
		}
		if(encodeImage){
			avifImageDestroy(encodeImage);
		}
		avifEncoderDestroy(encoder);
		throw;
	}
	
	avifRWDataFree(&output);
	avifRGBImageFreePixels(&rgbImage);
	avifImageDestroy(encodeImage);
	avifEncoderDestroy(encoder);
}


#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deAvifModuleInternal : public deInternalModule{
public:
	using Ref = deTObjectReference<deAvifModuleInternal>;
	
	deAvifModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("Avif");
		SetDescription("Handles images saved in the AVIF format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtImage);
		SetDirectoryName("avif");
		GetPatternList().Add(".avif");
		SetDefaultExtension(".avif");
		SetNoCompress(true);
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(AvifCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> deAvifRegisterInternalModule(deModuleSystem *system){
	return deAvifModuleInternal::Ref::New(system);
}
#endif
