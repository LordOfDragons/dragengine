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

#include "debpCollisionWorld.h"
#include "debpConstraintSolver.h"
#include "debpDelayedOperation.h"
#include "../dePhysicsBullet.h"
#include "../debpCollisionObject.h"
#include "../coldet/debpCollisionDetection.h"
#include "../coldet/debpBulletShapeCollision.h"
#include "../coldet/collision/debpDECollisionDetection.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../debug/debpDebug.h"
#include "../debug/debpDebugInformation.h"
#include "../world/debpWorld.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"



// #define DO_TIMING
// #define DO_TIMING_2

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	static decTimer timer;
	#define DEBUG_RESET_TIMERS				timer.Reset();
	#define DEBUG_PRINT_TIMER(what)			pWorld.GetBullet().LogInfoFormat("World Timer: %s = %iys", what, (int)(timer.GetElapsedTime() * 1000000.0))
	#ifdef DO_TIMING_2
		int timerWorldUpdateAabbs = 0;
		int timerWorldUpdateAabbsCount = 0;
	#endif
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
#endif



// Dummy manifold point to be used where bullet requires one but dragengine not
static btManifoldPoint vDummyManifoldPoint;

/**
 * \brief Contact callback replacing bullet collision testing with dragengine collision testing.
 * \details Required for the time being due to problems with bullet not using early exit for
 *          contact collision tests but instead calculating tons of contact points.
 */
class HitContactCallback : public btBroadphaseAabbCallback{
private:
	btCollisionObject *pCollisionObject;
	btCollisionWorld::ContactResultCallback &pResultCallback;
	debpCollider *pCollider;
	debpCollisionDetection &pColDet;
	debpCollisionResult pResult;
	btCollisionObjectWrapper pObj0Wrapper;
	
public:
	/** \brief Create a new callback. */
	HitContactCallback(debpCollisionWorld &world, btCollisionObject *collisionObject,
	btCollisionWorld::ContactResultCallback &resultCallback) :
	pCollisionObject(collisionObject),
	pResultCallback(resultCallback),
	pColDet(world.GetWorld().GetBullet().GetCollisionDetection()),
	pObj0Wrapper(NULL, collisionObject->getCollisionShape(), collisionObject, collisionObject->getWorldTransform(), -1, -1)
	{
		const debpCollisionObject &colobj = *((debpCollisionObject*)collisionObject->getUserPointer());
		pCollider = colobj.GetOwnerCollider();
	}
	
	/**
	 * \brief Process a collision proxy.
	 * \details This call is a modified version from btSingleContactCallback::process. The original
	 *          method does call algorithm collision functions which calculate all contact points.
	 *          This does not only potentially call btCollisionWorld::ContactResultCallback::addSingleResult
	 *          multiple times it also cases major slowdown with triangle meshes due to testing every
	 *          triangle in the mesh. Required is though only a first hit test where the rest of the
	 *          calculation is avoided once a hit is found. Since this is not possible with bullet as
	 *          it is right now the dragengine collision routines are used for the time being.
	 */
	virtual bool process(const btBroadphaseProxy *proxy){
		btCollisionObject * const bpColObj = (btCollisionObject*)proxy->m_clientObject;
		if(bpColObj == pCollisionObject){
			return true;
		}
		
		// only perform raycast if filterMask matches
		if(pResultCallback.needsCollision(bpColObj->getBroadphaseHandle())){
			const debpCollisionObject &colobj = *((debpCollisionObject*)bpColObj->getUserPointer());
			
			if(pCollider){
				pCollider->UpdateShapes();
			}
			
			// test against collider
			if(colobj.IsOwnerCollider()){
				debpCollider * const collider = colobj.GetOwnerCollider();
				
				/*
				if(collider->IsVolume()){
					const debpColliderVolume &colvol = *collider->CastToVolume();
					printf("processCollision volume collider %p(%f,%f,%f)\n", &colvol, colvol.GetPosition().x,
						colvol.GetPosition().y, colvol.GetPosition().z);
				}else if(collider->IsComponent()){
					const debpColliderComponent &colcomp = *collider->CastToComponent();
					printf("processCollision component collider %p(%f,%f,%f) mode=%i rigshapes=%i\n", &colcomp,
						colcomp.GetPosition().x, colcomp.GetPosition().y, colcomp.GetPosition().z,
						colcomp.GetTestMode(), colcomp.GetRigShapes().GetShapeCount());
				}else{
					printf("processCollision rigged collider %p\n", collider->CastToRigged());
				}
				*/
				
				if(pColDet.ColliderHitsCollider(pCollider, collider, pResult)){
					const btCollisionObjectWrapper obj1Wrap(NULL, bpColObj->getCollisionShape(), bpColObj, bpColObj->getWorldTransform(), -1, -1);
					//printf( "collision found: calling addSingleResult %p->%p\n", pObj0Wrapper.getCollisionObject()->getUserPointer(), obj1Wrap.getCollisionObject()->getUserPointer() );
					return pResultCallback.addSingleResult(vDummyManifoldPoint, &pObj0Wrapper, 0, -1, &obj1Wrap, 0, pResult.face) > (btScalar)0.5;
				}
				
			// test against height terrain
			}else if(colobj.IsOwnerHTSector()){
				//printf( "processCollision height terrain sector\n" );
				
				if(pColDet.ColliderHitsHeightTerrain(pCollider, colobj.GetOwnerHTSector(), pResult)){
					const btCollisionObjectWrapper obj1Wrap(NULL, bpColObj->getCollisionShape(), bpColObj, bpColObj->getWorldTransform(), -1, -1);
					//printf( "collision found: calling addSingleResult\n" );
					return pResultCallback.addSingleResult(vDummyManifoldPoint, &pObj0Wrapper, 0, -1, &obj1Wrap, 0, pResult.face) > (btScalar)0.5;
				}
			}
		}
		
		return true;
	}
};



