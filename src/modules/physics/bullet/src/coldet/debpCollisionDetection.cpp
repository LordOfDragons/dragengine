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

#include "debpCollisionDetection.h"
#include "debpCDVHitModelFace.h"
#include "debpCDVMoveHitModelFace.h"
#include "debpConvexResultCallback.h"
#include "debpSweepCollisionTest.h"
#include "debpContactResultCallback.h"
#include "debpRayResultCallback.h"
#include "debpPointContactCallback.h"
#include "collision/debpDCollisionBox.h"
#include "collision/debpDCollisionDetection.h"
#include "collision/debpDCollisionTriangle.h"
#include "octree/debpDefaultDOctree.h"
#include "../dePhysicsBullet.h"
#include "../collider/debpColliderBone.h"
#include "../collider/debpColliderBones.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../collider/debpColliderRig.h"
#include "../component/debpModel.h"
#include "../component/debpComponent.h"
#include "../component/debpModelOctree.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../shape/debpShape.h"
#include "../world/debpCollisionWorld.h"
#include "../world/debpCollisionDispatcher.h"
#include "../world/debpConstraintSolver.h"
#include "../world/debpSharedCollisionFiltering.h"
#include "../world/debpWorld.h"

#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btDefaultSoftBodySolver.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderRig.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpCollisionDetection
/////////////////////////////////

// Constructor, destructor
////////////////////////////

debpCollisionDetection::debpCollisionDetection(dePhysicsBullet &bullet) :
pBullet(bullet),
pRayHackShape(*this),
pShapeCollision(bullet),
pPointTestShape(NULL),
pPointTestBulletColObj(NULL),
pSharedCollisionFiltering(nullptr),
pColConfig(nullptr),
pColDisp(nullptr),
pBroadPhase(nullptr),
pConstraintSolver(nullptr),
pSoftBodySolver(nullptr),
pDynWorld(nullptr)
{
	(void)pBullet;
	
	pColInfo = deCollisionInfo::Ref::New();
	
	decShapeSphere hackSphere(0.001f);
	pRayHackShape.AddShape(hackSphere, decVector(1.0f, 1.0f, 1.0f));
	
	pPointTestShape = new btSphereShape((btScalar)0.001);
	pPointTestShape->setMargin(BT_ZERO);
	
	pPointTestBulletColObj = new btCollisionObject;
	pPointTestBulletColObj->setCollisionShape(pPointTestShape);
	
	pSharedCollisionFiltering = new debpSharedCollisionFiltering;
	pColConfig = new btSoftBodyRigidBodyCollisionConfiguration;
	pColDisp = new debpCollisionDispatcher(*pSharedCollisionFiltering, pColConfig);
	pBroadPhase = new btDbvtBroadphase;
	pConstraintSolver = new debpConstraintSolver;
	pSoftBodySolver = new btDefaultSoftBodySolver;
	pDynWorld = new btSoftRigidDynamicsWorld(pColDisp, pBroadPhase, pConstraintSolver, pColConfig, pSoftBodySolver);
}

debpCollisionDetection::~debpCollisionDetection(){
	if(pDynWorld){
		delete pDynWorld;
	}
	if(pSoftBodySolver){
		delete pSoftBodySolver;
	}
	if(pConstraintSolver){
		delete pConstraintSolver;
	}
	if(pBroadPhase){
		delete pBroadPhase;
	}
	if(pSharedCollisionFiltering){
		delete pSharedCollisionFiltering;
	}
	if(pColDisp){
		delete pColDisp;
	}
	if(pColConfig){
		delete pColConfig;
	}
	
	if(pPointTestBulletColObj){
		delete pPointTestBulletColObj;
	}
	if(pPointTestShape){
		delete pPointTestShape;
	}
}



// Management
///////////////

// #define SPECIAL_DEBUG(s) s
#define SPECIAL_DEBUG(s)



// Collision Detection
////////////////////////

void debpCollisionDetection::PointHits(const decDVector &point, debpWorld &world,
const decCollisionFilter &collisionFilter, deBaseScriptingCollider &listener){
	world.UpdateDynWorldAABBs();
	
	const btScalar radius = (btScalar)0.001f;
	const btVector3 center((btScalar)point.x, (btScalar)point.y, (btScalar)point.z);
	const btVector3 boxMin(center - btVector3(radius, radius, radius));
	const btVector3 boxMax(center + btVector3(radius, radius, radius));
	
	pPointTestBulletColObj->getWorldTransform().setOrigin(center);
	
	debpPointContactCallback result(center, collisionFilter, listener, *this);
	world.GetDynamicsWorld()->getBroadphase()->aabbTest(boxMin, boxMax, result);
}

void debpCollisionDetection::RayHits(const decDVector &origin, const decDVector &direction,
debpWorld &world, const decCollisionFilter &collisionFilter, deBaseScriptingCollider &listener){
#define BULLET_RAY_CAST_UNSTABLE 1
	world.UpdateDynWorldAABBs();
	
	const btVector3 btRayFrom((btScalar)origin.x, (btScalar)origin.y, (btScalar)origin.z);
	const decDVector rayTo = origin + direction;
	const btVector3 btRayTo((btScalar)rayTo.x, (btScalar)rayTo.y, (btScalar)rayTo.z);
	
	#ifdef BULLET_RAY_CAST_UNSTABLE
		// bullet has a broken ray-box test implementation using Gjk which has a tendency
		// to miss collisions half of the time. as a quick fix a sweep test is done with
		// a tiny sphere which yields a comparable result but is not prone to the problem
		const btQuaternion btQuaterion(BT_ZERO, BT_ZERO, BT_ZERO, BT_ONE);
		const btTransform btTransformFrom(btQuaterion, btRayFrom);
		const btTransform btTransformTo(btQuaterion, btRayTo);
		
		debpConvexResultCallback result(pColInfo);
		result.SetTestRay(&collisionFilter, &listener);
		pRayHackShape.SweepTest(*world.GetDynamicsWorld(), btTransformFrom, btTransformTo, result);
		
	#else
		debpRayResultCallback result(pColInfo);
		result.SetTestRay(origin, direction, &collisionFilter, &listener);
		
		world.GetDynamicsWorld()->rayTest(btRayFrom, btRayTo, result);
	#endif
}

void debpCollisionDetection::ColliderHits(debpCollider *collider, debpWorld *world, deBaseScriptingCollider *listener){
	DEASSERT_NOTNULL(collider)
	DEASSERT_NOTNULL(world)
	DEASSERT_NOTNULL(listener)
	
	world->UpdateDynWorldAABBs();
	
	debpCollisionWorld &dynamicsWorld = *world->GetDynamicsWorld();
	debpContactResultCallback resultCallback(pColInfo);
	
	if(collider->IsVolume()){
		btCollisionObject * const co = collider->CastToVolume()->GetStaticCollisionTestPrepare();
		if(!co){
			return;
		}
		
		resultCallback.SetTestCollider(co, collider, listener);
		pColInfo->SetStopTesting(false);
		
		//printf( "ColliderHits %p(%p)\n", staticCollisionTest, collider );
		dynamicsWorld.contactTest(co, resultCallback);
	}
}

void debpCollisionDetection::ColliderMoveHits(debpCollider *collider, const decDVector &displacement,
debpWorld *world, deBaseScriptingCollider *listener){
	if(!collider || !world || !listener){
		DETHROW(deeInvalidParam);
	}
	
	world->UpdateDynWorldAABBs();
	
	debpCollisionWorld &dynamicsWorld = *world->GetDynamicsWorld();
	debpConvexResultCallback resultCallback(pColInfo);
	resultCallback.SetTestCollider(collider, listener);
	
	if(collider->IsVolume()){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if(sweepCollisionTest.GetShapeList().GetCount() == 0){
			return;
		}
		
		const decDVector &from = colliderVolume.GetPosition();
		const btVector3 btfrom((btScalar)from.x, (btScalar)from.y, (btScalar)from.z);
		
		const decDVector to = from + displacement;
		const btVector3 btto((btScalar)to.x, (btScalar)to.y, (btScalar)to.z);
		
		const decQuaternion &orientation = colliderVolume.GetOrientation();
		const btQuaternion btorientation((btScalar)orientation.x, (btScalar)orientation.y,
			(btScalar)orientation.z, (btScalar)orientation.w);
		
		const btTransform transformFrom(btorientation, btfrom);
		const btTransform transformTo(btorientation, btto);
		
		sweepCollisionTest.SweepTest(dynamicsWorld, transformFrom, transformTo, resultCallback);
	}
}

void debpCollisionDetection::ColliderRotateHits(debpCollider *collider, const decVector &rotation,
debpWorld *world, deBaseScriptingCollider *listener){
	if(!collider || !world || !listener){
		DETHROW(deeInvalidParam);
	}
	
	world->UpdateDynWorldAABBs();
	
	debpCollisionWorld &dynamicsWorld = *world->GetDynamicsWorld();
	debpConvexResultCallback resultCallback(pColInfo);
	resultCallback.SetTestCollider(collider, listener);
	
	if(collider->IsVolume()){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if(sweepCollisionTest.GetShapeList().GetCount() == 0){
			return;
		}
		
		const decDVector &position = colliderVolume.GetPosition();
		const btVector3 btposition((btScalar)position.x, (btScalar)position.y, (btScalar)position.z);
		
		const decQuaternion &orientationFrom = colliderVolume.GetOrientation();
		const btQuaternion btRotFrom((btScalar)orientationFrom.x, (btScalar)orientationFrom.y,
			(btScalar)orientationFrom.z, (btScalar)orientationFrom.w);
		
		const decQuaternion orientationTo = orientationFrom * decQuaternion::CreateFromEuler(rotation);
		const btQuaternion btRotTo((btScalar)orientationTo.x, (btScalar)orientationTo.y,
			(btScalar)orientationTo.z, (btScalar)orientationTo.w);
		
		const btTransform transformFrom(btRotFrom, btposition);
		const btTransform transformTo(btRotTo, btposition);
		
		sweepCollisionTest.SweepTest(dynamicsWorld, transformFrom, transformTo, resultCallback);
	}
}

void debpCollisionDetection::ColliderMoveRotateHits(debpCollider *collider, const decVector &displacement,
const decVector &rotation, debpWorld *world, deBaseScriptingCollider *listener){
	if(!collider || !world || !listener){
		DETHROW(deeInvalidParam);
	}
	
	world->UpdateDynWorldAABBs();
	
	debpCollisionWorld &dynamicsWorld = *world->GetDynamicsWorld();
	debpConvexResultCallback resultCallback(pColInfo);
	resultCallback.SetTestCollider(collider, listener);
	
	if(collider->IsVolume()){
		debpColliderVolume &colliderVolume = *collider->CastToVolume();
		debpSweepCollisionTest &sweepCollisionTest = *colliderVolume.GetSweepCollisionTest();
		
		if(sweepCollisionTest.GetShapeList().GetCount() == 0){
			return;
		}
		
		const decDVector &positionFrom = colliderVolume.GetPosition();
		const btVector3 btPosFrom((btScalar)positionFrom.x, (btScalar)positionFrom.y, (btScalar)positionFrom.z);
		
		const decDVector positionTo = positionFrom + decDVector(displacement);
		const btVector3 btPosTo((btScalar)positionTo.x, (btScalar)positionTo.y, (btScalar)positionTo.z);
		
		const decQuaternion &orientationFrom = colliderVolume.GetOrientation();
		const btQuaternion btRotFrom((btScalar)orientationFrom.x, (btScalar)orientationFrom.y,
			(btScalar)orientationFrom.z, (btScalar)orientationFrom.w);
		
		const decQuaternion orientationTo = orientationFrom * decQuaternion::CreateFromEuler(rotation);
		const btQuaternion btRotTo((btScalar)orientationTo.x, (btScalar)orientationTo.y,
			(btScalar)orientationTo.z, (btScalar)orientationTo.w);
		
		const btTransform transformFrom(btRotFrom, btPosFrom);
		const btTransform transformTo(btRotTo, btPosTo);
		
		sweepCollisionTest.SweepTest(dynamicsWorld, transformFrom, transformTo, resultCallback);
	}
}



