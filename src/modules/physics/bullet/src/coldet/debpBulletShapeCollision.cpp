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

#include "debpBulletShapeCollision.h"
#include "collision/debpDCollisionBox.h"
#include "collision/debpDCollisionSphere.h"
#include "../dePhysicsBullet.h"

#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btPolyhedralConvexShape.h"
#include "BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h"
#include "BulletCollision/BroadphaseCollision/btDbvt.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



// Structs
////////////

struct LocalInfoAdder : public btCollisionWorld::ConvexResultCallback{
	ConvexResultCallback *pUserCallback;
	int pI;
	
	LocalInfoAdder(int i, ConvexResultCallback *user) : pUserCallback(user), pI(i){
		m_closestHitFraction = pUserCallback->m_closestHitFraction;
	}
	
	virtual bool needsCollision(btBroadphaseProxy *proxy) const{
		return pUserCallback->needsCollision(proxy);
	}
	
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult &r, bool b){
		btCollisionWorld::LocalShapeInfo shapeInfo;
		
		shapeInfo.m_shapePart = -1;
		shapeInfo.m_triangleIndex = pI;
		if(! r.m_localShapeInfo){
			r.m_localShapeInfo = &shapeInfo;
		}
		
		const btScalar result = pUserCallback->addSingleResult(r, b);
		m_closestHitFraction = pUserCallback->m_closestHitFraction;
		
		return result;
	}
};



// Class debpBulletShapeCollision
///////////////////////////////////

// Constructor, destructor
////////////////////////////

debpBulletShapeCollision::debpBulletShapeCollision(dePhysicsBullet &bullet) :
pBullet(bullet){
}

debpBulletShapeCollision::~debpBulletShapeCollision(){
}



// Management
///////////////

//int timerShapeCastConvex = 0; int timerShapeCastConvexCount = 0;

void debpBulletShapeCollision::ShapeCast(const btConvexShape *castShape, const btTransform &castFromTrans,
const btTransform &castToTrans, const btCollisionObjectWrapper *colObjWrap,
btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration){
	const btCollisionShape * const collisionShape = colObjWrap->getCollisionShape();
	
	if(collisionShape->isCompound()){
		ShapeCastCompound(castShape, castFromTrans, castToTrans, colObjWrap, resultCallback, allowedPenetration);
		
	}else if(collisionShape->isConvex()){
		//decTimer timer;
		//timer.Reset();
		
		#if 0
		ShapeCastConvex(castShape, castFromTrans, castToTrans, colObjWrap, resultCallback, allowedPenetration);
		
		#else
		btCollisionWorld::objectQuerySingleInternal(castShape, castFromTrans, castToTrans,
			colObjWrap, resultCallback, allowedPenetration);
		#endif
		//timerShapeCastConvex += ( int )( timer.GetElapsedTime() * 1e6f ); timerShapeCastConvexCount++;
		
	}else{
		// fall back to bullet collision testing
		btCollisionWorld::objectQuerySingleInternal(castShape, castFromTrans, castToTrans,
			colObjWrap, resultCallback, allowedPenetration);
	}
}



struct BroadphaseSweepTester : public btDbvt::ICollide{
	const btTransform &m_convexFromTrans;
	const btTransform &m_convexToTrans;
	btVector3 m_hitNormal;
	debpBulletShapeCollision &m_bulletShapeCollision;
	btCollisionWorld::ConvexResultCallback &m_resultCallback;
	const btScalar m_allowedCcdPenetration;
	const btConvexShape *m_castShape;
	const btCollisionObjectWrapper *m_colObjWrap;
	const btCompoundShape &m_compoundShape;
	
	BroadphaseSweepTester(const btConvexShape *castShape, const btCollisionObjectWrapper *colObjWrap,
	const btCompoundShape &compoundShape, const btTransform &convexFromTrans, const btTransform &convexToTrans,
	debpBulletShapeCollision &bulletShapeCollision, btCollisionWorld::ConvexResultCallback &resultCallback,
	btScalar allowedPenetration) :
	m_convexFromTrans(convexFromTrans),
	m_convexToTrans(convexToTrans),
	m_bulletShapeCollision(bulletShapeCollision),
	m_resultCallback(resultCallback),
	m_allowedCcdPenetration(allowedPenetration),
	m_castShape(castShape),
	m_colObjWrap(colObjWrap),
	m_compoundShape(compoundShape){
	}
	
