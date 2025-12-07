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
#include "meHeightTerrainNavSpace.h"
#include "meHeightTerrainNavSpaceType.h"
#include "meHeightTerrainNavSpaceFace.h"
#include "meHeightTerrainSector.h"
#include "../meWorld.h"
#include "../meWorldGuiParameters.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/image/deImage.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainNavSpaceType
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainNavSpaceType::meHeightTerrainNavSpaceType(const char *name) :
pNavSpace(NULL),
pName(name),
pColor(1.0f, 0.5f, 0.0f),
pType(0),
pActive(false),
pDDShape(NULL){
}

meHeightTerrainNavSpaceType::~meHeightTerrainNavSpaceType(){
	RemoveAllFaces();
}



// Management
///////////////

void meHeightTerrainNavSpaceType::SetNavSpace(meHeightTerrainNavSpace *navspace){
	if(navspace == pNavSpace){
		return;
	}
	
	SetDDShape(NULL);
	pNavSpace = navspace;
}

void meHeightTerrainNavSpaceType::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	NotifyChanged();
}

void meHeightTerrainNavSpaceType::SetColor(const decColor &color){
	if(pColor.IsEqualTo(color)){
		return;
	}
	
	pColor = color;
	
	if(pNavSpace){
		pNavSpace->UpdateDDColors();
	}
	
	NotifyChanged();
}

void meHeightTerrainNavSpaceType::SetType(int type){
	if(pType == type){
		return;
	}
	
	pType = type;
	
	NotifyTypeChanged();
}


void meHeightTerrainNavSpaceType::NotifyChanged(){
	if(! pNavSpace){
		return;
	}
	
	meHeightTerrainSector * const sector = pNavSpace->GetHTSector();
	if(! sector){
		return;
	}
	
	sector->NotifySectorChanged();
	if(sector->GetHeightTerrain()){
		sector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceTypeChanged(pNavSpace, this);
	}
}

void meHeightTerrainNavSpaceType::NotifyTypeChanged(){
	if(! pNavSpace){
		return;
	}
	
	meHeightTerrainSector * const sector = pNavSpace->GetHTSector();
	if(! sector){
		return;
	}
	
	sector->NotifySectorChanged();
	if(sector->GetHeightTerrain()){
		sector->GetHeightTerrain()->SetDepChanged(true);
		sector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceTypeChanged(pNavSpace, this);
	}
}



void meHeightTerrainNavSpaceType::SetDDShape(deDebugDrawerShape *shape){
	if(shape == pDDShape){
		return;
	}
	
	pDDShape = shape;
	pUpdateDDFaces();
}

void meHeightTerrainNavSpaceType::UpdateHeights(){
	if(! pDDShape || ! pNavSpace || ! pNavSpace->GetHTSector()
	|| ! pNavSpace->GetHTSector()->GetHeightTerrain()){
		return;
	}
	
	const meHeightTerrainSector &sector = *pNavSpace->GetHTSector();
	const sGrayscale32 * const heights = sector.GetHeightImage()->GetDataGrayscale32();
	const int imageDim = sector.GetHeightTerrain()->GetSectorResolution();
	const float scaleFactor = (float)(imageDim - 1);
	const int count = pFaces.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		deDebugDrawerShapeFace &ddface = *pDDShape->GetFaceAt(i);
		const int vertexCount = ddface.GetVertexCount();
		
		for(j=0; j<vertexCount; j++){
			decVector position(ddface.GetVertexAt(j));
			const int x = (int)((position.x + 0.5f) * scaleFactor + 0.5f);
			const int z = (int)((0.5f - position.z) * scaleFactor + 0.5f);
			position.y = heights[imageDim * z + x].value;
			ddface.SetVertexAt(j, position);
		}
		//ddface.CalculateNormal();
	}
}



// Faces
//////////

int meHeightTerrainNavSpaceType::GetFaceCount() const{
	return pFaces.GetCount();
}

meHeightTerrainNavSpaceFace *meHeightTerrainNavSpaceType::GetFaceAt(int index) const{
	return (meHeightTerrainNavSpaceFace*)pFaces.GetAt(index);
}

bool meHeightTerrainNavSpaceType::HasFace(meHeightTerrainNavSpaceFace *face) const{
	return pFaces.Has(face);
}