// Struct used in SingleRayTest
/////////////////////////////////

struct LocalInfoAdder2 : public btCollisionWorld::RayResultCallback{
	RayResultCallback *pUserCallback;
	int pI;
	
	LocalInfoAdder2(int i, RayResultCallback *user) : pUserCallback(user), pI(i){
		m_closestHitFraction = pUserCallback->m_closestHitFraction;
	}
	
	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &r, bool b){
		btCollisionWorld::LocalShapeInfo shapeInfo;
		
		shapeInfo.m_shapePart = -1;
		shapeInfo.m_triangleIndex = pI;
		if(!r.m_localShapeInfo){
			r.m_localShapeInfo = &shapeInfo;
		}
		
		const btScalar result = pUserCallback->addSingleResult(r, b);
		m_closestHitFraction = pUserCallback->m_closestHitFraction;
		
		return result;
	}
};



// Struct used by safeConvexSweepTest. Copy of btCollisionWorld::btSingleSweepCallback
// since this one is private and not accessible through header definition. Basically
// the same except replacing the call to the static btCollisionWorld::objectQuerySingle
// call with our own version allowing to replace the collision testing with faster
// versions in certain cases.
struct SingleSweepCallback : public btBroadphaseRayCallback{
	const btTransform &m_convexFromTrans;
	const btTransform &m_convexToTrans;
	btVector3 m_hitNormal;
	debpCollisionWorld &m_world;
	btCollisionWorld::ConvexResultCallback &m_resultCallback;
	const btScalar m_allowedCcdPenetration;
	const btConvexShape *m_castShape;
	//const btVector3 &m_castAabbMin;
	//const btVector3 &m_castAabbMax;
	
	SingleSweepCallback(const btConvexShape *castShape, const btTransform &convexFromTrans,
	const btTransform &convexToTrans, debpCollisionWorld &world,
	btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration /*,
	const btVector3 &castAabbMin, const btVector3 &castAabbMax*/) :
	m_convexFromTrans(convexFromTrans),
	m_convexToTrans(convexToTrans),
	m_world(world),
	m_resultCallback(resultCallback),
	m_allowedCcdPenetration(allowedPenetration),
	m_castShape(castShape){
	//m_castAabbMin( castAabbMin ),
	//m_castAabbMax( castAabbMax ){
		const btVector3 unnormalizedRayDir(m_convexToTrans.getOrigin() - m_convexFromTrans.getOrigin());
		const btVector3 rayDir(unnormalizedRayDir.normalized());
		///what about division by zero? --> just set rayDirection[i] to INF/BT_LARGE_FLOAT
		m_rayDirectionInverse[0] = ((rayDir[0] == (btScalar)0.0) ? (btScalar)BT_LARGE_FLOAT : ((btScalar)1.0 / rayDir[0]));
		m_rayDirectionInverse[1] = ((rayDir[1] == (btScalar)0.0) ? (btScalar)BT_LARGE_FLOAT : ((btScalar)1.0 / rayDir[1]));
		m_rayDirectionInverse[2] = ((rayDir[2] == (btScalar)0.0) ? (btScalar)BT_LARGE_FLOAT : ((btScalar)1.0 / rayDir[2]));
		m_signs[0] = m_rayDirectionInverse[0] < (btScalar)0.0;
		m_signs[1] = m_rayDirectionInverse[1] < (btScalar)0.0;
		m_signs[2] = m_rayDirectionInverse[2] < (btScalar)0.0;
		m_lambda_max = rayDir.dot(unnormalizedRayDir);
	}
	
	virtual bool process(const btBroadphaseProxy *proxy){
		///terminate further convex sweep tests, once the closestHitFraction reached zero
		if(m_resultCallback.m_closestHitFraction == (btScalar)0.0){
			return false;
		}
		
		btCollisionObject * const collisionObject = (btCollisionObject*)proxy->m_clientObject;
		
		//only perform raycast if filterMask matches
		if(m_resultCallback.needsCollision(collisionObject->getBroadphaseHandle())){
			m_world.objectQuerySingle(m_castShape, m_convexFromTrans, m_convexToTrans,
				collisionObject, collisionObject->getCollisionShape(), collisionObject->getWorldTransform(),
				m_resultCallback, m_allowedCcdPenetration);
		}
		
		return true;
	}
};



// Callbacks
//////////////

#if 0
static void cbPreTick(btDynamicsWorld *world, btScalar timeStep){
	// see http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Simulation_Tick_Callbacks
	// for how to apply forces
	((debpCollisionWorld*)world)->TickPreCollision(timeStep);
}

static void cbPostTick(btDynamicsWorld *world, btScalar timeStep){
	((debpCollisionWorld*)world)->TickPostCollision(timeStep);
}
#endif



// Class debpCollisionWorld
/////////////////////////////

// constructor, destructor
////////////////////////////
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

