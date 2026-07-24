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

#include "debpCreateBulletShape.h"
#include "../debpBulletShape.h"
#include "../debpBulletCompoundShape.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeSphere.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btCollisionMargin.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletCollision/CollisionShapes/btConeShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>



// #define DEBUGGING 1

// Class debpCreateBulletShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

debpCreateBulletShape::debpCreateBulletShape() :


pCcdThreshold(0.001f),
pCcdRadius(0.001f),
pScale(1.0f, 1.0f, 1.0f),
pNoMargin(true /*false*/),
pHasScale(false),
pShapeIndex(-1){
}

debpCreateBulletShape::~debpCreateBulletShape(){
	Reset();
}



// Management
///////////////

void debpCreateBulletShape::SetOffset(const decVector &offset){
	pOffset = offset;
}

void debpCreateBulletShape::SetScale(const decVector &scale){
	pScale = scale;
	pHasScale = !scale.IsEqualTo(decVector(1.0f, 1.0f, 1.0f));
}

void debpCreateBulletShape::SetNoMargin(bool noMargin){
	pNoMargin = noMargin;
}

void debpCreateBulletShape::Reset(){
	pBulletCompoundShape = nullptr;
	pBulletShape = nullptr;
	
	pOffset.SetZero();
	pCcdThreshold = 0.001f;
	pCcdRadius = 0.001f;
}

void debpCreateBulletShape::Finish(){
	if(pBulletCompoundShape && pHasScale){
		pBulletCompoundShape->GetShape()->setLocalScaling(btVector3(
			(btScalar)pScale.x, (btScalar)pScale.y, (btScalar)pScale.z));
		// setLocalScaling has to come last or scaling does not propagate
	}
}

void debpCreateBulletShape::SetShapeIndex(int index){
	pShapeIndex = index;
}

debpBulletShape::Ref debpCreateBulletShape::GetBulletShape() const{
	if(pBulletCompoundShape){
		return pBulletCompoundShape;
		
	}else{
		return pBulletShape;
	}
}

void debpCreateBulletShape::DebugPrintShape(dePhysicsBullet &bullet, const char *prefix) const{
	const debpBulletShape::Ref shape(GetBulletShape());
	if(shape){
		pDebugPrintShape(bullet, *shape->GetShape(), prefix);
		
	}else{
		bullet.LogInfoFormat("%s(null)", prefix);
	}
}



// Visiting
/////////////

void debpCreateBulletShape::VisitShape(decShape&){
}

void debpCreateBulletShape::VisitShapeSphere(decShapeSphere &sphere){
	const decVector position = sphere.GetPosition() + pOffset;
	const decQuaternion &orientation = sphere.GetOrientation();
	const decVector2 &axisScaling = sphere.GetAxisScaling();
	const float radius = sphere.GetRadius();
	const bool hasNoShape = pBulletShape.IsNull();
	debpBulletShape::Ref bulletShapeSphere;
	debpBulletCompoundShape::Ref bulletShapeCompound;
	debpBulletShape *shapeToAdd = NULL;
	float ccdRadius = 0.0f;
	
#ifdef DEBUGGING
printf("debpCreateBulletShape.VisitShapeSphere: r=%g as=(%g,%g) pos=(%g,%g,%g)\n",
	radius, axisScaling.x, axisScaling.y, position.x, position.y, position.z);
#endif
	
	// create the shape
	bool needsTransform = false;
	btTransform transform;
	transform.setIdentity();
	
	if(!orientation.IsEqualTo(decQuaternion()) || !position.IsZero()){
		transform.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
		transform.setOrigin(btVector3(position.x, position.y, position.z));
		needsTransform = true;
	}
	
	void * const userPointer = (void*)(intptr_t)(pShapeIndex + 1);
	ccdRadius = radius * 0.5f;
	
	if(axisScaling.IsEqualTo(decVector2(1.0f, 1.0f))){
		auto sphereShape = new btSphereShape(radius);
		if(pNoMargin){
			sphereShape->setMargin(BT_ZERO);
		}
		sphereShape->setUserPointer(userPointer);
		bulletShapeSphere = debpBulletShape::Ref::New(sphereShape);
		
	}else{
		ccdRadius = decMath::min(ccdRadius, radius * axisScaling.x * 0.5f, radius * axisScaling.y * 0.5f);
		
		const btVector3 positions[1]{{0.0f, 0.0f, 0.0f}};
		auto multiSphereShape = new btMultiSphereShape(positions, &radius, 1);
		if(pNoMargin){
			multiSphereShape->setMargin(BT_ZERO);
		}
		multiSphereShape->setUserPointer(userPointer);
		multiSphereShape->setLocalScaling({(btScalar)axisScaling.x, BT_ONE, (btScalar)axisScaling.y});
		bulletShapeSphere = debpBulletShape::Ref::New(multiSphereShape);
	}
	
	shapeToAdd = bulletShapeSphere;
	
	if(needsTransform){
		pAddTransformedCollisionShape(shapeToAdd, transform);
		
	}else{
		pAddCollisionShape(shapeToAdd);
	}
	
	if(hasNoShape || ccdRadius < pCcdRadius){
		pCcdRadius = ccdRadius;
	}
	if(hasNoShape || ccdRadius * 0.5f < pCcdThreshold){
		pCcdThreshold = ccdRadius * 0.5f;
	}
}

