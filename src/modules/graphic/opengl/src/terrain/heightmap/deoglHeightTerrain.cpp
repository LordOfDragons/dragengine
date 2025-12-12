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

#include "deoglHeightTerrain.h"
#include "deoglHTSector.h"
#include "deoglRHeightTerrain.h"

#include "../../deoglBasics.h"
#include "../../deGraphicOpenGl.h"
#include "../../world/deoglWorld.h"

#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/common/exceptions.h>



// Class deoglHeightTerrain
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglHeightTerrain::deoglHeightTerrain(deGraphicOpenGl &ogl, const deHeightTerrain &heightTerrain) :
pOgl(ogl),
pHeightTerrain(heightTerrain),
pDirtySectors(true)
{
	const int sectorCount = heightTerrain.GetSectorCount();
	int i;
	
	try{
		pRHeightTerrain.TakeOver(new deoglRHeightTerrain(ogl.GetRenderThread(), heightTerrain));
		
		for(i=0; i<sectorCount; i++){
			SectorAdded(heightTerrain.GetSectorAt(i));
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglHeightTerrain::~deoglHeightTerrain(){
	pCleanUp();
}



// Management
///////////////

void deoglHeightTerrain::Update(float elapsed){
	// TODO update skins... once we have this back working
}

void deoglHeightTerrain::SyncToRender(){
	const int count = pSectors.GetCount();
	int i;
	
	if(pDirtySectors){
		pRHeightTerrain->RemoveAllSectors();
		
		for(i=0; i<count; i++){
			deoglHTSector * const htsector = (deoglHTSector*)pSectors.GetAt(i);
			htsector->SyncToRender();
			pRHeightTerrain->AddSector(htsector->GetRSector());
		}
		
		pDirtySectors = false;
		
	}else{
		for(i=0; i<count; i++){
			((deoglHTSector*)pSectors.GetAt(i))->SyncToRender();
		}
	}
}



// Notifications
//////////////////

void deoglHeightTerrain::ParametersChanged(){
	pDirtySectors = true;
}

void deoglHeightTerrain::HeightChanged(const decPoint &fromSector,
const decPoint &fromCoordinates, const decPoint &toSector, const decPoint &toCoordinates){
	const int imageDim = pHeightTerrain.GetSectorResolution();
	const int count = pSectors.GetCount();
	decPoint localFrom, localTo;
	int i;
	
	for(i=0; i<count; i++){
		deoglHTSector &sector = *((deoglHTSector*)pSectors.GetAt(i));
		const decPoint &scoord = pHeightTerrain.GetSectorAt(i)->GetSector();
		
		if(scoord.x + 1 >= fromSector.x && scoord.x - 1 <= toSector.x && scoord.y + 1 >= fromSector.y && scoord.y - 1 <= toSector.y){
			if(fromSector.x == toSector.x){
				localFrom.x = fromCoordinates.x;
				localTo.x = toCoordinates.x;
				
			}else{
				if(scoord.x == toSector.x){
					localFrom.x = toCoordinates.x;
					
				}else{
					localFrom.x = 0;
				}
				
				if(scoord.x == fromSector.x){
					localTo.x = fromCoordinates.x;
					
				}else{
					localTo.x = imageDim;
				}
			}
			
			if(fromSector.y == toSector.y){
				localFrom.y = fromCoordinates.y;
				localTo.y = toCoordinates.y;
				
			}else{
				if(scoord.y == toSector.y){
					localFrom.y = toCoordinates.y;
					
				}else{
					localFrom.y = 0;
				}
				
				if(scoord.y == fromSector.y){
					localTo.y = fromCoordinates.y;
					
				}else{
					localTo.y = imageDim;
				}
			}
			
			if(localFrom.x > 0){
				localFrom.x--;
			}
			if(localFrom.y > 0){
				localFrom.y--;
			}
			if(localTo.x < imageDim){
				localTo.x++;
			}
			if(localTo.y < imageDim){
				localTo.y++;
			}
			
			sector.HeightChanged(localFrom, localTo);
		}
	}
}



void deoglHeightTerrain::SectorAdded(deHeightTerrainSector *sector){
	pSectors.Add(deoglHTSector::Ref::New(*this, *sector));
	pDirtySectors = true;
}

void deoglHeightTerrain::SectorRemoved(int index){
	pSectors.RemoveFrom(index);
	pDirtySectors = true;
}

void deoglHeightTerrain::AllSectorsRemoved(){
	pSectors.RemoveAll();
	pDirtySectors = true;
}

void deoglHeightTerrain::SectorChanged(int index){
	((deoglHTSector*)pSectors.GetAt(index))->SectorChanged();
	pDirtySectors = true;
}



void deoglHeightTerrain::DecalAdded(int sector, deDecal *decal){
}

void deoglHeightTerrain::DecalRemoved(int sector, deDecal *decal){
}

void deoglHeightTerrain::AllDecalsRemoved(int sector){
}



// Private functions
//////////////////////

void deoglHeightTerrain::pCleanUp(){
	AllSectorsRemoved();
}