#if 0
bool debpCollisionDetection::RayHitsHeightTerrain(const decDVector &origin, const decDVector &direction,
debpHTSector *sector, bool testHiddenFaces, debpCollisionResult &result){
	if(!sector) DETHROW(deeInvalidParam);
	
	int x, z, sx, sz, ex, ez, base;
	debpCollisionResult tresult;
	debpDCollisionBox clusterBox;
	decDVector cbcenter;
	
	debpDCollisionTriangle collisionTriangle;
	
	deHeightTerrain *engHT = sector->GetHeightTerrain()->GetHeightTerrain();
	int imageDim = engHT->GetSectorResolution();
	float sectorDim = engHT->GetSectorSize();
	
	deHeightTerrainSector *engHTSector = sector->GetSector();
	int c, clusterCount;
	debpHTSCluster *clusters;
	decVector *points;
	decDVector pt[4];
	
	const decPoint &scoord = engHTSector->GetSector();
	double soffsetx = (double)sectorDim * scoord.x;
	double soffsetz = (double)sectorDim * scoord.y;
	
	clusterCount = sector->GetClusterCount() * sector->GetClusterCount();
	clusters = sector->GetClusters();
	points = sector->GetPoints();
	
	double distance;
	
	double hitDist = 0.0;
	decDVector hitNormal;
	bool hasHit = false;
	
	decDVector hp; // TEMPORARY
	
	// test all cluster bounding boxes for a hit
	for(c=0; c<clusterCount; c++){
		cbcenter = clusters[c].center;
		cbcenter.x += soffsetx;
		cbcenter.z += soffsetz;
		
		clusterBox.SetCenter(cbcenter);
		clusterBox.SetHalfSize(clusters[c].halfExtends);
		
		if(clusterBox.RayHitsVolume(origin, direction, distance)){
			// determine the range of points to test for this cluster
			sx = clusters[c].firstPointX;
			sz = clusters[c].firstPointZ;
			ex = sx + clusters[c].pointCountX - 1;
			ez = sz + clusters[c].pointCountZ - 1;
			
			// test all faces in the cluster for a hit
			for(z=sz; z<ez; z++){
				base = imageDim * z;
				
				for(x=sx; x<ex; x++){
					if(testHiddenFaces || engHTSector->GetFaceVisibleAt(x, z)){
						// determine the four corner points
						pt[0] = points[base + x];
						pt[1] = points[base + x + 1];
						pt[2] = points[base + imageDim + x];
						pt[3] = points[base + imageDim + x + 1];
						
						pt[0].x += soffsetx;
						pt[0].z += soffsetz;
						pt[1].x += soffsetx;
						pt[1].z += soffsetz;
						pt[2].x += soffsetx;
						pt[2].z += soffsetz;
						pt[3].x += soffsetx;
						pt[3].z += soffsetz;
						
						// TODO ray hits triangle is not working yet
						
						// test first triangle ( 1, 2, 4 )
						collisionTriangle.SetCorners(pt[0], pt[1], pt[3]);
						//if( collisionTriangle.RayHitsVolume( origin, direction, distance ) ){
						distance = debpDCollisionDetection::RayPlane(pt[0], collisionTriangle.GetNormal(), origin, direction);
						if(distance >= 0.0){
							hp = origin + direction * distance;
							if(debpDCollisionDetection::PointInTriangle(pt[0], pt[1], pt[3], hp)){
								if(!hasHit || distance < hitDist){
									hitDist = distance;
									hitNormal = collisionTriangle.GetNormal();
									hasHit = true;
								}
							}
						}
						
						// test second traingle ( 1, 4, 3 )
						collisionTriangle.SetCorners(pt[0], pt[3], pt[2]);
						//if( collisionTriangle.RayHitsVolume( origin, direction, distance ) ){
						distance = debpDCollisionDetection::RayPlane(pt[0], collisionTriangle.GetNormal(), origin, direction);
						if(distance >= 0.0){
							hp = origin + direction * distance;
							if(debpDCollisionDetection::PointInTriangle(pt[0], pt[3], pt[2], hp)){
								if(!hasHit || distance < hitDist){
									hitDist = distance;
									hitNormal = collisionTriangle.GetNormal();
									hasHit = true;
								}
							}
						}
					}
				}
			}
		}
	}
	
	if(hasHit){
		result.face = -1; // possible encoding later on
		result.distance = (float)hitDist;
		result.normal = hitNormal;
		return true;
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::ShapeHitsHeightTerrain(debpShape *shape, debpHTSector *sector, debpCollisionResult &result){
	if(!shape || !sector) DETHROW(deeInvalidParam);
	
	debpDCollisionVolume *colvol = shape->GetCollisionVolume();
	int x, z, sx, sz, ex, ez, base;
	debpCollisionResult tresult;
	debpDCollisionBox clusterBox;
	debpDCollisionBox box;
	decDVector cbcenter;
	
	debpDCollisionTriangle collisionTriangle;
	
	deHeightTerrain *engHT = sector->GetHeightTerrain()->GetHeightTerrain();
	int imageDim = engHT->GetSectorResolution();
	float sectorDim = engHT->GetSectorSize();
	
	deHeightTerrainSector *engHTSector = sector->GetSector();
	int c, clusterCount;
	debpHTSCluster *clusters;
	decVector *points;
	decDVector pt[4];
	
	colvol->GetEnclosingBox(&box);
	
	const decPoint &scoord = engHTSector->GetSector();
	double soffsetx = (double)sectorDim * scoord.x;
	double soffsetz = (double)sectorDim * scoord.y;
	
	clusterCount = sector->GetClusterCount() * sector->GetClusterCount();
	clusters = sector->GetClusters();
	points = sector->GetPoints();
	
	// test all cluster bounding boxes for a hit
	for(c=0; c<clusterCount; c++){
		cbcenter = clusters[c].center;
		cbcenter.x += soffsetx;
		cbcenter.z += soffsetz;
		
		clusterBox.SetCenter(cbcenter);
		clusterBox.SetHalfSize(clusters[c].halfExtends);
		
		if(box.BoxHitsBox(&clusterBox)){
			// determine the range of points to test for this cluster
			sx = clusters[c].firstPointX;
			sz = clusters[c].firstPointZ;
			ex = sx + clusters[c].pointCountX - 1;
			ez = sz + clusters[c].pointCountZ - 1;
			
			// test all faces in the cluster for a hit
			for(z=sz; z<ez; z++){
				base = imageDim * z;
				
				for(x=sx; x<ex; x++){
					if(engHTSector->GetFaceVisibleAt(x, z)){
						// determine the four corner points
						pt[0] = points[base + x];
						pt[1] = points[base + x + 1];
						pt[2] = points[base + imageDim + x];
						pt[3] = points[base + imageDim + x + 1];
						
						pt[0].x += soffsetx;
						pt[0].z += soffsetz;
						pt[1].x += soffsetx;
						pt[1].z += soffsetz;
						pt[2].x += soffsetx;
						pt[2].z += soffsetz;
						pt[3].x += soffsetx;
						pt[3].z += soffsetz;
						
						// test first triangle ( 1, 2, 4 )
						collisionTriangle.SetCorners(pt[0], pt[1], pt[3]);
						if(colvol->TriangleHitsVolume(&collisionTriangle)){
							result.face = -1; // possible encoding later on
							return true;
						}
						
						// test second traingle ( 1, 4, 3 )
						collisionTriangle.SetCorners(pt[0], pt[3], pt[2]);
						if(colvol->TriangleHitsVolume(&collisionTriangle)){
							result.face = -1; // possible encoding later on
							return true;
						}
					}
				}
			}
		}
	}
	
	return false;
}
#endif

bool debpCollisionDetection::ColliderHitsHeightTerrain(debpCollider *collider, debpHTSector *sector, debpCollisionResult &result){
	if(!collider || !sector) DETHROW(deeInvalidParam);
	
	if(!sector->GetSector()->GetHeightImage()) return false;
	if(sector->GetSector()->GetTextureCount() == 0) return false;
	
	int x, z, sx, sz, ex, ez, base;
	debpCollisionResult tresult;
	debpDCollisionBox clusterBox;
	debpDCollisionBox box;
	decDVector cbcenter;
	int hitShape = -1;
	int hitBone = 0;
	
	deHeightTerrain *engHT = sector->GetHeightTerrain()->GetHeightTerrain();
	int imageDim = engHT->GetSectorResolution();
	float sectorDim = engHT->GetSectorSize();
	
	deHeightTerrainSector *engHTSector = sector->GetSector();
	int c, clusterCount;
	debpHTSCluster *clusters;
	decVector *points;
	decDVector pt[4];
	
	box.SetFromExtends(collider->GetShapeMinimumExtend(), collider->GetShapeMaximumExtend());
	
	const decPoint &scoord = engHTSector->GetSector();
	double soffsetx = (double)sectorDim * scoord.x;
	double soffsetz = (double)sectorDim * scoord.y;
	
	clusterCount = sector->GetClusterCount() * sector->GetClusterCount();
	clusters = sector->GetClusters();
	points = sector->GetPoints();
	
	// test all cluster bounding boxes for a hit
	for(c=0; c<clusterCount; c++){
		cbcenter = clusters[c].center;
		cbcenter.x += soffsetx;
		cbcenter.z += soffsetz;
		
		clusterBox.SetCenter(cbcenter);
		clusterBox.SetHalfSize(clusters[c].halfExtends);
		
		if(box.BoxHitsBox(&clusterBox)){
			// determine the range of points to test for this cluster
			sx = clusters[c].firstPointX;
			sz = clusters[c].firstPointZ;
			ex = sx + clusters[c].pointCountX - 1;
			ez = sz + clusters[c].pointCountZ - 1;
			
			// test all faces in the cluster for a hit
			for(z=sz; z<ez; z++){
				base = imageDim * z;
				
				for(x=sx; x<ex; x++){
					if(engHTSector->GetFaceVisibleAt(x, z)){
						// determine the four corner points
						pt[0] = points[base + x];
						pt[1] = points[base + x + 1];
						pt[2] = points[base + imageDim + x];
						pt[3] = points[base + imageDim + x + 1];
						
						pt[0].x += soffsetx;
						pt[0].z += soffsetz;
						pt[1].x += soffsetx;
						pt[1].z += soffsetz;
						pt[2].x += soffsetx;
						pt[2].z += soffsetz;
						pt[3].x += soffsetx;
						pt[3].z += soffsetz;
						
						// test first triangle ( 1, 2, 4 )
						if(ColliderHitsTriangle(collider, pt[0], pt[1], pt[3], tresult)){
							hitShape = tresult.shape1;
							hitBone = tresult.bone1;
							break;
						}
						
						// test second traingle ( 1, 4, 3 )
						if(ColliderHitsTriangle(collider, pt[0], pt[3], pt[2], tresult)){
							hitShape = tresult.shape1;
							hitBone = tresult.bone1;
							break;
						}
					}
				}
			}
		}
	}
	
	if(hitShape != -1){
		result.shape1 = hitShape;
		result.bone1 = hitBone;
		result.face = -1; // as there is no way yet to tell which face on a height map
		return true;
	}
	
	return false;
}

#if 0
bool debpCollisionDetection::ColliderMoveHitsHeightTerrain(debpCollider *collider, const decDVector &displacement,
debpHTSector *sector, debpCollisionResult &result){
	if(!collider || !sector) DETHROW(deeInvalidParam);
	
	if(!sector->GetSector()->GetHeightImage()) return false;
	if(sector->GetSector()->GetTextureCount() == 0) return false;
	
	int x, z, sx, sz, ex, ez, base;
	debpCollisionResult tresult;
	debpDCollisionBox clusterBox;
	debpDCollisionBox box;
	decDVector cbcenter;
	bool hasCollision = false;
	decDVector hitNormal;
	float hitDistance = 0.0f;
	int hitShape = 0;
	int hitBone = 0;
	
	deHeightTerrain *engHT = sector->GetHeightTerrain()->GetHeightTerrain();
	int imageDim = engHT->GetSectorResolution();
	float sectorDim = engHT->GetSectorSize();
	
	deHeightTerrainSector *engHTSector = sector->GetSector();
	int c, clusterCount;
	debpHTSCluster *clusters;
	decVector *points;
	decDVector pt[4];
	
	GetColliderMoveBoundingBox(*collider, displacement, box);
	//printf( "# d=(%f,%f,%f) c=(%f,%f,%f) he=(%f,%f,%f)\n", displacement.x, displacement.y, displacement.z,
	//	box.GetCenter().x, box.GetCenter().y, box.GetCenter().z, box.GetHalfSize().x, box.GetHalfSize().y, box.GetHalfSize().z );
	
	const decPoint &scoord = engHTSector->GetSector();
	double soffsetx = (double)sectorDim * scoord.x;
	double soffsetz = (double)sectorDim * scoord.y;
	
	clusterCount = sector->GetClusterCount() * sector->GetClusterCount();
	clusters = sector->GetClusters();
	points = sector->GetPoints();
	
	// test all cluster bounding boxes for a hit
	for(c=0; c<clusterCount; c++){
		cbcenter = clusters[c].center;
		cbcenter.x += soffsetx;
		cbcenter.z += soffsetz;
		
		clusterBox.SetCenter(cbcenter);
		clusterBox.SetHalfSize(clusters[c].halfExtends);
		
		if(box.BoxHitsBox(&clusterBox)){
			// determine the range of points to test for this cluster
			sx = clusters[c].firstPointX;
			sz = clusters[c].firstPointZ;
			ex = sx + clusters[c].pointCountX - 1;
			ez = sz + clusters[c].pointCountZ - 1;
			
			// test all faces in the cluster for a hit
			for(z=sz; z<ez; z++){
				base = imageDim * z;
				
				for(x=sx; x<ex; x++){
					if(engHTSector->GetFaceVisibleAt(x, z)){
						// determine the four corner points
						pt[0] = points[base + x];
						pt[1] = points[base + x + 1];
						pt[2] = points[base + imageDim + x];
						pt[3] = points[base + imageDim + x + 1];
						
						pt[0].x += soffsetx;
						pt[0].z += soffsetz;
						pt[1].x += soffsetx;
						pt[1].z += soffsetz;
						pt[2].x += soffsetx;
						pt[2].z += soffsetz;
						pt[3].x += soffsetx;
						pt[3].z += soffsetz;
						
						// test first triangle ( 1, 2, 4 )
						if(ColliderMoveHitsTriangle(collider, displacement, pt[0], pt[1], pt[3], tresult)){
							if(!hasCollision || tresult.distance < hitDistance){
								hitDistance = tresult.distance;
								hitNormal = tresult.normal;
								hitShape = tresult.shape1;
								hitBone = tresult.bone1;
								hasCollision = true;
							}
						}
						
						// test second traingle ( 1, 4, 3 )
						if(ColliderMoveHitsTriangle(collider, displacement, pt[0], pt[3], pt[2], tresult)){
							if(!hasCollision || tresult.distance < hitDistance){
								hitDistance = tresult.distance;
								hitNormal = tresult.normal;
								hitShape = tresult.shape1;
								hitBone = tresult.bone1;
								hasCollision = true;
							}
						}
					}
				}
			}
		}
	}
	
	if(hasCollision){
		result.shape1 = hitShape;
		result.bone1 = hitBone;
		result.face = -1; // as there is no way yet to tell which face on a height map
		result.distance = hitDistance;
		result.normal = hitNormal;
		return true;
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::ColliderRotateHitsHeightTerrain(debpCollider *collider, const decVector &rotation,
debpHTSector *sector, debpCollisionResult &result){
#if 0
	if(!collider || !sector) DETHROW(deeInvalidParam);
	
	// this test is not optimal yet. the currentl implementation of the collision tests
	// does not contain yet tests for rotation hit testing. since we do not yet use
	// bullet for the collision testing ( planned for the future ) a little hack has to
	// be used which is not fast and fully correct but works for the time being. for this
	// the collider is tested in the final rotation position. if this test fails it is
	// rotated half way back and tested again. from there on whenever a collision happens
	// the collider is rotated half way back the current rotation distance otherwise it
	// is rotated forward. once the rotation drops below a threshold the test can be
	// considered finished.
	
	int x, z, sx, sz, ex, ez, base;
	debpCollisionResult tresult;
	debpDCollisionBox clusterBox;
	debpDCollisionBox box;
	decDVector cbcenter;
	bool hasCollision = false;
	decDVector hitNormal;
	float hitRotation = 0.0f;
	int hitShape = 0;
	int hitBone = 0;
	
	deHeightTerrain *engHT = sector->GetHeightTerrain()->GetHeightTerrain();
	int imageDim = engHT->GetSectorResolution();
	float sectorDim = engHT->GetSectorSize();
	
	deHeightTerrainSector *engHTSector = sector->GetSector();
	int c, clusterCount;
	debpHTSCluster *clusters;
	decVector *points;
	decDVector pt[4];
	
	GetColliderMoveBoundingBox(collider, displacement, box);
	
	const decPoint &scoord = engHTSector->GetSector();
	double soffsetx = (double)sectorDim * scoord.x;
	double soffsetz = (double)sectorDim * scoord.y;
	
	clusterCount = sector->GetClusterCount() * sector->GetClusterCount();
	clusters = sector->GetClusters();
	points = sector->GetPoints();
	
	// test all cluster bounding boxes for a hit
	for(c=0; c<clusterCount; c++){
		cbcenter = clusters[c].center;
		cbcenter.x += soffsetx;
		cbcenter.z += soffsetz;
		
		clusterBox.SetCenter(cbcenter);
		clusterBox.SetHalfSize(clusters[c].halfExtends);
		
		if(box.BoxHitsBox(&clusterBox)){
			// determine the range of points to test for this cluster
			sx = clusters[c].firstPointX;
			sz = clusters[c].firstPointZ;
			ex = sx + clusters[c].pointCountX - 1;
			ez = sz + clusters[c].pointCountZ - 1;
			
			// test all faces in the cluster for a hit
			for(z=sz; z<ez; z++){
				base = imageDim * z;
				
				for(x=sx; x<ex; x++){
					if(engHTSector->GetFaceVisibleAt(x, z)){
						// determine the four corner points
						pt[0] = points[base + x];
						pt[1] = points[base + x + 1];
						pt[2] = points[base + imageDim + x];
						pt[3] = points[base + imageDim + x + 1];
						
						pt[0].x += soffsetx;
						pt[0].z += soffsetz;
						pt[1].x += soffsetx;
						pt[1].z += soffsetz;
						pt[2].x += soffsetx;
						pt[2].z += soffsetz;
						pt[3].x += soffsetx;
						pt[3].z += soffsetz;
						
						// test first triangle ( 1, 2, 4 )
						if(ColliderMoveHitsTriangle(collider, displacement, pt[0], pt[1], pt[3], tresult)){
							if(!hasCollision || tresult.distance < hitDistance){
								hitDistance = tresult.distance;
								hitNormal = tresult.normal;
								hitShape = tresult.shape1;
								hitBone = tresult.bone1;
								hasCollision = true;
							}
						}
						
						// test second traingle ( 1, 4, 3 )
						if(ColliderMoveHitsTriangle(collider, displacement, pt[0], pt[3], pt[2], tresult)){
							if(!hasCollision || tresult.distance < hitDistance){
								hitDistance = tresult.distance;
								hitNormal = tresult.normal;
								hitShape = tresult.shape1;
								hitBone = tresult.bone1;
								hasCollision = true;
							}
						}
					}
				}
			}
		}
	}
	
	if(hasCollision){
		result.shape1 = hitShape;
		result.bone1 = hitBone;
		result.face = -1; // as there is no way yet to tell which face on a height map
		result.distance = hitDistance;
		result.normal = hitNormal;
		return true;
	}
#endif
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::RayHitsCollider(const decDVector &origin, const decDVector &direction, debpCollider *collider,
debpCollisionResult &result){
	if(!collider) DETHROW(deeInvalidParam);
	
	if(collider->IsVolume()){
		if(RayHitsColliderVolume(origin, direction, *((debpColliderVolume*)collider), result)){
			result.bone2 = -1;
			return true;
		}
		
	}else if(collider->IsComponent()){
		if(RayHitsColliderComponent(origin, direction, *((debpColliderComponent*)collider), result)){
			return true;
		}
		
	}else{ // collider rigged
		if(RayHitsColliderRig(origin, direction, *((debpColliderRig*)collider), result)){
			return true;
		}
	}
	
	return false;
}
#endif

bool debpCollisionDetection::ColliderHitsCollider(debpCollider *collider1, debpCollider *collider2,
debpCollisionResult &result){
	DEASSERT_NOTNULL(collider1)
	DEASSERT_NOTNULL(collider2)
	
	// TODO this is not good. this uses the outdated dragengine collision routines.
	//      this test should use the bullet collision testing instead. not sure though
	//      how this works for a collider-collider direct test without the interference
	//      of needsCollision() and other stuff. we need to use the right algorithm
	//      directly it seems
	
	if(collider1->IsVolume()){
		debpColliderVolume &colliderVolume = *((debpColliderVolume*)collider1);
		
		if(collider2->IsVolume()){
			if(ColliderVolumeHitsColliderVolume(colliderVolume, *((debpColliderVolume*)collider2), result)){
				result.bone1 = -1;
				result.bone2 = -1;
				return true;
			}
			
		}else if(collider2->IsComponent()){
			if(ColliderVolumeHitsColliderComponent(colliderVolume, *((debpColliderComponent*)collider2), result)){
				result.bone1 = -1;
				return true;
			}
			
		}else{ // collider rigged
			if(ColliderVolumeHitsColliderRig(colliderVolume, *((debpColliderRig*)collider2), result)){
				result.bone1 = -1;
				return true;
			}
		}
		
	}else if(collider1->IsComponent()){ // collider component
		debpColliderComponent &colliderComponent = *((debpColliderComponent*)collider1);
		
		if(collider2->IsVolume()){
			if(ColliderVolumeHitsColliderComponent(*((debpColliderVolume*)collider2), colliderComponent, result)){
				// we changed the order of the colliders so we have to also change the order of the results
				result.shape2 = result.shape1;
				result.shape1 = -1;
				result.bone1 = result.bone2;
				result.bone2 = -1;
				return true;
			}
			
		}else if(collider2->IsComponent()){
			if(ColliderComponentHitsColliderComponent(colliderComponent, *((debpColliderComponent*)collider2), result)){
				result.shape1 = -1;
				result.shape2 = -1;
				return true;
			}
			
		}else{ // collider rigged
			if(ColliderComponentHitsColliderRig(colliderComponent, *((debpColliderRig*)collider2), result)){
				result.shape1 = -1;
				result.shape2 = -1;
				return true;
			}
		}
		
	}else{ // collider rigged
		// TODO implement similar to collider component
		return false;
	}
	
	return false;
}

void debpCollisionDetection::ColliderHitsCollider(debpCollider &collider1, debpCollider &collider2,
debpContactResultCallback &result, bool reversedColliders){
	static btManifoldPoint dummyManifoldPoint;
	
	if(collider1.IsVolume()){
		debpColliderVolume &cv = *collider1.CastToVolume();
		btCollisionObject * const co = cv.GetStaticCollisionTest();
		if(!co){
			return;
		}
		
		if(collider2.IsVolume()){
			btCollisionObject * const co2 = collider2.CastToVolume()->GetStaticCollisionTest();
			if(!co2){
				return;
			}
			
			result.SetTestCollisionObject(reversedColliders ? co2 : co);
			contactPairTest(co, co2, result);
			
		}else if(collider2.IsComponent()){
			debpColliderComponent &cv2 = *collider2.CastToComponent();
			if(!cv2.GetColliderComponent().GetComponent()){
				return;
			}
			
			switch(cv2.GetTestMode()){
			case debpColliderComponent::etmModelStatic:
			case debpColliderComponent::etmModelDynamic:{
				btCollisionObject * const co2 = cv2.GetStaticCollisionTestPrepare();
				if(!co2){
					return;
				}
				
				debpCollisionResult cdr;
				if(!ColliderVolumeHitsColliderComponent(cv, cv2, cdr)){
					return;
				}
				
				result.SetTestCollisionObject(reversedColliders ? co2 : co);
				
				const btCollisionObjectWrapper ow1(nullptr,
					co->getCollisionShape(), co, co->getWorldTransform(), -1, -1);
				const btCollisionObjectWrapper ow2(nullptr,
					co2->getCollisionShape(), co2, co2->getWorldTransform(), -1, -1);
				result.addSingleResult(dummyManifoldPoint, &ow1, 0, -1, &ow2, 0, cdr.face);
				}break;
				
			case debpColliderComponent::etmRigShape:{
				btCollisionObject * const co2 = cv2.GetStaticCollisionTestPrepare();
				if(!co2){
					return;
				}
				
				result.SetTestCollisionObject(reversedColliders ? co2 : co);
				contactPairTest(co, co2, result);
				}break;
				
			case debpColliderComponent::etmBoneShape:{
				debpColliderBones * const bones = cv2.GetBones();
				if(!bones || !bones->UpdateStaticCollisionTests()){
					return;
				}
				
				const int boneCount = bones->GetBoneCount();
				int i;
				for(i=0; i<boneCount; i++){
					btCollisionObject * const co2 = bones->GetBoneAt(i)->GetStaticCollisionTest();
					if(!co2){
						continue;
					}
					
					result.SetTestCollisionObject(reversedColliders ? co2 : co);
					contactPairTest(co, co2, result);
					if(pColInfo->GetStopTesting()){
						return;
					}
				}
				}break;
				
			default:
				break;
			}
			
		}else if(collider2.IsRigged()){
			debpColliderRig &cv2 = *collider2.CastToRigged();
			if(!cv2.GetColliderRig().GetRig()){
				return;
			}
			
			switch(cv2.GetTestMode()){
			case debpColliderRig::etmRigShape:{
				/*
				btGhostObject * const go2 = cv2.GetStaticCollisionTestPrepare();
				if(go2){
					result.SetTestCollisionObject(reversedColliders ? go2 : go);
					contactPairTest(go, go2, result);
				}
				*/
				}break;
				
			case debpColliderRig::etmBoneShape:{
				debpColliderBones * const bones = cv2.GetBones();
				if(!bones || !bones->UpdateStaticCollisionTests()){
					return;
				}
				
				const int boneCount = bones->GetBoneCount();
				int i;
				for(i=0; i<boneCount; i++){
					btCollisionObject * const co2 = bones->GetBoneAt(i)->GetStaticCollisionTest();
					if(!co2){
						continue;
					}
					
					result.SetTestCollisionObject(reversedColliders ? co2 : co);
					contactPairTest(co, co2, result);
					if(pColInfo->GetStopTesting()){
						return;
					}
				}
				}break;
				
			default:
				break;
			}
		}
		
	}else if(collider1.IsComponent()){
		//debpColliderComponent &cv = *collider1.CastToComponent();
		
		if(collider2.IsVolume()){
			ColliderHitsCollider(collider2, collider1, result, true);
			
		}else if(collider2.IsComponent()){
			/*
			if(ColliderComponentHitsColliderComponent(colliderComponent, *((debpColliderComponent*)collider2), result)){
				result.shape1 = -1;
				result.shape2 = -1;
				return true;
			}
			*/
			
		}else if(collider2.IsRigged()){
			/*
			if(ColliderComponentHitsColliderRig(colliderComponent, *((debpColliderRig*)collider2), result)){
				result.shape1 = -1;
				result.shape2 = -1;
				return true;
			}
			*/
		}
		
	}else if(collider1.IsRigged()){
		// TODO implement similar to collider component
	}
}

#if 0
bool debpCollisionDetection::ShapeHitsCollider(debpShape *shape, debpCollider *collider,
debpCollisionResult &result){
	if(!shape || !collider){
		DETHROW(deeInvalidParam);
	}
	
	if(collider->IsVolume()){
		return ShapeHitsColliderVolume(*shape, *((debpColliderVolume*)collider), result);
		
	}else if(collider->IsComponent()){
		return ShapeHitsColliderComponent(*shape, *((debpColliderComponent*)collider), result);
		
	}else{ // collider rigged
		return ShapeHitsColliderRig(*shape, *((debpColliderRig*)collider), result);
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::ColliderMoveHitsCollider(debpCollider *collider1, const decDVector &displacement,
debpCollider *collider2, debpCollisionResult &result){
	if(!collider1 || !collider2) DETHROW(deeInvalidParam);
	
	if(collider1->IsVolume()){
		debpColliderVolume &colliderVolume = *((debpColliderVolume*)collider1);
		
		if(collider2->IsVolume()){
			if(ColliderVolumeMoveHitsColliderVolume(colliderVolume, displacement, *((debpColliderVolume*)collider2), result)){
				result.bone1 = -1;
				result.bone2 = -1;
				result.face = -1;
				return true;
			}
			
		}else if(collider2->IsComponent()){
			if(ColliderVolumeMoveHitsColliderComponent(colliderVolume, displacement, *((debpColliderComponent*)collider2), result)){
				result.bone1 = -1;
				result.shape2 = -1;
				return true;
			}
			
		}else{ // collider rigged
			if(ColliderVolumeMoveHitsColliderRig(colliderVolume, displacement, *((debpColliderRig*)collider2), result)){
				result.bone1 = -1;
				result.shape2 = -1;
				return true;
			}
		}
		
	}else if(collider1->IsComponent()){
		debpColliderComponent &colliderComponent = *((debpColliderComponent*)collider1);
		
		if(collider2->IsVolume()){
			if(ColliderVolumeMoveHitsColliderComponent(*((debpColliderVolume*)collider2), displacement, colliderComponent, result)){
				// we changed the order of the colliders so we have to also change the order of the results
				result.shape2 = result.shape1;
				result.shape1 = -1;
				result.bone1 = result.bone2;
				result.bone2 = -1;
				result.normal = -result.normal;
				return true;
			}
			
		}else if(collider2->IsComponent()){
			if(ColliderComponentMoveHitsColliderComponent(colliderComponent, displacement, *((debpColliderComponent*)collider2), result)){
				result.shape1 = -1;
				result.shape2 = -1;
				return true;
			}
			
		}else{ // collider rigged
			if(ColliderComponentMoveHitsColliderRig(colliderComponent, displacement, *((debpColliderRig*)collider2), result)){
				result.shape1 = -1;
				result.shape2 = -1;
				return true;
			}
		}
		
	}else{ // collider rigged
		// TODO implement similar to collider component
		return false;
	}
	
	return false;
}
#endif



bool debpCollisionDetection::ShapeHitsModelFace(debpShape &shape, debpComponent &component, int face){
	if(!component.GetComponent()->GetModel()){
		return false;
	}
	
	const deModel &engModel = *component.GetComponent()->GetModel();
	const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
	const deModelFace &engFace = engModelLOD.GetFaceAt(face);
	debpDCollisionTriangle collisionTriangle;
	
	const deModelVertex &v1 = engModelLOD.GetVertexAt(engFace.GetVertex1());
	const deModelVertex &v2 = engModelLOD.GetVertexAt(engFace.GetVertex2());
	const deModelVertex &v3 = engModelLOD.GetVertexAt(engFace.GetVertex3());
	
	const decDVector p1(v1.GetPosition());
	const decDVector p2(v2.GetPosition());
	const decDVector p3(v3.GetPosition());
	
	decDVector normal((p2 - p1) % (p3 - p2));
	double length = normal.Length();
	
	if(length > 1e-5){
		normal /= length;
		collisionTriangle.SetCorners(p1, p2, p3, normal);
		
		return shape.GetCollisionVolume()->TriangleHitsVolume(&collisionTriangle);
	}
	
	return false;
}



bool debpCollisionDetection::ShapeMoveHitsModelFace(debpShape &shape, const decDVector &displacement,
const debpComponent &component, int face, debpCollisionResult &result){
	if(!component.GetComponent()->GetModel()){
		return false;
	}
	
	const deModel &engModel = *component.GetComponent()->GetModel();
	const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
	const deModelFace &engFace = engModelLOD.GetFaceAt(face);
	
	const deModelVertex &v1 = engModelLOD.GetVertexAt(engFace.GetVertex1());
	const deModelVertex &v2 = engModelLOD.GetVertexAt(engFace.GetVertex2());
	const deModelVertex &v3 = engModelLOD.GetVertexAt(engFace.GetVertex3());
	
	const decDVector p1(v1.GetPosition());
	const decDVector p2(v2.GetPosition());
	const decDVector p3(v3.GetPosition());
	
	decDVector triNormal((p2 - p1) % (p3 - p2));
	double length = triNormal.Length();
	
	if(length > 1e-5){
		debpDCollisionTriangle collisionTriangle;
		decDVector hitNormal;
		float distance;
		
		triNormal /= length;
		collisionTriangle.SetCorners(p1, p2, p3, triNormal);
		
		// ignore faces which can not be hit due to the displacement direction
		if(displacement * triNormal > 1e-6) return false;
		
		// test shape for a collision
		distance = (float)shape.GetCollisionVolume()->VolumeMoveHitsVolume(&collisionTriangle, displacement, &hitNormal);
		
		// distances nearly 1 are considered no collision
		if(distance >= 0.99999f) return false;
		
		// sanity test
		if(displacement * hitNormal > -1e-6) return false;
		
		// we have got a collision
		result.normal = hitNormal;
		result.distance = distance;
		
		return true;
	}
	
	return false;
}

bool debpCollisionDetection::ColliderHitsTriangle(debpCollider *collider, const decDVector &p1,
const decDVector &p2, const decDVector &p3, debpCollisionResult &result){
	if(!collider) DETHROW(deeInvalidParam);
	
	debpDCollisionTriangle collisionTriangle;
	
	// set up the test triangle collision volume
	collisionTriangle.SetCorners(p1, p2, p3);
	
	// test depending on what kind of collider we have
	if(collider->IsVolume()){
		debpColliderVolume *bpcolvol = (debpColliderVolume*)collider;
		const debpShapeList &shapes = bpcolvol->GetShapes();
		int s, shapeCount = shapes.GetShapeCount();
		
		collider->UpdateShapes();
		
		for(s=0; s<shapeCount; s++){
			if(shapes.GetShapeAt(s)->GetCollisionVolume()->TriangleHitsVolume(&collisionTriangle)){
				result.shape1 = s;
				result.bone1 = -1;
				return true;
			}
		}
	}
	
	// no collision
	return false;
}

#if 0
bool debpCollisionDetection::ColliderMoveHitsTriangle(debpCollider *collider, const decDVector &displacement,
const decDVector &p1, const decDVector &p2, const decDVector &p3, debpCollisionResult &result){
	if(!collider) DETHROW(deeInvalidParam);
	
	debpDCollisionTriangle collisionTriangle;
	decDVector normal;
	float distance;
	
	// set up the test triangle collision volume
	collisionTriangle.SetCorners(p1, p2, p3);
	
	// ignore faces which can not be hit due to the displacement direction
	if(displacement * collisionTriangle.GetNormal() > 1e-6f) return false;
	
	// test depending on what kind of collider we have
	if(collider->IsVolume()){
		debpColliderVolume *bpcolvol = (debpColliderVolume*)collider;
		const debpShapeList &shapes = bpcolvol->GetShapes();
		int s, shapeCount = shapes.GetShapeCount();
		
		for(s=0; s<shapeCount; s++){
			distance = shapes.GetShapeAt(s)->GetCollisionVolume()->VolumeMoveHitsVolume(&collisionTriangle, displacement, &normal);
			
			// distances nearly 1 are considered no collision
			if(distance >= 0.99999f) continue;
			
			// sanity test
			if(displacement * normal > -1e-6f) continue;
			
			// we have got a collision
			result.shape1 = s;
			result.bone1 = -1;
			result.normal = normal;
			result.distance = distance;
			return true;
		}
	}
	
	// no collision
	return false;
}
#endif

bool debpCollisionDetection::ColliderMoveHitsModelFace(debpCollider *collider, const decDVector &displacement,
const debpComponent &component, int face, debpCollisionResult &result){
	if(!collider){
		DETHROW(deeInvalidParam);
	}
	
	if(!component.GetComponent()->GetModel()){
		return false;
	}
	
	const deModel &engModel = *component.GetComponent()->GetModel();
	const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
	const deModelFace &engFace = engModelLOD.GetFaceAt(face);
	
	const deModelVertex &v1 = engModelLOD.GetVertexAt(engFace.GetVertex1());
	const deModelVertex &v2 = engModelLOD.GetVertexAt(engFace.GetVertex2());
	const deModelVertex &v3 = engModelLOD.GetVertexAt(engFace.GetVertex3());
	
	const decDVector p1(v1.GetPosition());
	const decDVector p2(v2.GetPosition());
	const decDVector p3(v3.GetPosition());
	
	decDVector triNormal((p2 - p1) % (p3 - p2));
	double length = triNormal.Length();
	
	if(length > 1e-5){
		debpDCollisionTriangle collisionTriangle;
		decDVector hitNormal;
		float distance;
		
		triNormal /= length;
		collisionTriangle.SetCorners(p1, p2, p3, triNormal);
		
		// ignore faces which can not be hit due to the displacement direction
		if(displacement * triNormal > 1e-6) return false;
		
		// test depending on what kind of collider we have
		if(collider->IsVolume()){
			debpColliderVolume &bpcolvol = *((debpColliderVolume*)collider);
			const debpShapeList &shapes = bpcolvol.GetShapes();
			int s, shapeCount = shapes.GetShapeCount();
			
			bpcolvol.UpdateShapes();
			
			for(s=0; s<shapeCount; s++){
				distance = (float)shapes.GetShapeAt(s)->GetCollisionVolume()->VolumeMoveHitsVolume(&collisionTriangle, displacement, &hitNormal);
				
				// distances nearly 1 are considered no collision
				if(distance >= 0.99999f) continue;
				
				// sanity test
				if(displacement * hitNormal > -1e-6) continue;
				
				// we have got a collision
				result.shape1 = s;
				result.bone1 = -1;
				result.normal = hitNormal;
				result.distance = distance;
				return true;
			}
		}
	}
	
	return false;
}



#if 0
void debpCollisionDetection::GetColliderMoveBoundingBox(debpCollider &collider, const decDVector &displacement, debpDCollisionBox &box){
	// retrieve the shape extend of the collider
	decDVector minExtend = collider.GetShapeMinimumExtend();
	decDVector maxExtend = collider.GetShapeMaximumExtend();
	
	// enlarge the bounding box at the correct sides by the displacement
	if(displacement.x < 0.0){
		minExtend.x += displacement.x;
		
	}else{
		maxExtend.x += displacement.x;
	}
	
	if(displacement.y < 0.0){
		minExtend.y += displacement.y;
		
	}else{
		maxExtend.y += displacement.y;
	}
	
	if(displacement.z < 0.0){
		minExtend.z += displacement.z;
		
	}else{
		maxExtend.z += displacement.z;
	}
	
	// add a small offset to the boundaries to avoid misses
	minExtend.x -= 0.01;
	minExtend.y -= 0.01;
	minExtend.z -= 0.01;
	maxExtend.x += 0.01;
	maxExtend.y += 0.01;
	maxExtend.z += 0.01;
	
	box.SetFromExtends(minExtend, maxExtend);
}
#endif



#if 0
bool debpCollisionDetection::RayHitsColliderVolume(const decDVector &origin, const decDVector &direction,
debpColliderVolume &collider, debpCollisionResult &result){
	debpShapeList &shapes = collider.GetShapes();
	int s, shapeCount = shapes.GetShapeCount();
	double hitDist = 0.0;
	decDVector hitNormal;
	int hitShape = -1;
	double distance;
	
	collider.UpdateShapes();
	
	for(s=0; s<shapeCount; s++){
		debpDCollisionVolume &collisionVolume = *shapes.GetShapeAt(s)->GetCollisionVolume();
		
		if(collisionVolume.RayHitsVolume(origin, direction, distance)){
			if(hitShape == -1 || distance < hitDist){
				hitShape = s;
				hitDist = distance;
				hitNormal = collisionVolume.NormalAtPoint(origin + direction * distance);
			}
		}
	}
	
	if(hitShape != -1){
		result.shape2 = hitShape;
		result.distance = hitDist;
		result.normal = hitNormal;
		return true;
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::RayHitsColliderComponent(const decDVector &origin, const decDVector &direction,
debpColliderComponent &collider, debpCollisionResult &result){
	if(!collider.GetColliderComponent().GetComponent()){
		return false;
	}
	
	deColliderComponent &colcomp = collider.GetColliderComponent();
	deComponent &component = *colcomp.GetComponent();
	int testMode = collider.GetTestMode();
	debpDCollisionTriangle colTri;
	debpDCollisionBox colBox;
	int s, shapeCount;
	int b, boneCount;
	int f, faceCount;
	
	decDVector p1, p2, p3, normal;
	double distance;
	double length;
	
	double hitDist = 0.0;
	decDVector hitNormal;
	bool hasHit = false;
	int hitShape = -1;
	int hitBone = -1;
	//int hitFace = -1;
	
	// determine how to test for collision
	if(testMode == debpColliderComponent::etmModelStatic || testMode == debpColliderComponent::etmModelDynamic){
		// the following check is brutal hack for the time beeing.
		// there are a couple of problems not adressed yet. for
		// one the extends calculated by the component are in world
		// coordinates instead of local to the bone. and second there
		// exists no octree yet or another form of speedup. hence we
		// use a brute force check here.
		if(!component.GetModel()){
			return false;
		}
		
		const deModel &model = *component.GetModel();
		const deModelLOD &modelLOD = *model.GetLODAt(0);
		
		// check if we hit the bounding box of the component
		component.PrepareMesh();
		
		colBox.SetFromExtends(component.GetMinimumExtend(), component.GetMaximumExtend());
		colBox.MoveBy(component.GetPosition());
		
		if(colBox.RayHitsVolume(origin, direction, distance)){
			// check model faces
			// TODO transform the ray origin and direction instead of the vertices
			
			const decMatrix &matrix = component.GetMatrix();
			faceCount = modelLOD.GetFaceCount();
			for(f=0; f<faceCount; f++){
				const deModelFace &face = modelLOD.GetFaceAt(f);
				
				// DEPRECATED TODO Remove GetVertex from deComponent !!!
				// NOTE in all cases the deModelLOD data needs to be retained once retaining is used.
				//      for the calculation if the model is static the deModelLOD vertices can be used
				//      directly. for the dynamic case the calculation has to be triggered here and
				//      cached in the debpComponent object
				p1 = matrix * component.GetVertex(face.GetVertex1());
				p2 = matrix * component.GetVertex(face.GetVertex2());
				p3 = matrix * component.GetVertex(face.GetVertex3());
				
				normal = (p2 - p1) % (p3 - p2);
				length = normal.Length();
				if(length > FLOAT_EPSILON){
					normal /= length;
					
					distance = debpDCollisionDetection::RayPlane(p1, normal, origin, direction);
					if(distance >= 0.0){
						if(!hasHit || distance < hitDist){
							if(debpDCollisionDetection::PointInTriangle(p1, p2, p3, origin + direction * distance)){
								hitShape = -1;
								hitBone = -1;
								//hitFace = f;
								hitDist = distance;
								hitNormal = normal;
								hasHit = true;
							}
						}
					}
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmRigShape){
		debpShapeList &shapes = collider.GetRigShapes();
		shapeCount = shapes.GetShapeCount();
		
		collider.UpdateShapes();
		
		for(s=0; s<shapeCount; s++){
			debpDCollisionVolume &collisionVolume = *shapes.GetShapeAt(s)->GetCollisionVolume();
			
			if(collisionVolume.RayHitsVolume(origin, direction, distance)){
				if(!hasHit || distance < hitDist){
					hitShape = s;
					hitBone = -1;
					//hitFace = -1;
					hitDist = distance;
					hitNormal = collisionVolume.NormalAtPoint(origin + direction * distance);
					hasHit = true;
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmBoneShape){
		collider.UpdateShapes();
		boneCount = collider.GetBoneCount();
		
		for(b=0; b<boneCount; b++){
			debpColliderBone *bpbone = collider.GetBoneAt(b);
			if(!bpbone){
				continue;
			}
			
			debpShapeList &shapes = bpbone->GetShapes();
			shapeCount = shapes.GetShapeCount();
			
			for(s=0; s<shapeCount; s++){
				debpDCollisionVolume &collisionVolume = *shapes.GetShapeAt(s)->GetCollisionVolume();
				
				if(collisionVolume.RayHitsVolume(origin, direction, distance)){
					if(!hasHit || distance < hitDist){
						hitShape = s;
						hitBone = b;
						//hitFace = -1;
						hitDist = distance;
						hitNormal = collisionVolume.NormalAtPoint(origin + direction * distance);
						hasHit = true;
					}
				}
			}
		}
	}
	
	if(hasHit){
		result.bone2 = hitBone;
		result.shape2 = hitShape;
		//result.face = hitFace; // requires a new face parameter for shapes only
		result.distance = hitDist;
		result.normal = hitNormal;
		return true;
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::RayHitsColliderRig(const decDVector &origin, const decDVector &direction,
debpColliderRig &collider, debpCollisionResult &result){
	const int testMode = collider.GetTestMode();
	debpDCollisionBox colBox;
	int s, shapeCount;
	int b, boneCount;
	
	decDVector p1, p2, p3, normal;
	double distance;
	
	double hitDist = 0.0;
	decDVector hitNormal;
	bool hasHit = false;
	int hitShape = -1;
	int hitBone = -1;
	
	// determine how to test for collision
	if(testMode == debpColliderComponent::etmRigShape){
		const debpShapeList &shapes = collider.GetRigShapes();
		shapeCount = shapes.GetShapeCount();
		
		collider.UpdateShapes();
		
		for(s=0; s<shapeCount; s++){
			debpDCollisionVolume &collisionVolume = *shapes.GetShapeAt(s)->GetCollisionVolume();
			
			if(collisionVolume.RayHitsVolume(origin, direction, distance)){
				if(!hasHit || distance < hitDist){
					hitShape = s;
					hitBone = -1;
					hitDist = distance;
					hitNormal = collisionVolume.NormalAtPoint(origin + direction * distance);
					hasHit = true;
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmBoneShape){
		debpColliderBone *bpbone;
		
		collider.UpdateShapes();
		boneCount = collider.GetBoneCount();
		
		for(b=0; b<boneCount; b++){
			bpbone = collider.GetBoneAt(b);
			if(!bpbone){
				continue;
			}
			
			const debpShapeList &shapes = bpbone->GetShapes();
			shapeCount = shapes.GetShapeCount();
			
			for(s=0; s<shapeCount; s++){
				debpDCollisionVolume &collisionVolume = *shapes.GetShapeAt(s)->GetCollisionVolume();
				
				if(collisionVolume.RayHitsVolume(origin, direction, distance)){
					if(!hasHit || distance < hitDist){
						hitShape = s;
						hitBone = b;
						hitDist = distance;
						hitNormal = collisionVolume.NormalAtPoint(origin + direction * distance);
						hasHit = true;
					}
				}
			}
		}
	}
	
	if(hasHit){
		result.bone2 = hitBone;
		result.shape2 = hitShape;
		result.distance = hitDist;
		result.normal = hitNormal;
		return true;
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::ShapeHitsColliderVolume(debpShape &shape,
debpColliderVolume &collider, debpCollisionResult &result){
	debpDCollisionVolume &collisionVolume = *shape.GetCollisionVolume();
	const debpShapeList &shapes = collider.GetShapes();
	int s, shapeCount = shapes.GetShapeCount();
	debpShape *cvshape;
	
	collider.UpdateShapes();
	
	for(s=0; s<shapeCount; s++){
		cvshape = shapes.GetShapeAt(s);
		
		if(collisionVolume.VolumeHitsVolume(cvshape->GetCollisionVolume())){
			result.bone2 = -1;
			result.face = -1;
			result.shape2 = s;
			return true;
		}
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::ShapeHitsColliderComponent(debpShape &shape, debpColliderComponent &collider, debpCollisionResult &result){
	debpDCollisionVolume &collisionVolume = *shape.GetCollisionVolume();
	deColliderComponent &colcomp = collider.GetColliderComponent();
	int testMode = collider.GetTestMode();
	debpDCollisionTriangle colTri;
	debpDCollisionBox colBox;
	
	// determine how to test for collision
	if(testMode == debpColliderComponent::etmModelStatic){
		/*
		debpCDVHitModelFace visitor(this);
		debpDCollisionBox box;
		
		component->PrepareMesh();
		
		shape->UpdateWithMatrix(shape???->GetMatrix() * collider->GetInverseMatrix());
		visitor.SetComponent((debpComponent*)component->GetPhysicsComponent());
		visitor.SetTestShape(shape);
		
		shape->GetCollisionVolume()->GetEnclosingBox(&box);
		((debpModel*)component->GetModel()->GetPeerPhysics())->GetOctree()->VisitNodesColliding(&visitor, &box);
		
		if(visitor.HasCollision()){
			result.face = visitor.GetResult().face;
			return true;
		}
		*/
		
		deComponent &component = *colcomp.GetComponent();
		if(!component.GetModel()){
			return false;
		}
		
		const deModel &model = *component.GetModel();
		const deModelLOD &modelLOD = *model.GetLODAt(0);
		int f, faceCount;
		
		component.PrepareMesh();
		
		colBox.SetFromExtends(component.GetMinimumExtend(), component.GetMaximumExtend());
		colBox.MoveBy(component.GetPosition());
		
		if(collisionVolume.BoxHitsVolume(&colBox)){
			// check model faces
			const decMatrix &matrix = component.GetMatrix();
			faceCount = modelLOD.GetFaceCount();
			for(f=0; f<faceCount; f++){
				const deModelFace &face = modelLOD.GetFaceAt(f);
				const decDVector fp1(matrix * component.GetVertex(face.GetVertex1()));
				const decDVector fp2(matrix * component.GetVertex(face.GetVertex2()));
				const decDVector fp3(matrix * component.GetVertex(face.GetVertex3()));
				decDVector normal((fp2 - fp1) % (fp3 - fp2));
				double length = normal.Length();
				
				if(length > 1e-5){ // only test against non-degenrated triangles
					normal /= length;
					colTri.SetCorners(fp1, fp2, fp3, normal);
					
					if(collisionVolume.TriangleHitsVolume(&colTri)){
						result.shape2 = -1;
						result.bone2 = -1;
						result.face = f;
						return true;
					}
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmModelDynamic){
		deComponent &component = *colcomp.GetComponent();
		if(!component.GetModel()){
			return false;
		}
		
		const deModel &model = *component.GetModel();
		const deModelLOD &modelLOD = *model.GetLODAt(0);
		int f, faceCount;
		
		component.PrepareMesh();
		
		colBox.SetFromExtends(component.GetMinimumExtend(), component.GetMaximumExtend());
		colBox.MoveBy(component.GetPosition());
		
		if(collisionVolume.BoxHitsVolume(&colBox)){
			// check model faces
			const decMatrix &matrix = component.GetMatrix();
			faceCount = modelLOD.GetFaceCount();
			for(f=0; f<faceCount; f++){
				const deModelFace &face = modelLOD.GetFaceAt(f);
				const decDVector fp1(matrix * component.GetVertex(face.GetVertex1()));
				const decDVector fp2(matrix * component.GetVertex(face.GetVertex2()));
				const decDVector fp3(matrix * component.GetVertex(face.GetVertex3()));
				decDVector normal((fp2 - fp1) % (fp3 - fp2));
				double length = normal.Length();
				
				if(length > 1e-5){ // only test against non-degenrated triangles
					normal /= length;
					colTri.SetCorners(fp1, fp2, fp3, normal);
					
					if(collisionVolume.TriangleHitsVolume(&colTri)){
						result.shape2 = -1;
						result.bone2 = -1;
						result.face = f;
						return true;
					}
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmRigShape){
		debpShapeList &shapes = collider.GetRigShapes();
		int s, shapeCount = shapes.GetShapeCount();
		
		collider.UpdateShapes();
		
		for(s=0; s<shapeCount; s++){
			if(collisionVolume.VolumeHitsVolume(shapes.GetShapeAt(s)->GetCollisionVolume())){
				result.bone2 = -1;
				result.face = -1;
				result.shape2 = s;
				return true;
			}
		}
		
	}else if(testMode == debpColliderComponent::etmBoneShape){
		int b, boneCount = collider.GetBoneCount();
		int s, shapeCount;
		
		collider.UpdateShapes();
		
		for(b=0; b<boneCount; b++){
			debpColliderBone * const cbone = collider.GetBoneAt(b);
			if(!cbone){
				continue;
			}
			
			const debpShapeList &shapes = cbone->GetShapes();
			shapeCount = shapes.GetShapeCount();
			
			for(s=0; s<shapeCount; s++){
				if(collisionVolume.VolumeHitsVolume(shapes.GetShapeAt(s)->GetCollisionVolume())){
					result.shape2 = -1;
					result.face = -1;
					result.bone2 = b;
					return true;
				}
			}
		}
	}
	
	return false;
}
#endif

#if 0
bool debpCollisionDetection::ShapeHitsColliderRig(debpShape &shape, debpColliderRig &collider, debpCollisionResult &result){
	debpDCollisionVolume &collisionVolume = *shape.GetCollisionVolume();
	int testMode = collider.GetTestMode();
	debpDCollisionBox colBox;
	
	// determine how to test for collision
	if(testMode == debpColliderComponent::etmRigShape){
		debpShapeList &shapes = collider.GetRigShapes();
		int s, shapeCount = shapes.GetShapeCount();
		
		collider.UpdateShapes();
		
		for(s=0; s<shapeCount; s++){
			if(collisionVolume.VolumeHitsVolume(shapes.GetShapeAt(s)->GetCollisionVolume())){
				result.bone2 = -1;
				result.face = -1;
				result.shape2 = s;
				return true;
			}
		}
		
	}else if(testMode == debpColliderComponent::etmBoneShape){
		int b, boneCount = collider.GetBoneCount();
		debpColliderBone *cbone;
		int s, shapeCount;
		
		collider.UpdateShapes();
		
		for(b=0; b<boneCount; b++){
			cbone = collider.GetBoneAt(b);
			
			if(cbone){
				const debpShapeList &shapes = cbone->GetShapes();
				shapeCount = shapes.GetShapeCount();
				
				for(s=0; s<shapeCount; s++){
					if(collisionVolume.VolumeHitsVolume(shapes.GetShapeAt(s)->GetCollisionVolume())){
						result.shape2 = -1;
						result.face = -1;
						result.bone2 = b;
						return true;
					}
				}
			}
		}
	}
	
	return false;
}
#endif



bool debpCollisionDetection::ColliderVolumeHitsColliderVolume(debpColliderVolume &collider1,
debpColliderVolume &collider2, debpCollisionResult &result){
	debpShapeList &shapes1 = collider1.GetShapes();
	debpShapeList &shapes2 = collider2.GetShapes();
	int s1, shapeCount1 = shapes1.GetShapeCount();
	int s2, shapeCount2 = shapes2.GetShapeCount();
	debpDCollisionVolume *collisionVolume;
	debpShape *shape1, *shape2;
	
	collider1.UpdateShapes();
	collider2.UpdateShapes();
	
	for(s1=0; s1<shapeCount1; s1++){
		shape1 = shapes1.GetShapeAt(s1);
		collisionVolume = shape1->GetCollisionVolume();
		
		for(s2=0; s2<shapeCount2; s2++){
			shape2 = shapes2.GetShapeAt(s2);
			
			if(collisionVolume->VolumeHitsVolume(shape2->GetCollisionVolume())){
				result.shape1 = s1;
				result.shape2 = s2;
				return true;
			}
		}
	}
	
	return false;
}

bool debpCollisionDetection::ColliderVolumeHitsColliderComponent(debpColliderVolume &collider1,
debpColliderComponent &collider2, debpCollisionResult &result){
	if(!collider2.GetColliderComponent().GetComponent()){
		return false;
	}
	
	debpComponent &component = *collider2.GetLinkedComponent();
	const debpShapeList &shapes1 = collider1.GetShapes();
	int s1, shapeCount1 = shapes1.GetShapeCount();
	int testMode = collider2.GetTestMode();
	
	// test against triangles directly
	if(testMode == debpColliderComponent::etmModelStatic){
		if(!component.GetModel()){
			return false;
		}
		
		SPECIAL_DEBUG(pBullet.LogInfoFormat("ColliderVolumeHitsColliderComponent: Static Model: %p %p %s\n",
			collider1.GetParentWorld(), &collider1, collider2.GetColliderComponent()->GetComponent()->GetModel()
				? collider2.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-"));
		debpCDVHitModelFace visitor(this);
		debpDCollisionBox box;
		
		component.PrepareMesh();
		
		collider1.UpdateShapesWithMatrix(collider1.GetMatrix().QuickMultiply(collider2.GetInverseMatrix()));
		
		visitor.SetComponent(&component);
		visitor.SetTestCollider(&collider1);
		
		box.SetFromExtends(collider1.GetShapeMinimumExtend(), collider1.GetShapeMaximumExtend());
		component.GetModel()->GetOctree()->VisitNodesColliding(&visitor, &box);
		
		if(visitor.HasCollision()){
			debpCollisionResult &vresult = visitor.GetResult();
			
			result.shape1 = vresult.shape1;
			result.shape2 = -1;
			result.face = vresult.face;
			result.bone2 = -1;
			
			return true;
		}
		
	}else if(testMode == debpColliderComponent::etmModelDynamic){
		if(!component.GetModel()){
			return false;
		}
		
		SPECIAL_DEBUG(pBullet.LogInfoFormat("ColliderVolumeHitsColliderComponent: Dynamic Model: %p %p %s\n",
			collider1.GetParentWorld(), &collider1, collider2.GetColliderComponent()->GetComponent()->GetModel()
				? collider2.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-"));
		const decMatrix &matrix = component.GetComponent()->GetMatrix();
		const deModel &engModel = component.GetModel()->GetModel();
		const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
		int f, faceCount = engModelLOD.GetFaceCount();
		debpDCollisionTriangle coltri;
		debpDCollisionBox colbox;
		
		component.PrepareMesh();
		component.PrepareExtends();
		
		colbox.SetFromExtends(component.GetMinimumExtend(), component.GetMaximumExtend());
		colbox.MoveBy(component.GetComponent()->GetPosition());
		
		collider1.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			if(colvol1.BoxHitsVolume(&colbox)){
				for(f=0; f<faceCount; f++){
					const deModelFace &face = engModelLOD.GetFaceAt(f);
					const decDVector fp1(matrix * component.GetVertex(face.GetVertex1()));
					const decDVector fp2(matrix * component.GetVertex(face.GetVertex2()));
					const decDVector fp3(matrix * component.GetVertex(face.GetVertex3()));
					decDVector normal((fp2 - fp1) % (fp3 - fp2));
					double length = normal.Length();
					
					if(length > 1e-5){ // only test against non-degenrated triangles
						normal /= length;
						coltri.SetCorners(fp1, fp2, fp3, normal);
						
						if(colvol1.TriangleHitsVolume(&coltri)){
							result.shape1 = s1;
							result.shape2 = -1;
							result.face = f;
							result.bone2 = -1;
							
							return true;
						}
					}
				}
			}
		}
		
	// test against rig shapes
	}else if(testMode == debpColliderComponent::etmRigShape){
		const debpShapeList &shapes2 = collider2.GetRigShapes();
		int s2, shapeCount2 = shapes2.GetShapeCount();
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(s2=0; s2<shapeCount2; s2++){
				if(colvol1.VolumeHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume())){
					result.shape1 = s1;
					result.shape2 = s2;
					result.face = -1;
					result.bone2 = -1;
					
					return true;
				}
			}
		}
		
	// test against bone shapes
	}else if(testMode == debpColliderComponent::etmBoneShape){
		if(!collider2.GetBones()){
			return false;
		}
		
		const debpColliderBones &bones2 = *collider2.GetBones();
		int b, boneCount = bones2.GetBonePhysicsCount();
		int s2, shapeCount2;
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(b=0; b<boneCount; b++){
				const debpShapeList &shapes2 = bones2.GetBonePhysicsAt(b).GetShapes();
				shapeCount2 = shapes2.GetShapeCount();
				
				for(s2=0; s2<shapeCount2; s2++){
					if(colvol1.VolumeHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume())){
						result.shape1 = s1;
						result.shape2 = s2;
						result.face = -1;
						result.bone2 = b;
						
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

bool debpCollisionDetection::ColliderVolumeHitsColliderRig(debpColliderVolume &collider1,
debpColliderRig &collider2, debpCollisionResult &result){
	const debpShapeList &shapes1 = collider1.GetShapes();
	int s1, shapeCount1 = shapes1.GetShapeCount();
	int testMode = collider2.GetTestMode();
	
	// test against rig shapes
	if(testMode == debpColliderComponent::etmRigShape){
		const debpShapeList &shapes2 = collider2.GetRigShapes();
		int s2, shapeCount2 = shapes2.GetShapeCount();
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(s2=0; s2<shapeCount2; s2++){
				if(colvol1.VolumeHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume())){
					result.shape1 = s1;
					result.shape2 = s2;
					result.face = -1;
					result.bone2 = -1;
					
					return true;
				}
			}
		}
		
	// test against bone shapes
	}else if(testMode == debpColliderComponent::etmBoneShape){
		if(!collider2.GetBones()){
			return false;
		}
		
		const debpColliderBones &bones2 = *collider2.GetBones();
		int b, boneCount = bones2.GetBonePhysicsCount();
		int s2, shapeCount2;
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(b=0; b<boneCount; b++){
				debpColliderBone &colbone = bones2.GetBonePhysicsAt(b);
				const debpShapeList &shapes2 = colbone.GetShapes();
				shapeCount2 = shapes2.GetShapeCount();
				
				for(s2=0; s2<shapeCount2; s2++){
					if(colvol1.VolumeHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume())){
						result.shape1 = s1;
						result.shape2 = s2;
						result.face = -1;
						result.bone2 = b;
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

bool debpCollisionDetection::ColliderComponentHitsColliderComponent(debpColliderComponent &collider1,
debpColliderComponent &collider2, debpCollisionResult &result){
	printf("*** TODO *** debpCollisionDetection.ColliderComponentHitsColliderComponent\n");
	// TODO implementation
	return false;
}

bool debpCollisionDetection::ColliderComponentHitsColliderRig(debpColliderComponent &collider1,
debpColliderRig &collider2, debpCollisionResult &result){
	printf("*** TODO *** debpCollisionDetection.ColliderComponentHitsColliderRig\n");
	// TODO implementation
	return false;
}



#if 0
bool debpCollisionDetection::ColliderVolumeMoveHitsColliderVolume(debpColliderVolume &collider1, const decDVector &displacement,
debpColliderVolume &collider2, debpCollisionResult &result){
	debpShapeList &shapes1 = collider1.GetShapes();
	debpShapeList &shapes2 = collider2.GetShapes();
	int s1, shapeCount1 = shapes1.GetShapeCount();
	int s2, shapeCount2 = shapes2.GetShapeCount();
	debpDCollisionVolume *collisionVolume;
	debpShape *shape1, *shape2;
	bool hasHit = false;
	decDVector normal;
	float distance;
	
	collider1.UpdateShapes();
	collider2.UpdateShapes();
	
	decDVector reldisp = displacement - collider2.GetPredictedDisplacement();
	
	for(s1=0; s1<shapeCount1; s1++){
		shape1 = shapes1.GetShapeAt(s1);
		collisionVolume = shape1->GetCollisionVolume();
		
		for(s2=0; s2<shapeCount2; s2++){
			shape2 = shapes2.GetShapeAt(s2);
			
			distance = collisionVolume->VolumeMoveHitsVolume(shape2->GetCollisionVolume(), reldisp, &normal);
			
			if(distance > 0.99999f) continue;
			if(displacement * normal > -1e-6f) continue;
			
			if(!hasHit || distance < result.distance){
				result.shape1 = s1;
				result.shape2 = s2;
				result.distance = distance;
				result.normal = normal;
				hasHit = true;
			}
		}
	}
	
	return hasHit;
}
#endif

#if 0
bool debpCollisionDetection::ColliderVolumeMoveHitsColliderComponent(debpColliderVolume &collider1,
const decDVector &displacement, debpColliderComponent &collider2, debpCollisionResult &result){
	if(!collider2.GetColliderComponent().GetComponent()){
		return false;
	}
	
	const deColliderComponent &engCollider2 = collider2.GetColliderComponent();
	deComponent &engComponent = *engCollider2.GetComponent();
	const debpShapeList &shapes1 = collider1.GetShapes();
	int s1, shapeCount1 = shapes1.GetShapeCount();
	int testMode = collider2.GetTestMode();
	decDVector hitNormal;
	bool hasHit = false;
	float distance;
	
	const decDVector reldisp = displacement; // - bpcollider->GetPredictedDisplacement();
	
	if(testMode == debpColliderComponent::etmModelStatic){
		if(!engComponent.GetModel()){
			return false;
		}
		
		SPECIAL_DEBUG(pBullet.LogInfoFormat("ColliderVolumeMoveHitsColliderComponent: Static Model: %p %p %s\n",
			collider1.GetParentWorld(), &collider1, collider2.GetColliderComponent()->GetComponent()->GetModel()
				? collider2.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-"));
		const decDVector localdisp = collider2.GetInverseMatrix().TransformNormal(reldisp);
		debpCDVMoveHitModelFace visitor(this);
		debpDCollisionBox box;
		
		collider1.UpdateShapesWithMatrix(collider1.GetMatrix() * collider2.GetInverseMatrix());
		
		visitor.SetComponent((debpComponent*)engComponent.GetPhysicsComponent());
		visitor.SetTestCollider(&collider1, localdisp);
		
		GetColliderMoveBoundingBox(collider1, localdisp, box);
		((debpModel*)engComponent.GetModel()->GetPeerPhysics())->GetOctree()->VisitNodesColliding(&visitor, &box);
		
		if(visitor.HasCollision()){
			const debpCollisionResult &vresult = visitor.GetResult();
			
			result.shape1 = vresult.shape1;
			result.shape2 = -1;
			result.bone1 = vresult.bone1;
			result.bone2 = -1;
			result.face = vresult.face;
			result.distance = vresult.distance;
			result.normal = collider2.GetMatrix().TransformNormal(vresult.normal);
			hasHit = true;
		}
		
	}else if(testMode == debpColliderComponent::etmModelDynamic){
		if(!engComponent.GetModel()){
			return false;
		}
		
		SPECIAL_DEBUG(pBullet.LogInfoFormat("ColliderVolumeMoveHitsColliderComponent: Dynamic Model: %p %p %s\n",
			collider1.GetParentWorld(), &collider1, collider2.GetColliderComponent()->GetComponent()->GetModel()
				? collider2.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-"));
		const deModel &engModel = *engComponent.GetModel();
		const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
		const decMatrix &matrix = engComponent.GetMatrix();
		int f, faceCount = engModelLOD.GetFaceCount();
		decDVector fp1, fp2, fp3, triNormal;
		debpDCollisionTriangle colTri;
		debpDCollisionBox colBox;
		double length;
		
		engComponent.PrepareMesh();
		
		colBox.SetFromExtends(engComponent.GetMinimumExtend(), engComponent.GetMaximumExtend());
		colBox.MoveBy(engComponent.GetPosition());
		
		collider1.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol = *shape1.GetCollisionVolume();
			
			distance = colvol.VolumeMoveHitsVolume(&colBox, reldisp, NULL);
			if(distance > 0.99999f) continue;
			
			for(f=0; f<faceCount; f++){
				const deModelFace &face = engModelLOD.GetFaceAt(f);
				fp1 = matrix * engComponent.GetVertex(face.GetVertex1());
				fp2 = matrix * engComponent.GetVertex(face.GetVertex2());
				fp3 = matrix * engComponent.GetVertex(face.GetVertex3());
				triNormal = (fp2 - fp1) % (fp3 - fp2);
				length = triNormal.Length();
				
				if(length > 1e-5){ // only test against non-degenrated triangles
					triNormal /= length;
					colTri.SetCorners(fp1, fp2, fp3, triNormal);
					
					distance = colvol.VolumeMoveHitsVolume(&colTri, reldisp, &hitNormal);
					if(distance > 0.99999f) continue;
					if(displacement * hitNormal >= -1e-6) continue;
					
					if(!hasHit || distance < result.distance){
						result.shape1 = s1;
						result.shape2 = -1;
						result.bone1 = -1; //bone1;
						result.bone2 = -1;
						result.face = f;
						result.distance = distance;
						result.normal = hitNormal;
						hasHit = true;
					}
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmRigShape){
		const debpShapeList &shapes2 = collider2.GetRigShapes();
		int s2, shapeCount2 = shapes2.GetShapeCount();
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(s2=0; s2<shapeCount2; s2++){
				distance = (float)colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
				
				if(distance > 0.99999f) continue;
				if(displacement * hitNormal >= -1e-6) continue;
				
				if(!hasHit || distance < result.distance){
					result.shape1 = s1;
					result.shape2 = s2;
					result.bone1 = -1; //bone1;
					result.bone2 = -1;
					result.face = -1;
					result.distance = distance;
					result.normal = hitNormal;
					hasHit = true;
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmBoneShape){
		int b, boneCount = collider2.GetBoneCount();
		debpColliderBone *colbone;
		int s2, shapeCount2;
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(b=0; b<boneCount; b++){
				colbone = collider2.GetBoneAt(b);
				
				if(colbone){
					const debpShapeList &shapes2 = colbone->GetShapes();
					shapeCount2 = shapes2.GetShapeCount();
					
					for(s2=0; s2<shapeCount2; s2++){
						distance = colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
						
						if(distance > 0.99999f) continue;
						if(displacement * hitNormal >= -1e-6) continue;
						
						if(!hasHit || distance < result.distance){
							result.shape1 = s1;
							result.shape2 = s2;
							result.bone1 = -1; // bone1;
							result.bone2 = b;
							result.face = -1;
							result.distance = distance;
							result.normal = hitNormal;
							hasHit = true;
						}
					}
				}
			}
		}
	}
	
	/*if( hasHit ){
		printf("has hit: s1=%i s2=%i b1=%i b2=%i f=%i d=%g n=(%g,%g,%g)\n", result.shape1, result.shape2, result.bone1, result.bone2, result.face,
		result.distance, result.normal.x, result.normal.y, result.normal.z);
	}*/
	
	return hasHit;
}
#endif

#if 0
bool debpCollisionDetection::ColliderVolumeMoveHitsColliderRig(debpColliderVolume &collider1, const decDVector &displacement,
debpColliderRig &collider2, debpCollisionResult &result){
	const debpShapeList &shapes1 = collider1.GetShapes();
	int s1, shapeCount1 = shapes1.GetShapeCount();
	int testMode = collider2.GetTestMode();
	decDVector hitNormal;
	bool hasHit = false;
	float distance;
	
	const decDVector reldisp(displacement); // - bpcollider->GetPredictedDisplacement();
	
	if(testMode == debpColliderComponent::etmRigShape){
		const debpShapeList &shapes2 = collider2.GetRigShapes();
		int s2, shapeCount2 = shapes2.GetShapeCount();
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(s2=0; s2<shapeCount2; s2++){
				distance = colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
				
				if(distance > 0.99999f) continue;
				if(displacement * hitNormal >= -1e-6) continue;
				
				if(!hasHit || distance < result.distance){
					result.shape1 = s1;
					result.shape2 = s2;
					result.bone1 = -1; //bone1;
					result.bone2 = -1;
					result.face = -1;
					result.distance = distance;
					result.normal = hitNormal;
					hasHit = true;
				}
			}
		}
		
	}else if(testMode == debpColliderComponent::etmBoneShape){
		int b, boneCount = collider2.GetBoneCount();
		debpColliderBone *colbone;
		int s2, shapeCount2;
		
		collider1.UpdateShapes();
		collider2.UpdateShapes();
		
		for(s1=0; s1<shapeCount1; s1++){
			const debpShape &shape1 = *shapes1.GetShapeAt(s1);
			debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
			
			for(b=0; b<boneCount; b++){
				colbone = collider2.GetBoneAt(b);
				
				if(colbone){
					const debpShapeList &shapes2 = colbone->GetShapes();
					shapeCount2 = shapes2.GetShapeCount();
					
					for(s2=0; s2<shapeCount2; s2++){
						distance = colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
						
						if(distance > 0.99999f) continue;
						if(displacement * hitNormal >= -1e-6) continue;
						
						if(!hasHit || distance < result.distance){
							result.shape1 = s1;
							result.shape2 = s2;
							result.bone1 = -1; // bone1;
							result.bone2 = b;
							result.face = -1;
							result.distance = distance;
							result.normal = hitNormal;
							hasHit = true;
						}
					}
				}
			}
		}
	}
	
	return hasHit;
}
#endif

#if 0
bool debpCollisionDetection::ColliderComponentMoveHitsColliderComponent(debpColliderComponent &collider1, const decDVector &displacement,
debpColliderComponent &collider2, debpCollisionResult &result){
	if(!collider1.GetColliderComponent().GetComponent() || !collider2.GetColliderComponent().GetComponent()){
		return false;
	}
	
	// test rig shapes against collider component
	if(collider1.GetTestMode() == debpColliderComponent::etmRigShape){
		const deColliderComponent &engCollider2 = collider2.GetColliderComponent();
		deComponent &engComponent = *engCollider2.GetComponent();
		const debpShapeList &shapes1 = collider1.GetRigShapes();
		int s1, shapeCount1 = shapes1.GetShapeCount();
		int testMode = collider2.GetTestMode();
		decDVector hitNormal;
		bool hasHit = false;
		float distance;
		
		const decDVector reldisp(displacement); // - bpcollider->GetPredictedDisplacement();
		
		if(testMode == debpColliderComponent::etmModelStatic){
			if(!engComponent.GetModel()){
				return false;
			}
			
			SPECIAL_DEBUG(pBullet.LogInfoFormat("ColliderComponentMoveHitsColliderComponent: Static Model: %p %s %s\n",
				collider1.GetParentWorld(), collider1.GetColliderComponent()->GetComponent()->GetModel()
					? collider1.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-",
				collider2.GetColliderComponent()->GetComponent()->GetModel()
					? collider2.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-"));
			debpCDVMoveHitModelFace visitor(this);
			debpDCollisionBox box;
			
			collider1.UpdateShapesWithMatrix(collider1.GetMatrix() * collider2.GetInverseMatrix());
			
			visitor.SetComponent((debpComponent*)engComponent.GetPhysicsComponent());
			visitor.SetTestCollider(&collider1, displacement);
			
			GetColliderMoveBoundingBox(collider1, displacement, box);
			((debpModel*)engComponent.GetModel()->GetPeerPhysics())->GetOctree()->VisitNodesColliding(&visitor, &box);
			
			collider1.UpdateShapes();
			
			if(visitor.HasCollision()){
				const debpCollisionResult &vresult = visitor.GetResult();
				
				result.shape1 = vresult.shape1;
				result.shape2 = -1;
				result.bone1 = vresult.bone1;
				result.bone2 = -1;
				result.face = vresult.face;
				result.distance = vresult.distance;
				result.normal = collider2.GetMatrix().TransformNormal(vresult.normal);
				hasHit = true;
			}
			
		}else if(testMode == debpColliderComponent::etmModelDynamic){
			if(!engComponent.GetModel()){
				return false;
			}
			
			SPECIAL_DEBUG(pBullet.LogInfoFormat("ColliderComponentMoveHitsColliderComponent: Dynamic Model: %p %s %s\n",
				collider1.GetParentWorld(), collider1.GetColliderComponent()->GetComponent()->GetModel()
					? collider1.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-",
				collider2.GetColliderComponent()->GetComponent()->GetModel()
					? collider2.GetColliderComponent()->GetComponent()->GetModel()->GetFilename() : "-"));
			const deModel &engModel = *engComponent.GetModel();
			const deModelLOD &engModelLOD = *engModel.GetLODAt(0);
			const decMatrix &matrix = engComponent.GetMatrix();
			int f, faceCount = engModelLOD.GetFaceCount();
			decDVector fp1, fp2, fp3, triNormal;
			debpDCollisionTriangle colTri;
			debpDCollisionBox colBox;
			double length;
			
			engComponent.PrepareMesh();
			
			colBox.SetFromExtends(engComponent.GetMinimumExtend(), engComponent.GetMaximumExtend());
			colBox.MoveBy(engComponent.GetPosition());
			
			collider1.UpdateShapes();
			
			for(s1=0; s1<shapeCount1; s1++){
				const debpShape &shape1 = *shapes1.GetShapeAt(s1);
				debpDCollisionVolume &colvol = *shape1.GetCollisionVolume();
				
				distance = colvol.VolumeMoveHitsVolume(&colBox, reldisp, NULL);
				if(distance > 0.99999f) continue;
				
				for(f=0; f<faceCount; f++){
					const deModelFace &face = engModelLOD.GetFaceAt(f);
					fp1 = matrix * engComponent.GetVertex(face.GetVertex1());
					fp2 = matrix * engComponent.GetVertex(face.GetVertex2());
					fp3 = matrix * engComponent.GetVertex(face.GetVertex3());
					triNormal = (fp2 - fp1) % (fp3 - fp2);
					length = triNormal.Length();
					
					if(length > 1e-5){ // only test against non-degenrated triangles
						triNormal /= length;
						colTri.SetCorners(fp1, fp2, fp3, triNormal);
						
						distance = colvol.VolumeMoveHitsVolume(&colTri, reldisp, &hitNormal);
						if(distance > 0.99999f) continue;
						if(displacement * hitNormal >= -1e-6) continue;
						
						if(!hasHit || distance < result.distance){
							result.shape1 = s1;
							result.shape2 = -1;
							result.bone1 = -1; //bone1;
							result.bone2 = -1;
							result.face = f;
							result.distance = distance;
							result.normal = hitNormal;
							hasHit = true;
						}
					}
				}
			}
			
		}else if(testMode == debpColliderComponent::etmRigShape){
			const debpShapeList &shapes2 = collider2.GetRigShapes();
			int s2, shapeCount2 = shapes2.GetShapeCount();
			
			collider1.UpdateShapes();
			collider2.UpdateShapes();
			
			for(s1=0; s1<shapeCount1; s1++){
				const debpShape &shape1 = *shapes1.GetShapeAt(s1);
				debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
				
				for(s2=0; s2<shapeCount2; s2++){
					distance = (float)colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
					
					if(distance > 0.99999f) continue;
					if(displacement * hitNormal >= -1e-6) continue;
					
					if(!hasHit || distance < result.distance){
						result.shape1 = s1;
						result.shape2 = s2;
						result.bone1 = -1; //bone1;
						result.bone2 = -1;
						result.face = -1;
						result.distance = distance;
						result.normal = hitNormal;
						hasHit = true;
					}
				}
			}
			
		}else if(testMode == debpColliderComponent::etmBoneShape){
			int b, boneCount = collider2.GetBoneCount();
			debpColliderBone *colbone;
			int s2, shapeCount2;
			
			collider1.UpdateShapes();
			collider2.UpdateShapes();
			
			for(s1=0; s1<shapeCount1; s1++){
				const debpShape &shape1 = *shapes1.GetShapeAt(s1);
				debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
				
				for(b=0; b<boneCount; b++){
					colbone = collider2.GetBoneAt(b);
					
					if(colbone){
						const debpShapeList &shapes2 = colbone->GetShapes();
						shapeCount2 = shapes2.GetShapeCount();
						
						for(s2=0; s2<shapeCount2; s2++){
							distance = colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
							
							if(distance > 0.99999f) continue;
							if(displacement * hitNormal >= -1e-6) continue;
							
							if(!hasHit || distance < result.distance){
								result.shape1 = s1;
								result.shape2 = s2;
								result.bone1 = -1; // bone1;
								result.bone2 = b;
								result.face = -1;
								result.distance = distance;
								result.normal = hitNormal;
								hasHit = true;
							}
						}
					}
				}
			}
		}
		
		/*if( hasHit ){
			printf("has hit: s1=%i s2=%i b1=%i b2=%i f=%i d=%g n=(%g,%g,%g)\n", result.shape1, result.shape2, result.bone1, result.bone2, result.face,
			result.distance, result.normal.x, result.normal.y, result.normal.z);
		}*/
		
		return hasHit;
	}
	
	printf("*** TODO *** debpCollisionDetection.ColliderComponentMoveHitsColliderComponent\n");
	// TODO implementation
	return false;
}
#endif

#if 0
bool debpCollisionDetection::ColliderComponentMoveHitsColliderRig(debpColliderComponent &collider1, const decDVector &displacement,
debpColliderRig &collider2, debpCollisionResult &result){
	if(!collider1.GetColliderComponent().GetComponent()){
		return false;
	}
	
	// test rig shapes against collider component
	if(collider1.GetTestMode() == debpColliderComponent::etmRigShape){
		const debpShapeList &shapes1 = collider1.GetRigShapes();
		int s1, shapeCount1 = shapes1.GetShapeCount();
		int testMode = collider2.GetTestMode();
		decDVector hitNormal;
		bool hasHit = false;
		float distance;
		
		const decDVector reldisp(displacement); // - bpcollider->GetPredictedDisplacement();
		
		if(testMode == debpColliderComponent::etmRigShape){
			const debpShapeList &shapes2 = collider2.GetRigShapes();
			int s2, shapeCount2 = shapes2.GetShapeCount();
			
			collider1.UpdateShapes();
			collider2.UpdateShapes();
			
			for(s1=0; s1<shapeCount1; s1++){
				const debpShape &shape1 = *shapes1.GetShapeAt(s1);
				debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
				
				for(s2=0; s2<shapeCount2; s2++){
					distance = colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
					
					if(distance > 0.99999f) continue;
					if(displacement * hitNormal >= -1e-6) continue;
					
					if(!hasHit || distance < result.distance){
						result.shape1 = s1;
						result.shape2 = s2;
						result.bone1 = -1; //bone1;
						result.bone2 = -1;
						result.face = -1;
						result.distance = distance;
						result.normal = hitNormal;
						hasHit = true;
					}
				}
			}
			
		}else if(testMode == debpColliderComponent::etmBoneShape){
			int b, boneCount = collider2.GetBoneCount();
			debpColliderBone *colbone;
			int s2, shapeCount2;
			
			collider1.UpdateShapes();
			collider2.UpdateShapes();
			
			for(s1=0; s1<shapeCount1; s1++){
				const debpShape &shape1 = *shapes1.GetShapeAt(s1);
				debpDCollisionVolume &colvol1 = *shape1.GetCollisionVolume();
				
				for(b=0; b<boneCount; b++){
					colbone = collider2.GetBoneAt(b);
					
					if(colbone){
						const debpShapeList &shapes2 = colbone->GetShapes();
						shapeCount2 = shapes2.GetShapeCount();
						
						for(s2=0; s2<shapeCount2; s2++){
							distance = colvol1.VolumeMoveHitsVolume(shapes2.GetShapeAt(s2)->GetCollisionVolume(), reldisp, &hitNormal);
							
							if(distance > 0.99999f) continue;
							if(displacement * hitNormal >= -1e-6) continue;
							
							if(!hasHit || distance < result.distance){
								result.shape1 = s1;
								result.shape2 = s2;
								result.bone1 = -1; // bone1;
								result.bone2 = b;
								result.face = -1;
								result.distance = distance;
								result.normal = hitNormal;
								hasHit = true;
							}
						}
					}
				}
			}
		}
		
		return hasHit;
	}
	
	printf("*** TODO *** debpCollisionDetection.ColliderComponentMoveHitsColliderRig\n");
	// TODO implementation
	return false;
}
#endif

void debpCollisionDetection::contactPairTest(btCollisionObject *colObjA,
btCollisionObject *colObjB, btDynamicsWorld::ContactResultCallback& resultCallback){
	pDynWorld->contactPairTest(colObjA, colObjB, resultCallback);
}

struct sContactResultBoolean : btManifoldResult{
	bool hasContact;
	
	sContactResultBoolean(const btCollisionObjectWrapper *o0, const btCollisionObjectWrapper *o1) :
	btManifoldResult(o0, o1), hasContact(false){}
	
	void addContactPoint(const btVector3 &, const btVector3 &, btScalar) override{
		hasContact = true;
	}
};

bool debpCollisionDetection::contactPairTest(btCollisionObject *colObjA, btCollisionObject *colObjB){
	btCollisionObjectWrapper obA(0, colObjA->getCollisionShape(),
		colObjA, colObjA->getWorldTransform(), -1, -1);
	btCollisionObjectWrapper obB(0, colObjB->getCollisionShape(),
		colObjB, colObjB->getWorldTransform(), -1, -1);
	
	btCollisionAlgorithm * const algorithm = pDynWorld->getDispatcher()->findAlgorithm(
		&obA, &obB, 0, BT_CLOSEST_POINT_ALGORITHMS);
	
	if(!algorithm){
		return false;
	}
	
	sContactResultBoolean result(&obA, &obB);
	
	algorithm->processCollision(&obA, &obB, pDynWorld->getDispatchInfo(), &result);
	algorithm->~btCollisionAlgorithm();
	pDynWorld->getDispatcher()->freeCollisionAlgorithm(algorithm);
	return result.hasContact;
}
