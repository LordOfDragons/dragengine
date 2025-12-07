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

#include "meUHTPaintHeight.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>



// Class meUHTPaintHeight
///////////////////////////

// Constructor, destructor
////////////////////////////

meUHTPaintHeight::meUHTPaintHeight(int drawMode, meWorld *world, const decPoint &sector, const decPoint &grid,
const decPoint &size, float *oldHeights){
	if(! world || ! oldHeights) DETHROW(deeInvalidParam);
	
	meHeightTerrain *hterrain = world->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	int pixelCount = size.x * size.y;
	int adjustX, adjustY;
	
	if(drawMode == meWorldGuiParameters::ehpdmRaise){
		SetShortInfo("Raise Terrain");
		
	}else if(drawMode == meWorldGuiParameters::ehpdmLower){
		SetShortInfo("Lower Terrain");
		
	}else if(drawMode == meWorldGuiParameters::ehpdmLevel){
		SetShortInfo("Level Terrain");
		
	}else{
		SetShortInfo("Smooth Terrain");
	}
	
	pWorld = world;
	pSize = size;
	pOldHeights = NULL;
	pNewHeights = NULL;
	
	pSector.x1 = sector.x;
	pSector.y1 = sector.y;
	pGrid.x1 = grid.x;
	pGrid.y1 = grid.y;
	pGrid.x2 = grid.x + size.x;
	pGrid.y2 = grid.y + size.y;
	
	adjustX = pGrid.x2 / imageDim;
	adjustY = pGrid.y2 / imageDim;
	
	pSector.x2 = sector.x + adjustX;
	pSector.y2 = sector.y + adjustY;
	pGrid.x2 -= adjustX * imageDim;
	pGrid.y2 -= adjustY * imageDim;
	pWorld->GetLogger()->LogInfoFormat("World Editor", "UHTPaintHeight: s=(%i,%i,%i,%i) g=(%i,%i,%i,%i)", pSector.x1, pSector.y1, pSector.x2, pSector.y2, pGrid.x1, pGrid.y1, pGrid.x2, pGrid.y2);
	
	SetMemoryConsumption(sizeof(meUHTPaintHeight) + sizeof(float) * pixelCount);
	
	try{
		pOldHeights = new float[pixelCount];
		if(! pOldHeights) DETHROW(deeOutOfMemory);
		
		pNewHeights = new float[pixelCount];
		if(! pNewHeights) DETHROW(deeOutOfMemory);
		
		pSaveHeights();
		
		memcpy(pOldHeights, oldHeights, sizeof(float) * pixelCount);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUHTPaintHeight::~meUHTPaintHeight(){
	pCleanUp();
}



// Management
///////////////

void meUHTPaintHeight::Undo(){
	pRestoreHeights(pOldHeights);
}

void meUHTPaintHeight::Redo(){
	pRestoreHeights(pNewHeights);
}



// Private Functions
//////////////////////

void meUHTPaintHeight::pCleanUp(){
	if(pNewHeights) delete [] pNewHeights;
	if(pOldHeights) delete [] pOldHeights;
}

void meUHTPaintHeight::pSaveHeights(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	meHeightTerrainSector *htsector;
	sGrayscale32 *pixels;
	decPoint scoord;
	int sgx, sgy;
	int adjust;
	int x, y;
	
	for(y=0; y<pSize.y; y++){
		for(x=0; x<pSize.x; x++){
			sgx = pGrid.x1 + x;
			adjust = sgx / imageDim;
			scoord.x = pSector.x1 + adjust;
			sgx -= adjust * imageDim;
			
			sgy = pGrid.y1 + y;
			adjust = sgy / imageDim;
			scoord.y = pSector.y1 + adjust;
			sgy -= adjust * imageDim;
			
			htsector = hterrain->GetSectorWith(scoord);
			if(htsector){
				pixels = htsector->GetHeightImage()->GetDataGrayscale32();
				pNewHeights[y * pSize.x + x] = pixels[sgy * imageDim + sgx].value;
				
			}else{
				pNewHeights[y * pSize.x + x] = 0.0f;
			}
		}
	}
}

void meUHTPaintHeight::pRestoreHeights(float *heights){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int s, sectorCount = hterrain->GetSectorCount();
	int imageDim = hterrain->GetSectorResolution();
	meHeightTerrainSector *htsector;
	sGrayscale32 *pixels;
	decPoint scoord;
	int sgx, sgy;
	int adjust;
	int x, y;
	
	for(y=0; y<pSize.y; y++){
		for(x=0; x<pSize.x; x++){
			sgx = pGrid.x1 + x;
			adjust = sgx / imageDim;
			scoord.x = pSector.x1 + adjust;
			sgx -= adjust * imageDim;
			
			sgy = pGrid.y1 + y;
			adjust = sgy / imageDim;
			scoord.y = pSector.y1 + adjust;
			sgy -= adjust * imageDim;
			
			htsector = hterrain->GetSectorWith(scoord);
			if(htsector){
				pixels = htsector->GetHeightImage()->GetDataGrayscale32();
				pixels[sgy * imageDim + sgx].value = heights[y * pSize.x + x];
			}
		}
	}
	
	hterrain->NotifyHeightsChanged(pSector, pGrid);
	
	for(s=0; s<sectorCount; s++){
		htsector = hterrain->GetSectorAt(s);
		const decPoint &scoord2 = htsector->GetCoordinates();
		
		if(scoord2.x >= pSector.x1 && scoord2.y >= pSector.y1 && scoord2.x <= pSector.x2 && scoord2.y <= pSector.y2){
			htsector->SetHeightImageChanged(true);
			pWorld->NotifyHTSHeightChanged(htsector);
		}
	}
}
