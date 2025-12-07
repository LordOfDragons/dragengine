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

#include "dedaiNavSpace.h"
#include "dedaiNavigator.h"
#include "costs/dedaiCostTable.h"
#include "pathfinding/dedaiPathFinderNavGrid.h"
#include "pathfinding/dedaiPathFinderNavMesh.h"
#include "dedaiPathCollisionListener.h"
#include "spaces/grid/dedaiSpaceGrid.h"
#include "spaces/grid/dedaiSpaceGridEdge.h"
#include "spaces/mesh/dedaiSpaceMeshFace.h"
#include "spaces/mesh/dedaiSpaceMesh.h"
#include "spaces/dedaiSpace.h"
#include "layer/dedaiLayer.h"
#include "../deDEAIModule.h"
#include "../world/dedaiWorld.h"
#include "../devmode/dedaiDeveloperMode.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/navigation/navigator/deNavigatorPath.h>
#include <dragengine/resources/navigation/navigator/deNavigatorType.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/world/deWorld.h>



// Class dedaiNavigator
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavigator::dedaiNavigator(deDEAIModule &deai, deNavigator &navigator) :
pDEAI(deai),
pNavigator(navigator),

pParentWorld(NULL),

pTypeMappings(NULL),
pTypeMappingCount(0),
pDirtyTypeMappings(true),

pLayer(NULL),

pDebugDrawer(NULL),
pDDSPath(NULL),
pDDSPathFaces(NULL),
pDDSPathFacesOpen(NULL),
pDDSPathFacesClosed(NULL){
}

dedaiNavigator::~dedaiNavigator(){
	pCleanUp();
}



// Management
///////////////

void dedaiNavigator::SetParentWorld(dedaiWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	if(pDebugDrawer && pParentWorld){
		pParentWorld->GetWorld().RemoveDebugDrawer(pDebugDrawer);
	}
	
	pParentWorld = world;
	
	if(world){
		pLayer = world->GetLayer(pNavigator.GetLayer());
		
		if(pDebugDrawer){
			world->GetWorld().AddDebugDrawer(pDebugDrawer);
		}
		
	}else{
		pLayer = NULL;
	}
	
	UpdateDDSPath();
}



float dedaiNavigator::GetFixCostFor(int costTableEntry) const{
	if(costTableEntry < 0 || costTableEntry >= pTypeMappingCount){
		return pNavigator.GetDefaultFixCost();
		
	}else if(pTypeMappings[costTableEntry]){
		return pTypeMappings[costTableEntry]->GetFixCost();
		
	}else{
		return pNavigator.GetDefaultFixCost();
	}
}

void dedaiNavigator::GetCostParametersFor(int costTableEntry, float &fixCost, float &costPerMeter) const{
	if(costTableEntry < 0 || costTableEntry >= pTypeMappingCount){
		fixCost = pNavigator.GetDefaultFixCost();
		costPerMeter = pNavigator.GetDefaultCostPerMeter();
		
	}else if(pTypeMappings[costTableEntry]){
		fixCost = pTypeMappings[costTableEntry]->GetFixCost();
		costPerMeter = pTypeMappings[costTableEntry]->GetCostPerMeter();
		
	}else{
		fixCost = pNavigator.GetDefaultFixCost();
		costPerMeter = pNavigator.GetDefaultCostPerMeter();
	}
}

void dedaiNavigator::Prepare(){
	if(! pParentWorld){
		return;
	}
	
	if(pDirtyTypeMappings){
		pUpdateTypeMappings();
		pDirtyTypeMappings = false;
	}
}