debpCollisionWorld::debpCollisionWorld(debpWorld &world, btDispatcher *dispatcher,
btBroadphaseInterface *pairCache, debpConstraintSolver *constraintSolver,
btCollisionConfiguration *collisionConfiguration, btSoftBodySolver *softBodySolver) :
btSoftRigidDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration, softBodySolver),
pWorld(world),
pDelayedOperation(NULL)
{
	btContactSolverInfo &solverInfo = getSolverInfo();
	
	pDelayedOperation = new debpDelayedOperation(*this);
	
	solverInfo.m_timeStep = world.GetSimulationTimeStep();
	
	// more iterations will allow the solver to converge to the actual solution better.
	// default 10
	// notes: 20 improves. 100 makes the head case work well. 1000 even more close.
	// tail case gets worse but once converged it works well even with less.
	// notes: bullet demos use 50, 100 or even 150
	solverInfo.m_numIterations = 100; //20; // 25 seems a nice value
	
	// global non-contact constraint damping. determines percentage of relative velocity to use.
	// hence 1 is no damping and everything below damps the velocity.
	// default 1.0
	//solverInfo.m_damping = 1.0f;
	
	// used as Baumgarte factor. affects contact penetration repair strength as well acting as the
	// default erp value in constraint solving. 0 equals to no strength.
	// default 0.2
	//solverInfo.m_erp = 0.2f;
	
	// constraint force mixing. applies to constraint solving as default value.
	// default 0.0
	//solverInfo.m_globalCfm = 0.0f;
	
	// use split impulse.
	// default false, -0.02
	//solverInfo.m_splitImpulse = false;
	//solverInfo.m_splitImpulsePenetrationThreshold = -0.02;
	
	// offsets the distance used in contact constraint solving.
	// default 0.0
	//solverInfo.m_linearSlop = 0.0f;
	
	// resting contact lifetime threshold to disable restitution
	// default 2;
	//solverInfo.m_restingContactRestitutionThreshold = 2;
	
	// defined but not used anywhere in the code
	//solverInfo.m_tau = 0.6f;
	//solverInfo.m_friction = 0.3f;
	//solverInfo.m_restitution = 0.0f;
	//solverInfo.m_maxErrorReduction = 20.0f;
	//solverInfo.m_sor = 1.0f;
	//solverInfo.m_erp2 = 0.1f;
	
	// set tick callbacks if required
#if 0
	setInternalTickCallback(cbPreTick, this, true);
	setInternalTickCallback(cbPostTick, this, false);
#endif
	
	// DEBUG TEST
#if 0
	{
	const btTransform tA(btQuaternion(0.000000,-0.707107,-0.000000,0.707107), btVector3(-9.000000,4.000000,15.500000));
	const btTransform tB(btQuaternion(0.468524,-0.502510,0.541482,0.484526), btVector3(-8.706805,4.262969,15.406411));
	const btVector3 he1(0.710000,0.960000,0.460000);
	const btVector3 he2(0.020000,0.020000,0.140000);
	const btTransform tBA(btQuaternion(0.000000,0.000000,0.000000,1.000000), btVector3(0.000000,1.000000,0.000000));
	const btTransform tBB(btQuaternion(0.000000,-0.000000,-0.000000,1.000000), btVector3(0.000000,0.000000,-0.030000));
	btBoxShape box1(he1), box2(he2);
	btCompoundShape cp1, cp2;
	cp1.addChildShape(tBA, &box1);
	cp2.addChildShape(tBB, &box2);
	btCollisionObject coA, coB;
	coA.setWorldTransform(tA);
	coB.setWorldTransform(tB);
	coA.setCollisionShape(&cp1);
	coB.setCollisionShape(&cp2);
// 	debpCollisionObject owner1, owner2;
// 	coA.setUserPointer(&owner1);
// 	coB.setUserPointer(&owner2);
	
	//const bool result = safeContactPairTest(&coA, &coB);
	bool result = false;
	{
		struct sResult: btManifoldResult{
			bool hasContact;
			sResult(const btCollisionObjectWrapper *obj0Wrap, const btCollisionObjectWrapper *obj1Wrap) :
			btManifoldResult(obj0Wrap, obj1Wrap), hasContact(false){}
			virtual void addContactPoint(const btVector3 &normalOnBInWorld, const btVector3 &pointInWorld, btScalar depth){hasContact = true;}
		};
		
		struct sDispatcher : btCollisionDispatcher{
			sDispatcher(btCollisionConfiguration *collisionConfiguration) : btCollisionDispatcher(collisionConfiguration){}
			virtual bool needsCollision(const btCollisionObject *body0, const btCollisionObject *body1){return true;}
			virtual bool needsResponse(const btCollisionObject *body0, const btCollisionObject *body1){return true;}
		};
		
		btCollisionObjectWrapper obA(0, &cp1, &coA, tA, -1, -1);
		btCollisionObjectWrapper obB(0, &cp2, &coB, tB, -1, -1);
		btSoftBodyRigidBodyCollisionConfiguration cconfig;
		sDispatcher dispatcher(&cconfig);
		
		btCollisionAlgorithm * const algorithm = dispatcher.findAlgorithm(&obA, &obB, 0, BT_CLOSEST_POINT_ALGORITHMS);
		sResult r(&obA, &obB);
		
		algorithm->processCollision(&obA, &obB, getDispatchInfo(), &r);
		algorithm->~btCollisionAlgorithm();
		dispatcher.freeCollisionAlgorithm(algorithm);
		result = r.hasContact;
	}
	
	printf("DEBUG-TEST: result=%d\n", result);
	}
#endif
}

