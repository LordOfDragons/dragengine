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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHeightTerrain.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainNavSpace.h"
#include "meHeightTerrainNavSpaceType.h"
#include "meHeightTerrainNavSpaceFace.h"
#include "../meWorld.h"
#include "../meWorldGuiParameters.h"
#include "../pathfinding/mePathFindTest.h"
#include "../../utils/meByteArray.h"

#include <deigde/loadsave/igdeLoadSaveHTNavSpace.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainNavSpace
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainNavSpace::meHeightTerrainNavSpace(deEngine &engine, const char *name) :
pEngine(engine),
pEngNavSpace(nullptr),
pHTSector(nullptr),

pName(name),
pSpaceType(deNavigationSpace::estMesh),
pLayer(0),
pSnapDistance(0.001f),
pSnapAngle(180.0f),

pNavSpaceChanged(false),
pNavSpaceSaved(false),

pActive(false),
pBulkUpdate(false)
{
	try{
		pDDTypeFaces = engine.GetDebugDrawerManager()->CreateDebugDrawer();
		pDDTypeFaces->SetXRay(true);
		pDDTypeFaces->SetVisible(false);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meHeightTerrainNavSpace::~meHeightTerrainNavSpace(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainNavSpace::SetHTSector(meHeightTerrainSector *sector){
	if(sector == pHTSector){
		return;
	}
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		RemoveDDFromWorld(*pHTSector->GetHeightTerrain()->GetWorld().GetEngineWorld());
	}
	
	SetEngineNavSpace(nullptr);
	pHTSector = sector;
	
	if(sector && sector->GetHeightTerrain()){
		AddDDToWorld(*sector->GetHeightTerrain()->GetWorld().GetEngineWorld());
	}
	
	pRepositionDD();
	UpdateDDColors();
}

deHeightTerrainNavSpace *meHeightTerrainNavSpace::CreateEngineNavSpace() const{
	int navCornerCount = 0;
	int navFaceCount = 0;
	
	pTypes.Visit([&](const meHeightTerrainNavSpaceType &type){
		type.GetFaces().Visit([&](const meHeightTerrainNavSpaceFace &face){
			navCornerCount += face.GetNavPoints().GetCount();
		});
		navFaceCount += type.GetFaces().GetCount();
	});
	
	deHeightTerrainNavSpace *navspace = nullptr;
	try{
		navspace = new deHeightTerrainNavSpace;
		navspace->SetType(pSpaceType);
		navspace->SetLayer(pLayer);
		navspace->SetSnapDistance(pSnapDistance);
		navspace->SetSnapAngle(pSnapAngle * DEG2RAD);
		navspace->SetCornerCount(navCornerCount);
		navspace->SetEdgeCount(0); // TODO
		navspace->SetFaceCount(navFaceCount);
		
		unsigned int *navCorners = navspace->GetCorners();
		deNavigationSpaceFace *navFaces = navspace->GetFaces();
		
		pTypes.Visit([&](const meHeightTerrainNavSpaceType &type){
			type.GetFaces().Visit([&](const meHeightTerrainNavSpaceFace &face){
				navFaces->SetType((unsigned short)type.GetType());
				navFaces->SetCornerCount(face.GetNavPoints().GetCount());
				navFaces++;
				
				face.GetNavPoints().Visit([&](int index){
					*navCorners++ = (unsigned int)index;
				});
			});
		});
		
	}catch(const deException &){
		if(navspace){
			delete navspace;
		}
		throw;
	}
	
	return navspace;
}

void meHeightTerrainNavSpace::SetEngineNavSpace(deHeightTerrainNavSpace *navspace){
	pEngNavSpace = navspace;
}



void meHeightTerrainNavSpace::SetName(const char *name){
	pName = name;
}

void meHeightTerrainNavSpace::SetSpaceType(deNavigationSpace::eSpaceTypes type){
	if(type < deNavigationSpace::estGrid || type > deNavigationSpace::estVolume){
		DETHROW(deeInvalidParam);
	}
	
	if(pSpaceType == type){
		return;
	}
	
	pSpaceType = type;
	
	if(pEngNavSpace){
		pEngNavSpace->SetType(type);
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceTypeChanged(engHTSector.IndexOfNavSpace(pEngNavSpace));
	}
	
	InvalidatePathTest();
	NotifyChanged();
}

void meHeightTerrainNavSpace::SetLayer(int layer){
	if(pLayer == layer){
		return;
	}
	
	pLayer = layer;
	
	if(pEngNavSpace){
		pEngNavSpace->SetLayer(layer);
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceLayerChanged(engHTSector.IndexOfNavSpace(pEngNavSpace));
		InvalidatePathTest();
	}
	
	NotifyChanged();
}

void meHeightTerrainNavSpace::SetSnapDistance(float distance){
	if(fabsf(distance - pSnapDistance) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSnapDistance = distance;
	
	if(pEngNavSpace){
		pEngNavSpace->SetSnapDistance(distance);
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceSnappingChanged(engHTSector.IndexOfNavSpace(pEngNavSpace));
		InvalidatePathTest();
	}
	
	NotifyChanged();
}

void meHeightTerrainNavSpace::SetSnapAngle(float angle){
	if(fabsf(angle - pSnapAngle) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSnapAngle = angle;
	
	if(pEngNavSpace){
		pEngNavSpace->SetSnapAngle(angle * DEG2RAD);
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceSnappingChanged(engHTSector.IndexOfNavSpace(pEngNavSpace));
		InvalidatePathTest();
	}
	
	NotifyChanged();
}



void meHeightTerrainNavSpace::NotifyChanged(){
	if(!pHTSector){
		return;
	}
	
	pHTSector->NotifySectorChanged();
	if(pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceChanged(this);
	}
}

void meHeightTerrainNavSpace::InvalidatePathTest() {
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return;
	}
	pHTSector->GetHeightTerrain()->GetWorld().GetPathFindTest()->Invalidate();
}



void meHeightTerrainNavSpace::HeightTerrainSizeChanged(){
	pRepositionDD();
}

void meHeightTerrainNavSpace::SectorSizeOrResChanged(){
	if(!pHTSector){
		return;
	}
	
	const int typeCount = pTypes.GetCount();
	int i;
	
	for(i=0; i<typeCount; i++){
		((meHeightTerrainNavSpaceType*)pTypes.GetAt(i))->RemoveAllFaces();
	}
	
	UpdateNavSpaceFaces();
}

void meHeightTerrainNavSpace::HeightTerrainHeightParametersChanged(){
	pRepositionDD();
}

void meHeightTerrainNavSpace::AddDDToWorld(deWorld &world){
	world.AddDebugDrawer(pDDTypeFaces);
}

void meHeightTerrainNavSpace::RemoveDDFromWorld(deWorld &world){
	world.RemoveDebugDrawer(pDDTypeFaces);
}

void meHeightTerrainNavSpace::InvalidateHeights(){
	pUpdateDDHeights();
}

void meHeightTerrainNavSpace::UpdateDDColors(){
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return;
	}
	
	const meWorld &world = pHTSector->GetHeightTerrain()->GetWorld();
	bool visible = false;
	
	switch(world.GetGuiParameters().GetWorkMode()){
	case meWorldGuiParameters::ewmNavSpaceEdit:
		visible = pActive;
		break;
		
	case meWorldGuiParameters::ewmHeightPaint:
	case meWorldGuiParameters::ewmMaskPaint:
	case meWorldGuiParameters::ewmVisibilityPaint:
		visible = pActive && world.GetGuiParameters().GetShowNavigationSpacesSelected();
		break;
		
	default:
		break;
	}
	
	if(world.GetGuiParameters().GetShowNavigationSpaces()){
		visible = true;
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deDebugDrawerShape &shape = *pDDTypeFaces->GetShapeAt(i);
		decColor color(0.5f, 0.5f, 0.5f, 1.0f);
		
		//if( pHeightTerrain->GetActive() ){
			color = ((meHeightTerrainNavSpaceType*)pTypes.GetAt(i))->GetColor();
		//}
		
		shape.SetEdgeColor(color);
		shape.SetFillColor(decColor(color, 0.1f));
	}
	
	pDDTypeFaces->NotifyShapeColorChanged();
	pDDTypeFaces->SetVisible(visible);
}



void meHeightTerrainNavSpace::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	UpdateDDColors();
}



// Types
//////////

meHeightTerrainNavSpaceType *meHeightTerrainNavSpace::GetTypeNamed(const char *name) const{
	return pTypes.FindOrDefault([&](const meHeightTerrainNavSpaceType &t) {
		return t.GetName() == name;
	});
}

bool meHeightTerrainNavSpace::HasTypeNamed(const char *name) const{
	return pTypes.HasMatching([&](const meHeightTerrainNavSpaceType &t){
		return t.GetName() == name;
	});
}

int meHeightTerrainNavSpace::IndexOfTypeNamed(const char *name) const{
	return pTypes.IndexOfMatching([&](const meHeightTerrainNavSpaceType &t){
		return t.GetName() == name;
	});
}

void meHeightTerrainNavSpace::AddType(meHeightTerrainNavSpaceType *type){
	DEASSERT_NOTNULL(type);
	DEASSERT_FALSE(HasTypeNamed(type->GetName()));
	
	pTypes.Add(type);
	type->SetNavSpace(this);
	
	if(!pBulkUpdate){
		pUpdateDDTypeFaces();
	}
	
	NotifyTypeCountChanged();
	
	if(!pActiveType){
		SetActiveType(type);
	}
}

void meHeightTerrainNavSpace::RemoveType(meHeightTerrainNavSpaceType *type){
	if(!pTypes.Has(type)){
		DETHROW(deeInvalidParam);
	}
	
	if(type == pActiveType){
		if(pTypes.GetCount() == 1){
			SetActiveType(nullptr);
			
		}else if(pTypes.GetAt(0) == type){
			SetActiveType((meHeightTerrainNavSpaceType*)pTypes.GetAt(1));
			
		}else{
			SetActiveType((meHeightTerrainNavSpaceType*)pTypes.GetAt(0));
		}
	}
	
	type->SetNavSpace(nullptr);
	
	pTypes.Remove(type);
	
	pUpdateDDTypeFaces();
	UpdateNavSpaceFaces();
	
	NotifyTypeCountChanged();
}

void meHeightTerrainNavSpace::RemoveAllTypes(){
	SetActiveType(nullptr);
	
	pTypes.Visit([](meHeightTerrainNavSpaceType &t){
		t.SetNavSpace(nullptr);
	});
	pTypes.RemoveAll();
	
	pUpdateDDTypeFaces();
	UpdateNavSpaceFaces();
	
	NotifyTypeCountChanged();
}

void meHeightTerrainNavSpace::SetActiveType(meHeightTerrainNavSpaceType *type){
	if(type == pActiveType){
		return;
	}
	
	pActiveType = type;
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceActiveTypeChanged(this);
	}
}