void dedaiNavigator::UpdateDDSPath(){
	// check if the developer mode is enabled and navigation spaces have to be shown
	const dedaiDeveloperMode &devmode = pDEAI.GetDeveloperMode();
	
	if(devmode.GetEnabled() && (devmode.GetShowPath() || devmode.GetShowPathFaces())){
		// ensure the debug drawer exists
		if(! pDebugDrawer){
			pDebugDrawer = pDEAI.GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer();
			pDebugDrawer->SetXRay(true);
			
			if(pParentWorld){
				pParentWorld->GetWorld().AddDebugDrawer(pDebugDrawer);
			}
		}
		
		// ensure the debug drawer shapes exists or not depending on the show states
		if(devmode.GetShowPath()){
			if(! pDDSPath){
				pDDSPath = new deDebugDrawerShape;
				pDDSPath->SetFillColor(decColor(1.0f, 0.5f, 0.0f, 0.1f));
				pDDSPath->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 0.8f));
				pDebugDrawer->AddShape(pDDSPath);
			}
			
		}else{
			if(pDDSPath){
				pDebugDrawer->RemoveShape(pDDSPath);
				pDDSPath = NULL;
			}
		}
		
		if(devmode.GetShowPathFaces()){
			if(! pDDSPathFaces){
				pDDSPathFaces = new deDebugDrawerShape;
				pDDSPathFaces->SetFillColor(decColor(0.0f, 1.0f, 1.0f, 0.2f));
				pDDSPathFaces->SetEdgeColor(decColor(0.0f, 1.0f, 1.0f, 0.8f));
				pDebugDrawer->AddShape(pDDSPathFaces);
			}
			/*
			if(! pDDSPathFacesOpen){
				pDDSPathFacesOpen = new deDebugDrawerShape;
				pDDSPathFacesOpen->SetFillColor(decColor(0.0f, 0.6f, 0.0f, 0.2f));
				pDDSPathFacesOpen->SetEdgeColor(decColor(0.0f, 0.6f, 0.0f, 0.8f));
				pDebugDrawer->AddShape(pDDSPathFacesOpen);
			}
			
			if(! pDDSPathFacesClosed){
				pDDSPathFacesClosed = new deDebugDrawerShape;
				pDDSPathFacesClosed->SetFillColor(decColor(0.6f, 0.0f, 0.0f, 0.2f));
				pDDSPathFacesClosed->SetEdgeColor(decColor(0.6f, 0.0f, 0.0f, 0.8f));
				pDebugDrawer->AddShape(pDDSPathFacesClosed);
			}
			*/
			
		}else{
			if(pDDSPathFaces){
				pDebugDrawer->RemoveShape(pDDSPathFaces);
				pDDSPathFaces = NULL;
			}
			if(pDDSPathFacesOpen){
				pDebugDrawer->RemoveShape(pDDSPathFacesOpen);
				pDDSPathFacesOpen = NULL;
			}
			if(pDDSPathFacesClosed){
				pDebugDrawer->RemoveShape(pDDSPathFacesClosed);
				pDDSPathFacesClosed = NULL;
			}
		}
		
	}else{
		// if the debug drawer exists remove it
		if(pDebugDrawer){
			if(pParentWorld){
				pParentWorld->GetWorld().RemoveDebugDrawer(pDebugDrawer);
			}
			
			pDDSPath = NULL;
			pDDSPathFaces = NULL;
			pDDSPathFacesOpen = NULL;
			pDDSPathFacesClosed = NULL;
			
			pDebugDrawer->FreeReference();
			pDebugDrawer = NULL;
		}
	}
}

