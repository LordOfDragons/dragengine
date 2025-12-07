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

#include "dedaiSpace.h"
#include "grid/dedaiSpaceGrid.h"
#include "mesh/dedaiSpaceMesh.h"
#include "../dedaiNavSpace.h"
#include "../blocker/dedaiNavBlocker.h"
#include "../costs/dedaiCostTable.h"
#include "../heightterrain/dedaiHeightTerrain.h"
#include "../heightterrain/dedaiHeightTerrainSector.h"
#include "../heightterrain/dedaiHeightTerrainNavSpace.h"
#include "../layer/dedaiLayer.h"
#include "../../deDEAIModule.h"
#include "../../devmode/dedaiDeveloperMode.h"
#include "../../world/dedaiWorld.h"
#include "../../utils/dedaiShapeToConvexVolume.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/image/deImage.h>



// Class dedaiSpace
/////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpace::dedaiSpace(deDEAIModule &deai) :
pDEAI(deai),
pParentWorld(NULL),

pOwnerNavSpace(NULL),
pOwnerHTNavSpace(NULL),

pType(deNavigationSpace::estGrid),
pLayerNumber(0),
pLayer(NULL),
pSnapDistance(0.001f),
pSnapAngle(DEG2RAD * 180.0f),
pBlockingPriority(0),

pGrid(NULL),
pMesh(NULL),

pDirtyMatrix(true),
pDirtyExtends(true),
pDirtyLayout(true),
pDirtyLinks(true),

pDirtyBlocking(true),

pDebugDrawer(NULL),
pDDSSpace(NULL),
pDDSCorners(NULL),
pDDSNormals(NULL),
pDDSMismatching(NULL),
pDDSHighlightCostType(NULL),

pDebugLastHighlightCostType(-1){
}

dedaiSpace::~dedaiSpace(){
	pCleanUp();
}



// Management
///////////////

void dedaiSpace::SetParentWorld(dedaiWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	ClearLinks();
	pInvalidateLayerBlocking();
	if(pParentWorld && pDebugDrawer){
		pParentWorld->GetWorld().RemoveDebugDrawer(pDebugDrawer);
	}
	
	pParentWorld = world;
	
	if(world){
		if(pDebugDrawer){
			pParentWorld->GetWorld().AddDebugDrawer(pDebugDrawer);
		}
		pLayer = world->GetLayer(pLayerNumber);
		
	}else{
		pLayer = NULL;
	}
	
	pDirtyLayout = true;
	pDirtyBlocking = true;
	pDirtyLinks = true;
	pInvalidateLayerBlocking();
	
	UpdateDDSSpace();
}



void dedaiSpace::SetOwnerNavSpace(dedaiNavSpace *navSpace){
	if(navSpace == pOwnerNavSpace){
		return;
	}
	
	ClearLinks();
	
	pOwnerNavSpace = navSpace;
	
	const deNavigationSpace &engNavSpace = navSpace->GetNavigationSpace();
	
	pType = engNavSpace.GetType();
	pLayerNumber = engNavSpace.GetLayer();
	pPosition = engNavSpace.GetPosition();
	pOrientation = engNavSpace.GetOrientation();
	pSnapDistance = engNavSpace.GetSnapDistance();
	pSnapAngle = engNavSpace.GetSnapAngle();
	pBlockingPriority = engNavSpace.GetBlockingPriority();
	pBlockerShape = engNavSpace.GetBlockerShapeList();
	
	if(pDebugDrawer){
		pDebugDrawer->SetPosition(pPosition);
		pDebugDrawer->SetOrientation(pOrientation);
	}
	
	pLayer = NULL;
	pDirtyMatrix = true;
	pDirtyExtends = true;
	pDirtyLayout = true;
	pDirtyBlocking = true;
	pDirtyLinks = true;
	
	pUpdateBlockerConvexVolumeList();
}