void meHeightTerrainNavSpace::NotifyTypeCountChanged(){
	if(!pHTSector){
		return;
	}
	
	pHTSector->NotifySectorChanged();
	
	if(pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->SetDepChanged(true);
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceTypeCountChanged(this);
	}
}



// Nav-space file
///////////////////

void meHeightTerrainNavSpace::SetPathNavSpace(const char *path, bool load){
	if(pPathNavSpace == path){
		return;
	}
	
	pPathNavSpace = path;
	
	if(load){
		LoadNavSpaceFromFile();
		
	}else{
		SetNavSpaceSaved(true);
	}
	SetNavSpaceChanged(true);
	
	NotifyTypeCountChanged();
}

void meHeightTerrainNavSpace::SetNavSpaceChanged(bool changed){
	if(changed == pNavSpaceChanged){
		return;
	}
	
	pNavSpaceChanged = changed;
	
	if(changed && pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->SetDepChanged(true);
	}
}

void meHeightTerrainNavSpace::SetNavSpaceSaved(bool saved){
	pNavSpaceSaved = saved;
}

void meHeightTerrainNavSpace::UpdateNavSpaceFaces(){
	if(!pEngNavSpace){
		return;
	}
	
	int navCornerCount = 0;
	int navFaceCount = 0;
	
	pTypes.Visit([&](const meHeightTerrainNavSpaceType &type){
		type.GetFaces().Visit([&](const meHeightTerrainNavSpaceFace &face){
			navCornerCount += face.GetNavPoints().GetCount();
		});
		navFaceCount += type.GetFaces().GetCount();
	});
	
	pEngNavSpace->SetCornerCount(navCornerCount);
	pEngNavSpace->SetEdgeCount(0); // TODO
	pEngNavSpace->SetFaceCount(navFaceCount);
	
	unsigned int *navCorners = pEngNavSpace->GetCorners();
	deNavigationSpaceFace *navFaces = pEngNavSpace->GetFaces();
	
	pTypes.Visit([&](const meHeightTerrainNavSpaceType &type){
		type.GetFaces().Visit([&](const meHeightTerrainNavSpaceFace &face){
			navFaces->SetType((unsigned short)type.GetType());
			navFaces->SetCornerCount(face.GetNavPoints().GetCount());
			navFaces++;
			
			face.GetNavPoints().Visit([&](int point){
				*navCorners++ = (unsigned int)point;
			});
		});
	});
	
	deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
	engHTSector.NotifyNavSpaceLayoutChanged(engHTSector.IndexOfNavSpace(pEngNavSpace));
	
	InvalidatePathTest();
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceFacesChanged(this);
	}
}

