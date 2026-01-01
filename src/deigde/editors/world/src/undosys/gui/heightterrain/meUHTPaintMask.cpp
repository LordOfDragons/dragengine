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

#include "meUHTPaintMask.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"
#include "../../../world/terrain/meHeightTerrainTexture.h"
#include "../../../world/terrain/meTerrainMaskImage.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>



// Class meUHTPaintMask
/////////////////////////

// Constructor, destructor
////////////////////////////

meUHTPaintMask::meUHTPaintMask(int drawMode, meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainTexture *texture, const decPoint &grid, const decPoint &size, unsigned char *oldValues){
	if(!world || !sector || !texture || !oldValues) DETHROW(deeInvalidParam);
	
	int pixelCount = size.x * size.y;
	
	if(drawMode == meWorldGuiParameters::empdmDraw){
		SetShortInfo("Draw Mask");
		
	}else{
		SetShortInfo("Erase Mask");
	}
	
	pWorld = world;
	pSector = sector;
	pTexture = texture;
	
	pSize = size;
	pOldValues = nullptr;
	pNewValues = nullptr;
	
	pGrid.x1 = grid.x;
	pGrid.y1 = grid.y;
	pGrid.x2 = grid.x + size.x;
	pGrid.y2 = grid.y + size.y;
	pWorld->GetLogger()->LogInfoFormat("World Editor", "UHTPaintMask: g=(%i,%i,%i,%i)", pGrid.x1, pGrid.y1, pGrid.x2, pGrid.y2);
	
	SetMemoryConsumption(sizeof(meUHTPaintMask) + sizeof(unsigned char) * pixelCount * 2);
	
	try{
		pOldValues = new unsigned char[pixelCount];
		pNewValues = new unsigned char[pixelCount];
		
		pSaveValues();
		
		memcpy(pOldValues, oldValues, sizeof(unsigned char) * pixelCount);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUHTPaintMask::~meUHTPaintMask(){
	pCleanUp();
}



// Management
///////////////

void meUHTPaintMask::Undo(){
	pRestoreValues(pOldValues);
}

void meUHTPaintMask::Redo(){
	pRestoreValues(pNewValues);
}



// Private Functions
//////////////////////

void meUHTPaintMask::pCleanUp(){
	if(pNewValues) delete [] pNewValues;
	if(pOldValues) delete [] pOldValues;
}

void meUHTPaintMask::pSaveValues(){
	deImage *mask = pTexture->GetMaskImage();
	meTerrainMaskImage tmi(mask);
	int sgx, sgy;
	int x, y;
	
	for(y=0; y<pSize.y; y++){
		for(x=0; x<pSize.x; x++){
			sgx = pGrid.x1 + x;
			sgy = pGrid.y1 + y;
			
			pNewValues[y * pSize.x + x] = tmi.GetMaskValueAt(sgx, sgy);
		}
	}
}

void meUHTPaintMask::pRestoreValues(unsigned char *values){
	deImage *mask = pTexture->GetMaskImage();
	meTerrainMaskImage tmi(mask);
	int sgx, sgy;
	int x, y;
	
	for(y=0; y<pSize.y; y++){
		for(x=0; x<pSize.x; x++){
			sgx = pGrid.x1 + x;
			sgy = pGrid.y1 + y;
			
			tmi.SetMaskValueAt(sgx, sgy, values[y * pSize.x + x]);
		}
	}
	
	pTexture->SetMaskChanged(true);
	pTexture->NotifyTextureMaskChanged();
}