void dedaiSpace::SetOwnerHTNavSpace(dedaiHeightTerrainNavSpace *htNavSpace){
	if(htNavSpace == pOwnerHTNavSpace){
		return;
	}
	
	ClearLinks();
	
	pOwnerHTNavSpace = htNavSpace;
	
	const deHeightTerrainNavSpace &engHTNavSpace = htNavSpace->GetNavigationSpace();
	
	pType = engHTNavSpace.GetType();
	pLayerNumber = engHTNavSpace.GetLayer();
	//pPosition = engHTNavSpace.GetPosition();
	//pOrientation = engHTNavSpace.GetOrientation();
	pSnapDistance = engHTNavSpace.GetSnapDistance();
	pSnapAngle = engHTNavSpace.GetSnapAngle();
	pBlockingPriority = 0;
	pBlockerShape.RemoveAll();
	
	if(pDebugDrawer){
		pDebugDrawer->SetPosition(pPosition);
		pDebugDrawer->SetOrientation(pOrientation);
	}
	
	pLayer = NULL;
	pDirtyMatrix = true;
	pDirtyExtends = true;
	pDirtyLayout = true;
	pDirtyBlocking = true;
	pDirtyLinks = true;
	
	pUpdateBlockerConvexVolumeList();
}



void dedaiSpace::SetType(deNavigationSpace::eSpaceTypes type){
	pType = type;
}

void dedaiSpace::SetLayerNumber(int layerNumber){
	if(layerNumber == pLayerNumber){
		return;
	}
	
	ClearLinks();
	pInvalidateLayerBlocking();
	
	pLayerNumber = layerNumber;
	
	pDirtyLayout = true;
	pDirtyBlocking = true;
	pDirtyLinks = true;
	
	if(pParentWorld){
		pLayer = pParentWorld->GetLayer(pLayerNumber);
		
	}else{
		pLayer = NULL;
	}
	pInvalidateLayerBlocking();
}

void dedaiSpace::SetPosition(const decDVector &position){
// 	if( position.IsEqualTo( pPosition ) ){
// 		return;
// 	}
	
	pPosition = position;
	
	pDirtyMatrix = true;
	pDirtyExtends = true;
	pDirtyLinks = true;
	
	ClearLinks();
	pInvalidateLayerBlocking();
	
	if(pDebugDrawer){
		pDebugDrawer->SetPosition(position);
	}
}

void dedaiSpace::SetOrientation(const decQuaternion &orientation){
// 	if( orientation.IsEqualTo( pOrientation ) ){
// 		return;
// 	}
	
	pOrientation = orientation;
	
	pDirtyMatrix = true;
	pDirtyExtends = true;
	pDirtyLinks = true;
	
	ClearLinks();
	pInvalidateLayerBlocking();
	
	if(pDebugDrawer){
		pDebugDrawer->SetOrientation(pOrientation);
	}
}

void dedaiSpace::SetSnapDistance(float distance){
	pSnapDistance = decMath::max(distance, 0.0f);
	
	ClearLinks();
	pInvalidateLayerLinks();
}

void dedaiSpace::SetSnapAngle(float angle){
	pSnapAngle = decMath::max(angle, 0.0f);
	
	ClearLinks();
	pInvalidateLayerLinks();
}

void dedaiSpace::SetBlockingPriority(int priority){
	pBlockingPriority = priority;
	
	ClearLinks();
	pInvalidateLayerBlocking();
}

void dedaiSpace::SetBlockerShape(const decShapeList &shape){
	pBlockerShape = shape;
	
	pUpdateBlockerConvexVolumeList();
	pInvalidateLayerBlocking();
}



const decDMatrix &dedaiSpace::GetMatrix(){
	if(pDirtyMatrix){
		pUpdateMatrices();
	}
	return pMatrix;
}

const decDMatrix &dedaiSpace::GetInverseMatrix(){
	if(pDirtyMatrix){
		pUpdateMatrices();
	}
	return pInvMatrix;
}

const decDVector &dedaiSpace::GetMinimumExtends(){
	if(pDirtyExtends){
		pUpdateExtends();
	}
	return pMinExtends;
}