	virtual void Process(const btDbvtNode *leaf){
		// early exit if at 0 distance
		if(m_resultCallback.m_closestHitFraction == (btScalar)0.0){
			return;
		}
		
		const int childIndex = leaf->dataAsInt;
		
		const btTransform &colObjWorldTransform = m_colObjWrap->getWorldTransform();
		const btTransform childTransform(colObjWorldTransform * m_compoundShape.getChildTransform(childIndex));
		const btCollisionObjectWrapper childWrap(m_colObjWrap, m_compoundShape.getChildShape(childIndex),
			m_colObjWrap->getCollisionObject(), childTransform, -1, childIndex);
		
		LocalInfoAdder cbAdder(childIndex, &m_resultCallback);
		m_bulletShapeCollision.ShapeCast(m_castShape, m_convexFromTrans,
			m_convexToTrans, &childWrap, cbAdder, m_allowedCcdPenetration);
	}
};

void debpBulletShapeCollision::ShapeCastCompound(const btConvexShape *castShape,
const btTransform &castFromTrans, const btTransform &castToTrans, const btCollisionObjectWrapper *colObjWrap,
btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration){
	// check each child with a castShape call
	const btCompoundShape &compoundShape = *((btCompoundShape*)(colObjWrap->getCollisionShape()));
	const int count = compoundShape.getNumChildShapes();
	
	if(count == 0){
		return;
	}
	
	// if there is a tree use it to accelerate the search. do this only if the compound shape has
	// more than 1 child shape. with 1 shape the compound aabb is the same as the child aabb
	const btDbvt * const tree = compoundShape.getDynamicAabbTree();
	if(tree && count > 1){
		// see btCollisionWorld.cpp:951
		#if 0
		const btTransform &colObjWorldTransform = colObjWrap->getWorldTransform();
		
		btVector3 fromLocalAabbMin, fromLocalAabbMax;
		btVector3 toLocalAabbMin, toLocalAabbMax;
		
		castShape->getAabb(colObjWorldTransform.inverse() * castFromTrans, fromLocalAabbMin, fromLocalAabbMax);
		castShape->getAabb(colObjWorldTransform.inverse() * castToTrans, toLocalAabbMin, toLocalAabbMax);
		
		fromLocalAabbMin.setMin(toLocalAabbMin);
		fromLocalAabbMax.setMax(toLocalAabbMax);
		
		BroadphaseSweepTester callback(castShape, colObjWrap, compoundShape, castFromTrans,
			castToTrans, *this, resultCallback, allowedPenetration);
		
		const ATTRIBUTE_ALIGNED16(btDbvtVolume) bounds(btDbvtVolume::FromMM(fromLocalAabbMin, fromLocalAabbMax));
		tree->collideTV(tree->m_root, bounds, callback);
		
		#else
		const btTransform compoundInverseTrans(colObjWrap->getWorldTransform().inverse());
		const btTransform localCastFromTrans(compoundInverseTrans * castFromTrans);
		const btTransform localCastToTrans(compoundInverseTrans * castToTrans);
		btVector3 castShapeAabbMin;
		btVector3 castShapeAabbMax;
		btVector3 linVel, angVel;
		btTransformUtil::calculateVelocity(localCastFromTrans, localCastToTrans, 1.0, linVel, angVel);
		const btTransform R(localCastFromTrans.getBasis());
		castShape->calculateTemporalAabb(R, linVel, angVel, 1.0, castShapeAabbMin, castShapeAabbMax);
		
		// test against the tree
		const btVector3 unnormalizedRayDir(localCastToTrans.getOrigin() - localCastFromTrans.getOrigin());
		const btVector3 rayDir(unnormalizedRayDir.normalized());
		
		const btVector3 rayDirectionInverse(
			(rayDir[0] == (btScalar)0.0) ? (btScalar)BT_LARGE_FLOAT : ((btScalar)1.0 / rayDir[0]),
			(rayDir[1] == (btScalar)0.0) ? (btScalar)BT_LARGE_FLOAT : ((btScalar)1.0 / rayDir[1]),
			(rayDir[2] == (btScalar)0.0) ? (btScalar)BT_LARGE_FLOAT : ((btScalar)1.0 / rayDir[2]));
		unsigned int signs[3] = {
			rayDirectionInverse[0] < (btScalar)0.0,
			rayDirectionInverse[1] < (btScalar)0.0,
			rayDirectionInverse[2] < (btScalar)0.0};
		const btScalar lambdaMax = rayDir.dot(unnormalizedRayDir);
		
		BroadphaseSweepTester convexCaster(castShape, colObjWrap, compoundShape, castFromTrans,
			castToTrans, *this, resultCallback, allowedPenetration);
		
		tree->rayTestInternal(tree->m_root, localCastFromTrans.getOrigin(), localCastToTrans.getOrigin(),
			rayDirectionInverse, signs, lambdaMax, castShapeAabbMin, castShapeAabbMax, pRayTestStacks, convexCaster);
		#endif
		
	// otherwise iterate over all children and test them
	}else{
		const btTransform &colObjWorldTransform = colObjWrap->getWorldTransform();
		int i;
		
		for(i=0; i<count; i++){
			const btTransform childTransform(colObjWorldTransform * compoundShape.getChildTransform(i));
			const btCollisionObjectWrapper childWrap(colObjWrap, compoundShape.getChildShape(i),
				colObjWrap->getCollisionObject(), childTransform, -1, i);
			LocalInfoAdder cbAdder(i, &resultCallback);
			ShapeCast(castShape, castFromTrans, castToTrans, &childWrap, cbAdder, allowedPenetration);
		}
	}
}



