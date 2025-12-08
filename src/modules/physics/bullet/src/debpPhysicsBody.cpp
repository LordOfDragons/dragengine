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
#include "debpPhysicsBody.h"
#include "debpMotionState.h"
#include "terrain/heightmap/debpHTSector.h"
#include "collider/debpCollider.h"
#include "collider/debpColliderConstraint.h"
#include "collider/debpColliderComponent.h"
#include "world/debpCollisionWorld.h"
#include "world/debpDelayedOperation.h"
#include "world/debpWorld.h"

#include <dragengine/common/exceptions.h>

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"



// Class debpPhysicsBody
//////////////////////////

// Constructor, destructor
////////////////////////////

debpPhysicsBody::debpPhysicsBody() :
pDynWorld(NULL),
pRigidBody(NULL),
pMotionState(NULL),
pShape(NULL),
pShapeSurface(0.0f),

pMass(0.0f),
pResponseType(ertDynamic),
pEnabled(false),
pDirtyMatrix(true),
pHasGravity(false),
pHasLinVelo(false),
pHasAngVelo(false),
pPreventUpdate(false),
pStateChanged(false),

pCcdThreshold(0.001f),
pCcdRadius(0.001f),

pConstraints(NULL),
pConstraintCount(0),
pConstraintSize(0)
{
	pMotionState = new debpMotionState;
}

debpPhysicsBody::~debpPhysicsBody(){
	pCleanUp();
}



// bullet objects
///////////////////

void debpPhysicsBody::SetDynamicsWorld(debpCollisionWorld *dynWorld){
	if(pDynWorld == dynWorld){
		return;
	}
	
	pFreeRigidBody();
	pDynWorld = dynWorld;
	pCreateRigidBody();
}

void debpPhysicsBody::SetShape(debpBulletShape *shape){
	if(pShape == shape){
		return;
	}
	
	if(shape){
		if(pRigidBody){
			pRigidBody->setCollisionShape(shape->GetShape());
			pRigidBody->updateInertiaTensor();
			SetDirtyAABB(true);
		}
		
	}else{
		pFreeRigidBody();
	}
	
	
	pShape = shape;
	pShapeSurface = 0.0f;
	
	pCreateRigidBody();
}

void debpPhysicsBody::SetShapeSurface(float surface){
	pShapeSurface = decMath::max(surface, 0.0f);
}



void debpPhysicsBody::Activate(){
	if(pRigidBody){
		pRigidBody->activate();
	}
}

bool debpPhysicsBody::UpdateFromBody(){
	pStateChanged = false;
	
	if(pRigidBody && pResponseType == ertDynamic && pRigidBody->isActive()){
		const btVector3 &linVelo = pRigidBody->getLinearVelocity();
		const btVector3 &angVelo = pRigidBody->getAngularVelocity();
		const decDVector &position = pMotionState->GetPosition();
		const decQuaternion &orientation = pMotionState->GetOrientation();
		
		pPreventUpdate = true;
		
		SetPosition(position);
		SetOrientation(orientation);
		SetLinearVelocity(decVector(linVelo.getX(), linVelo.getY(), linVelo.getZ()));
		SetAngularVelocity(decVector(angVelo.getX(), angVelo.getY(), angVelo.getZ()));
		
		pPreventUpdate = false;
	}
	
	return pStateChanged;
}

void debpPhysicsBody::CollisionFilteringChanged(){
	// see debpCollisionDispatcher for the reason why this is required
	if(!pRigidBody){
		return;
	}
	
	if(pDynWorld->GetDelayedOperation().IsLocked()){
		pDynWorld->GetDelayedOperation().AddClearFromBroadphase(pRigidBody);
		
	}else{
		pDynWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(
			pRigidBody->getBroadphaseHandle(), pDynWorld->getDispatcher());
	}
}



void debpPhysicsBody::SetPosition(const decDVector &position){
	if(pPosition.IsEqualTo(position)){
		return;
	}
	
	pPosition = position;
	
	if(!pPreventUpdate){
		pMotionState->SetPosition(position);
		
		if(pRigidBody){
			if(!pDynWorld->GetWorld().GetProcessingPhysics()){
				pUpdateTransform();
			}
			
			if(pResponseType == ertKinematic){
				pRigidBody->activate(true);
			}
		}
	}
	
	pStateChanged = true;
	pDirtyMatrix = true;
	SetDirtyAABB(true);
}