debpCollisionWorld::~debpCollisionWorld(){
	if(pDelayedOperation){
		delete pDelayedOperation;
	}
}



// Management
///////////////

void debpCollisionWorld::MarkAllAABBValid(){
	const btCollisionObjectArray &list = getCollisionObjectArray();
	const int count = list.size();
	int i;
	
	for(i=0; i<count; i++){
		((debpCollisionObject*)list.at(i)->getUserPointer())->SetDirtyAABB(false);
	}
}

void debpCollisionWorld::UpdateDirtyAABBs(){
	const btCollisionObjectArray &list = getCollisionObjectArray();
	const int count = list.size();
	int i;
	
	for(i=0; i<count; i++){
		btCollisionObject * const btColObj = list.at(i);
		debpCollisionObject &colObj = *((debpCollisionObject*)btColObj->getUserPointer());
		if(colObj.GetDirtyAABB()){
			colObj.SetDirtyAABB(false);
			updateSingleAabb(btColObj);
		}
	}
}

#ifdef DO_TIMING_2
void debpCollisionWorld::updateAabbs(){
	timer.Reset();
	btCollisionWorld::updateAabbs();
	timerWorldUpdateAabbs += (int)(timer.GetElapsedTime() * 1e6f);
	timerWorldUpdateAabbsCount++;
}
#endif



