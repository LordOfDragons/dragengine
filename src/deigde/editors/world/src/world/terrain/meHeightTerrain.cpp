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

#include "meHeightTerrain.h"
#include "meHeightTerrainNavSpace.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainPropField.h"
#include "../meWorld.h"
#include "../meWorldGuiParameters.h"
#include "../object/meObject.h"
#include "../heightterrain/meHTVegetationLayer.h"
#include "../pathfinding/mePathFindTest.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainManager.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/image/deImage.h>



// Class meHeightTerrain
//////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrain::meHeightTerrain(meWorld &world) :
pWorld(world),
pChanged(false),
pDepChanged(false),
pSaved(false),
pSectorSize(500),
pSectorResolution(1024),
pBaseHeight(0.0f),
pHeightScaling(100.0f),
pActiveSector(nullptr)
{
	try{
		pUpdateHeightTerrain();
		
	}catch(const deException &e){
		e.PrintError();
		pCleanUp();
		throw;
	}
}

meHeightTerrain::~meHeightTerrain(){
	RemoveAllSectors();
	pCleanUp();
}



// Management
///////////////

void meHeightTerrain::SetPathHT(const char *path){
	if(pPathHT == path){
		return;
	}
	
	pPathHT = path;
	
	SetChanged(true);
	pWorld.NotifyHTStateChanged();
}

decString meHeightTerrain::GetBaseDirectory() const{
	decPath path(decPath::AbsolutePathUnix(pPathHT, pWorld.GetDirectoryPath()));
	
	if(path.GetComponentCount() > 0){
		path.RemoveLastComponent();
		
	}else{
		path.SetFromNative("/"); // in new world files both path can be empty
	}
	
	return path.GetPathUnix();
}

void meHeightTerrain::SetDepChanged(bool changed){
	if(changed == pDepChanged){
		return;
	}
	
	pDepChanged = changed;
	
	if(changed){
		pWorld.SetDepChanged(true);
	}
	pWorld.NotifyHTStateChanged();
}

void meHeightTerrain::SetChanged(bool changed){
	if(changed == pChanged){
		return;
	}
	
	pChanged = changed;
	
	if(changed){
		pWorld.SetChanged(true);
		pWorld.SetDepChanged(true);
	}
	pWorld.NotifyHTStateChanged();
}

void meHeightTerrain::SetSaved(bool saved){
	pSaved = saved;
}

void meHeightTerrain::SetSectorSize(float size){
	if(size < 10.0f){
		DETHROW(deeInvalidParam);
	}
	if(fabsf(size - pSectorSize) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.SetSelectedNavPoints({});
	});
	
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(nullptr);
		pEngHT = nullptr;
	}
	
	pSectorSize = size;
	
	pUpdateHeightTerrain();
	
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.SectorSizeOrResChanged();
	});
	
	SetChanged(true);
	pWorld.NotifyHTChanged();
}

void meHeightTerrain::SetSectorResolution(int dimension){
	if(dimension < 2){
		DETHROW(deeInvalidParam);
	}
	
	if(dimension == pSectorResolution){
		return;
	}
	
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.SetSelectedNavPoints({});
	});
	
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(nullptr);
		pEngHT = nullptr;
	}
	
	pSectorResolution = dimension;
	
	pUpdateHeightTerrain();
	
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.SectorSizeOrResChanged();
	});
	
	SetChanged(true);
	pWorld.NotifyHTChanged();
}