void debpCreateBulletShape::VisitShapeBox(decShapeBox &box){
	const decVector position = box.GetPosition() + pOffset;
	const decQuaternion &orientation = box.GetOrientation();
	const decVector &halfExtends = box.GetHalfExtends();
	const decVector2 &tapering = box.GetTapering();
	float ccdRadius = 0.0f;
	bool hasNoShape = pBulletShape.IsNull();
	bool needsTransform = false;
	btConvexHullShape *hullShape = NULL;
	debpBulletShape::Ref bulletShapeHull;
	debpBulletShape::Ref bulletShapeBox;
	debpBulletShape *shapeToAdd = NULL;
	
	// determine if the box is tapered
	const bool isTapered = !tapering.IsEqualTo(decVector2(1.0f, 1.0f));
	
	// determine the smallest half extends
	float smallestHalfExtends = decMath::min(halfExtends.x, decMath::min(halfExtends.y, halfExtends.z));
	
	float taperedHalfExtendX = 0.0f;
	float taperedHalfExtendZ = 0.0f;
	if(isTapered){
		taperedHalfExtendX = halfExtends.x * tapering.x;
		taperedHalfExtendZ = halfExtends.z * tapering.y;
		smallestHalfExtends = decMath::min(smallestHalfExtends, taperedHalfExtendX, taperedHalfExtendZ);
	}
	
	// the margin is subtracted from the object producing a small gap. there is nothing wrong
	// with this unless the margin is larger than the half extends of the box. in this case
	// the margin has to be reduced to avoid nasty problems
	btScalar margin = CONVEX_DISTANCE_MARGIN;
	btScalar minSafeSize = (btScalar)0.01;
	
	if((btScalar)smallestHalfExtends - margin < minSafeSize){
		margin = (btScalar)smallestHalfExtends - minSafeSize;
		if(margin < BT_ZERO){
			margin = BT_ZERO;
		}
	}
	
	// create the shape
	btTransform transform;
	transform.setIdentity();
	
	if(!orientation.IsEqualTo(decQuaternion()) || !position.IsZero()){
		transform.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
		transform.setOrigin(btVector3(position.x, position.y, position.z));
		needsTransform = true;
	}
	
	if(isTapered){
#ifdef DEBUGGING
printf("debpCreateBulletShape.VisitShapeBox: hull\n");
#endif
		hullShape = new btConvexHullShape;
		
		hullShape->addPoint(btVector3((btScalar)taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)taperedHalfExtendZ));
		hullShape->addPoint(btVector3((btScalar)-taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)taperedHalfExtendZ));
		hullShape->addPoint(btVector3((btScalar)-taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)-taperedHalfExtendZ));
		hullShape->addPoint(btVector3((btScalar)taperedHalfExtendX, (btScalar)halfExtends.y, (btScalar)-taperedHalfExtendZ));
		
		hullShape->addPoint(btVector3((btScalar)halfExtends.x, (btScalar)-halfExtends.y, (btScalar)halfExtends.z));
		hullShape->addPoint(btVector3((btScalar)-halfExtends.x, (btScalar)-halfExtends.y, (btScalar)halfExtends.z));
		hullShape->addPoint(btVector3((btScalar)-halfExtends.x, (btScalar)-halfExtends.y, (btScalar)-halfExtends.z));
		hullShape->addPoint(btVector3((btScalar)halfExtends.x, (btScalar)-halfExtends.y, (btScalar)-halfExtends.z));
		
		hullShape->setUserPointer((void*)(intptr_t)(pShapeIndex + 1));
		
		bulletShapeHull = debpBulletShape::Ref::New(hullShape);
		shapeToAdd = bulletShapeHull;
		margin = BT_ZERO;
		
	}else{
		auto boxShape = new btBoxShape(btVector3((btScalar)halfExtends.x, (btScalar)halfExtends.y, (btScalar)halfExtends.z));
		boxShape->setUserPointer((void*)(intptr_t)(pShapeIndex + 1));
		
		bulletShapeBox = debpBulletShape::Ref::New(boxShape);
		shapeToAdd = bulletShapeBox;
#ifdef DEBUGGING
printf("debpCreateBulletShape.VisitShapeBox: he=(%g,%g,%g)\n", boxShape->getHalfExtentsWithoutMargin().getX(),
boxShape->getHalfExtentsWithoutMargin().getY(), boxShape->getHalfExtentsWithoutMargin().getZ());
#endif
	}
	
	if(pNoMargin){
		margin = BT_ZERO;
	}
	shapeToAdd->GetShape()->setMargin(margin);
	
	if(needsTransform){
		pAddTransformedCollisionShape(shapeToAdd, transform);
		
	}else{
		pAddCollisionShape(shapeToAdd);
	}
	
	//smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection ( does it really improve anymore? )
	ccdRadius = smallestHalfExtends * 0.5f;
	
	if(hasNoShape || ccdRadius < pCcdRadius){
		pCcdRadius = ccdRadius;
	}
	if(hasNoShape || smallestHalfExtends < pCcdThreshold){
		pCcdThreshold = smallestHalfExtends;
	}
}

