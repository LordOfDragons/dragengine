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

#include "debpBulletShape.h"
#include "debpGhostObject.h"
#include "world/debpCollisionWorld.h"
#include "world/debpDelayedOperation.h"

#include <dragengine/common/exceptions.h>

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>



// Class debpGhostObject
//////////////////////////

// Constructor, destructor
////////////////////////////

debpGhostObject::debpGhostObject(){
	pDynWorld = NULL;
	pGhostObject = NULL;
	pShape = NULL;
	
	pEnabled = false;
	pDirtyMatrix = true;
}

debpGhostObject::~debpGhostObject(){
	pCleanUp();
}



// bullet objects
///////////////////

void debpGhostObject::SetDynamicsWorld(debpCollisionWorld *dynWorld){
	if(pDynWorld != dynWorld){
		pFreeGhostObject();
		
		pDynWorld = dynWorld;
		
		if(pEnabled){
			pCreateGhostObject();
		}
	}
}

void debpGhostObject::SetShape(debpBulletShape *shape){
	if(shape == pShape){
		return;
	}
	
	if(shape){
		if(pGhostObject){
			pGhostObject->setCollisionShape(shape->GetShape());
		}
		
	}else{
		pFreeGhostObject();
	}
	
	
	pShape = shape;
	
	pCreateGhostObject();
}



void debpGhostObject::SetPosition(const decDVector &position){
	if(!pPosition.IsEqualTo(position)){
		pPosition = position;
		pUpdateTransform();
		pDirtyMatrix = true;
		SetDirtyAABB(true);
	}
}

void debpGhostObject::SetOrientation(const decQuaternion &orientation){
	if(!pOrientation.IsEqualTo(orientation)){
		pOrientation = orientation;
		pUpdateTransform();
		pDirtyMatrix = true;
		SetDirtyAABB(true);
	}
}



void debpGhostObject::SetEnabled(bool enabled){
	if(pEnabled != enabled){
		pFreeGhostObject();
		
		pEnabled = enabled;
		
		if(enabled){
			pCreateGhostObject();
		}
	}
}

const decDMatrix &debpGhostObject::GetMatrix(){
	if(pDirtyMatrix){
		pMatrix.SetWorld(pPosition, pOrientation);
		pDirtyMatrix = false;
	}
	
	return pMatrix;
}



void debpGhostObject::UpdateAABB(){
	if(pGhostObject){
		pDynWorld->updateSingleAabb(pGhostObject);
	}
}



// Private Functions
//////////////////////

void debpGhostObject::pCleanUp(){
	pFreeGhostObject();
	
}

void debpGhostObject::pCreateGhostObject(){
	if(!pGhostObject && pEnabled && pDynWorld && pShape){
		SetDirtyAABB(true);
		
		pGhostObject = new btGhostObject;
		pGhostObject->setUserPointer((debpCollisionObject*)this);
		pGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		pGhostObject->setCollisionShape(pShape->GetShape());
		pGhostObject->forceActivationState(0); // ensure the ghost object is in deactivated state
		
		pUpdateTransform();
		
		// add collision object to dynamics world or add it to the delayed operation if the world is locked
		if(pDynWorld->GetDelayedOperation().IsLocked()){
			pDynWorld->GetDelayedOperation().AddCollisionObject(pGhostObject);
			
		}else{
			// add ghost object to the world. btDiscreteDynamicsWorld overloads addCollisionObject with different
			// default filter parameters in contrary to the base class version which breaks physics. set here
			// explicitly the values used in the base version to get a consistent behavior
			pDynWorld->addCollisionObject(pGhostObject, btBroadphaseProxy::DefaultFilter, btBroadphaseProxy::AllFilter);
		}
	}
}

void debpGhostObject::pFreeGhostObject(){
	if(pGhostObject){
		// destroy the collision object or add it to the delayed operation if the world is locked
		if(pDynWorld->GetDelayedOperation().IsLocked()){
			pDynWorld->GetDelayedOperation().RemoveCollisionObject(pGhostObject);
			
		}else{
			pDynWorld->removeCollisionObject(pGhostObject);
			delete pGhostObject;
		}
		
		pGhostObject = NULL;
	}
}

void debpGhostObject::pUpdateTransform(){
	if(pGhostObject){
		pGhostObject->setWorldTransform(btTransform(
			btQuaternion((btScalar)pOrientation.x, (btScalar)pOrientation.y, (btScalar)pOrientation.z, (btScalar)pOrientation.w),
			btVector3((btScalar)pPosition.x, (btScalar)pPosition.y, (btScalar)pPosition.z)));
	}
}