void meHeightTerrain::SetBaseHeight(float height){
	if(fabsf(height - pBaseHeight) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBaseHeight = height;
	
	if(pEngHT){
		pEngHT->SetBaseHeight(height);
	}
	
	SetChanged(true);
	pWorld.NotifyHTChanged();
}

void meHeightTerrain::SetHeightScaling(float scaling){
	scaling = decMath::max(scaling, 0.0f);
	
	if(fabsf(scaling - pHeightScaling) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pHeightScaling = scaling;
	
	if(pEngHT){
		pEngHT->SetHeightScaling(scaling);
	}
	
	SetChanged(true);
	pWorld.NotifyHTChanged();
}

void meHeightTerrain::SetWorldChanged(){
	pWorld.SetChanged(true);
}



void meHeightTerrain::ShowStateChanged(){
	if(pActiveSector){
		pActiveSector->ShowStateChanged();
	}
}



void meHeightTerrain::NotifyHeightsChanged(const decPoint &fromSector,
const decPoint &fromGrid, const decPoint &toSector, const decPoint &toGrid){
	if(pEngHT){
		pEngHT->NotifyHeightChanged(fromSector, fromGrid, toSector, toGrid);
		pWorld.GetPathFindTest()->Invalidate();
	}
	
	InvalidateHeights(fromSector, fromGrid, toSector, toGrid);
	InvalidatePropFields(fromSector, fromGrid, toSector, toGrid);
}

void meHeightTerrain::NotifyHeightsChanged(const decBoundary &areaSector, const decBoundary &areaGrid){
	NotifyHeightsChanged(decPoint(areaSector.x1, areaSector.y1),
		decPoint(areaGrid.x1, areaGrid.y1),
		decPoint(areaSector.x2, areaSector.y2),
		decPoint(areaGrid.x2, areaGrid.y2));
}



void meHeightTerrain::InvalidateHeights(const decPoint &fromSector,
const decPoint &fromGrid, const decPoint &toSector, const decPoint &toGrid){
	pSectors.Visit([&](meHeightTerrainSector &s){
		const decPoint &scoord = s.GetCoordinates();
		
		if(scoord.x + 1 >= fromSector.x && scoord.x - 1 <= toSector.x
		&& scoord.y + 1 >= fromSector.y && scoord.y - 1 <= toSector.y){
			s.InvalidateHeights(fromGrid, toGrid);
		}
	});
}

void meHeightTerrain::InvalidateAllHeights(){
	const decPoint from, to(pSectorResolution - 1, pSectorResolution - 1);
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.InvalidateHeights(from, to);
	});
}



void meHeightTerrain::InvalidatePropFields(const decPoint &fromSector,
const decPoint &fromGrid, const decPoint &toSector, const decPoint &toGrid){
	// little note on this test. sectors can have a slight radius of influence
	// added to them due to vegetation rules requiring to search for world
	// elements in a certain radius. to obtain a correct result a sector has
	// to be tested also if it would lie outside the sector coordinates to
	// test but due to the added radius it still falls in the range. using only
	// coordinates this case would be easily missed. testing though now for all
	// sectors if they are inside the area including the radius of influence is
	// for most sectors a waste of time. therefore a little hack is used. this
	// hack assumes that only a maximum of sector_dimension meters every radius
	// of influence can have. this is a rather large number and a reasonable
	// assumption since using such large search radi are next to never used.
	// therefore the radius of influence is ignored and instead the search area
	// grown by one sector. this way a couple of sectors are tested in addition
	// to the required ones but all sectors outside are not tested improving
	// performance in large worlds with many sectors
	
	pSectors.Visit([&](meHeightTerrainSector &s){
		const decPoint &scoord = s.GetCoordinates();
		
		if(scoord.x + 1 >= fromSector.x && scoord.x - 1 <= toSector.x
		&& scoord.y + 1 >= fromSector.y && scoord.y - 1 <= toSector.y){
			s.InvalidatePropFields(fromGrid, toGrid);
		}
	});
}

void meHeightTerrain::InvalidatePropFields(const decBoundary &areaSector, const decBoundary &areaGrid){
	InvalidatePropFields(decPoint(areaSector.x1, areaSector.y1),
		decPoint(areaGrid.x1, areaGrid.y1),
		decPoint(areaSector.x2, areaSector.y2),
		decPoint(areaGrid.x2, areaGrid.y2));
}

