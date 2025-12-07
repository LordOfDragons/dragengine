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

#include "meTerrainHeightImage.h"

#include "dragengine/resources/terrain/heightmap/deTerrainHeightMap.h"
#include "dragengine/common/exceptions.h"



// Class meTerrainHeightImage
///////////////////////////////

// Constructor, destructor
////////////////////////////

meTerrainHeightImage::meTerrainHeightImage(deImage *image){
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

meTerrainHeightImage::~meTerrainHeightImage(){
}



// Management
///////////////

float meTerrainHeightImage::GetHeightAt(int x, int y) const{
	if(pData8){
		return (float)(pData8[pWidth * y + x].value - THM_8BIT_BASE) * THM_8BIT_PTOH;
		
	}else if(pData16){
		return (float)(pData16[pWidth * y + x].value - THM_16BIT_BASE) * THM_16BIT_PTOH;
		
	}else{
		return pData32[pWidth * y + x].value;
	}
}

float meTerrainHeightImage::GetHeightAt(int index) const{
	if(pData8){
		return (float)(pData8[index].value - THM_8BIT_BASE) * THM_8BIT_PTOH;
		
	}else if(pData16){
		return (float)(pData16[index].value - THM_16BIT_BASE) * THM_16BIT_PTOH;
		
	}else{
		return pData32[index].value;
	}
}

void meTerrainHeightImage::SetHeightAt(int x, int y, float height){
	if(pData8){
		pData8[pWidth * y + x].value = (unsigned char)((int)(height * THM_8BIT_HTOP) + THM_8BIT_BASE);
		
	}else if(pData16){
		pData16[pWidth * y + x].value = (unsigned short)((int)(height * THM_16BIT_HTOP) + THM_16BIT_BASE);
		
	}else{
		pData32[pWidth * y + x].value = height;
	}
}

void meTerrainHeightImage::SetHeightAt(int index, float height){
	if(pData8){
		pData8[index].value = (unsigned char)((int)(height * THM_8BIT_HTOP) + THM_8BIT_BASE);
		
	}else if(pData16){
		pData16[index].value = (unsigned short)((int)(height * THM_16BIT_HTOP) + THM_16BIT_BASE);
		
	}else{
		pData32[index].value = height;
	}
}
