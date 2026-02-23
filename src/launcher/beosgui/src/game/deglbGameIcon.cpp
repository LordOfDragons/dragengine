/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <Bitmap.h>
#include <BitmapStream.h>
#include <TranslationUtils.h>
#include <DataIO.h>

#include "deglbGameIcon.h"

#include <dragengine/common/exceptions.h>


// Class deglbGameIcon
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deglbGameIcon::deglbGameIcon(int size, const char *path) :
delGameIcon(size, path),
pBitmap(nullptr){
}

deglbGameIcon::~deglbGameIcon(){
	if(pBitmap){
		delete pBitmap;
	}
}



// Management
///////////////

BBitmap *deglbGameIcon::CreateScaledBitmap(int size) const{
	if(!pBitmap){
		return nullptr;
	}
	
	if(GetSize() == size){
		return new BBitmap(*pBitmap);
	}
	
	BBitmap * const scaled = new BBitmap(BRect(0, 0, size - 1, size - 1), B_RGBA32);
	pBitmap->Lock();
	scaled->Lock();
	
	// simple nearest-neighbor scale
	const int srcW = (int)pBitmap->Bounds().Width() + 1;
	const int srcH = (int)pBitmap->Bounds().Height() + 1;
	const int dstW = size;
	const int dstH = size;
	const uint8 * const srcData = (const uint8*)pBitmap->Bits();
	uint8 * const dstData = (uint8*)scaled->Bits();
	const int srcBpr = pBitmap->BytesPerRow();
	const int dstBpr = scaled->BytesPerRow();
	
	int x, y;
	for(y=0; y<dstH; y++){
		const int srcY = y * srcH / dstH;
		for(x=0; x<dstW; x++){
			const int srcX = x * srcW / dstW;
			const uint8 * const sp = srcData + srcY * srcBpr + srcX * 4;
			uint8 * const dp = dstData + y * dstBpr + x * 4;
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
			dp[3] = sp[3];
		}
	}
	
	scaled->Unlock();
	pBitmap->Unlock();
	
	return scaled;
}



// Protected Functions
////////////////////////

void deglbGameIcon::OnContentChanged(){
	pCreateBitmap();
}



// Private Functions
//////////////////////

void deglbGameIcon::pCreateBitmap(){
	if(pBitmap){
		delete pBitmap;
		pBitmap = nullptr;
	}
	
	if(!GetContent()){
		return;
	}
	
	if(GetPath().EndsWith(".png")){
		pCreatePNGBitmap();
	}
}

void deglbGameIcon::pCreatePNGBitmap(){
	try{
		BMemoryIO memIO(GetContent()->GetPointer(), GetContent()->GetLength());
		BBitmap * const bitmap = BTranslationUtils::GetBitmap(&memIO);
		if(bitmap){
			pBitmap = bitmap;
		}
	}catch(const deException &){
		if(pBitmap){
			delete pBitmap;
			pBitmap = nullptr;
		}
	}
}
