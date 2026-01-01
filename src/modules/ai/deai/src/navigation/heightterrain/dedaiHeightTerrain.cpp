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
#include <string.h>

#include "dedaiHeightTerrain.h"
#include "dedaiHeightTerrainNavSpace.h"
#include "dedaiHeightTerrainSector.h"
#include "../../deDEAIModule.h"
#include "../../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>



// Class dedaiHeightTerrain
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiHeightTerrain::dedaiHeightTerrain(deDEAIModule &deai, const deHeightTerrain &heightTerrain) :
pDEAI(deai),
pHeightTerrain(heightTerrain),
pParentWorld(NULL)
{
	const int sectorCount = heightTerrain.GetSectorCount();
	int i;
	
	try{
		for(i=0; i<sectorCount; i++){
			SectorAdded(heightTerrain.GetSectorAt(i));
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dedaiHeightTerrain::~dedaiHeightTerrain(){
	pCleanUp();
}



// Management
///////////////

void dedaiHeightTerrain::SetParentWorld(dedaiWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	pParentWorld = world;
	
	const int sectorCount = pSectors.GetCount();
	int i;
	
	for(i=0; i<sectorCount; i++){
		((dedaiHeightTerrainSector*)pSectors.GetAt(i))->ParentWorldChanged();
	}
}



// Sectors
////////////

int dedaiHeightTerrain::GetSectorCount() const{
	return pSectors.GetCount();
}

dedaiHeightTerrainSector *dedaiHeightTerrain::GetSectorAt(int index) const{
	return (dedaiHeightTerrainSector*)pSectors.GetAt(index);
}



// Notifications
//////////////////

void dedaiHeightTerrain::ParametersChanged(){
	const int sectorCount = pSectors.GetCount();
	int i;
	
	for(i=0; i<sectorCount; i++){
		((dedaiHeightTerrainSector*)pSectors.GetAt(i))->SectorChanged();
	}
}

void dedaiHeightTerrain::SectorAdded(deHeightTerrainSector *sector){
	pSectors.Add(dedaiHeightTerrainSector::Ref::New(*this, *sector));
}

void dedaiHeightTerrain::SectorRemoved(int index){
	pSectors.RemoveFrom(index);
}

void dedaiHeightTerrain::AllSectorsRemoved(){
	pSectors.RemoveAll();
}

void dedaiHeightTerrain::SectorChanged(int index){
	((dedaiHeightTerrainSector*)pSectors.GetAt(index))->SectorChanged();
}

void dedaiHeightTerrain::HeightChanged(const decPoint &fromSector,
const decPoint &fromCoordinates, const decPoint &toSector, const decPoint &toCoordinates){
	const int imageDim = pHeightTerrain.GetSectorResolution();
	const int sectorCount = pSectors.GetCount();
	decPoint localFrom, localTo;
	int i;
	
	for(i=0; i<sectorCount; i++){
		dedaiHeightTerrainSector &sector = *((dedaiHeightTerrainSector*)pSectors.GetAt(i));
		const decPoint &scoord = sector.GetSector().GetSector();
		
		if(scoord.x + 1 >= fromSector.x && scoord.x - 1 <= toSector.x
		&& scoord.y + 1 >= fromSector.y && scoord.y - 1 <= toSector.y){
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

void dedaiHeightTerrain::NavSpaceAdded(int sector, deHeightTerrainNavSpace *navspace){
	((dedaiHeightTerrainSector*)pSectors.GetAt(sector))->NavSpaceAdded(navspace);
}

void dedaiHeightTerrain::NavSpaceRemoved(int sector, int index){
	((dedaiHeightTerrainSector*)pSectors.GetAt(sector))->NavSpaceRemoved(index);
}

void dedaiHeightTerrain::AllNavSpacesRemoved(int sector){
	((dedaiHeightTerrainSector*)pSectors.GetAt(sector))->AllNavSpacesRemoved();
}

void dedaiHeightTerrain::NavSpaceLayerChanged(int sector, int index){
	((dedaiHeightTerrainSector*)pSectors.GetAt(sector))->NavSpaceLayerChanged(index);
}

void dedaiHeightTerrain::NavSpaceTypeChanged(int sector, int index){
	((dedaiHeightTerrainSector*)pSectors.GetAt(sector))->NavSpaceTypeChanged(index);
}

void dedaiHeightTerrain::NavSpaceSnappingChanged(int sector, int index){
	((dedaiHeightTerrainSector*)pSectors.GetAt(sector))->NavSpaceSnappingChanged(index);
}

void dedaiHeightTerrain::NavSpaceLayoutChanged(int sector, int index){
	((dedaiHeightTerrainSector*)pSectors.GetAt(sector))->NavSpaceLayoutChanged(index);
}



// Private Functions
//////////////////////

void dedaiHeightTerrain::pCleanUp(){
	SetParentWorld(NULL);
	AllSectorsRemoved();
}
