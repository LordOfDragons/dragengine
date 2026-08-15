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

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avif/avif.h>

#ifdef OS_W32
	#include <dragengine/app/deOSWindows.h>
#else
	#include <sys/time.h>
#endif

#include "deAvif3DModule.h"
#include "deAvif3DImageInfo.h"
#include "deAvif3DTarball.h"
#include "deAvif3DIO.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>


// Structures
///////////////

struct sTarballHeader{/* byte offset */
	char name[100];			/*   0 */
	char mode[8];			/* 100 */
	char uid[8];			/* 108 */
	char gid[8];			/* 116 */
	char size[12];			/* 124 */
	char mtime[12];			/* 136 */
	char chksum[8];			/* 148 */
	char typeflag;			/* 156 */
	char linkname[100];		/* 157 */
	char magic[6];			/* 257 */
	char version[2];		/* 263 */
	char uname[32];			/* 265 */
	char gname[32];			/* 297 */
	char devmajor[8];		/* 329 */
	char devminor[8];		/* 337 */
	char prefix[155];		/* 345 */
	char padding[12];		/* 500 */
};							/* 512 */

#define REGTYPE				'0'		/* regular file */
#define AREGTYPE			'\0'	/* regular file */


// Class deAvif3DTarball
//////////////////////////

// Constructor, destructor
////////////////////////////

deAvif3DTarball::deAvif3DTarball(deAvif3DModule &module) :
pModule(module){
}

deAvif3DTarball::~deAvif3DTarball() = default;


// Management
///////////////

void deAvif3DTarball::Get3DImageInfos(deAvif3DImageInfo &info, decBaseFileReader &file){
	bool firstImage = true;
	sTarballHeader header;
	sImageInfo info2D;
	unsigned short z;
	int i;
	
	info.SetWidth(0);
	info.SetHeight(0);
	info.SetDepth(1);
	info.SetHasAlpha(false);
	info.SetIsGrayscale(false);
	info.SetBitCount(8);
	
	while(true){
		file.Read(&header, 512);
		
		if(!header.name[0] && !header.linkname[0]){
			break;
		}
		
		const int position = file.GetPosition();
		
		bool skip = true;
		int filesize = 0;
		
		for(i=0; i<11; i++){
			if(skip){
				if(header.size[i] != ' '){
					skip = false;
					filesize = (int)(header.size[i] - '0');
				}
				
			}else{
				if(header.size[i] == ' '){
					break;
				}
				filesize = filesize * 8 + (int)(header.size[i] - '0');
			}
		}
		
		int padding = 512 - (filesize % 512);
		if(padding == 512){
			padding = 0;
		}
		
		if(header.typeflag == REGTYPE || header.typeflag == AREGTYPE){
			z = decString(header.name).GetMiddle(1, -5).ToInt();

			if((int)z >= info.GetDepth()){
				info.SetDepth((int)z + 1);
			}
			
			Get2DImageInfos(info2D, file, filesize);
			
			if(firstImage){
				info.SetWidth(info2D.width);
				info.SetHeight(info2D.height);
				info.SetHasAlpha(info2D.hasAlpha);
				info.SetIsGrayscale(info2D.isGrayscale);
				info.SetBitCount(info2D.bitCount);
				firstImage = false;
				
			}else{
				if(info2D.width != info.GetWidth()
				|| info2D.height != info.GetHeight()
				|| info2D.hasAlpha != info.GetHasAlpha()
				|| info2D.isGrayscale != info.GetIsGrayscale()
				|| info2D.bitCount != info.GetBitCount()){
					pModule.LogErrorFormat(
						"the files in the archive '%s' do not match in size or format.",
						info.GetFilename().GetString());
					DETHROW_INFO(deeInvalidFileFormat, info.GetFilename());
				}
			}
		}
		
		file.SetPosition(position + filesize + padding);
	}
	
	if(firstImage){
		pModule.LogErrorFormat(
			"no images in the archive '%s'. at least one image is required.",
			info.GetFilename().GetString());
		DETHROW_INFO(deeInvalidFileFormat, info.GetFilename());
	}
}