void debpPhysicsBody::SetOrientation(const decQuaternion &orientation){
	if(pOrientation.IsEqualTo(orientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(!pPreventUpdate){
		pMotionState->SetOrientation(orientation);
		
		if(pRigidBody){
			if(!pDynWorld->GetWorld().GetProcessingPhysics()){
				pUpdateTransform();
			}
			
			if(pResponseType == ertKinematic){
				pRigidBody->activate(true);
			}
		}
	}
	
	pStateChanged = true;
	pDirtyMatrix = true;
	SetDirtyAABB(true);
}

void debpPhysicsBody::SetLinearVelocity(const decVector &linVelo){
	if(pLinVelo.IsEqualTo(linVelo)){
		return;
	}
	
	pLinVelo = linVelo;
	pStateChanged = true;
	
	pHasLinVelo = pLinVelo.LengthSquared() > 1e-6f;
	
	if(!pPreventUpdate && pRigidBody){
		if(pResponseType == ertDynamic){
			if(pHasLinVelo){
				pRigidBody->activate();
				pRigidBody->setLinearVelocity(btVector3((btScalar)linVelo.x, (btScalar)linVelo.y, (btScalar)linVelo.z));
				
			}else{
				pRigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
			}
			
		}else if(pResponseType == ertKinematic){
			if(pHasLinVelo){
				pRigidBody->activate(true);
			}
		}
	}
}

void debpPhysicsBody::SetAngularVelocity(const decVector &angVelo){
	if(pAngVelo.IsEqualTo(angVelo)){
		return;
	}
	
	pAngVelo = angVelo;
	pStateChanged = true;
	
	pHasAngVelo = pAngVelo.LengthSquared() > 0.0003046174f;
	
	if(!pPreventUpdate && pRigidBody){
		if(pResponseType == ertDynamic){
			if(pHasAngVelo){
				pRigidBody->activate();
				//pRigidBody->setAngularVelocity( pRigidBody->getCenterOfMassTransform() * btVector3( angVelo.x, angVelo.y, angVelo.z ) );
				pRigidBody->setAngularVelocity(btVector3(angVelo.x, angVelo.y, angVelo.z));
				
			}else{
				pRigidBody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
			}
			
		}else if(pResponseType == ertKinematic){
			if(pHasAngVelo){
				pRigidBody->activate(true);
			}
		}
	}
}

void debpPhysicsBody::SetGravity(const decVector &gravity){
	if(pGravity.IsEqualTo(gravity)){
		return;
	}
	
	pGravity = gravity;
	
	pHasGravity = gravity.LengthSquared() > 1e-6f;
	
	if(!pPreventUpdate && pRigidBody && pResponseType == ertDynamic){
		pRigidBody->setGravity(btVector3((btScalar)gravity.x, (btScalar)gravity.y, (btScalar)gravity.z));
	}
}

void debpPhysicsBody::SetMass(float mass){
	if(fabs(pMass - mass) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMass = mass;
	
	if(pRigidBody && pResponseType == ertDynamic){
		btVector3 localInertia(0.0, 0.0, 0.0);
		pShape->GetShape()->calculateLocalInertia(mass, localInertia);
		pRigidBody->setMassProps(mass, localInertia);
		pRigidBody->updateInertiaTensor();
		
		pRigidBody->setGravity(btVector3(pGravity.x, pGravity.y, pGravity.z));
	}
}

void debpPhysicsBody::SetResponseType(eResponseTypes responseType){
	if(responseType < ertStatic || responseType > ertDynamic){
		DETHROW(deeInvalidParam);
	}
	
	if(pResponseType == responseType){
		return;
	}
	
	// the free/create is required since the change in response type has various complex effects
	// in bullet. the delayed operation takes care of proper remving and adding if required
	pFreeRigidBody();
	pResponseType = responseType;
	pCreateRigidBody();
}

void debpPhysicsBody::ResetKinematicInterpolation(){
	if(!pRigidBody /*|| pResponseType == ertDynamic*/){
		return;
	}
	
	pRigidBody->setInterpolationWorldTransform(pRigidBody->getCenterOfMassTransform());
	pRigidBody->setInterpolationLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	pRigidBody->setInterpolationAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
}

void debpPhysicsBody::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pFreeRigidBody();
	pEnabled = enabled;
	pCreateRigidBody();
}

bool debpPhysicsBody::GetIsActive() const{
	if(pRigidBody && pResponseType != ertStatic){
		return pRigidBody->isActive();
	}
	
	return false;
}

const decDMatrix &debpPhysicsBody::GetMatrix(){
	if(pDirtyMatrix){
		pMatrix.SetWorld(pPosition, pOrientation);
		pDirtyMatrix = false;
	}
	
	return pMatrix;
}



void debpPhysicsBody::SetCcdParameters(float threshold, float radius){
	pCcdThreshold = threshold;
	pCcdRadius = radius;
}



void debpPhysicsBody::ApplyGravity(float elapsed){
	SetLinearVelocity(pLinVelo + pGravity * elapsed);
}

void debpPhysicsBody::UpdateAABB(){
	if(pRigidBody){
		pDynWorld->updateSingleAabb(pRigidBody);
	}
}



// Forces
///////////

void debpPhysicsBody::ApplyImpuls(const decVector &impuls){
	if(!pRigidBody || pResponseType != ertDynamic){
		return;
	}
	
	pCreateRigidBody();
	
	pRigidBody->activate(true);
	
	const btVector3 btImpulse((btScalar)impuls.x, (btScalar)impuls.y, (btScalar)impuls.z);
	pRigidBody->applyCentralImpulse(btImpulse);
}

void debpPhysicsBody::ApplyImpulsAt(const decVector &impuls, const decVector &position){
	if(!pRigidBody || pResponseType != ertDynamic){
		return;
	}
	
	pCreateRigidBody();
	
	pRigidBody->activate(true);
	
	const btVector3 btImpulse((btScalar)impuls.x, (btScalar)impuls.y, (btScalar)impuls.z);
	const btVector3 btPosition((btScalar)position.x, (btScalar)position.y, (btScalar)position.z);
	pRigidBody->applyImpulse(btImpulse, btPosition);
}

void debpPhysicsBody::ApplyTorqueImpuls(const decVector &torqueImpuls){
	if(!pRigidBody || pResponseType != ertDynamic){
		return;
	}
	
	pCreateRigidBody();
	
	pRigidBody->activate(true);
	
	const btVector3 btTorqueImpulse((btScalar)torqueImpuls.x, (btScalar)torqueImpuls.y, (btScalar)torqueImpuls.z);
	pRigidBody->applyTorqueImpulse(btTorqueImpulse);
}

void debpPhysicsBody::ApplyForce(const decVector &force){
	if(!pRigidBody || pResponseType != ertDynamic){
		return;
	}
	
	pCreateRigidBody();
	
	pRigidBody->activate(true);
	
	const btVector3 btForce((btScalar)force.x, (btScalar)force.y, (btScalar)force.z);
	pRigidBody->applyCentralForce(btForce);
}

void debpPhysicsBody::ApplyForceAt(const decVector &force, const decVector &position){
	if(!pRigidBody || pResponseType != ertDynamic){
		return;
	}
	
	pCreateRigidBody();
	
	pRigidBody->activate(true);
	
	const btVector3 btForce((btScalar)force.x, (btScalar)force.y, (btScalar)force.z);
	const btVector3 btPosition((btScalar)position.x, (btScalar)position.y, (btScalar)position.z);
	pRigidBody->applyForce(btForce, btPosition);
}

void debpPhysicsBody::ApplyTorque(const decVector &torque){
	if(!pRigidBody || pResponseType != ertDynamic){
		return;
	}
	
	btVector3 btTorque(torque.x, torque.y, torque.z);
	
	pCreateRigidBody();
	
	pRigidBody->activate(true);
	pRigidBody->applyTorque(btTorque);
}



// Linked Constraints
///////////////////////

debpColliderConstraint *debpPhysicsBody::GetConstraintAt(int index) const{
	if(index < 0 || index >= pConstraintCount){
		DETHROW(deeInvalidParam);
	}
	
	return pConstraints[index];
}

int debpPhysicsBody::IndexOfConstraint(debpColliderConstraint *constraint) const{
	if(!constraint){
		DETHROW(deeInvalidParam);
	}
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(constraint == pConstraints[i]){
			return i;
		}
	}
	
	return -1;
}

bool debpPhysicsBody::HasConstraint(debpColliderConstraint *constraint) const{
	if(!constraint){
		DETHROW(deeInvalidParam);
	}
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(constraint == pConstraints[i]){
			return true;
		}
	}
	
	return false;
}