const decDVector &dedaiSpace::GetMaximumExtends(){
	if(pDirtyExtends){
		pUpdateExtends();
	}
	return pMaxExtends;
}



int dedaiSpace::AddTypeMapping(int typeNumber){
	if(!pLayer){
		DETHROW(deeInvalidParam);
	}	
	return pLayer->GetCostTable().GetTypeWith(typeNumber);
}



void dedaiSpace::Prepare(){
	if(!pParentWorld){
		return;
	}
	
	if(pDirtyLayout){
		pUpdateSpace();
		pDirtyLayout = false;
	}
	
	if(pDirtyBlocking){
		pUpdateBlocking();
		pDirtyBlocking = false;
	}
}

void dedaiSpace::PrepareLinks(){
	if(!pParentWorld){
		return;
	}
	if(!pDirtyLinks){
		return;
	}
	
	if(pMesh){
		pMesh->LinkToOtherMeshes();
		
	}else if(pGrid){
		pGrid->LinkToOtherGrids();
	}
	pDirtyLinks = false;
}

void dedaiSpace::ClearLinks(){
	if(pMesh){
		pMesh->RemoveAllLinks();
		
	}else if(pGrid){
		pGrid->RemoveAllLinks();
	}
}



void dedaiSpace::InvalidateBlocking(){
	pDirtyLayout = true; // right now updating blocking needs a full relayout
	pDirtyBlocking = true;
	pDirtyLinks = true;
	ClearLinks();
}



void dedaiSpace::OwnerLayoutChanged(){
	pDirtyLayout = true;
	pDirtyBlocking = true;
	pDirtyLinks = true;
	pDirtyExtends = true;
	
	ClearLinks();
	pInvalidateLayerBlocking();
}

void dedaiSpace::LinksRemoves(){
	pDirtyLinks = true;
}



void dedaiSpace::UpdateDDSSpace(){
	// check if the developer mode is enabled and navigation spaces have to be shown
	const dedaiDeveloperMode &devmode = pDEAI.GetDeveloperMode();
	
	if(devmode.GetEnabled() && devmode.GetShowSpaces()){
		// ensure the debug drawer exists
		if(!pDebugDrawer){
			pDebugDrawer = pDEAI.GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer();
			pDebugDrawer->SetXRay(true);
			
			if(pParentWorld){
				pParentWorld->GetWorld().AddDebugDrawer(pDebugDrawer);
			}
		}
		
		// update the position and orientation of the debug drawer
		pDebugDrawer->SetPosition(pPosition);
		pDebugDrawer->SetOrientation(pOrientation);
		
		// ensure the debug drawer shapes exists
		bool updateShapes = false;
		
		if(!pDDSSpace){
			pDDSSpace = new deDebugDrawerShape;
			pDDSSpace->SetFillColor(decColor(0.0f, 0.5f, 1.0f, 0.1f));
			pDDSSpace->SetEdgeColor(decColor(0.0f, 0.5f, 1.0f, 0.8f));
			pDebugDrawer->AddShape(pDDSSpace);
			updateShapes = true;
		}
		
		if(!pDDSCorners){
			pDDSCorners = new deDebugDrawerShape;
			pDDSCorners->SetFillColor(decColor(0.5f, 0.5f, 1.0f, 0.1f));
			pDDSCorners->SetEdgeColor(decColor(0.5f, 0.5f, 1.0f, 0.8f));
			pDebugDrawer->AddShape(pDDSCorners);
			updateShapes = true;
		}
		
		if(!pDDSNormals){
			pDDSNormals = new deDebugDrawerShape;
			pDDSNormals->SetFillColor(decColor(0.0f, 0.5f, 1.0f, 1.0f));
			pDDSNormals->SetEdgeColor(decColor(0.0f, 0.5f, 1.0f, 1.0f));
			pDebugDrawer->AddShape(pDDSNormals);
			updateShapes = true;
		}
		
		if(!pDDSMismatching){
			pDDSMismatching = new deDebugDrawerShape;
			pDDSMismatching->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.1f));
			pDDSMismatching->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 0.8f));
			pDebugDrawer->AddShape(pDDSMismatching);
			updateShapes = true;
		}
		
		if(!pDDSHighlightCostType){
			pDDSHighlightCostType = new deDebugDrawerShape;
			pDDSHighlightCostType->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.1f));
			pDDSHighlightCostType->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 0.8f));
			pDebugDrawer->AddShape(pDDSHighlightCostType);
			updateShapes = true;
		}
		
		// update the shapes if required
		if(devmode.GetSpaceHighlightCostType() != pDebugLastHighlightCostType){
			updateShapes = true;
			pDebugLastHighlightCostType = devmode.GetSpaceHighlightCostType();
		}
		
		if(updateShapes){
			Prepare();
			UpdateDDSSpaceShape();
		}
		
	}else{
		// if the debug drawer exists remove it
		if(pDebugDrawer){
			if(pParentWorld){
				pParentWorld->GetWorld().RemoveDebugDrawer(pDebugDrawer);
			}
			
			pDDSSpace = NULL;
			pDDSCorners = NULL;
			pDDSNormals = NULL;
			pDDSMismatching = NULL;
			pDDSHighlightCostType = NULL;
			
			pDebugDrawer->FreeReference();
			pDebugDrawer = NULL;
		}
	}
}

