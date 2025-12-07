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
#include <webp/demux.h>

#include "deWebpImageInfo.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>


// Class deWebpImageInfo
//////////////////////////

// Constructor, destructor
////////////////////////////

deWebpImageInfo::deWebpImageInfo(decBaseFileReader &reader) :
pFilename(reader.GetFilename()),
pWidth(0),
pHeight(0),
pHasAlpha(false),
	pIsGrayscale (false)
{
	const int size = reader.GetLength();
	pData.TakeOver(new decMemoryFile("data"));
	pData->Resize(size);
	reader.Read(pData->GetPointer(), size);
	
	WebPBitstreamFeatures features;
	Assert(WebPGetFeatures((uint8_t*)pData->GetPointer(), size, &features));
	
	pWidth = features.width;
	pHeight = features.height;
	pHasAlpha = features.has_alpha;
	
	// look for exif user comments to handle the "grayscale" problem
	WebPData wpdata = {(const uint8_t*)pData->GetPointer(), (size_t)size};
	WebPDemuxer * const demux = WebPDemux(&wpdata);
	DEASSERT_NOTNULL(demux)
	
	const uint32_t flags = WebPDemuxGetI(demux, WEBP_FF_FORMAT_FLAGS);
	if((flags & EXIF_FLAG) == EXIF_FLAG){
		WebPChunkIterator iter = {};
		if(WebPDemuxGetChunk(demux, "EXIF", 1, &iter) == 1){
			static const char * const tagGrayscale = "dewebp:grayscale";
			static const int tagGrayscaleLen = (int)strlen(tagGrayscale);
			
			const char * const exif = (const char*)iter.chunk.bytes;
			const int exifLen = (int)iter.chunk.size;
			int i;
			
			for(i=0; i<exifLen; i++){
				if(strncmp(exif + i, tagGrayscale, tagGrayscaleLen) == 0){
					pIsGrayscale = true;
					break;
				}
			}
			
			WebPDemuxReleaseChunkIterator(&iter);
		}
	}
	
	WebPDemuxDelete(demux);
}

deWebpImageInfo::~deWebpImageInfo(){
}



// Management
///////////////

int deWebpImageInfo::GetWidth(){
	return pWidth;
}

int deWebpImageInfo::GetHeight(){
	return pHeight;
}

int deWebpImageInfo::GetDepth(){
	return 1;
}

int deWebpImageInfo::GetComponentCount(){
	if(pHasAlpha){
		return pIsGrayscale ? 2 : 4;
		
	}else{
		return pIsGrayscale ? 1 : 3;
	}
}

int deWebpImageInfo::GetBitCount(){
	return 8;
}

void deWebpImageInfo::Assert(VP8StatusCode statusCode) const{
	switch(statusCode){
	case VP8_STATUS_OK:
		return;
		
	case VP8_STATUS_OUT_OF_MEMORY:
		DETHROW(deeOutOfMemory);
		
	case VP8_STATUS_INVALID_PARAM:
		DETHROW(deeInvalidParam);
		
	case VP8_STATUS_BITSTREAM_ERROR:
		DETHROW_INFO(deeInvalidFileFormat, "Bit stream error");
		
	case VP8_STATUS_UNSUPPORTED_FEATURE:
		DETHROW_INFO(deeInvalidFileFormat, "Unsupported feature");
		
	case VP8_STATUS_SUSPENDED:
		DETHROW_INFO(deeInvalidFileFormat, "Suspended");
		
	case VP8_STATUS_USER_ABORT:
		DETHROW_INFO(deeInvalidFileFormat, "User abort");
		
	case VP8_STATUS_NOT_ENOUGH_DATA:
		DETHROW_INFO(deeInvalidFileFormat, "Not enough data");
		
	default:
		DETHROW(deeInvalidFileFormat);
	}
}
