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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "debpCollisionDispatcher.h"
#include "debpConstraintSolver.h"
#include "debpGhostPairCallback.h"
#include "debpOverlapFilterCallback.h"
#include "debpSharedCollisionFiltering.h"
#include "debpWorld.h"
#include "../dePhysicsBullet.h"
#include "../debpConfiguration.h"
#include "../debpPhysicsBody.h"
#include "../debpMotionState.h"
#include "../debug/debpDebug.h"
#include "../debug/debpDebugInformation.h"
#include "../coldet/debpCollisionDetection.h"
#include "../coldet/unstuck/debpUnstuckCollider.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionTriangle.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../component/debpComponent.h"
#include "../decal/debpDecal.h"
#include "../forcefield/debpForceField.h"
#include "../particle/debpParticleEmitterInstance.h"
#include "../propfield/debpPropField.h"
#include "../propfield/debpPropFieldType.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../touchsensor/debpTouchSensor.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../world/debpCollisionWorld.h"

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btDefaultSoftBodySolver.h"
#include "BulletSoftBody/btSoftBody.h"
#ifndef BT_NO_PROFILE
	#include "LinearMath/btQuickprof.h"
#endif

#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/deEngine.h>



// Class debpWorld
////////////////////

// Constructor, destructor
////////////////////////////

debpWorld::debpWorld( dePhysicsBullet &bullet, deWorld &world ) :
pBullet( bullet ),
pWorld( world ),
pColInfo( NULL ),
pUnstuckCollider( NULL ),

pHeightTerrain( NULL ),

pSharedCollisionFiltering( NULL ),

pDynWorld( NULL ),
pBroadPhase( NULL ),
pColDisp( NULL ),
pConstraintSolver( NULL ),
pSoftBodySolver( NULL ),
pColConfig( NULL ),
pSoftBodyWorldInfo( NULL ),
pOverlapFilterCallback( NULL ),
pGhostPairCallback( NULL ),

pDirtyOctree( true ),
pDirtyDynWorldAABB( true ),

pLeftOverTime( 0.0f ),

pDetNextCollider( NULL ),
pDetNextTouchSensor( NULL ),

pColDetPrepareColliders( NULL ),
pColDetPrepareColliderCount( 0 ),
pColDetPrepareColliderSize( 0 ),
pColDetPrepareColliderProcessCount( 0 ),

pColDetFinishColliders( NULL ),
pColDetFinishColliderCount( 0 ),
pColDetFinishColliderSize( 0 ),

pPPCTColliders( NULL ),
pPPCTColliderCount( 0 ),
pPPCTColliderSize( 0 ),

pUpdateOctreeColliders( NULL ),
pUpdateOctreeColliderCount( 0 ),
pUpdateOctreeColliderSize( 0 ),

// max steps = max time (0.1s) divided by frquency (1/60) = 6
// erwin mentioned up to (10, 1/240)
// pSimMaxSubStep( 6 ),
// pSimTimeStep( 1.0f / 60.0f ),
pSimMaxSubStep( 10 ),
pSimTimeStep( 1.0f / 120.0f ),

