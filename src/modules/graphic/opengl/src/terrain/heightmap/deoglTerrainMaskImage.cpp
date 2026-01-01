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
#include "deoglTerrainMaskImage.h"
#include <dragengine/resources/terrain/heightmap/deTerrainHeightMap.h>
#include <dragengine/common/exceptions.h>



// Class deoglTerrainMaskImage
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTerrainMaskImage::deoglTerrainMaskImage(deImage &image) :
pImage(image),
pWidth(image.GetWidth()),
pHeight(image.GetHeight()),
pData8(NULL),
pData16(NULL),
pData32(NULL),
pRetained(false)
{
	image.RetainImageData();
	pRetained = true;
	
	if(image.GetBitCount() == 8){
		pData8 = image.GetDataGrayscale8();
		if(!pData8){
			DETHROW(deeInvalidParam);
		}
		
	}else if(image.GetBitCount() == 16){
		pData16 = image.GetDataGrayscale16();
		if(!pData16){
			DETHROW(deeInvalidParam);
		}
		
	}else{
		pData32 = image.GetDataGrayscale32();
		if(!pData32){
			DETHROW(deeInvalidParam);
		}
	}
}

deoglTerrainMaskImage::~deoglTerrainMaskImage(){
	if(pRetained){
		pImage.ReleaseImageData();
	}
}



// Management
///////////////

float deoglTerrainMaskImage::GetMaskValueAt(int x, int y) const{
	if(pData8){
		return (float)(pData8[pWidth * y + x].value) * THM_8BIT_PTOH;
		
	}else if(pData16){
		return (float)(pData16[pWidth * y + x].value) * THM_16BIT_PTOH;
		
	}else{
		return pData32[pWidth * y + x].value;
	}
}

float deoglTerrainMaskImage::GetMaskValueAt(int index) const{
	if(pData8){
		return (float)(pData8[index].value) * THM_8BIT_PTOH;
		
	}else if(pData16){
		return (float)(pData16[index].value) * THM_16BIT_PTOH;
		
	}else{
		return pData32[index].value;
	}
}
