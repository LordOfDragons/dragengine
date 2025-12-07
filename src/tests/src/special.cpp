#ifndef DETESTS_SPECIAL_OFF

#include "../../modules/physics/bullet/Bullet/BulletCollision/CollisionShapes/btBoxShape.h"
#include "../../modules/physics/bullet/Bullet/BulletCollision/CollisionShapes/btCompoundShape.h"
#include "../../modules/physics/bullet/Bullet/BulletCollision/CollisionShapes/btSphereShape.h"
#include "../../modules/physics/bullet/Bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/math/collision/decDCollisionBox.h>
#include <dragengine/common/math/collision/decDCollisionSphere.h>
#include <dragengine/common/utils/decTimer.h>

struct ConvexResultCallback : public btCollisionWorld::ConvexResultCallback{
public:
	float pHitDistance;
	
	ConvexResultCallback(){}
	virtual bool needsCollision(btBroadphaseProxy *proxy0) const{
		return true;
	}
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace){
		pHitDistance = convexResult.m_hitFraction;
		return 1.0f;
	}
};

void special(){
	const decDVector box1Pos(4.0, 3.0, 2.0);
	const decDVector box1HalfExt(3.0, 2.0, 1.0);
	const decDVector box2Pos(box1Pos + decDVector(6.0, 1.0, 0.5));
	const decDVector box2HalfExt(2.0, 1.0, 3.0);
	const decDVector ax(4.0, 0.5, 0.25);
	const decDVector sphere3Pos(box1Pos + decDVector(6.0, 0.0, 0.0));
	const double sphere3Radius = 2.0;
	const int count = 10000;
	decTimer timer;
	int i;
	
	const btQuaternion btqzero(0, 0, 0, 1);
	const btTransform bttzero(btqzero, btVector3(0, 0, 0));
	
	btBoxShape box1Inner(btVector3(box1HalfExt.x, box1HalfExt.y, box1HalfExt.z));
	btCompoundShape box1;
	box1.addChildShape(btTransform(btqzero, btVector3(box1Pos.x, box1Pos.y, box1Pos.z)), &box1Inner);
	
	btBoxShape box2Shape(btVector3(box2HalfExt.x, box2HalfExt.y, box2HalfExt.z));
	
	btSphereShape sphere3Shape(sphere3Radius);
	
	decDCollisionBox colBox1(box1Pos, box1HalfExt);
	decDCollisionBox colBox2(box2Pos, box2HalfExt);
	decDCollisionSphere colSphere3(sphere3Pos, sphere3Radius);
	
	// test 1
	{
	const btTransform castFrom(btqzero, btVector3(box2Pos.x, box2Pos.y, box2Pos.z));
	const btTransform castTo(btqzero, btVector3(box2Pos.x+ax.x, box2Pos.y+ax.y, box2Pos.z+ax.z));
	btCollisionObject dummyColObj;
	
	ConvexResultCallback cscallback;
	
	{
	cscallback.pHitDistance = 1.0f;
	btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
	btCollisionWorld::objectQuerySingleInternal(&box2Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	printf("Test1: Verify HitDistance should be 1 is %g \n", cscallback.pHitDistance);
	}
	
	{
	timer.Reset();
	for(i=0; i<count; i++){
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&box2Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test1: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	}
	
	// test 2
	{
	const btTransform castFrom(btqzero, btVector3(box2Pos.x, box2Pos.y, box2Pos.z));
	const btTransform castTo(btqzero, btVector3(box2Pos.x-ax.x, box2Pos.y-ax.y, box2Pos.z-ax.z));
	btCollisionObject dummyColObj;
	
	ConvexResultCallback cscallback;
	
	{
	cscallback.pHitDistance = 1.0f;
	btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
	btCollisionWorld::objectQuerySingleInternal(&box2Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	printf("Test2: Verify HitDistance should be 0.25 is %g \n", cscallback.pHitDistance);
	}
	
	{
	timer.Reset();
	for(i=0; i<count; i++){
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&box2Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test2: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	}
	
	// test 3
	{
	const btTransform castFrom(btqzero, btVector3(sphere3Pos.x, sphere3Pos.y, sphere3Pos.z));
	const btTransform castTo(btqzero, btVector3(sphere3Pos.x+ax.x, sphere3Pos.y+ax.y, sphere3Pos.z+ax.z));
	btCollisionObject dummyColObj;
	
	ConvexResultCallback cscallback;
	
	{
	cscallback.pHitDistance = 1.0f;
	btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
	btCollisionWorld::objectQuerySingleInternal(&sphere3Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	printf("Test3: Verify HitDistance should be 1 is %g \n", cscallback.pHitDistance);
	}
	
	{
	timer.Reset();
	for(i=0; i<count; i++){
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&sphere3Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test3: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	}
	
	// test 4
	{
	const btTransform castFrom(btqzero, btVector3(sphere3Pos.x, sphere3Pos.y, sphere3Pos.z));
	const btTransform castTo(btqzero, btVector3(sphere3Pos.x-ax.x, sphere3Pos.y-ax.y, sphere3Pos.z-ax.z));
	btCollisionObject dummyColObj;
	
	ConvexResultCallback cscallback;
	
	{
	cscallback.pHitDistance = 1.0f;
	btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
	btCollisionWorld::objectQuerySingleInternal(&sphere3Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	printf("Test4: Verify HitDistance should be 0.25 is %g \n", cscallback.pHitDistance);
	}
	
	{
	timer.Reset();
	for(i=0; i<count; i++){
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&sphere3Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test4: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	}
	
	
	
	// test 5
	{
	decDVector normal;
	printf("Test5: Verify HitDistance should be 1 is %g \n", colBox1.BoxMoveHitsBox(&colBox2, ax, &normal));
	
	timer.Reset();
	for(i=0; i<count; i++){
		colBox1.BoxMoveHitsBox(&colBox2, ax, &normal);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test5: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	
	// test 6
	{
	decDVector normal;
	printf("Test6: Verify HitDistance should be 0.25 is %g \n", colBox1.BoxMoveHitsBox(&colBox2, -ax, &normal));
	
	timer.Reset();
	for(i=0; i<count; i++){
		colBox1.BoxMoveHitsBox(&colBox2, -ax, &normal);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test6: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	
	// test 7
	{
	decDVector normal;
	printf("Test7: Verify HitDistance should be 1 is %g \n", colBox1.SphereMoveHitsBox(&colSphere3, ax, &normal));
	
	timer.Reset();
	for(i=0; i<count; i++){
		colBox1.SphereMoveHitsBox(&colSphere3, ax, &normal);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test7: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	
	// test 8
	{
	decDVector normal;
	printf("Test8: Verify HitDistance should be 0.25 is %g \n", colBox1.SphereMoveHitsBox(&colSphere3, -ax, &normal));
	
	timer.Reset();
	for(i=0; i<count; i++){
		colBox1.SphereMoveHitsBox(&colSphere3, -ax, &normal);
	}
	const float elapsed = timer.GetElapsedTime();
	printf("Test8: Total %iys per test %.2fys\n", (int)(elapsed * 1e6f), (elapsed / (float)count) * 1e6f);
	}
	
	
	
	// test 9
	{
	double elapsed[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	for(i=0; i<count; i++){
		const decDVector ranrot(
			360.0 * DEG2RAD * ((double)rand() / (double)RAND_MAX),
			360.0 * DEG2RAD * ((double)rand() / (double)RAND_MAX),
			360.0 * DEG2RAD * ((double)rand() / (double)RAND_MAX));
		const decDMatrix rotmat = decDMatrix::CreateRotation(ranrot);
		const double offsetDistance = 8.0; // 6.0 yields collisions at 0 point. decCollisionBox returns 0 in all situations
		                                   // while btBoxShape does return 1. this is when the movement moves out of the
		                                   // collision situation. is this a good idea for box too?
		const decDVector offset(rotmat.TransformView() * offsetDistance);
		const decDVector ax(rotmat.TransformView() * 6.0); // 4.0
		
		const decDVector box9Pos(box1Pos + offset);
		const decDVector sphere9Pos(box1Pos + offset);
		
		float distance[8];
		
		// test 1a
		{
		const btTransform castFrom(btqzero, btVector3(box9Pos.x, box9Pos.y, box9Pos.z));
		const btTransform castTo(btqzero, btVector3(box9Pos.x+ax.x, box9Pos.y+ax.y, box9Pos.z+ax.z));
		btCollisionObject dummyColObj;
		
		ConvexResultCallback cscallback;
		
		cscallback.pHitDistance = 1.0f;
		{
		timer.Reset();
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&box2Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
		elapsed[0] += (double)timer.GetElapsedTime();
		}
		distance[0] = cscallback.pHitDistance;
		}
		
		// test 1b
		{
		const btTransform castFrom(btqzero, btVector3(box9Pos.x, box9Pos.y, box9Pos.z));
		const btTransform castTo(btqzero, btVector3(box9Pos.x-ax.x, box9Pos.y-ax.y, box9Pos.z-ax.z));
		btCollisionObject dummyColObj;
		
		ConvexResultCallback cscallback;
		
		cscallback.pHitDistance = 1.0f;
		{
		timer.Reset();
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&box2Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
		elapsed[1] += (double)timer.GetElapsedTime();
		}
		distance[1] = cscallback.pHitDistance;
		}
		
		// test 1c
		{
		const btTransform castFrom(btqzero, btVector3(sphere9Pos.x, sphere9Pos.y, sphere9Pos.z));
		const btTransform castTo(btqzero, btVector3(sphere9Pos.x+ax.x, sphere9Pos.y+ax.y, sphere9Pos.z+ax.z));
		btCollisionObject dummyColObj;
		
		ConvexResultCallback cscallback;
		
		cscallback.pHitDistance = 1.0f;
		{
		timer.Reset();
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&sphere3Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
		elapsed[2] += (double)timer.GetElapsedTime();
		}
		distance[2] = cscallback.pHitDistance;
		}
		
		// test 1d
		{
		const btTransform castFrom(btqzero, btVector3(sphere9Pos.x, sphere9Pos.y, sphere9Pos.z));
		const btTransform castTo(btqzero, btVector3(sphere9Pos.x-ax.x, sphere9Pos.y-ax.y, sphere9Pos.z-ax.z));
		btCollisionObject dummyColObj;
		
		ConvexResultCallback cscallback;
		
		cscallback.pHitDistance = 1.0f;
		{
		timer.Reset();
		btCollisionObjectWrapper tmpOb(0, &box1, &dummyColObj, bttzero, -1, -1);
		btCollisionWorld::objectQuerySingleInternal(&sphere3Shape, castFrom, castTo, &tmpOb, cscallback, 0.0f);
		elapsed[3] += (double)timer.GetElapsedTime();
		}
		distance[3] = cscallback.pHitDistance;
		}
		
		// test 2a
		decDCollisionBox colBox9(box9Pos, box2HalfExt);
		decDVector normal;
		
		{
		timer.Reset();
		//distance[ 4 ] = colBox9.VolumeMoveHitsVolume( &colBox1, ax, &normal );
		distance[4] = colBox1.BoxMoveHitsBox(&colBox9, ax, &normal);
		elapsed[4] += (double)timer.GetElapsedTime();
		}
		
		// test 2b
		{
		timer.Reset();
		//distance[ 5 ] = colBox9.VolumeMoveHitsVolume( &colBox1, -ax, &normal );
		distance[5] = colBox1.BoxMoveHitsBox(&colBox9, -ax, &normal);
		elapsed[5] += (double)timer.GetElapsedTime();
		}
		
		// test 2c
		decDCollisionSphere colSphere9(sphere9Pos, sphere3Radius);
		
		{
		timer.Reset();
		//distance[ 6 ] = colSphere9.VolumeMoveHitsVolume( &colBox1, ax, &normal );
		distance[6] = colBox1.SphereMoveHitsBox(&colSphere9, ax, &normal);
		elapsed[6] += (double)timer.GetElapsedTime();
		}
		
		// test 2d
		{
		timer.Reset();
		//distance[ 7 ] = colSphere9.VolumeMoveHitsVolume( &colBox1, -ax, &normal );
		distance[7] = colBox1.SphereMoveHitsBox(&colSphere9, -ax, &normal);
		elapsed[7] += (double)timer.GetElapsedTime();
		}
		
		// validate. 1->5 can not be validated since bullet returns inaccurate or plain out wrong results
// 		const double errThreshold = 0.002 / offsetDistance; // 2mm accuracy
		const double errThreshold2 = 0.01; // accuracy of bullet is bad
		if(distance[0] != 1.0 || distance[4] != 1.0){
			printf("turn %i: wrong result distance[0]=%g distance[4]=%g pos=(%f,%f,%f) ax=(%f,%f,%f)\n", i,
				distance[0], distance[4], box9Pos.x, box9Pos.y, box9Pos.z, ax.x, ax.y, ax.z);
		}
		if(distance[1] == 1.0 || distance[5] == 1.0){
			printf("turn %i: wrong result distance[1]=%g distance[5]=%g pos=(%f,%f,%f) ax=(%f,%f,%f)\n", i,
				distance[1], distance[5], box9Pos.x, box9Pos.y, box9Pos.z, ax.x, ax.y, ax.z);
		}
// 		if( fabs( distance[ 1 ] - distance[ 5 ] ) > errThreshold ){
// 			printf( "turn %i: wrong result distance[1]=%g distance[5]=%g pos=(%f,%f,%f) ax=(%f,%f,%f)\n", i,
// 				distance[ 1 ], distance[ 5 ], box9Pos.x, box9Pos.y, box9Pos.z, ax.x, ax.y, ax.z );
// 		}
		if(distance[2] != distance[6]){
			printf("turn %i: wrong result distance[2]=%g distance[6]=%g pos=(%f,%f,%f) ax=(%f,%f,%f)\n", i,
				distance[2], distance[6], sphere9Pos.x, sphere9Pos.y, sphere9Pos.z, ax.x, ax.y, ax.z);
		}
		if(distance[3] == 1.0 || distance[7] == 1.0){
			printf("turn %i: wrong result distance[3]=%g distance[7]=%g pos=(%f,%f,%f) ax=(%f,%f,%f)\n", i,
				distance[3], distance[7], sphere9Pos.x, sphere9Pos.y, sphere9Pos.z, ax.x, ax.y, ax.z);
		}
		if(fabs(distance[3] - distance[7]) > errThreshold2){
			printf("turn %i: wrong result distance[3]=%g distance[7]=%g pos=(%f,%f,%f) ax=(%f,%f,%f)\n", i,
				distance[3], distance[7], sphere9Pos.x, sphere9Pos.y, sphere9Pos.z, ax.x, ax.y, ax.z);
		}
	}
	
	printf("Test9 1A: Total %iys per test %.2fys\n", (int)(elapsed[0] * 1e6), (elapsed[0] * 1e6 / (double)count));
	printf("Test9 1B: Total %iys per test %.2fys\n", (int)(elapsed[1] * 1e6), (elapsed[1] * 1e6 / (double)count));
	printf("Test9 1C: Total %iys per test %.2fys\n", (int)(elapsed[2] * 1e6), (elapsed[2] * 1e6 / (double)count));
	printf("Test9 1D: Total %iys per test %.2fys\n", (int)(elapsed[3] * 1e6), (elapsed[3] * 1e6 / (double)count));
	printf("Test9 2A: Total %iys per test %.2fys\n", (int)(elapsed[4] * 1e6), (elapsed[4] * 1e6 / (double)count));
	printf("Test9 2B: Total %iys per test %.2fys\n", (int)(elapsed[5] * 1e6), (elapsed[5] * 1e6 / (double)count));
	printf("Test9 2C: Total %iys per test %.2fys\n", (int)(elapsed[6] * 1e6), (elapsed[6] * 1e6 / (double)count));
	printf("Test9 2D: Total %iys per test %.2fys\n", (int)(elapsed[7] * 1e6), (elapsed[7] * 1e6 / (double)count));
	}
}

#endif