void debpPhysicsBody::AddConstraint(debpColliderConstraint *constraint){
	if(HasConstraint(constraint)){
		DETHROW(deeInvalidParam);
	}
	
	if(pConstraintCount == pConstraintSize){
		int newSize = pConstraintSize * 3 / 2 + 1;
		debpColliderConstraint **newArray = new debpColliderConstraint*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pConstraints){
			memcpy(newArray, pConstraints, sizeof(debpColliderConstraint*) * pConstraintSize);
			delete [] pConstraints;
		}
		pConstraints = newArray;
		pConstraintSize = newSize;
	}
	
	pConstraints[pConstraintCount] = constraint;
	pConstraintCount++;
}

void debpPhysicsBody::RemoveConstraint(debpColliderConstraint *constraint){
	int i, index = IndexOfConstraint(constraint);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	for(i=index+1; i<pConstraintCount; i++){
		pConstraints[i - 1] = pConstraints[i];
	}
	pConstraintCount--;
}



// Private Functions
//////////////////////

void debpPhysicsBody::pCleanUp(){
	pFreeRigidBody();
	
	
	// constraints should not be linked any more at this time. if though this is for
	// some strange reason still the case notify them that we are no more valid. we
	// take some measures to avoid problems in the case some constraint decides to
	// unregister itself during this process
	int currentIndex;
	
	while(pConstraintCount > 0){
		currentIndex = pConstraintCount - 1;
		
		pConstraints[currentIndex]->PhysicsBodyDestroy(this);
		
		if(currentIndex < pConstraintCount){
			pConstraintCount--;
		}
	}
	
	// now we can free the list
	if(pConstraints){
		delete [] pConstraints;
	}
	
	// free the motion state
	if(pMotionState){
		delete pMotionState;
	}
}