void debpCreateBulletShape::VisitShapeCylinder(decShapeCylinder &cylinder){
	const decVector position = cylinder.GetPosition() + pOffset;
	const decQuaternion &orientation = cylinder.GetOrientation();
	const float halfHeight = cylinder.GetHalfHeight();
	const float topRadius = cylinder.GetTopRadius();
	const float bottomRadius = cylinder.GetBottomRadius();
	const decVector2 &topAxisScaling = cylinder.GetTopAxisScaling();
	const decVector2 &bottomAxisScaling = cylinder.GetBottomAxisScaling();
	const bool hasNoShape = pBulletShape.IsNull();
	debpBulletShape::Ref bulletShapeCylinder;
	bool needsTransform = false;
	float ccdRadius = halfHeight + topRadius + bottomRadius;
	
	const bool hasTopScaling = !topAxisScaling.IsEqualTo({1.0f, 1.0f});
	const bool hasBottomScaling = !bottomAxisScaling.IsEqualTo({1.0f, 1.0f});
	void * const userPointer = (void*)(intptr_t)(pShapeIndex + 1);
	
	if(hasTopScaling || hasBottomScaling){
		// create convex hull approximation for scaled cylinders
		// with 16 points per circle and 2 circles this results in 32 hull points
		auto hullShape = new btConvexHullShape();
		
		const int pointsPerCircle = 16;
		
		const decVector2 rt(topAxisScaling * topRadius);
		const decVector2 rb(bottomAxisScaling * bottomRadius);
		
		// add points for top circle
		for(int i=0; i<pointsPerCircle; i++){
			const float angle = (TWO_PI * (float)i) / (float)pointsPerCircle;
			const float c = cosf(angle);
			const float s = sinf(angle);
			
			hullShape->addPoint({rt.x * c, halfHeight, rt.y * s});
		}
		
		// add points for bottom circle
		for(int i=0; i<pointsPerCircle; i++){
			const float angle = (TWO_PI * (float)i) / (float)pointsPerCircle;
			const float c = cosf(angle);
			const float s = sinf(angle);
			
			hullShape->addPoint({rb.x * c, -halfHeight, rb.y * s});
		}
		
		// finish the shape
		hullShape->recalcLocalAabb();
		
		if(pNoMargin){
			hullShape->setMargin(BT_ZERO);
		}
		hullShape->setUserPointer(userPointer);
		
		bulletShapeCylinder = debpBulletShape::Ref::New(hullShape);
		
		// update CCD radius
		ccdRadius = decMath::min(ccdRadius, topRadius * topAxisScaling.x * 0.5f,
			topRadius * topAxisScaling.y * 0.5f);
		ccdRadius = decMath::min(ccdRadius, bottomRadius * bottomAxisScaling.x * 0.5f,
			bottomRadius * bottomAxisScaling.y * 0.5f);
		
	}else if(fabsf(topRadius - bottomRadius) > 0.001f){
		const btVector3 positions[2]{{0.0f, halfHeight, 0.0f}, {0.0f, -halfHeight, 0.0f}};
		const btScalar radi[2]{topRadius, bottomRadius};
		
		auto cylinderShape = new btMultiSphereShape(positions, &radi[0], 2);
		if(pNoMargin){
			cylinderShape->setMargin(BT_ZERO);
		}
		cylinderShape->setUserPointer(userPointer);
		
		bulletShapeCylinder = debpBulletShape::Ref::New(cylinderShape);
		
	}else{
		auto cylinderShape = new btCylinderShape(btVector3(topRadius, halfHeight, topRadius));
		if(pNoMargin){
			cylinderShape->setMargin(BT_ZERO);
		}
		cylinderShape->setUserPointer(userPointer);
		
		bulletShapeCylinder = debpBulletShape::Ref::New(cylinderShape);
	}
	
	if(!orientation.IsEqualTo(decQuaternion())) needsTransform = true;
	if(!position.IsZero()) needsTransform = true;
	
#ifdef DEBUGGING
printf("debpCreateBulletShape.VisitShapeCylinder: tr=%g br=%g he=%g\n", topRadius, bottomRadius, halfHeight);
#endif
	if(needsTransform){
		const btTransform transform(
			btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
			btVector3(position.x, position.y, position.z));
		pAddTransformedCollisionShape(bulletShapeCylinder, transform);
		
	}else{
		pAddCollisionShape(bulletShapeCylinder);
	}
	
	float smallestHalfExtends = decMath::min(halfHeight, topRadius, bottomRadius);
	smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection (does it really improve anymore?)
	
	if(hasNoShape || ccdRadius < pCcdRadius){
		pCcdRadius = ccdRadius;
	}
	if(hasNoShape || smallestHalfExtends < pCcdThreshold){
		pCcdThreshold = smallestHalfExtends;
	}
}