void debpCollisionWorld::CheckDynamicCollisions(btScalar timeStep){
	debpDebugInformation *debugInfo = NULL;
	if(pWorld.GetBullet().GetDebug().GetEnabled()){
		debugInfo = pWorld.GetBullet().GetDebug().GetDIWorldCheckDynamicCollisions();
		pPerfTimer.Reset();
	}
	
	const btScalar velocityThreshold = (btScalar)pWorld.GetDynamicCollisionVelocityThreshold();
	deCollisionInfo * const colinfo = pWorld.GetCollisionInfo();
	btDispatcher &dispatcher = *getDispatcher();
	const int countManifolds = dispatcher.getNumManifolds();
	int i, j;
	
	for(i=0; i<countManifolds; i++){
		const btPersistentManifold &manifold = *dispatcher.getManifoldByIndexInternal(i);
		const btCollisionObject &body0 = *manifold.getBody0();
		const btCollisionObject &body1 = *manifold.getBody1();
		
		// ghost objects to not trigger a dynamic collision
		if(body0.getInternalType() == btCollisionObject::CO_GHOST_OBJECT
		&&  body1.getInternalType() == btCollisionObject::CO_GHOST_OBJECT){
			continue;
		}
		
		// only if ony body is dynamic. pure kinematic/static collisions are handled elsewhere
		if(body0.isStaticOrKinematicObject() && body1.isStaticOrKinematicObject()){
			continue;
		}
		
		// find the colliders involved in the collision
		const debpCollisionObject &colObj0 = *((debpCollisionObject*)body0.getUserPointer());
		const debpCollisionObject &colObj1 = *((debpCollisionObject*)body1.getUserPointer());
		debpCollider *collider0 = NULL;
		debpCollider *collider1 = NULL;
		
		if(colObj0.IsOwnerCollider()){
			collider0 = colObj0.GetOwnerCollider();
		}
		if(colObj1.IsOwnerCollider()){
			collider1 = colObj1.GetOwnerCollider();
		}
		
		if(!collider0 && !collider1){
			continue; // no colliders
		}
		
		// check points for a collision
		const int countContacts = manifold.getNumContacts();
		if(countContacts == 0){
			continue;
		}
		
		float impulse = 0.0f;
		btVector3 position((btScalar)0, (btScalar)0, (btScalar)0);
		btVector3 normal((btScalar)0, (btScalar)0, (btScalar)0);
		int shape0 = -1;
		int shape1 = -1;
		
		for(j=0; j<countContacts; j++){
			const btManifoldPoint &pt = manifold.getContactPoint(j);
			
// 			pWorld.GetBullet().LogInfoFormat( "AppliedImpulse(%f: %i/%i: %p,%p): impulse=%f distance=%f lifetime=%i",
// 				timeStep, j, countContacts, collider0, collider1, pt.getAppliedImpulse(), pt.getDistance(), pt.getLifeTime() );
			
			// bullets sets 1 as the lifetime of first contact points. in all following frames this
			// lifetime will be larger. an impact test makes only sense for first time contact
			// points. even if the impulse would be larger later on this would be more like pushing
			// harder against an object. this though does not qualify as a collision a game might
			// be interested in
			if(pt.getLifeTime() == 1 && pt.getAppliedImpulse() > impulse){
				impulse = pt.getAppliedImpulse();
				position = pt.getPositionWorldOnB();
				normal = pt.m_normalWorldOnB;
				shape0 = pt.m_index0; // this is wrong. can be child index relative to parent or triangle index
				shape1 = pt.m_index1; // this is wrong. can be child index relative to parent or triangle index
			}
		}
		
		// call collision detection unless no first time impact has been found or the impact is
		// not strong enough. using the first time impact is usually fine enough but to avoid
		// calling the collider collision callback too often the impulse has to surpass a threshold.
		// the basic idea here is that for a collider at rest during the time step the gravity
		// increases the velocity as if the collider the collider is resting on does not exist. the
		// constraints apply then an impulse equal to "gravity.length * timeStep * mass" (p = v*m = a*t*m).
		// colliders at rest ontop of each other have up to this amount of impulse always applied
		// to stay ontop of each other. an impact has to surpass this amount by at least the same
		// amount or better somewhat more
		if(impulse == 0.0f){
			continue;
		}
		
		// this condition is fine since the init value is constant and adding any number of
		// points is going to deviate from it. inequal test is faster than larger than test
		float impulseThreshold = 0.0f;
		if(collider0){
			impulseThreshold = velocityThreshold * collider0->GetCollider().GetMass();
		}
		if(collider1){
			impulseThreshold = decMath::max(impulseThreshold, velocityThreshold * collider1->GetCollider().GetMass());
		}
		
		if(impulse <= impulseThreshold){
			continue;
		}
		
// 		pWorld.GetBullet().LogInfoFormat( "BANG (%p:%i, %p:%i): impulse=%f vt=%f mass=%f", collider0, colObj0.GetOwnerBone(),
// 			collider1, colObj1.GetOwnerBone(), impulse, velocityThreshold,
// 			(collider0 != NULL) ? collider0->GetCollider().GetMass() : collider1->GetCollider().GetMass() );
// 		continue;
		
		// these parameters are not cleared and stay the same for both collision response calls
		colinfo->SetDistance(0.0f); // not supported on dynamic hits
		colinfo->SetNormal(decVector((float)normal.getX(), (float)normal.getY(), (float)normal.getZ()));
		colinfo->SetPosition(decVector((float)position.getX(), (float)position.getY(), (float)position.getZ()));
		colinfo->SetImpulse(impulse);
		
		// call collision response on first collider
		if(collider0){
			deCollider &collider = collider0->GetCollider();
			
			if(colObj1.IsOwnerCollider()){
				colinfo->SetCollider(&colObj1.GetOwnerCollider()->GetCollider(), colObj1.GetOwnerBone(), -1, -1);
				
			}else if(colObj1.IsOwnerHTSector()){
				const debpHTSector &htsector = *colObj1.GetOwnerHTSector();
				colinfo->SetHTSector(htsector.GetHeightTerrain()->GetHeightTerrain(), htsector.GetSector());
			}
			
			colinfo->SetOwnerBone(colObj0.GetOwnerBone());
			colinfo->SetOwnerShape(shape0);
			
			const deCollider::Ref guard(&collider); // script can potentially remove collider
			collider.GetPeerScripting()->CollisionResponse(&collider, colinfo);
		}
		
		// call collision response on second collider if dynamic and not removed by the first collider
		// WARNING the collision objects can potentially change if colliders are removed while in use.
		//         colObj0 and colObj1 have potentially changed and have to be re-acquired. see
		//         debpDelayedOperation::RemoveCollisionObject for the reason why
		const debpCollisionObject &colObj1Safe = *((debpCollisionObject*)body1.getUserPointer());
		if(colObj1Safe.IsOwnerCollider()){
			const debpCollisionObject &colObj0Safe = *((debpCollisionObject*)body0.getUserPointer());
			deCollider &collider = colObj1Safe.GetOwnerCollider()->GetCollider();
			
			if(colObj0Safe.IsOwnerCollider()){
				colinfo->SetCollider(&colObj0Safe.GetOwnerCollider()->GetCollider(), colObj0Safe.GetOwnerBone(), -1, -1);
				
			}else if(colObj0Safe.IsOwnerHTSector()){
				const debpHTSector &htsector = *colObj0Safe.GetOwnerHTSector();
				colinfo->SetHTSector(htsector.GetHeightTerrain()->GetHeightTerrain(), htsector.GetSector());
			}
			
			colinfo->SetOwnerBone(colObj1Safe.GetOwnerBone());
			colinfo->SetOwnerShape(shape1);
			
			const deCollider::Ref guard(&collider); // script can potentially remove collider
			collider.GetPeerScripting()->CollisionResponse(&collider, colinfo);
		}
		
		if(debugInfo){
			debugInfo->IncrementCounter(1);
		}
	}
	
	if(debugInfo){
		debugInfo->IncrementElapsedTime(pPerfTimer.GetElapsedTime());
	}
}



void debpCollisionWorld::safeRayTest(const btVector3 &rayFromWorld, const btVector3 &rayToWorld,
btCollisionWorld::RayResultCallback &resultCallback) const{
	pDelayedOperation->Lock();
	
	try{
		rayTest(rayFromWorld, rayToWorld, resultCallback);
		pDelayedOperation->Unlock();
		
	}catch(const deException &){
		pDelayedOperation->Unlock();
		throw;
	}
}

void debpCollisionWorld::safeConvexSweepTest(const btConvexShape *castShape,
const btTransform &from, const btTransform &to,
btCollisionWorld::ConvexResultCallback &resultCallback,
btScalar allowedCcdPenetration){
	pDelayedOperation->Lock();
	
	try{
		convexSweepTest(castShape, from, to, resultCallback, allowedCcdPenetration);
		pDelayedOperation->Unlock();
		
	}catch(const deException &){
		pDelayedOperation->Unlock();
		throw;
	}
}

void debpCollisionWorld::safeContactTest(btCollisionObject *colObj,
btCollisionWorld::ContactResultCallback &resultCallback){
	pDelayedOperation->Lock();
	
	try{
		contactTest(colObj, resultCallback);
		pDelayedOperation->Unlock();
		
	}catch(const deException &){
		pDelayedOperation->Unlock();
		throw;
	}
}