void meHeightTerrain::InvalidatePropFields(meHeightTerrainSector *sector){
	InvalidatePropFields(sector->GetCoordinates(), decPoint(), sector->GetCoordinates(), decPoint(pSectorResolution, pSectorResolution));
}

void meHeightTerrain::InvalidatePropFields(meObject *object){
	decDVector position = object->GetPosition();
	decPoint3 scoord; // = object->GetWorldSector()->GetCoordinates();
	double invSectorDim = 1.0 / (double)pSectorSize;
	
	int x = (int)(((position.x - (double)pSectorSize * (double)scoord.x) * invSectorDim + 0.5) * (double)pSectorResolution);
	int z = (int)((0.5 - (position.z - (double)pSectorSize * (double)scoord.z) * invSectorDim) * (double)pSectorResolution);
	
	decPoint fromToSector(scoord.x, scoord.z);
	decPoint fromGrid(x, z);
	decPoint toGrid(x + 1, z + 1);
	
	if(toGrid.x > pSectorResolution) toGrid.x = pSectorResolution;
	if(toGrid.y > pSectorResolution) toGrid.y = pSectorResolution;
	
	InvalidatePropFields(fromToSector, fromGrid, fromToSector, toGrid);
}

void meHeightTerrain::InvalidateAllPropFields(){
	const decPoint from, to(pSectorResolution - 1, pSectorResolution - 1);
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.InvalidatePropFields(from, to);
	});
}



void meHeightTerrain::InitDelegates(igdeEnvironment *environment){
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.InitDelegates(environment);
	});
}

/*
void meHeightTerrain::CheckDepChanged(){
	bool depChanged = false;
	
	pNavSpaces.Visit([&](meHeightTerrainNavSpace &n){
		if(n.GetNavSpaceChanged()){
			depChanged = true;
		}
	});
}
*/

void meHeightTerrain::Update(){
	//if( pWorld.GetAutoUpdateVegetation() ){
	//	ForceUpdateVegetation( false );
	//}
}

void meHeightTerrain::ForceUpdateVegetation(bool fullUpdate){
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.Update();
		if(fullUpdate){
			s.UpdateVInstances();
		}
	});
}

void meHeightTerrain::ClearVegetation(){
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.ClearVegetation();
	});
}

void meHeightTerrain::RebuildVegetationPropFieldTypes(){
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.RebuildVegetationPropFieldTypes();
	});
}



// Sectors
////////////

meHeightTerrainSector *meHeightTerrain::GetSectorWith(const decPoint &coordinates) const{
	return pSectors.FindOrDefault([&](const meHeightTerrainSector &s){
		return s.GetCoordinates() == coordinates;
	});
}

void meHeightTerrain::AddSector(meHeightTerrainSector *sector){
	if(!sector || GetSectorWith(sector->GetCoordinates())){
		DETHROW(deeInvalidParam);
	}
	
	pSectors.Add(sector);
	
	sector->SetHeightTerrain(this);
	
	if(!pActiveSector){
		SetActiveSector(sector);
	}
	
	SetWorldChanged();
}

void meHeightTerrain::RemoveSector(meHeightTerrainSector *sector){
	const meHeightTerrainSector::Ref guard(sector);
	
	if(pActiveSector == sector){
		pActiveSector = nullptr;
	}
	
	pSectors.Remove(sector);
	sector->SetHeightTerrain(nullptr);
	
	if(pSectors.IsNotEmpty()){
		SetActiveSector(pSectors.First());
	}
	
	SetWorldChanged();
}

void meHeightTerrain::RemoveAllSectors(){
	SetActiveSector(nullptr);
	
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.SetHeightTerrain(nullptr);
	});
	pSectors.RemoveAll();
	
	SetWorldChanged();
}



void meHeightTerrain::SetActiveSector(meHeightTerrainSector *sector){
	if(sector == pActiveSector){
		return;
	}
	
	if(pActiveSector){
		pActiveSector->SetActive(false);
	}
	
	pActiveSector = sector;
	
	if(sector){
		sector->SetActive(true);
	}
}