void deAvif3DTarball::Load3DImage(deAvif3DImageInfo &infos, decBaseFileReader &file, deImage &image){
	DEASSERT_TRUE(image.GetBitCount() == infos.GetBitCount())
	
	const int strideLine = image.GetWidth() * image.GetComponentCount();
	const int strideImage = strideLine * image.GetHeight();
	char * const imageData = reinterpret_cast<char*>(image.GetData());
	sTarballHeader header;
	unsigned short z;
	int i;
	
	while(true){
		file.Read(&header, 512);
		
		if(!header.name[0] && !header.linkname[0]){
			break;
		}
		
		const int position = file.GetPosition();
		
		bool skip = true;
		int filesize = 0;
		
		for(i=0; i<11; i++){
			if(skip){
				if(header.size[i] != ' '){
					skip = false;
					filesize = (int)(header.size[i] - '0');
				}
				
			}else{
				if(header.size[i] == ' '){
					break;
				}
				filesize = filesize * 8 + (int)(header.size[i] - '0');
			}
		}
		
		int padding = 512 - (filesize % 512);
		if(padding == 512){
			padding = 0;
		}
		
		if(header.typeflag == REGTYPE || header.typeflag == AREGTYPE){
			z = decString(header.name).GetMiddle(1, -5).ToInt();
			Load2DImage(infos, file, filesize, imageData + strideImage * (int)z);
		}
		
		file.SetPosition(position + filesize + padding);
	}
}

