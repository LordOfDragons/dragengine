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

#include "debpCollisionDetection.h"
#include "debpSweepCollisionTest.h"
#include "../debpCollisionObject.h"
#include "../debpGhostObject.h"
#include "../debpPhysicsBody.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../collider/debpColliderBone.h"
#include "../collider/debpColliderBones.h"
#include "../world/debpDelayedOperation.h"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/exceptions.h>



// Class debpSweepCollisionTest::cShape
/////////////////////////////////////////

debpSweepCollisionTest::cShape::cShape(btConvexShape *shape, const btTransform &transform)
: pShape(shape), pTransform(transform){
}

debpSweepCollisionTest::cShape::~cShape(){
	if(pShape){
		delete pShape;
	}
}



// Class debpSweepCollisionTest
/////////////////////////////////

// Constructor, destructor
////////////////////////////

debpSweepCollisionTest::debpSweepCollisionTest(debpCollisionDetection &coldet) :
pColDet(coldet){
}

debpSweepCollisionTest::~debpSweepCollisionTest(){
	RemoveAllShapes();
}



// Management
///////////////

void debpSweepCollisionTest::AddShape(decShape &shape, const decVector &scale){
	pScale = scale;
	shape.Visit(*this);
}

void debpSweepCollisionTest::AddShapes(const decShapeList &list, const decVector &scale){
	const int count = list.GetCount();
	int i;
	
	pScale = scale;
	
	for(i=0; i<count; i++){
		list.GetAt(i)->Visit(*this);
	}
}

void debpSweepCollisionTest::RemoveAllShapes(){
	const int count = pShapeList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (cShape*)pShapeList.GetAt(i);
	}
	pShapeList.RemoveAll();
}



void debpSweepCollisionTest::SweepTest(debpCollisionWorld &world, const btTransform &from,
const btTransform &to, debpCollisionWorld::ConvexResultCallback &resultCallback){
	const int count = pShapeList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const cShape &shape = *((cShape*)pShapeList.GetAt(i));
		const btTransform rfrom(from * shape.GetTransform());
		const btTransform rto(to * shape.GetTransform());
		world.safeConvexSweepTest(shape.GetShape(), rfrom, rto, resultCallback); //, 0.001);
	}
}

void debpSweepCollisionTest::SweepTest(debpGhostObject &ghostObject, const btTransform &from,
const btTransform &to, btCollisionWorld::ConvexResultCallback &resultCallback){
	if(!ghostObject.GetGhostObject()){
		return;
	}
	if(!ghostObject.GetDynamicsWorld()){
		DETHROW(deeInvalidParam); // tried to do collision detection on ghost object not in a world
	}
	
	// TODO here we only need to lock the ghost object not the entire world. for the
	//      time being the world is locked to get it working quickly
	debpCollisionWorld &world = *ghostObject.GetDynamicsWorld();
	world.GetDelayedOperation().Lock();
	
	try{
		const btAlignedObjectArray<btCollisionObject*> &colobjs = ghostObject.GetGhostObject()->getOverlappingPairs();
		const int count = pShapeList.GetCount();
		const int colobjCount = colobjs.size();
		int i, j;
		
		for(i=0; i<count; i++){
			const cShape &shape = *((cShape*)pShapeList.GetAt(i));
			const btTransform rfrom(from * shape.GetTransform());
			const btTransform rto(to * shape.GetTransform());
			
			btVector3 castShapeAabbMin;
			btVector3 castShapeAabbMax;
			btVector3 linVel, angVel;
			btTransformUtil::calculateVelocity(rfrom, rto, 1.0, linVel, angVel);
			const btTransform R(rfrom.getBasis());
			
			shape.GetShape()->calculateTemporalAabb(R, linVel, angVel, 1.0, castShapeAabbMin, castShapeAabbMax);
			
			for(j=0; j<colobjCount; j++){
				btCollisionObject * const colobj = colobjs.at(j);
				if(!resultCallback.needsCollision(colobj->getBroadphaseHandle())){
					continue;
				}
				
				btVector3 collisionObjectAabbMin;
				btVector3 collisionObjectAabbMax;
				colobj->getCollisionShape()->getAabb(colobj->getWorldTransform(), collisionObjectAabbMin, collisionObjectAabbMax);
				AabbExpand(collisionObjectAabbMin, collisionObjectAabbMax, castShapeAabbMin, castShapeAabbMax);
				btScalar hitLambda = (btScalar)1.0; //could use resultCallback.m_closestHitFraction, but needs testing
				btVector3 hitNormal;
				
				if(!btRayAabb(rfrom.getOrigin(), rto.getOrigin(), collisionObjectAabbMin, collisionObjectAabbMax, hitLambda, hitNormal)){
					continue;
				}
				
				world.objectQuerySingle(shape.GetShape(), rfrom, rto, colobj, colobj->getCollisionShape(),
					colobj->getWorldTransform(), resultCallback, (btScalar)0.0);
			}
		}
		
		world.GetDelayedOperation().Unlock();
		
	}catch(const deException &){
		world.GetDelayedOperation().Unlock();
		throw;
	}
}