void debpCollisionWorld::safeContactPairTest(btCollisionObject *colObjA,
btCollisionObject *colObjB, ContactResultCallback& resultCallback){
	pDelayedOperation->Lock();
	
	try{
		contactPairTest(colObjA, colObjB, resultCallback);
		pDelayedOperation->Unlock();
		
	}catch(const deException &){
		pDelayedOperation->Unlock();
		throw;
	}
}



struct sContactResultBoolean : btManifoldResult{
	bool hasContact;
	// btVector3 hackPointInWorld;
	// btScalar hackDepth;
	
	sContactResultBoolean(const btCollisionObjectWrapper *obj0Wrap,
		const btCollisionObjectWrapper *obj1Wrap) :
	btManifoldResult(obj0Wrap, obj1Wrap),
	hasContact(false){
	}
	
	virtual void addContactPoint(const btVector3 &normalOnBInWorld,
	const btVector3 &pointInWorld, btScalar depth){
		hasContact = true;
			// hackPointInWorld = pointInWorld;
			// hackDepth = depth;
	}
};

/*
struct sContactResultBoolean2 : btCollisionWorld::ContactResultCallback {
	bool hasContact;
	
	sContactResultBoolean2() : hasContact(false){
	}
	
	virtual bool needsCollision(btBroadphaseProxy *proxy0) const{
		return true;
	}
	
	virtual btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap,
	int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1){
		hasContact = true;
		return 0;
	}
};
*/

bool debpCollisionWorld::safeContactPairTest(
btCollisionObject *colObjA, btCollisionObject *colObjB){
	// this version is a modification of btCollisionWorld::contactPairTest removing the
	// contact point handling as much as possible to make it faster
	
	pDelayedOperation->Lock();
	
	/*
	const bool doDebug = colObjB->getWorldTransform().getOrigin().distance(btVector3(-8.65451,4.01,15.5307)) < 0.5;
	if(doDebug){
	const btVector3 &p1 = colObjA->getWorldTransform().getOrigin();
	const btVector3 &p2 = colObjB->getWorldTransform().getOrigin();
// 	printf( "ContactTest: (%f,%f,%f) - (%f,%f,%f)\n", p1.getX(), p1.getY(),
// 		p1.getZ(), p2.getX(), p2.getY(), p2.getZ() );
	
	const btCollisionShape &csA = *colObjA->getCollisionShape();
	const btCollisionShape &csB = *colObjB->getCollisionShape();
	if(csA.getShapeType()==COMPOUND_SHAPE_PROXYTYPE && csB.getShapeType()==COMPOUND_SHAPE_PROXYTYPE){
		const btCompoundShape &cpA = (btCompoundShape&)csA;
		const btCompoundShape &cpB = (btCompoundShape&)csB;
		if(cpA.getNumChildShapes()==1 && cpB.getNumChildShapes()==1){
			const btCollisionShape &cpAcs = *cpA.getChildShape(0);
			const btCollisionShape &cpBcs = *cpB.getChildShape(0);
			if(cpAcs.getShapeType()==BOX_SHAPE_PROXYTYPE && cpBcs.getShapeType()==BOX_SHAPE_PROXYTYPE){
				const btQuaternion &qA = colObjA->getWorldTransform().getRotation();
				const btQuaternion &qB = colObjB->getWorldTransform().getRotation();
				
				printf("const btTransform tA(btQuaternion(%f,%f,%f,%f), btVector3(%f,%f,%f));\n",
					qA.getX(), qA.getY(), qA.getZ(), qA.getW(), p1.getX(), p1.getY(), p1.getZ());
				printf("const btTransform tB(btQuaternion(%f,%f,%f,%f), btVector3(%f,%f,%f));\n",
					qB.getX(), qB.getY(), qB.getZ(), qB.getW(), p2.getX(), p2.getY(), p2.getZ());
				
				const btBoxShape &b1 = (btBoxShape&)cpAcs;
				const btBoxShape &b2 = (btBoxShape&)cpBcs;
				
				const btVector3 he1(b1.getHalfExtentsWithoutMargin());
				const btVector3 he2(b2.getHalfExtentsWithoutMargin());
				
				printf("const btVector3 he1(%f,%f,%f);\n", he1.getX(), he1.getY(), he1.getZ());
				printf("const btVector3 he2(%f,%f,%f);\n", he2.getX(), he2.getY(), he2.getZ());
				
				const btTransform &t1 = cpA.getChildTransform(0);
				const btTransform &t2 = cpB.getChildTransform(0);
				const btQuaternion &qT1 = t1.getRotation();
				const btQuaternion &qT2 = t2.getRotation();
				const btVector3 &pT1 = t1.getOrigin();
				const btVector3 &pT2 = t2.getOrigin();
				
				printf("const btTransform tBA(btQuaternion(%f,%f,%f,%f), btVector3(%f,%f,%f));\n",
					qT1.getX(), qT1.getY(), qT1.getZ(), qT1.getW(), pT1.getX(), pT1.getY(), pT1.getZ());
				printf("const btTransform tBB(btQuaternion(%f,%f,%f,%f), btVector3(%f,%f,%f));\n",
					qT2.getX(), qT2.getY(), qT2.getZ(), qT2.getW(), pT2.getX(), pT2.getY(), pT2.getZ());
				
				const bool nc = getDispatcher()->needsCollision(colObjA, colObjB);
				(void)nc;
				printf("needsCollision %d\n", nc);
			}
		}
	}
	}
	*/
	
	/*
	struct sDispatcher : btCollisionDispatcher{
		sDispatcher(btCollisionConfiguration *collisionConfiguration) : btCollisionDispatcher(collisionConfiguration){}
		virtual bool needsCollision(const btCollisionObject *body0, const btCollisionObject *body1){return true;}
		virtual bool needsResponse(const btCollisionObject *body0, const btCollisionObject *body1){return true;}
	};
	btSoftBodyRigidBodyCollisionConfiguration cconfig;
	sDispatcher dispatcher(&cconfig);
	*/
	
	try{
		btCollisionObjectWrapper obA(0, colObjA->getCollisionShape(),
			colObjA, colObjA->getWorldTransform(), -1, -1);
		btCollisionObjectWrapper obB(0, colObjB->getCollisionShape(),
			colObjB, colObjB->getWorldTransform(), -1, -1);
		
		btCollisionAlgorithm * const algorithm = getDispatcher()->findAlgorithm(
			&obA, &obB, 0, BT_CLOSEST_POINT_ALGORITHMS);
		
		if(!algorithm){
			return false;
		}
		
		sContactResultBoolean result(&obA, &obB);
		
		algorithm->processCollision(&obA, &obB, getDispatchInfo(), &result);
		algorithm->~btCollisionAlgorithm();
		getDispatcher()->freeCollisionAlgorithm(algorithm);
		
		pDelayedOperation->Unlock();
		
			// if(result.hasContact) pWorld.GetBullet().LogInfoFormat("BulletBug.safeContactPairTest: (%g,%g,%g) %g",
			// 	result.hackPointInWorld.x(), result.hackPointInWorld.y(), result.hackPointInWorld.z(), result.hackDepth);
		return result.hasContact;
		
	}catch(const deException &){
		pDelayedOperation->Unlock();
		throw;
	}
}