void debpCreateBulletShape::VisitShapeCapsule(decShapeCapsule &capsule){
	const decVector position = capsule.GetPosition() + pOffset;
	const decQuaternion &orientation = capsule.GetOrientation();
	const float halfHeight = capsule.GetHalfHeight();
	const float topRadius = capsule.GetTopRadius();
	const float bottomRadius = capsule.GetBottomRadius();
	const decVector2 &topAxisScaling = capsule.GetTopAxisScaling();
	const decVector2 &bottomAxisScaling = capsule.GetBottomAxisScaling();
	const bool hasNoShape = pBulletShape.IsNull();
	debpBulletShape::Ref bulletShapeCapsule;
	bool needsTransform = false;
	float ccdRadius = halfHeight + topRadius + bottomRadius;
	
	const bool hasTopScaling = !topAxisScaling.IsEqualTo({1.0f, 1.0f});
	const bool hasBottomScaling = !bottomAxisScaling.IsEqualTo({1.0f, 1.0f});
	void * const userPointer = (void*)(intptr_t)(pShapeIndex + 1);
	
	if(hasTopScaling || hasBottomScaling){
		// create convex hull approximation.
		// with 12 segments and 8 points per segment this results in 98 hull points:
		// - 2 pole points
		// - 12 segments * 8 points per segment = 96 points
		// with 12 segments and 16 points per segment this results in 194 hull points:
		// - 2 pole points
		// - 12 segments * 16 points per segment = 192 points
		auto hullShape = new btConvexHullShape();
		
		const int segments = 12;
		const int pointsPerSegment = 16; //8
		
		for(int i=0; i<segments/2; i++){
			const float angle = (HALF_PI * (float)i) / (float)segments;
			const float c = cosf(angle);
			const float s = sinf(angle);
			
			const float yt = halfHeight + topRadius * s;
			const float yb = -halfHeight - bottomRadius * s;
			
			const decVector2 rt(topAxisScaling * (c * topRadius));
			const decVector2 rb(bottomAxisScaling * (c * bottomRadius));
			
			for(int j=0; j<pointsPerSegment; j++){
				const float angle2 = (TWO_PI * (float)j) / (float)pointsPerSegment;
				const float c2 = cosf(angle2);
				const float s2 = sinf(angle2);
				
				hullShape->addPoint({rt.x * c2, yt, rt.y * s2});
				hullShape->addPoint({rb.x * c2, yb, rb.y * s2});
			}
		}
		
		hullShape->addPoint({BT_ZERO, halfHeight + topRadius, BT_ZERO});
		hullShape->addPoint({BT_ZERO, -halfHeight - bottomRadius, BT_ZERO});
		
		// finish the shape
		hullShape->recalcLocalAabb();
		
		if(pNoMargin){
			hullShape->setMargin(BT_ZERO);
		}
		hullShape->setUserPointer(userPointer);
		
		bulletShapeCapsule = debpBulletShape::Ref::New(hullShape);
		
		// update CCD radius
		ccdRadius = decMath::min(ccdRadius, topRadius * topAxisScaling.x * 0.5f,
			topRadius * topAxisScaling.y * 0.5f);
		ccdRadius = decMath::min(ccdRadius, bottomRadius * bottomAxisScaling.x * 0.5f,
			bottomRadius * bottomAxisScaling.y * 0.5f);
		
	}else if(fabsf(topRadius - bottomRadius) > 0.001f){
		const btVector3 positions[2]{{0.0f, halfHeight, 0.0f}, {0.0f, -halfHeight, 0.0f}};
		const btScalar radi[2]{topRadius, bottomRadius};
		
		auto capsuleShape = new btMultiSphereShape(positions, &radi[0], 2);
		if(pNoMargin){
			capsuleShape->setMargin(BT_ZERO);
		}
		capsuleShape->setUserPointer(userPointer);
		
		bulletShapeCapsule = debpBulletShape::Ref::New(capsuleShape);
		
	}else{
		auto capsuleShape = new btCapsuleShape(topRadius, halfHeight * 2.0f);
		if(pNoMargin){
			capsuleShape->setMargin(BT_ZERO);
		}
		capsuleShape->setUserPointer(userPointer);
		
		bulletShapeCapsule = debpBulletShape::Ref::New(capsuleShape);
	}
	
	if(!orientation.IsEqualTo(decQuaternion())) needsTransform = true;
	if(!position.IsZero()) needsTransform = true;
	
#ifdef DEBUGGING
printf("debpCreateBulletShape.VisitShapeCapsule: n=%i s1=(%g,%g,%g) s1=(%g,%g,%g) r1=%g r2=%g\n", capsuleShape->getSphereCount(),
capsuleShape->getSpherePosition(0).getX(), capsuleShape->getSpherePosition(0).getY(), capsuleShape->getSpherePosition(0).getZ(),
capsuleShape->getSpherePosition(1).getX(), capsuleShape->getSpherePosition(1).getY(), capsuleShape->getSpherePosition(1).getZ(),
capsuleShape->getSphereRadius(0), capsuleShape->getSphereRadius(0));
#endif
	if(needsTransform){
		const btTransform transform(
			btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
			btVector3(position.x, position.y, position.z));
		pAddTransformedCollisionShape(bulletShapeCapsule, transform);
		
	}else{
		pAddCollisionShape(bulletShapeCapsule);
	}
	
	float smallestHalfExtends = decMath::min(halfHeight, topRadius, bottomRadius);
	smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection (does it really improve anymore?)
	
	if(hasNoShape || ccdRadius < pCcdRadius){
		pCcdRadius = ccdRadius;
	}
	if(hasNoShape || smallestHalfExtends < pCcdThreshold){
		pCcdThreshold = smallestHalfExtends;
	}
}