void meHeightTerrainNavSpace::LoadNavSpaceFromFile(){
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return; // this is a problem. we can not load the file without an environment
	}
	
	meWorld &world = pHTSector->GetHeightTerrain()->GetWorld();
	world.GetUndoSystem()->RemoveAll(); // sorry, no better way yet to keep this consistent... without using a complex undo action
	
	if(!pPathNavSpace.IsEmpty()){
		const decString baseDir(pHTSector->GetHeightTerrain()->GetBaseDirectory());
		const decPath path(decPath::AbsolutePathUnix(pPathNavSpace, baseDir));
		
		if(pEngine.GetVirtualFileSystem()->ExistsFile(path)){
			igdeLoadSaveHTNavSpace loadNavSpace(*world.GetEnvironment(), LOGSOURCE);
			deHeightTerrainNavSpace *engNavSpace = nullptr;
			
			try{
				pBulkUpdate = true;
				
				engNavSpace = new deHeightTerrainNavSpace;
				loadNavSpace.Load(*engNavSpace, pEngine.GetVirtualFileSystem()->OpenFileForReading(path));
				
				pTypes.Visit([](meHeightTerrainNavSpaceType &type){
					type.RemoveAllFaces();
				});
				
				const int navFaceCount = engNavSpace->GetFaceCount();
				const deNavigationSpaceFace * const navFaces = engNavSpace->GetFaces();
				const unsigned int *navCorners = engNavSpace->GetCorners();
				
				int i;
				for(i=0; i<navFaceCount; i++){
					const int cornerCount = navFaces[i].GetCornerCount();
					const int navType = (int)navFaces[i].GetType();
					
					meHeightTerrainNavSpaceType *type = pTypes.FindOrDefault([&](const meHeightTerrainNavSpaceType &t){
						return t.GetType() == navType;
					});
					
					if(!type){
						decString name("Type ");
						name.AppendValue(navType);
						
						const meHeightTerrainNavSpaceType::Ref newType(meHeightTerrainNavSpaceType::Ref::New());
						newType->SetName(name);
						newType->SetType(navType);
						AddType(newType);
						type = newType;
					}
					
					const meHeightTerrainNavSpaceFace::Ref newFace(meHeightTerrainNavSpaceFace::Ref::New());
					decTList<int> &newFacePoints = newFace->GetNavPoints();
					int j;
					for(j=0; j<cornerCount; j++){
						newFacePoints.Add((int)(*(navCorners++)));
					}
					type->AddFace(newFace);
				}
				
				pBulkUpdate = false;
				
			}catch(const deException &e){
				pBulkUpdate = false;
				world.GetLogger()->LogException(LOGSOURCE, e);
			}
		}
	}
	
	pTypes.Visit([&](meHeightTerrainNavSpaceType &type){
		type.UpdateDDFaces();
	});
	
	pUpdateDDTypeFaces();
	
	SetNavSpaceSaved(true);
	SetNavSpaceChanged(true);
	
	NotifyTypeCountChanged();
	
	world.NotifyHTNavSpaceFacesChanged(this);
}