void debpBulletShapeCollision::ShapeCastConvex(const btConvexShape *castShape, const btTransform &castFromTrans,
const btTransform &castToTrans, const btCollisionObjectWrapper *colObjWrap,
btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration){
	// check if the transformation contains rotation. fast versions can only be used without rotation
	if(! HasTransformRotation(castFromTrans,castToTrans)){
		const btCollisionShape * const collisionShape = colObjWrap->getCollisionShape();
		
		if(castShape->getShapeType() == BOX_SHAPE_PROXYTYPE){
			if(collisionShape->getShapeType() == BOX_SHAPE_PROXYTYPE){
				debpDCollisionBox castBox;
				BoxVolumeFromBoxShape(castBox, *((btBoxShape*)castShape), castFromTrans);
				debpDCollisionBox hitBox;
				BoxVolumeFromBoxShape(hitBox, *((btBoxShape*)collisionShape), colObjWrap->getWorldTransform());
				VolumeCastVolume(&castBox, &hitBox, colObjWrap, castFromTrans, castToTrans, resultCallback);
				return;
				
			}else if(collisionShape->getShapeType() == SPHERE_SHAPE_PROXYTYPE){
				debpDCollisionBox castBox;
				BoxVolumeFromBoxShape(castBox, *((btBoxShape*)castShape), castFromTrans);
				debpDCollisionSphere hitSphere;
				SphereVolumeFromSphereShape(hitSphere, *((btSphereShape*)collisionShape), colObjWrap->getWorldTransform());
				VolumeCastVolume(&castBox, &hitSphere, colObjWrap, castFromTrans, castToTrans, resultCallback);
				return;
			}
			
		}else if(castShape->getShapeType() == SPHERE_SHAPE_PROXYTYPE){
			if(collisionShape->getShapeType() == BOX_SHAPE_PROXYTYPE){
				debpDCollisionSphere castSphere;
				SphereVolumeFromSphereShape(castSphere, *((btSphereShape*)castShape), castFromTrans);
				debpDCollisionBox hitBox;
				BoxVolumeFromBoxShape(hitBox, *((btBoxShape*)collisionShape), colObjWrap->getWorldTransform());
				VolumeCastVolume(&castSphere, &hitBox, colObjWrap, castFromTrans, castToTrans, resultCallback);
				return;
				
			}else if(collisionShape->getShapeType() == SPHERE_SHAPE_PROXYTYPE){
				debpDCollisionSphere castSphere;
				SphereVolumeFromSphereShape(castSphere, *((btSphereShape*)castShape), castFromTrans);
				debpDCollisionSphere hitSphere;
				SphereVolumeFromSphereShape(hitSphere, *((btSphereShape*)collisionShape), colObjWrap->getWorldTransform());
				VolumeCastVolume(&castSphere, &hitSphere, colObjWrap, castFromTrans, castToTrans, resultCallback);
				return;
			}
		}
	}
	
	// fall back to bullet collision testing
	btCollisionWorld::objectQuerySingleInternal(castShape, castFromTrans, castToTrans,
		colObjWrap, resultCallback, allowedPenetration);
}