void debpCreateBulletShape::VisitShapeHull(decShapeHull &hull){
	const decVector position(hull.GetPosition() + pOffset);
	const decQuaternion &orientation = hull.GetOrientation();
	const int pointCount = hull.GetPoints().GetCount();
	const bool hasNoShape = pBulletShape.IsNull();
	decVector center, minExtends, maxExtends;
	debpBulletShape::Ref bulletShapeHull;
	btConvexHullShape *hullShape = nullptr;
	bool needsTransform = false;
	int i;
	
	hullShape = new btConvexHullShape;
	
	for(i=0; i<pointCount; i++){
		const decVector &p = hull.GetPoints().GetAt(i);
		center += p;
		minExtends.SetSmallest(p);
		maxExtends.SetLargest(p);
		hullShape->addPoint(btVector3((btScalar)p.x, (btScalar)p.y, (btScalar)p.z), false);
	}
	hullShape->recalcLocalAabb();
	
	if(pNoMargin){
		hullShape->setMargin(BT_ZERO);
	}
	hullShape->setUserPointer((void*)(intptr_t)(pShapeIndex + 1));
	
	bulletShapeHull = debpBulletShape::Ref::New(hullShape);
	
	if(!orientation.IsEqualTo(decQuaternion()) || !position.IsZero()){
		needsTransform = true;
	}
	
	#ifdef DEBUGGING
	printf("debpCreateBulletShape.VisitShapeHull: points=%d\n", pointCount);
	#endif
	if(needsTransform){
		pAddTransformedCollisionShape(bulletShapeHull, btTransform(
			btQuaternion((btScalar)orientation.x, (btScalar)orientation.y,
				(btScalar)orientation.z, (btScalar)orientation.w),
			btVector3((btScalar)position.x, (btScalar)position.y, (btScalar)position.z)));
		
	}else{
		pAddCollisionShape(bulletShapeHull);
	}
	
	const decVector halfExtends((maxExtends - minExtends) * 0.5f);
	float smallestHalfExtends = decMath::min(halfExtends.x, halfExtends.y, halfExtends.z);
	float ccdRadius = 0.0f;
	
	if(pointCount > 0){
		center /= (float)pointCount;
		for(i=0; i<pointCount; i++){
			ccdRadius = decMath::max(ccdRadius, (hull.GetPoints().GetAt(i) - center).LengthSquared());
		}
		ccdRadius = sqrtf(ccdRadius);
	}
	
	smallestHalfExtends *= 0.25f; // 0.25 is a little ratio to improve detection (does it really improve anymore?)
	
	if(hasNoShape || ccdRadius < pCcdRadius){
		pCcdRadius = ccdRadius;
	}
	if(hasNoShape || smallestHalfExtends < pCcdThreshold){
		pCcdThreshold = smallestHalfExtends;
	}
}