pDynCollisionVelocityThreshold( 0.0f ),
pProcessingPhysics( false )
{
	// init
	try{
		pColInfo = new deCollisionInfo;
		pUnstuckCollider = new debpUnstuckCollider( *this );
		
		pSharedCollisionFiltering = new debpSharedCollisionFiltering( *this );
		
		// create physics environment
		//const decDVector &size = world->GetSize();
		//btVector3 worldAabbMin( -size.x * 0.5, -size.y * 0.5, -size.z * 0.5 ); //-3e5f, -3e5f, -3e5f );
		//btVector3 worldAabbMax( size.x * 0.5, size.y * 0.5, size.z * 0.5 ); //3e5f, 3e5f, 3e5f );
		//pBroadPhase = new btAxisSweep3( worldAabbMin, worldAabbMax );
		pBroadPhase = new btDbvtBroadphase;
		
		pColConfig = new btSoftBodyRigidBodyCollisionConfiguration;
		pColDisp = new debpCollisionDispatcher( *pSharedCollisionFiltering, pColConfig );
		
		pConstraintSolver = new debpConstraintSolver;
		pSoftBodySolver = new btDefaultSoftBodySolver;
		
		pOverlapFilterCallback = new debpOverlapFilterCallback( *pSharedCollisionFiltering );
		pGhostPairCallback = new debpGhostPairCallback;
		
		pSoftBodyWorldInfo = new btSoftBodyWorldInfo;
		pSoftBodyWorldInfo->m_dispatcher = pColDisp;
		pSoftBodyWorldInfo->m_gravity.setZero();
		pSoftBodyWorldInfo->m_sparsesdf.Initialize();
		
		pDynWorld = new debpCollisionWorld( *this, pColDisp, pBroadPhase, pConstraintSolver, pColConfig, pSoftBodySolver );
		pDynWorld->setGravity( btVector3( 0.0f, 0.0f, 0.0f ) );
		pDynWorld->getPairCache()->setOverlapFilterCallback( pOverlapFilterCallback );
		pDynWorld->getPairCache()->setInternalGhostPairCallback( pGhostPairCallback );
		
		HeightTerrainChanged();
		PhysicsChanged();
		
		deCollider *collider = world.GetRootCollider();
		while( collider ){
			ColliderAdded( collider );
			collider = collider->GetLLWorldNext();
		}
		
		deTouchSensor *touchSensor = world.GetRootTouchSensor();
		while( touchSensor ){
			TouchSensorAdded( touchSensor );
			touchSensor = touchSensor->GetLLWorldNext();
		}
		
		deParticleEmitterInstance *emitter = world.GetRootParticleEmitter();
		while( emitter ){
			ParticleEmitterAdded( emitter );
			emitter = emitter->GetLLWorldNext();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debpWorld::~debpWorld(){
	pCleanUp();
}



// Management
///////////////

// #define DO_TIMING
// #define DO_TIMING2

#if defined DO_TIMING || defined DO_TIMING2
	#include <dragengine/common/utils/decTimer.h>
#endif

#ifdef DO_TIMING
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)			pBullet.LogInfoFormat( "World Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )
	#define DEBUG_PRINT_TIMER_TOTAL()		pBullet.LogInfoFormat( "World Timer-Total = %iys", ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
	
	#ifdef DO_TIMING2
		int timerUpdateOctrees = 0;
		int timerUpdateOctreesCount = 0;
		int timerUpdateDynWorldAABBs = 0;
		int timerUpdateDynWorldAABBsCount = 0;
		int timerRayHits = 0;
		int timerRayHitsCount = 0;
		int timerColliderHits = 0;
		int timerColliderHitsCount = 0;
		int timerColliderMoveHits = 0;
		int timerColliderMoveHitsCount = 0;
		int timerColliderRotateHits = 0;
		int timerColliderRotateHitsCount = 0;
		int timerColliderMoveRotateHits = 0;
		int timerColliderMoveRotateHitsCount = 0;
	#endif
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL()
#endif



void debpWorld::Update( float elapsed ){
	(void)elapsed;
	UpdateOctrees();
}

void debpWorld::UpdateOctrees(){
	if( ! pDirtyOctree ){
		return;
	}
	
	debpDebugInformation *debugInfoCollider = NULL;
	debpDebugInformation *debugInfoWorld = NULL;
	if( pBullet.GetDebug().GetEnabled() ){
		debugInfoCollider = pBullet.GetDebug().GetDIColliderUpdateOctree();
		debugInfoWorld = pBullet.GetDebug().GetDIWorldUpdateOctrees();
		pPerfTimer.Reset();
	}
	
#ifdef DO_TIMING2
timer.Reset();
#endif
	pDirtyOctree = false; // just in case somebody tries to loop on us
	
	// TODO UpdateOctreePosition causes nested UpdateOctrees calls due to touch sensor check
	// TODO touch sensor check only works after movement not on partial movement
	// TODO touch sensor test in UpdateOctreePosition can cause troubles if collider is removed from the world while touching a touch sensor
	
	// update height terrain
	if( pHeightTerrain ){
		pHeightTerrain->Update();
	}
	
	// check all colliders for octree dirtiness
	//count = pWorld.GetColliderCount();
	const int count = pUpdateOctreeColliderCount;
	int i;
	
	if( debugInfoCollider ){
		pPerfTimer2.Reset();
	}
	for( i=0; i<count; i++ ){
		debpCollider * const collider = pUpdateOctreeColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		pUpdateOctreeColliders[ i ] = NULL;
		collider->SetUpdateOctreeIndex( -1 );
		
		collider->UpdateOctreePosition();
		
		if( debugInfoCollider ){
			debugInfoCollider->IncrementElapsedTime( pPerfTimer2.GetElapsedTime() );
			debugInfoCollider->IncrementCounter( 1 );
		}
	}
	
	int next = 0;
	for( i=count; i<pUpdateOctreeColliderCount; i++ ){
		debpCollider * const collider = pUpdateOctreeColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		if( i != next ){
			pUpdateOctreeColliders[ next ] = collider;
			pUpdateOctreeColliders[ i ] = NULL;
			collider->SetUpdateOctreeIndex( next );
		}
		next++;
	}
	
	pUpdateOctreeColliderCount = next;
	
	// debug
	if( debugInfoWorld ){
		debugInfoWorld->IncrementElapsedTime( pPerfTimer.GetElapsedTime() );
		debugInfoWorld->IncrementCounter( 1 );
	}
	
#ifdef DO_TIMING2
timerUpdateOctrees += ( int )( timer.GetElapsedTime() * 1e6f );
timerUpdateOctreesCount++;
#endif
}

void debpWorld::MarkOctreeDirty(){
	pDirtyOctree = true;
	pDirtyDynWorldAABB = true;
}

void debpWorld::UpdateDynWorldAABBs(){
	if( pDirtyDynWorldAABB ){
#ifdef DO_TIMING2
timer.Reset();
#endif
		pDynWorld->UpdateDirtyAABBs();
		pDirtyDynWorldAABB = false;
#ifdef DO_TIMING2
timerUpdateDynWorldAABBs += ( int )( timer.GetElapsedTime() * 1e6f );
timerUpdateDynWorldAABBsCount++;
#endif
	}
}



void debpWorld::pColDetPrepareColliderAdd( debpCollider *collider ){
	if( collider->GetColDetPrepareIndex() != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pColDetPrepareColliderCount == pColDetPrepareColliderSize ){
		const int newSize = pColDetPrepareColliderSize + 10;
		debpCollider ** const newArray = new debpCollider*[ newSize ];
		if( pColDetPrepareColliders ){
			memcpy( newArray, pColDetPrepareColliders, sizeof( debpCollider* ) * pColDetPrepareColliderSize );
			delete [] pColDetPrepareColliders;
		}
		pColDetPrepareColliders = newArray;
		pColDetPrepareColliderSize = newSize;
	}
	
	collider->SetColDetPrepareIndex( pColDetPrepareColliderCount );
	pColDetPrepareColliders[ pColDetPrepareColliderCount++ ] = collider;
}

void debpWorld::pColDetPrepareColliderRemove( debpCollider *collider ){
	if( collider->GetColDetPrepareIndex() == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pColDetPrepareColliders[ collider->GetColDetPrepareIndex() ] = NULL;
	collider->SetColDetPrepareIndex( -1 );
}



void debpWorld::pColDetFinishColliderAdd( debpCollider *collider ){
	if( collider->GetColDetFinishIndex() != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pColDetFinishColliderCount == pColDetFinishColliderSize ){
		const int newSize = pColDetFinishColliderSize + 10;
		debpCollider ** const newArray = new debpCollider*[ newSize ];
		if( pColDetFinishColliders ){
			memcpy( newArray, pColDetFinishColliders, sizeof( debpCollider* ) * pColDetFinishColliderSize );
			delete [] pColDetFinishColliders;
		}
		pColDetFinishColliders = newArray;
		pColDetFinishColliderSize = newSize;
	}
	
	collider->SetColDetFinishIndex( pColDetFinishColliderCount );
	pColDetFinishColliders[ pColDetFinishColliderCount++ ] = collider;
}

void debpWorld::pColDetFinishColliderRemove( debpCollider *collider ){
	if( collider->GetColDetFinishIndex() == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pColDetFinishColliders[ collider->GetColDetFinishIndex() ] = NULL;
	collider->SetColDetFinishIndex( -1 );
}



void debpWorld::pPPCTColliderAdd( debpCollider *collider ){
	if( collider->GetPPCProcessingIndex() != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPPCTColliderCount == pPPCTColliderSize ){
		const int newSize = pPPCTColliderSize + 10;
		debpCollider ** const newArray = new debpCollider*[ newSize ];
		if( pPPCTColliders ){
			memcpy( newArray, pPPCTColliders, sizeof( debpCollider* ) * pPPCTColliderSize );
			delete [] pPPCTColliders;
		}
		pPPCTColliders = newArray;
		pPPCTColliderSize = newSize;
	}
	
	collider->SetPPCProcessingIndex( pPPCTColliderCount );
	pPPCTColliders[ pPPCTColliderCount++ ] = collider;
}

void debpWorld::pPPCTColliderRemove( debpCollider *collider ){
	if( collider->GetPPCProcessingIndex() == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pPPCTColliders[ collider->GetPPCProcessingIndex() ] = NULL;
	collider->SetPPCProcessingIndex( -1 );
}


void debpWorld::pUpdateOctreeColliderAdd( debpCollider *collider ){
	if( collider->GetUpdateOctreeIndex() != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUpdateOctreeColliderCount == pUpdateOctreeColliderSize ){
		const int newSize = pUpdateOctreeColliderSize + 10;
		debpCollider ** const newArray = new debpCollider*[ newSize ];
		if( pUpdateOctreeColliders ){
			memcpy( newArray, pUpdateOctreeColliders, sizeof( debpCollider* ) * pUpdateOctreeColliderSize );
			delete [] pUpdateOctreeColliders;
		}
		pUpdateOctreeColliders = newArray;
		pUpdateOctreeColliderSize = newSize;
	}
	
	collider->SetUpdateOctreeIndex( pUpdateOctreeColliderCount );
	pUpdateOctreeColliders[ pUpdateOctreeColliderCount++ ] = collider;
}

void debpWorld::pUpdateOctreeColliderRemove( debpCollider *collider ){
	if( collider->GetUpdateOctreeIndex() == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pUpdateOctreeColliders[ collider->GetUpdateOctreeIndex() ] = NULL;
	collider->SetUpdateOctreeIndex( -1 );
}



#ifdef DO_TIMING2
extern int timerWorldUpdateAabbs;
extern int timerWorldUpdateAabbsCount;
extern int timerTSPositionChanged;
extern int timerTSPositionChangedCount;
extern int timerTSOrientationChanged;
extern int timerTSOrientationChangedCount;
extern int timerTSRayHits;
extern int timerTSRayHitsCount;
extern int timerTSColliderHits;
extern int timerTSColliderHitsCount;
extern int timerTSColliderMoveHits;
extern int timerTSColliderMoveHitsCount;
extern int timerTSColliderRotateHits;
extern int timerTSColliderRotateHitsCount;
extern int timerTSColliderMoveRotateHits;
extern int timerTSColliderMoveRotateHitsCount;
extern int timerShapeCastConvex;
extern int timerShapeCastConvexCount;
extern int timerObjectQuerySingle;
extern int timerObjectQuerySingleCount;
#endif
#ifndef BT_NO_PROFILE
	#include <LinearMath/btQuickprof.h>
#endif
void debpWorld::ProcessPhysics( float elapsed ){
	pBullet.GetDebug().BeginProcessPhysics( this );
	
#ifdef DO_TIMING2
	pBullet.LogInfoFormat( "Transit UpdateOctrees (%i) = %iys", timerUpdateOctreesCount, timerUpdateOctrees );
	pBullet.LogInfoFormat( "Transit UpdateDynWorldAABBs (%i) = %iys", timerUpdateDynWorldAABBsCount, timerUpdateDynWorldAABBs );
	pBullet.LogInfoFormat( "Transit RayHits (%i) = %iys", timerRayHitsCount, timerRayHits );
	pBullet.LogInfoFormat( "Transit ColliderHits (%i) = %iys", timerColliderHitsCount, timerColliderHits );
	pBullet.LogInfoFormat( "Transit ColliderMoveHits (%i) = %iys", timerColliderMoveHitsCount, timerColliderMoveHits );
	pBullet.LogInfoFormat( "Transit ColliderRotateHits (%i) = %iys", timerColliderRotateHitsCount, timerColliderRotateHits );
	pBullet.LogInfoFormat( "Transit ColliderMoveRotateHits (%i) = %iys", timerColliderMoveRotateHitsCount, timerColliderMoveRotateHits );
	pBullet.LogInfoFormat( "Transit World updateAabbs (%i) = %iys", timerWorldUpdateAabbsCount, timerWorldUpdateAabbs );
	pBullet.LogInfoFormat( "Transit TS Position Changed (%i) = %iys", timerTSPositionChangedCount, timerTSPositionChanged );
	pBullet.LogInfoFormat( "Transit TS Orientation Changed (%i) = %iys", timerTSOrientationChangedCount, timerTSOrientationChanged );
	pBullet.LogInfoFormat( "Transit TS Ray Hits (%i) = %iys", timerTSRayHitsCount, timerTSRayHits );
	pBullet.LogInfoFormat( "Transit TS Collider Hits (%i) = %iys", timerTSColliderHitsCount, timerTSColliderHits );
	pBullet.LogInfoFormat( "Transit TS Collider Move Hits (%i) = %iys", timerTSColliderMoveHitsCount, timerTSColliderMoveHits );
	pBullet.LogInfoFormat( "Transit TS Collider Rotate Hits (%i) = %iys", timerTSColliderRotateHitsCount, timerTSColliderRotateHits );
	pBullet.LogInfoFormat( "Transit TS Collider Move Rotate Hits (%i) = %iys", timerTSColliderMoveRotateHitsCount, timerTSColliderMoveRotateHits );
	pBullet.LogInfoFormat( "Transit ShapeCastConvex (%i) = %iys", timerShapeCastConvexCount, timerShapeCastConvex );
	pBullet.LogInfoFormat( "Transit ObjectQuerySingle (%i) = %iys", timerObjectQuerySingleCount, timerObjectQuerySingle );
	timerUpdateOctrees = 0; timerUpdateOctreesCount = 0;
	timerUpdateDynWorldAABBs = 0; timerUpdateDynWorldAABBsCount = 0;
	timerRayHits = 0; timerRayHitsCount = 0;
	timerColliderHits = 0; timerColliderHitsCount = 0;
	timerColliderMoveHits = 0; timerColliderMoveHitsCount = 0;
	timerColliderRotateHits = 0; timerColliderRotateHitsCount = 0;
	timerColliderMoveRotateHits = 0; timerColliderMoveRotateHitsCount = 0;
	timerWorldUpdateAabbs = 0; timerWorldUpdateAabbsCount = 0;
	timerTSPositionChanged = 0; timerTSPositionChangedCount = 0;
	timerTSOrientationChanged = 0; timerTSOrientationChangedCount = 0;
	timerTSRayHits = 0; timerTSRayHitsCount = 0;
	timerTSColliderHits = 0; timerTSColliderHitsCount = 0;
	timerTSColliderMoveHits = 0; timerTSColliderMoveHitsCount = 0;
	timerTSColliderRotateHits = 0; timerTSColliderRotateHitsCount = 0;
	timerTSColliderMoveRotateHits = 0; timerTSColliderMoveRotateHitsCount = 0;
	timerShapeCastConvex = 0; timerShapeCastConvexCount = 0;
	timerObjectQuerySingle = 0; timerObjectQuerySingleCount = 0;
#endif
	
DEBUG_RESET_TIMERS;
	try{
		pProcessPhysics( elapsed );
		pProcessingPhysics = false;
		
	}catch( const deException & ){
		pProcessingPhysics = false;
		throw;
	}
	
DEBUG_PRINT_TIMER_TOTAL();
#ifdef DO_TIMING2
	pBullet.LogInfoFormat( "DetectCollisions UpdateOctrees (%i) = %iys", timerUpdateOctreesCount, timerUpdateOctrees );
	pBullet.LogInfoFormat( "DetectCollisions UpdateDynWorldAABBs (%i) = %iys", timerUpdateDynWorldAABBsCount, timerUpdateDynWorldAABBs );
	pBullet.LogInfoFormat( "DetectCollisions RayHits (%i) = %iys", timerRayHitsCount, timerRayHits );
	pBullet.LogInfoFormat( "DetectCollisions ColliderHits (%i) = %iys", timerColliderHitsCount, timerColliderHits );
	pBullet.LogInfoFormat( "DetectCollisions ColliderMoveHits (%i) = %iys", timerColliderMoveHitsCount, timerColliderMoveHits );
	pBullet.LogInfoFormat( "DetectCollisions ColliderRotateHits (%i) = %iys", timerColliderRotateHitsCount, timerColliderRotateHits );
	pBullet.LogInfoFormat( "DetectCollisions ColliderMoveRotateHits (%i) = %iys", timerColliderMoveRotateHitsCount, timerColliderMoveRotateHits );
	pBullet.LogInfoFormat( "DetectCollisions World updateAabbs (%i) = %iys", timerWorldUpdateAabbsCount, timerWorldUpdateAabbs );
	pBullet.LogInfoFormat( "DetectCollisions World TS Position Changed (%i) = %iys", timerTSPositionChangedCount, timerTSPositionChanged );
	pBullet.LogInfoFormat( "DetectCollisions World TS Orientation Changed (%i) = %iys", timerTSOrientationChangedCount, timerTSOrientationChanged );
	pBullet.LogInfoFormat( "DetectCollisions World TS Ray Hits (%i) = %iys", timerTSRayHitsCount, timerTSRayHits );
	pBullet.LogInfoFormat( "DetectCollisions World TS Collider Hits (%i) = %iys", timerTSColliderHitsCount, timerTSColliderHits );
	pBullet.LogInfoFormat( "DetectCollisions World TS Collider Move Hits (%i) = %iys", timerTSColliderMoveHitsCount, timerTSColliderMoveHits );
	pBullet.LogInfoFormat( "DetectCollisions World TS Collider Rotate Hits (%i) = %iys", timerTSColliderRotateHitsCount, timerTSColliderRotateHits );
	pBullet.LogInfoFormat( "DetectCollisions World TS Collider Move Rotate Hits (%i) = %iys", timerTSColliderMoveRotateHitsCount, timerTSColliderMoveRotateHits );
	pBullet.LogInfoFormat( "DetectCollisions World ShapeCastConvex (%i) = %iys", timerShapeCastConvexCount, timerShapeCastConvex );
	pBullet.LogInfoFormat( "DetectCollisions World ObjectQuerySingle (%i) = %iys", timerObjectQuerySingleCount, timerObjectQuerySingle );
	timerUpdateOctrees = 0; timerUpdateOctreesCount = 0;
	timerUpdateDynWorldAABBs = 0; timerUpdateDynWorldAABBsCount = 0;
	timerRayHits = 0; timerRayHitsCount = 0;
	timerColliderHits = 0; timerColliderHitsCount = 0;
	timerColliderMoveHits = 0; timerColliderMoveHitsCount = 0;
	timerColliderRotateHits = 0; timerColliderRotateHitsCount = 0;
	timerColliderMoveRotateHits = 0; timerColliderMoveRotateHitsCount = 0;
	timerWorldUpdateAabbs = 0; timerWorldUpdateAabbsCount = 0;
	timerTSPositionChanged = 0; timerTSPositionChangedCount = 0;
	timerTSOrientationChanged = 0; timerTSOrientationChangedCount = 0;
	timerTSRayHits = 0; timerTSRayHitsCount = 0;
	timerTSColliderHits = 0; timerTSColliderHitsCount = 0;
	timerTSColliderMoveHits = 0; timerTSColliderMoveHitsCount = 0;
	timerTSColliderRotateHits = 0; timerTSColliderRotateHitsCount = 0;
	timerTSColliderMoveRotateHits = 0; timerTSColliderMoveRotateHitsCount = 0;
	timerShapeCastConvex = 0; timerShapeCastConvexCount = 0;
	timerObjectQuerySingle = 0; timerObjectQuerySingleCount = 0;
#endif
	
	pBullet.GetDebug().EndProcessPhysics( this );
	if( pBullet.GetDebug().GetEnabled() ){
		pBullet.LogInfoFormat( "Stats World: col=%d colObj=%d nonStaRigBod=%d broadNumOPairs=%d",
			pWorld.GetColliderCount(),
			pDynWorld->getNumCollisionObjects(),
			pDynWorld->GetNumNonStaticRigidBodies(),
			pDynWorld->getBroadphase()->getOverlappingPairCache()->getNumOverlappingPairs() );
		
		#if 0
		const btCollisionObjectArray &cobjs = pDynWorld->getCollisionObjectArray();
		for( int i=0; i<cobjs.size(); i++ ){
			if( ! cobjs.at( i )->isActive() ) continue;
			const debpCollisionObject &co0 = *( debpCollisionObject* )( cobjs.at( i )->getUserPointer() );
			decString text;
			text.Format( "- Active Collision Object %d: ", i );
			if( co0.IsOwnerCollider() ){
				const decCollisionFilter &cf = co0.GetOwnerCollider()->GetCollider().GetCollisionFilter();
				text.AppendFormat( "col %s:%s (%.1f,%1f,%1f)", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString(),
					co0.GetOwnerCollider()->GetCollider().GetPosition().x,
					co0.GetOwnerCollider()->GetCollider().GetPosition().y,
					co0.GetOwnerCollider()->GetCollider().GetPosition().z );
			}else if( co0.IsOwnerTouchSensor() ){
				const decCollisionFilter &cf = co0.GetOwnerTouchSensor()->GetTouchSensor().GetCollisionFilter();
				text.AppendFormat( "ts %s:%s (%.1f,%1f,%1f)", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString(),
					co0.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().x,
					co0.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().y,
					co0.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().z );
			}else{
				const decCollisionFilter &cf = co0.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter();
				text.AppendFormat( "ht %s:%s", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString() );
			}
			pBullet.LogInfo( text );
		}
		#endif
		
		#if 0
		btOverlappingPairCache &opc = *pDynWorld->getBroadphase()->getOverlappingPairCache();
		const btBroadphasePairArray &bpa = opc.getOverlappingPairArray();
		const int opcount = opc.getNumOverlappingPairs();
		for( int i=0; i<opcount; i++ ){
			const btBroadphasePair &pair = bpa.at( i );
			const debpCollisionObject &co0 = *( debpCollisionObject* )( ( ( btCollisionObject* )pair.m_pProxy0->m_clientObject )->getUserPointer() );
			const debpCollisionObject &co1 = *( debpCollisionObject* )( ( ( btCollisionObject* )pair.m_pProxy1->m_clientObject )->getUserPointer() );
			decString text;
			text.Format( "- Overlapping Pair %d: co0(", i );
			if( co0.IsOwnerCollider() ){
				const decCollisionFilter &cf = co0.GetOwnerCollider()->GetCollider().GetCollisionFilter();
				text.AppendFormat( "col %s:%s (%.1f,%1f,%1f)", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString(),
					co0.GetOwnerCollider()->GetCollider().GetPosition().x,
					co0.GetOwnerCollider()->GetCollider().GetPosition().y,
					co0.GetOwnerCollider()->GetCollider().GetPosition().z );
			}else if( co0.IsOwnerTouchSensor() ){
				const decCollisionFilter &cf = co0.GetOwnerTouchSensor()->GetTouchSensor().GetCollisionFilter();
				text.AppendFormat( "ts %s:%s (%.1f,%1f,%1f)", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString(),
					co0.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().x,
					co0.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().y,
					co0.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().z );
			}else{
				const decCollisionFilter &cf = co0.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter();
				text.AppendFormat( "ht %s:%s", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString() );
			}
			text.Append( ") co1(" );
			if( co1.IsOwnerCollider() ){
				const decCollisionFilter &cf = co1.GetOwnerCollider()->GetCollider().GetCollisionFilter();
				text.AppendFormat( "col %s:%s (%.1f,%1f,%1f)", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString(),
					co1.GetOwnerCollider()->GetCollider().GetPosition().x,
					co1.GetOwnerCollider()->GetCollider().GetPosition().y,
					co1.GetOwnerCollider()->GetCollider().GetPosition().z );
			}else if( co1.IsOwnerTouchSensor() ){
				const decCollisionFilter &cf = co1.GetOwnerTouchSensor()->GetTouchSensor().GetCollisionFilter();
				text.AppendFormat( "ts %s:%s (%.1f,%1f,%1f)", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString(),
					co1.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().x,
					co1.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().y,
					co1.GetOwnerTouchSensor()->GetTouchSensor().GetPosition().z );
			}else{
				const decCollisionFilter &cf = co1.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter();
				text.AppendFormat( "ht %s:%s", cf.GetCategory().ToBitString().GetLeft(12).GetString(),
					cf.GetFilter().ToBitString().GetLeft(12).GetString() );
			}
			text.Append( ")" );
			pBullet.LogInfo( text );
		}
		#endif
	}
	
	// debug drawer update
	if( pBullet.GetDeveloperMode().GetEnabled()
	&& pBullet.GetDeveloperMode().GetShowCategory().MatchesNot( pLastDebugShowCategory ) ){
		pLastDebugShowCategory = pBullet.GetDeveloperMode().GetShowCategory();
		deCollider *collider = pWorld.GetRootCollider();
		while( collider ){
			( ( debpCollider* )collider->GetPeerPhysics() )->UpdateDebugDrawer();
			collider = collider->GetLLWorldNext();
		}
	}
}



// Notifications
//////////////////

void debpWorld::SizeChanged(){
}

void debpWorld::PhysicsChanged(){
	// check if gravity changed
	const decVector &gravity = pWorld.GetGravity();
	if( ! gravity.IsEqualTo( pGravity ) ){
		pGravity = gravity;
		
		// notify colliders the gravity potentially changed
		deCollider *collider = pWorld.GetRootCollider();
		while( collider ){
			( ( debpCollider* )collider->GetPeerPhysics() )->GravityChanged();
			collider = collider->GetLLWorldNext();
		}
		
		// update the gravity in all particle systems
		/*
		count = pWorld.GetParticleEmitterCount();
		for( i=0; i<count; i++ ){
			( ( debpParticleEmitter* )pWorld.GetParticleEmitterAt( i )->GetPeerPhysics() )->GravityChanged();
		}
		*/
		
		// update the dynamic collision impulse threshold. see debpCollisionWorld::CheckDynamicCollisions
		// for why this is calculated like this. Gravity times 1 is the minimum amount of impulse applied
		// by gravity. to avoid incorrect results a value of gravity times 1.2 or even 1.5 is used.
		pDynCollisionVelocityThreshold = decMath::max( gravity.Length() * 1.2f, 1.0f ) * pSimTimeStep;
	}
	
	// update octrees
	UpdateOctrees();
}

void debpWorld::HeightTerrainChanged(){
	deHeightTerrain *heightTerrain = pWorld.GetHeightTerrain();
	
	if( pHeightTerrain ){
		pHeightTerrain->SetParentWorld( NULL );
		pHeightTerrain = NULL;
	}
	
	if( heightTerrain ){
		pHeightTerrain = ( debpHeightTerrain* )heightTerrain->GetPeerPhysics();
		pHeightTerrain->SetParentWorld( this );
	}
}

void debpWorld::ComponentAdded( deComponent *component ){
	(void)component;
}

void debpWorld::ComponentRemoved( deComponent *component ){
	(void)component;
}

void debpWorld::AllComponentsRemoved(){
}

void debpWorld::ColliderAdded( deCollider *collider ){
	( ( debpCollider* )collider->GetPeerPhysics() )->SetParentWorld( this );
}

void debpWorld::ColliderRemoved( deCollider *collider ){
	( ( debpCollider* )collider->GetPeerPhysics() )->SetParentWorld( NULL );
	
	if( pDetNextCollider == collider ){
		pDetNextCollider = pDetNextCollider->GetLLWorldNext();
	}
}

void debpWorld::AllCollidersRemoved(){
	deCollider *collider = pWorld.GetRootCollider();
	while( collider ){
		debpCollider * const bpCollider = ( debpCollider* )collider->GetPeerPhysics();
		if( bpCollider ){
			bpCollider->SetParentWorld( NULL );
		}
		collider = collider->GetLLWorldNext();
	}
	
	pDetNextCollider = NULL;
}

void debpWorld::TouchSensorAdded( deTouchSensor *touchSensor ){
	( ( debpTouchSensor* )touchSensor->GetPeerPhysics() )->SetParentWorld( this );
}

void debpWorld::TouchSensorRemoved( deTouchSensor *touchSensor ){
	if( pDetNextTouchSensor == touchSensor ){
		pDetNextTouchSensor = pDetNextTouchSensor->GetLLWorldNext();
	}
	
	( ( debpTouchSensor* )touchSensor->GetPeerPhysics() )->SetParentWorld( NULL );
}

void debpWorld::AllTouchSensorsRemoved(){
	pDetNextTouchSensor = NULL;
	
	deTouchSensor *touchSensor = pWorld.GetRootTouchSensor();
	while( touchSensor ){
		debpTouchSensor * const bpTouchSensor = ( debpTouchSensor* )touchSensor->GetPeerPhysics();
		if( bpTouchSensor ){
			bpTouchSensor->SetParentWorld( NULL );
		}
		touchSensor = touchSensor->GetLLWorldNext();
	}
}



void debpWorld::PropFieldAdded( dePropField *propField ){
	(void)propField;
}

void debpWorld::PropFieldRemoved( dePropField *propField ){
	(void)propField;
}

void debpWorld::AllPropFieldsRemoved(){
}



void debpWorld::ForceFieldAdded( deForceField *forceField ){
	(void)forceField;
}

void debpWorld::ForceFieldRemoved( deForceField *forceField ){
	(void)forceField;
}

void debpWorld::AllForceFieldsRemoved(){
}



void debpWorld::ParticleEmitterAdded( deParticleEmitterInstance *emitter ){
	debpParticleEmitterInstance *peer = ( debpParticleEmitterInstance* )emitter->GetPeerPhysics();
	
	peer->SetParentWorld( this );
}

void debpWorld::ParticleEmitterRemoved( deParticleEmitterInstance *emitter ){
	debpParticleEmitterInstance *peer = ( debpParticleEmitterInstance* )emitter->GetPeerPhysics();
	
	peer->SetParentWorld( NULL );
}

void debpWorld::AllParticleEmittersRemoved(){
	deParticleEmitterInstance *emitter = pWorld.GetRootParticleEmitter();
	debpParticleEmitterInstance *peer;
	
	while( emitter ){
		peer = ( debpParticleEmitterInstance* )emitter->GetPeerPhysics();
		emitter = emitter->GetLLWorldNext();
		
		if( peer ){
			peer->SetParentWorld( NULL );
		}
	}
}



// Collision Detection
////////////////////////

void debpWorld::PointHits( const decDVector &point, deBaseScriptingCollider *listener,
const decCollisionFilter &collisionFilter ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pBullet.GetCollisionDetection().PointHits( point, *this, collisionFilter, *listener );
}

void debpWorld::RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
deBaseScriptingCollider *listener, const decCollisionFilter &collisionFilter ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	//printf( "RayHits (%f,%f,%f)(%f,%f,%f)\n", rayOrigin.x, rayOrigin.y, rayOrigin.z, rayDirection.x, rayDirection.y, rayDirection.z );
	pBullet.GetCollisionDetection().RayHits( rayOrigin, rayDirection, *this, collisionFilter, *listener );
#ifdef DO_TIMING2
timerRayHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerRayHitsCount++;
#endif
}

void debpWorld::ColliderHits( deCollider *collider, deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	// do we need an update octree?
	UpdateOctrees();
	
	debpCollider * const bpcollider = ( debpCollider* )collider->GetPeerPhysics();
	pBullet.GetCollisionDetection().ColliderHits( bpcollider, this, listener );
#ifdef DO_TIMING2
timerColliderHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerColliderHitsCount++;
#endif
}

void debpWorld::ColliderMoveHits( deCollider *collider, const decVector &displacement, deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	UpdateOctrees();
	
	debpCollider * const bpcollider = ( debpCollider* )collider->GetPeerPhysics();
	pBullet.GetCollisionDetection().ColliderMoveHits( bpcollider, displacement, this, listener );
#ifdef DO_TIMING2
timerColliderMoveHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerColliderMoveHitsCount++;
#endif
}

void debpWorld::ColliderRotateHits( deCollider *collider, const decVector &rotation, deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	UpdateOctrees();
	
	debpCollider * const bpcollider = ( debpCollider* )collider->GetPeerPhysics();
	pBullet.GetCollisionDetection().ColliderRotateHits( bpcollider, rotation, this, listener );
#ifdef DO_TIMING2
timerColliderRotateHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerColliderRotateHitsCount++;
#endif
}

void debpWorld::ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener ){
#ifdef DO_TIMING2
timer.Reset();
#endif
	if( ! collider || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	UpdateOctrees();
	
	debpCollider * const bpcollider = ( debpCollider* )collider->GetPeerPhysics();
	pBullet.GetCollisionDetection().ColliderMoveRotateHits( bpcollider, displacement, rotation, this, listener );
#ifdef DO_TIMING2
timerColliderRotateHits += ( int )( timer.GetElapsedTime() * 1e6f );
timerColliderRotateHitsCount++;
#endif
}



// private functions
//////////////////////

void debpWorld::pCleanUp(){
	AllTouchSensorsRemoved();
	AllComponentsRemoved();
	AllCollidersRemoved();
	
	if( pHeightTerrain ){
		pHeightTerrain->SetParentWorld( NULL );
		pHeightTerrain = NULL;
	}
	
	if( pUnstuckCollider ){
		delete pUnstuckCollider;
	}
	if( pColInfo ){
		pColInfo->FreeReference();
	}
	
	if( pUpdateOctreeColliders ){
		delete [] pUpdateOctreeColliders;
	}
	if( pPPCTColliders ){
		delete [] pPPCTColliders;
	}
	if( pColDetFinishColliders ){
		delete [] pColDetFinishColliders;
	}
	if( pColDetPrepareColliders ){
		delete [] pColDetPrepareColliders;
	}
	if( pDynWorld ){
		delete pDynWorld;
	}
	if( pSoftBodyWorldInfo ){
		delete pSoftBodyWorldInfo;
	}
	if( pGhostPairCallback ){
		delete pGhostPairCallback;
	}
	if( pOverlapFilterCallback ){
		delete pOverlapFilterCallback;
	}
	if( pSoftBodySolver ){
		delete pSoftBodySolver;
	}
	if( pConstraintSolver ){
		delete pConstraintSolver;
	}
	if( pBroadPhase ){
		delete pBroadPhase;
	}
	
	if( pSharedCollisionFiltering ){
		delete pSharedCollisionFiltering;
	}
	
	if( pColDisp ){
		delete pColDisp;
	}
	if( pColConfig ){
		delete pColConfig;
	}
}



void debpWorld::pProcessPhysics( float elapsed ){
	// prevent rigid bodies from updating transforms. updating transform is required
	// for teleporting objects without causing the displacement to be considered a
	// huge velocity. updating transforms though causes dynamic bodies to not be
	// moved if in contact with moving kinematic bodies. hence calls to
	// DetectCustomCollision until stepSimulation have to be protected
	pProcessingPhysics = true;
	
	// prepare for detection
	pPrepareDetection( elapsed );
DEBUG_PRINT_TIMER( "Prepare Detection" );
	
	// process kinematic physics. this moves collider with kinematic response type
	// first along their velocity and then along the gravity. colliders are moved
	// one by one. this is not the best solution yet but it works for the time
	// being. one of the main problems is that if a collider has been moved already
	// and then is altered by a collision later on it is not processed again. one
	// solution would be to use a processed flag or list and re-process colliders
	// if they get touched during collision detection.
	if( elapsed > 1e-6f ){
		pColInfo->Clear();
		
		UpdateOctrees(); // deprecated
		UpdateDynWorldAABBs();
		
		debpDebugInformation *debugInfo = NULL;
		if( pBullet.GetDebug().GetEnabled() ){
			debugInfo = pBullet.GetDebug().GetDIColliderDetectCustomCollision();
			pPerfTimer.Reset();
		}
		
		int i;
		for( i=0; i<pColDetPrepareColliderProcessCount; i++ ){
			if( ! pColDetPrepareColliders[ i ] ){
				continue;
			}
			pColDetPrepareColliders[ i ]->DetectCustomCollision( elapsed );
			
			if( debugInfo ){
				debugInfo->IncrementElapsedTime( pPerfTimer.GetElapsedTime() );
				debugInfo->IncrementCounter( 1 );
			}
		}
	}
DEBUG_PRINT_TIMER( "Detection Loop" );
	
	// process dynamic physics. this does a bullet simulation step. due to collisions
	// with kinematic colliders they can obtain new velocities. these are though
	// ignored and are considered to be taken into account in the next update.
	pPrepareForStep();
DEBUG_PRINT_TIMER( "Prepare For Step" );
	
	// TODO
	// this runs only one simulation step. if the elapsed time is larger than 1/60
	// the simulation slows down as well as producing weird results. this has to be
	// fixed somehow but how is unknown yet (too many recent changes in the code base).
	#ifdef DO_TIMING
		pBullet.LogInfoFormat( "World: colObj=%d nonStaRigBod=%d broadNumOPairs=%d",
			pDynWorld->getNumCollisionObjects(), pDynWorld->GetNumNonStaticRigidBodies(),
			pDynWorld->getBroadphase()->getOverlappingPairCache()->getNumOverlappingPairs() );
	#endif
	if( pBullet.GetDebug().GetEnabled() ){
		debpDebugInformation &debugInfo = *pBullet.GetDebug().GetDIWorldStepSimulation();
		decTimer timer;
		pDynWorld->stepSimulation( elapsed, pSimMaxSubStep, pSimTimeStep );
		debugInfo.IncrementElapsedTime( timer.GetElapsedTime() );
		debugInfo.IncrementCounter( 1 );
		
	}else{
		pDynWorld->stepSimulation( elapsed, pSimMaxSubStep, pSimTimeStep );
	}
	
	#ifndef BT_NO_PROFILE
		#ifdef DO_TIMING
			//CProfileManager::dumpAll();
		#endif
		if( pBullet.GetDebug().GetEnabled() ){
			CProfileManager::dumpAll();
		}
	#endif
	pDynWorld->MarkAllAABBValid();
	pDirtyDynWorldAABB = false;
DEBUG_PRINT_TIMER( "Step Simulation" );
	
	// now updating rigid body transforms can be enabled again
	pProcessingPhysics = false;
	
	// update positions
	pUpdateFromBody();
DEBUG_PRINT_TIMER( "Update Positions" );
	
	// finish detection. this operates on colliders. this is required to be done before
	// elements relying on colliders potentially changing their geometry information
	pFinishDetection();
DEBUG_PRINT_TIMER( "Finish Detection" );
	
	// make touch sensors notify their peers about touch changes accumulated during collision
	// detection. this potentially modifies colliders including adding or removing them
	pApplyTouchSensorChanges();
DEBUG_PRINT_TIMER( "Apply Touch Sensor Changes" );
	
	// update collider post physics collision tests
	pUpdatePostPhysicsCollisionTests();
DEBUG_PRINT_TIMER( "Update Post Physics Collision Tests" );
	
	// simulate particles. this is done after colliders since particle emitters are potentially
	// moved by due to colliders moving hence they have to be simulated after colliders
	pStepParticleEmitters( elapsed );
DEBUG_PRINT_TIMER( "Particle Emitters: Step" );
	
	// this is a bit hacky right now. we do a full run using the force fields.
	// a better solution has to be implemented later on
	pStepForceFields( elapsed );
DEBUG_PRINT_TIMER( "Step Force Fields" );
}

void debpWorld::pPrepareDetection( float elapsed ){
	// WARNING colliders required to call RegisterColDetPrepare() if force fields touch them.
	//         requires keeping track of which collider touches which force field. one solution
	//         would be to call RegisterColDetPrepare() inside force field Updaet() call for
	
	// prepare force fields for collision detection
	deForceField *forceField = pWorld.GetRootForceField();
	while( forceField ){
		( ( debpForceField* )forceField->GetPeerPhysics() )->Update( elapsed );
		forceField = forceField->GetLLWorldNext();
	}
	
	// prepare colliders for collision detection
	debpDebugInformation *debugInfo = NULL;
	if( pBullet.GetDebug().GetEnabled() ){
		debugInfo = pBullet.GetDebug().GetDIColliderPrepareDetection();
		pPerfTimer.Reset();
	}
	
	int i;
	pColDetPrepareColliderProcessCount = pColDetPrepareColliderCount;
	for( i=0; i<pColDetPrepareColliderProcessCount; i++ ){
		if( ! pColDetPrepareColliders[ i ] ){
			continue;
		}
		
		pColDetPrepareColliders[ i ]->PrepareDetection( elapsed );
		
		if( debugInfo ){
			debugInfo->IncrementElapsedTime( pPerfTimer.GetElapsedTime() );
			debugInfo->IncrementCounter( 1 );
		}
	}
	
// 	pBullet.LogInfoFormat( "World.Prepare: count=%d/%d", pColDetPrepareColliderProcessCount, pWorld.GetColliderCount() );
}



void debpWorld::pPrepareForStep(){
	UpdateOctrees();
	
	int i;
	for( i=0; i<pColDetPrepareColliderProcessCount; i++ ){
		if( pColDetPrepareColliders[ i ] ){
			pColDetPrepareColliders[ i ]->PrepareForStep();
		}
	}
}

#if 0
bool debpWorld::pStepPhysics(){
	float defaultStepSize = 0.01f;
	float stepSize;
	
/*
#ifdef DO_TIMING
float elapsedPrepare = 0.0f;
float elapsedStep = 0.0f;
float elapsedUpdate = 0.0f;
decTimer timer;
#endif
*/
	
	// process physics
	stepSize = defaultStepSize;
	while( pLeftOverTime >= stepSize ){
		// prepare for the next step
		pPrepareForStep();
/*
#ifdef DO_TIMING
elapsedPrepare += timer.GetElapsedTime();
#ifdef USE_QUICKPROF
btProfiler::init( "bullet.profiling", btProfiler::BLOCK_TOTAL_MICROSECONDS );
#endif
#endif
*/
		
		// step simulation
		pDynWorld->stepSimulation( stepSize, 1 );
/*
#ifdef DO_TIMING
elapsedStep += timer.GetElapsedTime();
#ifdef USE_QUICKPROF
btProfiler::endProfilingCycle();
#endif
#endif
*/
		
		// update positions
		pUpdateFromBody();
/*
#ifdef DO_TIMING
elapsedUpdate += timer.GetElapsedTime();
#endif
*/
		
		// update elapsed time
		pLeftOverTime -= stepSize;
	}
/*
#ifdef DO_TIMING
pBullet.LogInfoFormat( "World Timer: pStepPhysics: Prepare = %iys", ( int )( elapsedPrepare * 1000000.0 ) );
pBullet.LogInfoFormat( "World Timer: pStepPhysics: Step = %iys", ( int )( elapsedStep * 1000000.0 ) );
pBullet.LogInfoFormat( "World Timer: pStepPhysics: Update = %iys", ( int )( elapsedUpdate * 1000000.0 ) );
#endif
*/
	
	// no collision happened
	return false;
}
#endif

void debpWorld::pStepForceFields( float elapsed ){
	if( ! pBullet.GetConfiguration()->GetSimulatePropFields() ){
		return;
	}
	
	// optimization ideas:
	// 1) store for each instance 2 byte vectors which point into the x and
	//    z bending direction. the vectors are encoded using 1 byte for each
	//    component mapping [-128,127] to [-1,1] . this way the rotation
	//    matrix calculation and the normal transform can be replaced with
	//    a simple ( and already existing ) dot product.
	//
	float bx, bz, rotPerForce, bvx, bvz;
	dePropFieldBendState *engPFBendStates;
	debpPropFieldBendState *pfBendStates;
	dePropFieldType *engPFType;
	int ptb, pftBendStateCount;
	debpPropFieldType *pfType;
	decVector localPosition;
	int pt, pfTypeCount;
	decVector direction;
	decMatrix matrix;
	float distance;
	//float vortexStrength;
	
	//float elapsedDamped = 0.5f * elapsed; // 5%
	
	// update prop fields if required
	dePropField *propField = pWorld.GetRootPropField();
	while( propField ){
		( ( debpPropField* )propField->GetPeerPhysics() )->Update( elapsed );
		propField = propField->GetLLWorldNext();
	}
	
	const float flucAngle = DEG2RAD * 180.0f;
	float flucDirSin, flucDirCos, flucStr;
	float forceFactor;
	
	// apply force field influences
	deForceField *forceField = pWorld.GetRootForceField();
	while( forceField ){
		debpForceField &btForceField = *( ( debpForceField* )forceField->GetPeerPhysics() );
		
		//const decMatrix &ffMatVortex = forceField->GetVortexMatrix();
		const decDVector &ffpos = forceField->GetPosition();
		const decVector &ffDir = btForceField.GetDirection();
		const float ffForce = forceField->GetForce();
		const float ffRadius = forceField->GetRadius();
		const float ffExponent = forceField->GetExponent();
		const float ffFluctuationDirection = forceField->GetFluctuationDirection();
		const float ffFluctuationForce = forceField->GetFluctuationForce();
		const deForceField::eFieldTypes ffFieldType = forceField->GetFieldType();
		const deForceField::eApplicationTypes ffApplyType = forceField->GetApplicationType();
		
		if( ffApplyType == deForceField::eatSpeed ){
			forceField = forceField->GetLLWorldNext();
			continue; // props do not move so this force field has no effect
		}
		
		propField = pWorld.GetRootPropField();
		while( propField ){
			const decDVector &pfpos = propField->GetPosition();
			
			debpPropField &btPropField = *( ( debpPropField* )propField->GetPeerPhysics() );
			pfTypeCount = propField->GetTypeCount();
			
			// TODO test if the prop field is inside the area of influence of the force field
			
			localPosition = ( ffpos - pfpos ).ToVector();
			
			for( pt=0; pt<pfTypeCount; pt++ ){
				engPFType = propField->GetTypeAt( pt );
				
				if( forceField->GetCollisionFilter().CollidesNot( engPFType->GetCollisionFilter() ) ){
					continue;
				}
				
				pfType = btPropField.GetTypeAt( pt );
				
				pftBendStateCount = engPFType->GetBendStateCount();
				engPFBendStates = engPFType->GetBendStates();
				
				switch( ffApplyType ){
				case deForceField::eatDirect:
					forceFactor = 1.0f;
					break;
					
				case deForceField::eatSurface:
					// TODO prop field type has no information about size of prop yet. to get the right
					//      result for the test case a value of 0.285 is used. this is the area of the
					//      farn model. for the final solution these values need to result:
					//      - farn: 0.75 * 0.38 = 0.285 (volume: * 0.75 = 0.213)
					//      - plant1: 0.35 * 0.37 = 0.13 (volume: * 0.23 = 0.03)
					//      - plant2: 0.32 * 0.66 = 0.211 (volume: * 0.4 = 0.084)
					forceFactor = 0.285f;
					break;
					
				case deForceField::eatMass:
					forceFactor = 0.213;
					break;
					
				default:
					DETHROW( deeInvalidParam );
				}
				
				rotPerForce = ffForce * forceFactor * engPFType->GetRotationPerForce();
				
				pfBendStates = pfType->GetBendStates();
				
				for( ptb=0; ptb<pftBendStateCount; ptb++ ){
					const debpPropFieldBendState &bstate = pfBendStates[ ptb ];
					dePropFieldBendState &engBState = engPFBendStates[ ptb ];
					const decVector &ipos = bstate.position;
					
					direction = ipos - localPosition;
					distance = direction.Length();
					
					if( distance > 1e-5f && distance < ffRadius ){
						if( ffFieldType == deForceField::eftLinear ){
							direction = ffDir;
							
						}else{
							direction /= distance;
						}
						
						//vortexStrength = distance / ffRadius;
						//direction += ffMatVortex.TransformNormal( direction ) * vortexStrength;
						
						bvx = engBState.GetVelocityX();
						bvz = engBState.GetVelocityZ();
						
						flucDirSin = sinf( bstate.flucDir * ffFluctuationDirection * flucAngle );
						flucDirCos = cosf( bstate.flucDir * ffFluctuationDirection * flucAngle );
						flucStr = rotPerForce * ( 1.0f + ffFluctuationForce * bstate.flucStr )
							* powf( 1.0f - distance / ffRadius, ffExponent ) * elapsed;
						
						bvx -= ( direction.z * flucDirCos - direction.x * flucDirSin ) * flucStr;
						bvz += ( direction.x * flucDirCos + direction.z * flucDirSin ) * flucStr;
						
						engBState.SetVelocityX( bvx );
						engBState.SetVelocityZ( bvz );
					}
				}
			}
			
			propField = propField->GetLLWorldNext();
		}
		
		forceField = forceField->GetLLWorldNext();
	}
	
	// apply restoration force and update position
	float damping = powf( 1.0f - 0.5f, elapsed ); //1.0f - 0.2f * elapsed; // 5%
	float minBendX = DEG2RAD * -90.0f;
	float maxBendX = DEG2RAD * 90.0f;
	float minBendZ = DEG2RAD * -90.0f;
	float maxBendZ = DEG2RAD * 90.0f;
	float restitution;
	
	propField = pWorld.GetRootPropField();
	while( propField ){
		//debpPropField &bpPropField = *( ( debpPropField* )propField->GetPeerPhysics() );
		pfTypeCount = propField->GetTypeCount();
		
		for( pt=0; pt<pfTypeCount; pt++ ){
			engPFType = propField->GetTypeAt( pt );
			pftBendStateCount = engPFType->GetBendStateCount();
			engPFBendStates = engPFType->GetBendStates();
			restitution = engPFType->GetRestitution() * elapsed;
			
			for( ptb=0; ptb<pftBendStateCount; ptb++ ){
				dePropFieldBendState &engBState = engPFBendStates[ ptb ];
				
				bvx = engBState.GetVelocityX();
				bvz = engBState.GetVelocityZ();
				bx = engBState.GetBendX();
				bz = engBState.GetBendZ();
				
				bvx = ( bvx - bx * restitution ) * damping;
				bvz = ( bvz - bz * restitution ) * damping;
				bx = decMath::clamp( bx + bvx * elapsed, minBendX, maxBendX );
				bz = decMath::clamp( bz + bvz * elapsed, minBendZ, maxBendZ );
				
				engBState.SetVelocityX( bvx );
				engBState.SetVelocityZ( bvz );
				engBState.SetBendX( bx );
				engBState.SetBendZ( bz );
			}
			
			propField->NotifyBendStatesChanged( pt );
		}
		
		propField = propField->GetLLWorldNext();
	}
}



void debpWorld::pPrepareParticleEmitters( float elapsed ){
	// not used right now
}

void debpWorld::pStepParticleEmitters( float elapsed ){
// decTimer timer2;
	// prepare particles. this applies also force fields if present
	deParticleEmitterInstance *engEmitterInstance = pWorld.GetRootParticleEmitter();
	while( engEmitterInstance ){
		debpParticleEmitterInstance &emitterInstance = *( ( debpParticleEmitterInstance* )engEmitterInstance->GetPeerPhysics() );
		engEmitterInstance = engEmitterInstance->GetLLWorldNext();
		emitterInstance.PrepareParticles( elapsed );
	}
	
	// stpe particles
	const float maxStepSize = 1.0f / 30.0f;
	while( elapsed > FLOAT_SAFE_EPSILON ){
		const float stepElapsed = decMath::min( elapsed, maxStepSize );
		
		//int debugCount = 0;
		engEmitterInstance = pWorld.GetRootParticleEmitter();
		while( engEmitterInstance ){
			debpParticleEmitterInstance &emitterInstance = *( ( debpParticleEmitterInstance* )engEmitterInstance->GetPeerPhysics() );
			engEmitterInstance = engEmitterInstance->GetLLWorldNext();
			emitterInstance.StepParticles( stepElapsed );
			//debugCount++;
		}
		//pBullet.LogInfoFormat( "pParticleEmittersStep: processed instances %i\n", debugCount );
		
		elapsed -= stepElapsed;
	}
	
	engEmitterInstance = pWorld.GetRootParticleEmitter();
	while( engEmitterInstance ){
		debpParticleEmitterInstance &emitterInstance = *( ( debpParticleEmitterInstance* )engEmitterInstance->GetPeerPhysics() );
		engEmitterInstance = engEmitterInstance->GetLLWorldNext();
		emitterInstance.FinishStepping();
	}
	
//pBullet.LogInfoFormat( "pParticleEmittersStep = %iys", ( int )( timer2.GetElapsedTime() * 1000000.0f ) );
//printf( "pParticleEmittersStep = %iys\n", ( int )( timer2.GetElapsedTime() * 1000000.0f ) );
}



void debpWorld::pUpdateFromBody(){
	debpDebugInformation *debugInfoUpdateFromBody = NULL;
	if( pBullet.GetDebug().GetEnabled() ){
		debugInfoUpdateFromBody = pBullet.GetDebug().GetDIColliderUpdateFromBody();
		pPerfTimer.Reset();
	}
	
	int i;
	for( i=0; i<pColDetPrepareColliderProcessCount; i++ ){
		if( ! pColDetPrepareColliders[ i ] ){
			continue;
		}
		
		pColDetPrepareColliders[ i ]->UpdateFromBody();
		
		if( debugInfoUpdateFromBody ){
			debugInfoUpdateFromBody->IncrementElapsedTime( pPerfTimer.GetElapsedTime() );
			debugInfoUpdateFromBody->IncrementCounter( 1 );
		}
	}
}

void debpWorld::pFinishDetection(){
	int i, next;
	
	// clear registered prepare
	for( next=0, i=0; i<pColDetPrepareColliderProcessCount; i++ ){
		debpCollider * const collider = pColDetPrepareColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		if( collider->GetAutoColDetPrepare() ){
			if( i != next ){
				pColDetPrepareColliders[ next ] = collider;
				pColDetPrepareColliders[ i ] = NULL;
				collider->SetColDetPrepareIndex( next );
			}
			next++;
			
		}else{
			pColDetPrepareColliders[ i ] = NULL;
			collider->SetColDetPrepareIndex( -1 );
		}
	}
	
	for( i=pColDetPrepareColliderProcessCount; i<pColDetPrepareColliderCount; i++ ){
		debpCollider * const collider = pColDetPrepareColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		if( i != next ){
			pColDetPrepareColliders[ next ] = collider;
			pColDetPrepareColliders[ i ] = NULL;
			collider->SetColDetPrepareIndex( next );
		}
		next++;
	}
	
// 	pBullet.LogInfoFormat( "World.Finish: XXX in=%d out=%d colliders=%d", pColDetPrepareColliderCount, next, pWorld.GetColliderCount() );
	pColDetPrepareColliderCount = next;
	pColDetPrepareColliderProcessCount = 0;
	
	// finish detection and clear registered finish
	const int finishCount = pColDetFinishColliderCount;
	debpDebugInformation *debugInfo = NULL;
	
	if( pBullet.GetDebug().GetEnabled() ){
		debugInfo = pBullet.GetDebug().GetDIColliderFinishDetection();
		pPerfTimer.Reset();
	}
	
	for( next=0, i=0; i<finishCount; i++ ){
		debpCollider *collider = pColDetFinishColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		// NOTE we have to do FinishDetection before potentially unregistering the collider.
		//      required since FinishDetection typically calls into the scripting module
		//      causing collider changes potentially triggering registering the collider
		//      again. if done after removing we can end up with colliders permanently
		//      re-registering themselves although not required at all
		collider->FinishDetection();
		
		// WARNING collider is potentially NULL after FinishDetection returns
		collider = pColDetFinishColliders[ i ];
		
		if( collider ){
			if( collider->GetAutoColDetFinish() ){
				if( i != next ){
					pColDetFinishColliders[ next ] = collider;
					pColDetFinishColliders[ i ] = NULL;
					collider->SetColDetFinishIndex( next );
				}
				next++;
				
			}else{
				pColDetFinishColliders[ i ] = NULL;
				collider->SetColDetFinishIndex( -1 );
			}
		}
		
		if( debugInfo ){
			debugInfo->IncrementElapsedTime( pPerfTimer.GetElapsedTime() );
			debugInfo->IncrementCounter( 1 );
		}
	}
	
	for( i=finishCount; i<pColDetFinishColliderCount; i++ ){
		debpCollider * const collider = pColDetFinishColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		if( i != next ){
			pColDetFinishColliders[ next ] = collider;
			pColDetFinishColliders[ i ] = NULL;
			collider->SetColDetFinishIndex( next );
		}
		next++;
	}
	
	pColDetFinishColliderCount = next;
//  	pBullet.LogInfoFormat( "World.Finish: in=%d out=%d colliders=%d", finishCount, pColDetFinishColliderCount, pWorld.GetColliderCount() );
}

void debpWorld::pUpdatePostPhysicsCollisionTests(){
	const int count = pPPCTColliderCount;
	debpDebugInformation *debugInfo = NULL;
	int i, next;
	
	if( pBullet.GetDebug().GetEnabled() ){
		debugInfo = pBullet.GetDebug().GetDIColliderCollisionTests();
		pPerfTimer.Reset();
	}
	
	for( next=0, i=0; i<count; i++ ){
		debpCollider * const collider = pPPCTColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		if( i != next ){
			pPPCTColliders[ next ] = collider;
			pPPCTColliders[ i ] = NULL;
			collider->SetPPCProcessingIndex( next );
		}
		next++;
		
		collider->ProcessColliderCollisionTests();
		
		if( debugInfo ){
			debugInfo->IncrementElapsedTime( pPerfTimer.GetElapsedTime() );
			debugInfo->IncrementCounter( 1 );
		}
	}
	
	for( i=count; i<pPPCTColliderCount; i++ ){
		debpCollider * const collider = pPPCTColliders[ i ];
		if( ! collider ){
			continue;
		}
		
		if( i != next ){
			pPPCTColliders[ next ] = collider;
			pPPCTColliders[ i ] = NULL;
			collider->SetPPCProcessingIndex( next );
		}
		next++;
	}
	
	pPPCTColliderCount = next;
}

void debpWorld::pApplyTouchSensorChanges(){
	debpDebugInformation *debugInfo = NULL;
	if( pBullet.GetDebug().GetEnabled() ){
		debugInfo = pBullet.GetDebug().GetDITouchSensorApplyChanges();
		pPerfTimer.Reset();
	}
	
	pDetNextTouchSensor = pWorld.GetRootTouchSensor();
	while( pDetNextTouchSensor ){
		deTouchSensor * const touchSensor = pDetNextTouchSensor;
		pDetNextTouchSensor = pDetNextTouchSensor->GetLLWorldNext();
		( ( debpTouchSensor* )touchSensor->GetPeerPhysics() )->ApplyChanges();
		
		if( debugInfo ){
			debugInfo->IncrementElapsedTime( pPerfTimer.GetElapsedTime() );
			debugInfo->IncrementCounter( 1 );
		}
	}
}