bool debpBulletShapeCollision::IsPointInside(const btCollisionObject &shape,
const btVector3 &position) const{
	return IsPointInside(*shape.getCollisionShape(),
		shape.getWorldTransform().invXform(position));
}

bool debpBulletShapeCollision::IsPointInside(const btCollisionShape &shape,
const btVector3 &position) const{
	switch(shape.getShapeType()){
	case COMPOUND_SHAPE_PROXYTYPE:{
		const btCompoundShape &compound = (const btCompoundShape &)shape;
		const int count = compound.getNumChildShapes();
		int i;
		for(i=0; i<count; i++){
			if(IsPointInside(*compound.getChildShape(i),
			compound.getChildTransform(i).invXform(position))){
				return true;
			}
		}
		}return false;
		
	case BOX_SHAPE_PROXYTYPE:{
		const btBoxShape &box = (const btBoxShape &)shape;
		return btFabs(position.getX()) <= box.getHalfExtentsWithoutMargin().getX()
			&& btFabs(position.getY()) <= box.getHalfExtentsWithoutMargin().getY()
			&& btFabs(position.getZ()) <= box.getHalfExtentsWithoutMargin().getZ();
		}
		
	case SPHERE_SHAPE_PROXYTYPE:{
		const btScalar radius = ((const btSphereShape &)shape).getRadius();
		return position.length2() <= radius * radius;
		}
		
	case CAPSULE_SHAPE_PROXYTYPE:{
		const btCapsuleShape &capsule = (const btCapsuleShape &)shape;
		const btScalar halfHeight = capsule.getHalfHeight();
		const btScalar radius = capsule.getRadius();
		const int upAxis = capsule.getUpAxis();
		
		btVector3 local(BT_ZERO, BT_ZERO, BT_ZERO);
		local[upAxis] = btClamped(position[upAxis], -halfHeight, halfHeight);
		
		return (position - local).length2() <= radius * radius;
		}
		
	case CONE_SHAPE_PROXYTYPE:{
		const btConeShape &cone = (const btConeShape &)shape;
		const btScalar height = cone.getHeight();
		if(height < (btScalar)FLOAT_SAFE_EPSILON){
			return false;
		}
		
		const int upIndex = cone.getConeUpIndex();
		const btScalar linePosition = height * (btScalar)0.5 - position[upIndex];
		if(linePosition < BT_ZERO || linePosition > height){
			return false;
		}
		
		btVector3 local(BT_ZERO, BT_ZERO, BT_ZERO);
		local[upIndex] = position[upIndex];
		
		const btScalar radius = cone.getRadius() * linePosition / height;
		return (position - local).length2() <= radius * radius;
		}
		
	case MULTI_SPHERE_SHAPE_PROXYTYPE:{
		const btMultiSphereShape &msphere = (const btMultiSphereShape &)shape;
		const int count = msphere.getSphereCount();
		
		if(count == 0){
			return false;
			
		}else if(count == 1){
			const btScalar radius = msphere.getSphereRadius(0);
			return (position - msphere.getSpherePosition(0)).length2() <= radius * radius;
			
		}else if(count == 2){
			const btVector3 &center1 = msphere.getSpherePosition(0);
			const btVector3 &center2 = msphere.getSpherePosition(1);
			const btScalar radius1 = msphere.getSphereRadius(0);
			const btScalar radius2 = msphere.getSphereRadius(1);
			const btVector3 line(center2 - center1);
			const btScalar lenLine(line.length());
			
			if(lenLine < (btScalar)FLOAT_SAFE_EPSILON){
				const btScalar lradius(btMax(radius1, radius2));
				return (position - center1).length2() <= lradius * lradius;
			}
			
			const btScalar invLenLine = BT_ONE / lenLine;
			const btVector3 lineNorm(line * invLenLine);
			const btScalar lambda = lineNorm.dot(position - center1) * invLenLine;
			const btScalar radius = radius1 * (BT_ONE - lambda) + radius2 * lambda;
			return (position - (center1 + lineNorm * lambda)).length2() <= radius * radius;
			
		}else{
			// multisphere is currently used only to simulate capsules with different
			// top and bottom radius. all other situations are tricky to calculate
			pBullet.LogWarn("IsPointInside not supported on btMultiSphereShape with more than 2 shperes");
			return false;
		}
		}break;
		
	case CYLINDER_SHAPE_PROXYTYPE:{
		// we use only y-axis orientation cylinders so no upAxis used
		const btCylinderShape &cylinder = (const btCylinderShape &)shape;
		const btScalar halfHeight = cylinder.getHalfExtentsWithoutMargin().getY() * (btScalar)0.5;
		const btScalar radius = cylinder.getHalfExtentsWithoutMargin().getX();
		
		return btFabs(position.getY()) <= halfHeight
		&& (position - btVector3(BT_ZERO, position.getY(), BT_ZERO))
			.length2() <= radius * radius;
		}
		
	case TRIANGLE_SHAPE_PROXYTYPE:
	case CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE:
	case CONVEX_HULL_SHAPE_PROXYTYPE:
		return ((const btPolyhedralConvexShape &)shape).isInside(position, BT_ZERO);
		
	default:
		pBullet.LogWarnFormat("IsPointInside does not support shape type %d", shape.getShapeType());
	}
	
	return false;
}