// Vegetation
///////////////

void meHeightTerrain::AddVLayer(meHTVegetationLayer *vlayer){
	DEASSERT_NOTNULL(vlayer)
	
	pVLayers.Add(vlayer);
	vlayer->SetHeightTerrain(this);
	
	SetChanged(true);
	pWorld.NotifyHTVLayerCountChanged();
	
	if(!pActiveVLayer){
		SetActiveVLayer(vlayer);
	}
	
	InvalidateAllPropFields();
}

void meHeightTerrain::InsertVLayer(int before, meHTVegetationLayer *vlayer){
	DEASSERT_NOTNULL(vlayer)
	
	pVLayers.Insert(vlayer, before);
	vlayer->SetHeightTerrain(this);
	
	SetChanged(true);
	pWorld.NotifyHTVLayerCountChanged();
	
	if(!pActiveVLayer){
		SetActiveVLayer(vlayer);
	}
	
	InvalidateAllPropFields();
}

void meHeightTerrain::MoveVLayer(meHTVegetationLayer *vlayer, int moveTo){
	pVLayers.Move(vlayer, moveTo);
	SetChanged(true);
	pWorld.NotifyHTVLayerCountChanged();
	InvalidateAllPropFields();
}

void meHeightTerrain::RemoveVLayer(meHTVegetationLayer *vlayer){
	const meHTVegetationLayer::Ref guard(vlayer);
	
	if(vlayer == pActiveVLayer){
		SetActiveVLayer(nullptr);
	}
	
	pVLayers.Remove(vlayer);
	vlayer->SetHeightTerrain(nullptr);
	
	SetChanged(true);
	pWorld.NotifyHTVLayerCountChanged();
	InvalidateAllPropFields();
}

void meHeightTerrain::RemoveAllVLayers(){
	SetActiveVLayer(nullptr);
	
	pVLayers.Visit([](meHTVegetationLayer &vlayer){
		vlayer.SetHeightTerrain(nullptr);
	});
	pVLayers.RemoveAll();
	
	SetChanged(true);
	pWorld.NotifyHTVLayerCountChanged();
	InvalidateAllPropFields();
}

void meHeightTerrain::SetActiveVLayer(meHTVegetationLayer *vlayer){
	if(vlayer == pActiveVLayer){
		return;
	}
	
	pActiveVLayer = vlayer;
	pWorld.NotifyHTActiveVLayerChanged();
}

void meHeightTerrain::NotifyVLayerChanged(meHTVegetationLayer *vlayer){
	DEASSERT_NOTNULL(vlayer)
	pWorld.NotifyHTVLayerChanged(vlayer);
}



// Private Functions
//////////////////////

void meHeightTerrain::pCleanUp(){
	RemoveAllVLayers();
	RemoveAllSectors();
	
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(nullptr);
	}
}

void meHeightTerrain::pUpdateHeightTerrain(){
	deWorld &world = *pWorld.GetEngineWorld();
	
	// remove all sectors from the height terrain
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.DestroyEngineSector();
	});
	
	// remove from the world
	world.SetHeightTerrain(nullptr);
	
	// release the height terrain
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(nullptr);
		pEngHT = nullptr;
	}
	
	// create a new height terrain
	pEngHT = pWorld.GetEngine()->GetHeightTerrainManager()->CreateHeightTerrain(pSectorSize, pSectorResolution);
	pWorld.GetEngineWorld()->SetHeightTerrain(pEngHT);
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit(meWorld::eclmHeightTerrains);
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit(meWorld::eclmEditing);
	
	pEngHT->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
	
	pEngHT->SetBaseHeight(pBaseHeight);
	pEngHT->SetHeightScaling(pHeightScaling);
	
	// set the height terrain to the world
	world.SetHeightTerrain(pEngHT);
	
	// add sectors to the height terrain
	pSectors.Visit([&](meHeightTerrainSector &s){
		s.CreateEngineSector();
	});
}
