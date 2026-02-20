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

#include "deoglHTSector.h"
#include "deoglRHTSector.h"
#include "deoglHeightTerrain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>



// Class deoglHTSector
////////////////////////

// Constructor, destructor
////////////////////////////

deoglHTSector::deoglHTSector(deoglHeightTerrain &heightTerrain, const deHeightTerrainSector &sector) :
pHeightTerrain(heightTerrain),
pSector(sector),
pSectorChanged(true),
pDirtyHeights(false){
}

deoglHTSector::~deoglHTSector(){
}



// Management
///////////////

void deoglHTSector::SyncToRender(){
	if(!pRSector){
		pRSector = deoglRHTSector::Ref::New(pHeightTerrain.GetRHeightTerrain(), pSector);
		pSectorChanged = true;
	}
	
	if(pSectorChanged){
		pRSector->SectorChanged(pSector);
		pSectorChanged = false;
		
		const int imageDim = pHeightTerrain.GetHeightTerrain().GetSectorResolution();
		pHeightChangeFrom.Set(0, 0);
		pHeightChangeTo.Set(imageDim - 1, imageDim -1);
		pDirtyHeights = true;
	}
	
	if(pDirtyHeights){
		pRSector->HeightChanged(pSector, pHeightChangeFrom, pHeightChangeTo);
		pDirtyHeights = false;
	}
}

void deoglHTSector::SectorChanged(){
	pSectorChanged = true;
}

void deoglHTSector::HeightChanged(const decPoint &from, const decPoint &to){
	if(pDirtyHeights){
		pHeightChangeFrom.SetSmallest(from);
		pHeightChangeTo.SetLargest(to);
		
	}else{
		pHeightChangeFrom = from;
		pHeightChangeTo = to;
		pDirtyHeights = true;
	}
}