void deAvif3DTarball::Save3DImage(decBaseFileWriter &file, const deImage &image){
	if(image.GetBitCount() != 8 && image.GetBitCount() != 16){
		DETHROW_INFO(deeInvalidParam, "Only 8-Bit or 16-Bit supported");
	}
	
	DEASSERT_TRUE(image.GetComponentCount() >= 1)
	
	char paddingBytes[512];
	struct timeval curtime;
	unsigned int checksum;
	int z, r;
	
	auto encoder = avifEncoderCreate();
	if(!encoder){
		DETHROW(deeOutOfMemory);
	}
	
	const int componentCount = image.GetComponentCount();
	const int bitCount = image.GetBitCount();
	const int height = image.GetHeight();
	const int width = image.GetWidth();
	const int strideImage = width * componentCount * (bitCount == 16 ? 2 : 1);
	const char * const imageData = reinterpret_cast<const char*>(image.GetData());
	
	// avif encoding in format 4:2:0 does not allow odd sizes. for this to work the image has
	// to be padded to even size and a clap box applied to restore the original size
	const int encodeWidth = width + (width % 2);
	const int encodeHeight = height + (height % 2);
	
	sTarballHeader header;
	memset(&header, '\0', 512);
	
	#ifdef OS_W32
		strcpy_s(&header.mode[0], sizeof(header.mode), "100600 ");
		strcpy_s(&header.uid[0], sizeof(header.uid),   "  1750 ");
		strcpy_s(&header.gid[0], sizeof(header.gid),   "   144 ");
	#else
		strcpy(&header.mode[0], "100600 ");
		strcpy(&header.uid[0],  "  1750 ");
		strcpy(&header.gid[0],  "   144 ");
	#endif
	
	#ifdef OS_W32
		{
		ULARGE_INTEGER x;
		ULONGLONG usec;
		static const ULONGLONG epoch_offset_us = 11644473600000000ULL;
		
		FILETIME filetime;
		GetSystemTimeAsFileTime(&filetime);
		
		x.LowPart = filetime.dwLowDateTime;
		x.HighPart = filetime.dwHighDateTime;
		usec = x.QuadPart / 10  -  epoch_offset_us;
		curtime.tv_sec  = (long)(usec / 1000000ULL);
		curtime.tv_usec = (long)(usec % 1000000ULL);
		}
	#else
		gettimeofday(&curtime, nullptr);
	#endif
	
	header.mtime[11] = ' ';
	for(r=10; r>=0; r--){
		if(curtime.tv_sec){
			header.mtime[r] = (char)('0' + (char)(curtime.tv_sec & 0x7));
		}else{
			header.mtime[r] = ' ';
		}
		curtime.tv_sec >>= 3;
	}
	
	header.typeflag = (char)REGTYPE;
	
	const unsigned char * const headerBytes = (const unsigned char *)&header;
	
	memset(&paddingBytes[0], '\0', 512);
	
	// encode and save each z-layer
	for(z=0; z<image.GetDepth(); z++){
		avifImage *encodeImage = nullptr;
		avifRGBImage rgbImage{};
		bool isDataAllocated = false;
		avifRWData output = AVIF_DATA_EMPTY;
		
		try{
			encodeImage = avifImageCreate(encodeWidth, encodeHeight, bitCount, AVIF_PIXEL_FORMAT_YUV420);
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
			const char * const layerData = imageData + strideImage * z;
			
			if(bitCount == 16){
				if(componentCount == 1){
					const auto src = reinterpret_cast<const sGrayscale16*>(layerData);
					for(int y=0; y<height; y++){
						auto destRow = reinterpret_cast<uint16_t*>(
							rgbImage.pixels + rgbImage.rowBytes * y);
						auto srcRow = src + width * y;
						
						for(int x=0; x<width; x++){
							destRow[x] = (uint16_t)srcRow[x].value;
						}
					}
					
				}else if(componentCount == 2){
					const auto src = reinterpret_cast<const sGrayscaleAlpha16*>(layerData);
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
					const auto src = reinterpret_cast<const sRGB16*>(layerData);
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
					const auto src = reinterpret_cast<const sRGBA16*>(layerData);
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
					const auto src = reinterpret_cast<const sGrayscale8*>(layerData);
					for(int y=0; y<height; y++){
						auto destRow = rgbImage.pixels + rgbImage.rowBytes * y;
						auto srcRow = src + width * y;
						
						for(int x=0; x<width; x++){
							destRow[x] = (uint8_t)srcRow[x].value;
						}
					}
					
				}else if(componentCount == 2){
					const auto src = reinterpret_cast<const sGrayscaleAlpha8*>(layerData);
					for(int y=0; y<height; y++){
						auto destRow = rgbImage.pixels + rgbImage.rowBytes * y;
						auto srcRow = src + width * y;
						
						for(int x=0; x<width; x++){
							*(destRow++) = (uint8_t)srcRow[x].value;
							*(destRow++) = (uint8_t)srcRow[x].alpha;
						}
					}
					
				}else if(componentCount == 3){
					const auto src = reinterpret_cast<const sRGB8*>(layerData);
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
					const auto src = reinterpret_cast<const sRGBA8*>(layerData);
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
			
			// if size is odd add clap box for decoders to produce correct size
			if(encodeWidth != width || encodeHeight != height){
				encodeImage->transformFlags |= AVIF_TRANSFORM_CLAP;
				encodeImage->clap.widthN = (int32_t)width;
				encodeImage->clap.widthD = 1;
				encodeImage->clap.heightN = (int32_t)height;
				encodeImage->clap.heightD = 1;
				encodeImage->clap.horizOffN = 0;
				encodeImage->clap.horizOffD = 1;
				encodeImage->clap.vertOffN = 0;
				encodeImage->clap.vertOffD = 1;
			}
			
			// encode image
			encoder->quality = 80;
			encoder->qualityAlpha = 80;
			encoder->speed = 6; // 0-10, higher is faster
			
			result = avifEncoderWrite(encoder, encodeImage, &output);
			if(result != AVIF_RESULT_OK){
				DETHROW_INFO(deeWriteFile, avifResultToString(result));
			}
			
			// write tar header and data
			int filesize = (int)output.size;
			int padding = 512 - (filesize % 512);
			if(padding == 512){
				padding = 0;
			}
			
			#ifdef OS_W32
				sprintf_s(&header.name[0], sizeof(header.name), "z%hu.avif", (unsigned short)z);
			#else
				sprintf(&header.name[0], "z%hu.avif", (unsigned short)z);
			#endif
			
			int tempSize = filesize;
			header.size[11] = ' ';
			for(r=10; r>=0; r--){
				if(tempSize){
					header.size[r] = (char)('0' + (char)(tempSize & 0x7));
				}else{
					header.size[r] = ' ';
				}
				tempSize >>= 3;
			}
			
			memset(&header.chksum[0], ' ', 8);
			checksum = 0;
			for(r=0; r<512; r++){
				checksum += headerBytes[r];
			}
			header.chksum[7] = '\0';
			header.chksum[6] = ' ';
			for(r=5; r>=0; r--){
				if(checksum){
					header.chksum[r] = (char)('0' + (char)(checksum & 0x7));
				}else{
					header.chksum[r] = ' ';
				}
				checksum >>= 3;
			}
			
			file.Write(&header, 512);
			if(filesize > 0){
				file.Write(output.data, filesize);
				if(padding > 0){
					file.Write(&paddingBytes, padding);
				}
			}
			
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
	}
	
	avifEncoderDestroy(encoder);
	
	// write two padding blocks at the end
	file.Write(&paddingBytes, 512);
	file.Write(&paddingBytes, 512);
}



void deAvif3DTarball::Get2DImageInfos(sImageInfo &info2D, decBaseFileReader &file, int size){
	const decMemoryFile::Ref data(decMemoryFile::Ref::New("data"));
	data->Resize(size);
	file.Read(data->GetPointer(), size);
	
	const decMemoryFileReader::Ref reader(decMemoryFileReader::Ref::New(data));
	deAvif3DIO ioHandler(*reader);
	
	auto decoder = avifDecoderCreate();
	DEASSERT_NOTNULL(decoder)
	
	avifImage *image = nullptr;
	
	try{
		avifDecoderSetIO(decoder, &ioHandler.GetIO());
		
		image = avifImageCreateEmpty();
		DEASSERT_NOTNULL(image)
		
		auto result = avifDecoderRead(decoder, image);
		if(result != AVIF_RESULT_OK){
			DETHROW_INFO(deeInvalidFileFormat, avifResultToString(result));
		}
		
		info2D.width = image->width;
		info2D.height = image->height;
		info2D.hasAlpha = image->alphaPlane != nullptr;
		info2D.isGrayscale = image->yuvFormat == AVIF_PIXEL_FORMAT_YUV400;
		info2D.bitCount = image->depth > 8 ? 16 : 8;
		
	}catch(const deException &){
		if(image){
			avifImageDestroy(image);
		}
		avifDecoderDestroy(decoder);
		throw;
	}
	
	avifImageDestroy(image);
	avifDecoderDestroy(decoder);
}

void deAvif3DTarball::Load2DImage(deAvif3DImageInfo &info3D,
decBaseFileReader &file, int size, void *imagedata){
	DEASSERT_NOTNULL(imagedata)
	
	const decMemoryFile::Ref data(decMemoryFile::Ref::New("data"));
	data->Resize(size);
	file.Read(data->GetPointer(), size);
	
	const decMemoryFileReader::Ref reader(decMemoryFileReader::Ref::New(data));
	deAvif3DIO ioHandler(*reader);
	
	const int componentCount = info3D.GetComponentCount();
	const int bitCount = info3D.GetBitCount();
	const int height = info3D.GetHeight();
	const int width = info3D.GetWidth();
	avifImage *decodedImage = nullptr;
	bool isDataAllocated = false;
	avifRGBImage rgbImage{};
	
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
		
		if(bitCount == 16){
			if(componentCount == 1){
				auto dest = reinterpret_cast<sGrayscale16*>(imagedata);
				for(y=0; y<height; y++){
					auto srcRow = reinterpret_cast<const uint16_t*>(rgbData + rgbRowBytes * y);
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = (unsigned short)srcRow[x];
					}
				}
				
			}else if(componentCount == 2){
				auto dest = reinterpret_cast<sGrayscaleAlpha16*>(imagedata);
				for(y=0; y<height; y++){
					auto srcRow = reinterpret_cast<const uint16_t*>(rgbData + rgbRowBytes * y);
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = (unsigned short)*(srcRow++);
						destRow[x].alpha = (unsigned short)*(srcRow++);
					}
				}
				
			}else if(componentCount == 3){
				auto dest = reinterpret_cast<sRGB16*>(imagedata);
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
				auto dest = reinterpret_cast<sRGBA16*>(imagedata);
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
				auto dest = reinterpret_cast<sGrayscale8*>(imagedata);
				for(y=0; y<height; y++){
					auto srcRow = rgbData + rgbRowBytes * y;
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = srcRow[x];
					}
				}
				
			}else if(componentCount == 2){
				auto dest = reinterpret_cast<sGrayscaleAlpha8*>(imagedata);
				for(y=0; y<height; y++){
					auto srcRow = rgbData + rgbRowBytes * y;
					auto destRow = dest + width * y;
					for(x=0; x<width; x++){
						destRow[x].value = *(srcRow++);
						destRow[x].alpha = *(srcRow++);
					}
				}
				
			}else if(componentCount == 3){
				auto dest = reinterpret_cast<sRGB8*>(imagedata);
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
				auto dest = reinterpret_cast<sRGBA8*>(imagedata);
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