bool debpBulletShapeCollision::HasTransformRotation(const btTransform &castFromTrans, const btTransform &castToTrans) const{
	// check if there is a rotation present. this is the case if the rotation part of the from and
	// to matrices are identical. correctly the quaternion orientation would have to be compared.
	// this requires though expensive calculations. checking the 3x3 rotation matrix for equality
	// is a lot faster
	const btMatrix3x3 rotDiff(castToTrans.getBasis() - castFromTrans.getBasis());
	return btFabs(rotDiff[0].x()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[0].y()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[0].z()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[1].x()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[1].y()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[1].z()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[2].x()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[2].y()) > (btScalar)FLOAT_SAFE_EPSILON
		|| btFabs(rotDiff[2].z()) > (btScalar)FLOAT_SAFE_EPSILON;
}

void debpBulletShapeCollision::BoxVolumeFromBoxShape(debpDCollisionBox &boxVolume,
btBoxShape &boxShape, const btTransform &shapeTransform) const{
	// this is a bit unfortunate here. debpDCollisionBox.SetOrientation calculates a matrix from the
	// orientation which is used to set the axes for later collision detection. Furthermore
	// shapeTransform.getRotation() calculates a quaternion from the matrix. the quaternion orientation
	// in the debpDCollisionBox is not required for this collision test so it would be left incorrect
	// and just the matrix 3x3 part used to set the axes.
	
#if 0
	struct QuickHack{
		float FILLER1, FILLER2;
		decDVector center;
		decDVector halfSize;
		decQuaternion orientation;
		decDVector axisX;
		decDVector axisY;
		decDVector axisZ;
		bool oriented;
	} *quickHack = (QuickHack*)&boxVolume;
	
	const btVector3 halfExtends = boxShape.getHalfExtentsWithMargin();
	const btVector3 &position = shapeTransform.getOrigin();
	const btMatrix3x3 &rotmat = shapeTransform.getBasis();
	const btVector3 &rotmatX = rotmat[0];
	const btVector3 &rotmatY = rotmat[1];
	const btVector3 &rotmatZ = rotmat[2];
	
	quickHack->center.x = (double)position.x();
	quickHack->center.y = (double)position.y();
	quickHack->center.z = (double)position.z();
	
	quickHack->halfSize.x = (double)halfExtends.x();
	quickHack->halfSize.y = (double)halfExtends.y();
	quickHack->halfSize.z = (double)halfExtends.z();
	
	quickHack->axisX.x = (double)rotmatX.x();
	quickHack->axisX.y = (double)rotmatY.x();
	quickHack->axisX.z = (double)rotmatZ.x();
	
	quickHack->axisY.x = (double)rotmatX.y();
	quickHack->axisY.y = (double)rotmatY.y();
	quickHack->axisY.z = (double)rotmatZ.y();
	
	quickHack->axisZ.x = (double)rotmatX.z();
	quickHack->axisZ.y = (double)rotmatY.z();
	quickHack->axisZ.z = (double)rotmatZ.z();
	
	quickHack->oriented = true;
	
#else
	const btVector3 halfExtends = boxShape.getHalfExtentsWithMargin();
	const btQuaternion orientation = shapeTransform.getRotation();
	const btVector3 &position = shapeTransform.getOrigin();
	
	boxVolume.SetCenter(decDVector((double)position.x(), (double)position.y(), (double)position.z()));
	boxVolume.SetHalfSize(decDVector((double)halfExtends.x(), (double)halfExtends.y(), (double)halfExtends.z()));
	boxVolume.SetOrientation(decQuaternion((float)orientation.x(), (float)orientation.y(),
		(float)orientation.z(), (float)orientation.w()));
#endif
}