// Private Functions
//////////////////////

void meHeightTerrainNavSpace::pCleanUp(){
	SetHTSector(nullptr);
	
	RemoveAllTypes();
}



void meHeightTerrainNavSpace::pRepositionDD(){
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return;
	}
	
	const float sectorSize = pHTSector->GetHeightTerrain()->GetSectorSize();
	const float heightScaling = pHTSector->GetHeightTerrain()->GetHeightScaling();
	const float baseHeight = pHTSector->GetHeightTerrain()->GetBaseHeight();
	const decPoint &coordinates = pHTSector->GetCoordinates();
	const decVector scaling(sectorSize, heightScaling, sectorSize);
	
	decDVector position;
	position.x = (double)sectorSize * (double)coordinates.x;
	position.y = (double)baseHeight;
	position.z = (double)sectorSize * (double)coordinates.y;
	
	pDDTypeFaces->SetPosition(position);
	pDDTypeFaces->SetScale(scaling);
}

void meHeightTerrainNavSpace::pUpdateDDTypeFaces(){
	pTypes.Visit([](meHeightTerrainNavSpaceType &type){
		type.SetDDShape(nullptr);
	});
	pDDTypeFaces->RemoveAllShapes();
	
	deDebugDrawerShape *shape = nullptr;
	deDebugDrawerShape *safeShape;
	
	try{
		pTypes.Visit([&](meHeightTerrainNavSpaceType &type){
			shape = new deDebugDrawerShape;
			pDDTypeFaces->AddShape(shape);
			safeShape = shape;
			shape = nullptr;
			
			type.SetDDShape(safeShape);
		});
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
	
	pDDTypeFaces->NotifyShapeLayoutChanged();
	
	UpdateDDColors();
	UpdateNavSpaceFaces();
}

void meHeightTerrainNavSpace::pUpdateDDHeights(){
	if(!pHTSector){
		return;
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((meHeightTerrainNavSpaceType*)pTypes.GetAt(i))->UpdateHeights();
	}
	pDDTypeFaces->NotifyShapeContentChanged();
}