void debpSweepCollisionTest::SweepTest(debpCollider &collider, const btTransform &from,
const btTransform &to, btCollisionWorld::ConvexResultCallback &resultCallback){
	const int count = pShapeList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const cShape &shape = *((cShape*)pShapeList.GetAt(i));
		const btTransform rfrom(from * shape.GetTransform());
		const btTransform rto(to * shape.GetTransform());
		
		btVector3 castShapeAabbMin;
		btVector3 castShapeAabbMax;
		btVector3 linVel, angVel;
		btTransformUtil::calculateVelocity(rfrom, rto, 1.0, linVel, angVel);
		const btTransform R(rfrom.getBasis());
		
		shape.GetShape()->calculateTemporalAabb(R, linVel, angVel, 1.0, castShapeAabbMin, castShapeAabbMax);
		
		const btConvexShape &castShape = *shape.GetShape();
		
		if(collider.IsVolume()){
			btCollisionObject * const co = ((debpColliderVolume&)collider).GetStaticCollisionTest();
			if(co){
				SweepTest(*co, castShapeAabbMin, castShapeAabbMax, rfrom, rto, castShape, resultCallback);
			}
			
		}else if(collider.IsComponent()){
			debpColliderComponent &colliderComponent = (debpColliderComponent&)collider;
			
			switch(colliderComponent.GetTestMode()){
			case debpColliderComponent::etmRigShape:{
				btCollisionObject * const co = colliderComponent.GetStaticCollisionTest();
				if(co){
					SweepTest(*co, castShapeAabbMin, castShapeAabbMax, rfrom, rto, castShape, resultCallback);
				}
				}break;
				
			case debpColliderComponent::etmBoneShape:
				if(colliderComponent.GetBones()){
					const debpColliderBones &bones = *colliderComponent.GetBones();
					const int boneCount = bones.GetBonePhysicsCount();
					int j;
					for(j=0; j<boneCount; j++){
						btCollisionObject * const co = bones.GetBonePhysicsAt(j).GetStaticCollisionTestPrepare();
						if(co){
							SweepTest(*co, castShapeAabbMin, castShapeAabbMax, rfrom, rto, castShape, resultCallback);
						}
					}
				}
				break;
				
			case debpColliderComponent::etmModelDynamic:
			case debpColliderComponent::etmModelStatic:
				// TODO figure out how this can work
				break;
				
			case debpColliderComponent::etmNone:
				break;
			}
		}
	}
}


// Visiting
/////////////

void debpSweepCollisionTest::VisitShape(decShape&){
}

void debpSweepCollisionTest::VisitShapeSphere(decShapeSphere &sphere){
	const decVector2 &axisScaling = sphere.GetAxisScaling();
	const decVector position(sphere.GetPosition().Multiply(pScale));
	const float scaleRadius = (pScale.x + pScale.y + pScale.z) / 3.0f;
	const float radius = sphere.GetRadius() * scaleRadius;
	btSphereShape *sphereShape = NULL;
	btTransform transform;
	cShape *shape = NULL;
	
	// determine if the sphere is an ellipsoid. we can't handle this for the time being
	const bool isEllipsoid = !axisScaling.IsEqualTo(decVector2(1.0f, 1.0f));
	
	if(isEllipsoid){
		return;
	}
	
	// create the shape
	try{
		sphereShape = new btSphereShape(radius);
		
		transform.setOrigin(btVector3((btScalar)position.x, (btScalar)position.y, (btScalar)position.z));
		transform.setRotation(btQuaternion((btScalar)0.0, (btScalar)0.0, (btScalar)0.0, (btScalar)1.0));
		
		shape = new cShape(sphereShape, transform);
		
		pShapeList.Add(shape);
		
	}catch(const deException &){
		if(sphereShape){
			delete sphereShape;
		}
		if(shape){
			delete shape;
		}
		throw;
	}
}

