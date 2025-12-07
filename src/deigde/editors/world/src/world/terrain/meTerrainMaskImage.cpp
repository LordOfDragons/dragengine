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

#include "meTerrainMaskImage.h"

#include "dragengine/common/exceptions.h"



// Definitions
////////////////

#define INV_FLOAT_255		(1.0f / 255.0f)



// Class meTerrainMaskImage
/////////////////////////////

// Constructor, destructor
////////////////////////////

meTerrainMaskImage::meTerrainMaskImage(deImage *image){
	if(! image) DETHROW(deeInvalidParam);
	
	pWidth = image->GetWidth();
	pHeight = image->GetHeight();
	
	pData8 = NULL;
	pData16 = NULL;
	pData32 = NULL;
	
	if(image->GetBitCount() == 8){
		pData8 = image->GetDataGrayscale8();
		
	}else if(image->GetBitCount() == 16){
		pData16 = image->GetDataGrayscale16();
		
	}else{
		pData32 = image->GetDataGrayscale32();
	}
}

meTerrainMaskImage::~meTerrainMaskImage(){
}



// Management
///////////////

unsigned char meTerrainMaskImage::GetMaskValueAt(int x, int y) const{
	if(pData8){
		return pData8[pWidth * y + x].value;
		
	}else if(pData16){
		return (unsigned char)(pData16[pWidth * y + x].value >> 8);
		
	}else{
		return (unsigned char)(pData32[pWidth * y + x].value * 255.0f);
	}
}

unsigned char meTerrainMaskImage::GetMaskValueAt(int index) const{
	if(pData8){
		return pData8[index].value;
		
	}else if(pData16){
		return (unsigned char)(pData16[index].value >> 8);
		
	}else{
		return (unsigned char)(pData32[index].value * 255.5f);
	}
}

void meTerrainMaskImage::SetMaskValueAt(int x, int y, unsigned char value){
	if(pData8){
		pData8[pWidth * y + x].value = value;
		
	}else if(pData16){
		pData16[pWidth * y + x].value = (unsigned short)value << 8;
		
	}else{
		pData32[pWidth * y + x].value = (float)value * INV_FLOAT_255;
	}
}

void meTerrainMaskImage::SetMaskValueAt(int index, unsigned char value){
	if(pData8){
		pData8[index].value = value;
		
	}else if(pData16){
		pData16[index].value = (unsigned short)value << 8;
		
	}else{
		pData32[index].value = (float)value * INV_FLOAT_255;
	}
}
