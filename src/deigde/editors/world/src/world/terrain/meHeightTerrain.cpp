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
pEngHT(NULL),
pChanged(false),
pDepChanged(false),
pSaved(false),
pSectorSize(500),
pSectorResolution(1024),
pBaseHeight(0.0f),
pHeightScaling(100.0f),
pActiveSector(NULL),
pActiveVLayer(NULL)
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
	
	const int sectorCount = pSectors.GetCount();
	int i;
	for(i=0; i<sectorCount; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->SetSelectedNavPoints(decIntList());
	}
	
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(NULL);
		pEngHT->FreeReference();
		pEngHT = NULL;
	}
	
	pSectorSize = size;
	
	pUpdateHeightTerrain();
	
	for(i=0; i<sectorCount; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->SectorSizeOrResChanged();
	}
	
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
	
	const int sectorCount = pSectors.GetCount();
	int i;
	for(i=0; i<sectorCount; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->SetSelectedNavPoints(decIntList());
	}
	
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(NULL);
		pEngHT->FreeReference();
		pEngHT = NULL;
	}
	
	pSectorResolution = dimension;
	
	pUpdateHeightTerrain();
	
	for(i=0; i<sectorCount; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->SectorSizeOrResChanged();
	}
	
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
	
	InvalidateAllHeights();
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
	
	InvalidateAllHeights();
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
	const int count = pSectors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		meHeightTerrainSector &sector = *((meHeightTerrainSector*)pSectors.GetAt(i));
		const decPoint &scoord = sector.GetCoordinates();
		
		if(scoord.x + 1 >= fromSector.x && scoord.x - 1 <= toSector.x
		&& scoord.y + 1 >= fromSector.y && scoord.y - 1 <= toSector.y){
			sector.InvalidateHeights(fromGrid, toGrid);
		}
	}
}

void meHeightTerrain::InvalidateAllHeights(){
	const decPoint from, to(pSectorResolution - 1, pSectorResolution - 1);
	const int count = pSectors.GetCount();
	int i;
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->InvalidateHeights(from, to);
	}
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
	
	const int count = pSectors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		meHeightTerrainSector &sector = *((meHeightTerrainSector*)pSectors.GetAt(i));
		const decPoint &scoord = sector.GetCoordinates();
		
		if(scoord.x + 1 >= fromSector.x && scoord.x - 1 <= toSector.x
		&& scoord.y + 1 >= fromSector.y && scoord.y - 1 <= toSector.y){
			sector.InvalidatePropFields(fromGrid, toGrid);
		}
	}
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
	const int count = pSectors.GetCount();
	int i;
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->InvalidatePropFields(from, to);
	}
}



void meHeightTerrain::InitDelegates(igdeEnvironment *environment){
	const int count = pSectors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->InitDelegates(environment);
	}
}

/*
void meHeightTerrain::CheckDepChanged(){
	bool depChanged = false;
	
	const int navSpaceCount = pNavSpaces.GetCount();
	for(i=0; i<navSpaceCount; i++){
		if(((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i))->GetNavSpaceChanged()){
			depChanged = true;
		}
	}
}
*/

void meHeightTerrain::Update(){
	//if( pWorld.GetAutoUpdateVegetation() ){
	//	ForceUpdateVegetation( false );
	//}
}

void meHeightTerrain::ForceUpdateVegetation(bool fullUpdate){
	const int count = pSectors.GetCount();
	int i;
	for(i=0; i<count; i++){
		meHeightTerrainSector &sector = *((meHeightTerrainSector*)pSectors.GetAt(i));
		sector.Update();
		if(fullUpdate){
			sector.UpdateVInstances();
		}
	}
}

void meHeightTerrain::ClearVegetation(){
	const int count = pSectors.GetCount();
	int i;
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->ClearVegetation();
	}
}

void meHeightTerrain::RebuildVegetationPropFieldTypes(){
	const int count = pSectors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->RebuildVegetationPropFieldTypes();
	}
}



// Sectors
////////////

int meHeightTerrain::GetSectorCount() const{
	return pSectors.GetCount();
}