void debpSweepCollisionTest::VisitShapeBox(decShapeBox &box){
	const decQuaternion &orientation = box.GetOrientation();
	const decVector halfExtends(box.GetHalfExtends().Multiply(pScale));
	const decVector2 &tapering = box.GetTapering();
	const decVector position(box.GetPosition().Multiply(pScale));
	float smallestHalfExtends;
	btConvexHullShape *hullShape = NULL;
	btConvexShape *shapeToAdd = NULL;
	btBoxShape *boxShape = NULL;
	float taperedHalfExtendX = 0.0f;
	float taperedHalfExtendZ = 0.0f;
	btTransform transform;
	cShape *shape = NULL;
	
	// determine if the box is tapered
	const bool isTapered = !tapering.IsEqualTo(decVector2(1.0f, 1.0f));
	
	// determine the smallest half extends
	smallestHalfExtends = halfExtends.x;
	if(halfExtends.y < smallestHalfExtends){
		smallestHalfExtends = halfExtends.y;
	}
	if(halfExtends.z < smallestHalfExtends){
		smallestHalfExtends = halfExtends.z;
	}
	
	if(isTapered){
		taperedHalfExtendX = halfExtends.x * tapering.x;
		if(taperedHalfExtendX < smallestHalfExtends){
			smallestHalfExtends = taperedHalfExtendX;
		}
		
		taperedHalfExtendZ = halfExtends.z * tapering.y;
		if(taperedHalfExtendZ < smallestHalfExtends){
			smallestHalfExtends = taperedHalfExtendZ;
		}
	}
	
	// create the shape
	try{
		if(isTapered){
			hullShape = new btConvexHullShape;
			
			hullShape->addPoint(btVector3((btScalar)taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)taperedHalfExtendZ));
			hullShape->addPoint(btVector3((btScalar)-taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)taperedHalfExtendZ));
			hullShape->addPoint(btVector3((btScalar)-taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)-taperedHalfExtendZ));
			hullShape->addPoint(btVector3((btScalar)taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)-taperedHalfExtendZ));
			
			hullShape->addPoint(btVector3((btScalar)halfExtends.x, (btScalar)-halfExtends.y, (btScalar)halfExtends.z));
			hullShape->addPoint(btVector3((btScalar)-halfExtends.x, (btScalar)-halfExtends.y, (btScalar)halfExtends.z));
			hullShape->addPoint(btVector3((btScalar)-halfExtends.x, (btScalar)-halfExtends.y, (btScalar)-halfExtends.z));
			hullShape->addPoint(btVector3((btScalar)halfExtends.x, (btScalar)-halfExtends.y, (btScalar)-halfExtends.z));
			
			shapeToAdd = hullShape;
			
		}else{
			boxShape = new btBoxShape(btVector3((btScalar)halfExtends.x, (btScalar)halfExtends.y, (btScalar)halfExtends.z));
			shapeToAdd = boxShape;
		}
		
		transform.setOrigin(btVector3((btScalar)position.x, (btScalar)position.y, (btScalar)position.z));
		transform.setRotation(btQuaternion((btScalar)orientation.x, (btScalar)orientation.y,
			(btScalar)orientation.z, (btScalar)orientation.w));
		
		shape = new cShape(shapeToAdd, transform);
		
		pShapeList.Add(shape);
		
	}catch(const deException &){
		if(hullShape){
			delete hullShape;
		}
		if(boxShape){
			delete boxShape;
		}
		if(shape){
			delete shape;
		}
		throw;
	}
}