void dedaiSpace::UpdateDDSSpaceShape(){
	if(!pDebugDrawer){
		return;
	}
	
	if(pGrid){
		pGrid->UpdateDDSSpaceShape();
	}
	if(pMesh){
		pMesh->UpdateDDSSpaceShape();
	}
	
	pDebugDrawer->NotifyShapeContentChanged();
}



void dedaiSpace::AddBlockerSplitters(decConvexVolumeList &list){
	if(!pParentWorld){
		DETHROW(deeInvalidParam);
	}
	
	const decDVector &minExtend = GetMinimumExtends();
	const decDVector &maxExtend = GetMaximumExtends();
	decConvexVolumeFace *volumeFace = NULL;
	decConvexVolume *volume = NULL;
	int i, j, k;
	
	// add splitter volumes from all overlapping blockers of appropriate priority
	try{
		deNavigationBlocker *engNavBlocker = pParentWorld->GetWorld().GetRootNavigationBlocker();
		while(engNavBlocker){
			if(!engNavBlocker->GetEnabled()){
				engNavBlocker = engNavBlocker->GetLLWorldNext();
				continue;
			}
			
			if(engNavBlocker->GetSpaceType() != pType){
				engNavBlocker = engNavBlocker->GetLLWorldNext();
				continue;
			}
			
			// only blocked by blockers on the same layer
			if(engNavBlocker->GetLayer() != pLayer->GetLayer()){
				engNavBlocker = engNavBlocker->GetLLWorldNext();
				continue;
			}
			
			// only blocked by blockers of equal or larger priority
			if(engNavBlocker->GetBlockingPriority() < pBlockingPriority){
				engNavBlocker = engNavBlocker->GetLLWorldNext();
				continue;
			}
			
			// only blocked by overlapping blockers. this is a performance optimization
			dedaiNavBlocker &blocker = *((dedaiNavBlocker*)engNavBlocker->GetPeerAI());
			
			const decDVector &blockerMinExtend = blocker.GetMinimumExtends();
			const decDVector &blockerMaxExtend = blocker.GetMaximumExtends();
			
			if(blockerMaxExtend < minExtend || blockerMinExtend > maxExtend){
				engNavBlocker = engNavBlocker->GetLLWorldNext();
				continue;
			}
			
			// add blocker to splitter list
			const decConvexVolumeList &bcvlist = blocker.GetConvexVolumeList();
			const int bcvcount = bcvlist.GetVolumeCount();
			
			if(bcvcount > 0){
				const decMatrix matrix(blocker.GetMatrix() * GetInverseMatrix());
				
				for(i=0; i< bcvcount; i++){
					const decConvexVolume &bcv = *bcvlist.GetVolumeAt(i);
					const int vertexCount = bcv.GetVertexCount();
					const int faceCount = bcv.GetFaceCount();
					
					volume = new decConvexVolume;
					for(j=0; j<vertexCount; j++){
						volume->AddVertex(matrix * bcv.GetVertexAt(j));
					}
					for(j=0; j<faceCount; j++){
						const decConvexVolumeFace &bcvface = *bcv.GetFaceAt(j);
						const int cornerCount = bcvface.GetVertexCount();
						
						if(cornerCount >= 3){
							volumeFace = new decConvexVolumeFace;
							for(k=0; k<cornerCount; k++){
								volumeFace->AddVertex(bcvface.GetVertexAt(k));
							}
							
							const decVector &fv1 = volume->GetVertexAt(bcvface.GetVertexAt(0));
							const decVector &fv2 = volume->GetVertexAt(bcvface.GetVertexAt(1));
							const decVector &fv3 = volume->GetVertexAt(bcvface.GetVertexAt(2));
							const decVector edgeVector((fv2 - fv1) % (fv3 - fv2));
							if(!edgeVector.IsZero()){
								volumeFace->SetNormal(edgeVector.Normalized());
								
							}else{
								volumeFace->SetNormal(decVector(0.0f, 1.0f, 0.0f));
							}
							
							volume->AddFace(volumeFace);
							volumeFace = NULL;
						}
					}
					
					list.AddVolume(volume);
					volume = NULL;
				}
			}
			
			engNavBlocker = engNavBlocker->GetLLWorldNext();
		}
		
	}catch(const deException &){
		if(volumeFace){
			delete volumeFace;
		}
		if(volume){
			delete volume;
		}
		throw;
	}
}

