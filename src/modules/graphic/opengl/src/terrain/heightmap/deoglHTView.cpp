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

#include "deoglHTView.h"
#include "deoglHTViewSector.h"
#include "deoglHTSector.h"
#include "deoglHTSCluster.h"
#include "deoglRHeightTerrain.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>



// Class deoglHTView::void deoglHTView
////////////////////////////////////////

deoglHTView::HTListener::HTListener(deoglHTView &htview) : pHTView(htview){
}

void deoglHTView::HTListener::SectorsChanged(deoglRHeightTerrain &heightTerrain){
	pHTView.SectorsChanged(heightTerrain);
}




// Class deoglHTView
//////////////////////

// Constructor, destructor
////////////////////////////

deoglHTView::deoglHTView(deoglRHeightTerrain *heightTerrain) :
pHeightTerrain(heightTerrain),
pHTListener(HTListener::Ref::New(*this)),
pSectors(nullptr),
pSectorCount(0),
pSectorSize(0),
pDirtySectors(true)
{
	DEASSERT_NOTNULL(heightTerrain)
	heightTerrain->AddListener(pHTListener);
}

deoglHTView::~deoglHTView(){
	RemoveAllSectors();
	if(pSectors){
		delete [] pSectors;
	}
	
	pHeightTerrain->RemoveListener(pHTListener);
}



// Management
///////////////

deoglHTViewSector *deoglHTView::GetSectorAt(int index) const{
	if(index < 0 || index >= pSectorCount){
		DETHROW(deeInvalidParam);
	}
	
	return pSectors[index];
}

void deoglHTView::AddSector(deoglHTViewSector *sector){
	DEASSERT_NOTNULL(sector)
	
	if(pSectorCount == pSectorSize){
		const int newSize = pSectorSize * 3 / 2 + 1;
		deoglHTViewSector ** const newArray = new deoglHTViewSector*[newSize];
		if(pSectors){
			memcpy(newArray, pSectors, sizeof(deoglHTViewSector*) * pSectorSize);
			delete [] pSectors;
		}
		pSectors = newArray;
		pSectorSize = newSize;
	}
	
	pSectors[pSectorCount++] = sector;
}

void deoglHTView::RemoveAllSectors(){
	while(pSectorCount > 0){
		pSectorCount--;
		delete pSectors[pSectorCount];
	}
}



void deoglHTView::ResetClusters(){
	int i;
	for(i=0; i<pSectorCount; i++){
		pSectors[i]->ResetClusters();
	}
}

void deoglHTView::UpdateLODLevels(const decVector &camera){
	int i;
	for(i=0; i<pSectorCount; i++){
		pSectors[i]->UpdateLODLevels(camera);
	}
}



void deoglHTView::Prepare(){
	if(pDirtySectors){
		pDirtySectors = false;
		pRebuildSectors();
	}
}

void deoglHTView::PrepareForRendering(){
}

void deoglHTView::UpdateAllRTSInstances(){
	int i;
	for(i=0; i<pSectorCount; i++){
		pSectors[i]->UpdateAllRTSInstances();
	}
}



// Private functions
//////////////////////

void deoglHTView::pRebuildSectors(){
	const int count = pHeightTerrain->GetSectorCount();
	deoglHTViewSector *sector = NULL;
	int i;
	
	try{
		RemoveAllSectors();
		
		for(i=0; i<count; i++){
			sector = new deoglHTViewSector(*this, pHeightTerrain->GetSectorAt(i));
			AddSector(sector);
			sector = NULL;
		}
		
	}catch(const deException &){
		if(sector){
			delete sector;
		}
		throw;
	}
}



void deoglHTView::SectorsChanged (deoglRHeightTerrain &heightTerrain){
	pDirtySectors = true;
}
