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

#include "meObjectShape.h"
#include "../meWorld.h"
#include "../meWorldGuiParameters.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"


 
// Class meObjectShape
////////////////////////

// Constructor, destructor
////////////////////////////

meObjectShape::meObjectShape(igdeEnvironment *environment, const decShape &shape) :
pEnvironment(environment),

pColliderOwner(this)
{
	if(! environment){
		DETHROW(deeInvalidParam);
	}
	
	pWorld = NULL;
	pParentObject = NULL;
	
	pDDSShape = NULL;
	pEngCollider = NULL;
	
	pShape = NULL;
	
	pSelected = false;
	pActive = false;
	
	deEngine &engine = *environment->GetEngineController()->GetEngine();
	
	try{
		pEngCollider = engine.GetColliderManager()->CreateColliderVolume();
		pEngCollider->SetEnabled(true);
		pEngCollider->SetResponseType(deCollider::ertStatic);
		pEngCollider->SetUseLocalGravity(true);
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmObjectShape);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmEditing);
		
		pEngCollider->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
		
		pEnvironment->SetColliderUserPointer(pEngCollider, &pColliderOwner);
		
		// create debug drawer and shapes
		pDebugDrawer = engine.GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		
		pDDSShape = new igdeWDebugDrawerShape;
		pDDSShape->SetVisible(true);
		pDDSShape->SetParentDebugDrawer(pDebugDrawer);
		
		// init the rest
		pShape = shape.Copy();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meObjectShape::~meObjectShape(){
	pCleanUp();
}



// Management
///////////////

void meObjectShape::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->GetEngineWorld()->RemoveCollider(pEngCollider);
		pWorld->GetEngineWorld()->RemoveDebugDrawer(pDebugDrawer);
	}
	
	pWorld = world;
	
	if(world){
		world->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
		world->GetEngineWorld()->AddCollider(pEngCollider);
	}
	
	ShowStateChanged();
}

void meObjectShape::SetParentObject(meObject *parentObject){
	if(parentObject == pParentObject){
		return;
	}
	
	pParentObject = parentObject;
	UpdateShape();
	UpdateDDSColors();
}



void meObjectShape::SetShape(const decShape &shape){
	decShape *newShape = NULL;
	
	try{
		newShape = shape.Copy();
		if(pShape){
			delete pShape;
		}
		
	}catch(const deException &){
		if(newShape){
			delete newShape;
		}
		throw;
	}
	
	pShape = newShape;
	
	UpdateShape();
}



void meObjectShape::SetSelected(bool selected){
	if(selected == pSelected){
		return;
	}
	
	pSelected = selected;
	UpdateDDSColors();
}

void meObjectShape::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	UpdateDDSColors();
}



void meObjectShape::UpdateShape(){
	// scaling is a problem. get first the scaling from the parent object or whatever parent we have.
	// then for the created shape the scaling would have to be applied which is a problem.
	decShape *shape = NULL;
	decShapeList shapeList;
	
	pDDSShape->RemoveAllShapes();
	
	try{
		shape = pShape->Copy();
		shapeList.Add(shape);
		shape = NULL;
		
		shape = pShape->Copy();
		pDDSShape->AddShape(shape);
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
	
	pEngCollider->SetShapes(shapeList);
	
	decQuaternion orientation;
	decDVector position;
	
	if(pParentObject){
		orientation = decQuaternion::CreateFromEuler(pParentObject->GetRotation() * DEG2RAD);
		position = pParentObject->GetPosition();
	}
	
	pDebugDrawer->SetPosition(position);
	pDebugDrawer->SetOrientation(orientation);
	
	pEngCollider->SetPosition(position);
	pEngCollider->SetOrientation(orientation);
}

void meObjectShape::UpdateDDSColors(){
	if(pActive){
		pDDSShape->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 1.0f));
		pDDSShape->SetFillColor(decColor(1.0f, 0.5f, 0.0f, 0.2f));
		
	}else if(pSelected){
		pDDSShape->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 1.0f));
		pDDSShape->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.2f));
		
	}else{
		pDDSShape->SetEdgeColor(decColor(0.0f, 0.25f, 1.0f, 1.0f));
		pDDSShape->SetFillColor(decColor(0.0f, 0.25f, 1.0f, 0.2f));
	}
}



void meObjectShape::ShowStateChanged(){
	if(!pWorld){
		return;
	}
	
	const meWorldGuiParameters &guiParams = pWorld->GetGuiParameters();
	const meWorldGuiParameters::eElementModes elementMode = guiParams.GetElementMode();
	const bool modeObjShape = elementMode == meWorldGuiParameters::eemObjectShape;
	
	pDDSShape->SetVisible(modeObjShape
		|| guiParams.GetShowShapes()
		|| guiParams.GetShowShapesSelected());
}



// Private Functions
//////////////////////

void meObjectShape::pCleanUp(){
	SetWorld(NULL);
	
	if(pEngCollider){
		pEnvironment->SetColliderUserPointer(pEngCollider, NULL);
		pEngCollider->FreeReference();
	}
	
	if(pDDSShape){
		delete pDDSShape;
	}
	if(pDebugDrawer){
		pDebugDrawer->FreeReference();
	}
}