void dedaiSpace::AddSpaceBlockerSplitters(decConvexVolumeList &list){
	const decDVector &minExtend = GetMinimumExtends();
	const decDVector &maxExtend = GetMaximumExtends();
	decConvexVolumeFace *volumeFace = NULL;
	decConvexVolume *volume = NULL;
	int i, j, k;
	
	// add splitter volumes from all overlapping navigation space blockers except ourself
	try{
		deNavigationSpace *engNavSpace = pParentWorld->GetWorld().GetRootNavigationSpace();
		while(engNavSpace){
			dedaiNavSpace * const navspace = (dedaiNavSpace*)engNavSpace->GetPeerAI();
			
			// only blocked by a different navigation space than our owner
			if(navspace == pOwnerNavSpace){
				engNavSpace = engNavSpace->GetLLWorldNext();
				continue;
			}
			
			// only blocked by navigation space blockers on the same layer
			if(engNavSpace->GetLayer() != pLayer->GetLayer()){
				engNavSpace = engNavSpace->GetLLWorldNext();
				continue;
			}
			
			// only blocked by navigation space blockers of equal or larger priority
			if(engNavSpace->GetBlockingPriority() < pBlockingPriority){
				engNavSpace = engNavSpace->GetLLWorldNext();
				continue;
			}
			
			// only blocked by overlapping navigation space blockers. this is a performance optimization
			dedaiSpace &space = *navspace->GetSpace();
			const decDVector &blockerMinExtend = space.GetMinimumExtends();
			const decDVector &blockerMaxExtend = space.GetMaximumExtends();
			
			if(blockerMaxExtend < minExtend || blockerMinExtend > maxExtend){
				engNavSpace = engNavSpace->GetLLWorldNext();
				continue;
			}
			
			const decConvexVolumeList &bcvlist = space.GetBlockerConvexVolumeList();
			const int bcvcount = bcvlist.GetVolumeCount();
			
			if(bcvcount > 0){
				const decMatrix matrix(space.GetMatrix() * GetInverseMatrix());
				
				for(i=0; i< bcvcount; i++){
					const decConvexVolume &bcv = *bcvlist.GetVolumeAt(i);
					const int vertexCount = bcv.GetVertexCount();
					const int faceCount = bcv.GetFaceCount();
					
					volume = new decConvexVolume;
					for(j=0; j<vertexCount; j++){
						volume->AddVertex(matrix * bcv.GetVertexAt(j));
					}
					for(j=0; j<faceCount; j++){
						const decConvexVolumeFace &bcvface = *bcv.GetFaceAt(j);
						const int cornerCount = bcvface.GetVertexCount();
						
						if(cornerCount >= 3){
							volumeFace = new decConvexVolumeFace;
							for(k=0; k<cornerCount; k++){
								volumeFace->AddVertex(bcvface.GetVertexAt(k));
							}
							
							const decVector &fv1 = volume->GetVertexAt(bcvface.GetVertexAt(0));
							const decVector &fv2 = volume->GetVertexAt(bcvface.GetVertexAt(1));
							const decVector &fv3 = volume->GetVertexAt(bcvface.GetVertexAt(2));
							const decVector edgeVector((fv2 - fv1) % (fv3 - fv2));
							if(!edgeVector.IsZero()){
								volumeFace->SetNormal(edgeVector.Normalized());
								
							}else{
								volumeFace->SetNormal(decVector(0.0f, 1.0f, 0.0f));
							}
							
							volume->AddFace(volumeFace);
							volumeFace = NULL;
						}
					}
					
					list.AddVolume(volume);
					volume = NULL;
				}
			}
			
			engNavSpace = engNavSpace->GetLLWorldNext();
		}
		
	}catch(const deException &){
		if(volumeFace){
			delete volumeFace;
		}
		if(volume){
			delete volume;
		}
		throw;
	}
}



