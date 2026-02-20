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

#include "debpHTSector.h"
#include "debpHeightTerrain.h"

#include "../../world/debpWorld.h"

#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/common/exceptions.h>



// Class debpHeightTerrain
////////////////////////////

// Constructor, destructor
////////////////////////////

debpHeightTerrain::debpHeightTerrain(dePhysicsBullet *bullet, deHeightTerrain *heightTerrain) :
pBullet(bullet),
pHeightTerrain(heightTerrain),
pParentWorld(NULL){
	DEASSERT_NOTNULL(bullet);
	DEASSERT_NOTNULL(heightTerrain);
	
	heightTerrain->GetSectors().Visit([&](deHeightTerrainSector *sector){
		SectorAdded(sector);
	});
}

debpHeightTerrain::~debpHeightTerrain(){
}



// Management
///////////////

void debpHeightTerrain::SetParentWorld(debpWorld *parentWorld){
	pParentWorld = parentWorld;
	pSectors.Visit([&](debpHTSector &s){
		s.ParentWorldChanged();
	});
}



debpHTSector *debpHeightTerrain::GetSectorAt(int index) const{
	return pSectors.GetAt(index);
}

debpHTSector *debpHeightTerrain::GetSectorWith(int x, int y) const{
	return pSectors.FindOrNull([&](const debpHTSector &s){
		const decPoint &c = s.GetSector()->GetSector();
		return c.x == x && c.y == y;
	});
}


void debpHeightTerrain::Update(){
	pSectors.Visit([&](debpHTSector &s){
		s.Update();
	});
}



// Notifications
//////////////////

void debpHeightTerrain::ParametersChanged(){
}

void debpHeightTerrain::CollisionFilterChanged(){
	/*
	int s;
	
	for(s=0; s<pTouchSensorCount; s++){
		pTouchSensors[s]->TestTerrain(this);
	}
	*/
}

void debpHeightTerrain::HeightChanged(const decPoint &fromSector, const decPoint &fromCoordinates,
const decPoint &toSector, const decPoint &toCoordinates){
	int imageDim = pHeightTerrain->GetSectorResolution();
	pSectors.Visit([&](debpHTSector &s){
//		const decPoint &c = s.GetSector()->GetSector();
//		if(c.x >= fromSector.x && c.y >= fromSector.y && c.x <= toSector.x && c.y <= toSector.y){
			// TODO determine the area to update
			s.UpdateHeights(0, 0, imageDim, imageDim);
//		}
	});
}



void debpHeightTerrain::SectorAdded(deHeightTerrainSector *sector){
	pSectors.Add(deTUniqueReference<debpHTSector>::New(this, sector));
	if(pParentWorld){
		pSectors.Last()->ParentWorldChanged();
	}
}

void debpHeightTerrain::SectorRemoved(int index){
	pSectors.RemoveFrom(index);
}

void debpHeightTerrain::AllSectorsRemoved(){
	pSectors.RemoveAll();
}

void debpHeightTerrain::SectorChanged(int index){
	pSectors.GetAt(index)->SectorChanged();
}



void debpHeightTerrain::DecalAdded(int sector, deDecal *decal){
}

void debpHeightTerrain::DecalRemoved(int sector, deDecal *decal){
}

void debpHeightTerrain::AllDecalsRemoved(int sector){
}



// Collision Detection
////////////////////////

void debpHeightTerrain::FindDecalsAt(const decDVector &point, deDecal::List &list){
}

void debpHeightTerrain::FindDecalsTouching(const decShape &shape, deDecal::List &list){
}
