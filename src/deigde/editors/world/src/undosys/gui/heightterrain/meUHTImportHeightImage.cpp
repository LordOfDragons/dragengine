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

#include "meUHTImportHeightImage.h"
#include "../../../world/meWorld.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/common/exceptions.h>



// Class meUHTImportHeightImage
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTImportHeightImage::meUHTImportHeightImage(meWorld::Ref world, meHeightTerrainSector *sector, deImage *image){
	if(!world || !sector || !image) DETHROW(deeInvalidParam);
	
	if(image->GetComponentCount() != 1) DETHROW(deeInvalidParam);
	
	int resolution = (int)(sector->GetHeightTerrain()->GetSectorSize() + 0.1f);
	
	if(image->GetWidth() != resolution || image->GetHeight() != resolution) DETHROW(deeInvalidParam);
	
	int p, pixelCount = resolution * resolution;
	
	pWorld = NULL;
	pSector = sector;
	
	pOldHeights = NULL;
	pNewHeights = NULL;
	
	SetShortInfo("Import Height Image");
	SetMemoryConsumption(sizeof(meUHTImportHeightImage) + 2 * sizeof(float) * pixelCount);
	
	try{
		// create arrays
		pOldHeights = new float[pixelCount];
		pNewHeights = new float[pixelCount];
		
		// snapshot old heights
		memcpy(pOldHeights, sector->GetHeightImage()->GetDataGrayscale32(), sizeof(float) * pixelCount);
		
		// store new heights
		if(image->GetBitCount() == 8){
			sGrayscale8 *data = image->GetDataGrayscale8();
			
			for(p=0; p<pixelCount; p++){
				pNewHeights[p] = (float)(data[p].value - HT_8BIT_BASE) * HT_8BIT_PTOH;
			}
			
		}else if(image->GetBitCount() == 16){
			sGrayscale16 *data = image->GetDataGrayscale16();
			
			for(p=0; p<pixelCount; p++){
				pNewHeights[p] = (float)(data[p].value - HT_16BIT_BASE) * HT_16BIT_PTOH;
			}
			
		}else{ // image->GetBitCount() == 32
			memcpy(pNewHeights, image->GetDataGrayscale32(), sizeof(float) * pixelCount);
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
}

meUHTImportHeightImage::~meUHTImportHeightImage(){
	pCleanUp();
}



// Management
///////////////

void meUHTImportHeightImage::Undo(){
	pDoIt(pOldHeights);
}

void meUHTImportHeightImage::Redo(){
	pDoIt(pNewHeights);
}



// Private Functions
//////////////////////

void meUHTImportHeightImage::pCleanUp(){
	if(pNewHeights) delete [] pNewHeights;
	if(pOldHeights) delete [] pOldHeights;
	
	if(pWorld) pWorld->FreeReference();
}

void meUHTImportHeightImage::pDoIt(float *heights){
	const decPoint &scoord = pSector->GetCoordinates();
	int resolution = pSector->GetHeightTerrain()->GetSectorResolution();
	int pixelCount = resolution * resolution;
	decBoundary areaSector, areaGrid;
	
	memcpy(pSector->GetHeightImage()->GetDataGrayscale32(), heights, sizeof(float) * pixelCount);
	pSector->SetHeightImageChanged(true);
	
	areaSector.x1 = scoord.x;
	areaSector.x2 = scoord.x;
	areaSector.y1 = scoord.y;
	areaSector.y2 = scoord.y;
	areaGrid.x1 = 0;
	areaGrid.x2 = resolution - 1;
	areaGrid.y1 = 0;
	areaGrid.y2 = resolution - 1;
	pWorld->GetHeightTerrain()->NotifyHeightsChanged(areaSector, areaGrid);
	
	pWorld->NotifyHTSHeightChanged(pSector);
}