int meHeightTerrainNavSpaceType::IndexOfFace(meHeightTerrainNavSpaceFace *face) const{
	return pFaces.IndexOf(face);
}

void meHeightTerrainNavSpaceType::AddFace(meHeightTerrainNavSpaceFace *face){
	if(! face){
		DETHROW(deeInvalidParam);
	}
	
	pFaces.Add(face);
	face->SetType(this);
	
	if(pNavSpace){
		if(pNavSpace->GetBulkUpdate()){
			return;
		}
		
		pNavSpace->UpdateNavSpaceFaces();
	}
	
	pUpdateDDFaces();
	NotifyFacesChanged();
}

void meHeightTerrainNavSpaceType::RemoveFace(meHeightTerrainNavSpaceFace *face){
	if(! pFaces.Has(face)){
		DETHROW(deeInvalidParam);
	}
	face->SetType(NULL);
	pFaces.Remove(face);
	
	if(pNavSpace){
		pNavSpace->UpdateNavSpaceFaces();
	}
	
	pUpdateDDFaces();
	NotifyFacesChanged();
}

void meHeightTerrainNavSpaceType::RemoveAllFaces(){
	const int count = pFaces.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		((meHeightTerrainNavSpaceFace*)pFaces.GetAt(i))->SetType(NULL);
	}
	
	pFaces.RemoveAll();
	
	if(pNavSpace){
		pNavSpace->UpdateNavSpaceFaces();
	}
	
	pUpdateDDFaces();
	NotifyFacesChanged();
}

void meHeightTerrainNavSpaceType::NotifyFacesChanged(){
	if(! pNavSpace || ! pNavSpace->GetHTSector()){
		return;
	}
	
	pNavSpace->SetNavSpaceChanged(true);
	
	meHeightTerrainSector * const sector = pNavSpace->GetHTSector();
	sector->NotifySectorChanged();
	if(sector->GetHeightTerrain()){
		sector->GetHeightTerrain()->SetDepChanged(true);
		sector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceFacesChanged(pNavSpace);
	}
}

void meHeightTerrainNavSpaceType::UpdateDDFaces(){
	pUpdateDDFaces();
}



// Private functions
//////////////////////

void meHeightTerrainNavSpaceType::pUpdateDDFaces(){
	if(! pDDShape){
		return;
	}
	
	pDDShape->RemoveAllFaces();
	
	if(! pNavSpace || ! pNavSpace->GetHTSector() || ! pNavSpace->GetHTSector()->GetHeightTerrain()){
		return;
	}
	
	const int count = pFaces.GetCount();
	if(count == 0){
		pNavSpace->GetDDTypeFaces()->NotifyShapeContentChanged();
		return;
	}
	
	const meHeightTerrainSector &sector = *pNavSpace->GetHTSector();
	const sGrayscale32 * const heights = sector.GetHeightImage()->GetDataGrayscale32();
	const int imageDim = sector.GetHeightTerrain()->GetSectorResolution();
	const float scaleFactor = 1.0f / (float)(imageDim - 1);
	deDebugDrawerShapeFace *ddface = NULL;
	int i, j;
	
	try{
		for(i=0; i<count; i++){
			const meHeightTerrainNavSpaceFace &typeFace = *((meHeightTerrainNavSpaceFace*)pFaces.GetAt(i));
			const decIntList &navpoints = typeFace.GetNavPoints();
			const int navpointCount = navpoints.GetCount();
			
			ddface = new deDebugDrawerShapeFace;
			for(j=0; j<navpointCount; j++){
				const int navpoint = navpoints.GetAt(j);
				ddface->AddVertex(decVector(
					(float)(navpoint % imageDim) * scaleFactor - 0.5f,
					heights[navpoint].value,
					0.5f - (float)(navpoint / imageDim) * scaleFactor));
			}
			
			ddface->AddVertex(decVector(ddface->GetVertexAt(0))); // copy required otherwise memory error
			//ddface->CalculateNormal();
			
			pDDShape->AddFace(ddface);
			ddface = NULL;
		}
		
	}catch(const deException &){
		if(ddface){
			delete ddface;
		}
		throw;
	}
	
	pNavSpace->GetDDTypeFaces()->NotifyShapeContentChanged();
}