void debpBulletShapeCollision::SphereVolumeFromSphereShape(debpDCollisionSphere &sphereVolume,
btSphereShape &sphereShape, const btTransform &shapeTransform) const{
	const btVector3 &position = shapeTransform.getOrigin();
	
	sphereVolume.SetCenter(decDVector((double)position.x(), (double)position.y(), (double)position.z()));
	sphereVolume.SetRadius((double)sphereShape.getRadius());
}



void debpBulletShapeCollision::VolumeCastVolume(debpDCollisionVolume *castVolume, debpDCollisionVolume *hitVolume,
const btCollisionObjectWrapper *colObjWrap, const btTransform &castFromTrans, const btTransform &castToTrans,
btCollisionWorld::ConvexResultCallback &resultCallback) const{
	// determine the displacement
	const btVector3 btdisplacement = castToTrans.getOrigin() - castFromTrans.getOrigin();
	const decDVector displacement((double)btdisplacement.x(), (double)btdisplacement.y(), (double)btdisplacement.z());
	
	// test for collision
	decDVector normal;
	const double distance = castVolume->VolumeMoveHitsVolume(hitVolume, displacement, &normal);
	
	// if a collision has been found send it to the callback
	if(distance < 1.0){
		const btScalar btdistance = (btScalar)distance;
		
		if(btdistance < resultCallback.m_closestHitFraction){
			// hitpoint is not used by the game engine in callbacks. can this be called by anybody requiring this?
			const btVector3 btnormal((btScalar)normal.x, (btScalar)normal.y, (btScalar)normal.z);
			const btVector3 hitpoint = castFromTrans.getOrigin() + btdisplacement * btdistance;
			
			btCollisionWorld::LocalConvexResult localConvexResult(colObjWrap->getCollisionObject(),
				colObjWrap->getCollisionShape(), 0, btnormal, hitpoint, btdistance);
			resultCallback.addSingleResult(localConvexResult, true);
		}
	}
}