// Private Functions
//////////////////////

void dedaiSpace::pCleanUp(){
	if(pMesh){
		delete pMesh;
	}
	if(pGrid){
		delete pGrid;
	}
	
	if(pDebugDrawer){
		pDebugDrawer->FreeReference();
	}
}

void dedaiSpace::pUpdateMatrices(){
	pMatrix.SetWorld(pPosition, pOrientation);
	pInvMatrix = pMatrix.Invert();
	pDirtyMatrix = false;
}



void dedaiSpace::pUpdateExtends(){
	if(pOwnerNavSpace){
		pUpdateExtendsNavSpace();
		
	}else if(pOwnerHTNavSpace){
		pUpdateExtendsHTNavSpace();
	}
}

void dedaiSpace::pUpdateExtendsNavSpace(){
	const deNavigationSpace &engNavSpace = pOwnerNavSpace->GetNavigationSpace();
	const int vertexCount = engNavSpace.GetVertexCount();
	
	if(vertexCount == 0){
		pMaxExtends = pMinExtends = pPosition;
		
	}else{
		const decVector * const vertices = engNavSpace.GetVertices();
		const decDMatrix &matrix = GetMatrix();
		int i;
		
		pMinExtends = matrix * decDVector(vertices[0]);
		pMaxExtends = pMinExtends;
		
		for(i=1; i<vertexCount; i++){
			const decDVector vertex = matrix * decDVector(vertices[i]);
			pMinExtends.SetSmallest(vertex);
			pMaxExtends.SetLargest(vertex);
		}
	}
	
	const int volumeCount = pBlockerConvexVolumeList.GetVolumeCount();
	if(volumeCount == 0){
		return;
	}
	
	const decDMatrix &matrix = GetMatrix();
	int i, j, k;
	
	for(i=0; i< volumeCount; i++){
		const decConvexVolume &volume = *pBlockerConvexVolumeList.GetVolumeAt(i);
		const int faceCount = volume.GetFaceCount();
		
		for(j=0; j<faceCount; j++){
			const decConvexVolumeFace &face = *volume.GetFaceAt(j);
			const int faceVertexCount = face.GetVertexCount();
			
			for(k=0; k<faceVertexCount; k++){
				const decDVector position = matrix * decDVector(volume.GetVertexAt(face.GetVertexAt(k)));
				pMinExtends.SetSmallest(position);
				pMaxExtends.SetLargest(position);
			}
		}
	}
}