void debpPhysicsBody::pCreateRigidBody(){
	if(pRigidBody || !pEnabled || !pDynWorld || !pShape || !pShape->GetShape()){
		return;
	}
	
	SetDirtyAABB(true);
	
	// update the motion state to be sure anything works.
	pMotionState->SetPosition(pPosition);
	pMotionState->SetOrientation(pOrientation);
	
	// create rigid body
	btCollisionShape * const shape = pShape->GetShape();
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	float mass = 0.0f;
	
	if(pResponseType == ertDynamic){
		mass = pMass;
		shape->calculateLocalInertia(mass, localInertia);
	}
	
	btRigidBody::btRigidBodyConstructionInfo cinfo(mass, pMotionState, shape, localInertia);
	
	cinfo.m_linearDamping = 0.05f; /*0.001f, 0.3f*/ // default 0
// 	cinfo.m_linearDamping = 0.0f;
	
	cinfo.m_angularDamping = 0.1f; /*0.01f, 0.3f*/ // default 0
// 	cinfo.m_angularDamping = 0.0f;
	
	cinfo.m_friction = 0.5f; // default 0.5
	
// 	cinfo.m_rollingFriction = 0.1f; // this solves the rolling issue but totally breaks physics
	                                // unless anisotropic rolling friction below is also used
	cinfo.m_rollingFriction = 0.001f;
	//cinfo.m_rollingFriction = 0.0f; // default 0, objects roll away and accelerate
	
// 	cinfo.m_spinningFriction = 0.1f; // default 0
	cinfo.m_spinningFriction = 0.001f; // default 0
	
	cinfo.m_restitution = 0.0f; // default 0
	
	// sleeping thresholds. body goes to sleep if both linear and angular velocity magnitudes
	// are less than threshold for the entire sleep time (2s).
	
	// default is 0.8 m/s . this is quite high and causes objects to stop moving quickly.
	// the value used here is 5cm/s . most probably this value should be adjusted by the
	// size or the mass of the object. larger objects should most probably have larger
	// threshold while smaller objects should have lower. needs more testing. in general
	// damping and friction should make the velocity drop below the threshold to get
	// stable results not using high thresholds.
	cinfo.m_linearSleepingThreshold = 0.05f;
	
	// default is 1 rad/s which is roughly 57 deg/s . this is even more huge than the linear
	// velocity threshold above. using 2 deg/s instead. again size or mass could be used
	// later on to modify this value.
	cinfo.m_angularSleepingThreshold = DEG2RAD * 2.0f;
	
	pRigidBody = new btRigidBody(cinfo);
	
	pRigidBody->setAnisotropicFriction(shape->getAnisotropicRollingFrictionDirection(),
		btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	
	// set parameters
	pRigidBody->setGravity(btVector3(pGravity.x, pGravity.y, pGravity.z));
	pRigidBody->setFlags(pRigidBody->getFlags() | BT_DISABLE_WORLD_GRAVITY);
	
	if(pResponseType == ertStatic){
		pRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT
			| btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
// 		pRigidBody->forceActivationState( 0 ); // make sure the rigid body is in deactivated state
// 		pRigidBody->setDeactivationTime( 0.1f );
		pRigidBody->forceActivationState(ISLAND_SLEEPING); // bullet demo
		pRigidBody->setDeactivationTime(0.0f);
		
	}else if(pResponseType == ertKinematic){
		pRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT
			| btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
// 		pRigidBody->forceActivationState( 0 ); // make sure the rigid body is in deactivated state
// 		pRigidBody->setDeactivationTime( 0.1f );
// 		pRigidBody->forceActivationState( DISABLE_DEACTIVATION ); // bullet demo but set by addRigidBody
		pRigidBody->setDeactivationTime(0.0f);
		
	}else{
		pRigidBody->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
// 		pRigidBody->forceActivationState( ACTIVE_TAG ); // set by addRigidBody
		pRigidBody->setDeactivationTime(0.0f);
	}
	// NOTE actually forceActivationState helps nothing since addRigidBody() resets it
	
	//pRigidBody->setCcdSquareMotionThreshold( pCcdThreshold );
	//pRigidBody->setCcdSweptSphereRadius( pCcdRadius );
	pRigidBody->setUserPointer((debpCollisionObject*)this);
	
	// set the motion parameters not covered by the motion state
	if(pResponseType == ertDynamic){
		//const btTransform &xform = pRigidBody->getCenterOfMassTransform();
		
		pRigidBody->setLinearVelocity(btVector3(pLinVelo.x, pLinVelo.y, pLinVelo.z));
		//pRigidBody->setAngularVelocity( xform.getBasis() * btVector3( pAngVelo.x, pAngVelo.y, pAngVelo.z ) );
		pRigidBody->setAngularVelocity(btVector3(pAngVelo.x, pAngVelo.y, pAngVelo.z));
	}
	
	// add rigid body to dynamics world or add it to the delayed operation if the world is locked
	if(pDynWorld->GetDelayedOperation().IsLocked()){
		pDynWorld->GetDelayedOperation().AddRigidBody(pRigidBody);
		
	}else{
		pDynWorld->addRigidBody(pRigidBody);
	}
	
	// notify all constraints that the rigid body has been created
	int i;
	for(i=0; i<pConstraintCount; i++){
		pConstraints[i]->RigidBodyCreated(this);
	}
}

void debpPhysicsBody::pFreeRigidBody(){
	if(!pRigidBody){
		return;
	}
	
	int c;
	
	// notify all constraints that the rigid body is about to be destroyed
	for(c=0; c<pConstraintCount; c++){
		pConstraints[c]->RigidBodyDestroy(this);
	}
	
	// destroy the rigid body or add it to the delayed operation if the world is locked
	if(pDynWorld->GetDelayedOperation().IsLocked()){
		pDynWorld->GetDelayedOperation().RemoveRigidBody(pRigidBody);
		
	}else{
		pDynWorld->removeRigidBody(pRigidBody);
		delete pRigidBody;
	}
	
	pRigidBody = NULL;
}

void debpPhysicsBody::pUpdateTransform(){
	if(!pRigidBody){
		return;
	}
	
	pRigidBody->setCenterOfMassTransform(btTransform(
		btQuaternion((btScalar)pOrientation.x, (btScalar)pOrientation.y,
			(btScalar)pOrientation.z, (btScalar)pOrientation.w),
		btVector3((btScalar)pPosition.x, (btScalar)pPosition.y, (btScalar)pPosition.z)));
}