void debpCollisionWorld::convexSweepTest(const btConvexShape* castShape, const btTransform& from,
const btTransform& to, btCollisionWorld::ConvexResultCallback& resultCallback,
btScalar allowedCcdPenetration){
	btVector3 castShapeAabbMin;
	btVector3 castShapeAabbMax;
	
	// Compute AABB that encompasses angular movement. this needs optimization
	btVector3 linVel, angVel;
	btTransformUtil::calculateVelocity(from, to, 1.0f, linVel, angVel);
	const btVector3 zeroLinVel((btScalar)0.0, (btScalar)0.0, (btScalar)0.0);
	const btTransform R(from.getBasis()); // origin=(0,0,0), rotation=from.rotation
	castShape->calculateTemporalAabb(R, zeroLinVel, angVel, 1.0f, castShapeAabbMin, castShapeAabbMax);
	
	// sweep test
	SingleSweepCallback convexCB(castShape, from, to, *this, resultCallback,
		allowedCcdPenetration /*, castShapeAabbMin, castShapeAabbMax*/);
	
	btBroadphaseInterface &broadphase = *((btBroadphaseInterface*)getBroadphase());
	broadphase.rayTest(from.getOrigin(), to.getOrigin(), convexCB, castShapeAabbMin, castShapeAabbMax);
}



#ifdef DO_TIMING_2
int timerObjectQuerySingle = 0; int timerObjectQuerySingleCount = 0; extern int timerShapeCastConvexCount;
#endif

void debpCollisionWorld::objectQuerySingle(const btConvexShape *castShape, const btTransform &convexFromTrans,
const btTransform &convexToTrans, btCollisionObject *collisionObject, const btCollisionShape *collisionShape,
const btTransform &colObjWorldTransform, btCollisionWorld::ConvexResultCallback &resultCallback,
btScalar allowedPenetration){
#ifdef DO_TIMING_2
	//const int oldtimerShapeCastConvexCount = timerShapeCastConvexCount;
	decTimer timer;
	timer.Reset();
#endif
	
	const btCollisionObjectWrapper castWrap(0, collisionShape, collisionObject, colObjWorldTransform, -1, -1);
	pWorld.GetBullet().GetCollisionDetection().GetBulletShapeCollision().ShapeCast(
		castShape, convexFromTrans, convexToTrans, &castWrap, resultCallback, allowedPenetration);
	
#ifdef DO_TIMING_2
	timerObjectQuerySingle += (int)(timer.GetElapsedTime() * 1e6f); timerObjectQuerySingleCount++;
	//pWorld.GetBullet().LogInfoFormat( "objectQuerySingle caused %i shapeCastConvex calls", timerShapeCastConvexCount - oldtimerShapeCastConvexCount );
#endif
}



#if 0
// Straight copy from btCollisionWorld.cpp . Since it's not a global struct we have to copy it
struct SingleSweepCallback : public btBroadphaseRayCallback{
	btTransform pConvexFromTrans;
	btTransform pConvexToTrans;
	const btCollisionWorld *pWorld;
	btCollisionWorld::ConvexResultCallback &pResultCallback;
	btScalar pAllowedCcdPenetration;
	const btConvexShape *pCastShape;
	