meHeightTerrainSector *meHeightTerrain::GetSectorAt(int index) const{
	return (meHeightTerrainSector*)pSectors.GetAt(index);
}

meHeightTerrainSector *meHeightTerrain::GetSectorWith(const decPoint &coordinates) const{
	const int count = pSectors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		meHeightTerrainSector * const sector = (meHeightTerrainSector*)pSectors.GetAt(i);
		if(sector->GetCoordinates() == coordinates){
			return sector;
		}
	}
	
	return NULL;
}

int meHeightTerrain::IndexOfSector(meHeightTerrainSector *sector) const{
	return pSectors.IndexOf(sector);
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
	if(!pSectors.Has(sector)){
		DETHROW(deeInvalidParam);
	}
	
	if(pActiveSector == sector){
		pActiveSector = NULL;
	}
	
	sector->SetHeightTerrain(NULL);
	pSectors.Remove(sector);
	
	if(pSectors.GetCount() > 0){
		SetActiveSector((meHeightTerrainSector*)pSectors.GetAt(0));
	}
	
	SetWorldChanged();
}

void meHeightTerrain::RemoveAllSectors(){
	SetActiveSector(NULL);
	
	const int count = pSectors.GetCount();
	int i;
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->SetHeightTerrain(NULL);
	}
	
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

int meHeightTerrain::GetVLayerCount() const{
	return pVLayers.GetCount();
}

meHTVegetationLayer *meHeightTerrain::GetVLayerAt(int index) const{
	return (meHTVegetationLayer*)pVLayers.GetAt(index);
}

int meHeightTerrain::IndexOfVLayer(meHTVegetationLayer *vlayer) const{
	return pVLayers.IndexOf(vlayer);
}

bool meHeightTerrain::HasVLayer(meHTVegetationLayer *vlayer) const{
	return pVLayers.Has(vlayer);
}

void meHeightTerrain::AddVLayer(meHTVegetationLayer *vlayer){
	if(!vlayer){
		DETHROW(deeInvalidParam);
	}
	
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
	if(!vlayer){
		DETHROW(deeInvalidParam);
	}
	
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
	const int index = pVLayers.IndexOf(vlayer);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(vlayer == pActiveVLayer){
		SetActiveVLayer(NULL);
	}
	
	vlayer->SetHeightTerrain(NULL);
	pVLayers.RemoveFrom(index);
	
	SetChanged(true);
	pWorld.NotifyHTVLayerCountChanged();
	InvalidateAllPropFields();
}

void meHeightTerrain::RemoveAllVLayers(){
	SetActiveVLayer(NULL);
	
	const int count = pVLayers.GetCount();
	int i;
	for(i=0; i<count; i++){
		((meHTVegetationLayer*)pVLayers.GetAt(i))->SetHeightTerrain(NULL);
	}
	pVLayers.RemoveAll();
	
	SetChanged(true);
	pWorld.NotifyHTVLayerCountChanged();
	InvalidateAllPropFields();
}

void meHeightTerrain::SetActiveVLayer(meHTVegetationLayer *vlayer){
	if(vlayer != pActiveVLayer){
		pActiveVLayer = vlayer;
		pWorld.NotifyHTActiveVLayerChanged();
	}
}

void meHeightTerrain::NotifyVLayerChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	pWorld.NotifyHTVLayerChanged(vlayer);
}



// Private Functions
//////////////////////

void meHeightTerrain::pCleanUp(){
	RemoveAllVLayers();
	RemoveAllSectors();
	
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(NULL);
		pEngHT->FreeReference();
	}
}

void meHeightTerrain::pUpdateHeightTerrain(){
	deWorld &world = *pWorld.GetEngineWorld();
	const int count = pSectors.GetCount();
	int i;
	
	// remove all sectors from the height terrain
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->DestroyEngineSector();
	}
	
	// remove from the world
	world.SetHeightTerrain(NULL);
	
	// release the height terrain
	if(pEngHT){
		pWorld.GetEngineWorld()->SetHeightTerrain(NULL);
		pEngHT->FreeReference();
		pEngHT = NULL;
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
	for(i=0; i<count; i++){
		((meHeightTerrainSector*)pSectors.GetAt(i))->CreateEngineSector();
	}
}
