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

#include "deAvifImageInfo.h"
#include "deAvifIO.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>


// Class deAvifImageInfo
//////////////////////////

// Constructor, destructor
////////////////////////////

deAvifImageInfo::deAvifImageInfo(decBaseFileReader &reader) :
pFilename(reader.GetFilename()),
pWidth(0),
pHeight(0),
pBitCount(8),
pHasAlpha(false),
pIsGrayscale(false)
{
	deAvifIO ioHandler(reader);
	
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
		
		pWidth = image->width;
		pHeight = image->height;
		pBitCount = image->depth > 8 ? 16 : 8;
		
		pHasAlpha = image->alphaPlane != nullptr;
		pIsGrayscale = image->yuvFormat == AVIF_PIXEL_FORMAT_YUV400;
		
		if(pIsGrayscale && pHasAlpha){
			pComponentCount = 2;
			
		}else if(pIsGrayscale){
			pComponentCount = 1;
			
		}else if(pHasAlpha){
			pComponentCount = 4;
			
		}else{
			pComponentCount = 3;
		}
		
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

deAvifImageInfo::~deAvifImageInfo() = default;


// Management
///////////////

int deAvifImageInfo::GetWidth(){
	return pWidth;
}

int deAvifImageInfo::GetHeight(){
	return pHeight;
}

int deAvifImageInfo::GetDepth(){
	return 1;
}

int deAvifImageInfo::GetComponentCount(){
	return pComponentCount;
}

int deAvifImageInfo::GetBitCount(){
	return pBitCount;
}