// Private Functions
//////////////////////

void debpCreateBulletShape::pCreateCompoundShape(){
	#ifdef DEBUGGING
	printf("debpCreateBulletShape.pCreateCompoundShape\n");
	#endif
	
	btCompoundShape *compoundShape = new btCompoundShape(true);
	if(pNoMargin){
		compoundShape->setMargin(BT_ZERO);
	}
	compoundShape->setUserPointer((void*)(intptr_t)(pShapeIndex + 1));
		// setLocalScaling has to come last or scaling does not propagate.
		// for this reason it is applied during Finish() not here
	
	auto bulletShape = debpBulletCompoundShape::Ref::New(compoundShape);
	
	if(pBulletShape){
		// place bullet shape in compound clearing the reference
		#ifdef DEBUGGING
		printf("debpCreateBulletShape.pCreateCompoundShape: add present shape as child shape\n");
		#endif
		btTransform transform;
		transform.setIdentity(); // required, constructor does not initialize anything
		compoundShape->addChildShape(transform, pBulletShape->GetShape());
		bulletShape->AddChildShape(pBulletShape);
		pBulletShape = nullptr;
	}
	pBulletCompoundShape = bulletShape;
}

void debpCreateBulletShape::pAddCollisionShape(debpBulletShape *collisionShape){
	#ifdef DEBUGGING
	printf("debpCreateBulletShape.pAddCollisionShape\n");
	#endif
	
	if(pBulletShape || (!pBulletCompoundShape && pHasScale)){
		pCreateCompoundShape();
	}
	
	if(pBulletCompoundShape){
		#ifdef DEBUGGING
		printf("debpCreateBulletShape.pAddCollisionShape: add child shape\n");
		#endif
		btTransform transform;
		transform.setIdentity(); // required, constructor does not initialize anything
		pBulletCompoundShape->GetCompoundShape()->addChildShape(transform, collisionShape->GetShape());
		pBulletCompoundShape->AddChildShape(collisionShape);
		
	}else{
		#ifdef DEBUGGING
		printf("debpCreateBulletShape.pAddCollisionShape: set collision shape\n");
		#endif
		pBulletShape = collisionShape;
	}
}

