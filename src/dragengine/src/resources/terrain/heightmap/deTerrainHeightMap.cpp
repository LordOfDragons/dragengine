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
#include "deTerrainHeightMap.h"
#include "../../image/deImage.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deTerrainHeightMap
/////////////////////////////

// NOTE:
// the current visible faces system uses a simple bit-per-face array
// to store the visibility. a better system would be to use an RLE
// based system with the following encoding:
// X Y X Y ...
// where X indicates the number of 1s and Y the number of 0s. this
// encoding does not require to store the value to repeat since it
// alternates between 1 and 0 starting with 1 for each line. The value
// 255 is special indicating that 254 of 0 or 1 are written and that
// the next value is of the same type. Therefore the data
// 200 times 1, 500 times 0, 200 times 1
// would be encoded like this ( in paranthesis the bit used for this
// run as well as the number of encoded elements ):
// 200(1/200), 255(0/254), 246(0/246), 200(1/200)

// Constructor, destructor
////////////////////////////

deTerrainHeightMap::deTerrainHeightMap(){
	pPointCount.Set(2, 2);
	pSize.Set(1.0f, 1.0f);
	pScaling = 1.0f;
	
	pHeightImage = NULL;
	
	pVisibleFaces = NULL;
	pVFByteCount = 0;
}

deTerrainHeightMap::~deTerrainHeightMap(){
	if(pVisibleFaces) delete [] pVisibleFaces;
	
	if(pHeightImage) pHeightImage->FreeReference();
}



// Management
///////////////

void deTerrainHeightMap::SetPointCount(const decPoint &pointCount){
	if(pointCount.x < 2 || pointCount.y < 2) DETHROW(deeInvalidParam);
	
	if(pointCount != pPointCount){
		if(pVisibleFaces){
			delete [] pVisibleFaces;
			pVisibleFaces = NULL;
			pVFByteCount = 0;
		}
		
		SetHeightImage(NULL);
		
		pPointCount = pointCount;
	}
}

void deTerrainHeightMap::SetSize(const decVector2 &size){
	if(size.x < 1e-5f || size.y < 1e-5f) DETHROW(deeInvalidParam);
	
	pSize = size;
}

void deTerrainHeightMap::SetScaling(float scaling){
	if(scaling < 1e-5f) DETHROW(deeInvalidParam);
	
	pScaling = scaling;
}



void deTerrainHeightMap::SetPathHeightImage(const char *path){
	pPathHeightImage = path;
}

void deTerrainHeightMap::SetHeightImage(deImage *heightImage){
	if(heightImage){
		if(heightImage->GetWidth() != pPointCount.x){
			DETHROW(deeInvalidParam);
		}
		if(heightImage->GetHeight() != pPointCount.y){
			DETHROW(deeInvalidParam);
		}
		if(heightImage->GetComponentCount() != 1){
			DETHROW(deeInvalidParam);
		}
	}
	
	if(heightImage == pHeightImage){
		return;
	}
	
	// note: calculate extends requires image data to be retained. thing is
	//       deprecated anyways and should be done by physics module
	if(pHeightImage){
		pHeightImage->ReleaseImageData();
	}
	
	pHeightImage = heightImage;
	
	if(heightImage){
		heightImage->RetainImageData();
	}
}

void deTerrainHeightMap::CalculateExtends(decVector &minExtend, decVector &maxExtend){
	int i, size = pPointCount.x * pPointCount.y;
	float curHeight, scaling;
	float minHeight = 0.0f;
	float maxHeight = 0.0f;
	float compare;
	
	if(pHeightImage){
		if(pHeightImage->GetBitCount() == 8){
			sGrayscale8 *imageData = pHeightImage->GetDataGrayscale8();
			scaling = pScaling * THM_8BIT_PTOH;
			
			for(i=0; i<size; i++){
				curHeight = (imageData[i].value - THM_8BIT_BASE) * scaling;
				
				if(curHeight < minHeight){
					minHeight = curHeight;
					
				}else if(curHeight > maxHeight){
					maxHeight = curHeight;
				}
			}
			
		}else if(pHeightImage->GetBitCount() == 16){
			sGrayscale16 *imageData = pHeightImage->GetDataGrayscale16();
			scaling = pScaling * THM_16BIT_PTOH;
			
			for(i=0; i<size; i++){
				curHeight = (imageData[i].value - THM_16BIT_BASE) * scaling;
				
				if(curHeight < minHeight){
					minHeight = curHeight;
					
				}else if(curHeight > maxHeight){
					maxHeight = curHeight;
				}
			}
			
		}else{
			sGrayscale32 *imageData = pHeightImage->GetDataGrayscale32();
			
			for(i=0; i<size; i++){
				curHeight = imageData[i].value * pScaling;
				
				if(curHeight < minHeight){
					minHeight = curHeight;
					
				}else if(curHeight > maxHeight){
					maxHeight = curHeight;
				}
			}
		}
	}
	
	compare = pSize.x * 0.5f;
	if(-compare < minExtend.x) minExtend.x = -compare;
	if(compare > maxExtend.x) maxExtend.x = compare;
	
	if(minHeight < minExtend.y) minExtend.y = minHeight;
	if(maxHeight > maxExtend.y) maxExtend.y = maxHeight;
	
	compare = pSize.y * 0.5f;
	if(-compare < minExtend.z) minExtend.z = -compare;
	if(compare > maxExtend.z) maxExtend.z = compare;
}



// Face Visibility
////////////////////

void deTerrainHeightMap::SetPathVisibilityImage(const char *path){
	pPathVisImage = path;
}

bool deTerrainHeightMap::GetFaceVisibleAt(int x, int y){
	if(x < 0 || x >= pPointCount.x - 1 || y < 0 || y >= pPointCount.y - 1) DETHROW(deeInvalidParam);
	
	if(pVisibleFaces){
		int bitOffset = (pPointCount.x - 1) * y + x;
		
		return (pVisibleFaces[bitOffset >> 3] & (1 << (bitOffset & 0x7))) != 0;
		
	}else{
		return true;
	}
}

void deTerrainHeightMap::SetFaceVisibleAt(int x, int y, bool visible){
	if(x < 0 || x >= pPointCount.x - 1 || y < 0 || y >= pPointCount.y - 1) DETHROW(deeInvalidParam);
	
	pCreateVisibleFaces();
	
	int bitOffset = (pPointCount.x - 1) * y + x;
	
	if(visible){
		pVisibleFaces[bitOffset >> 3] |= (unsigned char)(1 << (bitOffset & 0x7));
		
	}else{
		pVisibleFaces[bitOffset >> 3] &= ~((unsigned char)(1 << (bitOffset & 0x7)));
	}
}

void deTerrainHeightMap::SetAllFacesVisible(bool visible){
	if(visible){
		if(pVisibleFaces){
			delete [] pVisibleFaces;
			pVisibleFaces = NULL;
		}
		
	}else{
		pCreateVisibleFaces();
		
		memset(pVisibleFaces, 0, pVFByteCount);
	}
}



// Private Functions
//////////////////////

void deTerrainHeightMap::pCreateVisibleFaces(){
	if(!pVisibleFaces){
		int byteCount = (((pPointCount.x - 1) * (pPointCount.y - 1) - 1) >> 3) + 1;
		
		pVisibleFaces = new unsigned char[byteCount];
		if(!pVisibleFaces) DETHROW(deeOutOfMemory);
		
		memset(pVisibleFaces, 255, byteCount);
		
		pVFByteCount = byteCount;
	}
}