void debpSweepCollisionTest::VisitShapeCylinder(decShapeCylinder &cylinder){
	const decQuaternion &orientation = cylinder.GetOrientation();
	const decVector position(cylinder.GetPosition().Multiply(pScale));
	const float scaleRadius = (pScale.x + pScale.z) / 2.0f;
	const float halfHeight = cylinder.GetHalfHeight() * pScale.y;
	const float topRadius = cylinder.GetTopRadius() * scaleRadius;
	//float bottomRadius = cylinder.GetBottomRadius() * scaleRadius;
	btCylinderShape *cylinderShape = NULL;
	btTransform transform;
	cShape *shape = NULL;
	
	const btVector3 bthe(topRadius, halfHeight, topRadius);
	
	try{
		cylinderShape = new btCylinderShape(bthe);
		
		transform.setOrigin(btVector3((btScalar)position.x, (btScalar)position.y, (btScalar)position.z));
		transform.setRotation(btQuaternion((btScalar)orientation.x, (btScalar)orientation.y,
			(btScalar)orientation.z, (btScalar)orientation.w));
		
		shape = new cShape(cylinderShape, transform);
		pShapeList.Add(shape);
		
	}catch(const deException &){
		if(cylinderShape){
			delete cylinderShape;
		}
		if(shape){
			delete shape;
		}
		throw;
	}
}

void debpSweepCollisionTest::VisitShapeCapsule(decShapeCapsule &capsule){
	const decQuaternion &orientation = capsule.GetOrientation();
	const decVector position(capsule.GetPosition().Multiply(pScale));
	const float scaleRadius = (pScale.x + pScale.z) / 2.0f;
	const float halfHeight = capsule.GetHalfHeight() * pScale.y;
	const float topRadius = capsule.GetTopRadius() * scaleRadius;
	const float bottomRadius = capsule.GetBottomRadius() * scaleRadius;
	//const decVector2 &topAxisScaling = capsule->GetTopAxisScaling();
	//const decVector2 &bottomAxisScaling = capsule->GetBottomAxisScaling();
	btMultiSphereShape *capsuleShape = NULL;
	btTransform transform;
	btVector3 positions[2];
	btScalar radi[2];
	cShape *shape = NULL;
	
	positions[0].setValue(0.0f, halfHeight, 0.0f);
	positions[1].setValue(0.0f, -halfHeight, 0.0f);
	radi[0] = topRadius;
	radi[1] = bottomRadius;
	
	try{
		capsuleShape = new btMultiSphereShape((const btVector3 *)&positions[0], (const btScalar *)&radi[0], 2);
		
		transform.setOrigin(btVector3((btScalar)position.x, (btScalar)position.y, (btScalar)position.z));
		transform.setRotation(btQuaternion((btScalar)orientation.x, (btScalar)orientation.y,
			(btScalar)orientation.z, (btScalar)orientation.w));
		
		shape = new cShape(capsuleShape, transform);
		pShapeList.Add(shape);
		
	}catch(const deException &){
		if(capsuleShape){
			delete capsuleShape;
		}
		if(shape){
			delete shape;
		}
		throw;
	}
}

void debpSweepCollisionTest::VisitShapeHull(decShapeHull &hull){
	// not supported
	VisitShape(hull);
}



// Private functions
//////////////////////

void debpSweepCollisionTest::SweepTest(btCollisionObject &collisionObject,
const btVector3 &castShapeAabbMin, const btVector3 &castShapeAabbMax,
const btTransform &rfrom, const btTransform &rto, const btConvexShape &castShape,
debpCollisionWorld::ConvexResultCallback &resultCallback){
	btCollisionShape * const staticCollisionShape = collisionObject.getCollisionShape();
	if(!staticCollisionShape){
		return;
	}
	
	btVector3 collisionObjectAabbMin;
	btVector3 collisionObjectAabbMax;
	staticCollisionShape->getAabb(collisionObject.getWorldTransform(), collisionObjectAabbMin, collisionObjectAabbMax);
	AabbExpand(collisionObjectAabbMin, collisionObjectAabbMax, castShapeAabbMin, castShapeAabbMax);
	
	btScalar hitLambda = (btScalar)1.0; //could use resultCallback.m_closestHitFraction, but needs testing
	btVector3 hitNormal;
	if(!btRayAabb(rfrom.getOrigin(), rto.getOrigin(), collisionObjectAabbMin, collisionObjectAabbMax, hitLambda, hitNormal)){
		return;
	}
	
	const btCollisionObjectWrapper castWrap(0, staticCollisionShape, &collisionObject, collisionObject.getWorldTransform(), -1, -1);
	// TODO last parameter is allowPenetration. code shows it is used to reject collision like this:
	//      if normal * r >= -allowPenetration then no-collision
	//      maybe this can help to deal with touching collisions at 0-distance causing troubles
	pColDet.GetBulletShapeCollision().ShapeCast(&castShape, rfrom, rto, &castWrap, resultCallback, (btScalar)0.0);
}