void debpCreateBulletShape::pAddTransformedCollisionShape(debpBulletShape *collisionShape, const btTransform &transform){
	#ifdef DEBUGGING
	printf("debpCreateBulletShape.pAddTransformedCollisionShape: t=(%g,%g,%g) r=(%g,%g,%g,%g)\n", transform.getOrigin().getX(),
	transform.getOrigin().getY(), transform.getOrigin().getZ(), transform.getRotation().getX(), transform.getRotation().getY(),
	transform.getRotation().getZ(), transform.getRotation().getW());
	#endif
	
	if(!pBulletCompoundShape){
		pCreateCompoundShape();
	}
	
	#ifdef DEBUGGING
	printf("debpCreateBulletShape.pAddTransformedCollisionShape: add child\n");
	#endif
	pBulletCompoundShape->GetCompoundShape()->addChildShape(transform, collisionShape->GetShape());
	pBulletCompoundShape->AddChildShape(collisionShape);
}

void debpCreateBulletShape::pDebugPrintShape(dePhysicsBullet &bullet,
const btCollisionShape &shape, const char *prefix) const
{
	switch(shape.getShapeType()){
	case SPHERE_SHAPE_PROXYTYPE:{
		const btSphereShape &sphere = (btSphereShape&)shape;
		bullet.LogInfoFormat("%ssphere r=%f ls=(%f,%f,%f)", prefix, sphere.getRadius(),
			sphere.getLocalScaling().x(), sphere.getLocalScaling().y(), sphere.getLocalScaling().z());
		}break;
		
	case BOX_SHAPE_PROXYTYPE:{
		const btBoxShape &box = (btBoxShape&)shape;
		bullet.LogInfoFormat("%sbox he=(%f,%f,%f) ls=(%f,%f,%f)", prefix,
			box.getHalfExtentsWithoutMargin().x(), box.getHalfExtentsWithoutMargin().y(),
			box.getHalfExtentsWithoutMargin().z(), box.getLocalScaling().x(),
			box.getLocalScaling().y(), box.getLocalScaling().z());
		}break;
		
	case COMPOUND_SHAPE_PROXYTYPE:{
		const btCompoundShape &compound = (btCompoundShape&)shape;
		bullet.LogInfoFormat("%scompound ls=(%f,%f,%f)", prefix, compound.getLocalScaling().x(),
			compound.getLocalScaling().y(), compound.getLocalScaling().z());
		const decString childPrefix(decString(prefix) + "- ");
		const decString childPrefix2(decString(prefix) + "    ");
		const int count = compound.getNumChildShapes();
		int i;
		for(i=0; i<count; i++){
			const btTransform &t = compound.getChildTransform(i);
			const btVector3 p(t.getOrigin());
			const btQuaternion o(t.getRotation());
			const decVector r(decQuaternion(o.x(), o.y(), o.z(), o.w()).GetEulerAngles() * RAD2DEG);
			bullet.LogInfoFormat("%schild t=[(%f,%f,%f) (%f,%f,%f)]",
				childPrefix.GetString(), p.x(), p.y(), p.z(), r.x, r.y, r.z);
			pDebugPrintShape(bullet, *compound.getChildShape(i), childPrefix2);
		}
		}break;
		
	default:
		bullet.LogInfoFormat("%sshape ls=(%f,%f,%f)", prefix, shape.getLocalScaling().x(),
			shape.getLocalScaling().y(), shape.getLocalScaling().z());
	}
}
