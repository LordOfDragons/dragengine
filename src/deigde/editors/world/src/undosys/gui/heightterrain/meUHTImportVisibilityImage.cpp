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

#include "meUHTImportVisibilityImage.h"
#include "../../../utils/meBitArray.h"
#include "../../../world/meWorld.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/common/exceptions.h>



// Class meUHTImportVisibilityImage
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTImportVisibilityImage::meUHTImportVisibilityImage(meWorld *world, meHeightTerrainSector *sector, deImage *image){
	if(!world || !sector || !image) DETHROW(deeInvalidParam);
	
	if(image->GetComponentCount() != 1) DETHROW(deeInvalidParam);
	
	int resolution = sector->GetHeightTerrain()->GetSectorResolution();
	
	if(image->GetWidth() != resolution || image->GetHeight() != resolution) DETHROW(deeInvalidParam);
	
	int x, y, pixelCount = resolution * resolution;
	
	pWorld = NULL;
	pSector = sector;
	
	pOldVis = NULL;
	pNewVis = NULL;
	
	SetShortInfo("Import Visibility Image");
	SetMemoryConsumption(sizeof(meUHTImportVisibilityImage) + 2 * (pixelCount / 8 + 1));
	
	try{
		// create arrays
		pOldVis = new meBitArray(resolution, resolution);
		if(!pOldVis) DETHROW(deeOutOfMemory);
		
		pNewVis = new meBitArray(resolution, resolution);
		if(!pNewVis) DETHROW(deeOutOfMemory);
		
		// snapshot old heights
		sector->GetVisibilityFaces()->CopyTo(*pOldVis);
		
		// store new heights
		if(image->GetBitCount() == 8){
			sGrayscale8 *data = image->GetDataGrayscale8();
			
			for(y=0; y<resolution; y++){
				for(x=0; x<resolution; x++){
					pNewVis->SetValueAt(x, y, data[y * resolution + x].value > HT_8BIT_BASE);
				}
			}
			
		}else if(image->GetBitCount() == 16){
			sGrayscale16 *data = image->GetDataGrayscale16();
			
			for(y=0; y<resolution; y++){
				for(x=0; x<resolution; x++){
					pNewVis->SetValueAt(x, y, data[y * resolution + x].value > HT_16BIT_BASE);
				}
			}
			
		}else{ // image->GetBitCount() == 32
			sGrayscale32 *data = image->GetDataGrayscale32();
			
			for(y=0; y<resolution; y++){
				for(x=0; x<resolution; x++){
					pNewVis->SetValueAt(x, y, data[y * resolution + x].value > 0.0f);
				}
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
}

meUHTImportVisibilityImage::~meUHTImportVisibilityImage(){
	pCleanUp();
}



// Management
///////////////

void meUHTImportVisibilityImage::Undo(){
	pDoIt(pOldVis);
}

void meUHTImportVisibilityImage::Redo(){
	pDoIt(pNewVis);
}



// Private Functions
//////////////////////

void meUHTImportVisibilityImage::pCleanUp(){
	if(pNewVis) delete pNewVis;
	if(pOldVis) delete pOldVis;
	
	if(pWorld) pWorld->FreeReference();
}

void meUHTImportVisibilityImage::pDoIt(meBitArray *vis){
	vis->CopyTo(*pSector->GetVisibilityFaces());
	pSector->SetVisibilityChanged(true);
	
	if(pSector->GetEngineSector()){
		pSector->UpdateVisibilitySector(pSector->GetEngineSector());
	}
	pSector->RebuildEngineSector();
	
	pWorld->NotifyHTSVisibilityChanged(pSector);
}