void dedaiNavigator::UpdateDDSPathShape(const deNavigatorPath &path){
	if(! pDDSPath){
		return;
	}
	
	pDDSPath->RemoveAllFaces();
	pDDSPath->GetShapeList().RemoveAll();
	
	const decDVector &refpos = pDebugDrawer->GetPosition();
	const int count = path.GetCount();
	decVector halfExtends(0.02f, 0.02f, 0.02f);
	const decVector upAlt(0.0f, 0.0f, 1.0f);
	const decVector up(0.0f, 1.0f, 0.0f);
	decQuaternion orientation;
	decShape *shape = NULL;
	decVector p1, p2, pd;
	int i;
	
	try{
		if(count > 0){
			shape = new decShapeSphere(0.05f);
			pDDSPath->GetShapeList().Add(shape);
			shape = NULL;
		}
		
		if(count > 1){
			shape = new decShapeSphere(0.05f, (path.GetAt(count - 1) - refpos).ToVector());
			pDDSPath->GetShapeList().Add(shape);
			shape = NULL;
		}
		
		for(i=0; i<count; i++){
			if(i > 0){
				p1 = (path.GetAt(i - 1) - refpos).ToVector();
			}
			p2 = (path.GetAt(i) - refpos).ToVector();
			pd = p2 - p1;
			
			halfExtends.z = pd.Length() * 0.5f;
			
			if(halfExtends.z > FLOAT_SAFE_EPSILON){
				pd.Normalize();
				
				if(fabsf(pd.y) < 0.99f){
					orientation = decMatrix::CreateWorld(p1, pd, up).ToQuaternion();
					
				}else{
					orientation = decMatrix::CreateWorld(p1, pd, upAlt).ToQuaternion();
				}
				
				shape = new decShapeBox(halfExtends, (p1 + p2) * 0.5f, orientation);
				pDDSPath->GetShapeList().Add(shape);
				shape = NULL;
			}
		}
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
}



// Notifications
//////////////////

void dedaiNavigator::LayerChanged(){
	if(! pParentWorld){
		return;
	}
	
	pLayer = pParentWorld->GetLayer(pNavigator.GetLayer());
	
	pDevModeMarkLayerDirty();
}

void dedaiNavigator::SpaceTypeChanged(){
	pDevModeMarkLayerDirty();
}

void dedaiNavigator::CostsChanged(){
	pDevModeMarkLayerDirty();
}

void dedaiNavigator::TypesChanged(){
	pDirtyTypeMappings = true;
	pDevModeMarkLayerDirty();
}

void dedaiNavigator::ParametersChanged(){
}



void dedaiNavigator::FindPath(deNavigatorPath &path, const decDVector &start, const decDVector &goal){
	if(! pParentWorld){
		return;
	}
	
	path.RemoveAll();
	
	pLayer->Prepare();
	Prepare();
	
	if(pNavigator.GetSpaceType() == deNavigationSpace::estGrid){
		dedaiPathFinderNavGrid pathfinder;
		pathfinder.SetWorld(pParentWorld);
		pathfinder.SetNavigator(this);
		pathfinder.SetStartPoint(start);
		pathfinder.SetEndPoint(goal);
		pathfinder.FindPath();
		
		const decDVector * const pfpath = pathfinder.GetPathPoints();
		const int count = pathfinder.GetPathPointCount();
		int i;
		
		for(i=0; i<count; i++){
			path.Add(pfpath[i]);
		}
		
		if(pDDSPath){
			pDebugDrawer->SetPosition(start);
			UpdateDDSPathShape(path);
			pDebugDrawer->NotifyShapeContentChanged();
		}
		
	}else if(pNavigator.GetSpaceType() == deNavigationSpace::estMesh){
		dedaiPathFinderNavMesh pathfinder;
		pathfinder.SetWorld(pParentWorld);
		pathfinder.SetNavigator(this);
		pathfinder.SetStartPoint(start);
		pathfinder.SetEndPoint(goal);
		pathfinder.SetDDSListOpen(pDDSPathFacesOpen);
		pathfinder.SetDDSListClosed(pDDSPathFacesClosed);
		pathfinder.FindPath();
		
		const decDVector * const pfpath = pathfinder.GetPathPoints();
		const int count = pathfinder.GetPathPointCount();
		int i;
		
		for(i=0; i<count; i++){
			path.Add(pfpath[i]);
		}
		
		if(pDDSPath || pDDSPathFaces){
			pDebugDrawer->SetPosition(start);
			if(pDDSPath){
				UpdateDDSPathShape(path);
			}
			if(pDDSPathFaces){
				pathfinder.UpdateDDSShapeFaces(*pDDSPathFaces);
			}
			pDebugDrawer->NotifyShapeContentChanged();
		}
	}
}

void dedaiNavigator::CostTableDefinitionChanged(){
	pDirtyTypeMappings = true;
}



bool dedaiNavigator::NearestPoint(const decDVector &point, float radius,
decDVector &nearestPoint, int &nearestType){
	if(! pParentWorld){
		return false;
	}
	
	pLayer->Prepare();
	Prepare();
	
	switch(pNavigator.GetSpaceType()){
	case deNavigationSpace::estGrid:{
		float nearestLambda;
		dedaiSpaceGridEdge * const edge = pLayer->GetGridNearestPoint(point, radius, nearestPoint, nearestLambda);
		if(edge){
			nearestType = edge->GetGrid()->GetSpace().GetLayer()->GetCostTable().GetTypeAt(
				nearestLambda < 0.5f ? edge->GetTypeNumber1() : edge->GetTypeNumber2());
			return true;
		}
		}break;
		
	case deNavigationSpace::estMesh:{
		dedaiSpaceMeshFace * const face = pLayer->GetNavMeshNearestPoint(point, radius, nearestPoint);
		if(face){
			nearestType = face->GetMesh()->GetSpace().GetLayer()->GetCostTable().GetTypeAt(face->GetTypeNumber());
			return true;
		}
		}break;
		
	case deNavigationSpace::estVolume:
		pDEAI.LogError("TODO: Navigator.NearestPoint: implementation for Volume type");
		break;
	}
	
	return false;
}

bool dedaiNavigator::LineCollide(const decDVector &origin, const decVector &direction, float &distance) {
	if(! pParentWorld){
		return false;
	}
	
	pLayer->Prepare();
	Prepare();
	
	switch(pNavigator.GetSpaceType()){
	case deNavigationSpace::estGrid:
		DETHROW_INFO(deeInvalidAction, "not supported for grid type");
		
	case deNavigationSpace::estMesh:
		return pLayer->NavMeshLineCollide(origin, direction, distance);
		
	case deNavigationSpace::estVolume:
		pDEAI.LogError("TODO: Navigator.LineCollide: implementation for Volume type");
		break;
	}
	
	return false;
}



bool dedaiNavigator::PathCollideRay(const deNavigatorPath &path, deCollider &collider,
int &hitAfterPoint, float &hitDistance){
	const int pointCount = path.GetCount();
	dedaiPathCollisionListener listener;
	int i;
	
	for(i=0; i<pointCount-1; i++){
		const decDVector &from = path.GetAt(i);
		const decDVector &to = path.GetAt(i + i);
		
		collider.RayHits(from, (to - from).ToVector(), &listener);
		
		if(listener.GetHitDistance() < -0.5){
			continue;
		}
		
		hitAfterPoint = i - 1;
		hitDistance = listener.GetHitDistance();
		return true;
	}
	
	return false;
}

bool dedaiNavigator::PathCollideRay(const deNavigatorPath &path, deCollider &collider,
const decDVector &startPosition, int nextPoint, float maxDistance, int &hitAfterPoint,
float &hitDistance){
	if(maxDistance <= 0.0f){
		return false;
	}
	
	const int pointCount = path.GetCount();
	dedaiPathCollisionListener listener;
	float distance = 0.0f;
	int i;
	
	if(nextPoint < 0 || nextPoint > pointCount){
		DETHROW(deeOutOfBoundary);
	}
	
	for(i=nextPoint; i<pointCount; i++){
		const decDVector &from = i > nextPoint ? path.GetAt(i - 1) : startPosition;
		const decDVector &to = path.GetAt(i);
		
		decVector segment(to - from);
		const float segmentLength = segment.Length();
		
		if(segmentLength > 0.001f && distance + segmentLength > maxDistance){
			segment = (to - from).ToVector() * ((maxDistance - distance) / segmentLength);
		}
		
		distance += segmentLength;
		
		collider.RayHits(from, segment, &listener);
		
		if(listener.GetHitDistance() < -0.5){
			if(distance >= maxDistance){
				break;
			}
			continue;
		}
		
		hitAfterPoint = i - 1;
		hitDistance = listener.GetHitDistance();
		return true;
	}
	
	return false;
}

bool dedaiNavigator::PathCollideShape(const deNavigatorPath &path, deCollider &collider,
deCollider &agent, int &hitAfterPoint, float &hitDistance){
	const int pointCount = path.GetCount();
	dedaiPathCollisionListener listener;
	int i;
	
	for(i=0; i<pointCount-1; i++){
		const decDVector &from = path.GetAt(i);
		const decDVector &to = path.GetAt(i + 1);
		
		agent.SetPosition(from);
		collider.ColliderMoveHits(&agent, (to - from).ToVector(), &listener);
		
		if(listener.GetHitDistance() < -0.5){
			continue;
		}
		
		hitAfterPoint = i - 1;
		hitDistance = listener.GetHitDistance();
		return true;
	}
	
	return false;
}

bool dedaiNavigator::PathCollideShape(const deNavigatorPath &path, deCollider &collider,
deCollider &agent, const decDVector &startPosition, int nextPoint, float maxDistance,
int &hitAfterPoint, float &hitDistance){
	if(maxDistance <= 0.0f){
		return false;
	}
	
	const int pointCount = path.GetCount();
	dedaiPathCollisionListener listener;
	float distance = 0.0f;
	int i;
	
	if(nextPoint < 0 || nextPoint > pointCount){
		DETHROW(deeOutOfBoundary);
	}
	
	for(i=nextPoint; i<pointCount; i++){
		const decDVector &from = i > nextPoint ? path.GetAt(i - 1) : startPosition;
		const decDVector &to = path.GetAt(i);
		
		decVector segment(to - from);
		const float segmentLength = segment.Length();
		
		if(segmentLength > 0.001f && distance + segmentLength > maxDistance){
			segment = (to - from).ToVector() * ((maxDistance - distance) / segmentLength);
		}
		
		distance += segmentLength;
		
		collider.ColliderMoveHits(&agent, segment, &listener);
		
		if(listener.GetHitDistance() < -0.5){
			if(distance >= maxDistance){
				break;
			}
			continue;
		}
		
		hitAfterPoint = i - 1;
		hitDistance = listener.GetHitDistance();
		return true;
	}
	
	return false;
}



// Private Functions
//////////////////////

void dedaiNavigator::pCleanUp(){
	SetParentWorld(NULL);
	
	if(pTypeMappings){
		delete [] pTypeMappings;
	}
	if(pDebugDrawer){
		pDebugDrawer->FreeReference();
	}
}

void dedaiNavigator::pUpdateTypeMappings(){
	const int typeCount = pLayer->GetCostTable().GetTypeCount();
	if(typeCount != pTypeMappingCount){
		if(pTypeMappings){
			delete [] pTypeMappings;
			pTypeMappings = NULL;
			pTypeMappingCount = 0;
		}
		
		if(typeCount > 0){
			pTypeMappings = new deNavigatorType*[typeCount];
		}
	}
	
	for(pTypeMappingCount=0; pTypeMappingCount<typeCount; pTypeMappingCount++){
		pTypeMappings[pTypeMappingCount] = pNavigator.GetTypeWith(pLayer->GetCostTable().GetTypeAt(pTypeMappingCount));
	}
}

void dedaiNavigator::pDevModeMarkLayerDirty(){
	if(! pLayer){
		return;
	}
	
	if(pDEAI.GetDeveloperMode().GetEnabled()){
		pLayer->MarkDirty();
	}
}