	SingleSweepCallback(const btConvexShape *castShape, const btTransform &convexFromTrans,
	const btTransform &convexToTrans, const btCollisionWorld *world,
	btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration) :
	pConvexFromTrans(convexFromTrans),
	pConvexToTrans(convexToTrans),
	pWorld(world),
	pResultCallback(resultCallback),
	pAllowedCcdPenetration(allowedPenetration),
	pCastShape(castShape){
		const btVector3 unnormalizedRayDir = pConvexToTrans.getOrigin() - pConvexFromTrans.getOrigin();
		const btVector3 rayDir = unnormalizedRayDir.normalized();
		///what about division by zero? --> just set rayDirection[i] to INF/BT_LARGE_FLOAT
		m_rayDirectionInverse[0] = rayDir[0] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / rayDir[0];
		m_rayDirectionInverse[1] = rayDir[1] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / rayDir[1];
		m_rayDirectionInverse[2] = rayDir[2] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / rayDir[2];
		m_signs[0] = m_rayDirectionInverse[0] < 0.0;
		m_signs[1] = m_rayDirectionInverse[1] < 0.0;
		m_signs[2] = m_rayDirectionInverse[2] < 0.0;
		
		m_lambda_max = rayDir.dot(unnormalizedRayDir);
	}
	
	virtual bool process(const btBroadphaseProxy *proxy){
		///terminate further convex sweep tests, once the closestHitFraction reached zero
		if(pResultCallback.m_closestHitFraction == btScalar(0.f)){
			return false;
		}
		
		btCollisionObject * const collisionObject = (btCollisionObject*)proxy->m_clientObject;
		
		//only perform raycast if filterMask matches
		if(pResultCallback.needsCollision(collisionObject->getBroadphaseHandle())){
			pWorld->objectQuerySingle(pCastShape, pConvexFromTrans,pConvexToTrans, collisionObject,
				collisionObject->getCollisionShape(), collisionObject->getWorldTransform(),
				pResultCallback, pAllowedCcdPenetration);
		}
		
		return true;
	}
};
#endif



#if 0
void debpCollisionWorld::convexSweepTest(const btConvexShape *castShape, const btTransform &from,
const btTransform &to, btCollisionWorld::ConvexResultCallback &resultCallback,
btScalar allowedCcdPenetration) const{
	btVector3 castShapeAabbMin;
	btVector3 castShapeAabbMax;
	
	btVector3 linVel, angVel;
	btTransformUtil::calculateVelocity(from, to, 1.0f, linVel, angVel);
printf("linVel(%f,%f,%f) angVel(%f,%f,%f)\n", linVel.x(), linVel.y(), linVel.z(), angVel.x()/DEG2RAD, angVel.y()/DEG2RAD, angVel.z()/DEG2RAD);
	//btTransform rottransform;
	//rottransform.setIdentity();
	//rottransform.setRotation( from.getRotation() );
	castShape->calculateTemporalAabb(from/*rottransform*/, linVel, angVel, 1.0f, castShapeAabbMin, castShapeAabbMax);
printf("aabb (%f,%f,%f) (%f,%f,%f)\n", castShapeAabbMin.x(), castShapeAabbMin.y(), castShapeAabbMin.z(),
	castShapeAabbMax.x(), castShapeAabbMax.y(), castShapeAabbMax.z());
	
	SingleSweepCallback convexCB(castShape, from, to, this, resultCallback, allowedCcdPenetration);
	
	m_broadphasePairCache->rayTest(from.getOrigin(), to.getOrigin(), convexCB, castShapeAabbMin, castShapeAabbMax);
}
#endif

void debpCollisionWorld::contactTest(btCollisionObject *colObj, btCollisionWorld::ContactResultCallback &resultCallback){
#define USE_BULLET_COLLISION 1
	
	#ifdef USE_BULLET_COLLISION
		// if this is enabled a hit is reported for every shape in colObj. the used
		// resultCallback has to deal with this problem. debpContactResultCallback
		// for example ignores multiple contact point beyond the first by default
		btCollisionWorld::contactTest(colObj, resultCallback);
		
	#else
		// if this is enabled we get one hit not one for each shape. this is what we want. but
		// this solution fails to detect collisions with cylinders since the dragengine collision
		// tests do not contain code for this case yet
		btVector3 aabbMin, aabbMax;
		colObj->getCollisionShape()->getAabb(colObj->getWorldTransform(), aabbMin, aabbMax);
		
		HitContactCallback contactCB(*this, colObj, resultCallback);
		
		getBroadphase()->aabbTest(aabbMin, aabbMax, contactCB);
	#endif
}



void debpCollisionWorld::solveConstraints(btContactSolverInfo &solverInfo){
	btSoftRigidDynamicsWorld::solveConstraints(solverInfo);
}



// Debug
//////////

int debpCollisionWorld::GetNumNonStaticRigidBodies() const{
	return m_nonStaticRigidBodies.size();
}



// Protected functions
////////////////////////

void debpCollisionWorld::internalSingleStepSimulation(btScalar timeStep){
	pDelayedOperation->Lock();
	
	try{
		btSoftRigidDynamicsWorld::internalSingleStepSimulation(timeStep);
		
		// NOTE For btSoftMultiBodyDynamicsWorld::internalSingleStepSimulation the pre and post tick
		//      callback are called right at the start and end of the function. For btSoftRigidDynamicsWorld
		//      though some additional code is wrapped around outside these callback calls. This
		//      includes handling soft collisions. Might lead to problems in the future. Proper
		//      callbacks would be set with setInternalTickCallback.
		CheckDynamicCollisions(timeStep);
		
		pDelayedOperation->Unlock();
		
	}catch(const deException &){
		pDelayedOperation->Unlock();
		throw;
	}
}
