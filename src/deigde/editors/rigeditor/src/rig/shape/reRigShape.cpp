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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigShape.h"
#include "reSelectionShapes.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/exceptions.h>



// Class reRigShape
/////////////////////

// Constructor, destructor
////////////////////////////

reRigShape::reRigShape(deEngine *engine, reRigShape::eShapeTypes shapeType){
	DEASSERT_NOTNULL(engine)
	
	pEngine = engine;
	pRig = nullptr;
	pRigBone = nullptr;
	
	pDebugDrawer = nullptr;
	pDDSShape = nullptr;
	pCollider = nullptr;
	
	pShapeType = shapeType;
	
	pSelected = false;
	pActive = false;
	
	pDirtyShape = true;
	
	try{
		
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertKinematic);
		pCollider->SetUseLocalGravity(true);
		
		decLayerMask layerMask;
		layerMask.SetBit(reRig::eclmShapes);
		
		pCollider->SetCollisionFilter(decCollisionFilter(layerMask));
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		
		pDDSShape = new igdeWDebugDrawerShape;
		pUpdateDDSColor();
		pDDSShape->SetParentDebugDrawer(pDebugDrawer);
		
	}catch(const deException &){
		pRSCleanUp();
		throw;
	}
}

reRigShape::~reRigShape(){
	pRSCleanUp();
}



// Management
///////////////

void reRigShape::SetRig(reRig *rig){
	pSetRig(rig, true);
}

void reRigShape::SetRigBone(reRigBone *rigBone){
	if(rigBone == pRigBone){
		return;
	}
	
	pRigBone = rigBone;
	
	ShowStateChanged();
}

void reRigShape::SetPosition(const decVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	NotifyShapeChanged();
}

void reRigShape::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	NotifyShapeChanged();
}

void reRigShape::SetProperty(const char *property){
	if(pProperty == property){
		return;
	}
	
	pProperty = property;
	
	NotifyShapeChanged();
}

void reRigShape::SetSelected(bool selected){
	if(selected == pSelected){
		return;
	}
	
	pSelected = selected;
	
	pUpdateDDSColor();
}

void reRigShape::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	
	pUpdateDDSColor();
}



void reRigShape::ShowStateChanged(){
	const bool visible = IsVisible();
	
	if(pRig){
		pDebugDrawer->SetXRay(pRig->GetShapeXRay());
	}
	
	pDebugDrawer->SetVisible(visible);
	pCollider->SetEnabled(visible);
}

void reRigShape::Update(){
	pUpdateShapes();
	pRepositionShapes();
}

void reRigShape::NotifyShapeChanged(){
	if(pRig){
		pRig->NotifyAllShapeChanged(this);
	}
	
	InvalidateShape();
	InvalidatePosition();
}

void reRigShape::InvalidateShape(){
	pDirtyShape = true;
}

void reRigShape::InvalidatePosition(){
	pDirtyShape = true;
}



bool reRigShape::IsVisible() const{
	if(pRig){
		if(pRigBone){
			return pRig->GetShowAllBoneShapes() || pRigBone->GetSelected();
		}
		
		return pRig->GetShowRigShapes();
	}
	
	return false;
}



// Private Functions
//////////////////////

void reRigShape::pRSCleanUp(){
	pRigBone = nullptr;
	pSetRig(nullptr, false);
	if(pDDSShape){
		delete pDDSShape;
	}
}



void reRigShape::pRepositionShapes(){
	decDMatrix matrix;
	
	if(pRigBone){
		matrix = pRigBone->GetPoseMatrix();
		
	}else if(pRig){
		matrix = pRig->GetPoseMatrix();
	}
	
	const decDVector position = matrix.GetPosition();
	const decQuaternion orientation = matrix.ToQuaternion();
	
	pDebugDrawer->SetPosition(position);
	pDebugDrawer->SetOrientation(orientation);
	
	pCollider->SetPosition(position);
	pCollider->SetOrientation(orientation);
}

void reRigShape::pUpdateDDSColor(){
	if(pActive){
		pDDSShape->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 1.0f));
		pDDSShape->SetFillColor(decColor(1.0f, 0.5f, 0.0f, 0.1f));
		
	}else if(pSelected){
		pDDSShape->SetEdgeColor(decColor(0.75f, 0.75f, 0.0f, 1.0f));
		pDDSShape->SetFillColor(decColor(0.75f, 0.75f, 0.0f, 0.1f));
		
	}else{
		pDDSShape->SetEdgeColor(decColor(0.5f, 0.5f, 0.0f, 1.0f));
		pDDSShape->SetFillColor(decColor(0.5f, 0.5f, 0.0f, 0.1f));
	}
}

void reRigShape::pUpdateShapes(){
	if(!pDirtyShape){
		return;
	}
	
	// debug drawer shape
	decShape *shape = nullptr;
	pDDSShape->RemoveAllShapes();
	
	try{
		shape = CreateShape();
		pDDSShape->AddShape(shape);
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
	
	// collider
	shape = nullptr;
	decShapeList shapeList;
	
	try{
		shape = CreateShape();
		shapeList.Add(shape);
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
	
	pCollider->SetShapes(shapeList);
	
	// no more dirty
	pDirtyShape = false;
}

void reRigShape::pSetRig(reRig *rig, bool update){
	if(rig == pRig){
		return;
	}
	
	if(pRig){
		pRig->GetEngineWorld()->RemoveCollider(pCollider);
		pRig->GetEngineWorld()->RemoveDebugDrawer(pDebugDrawer);
	}
	
	pRig = rig;
	
	if(rig){
		rig->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
		rig->GetEngineWorld()->AddCollider(pCollider);
	}
	
	if(update){
		ShowStateChanged();
		pUpdateShapes();
	}
}