void dedaiSpace::pUpdateExtendsHTNavSpace(){
	const deHeightTerrainNavSpace &engNavSpace = pOwnerHTNavSpace->GetNavigationSpace();
	const int cornerCount = engNavSpace.GetCornerCount();
	
	if(cornerCount == 0){
		pMaxExtends = pMinExtends = pPosition;
		return;
	}
	
	const dedaiHeightTerrainSector &sector = pOwnerHTNavSpace->GetSector();
	const deHeightTerrain &engHT = sector.GetHeightTerrain().GetHeightTerrain();
	const float * const heights = sector.GetHeights();
	const unsigned int * const corners = engNavSpace.GetCorners();
	const int imageDim = engHT.GetSectorResolution();
	const float posScale = (float)engHT.GetSectorSize() / (float)(imageDim - 1);
	const float posOffset = (float)engHT.GetSectorSize() * 0.5f;
	const decDMatrix &matrix = GetMatrix();
	int i;
	
	const decPoint point1(corners[0] % imageDim, corners[0] / imageDim);
	pMaxExtends = pMinExtends = matrix.Transform(
		posScale * (double)point1.x - posOffset,
		(double)heights[corners[0]],
		posOffset - posScale * (double)point1.y);
	
	for(i=1; i< cornerCount; i++){
		const decPoint point(corners[i] % imageDim, corners[i] / imageDim);
		const decDVector position(matrix.Transform(
			posScale * (double)point.x - posOffset,
			(double)heights[corners[i]],
			posOffset - posScale * (double)point.y));
		pMinExtends.SetSmallest(position);
		pMaxExtends.SetLargest(position);
	}
}



void dedaiSpace::pUpdateSpace(){
	switch(pType){
	case deNavigationSpace::estGrid:
		if(pMesh){
			delete pMesh;
			pMesh = NULL;
		}
		
		if(pGrid){
			pGrid->Clear();
			
		}else{
			pGrid = new dedaiSpaceGrid(*this);
		}
		
		pGrid->InitFromSpace();
		break;
		
	case deNavigationSpace::estMesh:
		if(pGrid){
			delete pGrid;
			pGrid = NULL;
		}
		
		if(pMesh){
			pMesh->Clear();
			
		}else{
			pMesh = new dedaiSpaceMesh(*this);
		}
		
		pMesh->InitFromSpace();
		break;
		
	default:
		break;
	}
	
	if(pDDSSpace || pDDSCorners || pDDSNormals || pDDSMismatching || pDDSHighlightCostType){
		UpdateDDSSpaceShape();
	}
}

void dedaiSpace::pUpdateBlocking(){
	if(pGrid){
		pGrid->UpdateBlocking();
		
	}else if(pMesh){
		pMesh->UpdateBlocking();
		
	}else{
	}
	
	if(pDDSSpace || pDDSCorners || pDDSNormals || pDDSMismatching || pDDSHighlightCostType){
		UpdateDDSSpaceShape();
	}
}

void dedaiSpace::pUpdateBlockerConvexVolumeList(){
	dedaiShapeToConvexVolume visitor;
	visitor.SetList(&pBlockerConvexVolumeList);
	
	pBlockerConvexVolumeList.RemoveAllVolumes();
	
	const int shapeCount = pBlockerShape.GetCount();
	int i;
	
	for(i=0; i<shapeCount; i++){
		pBlockerShape.GetAt(i)->Visit(visitor);
	}
}



void dedaiSpace::pInvalidateLayerBlocking(){
	if(!pLayer){
		return;
	}
	
	if(pBlockerShape.GetCount() == 0){
		pDirtyLayout = true; // a blocker could be located ontop of us
		pDirtyBlocking = true; // a blocker could be located ontop of us
		pLayer->InvalidateLinks(pType, GetMinimumExtends(), GetMaximumExtends());
		
	}else{
		pLayer->InvalidateBlocking(pType, GetMinimumExtends(), GetMaximumExtends());
	}
}

void dedaiSpace::pInvalidateLayerLinks(){
	if(!pLayer){
		return;
	}
	pLayer->InvalidateLinks(pType, GetMinimumExtends(), GetMaximumExtends());
}
