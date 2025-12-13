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

#include "debpColliderConstraint.h"
#include "bpconstraint/debpBPConstraint6Dof.h"
#include "bpconstraint/debpBPConstraint6DofSpring.h"
#include "bpconstraint/debpBPConstraintPoint2Point.h"
#include "bpconstraint/debpBPConstraintHinge.h"
#include "bpconstraint/debpBPConstraintConeTwist.h"
#include "bpconstraint/debpBPConstraintSlider.h"
#include "../debpPhysicsBody.h"
#include "../debpMotionState.h"
#include "../dePhysicsBullet.h"
#include "../debpConfiguration.h"
#include "../world/debpWorld.h"
#include "../world/debpCollisionWorld.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btConeTwistConstraint.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>

#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/common/exceptions.h>



// Class debpColliderConstraint
/////////////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderConstraint::debpColliderConstraint(dePhysicsBullet &bullet, deColliderConstraint &constraint) :
pBullet(bullet),
pConstraint(constraint),
pRigBoneConstraintIndex(-1),

pDynWorld(nullptr),
pBpConstraint(nullptr),
pPhyBody1(nullptr),
pPhyBody2(nullptr),

pConstraintType(ectHinge),
pEnabled(true),
pHasBroken(false)
{
	pDetectConstraintType();
}

debpColliderConstraint::~debpColliderConstraint(){
	pCleanUp();
}



// Management
///////////////

void debpColliderConstraint::SetRigBoneConstraintIndex(int index){
	pRigBoneConstraintIndex = index;
}

void debpColliderConstraint::SetDynamicsWorld(debpCollisionWorld *dynWorld){
	if(pDynWorld == dynWorld){
		return;
	}
	
	pFreeConstraint();
	
	pDynWorld = dynWorld;
	
	pCreateConstraint();
}

void debpColliderConstraint::SetFirstBody(debpPhysicsBody *body){
	if(body != pPhyBody1){
		pFreeConstraint();
		if(pPhyBody1){
			pPhyBody1->RemoveConstraint(this);
		}
		
		pPhyBody1 = body;
		if(body){
			body->AddConstraint(this);
		}
		
		pCreateConstraint();
	}
}

void debpColliderConstraint::SetSecondBody(debpPhysicsBody *body){
	if(body != pPhyBody2){
		pFreeConstraint();
		if(pPhyBody2){
			pPhyBody2->RemoveConstraint(this);
		}
		
		pPhyBody2 = body;
		if(body){
			body->AddConstraint(this);
		}
		
		pCreateConstraint();
	}
}

void debpColliderConstraint::SetFirstOffset(const decVector &offset){
	pOffset1 = offset;
}

void debpColliderConstraint::SetSecondOffset(const decVector &offset){
	pOffset2 = offset;
}

bool debpColliderConstraint::IsBreakable() const{
	return pConstraint.GetBreakingThreshold() > 0.01f;
}

void debpColliderConstraint::SetEnabled(bool enabled){
	if(pEnabled != enabled){
		pFreeConstraint();
		pEnabled = enabled;
		pCreateConstraint();
	}
}

void debpColliderConstraint::SetHasBroken(bool hasBroken){
	pHasBroken = hasBroken;
}

void debpColliderConstraint::PrepareForStep(){
	if(!pBpConstraint){
		return;
	}
	
	debpBPConstraintBase * const base = debpBPConstraintBase::GetBase(pBpConstraint);
	if(base){
		base->PrepareForStep();
	}
}

bool debpColliderConstraint::CheckHasBroken(){
	if(!pHasBroken && pBpConstraint && !pBpConstraint->isEnabled()){
		pHasBroken = true;
		return true;
	}
	return false;
}

bool debpColliderConstraint::RequiresAutoDirty() const{
	return pEnabled && !pHasBroken && pBpConstraint && pPhyBody2 && pPhyBody2->GetIsActive();
}



void debpColliderConstraint::ConstraintChanged(){
	pFreeConstraint();
	pDetectConstraintType();
	pCreateConstraint();
	
	if(pPhyBody1){
		pPhyBody1->Activate();
	}
	if(pPhyBody2){
		pPhyBody2->Activate();
	}
}



void debpColliderConstraint::RigidBodyCreated(debpPhysicsBody *body){
	if(!body){
		DETHROW(deeInvalidParam);
	}
	
	pCreateConstraint();
}

void debpColliderConstraint::RigidBodyDestroy(debpPhysicsBody *body){
	if(!body){
		DETHROW(deeInvalidParam);
	}
	
	pFreeConstraint();
}

void debpColliderConstraint::PhysicsBodyDestroy(debpPhysicsBody *body){
	if(!body){
		DETHROW(deeInvalidParam);
	}
	
	pFreeConstraint();
	
	if(body == pPhyBody1){
		pPhyBody1 = NULL;
		
	}else if(body == pPhyBody2){
		pPhyBody2 = NULL;
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	pCreateConstraint();
}



// Internal use only
//////////////////////

decVector debpColliderConstraint::AngularJointFrictionVelocity(float timeStep) const{
// 	const float timeStep = 1.0f / ( float )info.fps;
// 	const float staticFriction = dof.GetStaticFriction();
// 	const float kinematicFriction = dof.GetKinematicFriction();
	// friction is force [F]. for constraint error we need velocity [m/s].
	// F=am thus a=F/m. With v=at this is v=Ft/m
	//info.m_constraintError[ row ] = ...
	return decVector();
}

decVector debpColliderConstraint::LinearJointFrictionVelocity(float timeStep) const{
	return decVector();
}



// Private functions
//////////////////////

void debpColliderConstraint::pCleanUp(){
	pFreeConstraint();
	
	if(pPhyBody2){
		pPhyBody2->RemoveConstraint(this);
	}
	if(pPhyBody1){
		pPhyBody1->RemoveConstraint(this);
	}
}

void debpColliderConstraint::pDetectConstraintType(){
	const deColliderConstraintDof &dofLinearX = pConstraint.GetDofLinearX();
	const deColliderConstraintDof &dofLinearY = pConstraint.GetDofLinearY();
	const deColliderConstraintDof &dofLinearZ = pConstraint.GetDofLinearZ();
	const deColliderConstraintDof &dofAngularX = pConstraint.GetDofAngularX();
	const deColliderConstraintDof &dofAngularY = pConstraint.GetDofAngularY();
	const deColliderConstraintDof &dofAngularZ = pConstraint.GetDofAngularZ();
	const debpConfiguration &configuration = *pBullet.GetConfiguration();
	
	pTypeInfo.hasAngularSprings = false;
	pTypeInfo.hasLinearSprings = false;
	pTypeInfo.hasAngularJointFriction = false;
	pTypeInfo.hasLinearJointFriction = false;
	pTypeInfo.countAngularLocks = 0;
	pTypeInfo.countLinearLocks = 0;
	
	// determine the number of locked degrees of freedom
	if(dofAngularX.GetUpperLimit() - dofAngularX.GetLowerLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeAngularX = eatLimited;
		
	}else if(dofAngularX.GetLowerLimit() - dofAngularX.GetUpperLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeAngularX = eatFree;
		
	}else{
		pTypeInfo.typeAngularX = eatLocked;
		pTypeInfo.countAngularLocks++;
	}
	
	if(dofAngularY.GetUpperLimit() - dofAngularY.GetLowerLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeAngularY = eatLimited;
		
	}else if(dofAngularY.GetLowerLimit() - dofAngularY.GetUpperLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeAngularY = eatFree;
		
	}else{
		pTypeInfo.typeAngularY = eatLocked;
		pTypeInfo.countAngularLocks++;
	}
	
	if(dofAngularZ.GetUpperLimit() - dofAngularZ.GetLowerLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeAngularZ = eatLimited;
		
	}else if(dofAngularZ.GetLowerLimit() - dofAngularZ.GetUpperLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeAngularZ = eatFree;
		
	}else{
		pTypeInfo.typeAngularZ = eatLocked;
		pTypeInfo.countAngularLocks++;
	}
	
	if(dofLinearX.GetUpperLimit() - dofLinearX.GetLowerLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeLinearX = eatLimited;
		
	}else if(dofLinearX.GetLowerLimit() - dofLinearX.GetUpperLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeLinearX = eatFree;
		
	}else{
		pTypeInfo.typeLinearX = eatLocked;
		pTypeInfo.countLinearLocks++;
	}
	
	if(dofLinearY.GetUpperLimit() - dofLinearY.GetLowerLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeLinearY = eatLimited;
		
	}else if(dofLinearY.GetLowerLimit() - dofLinearY.GetUpperLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeLinearY = eatFree;
		
	}else{
		pTypeInfo.typeLinearY = eatLocked;
		pTypeInfo.countLinearLocks++;
	}
	
	if(dofLinearZ.GetUpperLimit() - dofLinearZ.GetLowerLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeLinearZ = eatLimited;
		
	}else if(dofLinearZ.GetLowerLimit() - dofLinearZ.GetUpperLimit() > FLOAT_SAFE_EPSILON){
		pTypeInfo.typeLinearZ = eatFree;
		
	}else{
		pTypeInfo.typeLinearZ = eatLocked;
		pTypeInfo.countLinearLocks++;
	}
	
	// determine if there are degrees of freedoms with springs
	if(dofLinearX.GetSpringStiffness() > FLOAT_SAFE_EPSILON
	|| dofLinearY.GetSpringStiffness() > FLOAT_SAFE_EPSILON
	|| dofLinearZ.GetSpringStiffness() > FLOAT_SAFE_EPSILON){
		pTypeInfo.hasLinearSprings = true;
	}
	
	if(dofAngularX.GetSpringStiffness() > FLOAT_SAFE_EPSILON
	|| dofAngularY.GetSpringStiffness() > FLOAT_SAFE_EPSILON
	|| dofAngularZ.GetSpringStiffness() > FLOAT_SAFE_EPSILON){
		pTypeInfo.hasAngularSprings = true;
	}
	
	// determine if there are degrees of freedoms with joint friction
	if(dofLinearX.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearY.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearZ.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearX.GetKinematicFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearY.GetKinematicFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearZ.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		pTypeInfo.hasLinearJointFriction = true;
	}
	
	if(dofAngularX.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularY.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularZ.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularX.GetKinematicFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularY.GetKinematicFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularZ.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		pTypeInfo.hasAngularJointFriction = true;
	}
	
	// by default we use a 6-dof constraint
	pConstraintType = ectGeneric;
	
	// any kind of constraint having springs uses the generic spring type
	if(pTypeInfo.hasLinearSprings || pTypeInfo.hasAngularSprings){
		pConstraintType = ectGenericSpring;
		
	// any kind of constraint having joint frictions uses the generic type
	}else if(pTypeInfo.hasLinearJointFriction || pTypeInfo.hasAngularJointFriction){
		pConstraintType = ectGeneric;
		
	// static, ball socket and hinge joint all have 3 linear locks
	}else if(pTypeInfo.countLinearLocks == 3){
		// a static joint has 3 linear and angular locks
		if(pTypeInfo.countAngularLocks == 3){
			pConstraintType = ectStatic;
			
		// a hinge has 3 linear and 2 angular locks
		}else if(pTypeInfo.countAngularLocks == 2){
			if(configuration.GetEnableConstraintHinge()){
				pConstraintType = ectHinge; // bugged since version 2.75! (doors flipping around at 0 point!)
			}
			
		// cone twist joints can be used to simulate 1 angular lock if all others are restricted
		}else if(pTypeInfo.countAngularLocks == 1){
			if(configuration.GetEnableConstraintConeTwist()){
				if(pTypeInfo.typeAngularX == eatLocked && pTypeInfo.typeAngularY != eatFree && pTypeInfo.typeAngularZ != eatFree){
					pConstraintType = ectConeTwist;
					
				}else if(pTypeInfo.typeAngularY == eatLocked && pTypeInfo.typeAngularX != eatFree && pTypeInfo.typeAngularZ != eatFree){
					pConstraintType = ectConeTwist;
					
				}else if(pTypeInfo.typeAngularZ == eatLocked && pTypeInfo.typeAngularX != eatFree && pTypeInfo.typeAngularY != eatFree){
					pConstraintType = ectConeTwist;
				}
			}
			
		// a ball socket or cone twist joint has 3 linear locks and no angular locks
		}else{
			// ball socket requires all angular freedoms to be unrestricted
			if(pTypeInfo.typeAngularX == eatFree && pTypeInfo.typeAngularY == eatFree && pTypeInfo.typeAngularZ == eatFree){
				if(configuration.GetEnableConstraintPoint2Point()){
					pConstraintType = ectBallSocket;
				}
				
			// cone-twist can be used if no angular freedom is unrestricted
			}else{// if(pTypeInfo.typeAngularX == eatLimited && pTypeInfo.typeAngularY == eatLimited && pTypeInfo.typeAngularZ == eatLimited){
				if(configuration.GetEnableConstraintConeTwist()){
					pConstraintType = ectConeTwist;
				}
			}
		}
		
	// two linear locks can be done using a slider. disabled since they are bugged.
	// in the bullet demo it is disabled too so there seems to be something wrong
	// with the slider constraint. in tests the linear limits are volatile and
	// arbitrarily flipped for some axes but not the others. furthermore the
	// angular limit does not work at all.
	/*
	}else if(countLinearLocks == 2){
		// works only with a limited third lock
		if(typeLinearX == LIMITED || typeLinearY == LIMITED || typeLinearZ == LIMITED){
			// slider works if there is at most one not locked angular degree of freedom coincident with the linear axis
			if(countAngularLocks == 3){
				if(configuration->GetEnableConstraintSlider()){
					pConstraintType = ectSlider;
				}
				
			}else if(countAngularLocks == 2){
				if((typeLinearX == LIMITED && typeAngularX != LOCKED)
				|| (typeLinearY == LIMITED && typeAngularY != LOCKED)
				|| (typeLinearZ == LIMITED && typeAngularZ != LOCKED)){
					if(configuration->GetEnableConstraintSlider()){
						pConstraintType = ectSlider;
					}
				}
			}
		}
	*/
	}
	
// 	pConstraintType = ectGeneric;
	
//	pBullet.LogInfoFormat( "constraint %p: ll=%i al=%i type=%i", pConstraint, countLinearLocks, countAngularLocks, pConstraintType );
	//pBullet.LogInfoFormat( "constraint %p: ta=(%i,%i,%i) ll=%i al=%i type=%i", pConstraint, typeAngularX,
	//	typeAngularY, typeAngularZ, countLinearLocks, countAngularLocks, pConstraintType );
}

void debpColliderConstraint::pCreateConstraint(){
// 	pBullet.LogInfoFormat( "pCreateConstraint c=%p bc=%p e=%i dw=%p pb1=%p pb2=%p", pConstraint, pBpConstraint, pEnabled?1:0, pWorld, pPhyBody1, pPhyBody2 );
	if(!pBpConstraint && pEnabled && pDynWorld
	&& pPhyBody1 && pPhyBody1->GetRigidBody()
	&& !(pPhyBody2 && !pPhyBody2->GetRigidBody())){
		if(pConstraintType == ectStatic){
			pCreateStaticConstraint();
			
		}else if(pConstraintType == ectHinge){
			pCreateHingeConstraint();
			
		}else if(pConstraintType == ectBallSocket){
			pCreateBallSocketConstraint();
			
		}else if(pConstraintType == ectConeTwist){
			//pBullet.LogInfoFormat( "Create Cone-Twist Constraint for %p.", pConstraint );
			pCreateConeTwistConstraint();
			
		}else if(pConstraintType == ectSlider){
			pCreateSliderConstraint();
			
		}else if(pConstraintType == ectGenericSpring){
			pCreateGenericSpringConstraint();
			
		}else{
			pCreateGenericConstraint();
		}
		
// 		if( pBpConstraint ){
// 			if( pConstraintType == ectHinge ){
// 				pBpConstraint.setBreakingImpulseThreshold( 40.0 );
// 			}else{
// 				pBpConstraint.setBreakingImpulseThreshold( 150000.0 );
// 			}
// 		}
		
		pDynWorld->addConstraint(pBpConstraint, true);
		
		pHasBroken = false;
	}
}

void debpColliderConstraint::pFreeConstraint(){
	if(pBpConstraint){
		pDynWorld->removeConstraint(pBpConstraint);
		
		delete pBpConstraint;
		pBpConstraint = NULL;
		
		pHasBroken = false;
	}
}

void debpColliderConstraint::pCreateStaticConstraint(){
	//pCreateGenericConstraint();
	
	const decQuaternion &orientation1 = pConstraint.GetOrientation1();
	const decQuaternion &orientation2 = pConstraint.GetOrientation2();
	const decVector position1 = pConstraint.GetPosition1() - pOffset1;
	const decVector position2 = pConstraint.GetPosition2() - pOffset2;
	btFixedConstraint *fixed = NULL;
	
	// create a constraint for two bodies if phy body 2 is not NULL
	if(pPhyBody2){
		fixed = new btFixedConstraint(*pPhyBody1->GetRigidBody(), *pPhyBody2->GetRigidBody(),
			btTransform(
				btQuaternion((btScalar)orientation1.x, (btScalar)orientation1.y,
					(btScalar)orientation1.z, (btScalar)orientation1.w),
				btVector3((btScalar)position1.x, (btScalar)position1.y, (btScalar)position1.z)),
			btTransform(
				btQuaternion((btScalar)orientation2.x, (btScalar)orientation2.y,
					(btScalar)orientation2.z, (btScalar)orientation2.w),
				btVector3((btScalar)position2.x, (btScalar)position2.y, (btScalar)position2.z)));
		
	// otherwise create a single body constraint
	}else{
		fixed = new btFixedConstraint(*pPhyBody1->GetRigidBody(), btTypedConstraint::getFixedBody(),
			btTransform(
				btQuaternion((btScalar)orientation1.x, (btScalar)orientation1.y,
					(btScalar)orientation1.z, (btScalar)orientation1.w),
				btVector3((btScalar)position1.x, (btScalar)position1.y, (btScalar)position1.z)),
			btTransform(
				btQuaternion((btScalar)orientation2.x, (btScalar)orientation2.y,
					(btScalar)orientation2.z, (btScalar)orientation2.w),
				btVector3((btScalar)position2.x, (btScalar)position2.y, (btScalar)position2.z)));
	}
	
	if(pConstraint.GetBreakingThreshold() > 0.001f){
		fixed->setBreakingImpulseThreshold(pConstraint.GetBreakingThreshold());
	}
	
	pBpConstraint = fixed;
}

void debpColliderConstraint::pCreateBallSocketConstraint(){
	decVector position1 = pConstraint.GetPosition1() - pOffset1;
	decVector position2 = pConstraint.GetPosition2() - pOffset2;
	btPoint2PointConstraint *p2p = NULL;
	
	// create a constraint for two bodies if phy body 2 is not NULL
	if(pPhyBody2){
		p2p = new debpBPConstraintPoint2Point(*this,
			*pPhyBody1->GetRigidBody(), *pPhyBody2->GetRigidBody(),
			btVector3(position1.x, position1.y, position1.z),
			btVector3(position2.x, position2.y, position2.z));
		
	// otherwise create a single body constraint
	}else{
		p2p = new debpBPConstraintPoint2Point(*this,
			*pPhyBody1->GetRigidBody(),
			btVector3(position1.x, position1.y, position1.z),
			btVector3(position2.x, position2.y, position2.z));
	}
	
	if(pConstraint.GetBreakingThreshold() > 0.001f){
		p2p->setBreakingImpulseThreshold(pConstraint.GetBreakingThreshold());
	}
	
	pBpConstraint = p2p;
}

void debpColliderConstraint::pCreateHingeConstraint(){
	const deColliderConstraintDof &dofLinearX = pConstraint.GetDofLinearX();
	const deColliderConstraintDof &dofLinearY = pConstraint.GetDofLinearY();
	const deColliderConstraintDof &dofLinearZ = pConstraint.GetDofLinearZ();
	const deColliderConstraintDof &dofAngularX = pConstraint.GetDofAngularX();
	const deColliderConstraintDof &dofAngularY = pConstraint.GetDofAngularY();
	const deColliderConstraintDof &dofAngularZ = pConstraint.GetDofAngularZ();
	decQuaternion orientation1 = pConstraint.GetOrientation1();
	decQuaternion orientation2 = pConstraint.GetOrientation2();
	decVector position1 = pConstraint.GetPosition1() - pOffset1;
	decVector position2 = pConstraint.GetPosition2() - pOffset2;
	btHingeConstraint *hinge = NULL;
	decQuaternion orientation;
	decMatrix axisMatrix;
	float center, swing;
	float lower, upper;
	
	// fetch values
	const decVector linearOffset(dofLinearX.GetLowerLimit(), dofLinearY.GetLowerLimit(), dofLinearZ.GetLowerLimit());
	
	// alter the positions using the locked linear axes
	position1 += decMatrix::CreateFromQuaternion(orientation1) * linearOffset;
	
	// alter the axis matrix if the rotation axis is not the z axis and store the limits
	if(fabsf(dofAngularX.GetUpperLimit() - dofAngularX.GetLowerLimit()) >= FLOAT_SAFE_EPSILON){
		axisMatrix.SetVU(decVector(1.0f, 0.0f, 0.0f), decVector(0.0f, 0.0f, 1.0f));
		lower = dofAngularX.GetLowerLimit();
		upper = dofAngularX.GetUpperLimit();
		
	}else if(fabsf(dofAngularY.GetUpperLimit() - dofAngularY.GetLowerLimit()) >= FLOAT_SAFE_EPSILON){
		axisMatrix.SetVU(decVector(0.0f, 1.0f, 0.0f), decVector(1.0f, 0.0f, 0.0f));
		lower = dofAngularY.GetLowerLimit();
		upper = dofAngularY.GetUpperLimit();
		
	}else{
		lower = dofAngularZ.GetLowerLimit();
		upper = dofAngularZ.GetUpperLimit();
	}
	
	// for limited hinges we need a little correction
	if(lower < upper){
		// determine the center and swing of the hinge and apply a correction matrix for better results.
		// included is also a fix for the different representation of zero points of angles
		center = (lower + upper) * 0.5f;
		swing = (upper - lower) * 0.5f;
		
		// retrieve the orientation from the final matrix
		orientation1 = axisMatrix.ToQuaternion() * orientation1;
		orientation2 = decQuaternion::CreateFromEuler(0.0f, 0.0f, center) * axisMatrix.ToQuaternion() * orientation2;
		
	}else{
		center = 0.0f;
		swing = 0.0f;
		
		// retrieve the orientation from the final matrix
		orientation1 = axisMatrix.ToQuaternion() * orientation1;
		orientation2 = axisMatrix.ToQuaternion() * orientation2;
	}
	
	// create a constraint for two bodies if phy body 2 is not NULL
	if(pPhyBody2){
		hinge = new debpBPConstraintHinge(*pPhyBody1->GetRigidBody(), *pPhyBody2->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
		
	// otherwise create a single body constraint
	}else{
		hinge = new debpBPConstraintHinge(*pPhyBody1->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
	}
	pBpConstraint = hinge;
	
	// set the limits if not free
	if(lower < upper){
		hinge->setLimit(-swing, swing, 1.0f, 0.3f, 1.0f); // 0.9f, 0.3f, 1.0f
		
// 		float cfm = 0.0f; // default 0.0
// 		float erp = 0.8f; // default 0.2
// 		hinge->setParam( BT_CONSTRAINT_CFM, cfm, 5 ); // default 0.0f
// 		hinge->setParam( BT_CONSTRAINT_STOP_CFM, cfm, 5 ); // default 0.0f
// 		hinge->setParam( BT_CONSTRAINT_STOP_ERP, erp, 5 ); // default 0.2f
	}
	
	if(pConstraint.GetBreakingThreshold() > 0.001f){
		hinge->setBreakingImpulseThreshold(pConstraint.GetBreakingThreshold());
	}
}

void debpColliderConstraint::pCreateConeTwistConstraint(){
	const deColliderConstraintDof &dofLinearX = pConstraint.GetDofLinearX();
	const deColliderConstraintDof &dofLinearY = pConstraint.GetDofLinearY();
	const deColliderConstraintDof &dofLinearZ = pConstraint.GetDofLinearZ();
	const deColliderConstraintDof &dofAngularX = pConstraint.GetDofAngularX();
	const deColliderConstraintDof &dofAngularY = pConstraint.GetDofAngularY();
	const deColliderConstraintDof &dofAngularZ = pConstraint.GetDofAngularZ();
	decQuaternion orientation1 = pConstraint.GetOrientation1();
	decQuaternion orientation2 = pConstraint.GetOrientation2();
	decVector position1 = pConstraint.GetPosition1() - pOffset1;
	decVector position2 = pConstraint.GetPosition2() - pOffset2;
	decVector center, swing, aswing, lower, upper;
	btConeTwistConstraint *coneTwist = NULL;
	decQuaternion orientation;
	decMatrix axisMatrix;
	
	// NOTES:
	// setLimit( span1, span2, twist )
	// span1 = Y-Axis
	// span2 = Z-Axis
	// twist = X-Axis (0 if not allowed to twist along x axis)
	// span is half the entire range
	
	// fetch values
	const decVector lowerLimits(dofAngularX.GetLowerLimit(), dofAngularY.GetLowerLimit(), dofAngularZ.GetLowerLimit());
	const decVector upperLimits(dofAngularX.GetUpperLimit(), dofAngularY.GetUpperLimit(), dofAngularZ.GetUpperLimit());
	const decVector linearOffset(dofLinearX.GetLowerLimit(), dofLinearY.GetLowerLimit(), dofLinearZ.GetLowerLimit());
	
	// alter the positions using the locked linear axes
	position1 += decMatrix::CreateFromQuaternion(orientation1) * linearOffset;
	
	// determine the swing spans of all angular degrees of freedom. to prevent
	// potential numerical problems the swing spans are clamped to zero.
	swing = (upperLimits - lowerLimits) * 0.5f;
	if(swing.x < 0.0f){
		swing.x = 0.0f;
	}
	if(swing.y < 0.0f){
		swing.y = 0.0f;
	}
	if(swing.z < 0.0f){
		swing.z = 0.0f;
	}
	center = (upperLimits + lowerLimits) * 0.5f;
	
	// if one of the axes is 0 use it as the twist axis
	if(swing.x < FLOAT_SAFE_EPSILON){
		//axisMatrix.SetIdentity(); // default by construction
		aswing = swing;
		
	}else if(swing.y < FLOAT_SAFE_EPSILON){
		axisMatrix.a11 = 0.0f; axisMatrix.a12 = 0.0f; axisMatrix.a13 = 1.0f;
		axisMatrix.a21 = 1.0f; axisMatrix.a22 = 0.0f; axisMatrix.a23 = 0.0f;
		axisMatrix.a31 = 0.0f; axisMatrix.a32 = 1.0f; axisMatrix.a33 = 0.0f;
		aswing.Set(swing.y, swing.z, swing.x);
		
	}else if(swing.z < FLOAT_SAFE_EPSILON){
		axisMatrix.a11 = 0.0f; axisMatrix.a12 = 1.0f; axisMatrix.a13 = 0.0f;
		axisMatrix.a21 = 0.0f; axisMatrix.a22 = 0.0f; axisMatrix.a23 = 1.0f;
		axisMatrix.a31 = 1.0f; axisMatrix.a32 = 0.0f; axisMatrix.a33 = 0.0f;
		aswing.Set(swing.z, swing.x, swing.y);
		
	}else{
		// if no axis is 0 use the axis with the largest swing span as the twist axis. if more than one swing span
		// is the smallest x wins over y and y over z. using the largest swing span as the twist axis is more stable
		if(swing.x > swing.y && swing.x > swing.z){ // x span is largest
			//axisMatrix.SetIdentity(); // default by construction
			aswing = swing;
			
		}else if(swing.y > swing.z){ // y span is largest
			axisMatrix.a11 = 0.0f; axisMatrix.a12 = 0.0f; axisMatrix.a13 = 1.0f;
			axisMatrix.a21 = 1.0f; axisMatrix.a22 = 0.0f; axisMatrix.a23 = 0.0f;
			axisMatrix.a31 = 0.0f; axisMatrix.a32 = 1.0f; axisMatrix.a33 = 0.0f;
			aswing.Set(swing.y, swing.z, swing.x);
			
		}else{ // z span is smallest
			axisMatrix.a11 = 0.0f; axisMatrix.a12 = 1.0f; axisMatrix.a13 = 0.0f;
			axisMatrix.a21 = 0.0f; axisMatrix.a22 = 0.0f; axisMatrix.a23 = 1.0f;
			axisMatrix.a31 = 1.0f; axisMatrix.a32 = 0.0f; axisMatrix.a33 = 0.0f;
			aswing.Set(swing.z, swing.x, swing.y);
		}
	}
	
	// determines the orientation of the bodies. the orientation takes into account
	// the center correction and axis realignment
	orientation1 = axisMatrix.ToQuaternion() * orientation1;
	orientation2 = decQuaternion::CreateFromEuler(center) * axisMatrix.ToQuaternion() * orientation2;
	
	// create a constraint for two bodies if phy body 2 is not NULL
	if(pPhyBody2){
		coneTwist = new debpBPConstraintConeTwist(*pPhyBody1->GetRigidBody(), *pPhyBody2->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
		
	// otherwise create a single body constraint
	}else{
		coneTwist = new debpBPConstraintConeTwist(*pPhyBody1->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
	}
	pBpConstraint = coneTwist;
	
	// set the limits using the realligned swing spans
	/*
	setLimit(), a few notes:
	_softness:
		0->1, recommend ~0.8->1.
		describes % of limits where movement is free.
		beyond this softness %, the limit is gradually enforced until the "hard" (1.0) limit is reached.
	_biasFactor:
		0->1?, recommend 0.3 +/-0.3 or so.
		strength with which constraint resists zeroth order (angular, not angular velocity) limit violation.
	__relaxationFactor:
		0->1, recommend to stay near 1.
		the lower the value, the less the constraint will fight velocities which violate the angular limits.
	*/
	//coneTwist->setLimit( aswing.x, aswing.y, aswing.z, 0.8f, 0.3f, 1.0f ); //_softness=1.0f, _biasFactor=0.3f, _relaxationFactor=1.0f
	coneTwist->setLimit(aswing.z, aswing.y, aswing.x, 1.0f, 0.3f, 1.0f);
	
	if(pConstraint.GetBreakingThreshold() > 0.001f){
		coneTwist->setBreakingImpulseThreshold(pConstraint.GetBreakingThreshold());
	}
	
// 	pBullet.LogInfoFormat( "cone-twist %p", pConstraint );
// 	pBullet.LogInfoFormat( "  swing(%f,%f,%f)", swing.x * RAD2DEG, swing.y * RAD2DEG, swing.z * RAD2DEG );
// 	pBullet.LogInfoFormat( "  center(%f,%f,%f)", center.x, center.y, center.z );
// 	pBullet.LogInfoFormat( "  aswing(%f,%f,%f)", aswing.x * RAD2DEG, aswing.y * RAD2DEG, aswing.z * RAD2DEG );
// 	pBullet.LogInfoFormat( "  axisMatrix:" );
// 	pBullet.LogInfoFormat( "    [%f,%f,%f]", axisMatrix.a11, axisMatrix.a12, axisMatrix.a13 );
// 	pBullet.LogInfoFormat( "    [%f,%f,%f]", axisMatrix.a21, axisMatrix.a22, axisMatrix.a23 );
// 	pBullet.LogInfoFormat( "    [%f,%f,%f]", axisMatrix.a31, axisMatrix.a32, axisMatrix.a33 );
}

void debpColliderConstraint::pCreateSliderConstraint(){
	const deColliderConstraintDof &dofLinearX = pConstraint.GetDofLinearX();
	const deColliderConstraintDof &dofLinearY = pConstraint.GetDofLinearY();
	const deColliderConstraintDof &dofLinearZ = pConstraint.GetDofLinearZ();
	const deColliderConstraintDof &dofAngularX = pConstraint.GetDofAngularX();
	const deColliderConstraintDof &dofAngularY = pConstraint.GetDofAngularY();
	const deColliderConstraintDof &dofAngularZ = pConstraint.GetDofAngularZ();
	const decVector position1 = pConstraint.GetPosition1() - pOffset1;
	const decVector position2 = pConstraint.GetPosition2() - pOffset2;
	decQuaternion orientation1 = pConstraint.GetOrientation1();
	decQuaternion orientation2 = pConstraint.GetOrientation2();
	btSliderConstraint *slider = NULL;
	decMatrix matrix, axisMatrix;
	decQuaternion orientation;
	float angularLower, angularUpper;
	float linearLower, linearUpper;
	
	const decVector linearLowerLimits = decVector(dofLinearX.GetLowerLimit(), dofLinearY.GetLowerLimit(), dofLinearZ.GetLowerLimit());
	const decVector linearUpperLimits = decVector(dofLinearX.GetUpperLimit(), dofLinearY.GetUpperLimit(), dofLinearZ.GetUpperLimit());
	const decVector angularLowerLimits = decVector(dofAngularX.GetLowerLimit(), dofAngularY.GetLowerLimit(), dofAngularZ.GetLowerLimit());
	const decVector angularUpperLimits = decVector(dofAngularX.GetUpperLimit(), dofAngularY.GetUpperLimit(), dofAngularZ.GetUpperLimit());
	
	// alter the axis matrix if the slide axis is not the x axis and store the limits
	if(fabs(linearUpperLimits.z - linearLowerLimits.z) >= 1e-5){
		axisMatrix.SetVU(decVector(1.0f, 0.0f, 0.0f), decVector(0.0f, 1.0f, 0.0f));
		linearLower = linearLowerLimits.z;
		linearUpper = linearUpperLimits.z;
		angularLower = angularLowerLimits.z;
		angularUpper = angularUpperLimits.z;
		
	}else if(fabs(linearUpperLimits.y - linearLowerLimits.y) >= 1e-5){
		axisMatrix.SetVU(decVector(0.0f, 0.0f, 1.0f), decVector(1.0f, 0.0f, 0.0f));
		linearLower = linearLowerLimits.y;
		linearUpper = linearUpperLimits.y;
		angularLower = angularLowerLimits.y;
		angularUpper = angularUpperLimits.y;
		
	}else{
		linearLower = linearLowerLimits.x;
		linearUpper = linearUpperLimits.x;
		angularLower = angularLowerLimits.x;
		angularUpper = angularUpperLimits.x;
	}
	
	// retrieve the orientation from the final matrix
	orientation1 = axisMatrix.ToQuaternion() * orientation1;
	orientation2 = axisMatrix.ToQuaternion() * orientation2;
	
	// create a constraint for two bodies if phy body 2 is not NULL
	if(pPhyBody2){
		slider = new debpBPConstraintSlider(*pPhyBody1->GetRigidBody(), *pPhyBody2->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
		
	// otherwise create a single body constraint
	}else{
		slider = new debpBPConstraintSlider(*pPhyBody1->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
	}
	pBpConstraint = slider;
	
	// set the limits
	slider->setLowerLinLimit(linearLower);
	slider->setUpperLinLimit(linearUpper);
	slider->setLowerAngLimit(angularLower);
	slider->setUpperAngLimit(angularUpper);
	
	if(pConstraint.GetBreakingThreshold() > 0.001f){
		slider->setBreakingImpulseThreshold(pConstraint.GetBreakingThreshold());
	}
	
	//slider->setDampingDirLin(0.005F);
	//slider->setRestitutionLimLin(1.1F);
}

void debpColliderConstraint::pCreateGenericConstraint(){
	const deColliderConstraintDof &dofLinearX = pConstraint.GetDofLinearX();
	const deColliderConstraintDof &dofLinearY = pConstraint.GetDofLinearY();
	const deColliderConstraintDof &dofLinearZ = pConstraint.GetDofLinearZ();
	const deColliderConstraintDof &dofAngularX = pConstraint.GetDofAngularX();
	const deColliderConstraintDof &dofAngularY = pConstraint.GetDofAngularY();
	const deColliderConstraintDof &dofAngularZ = pConstraint.GetDofAngularZ();
	decQuaternion orientation1(pConstraint.GetOrientation1());
	decQuaternion orientation2(pConstraint.GetOrientation2());
	decVector position1(pConstraint.GetPosition1() - pOffset1);
	decVector position2(pConstraint.GetPosition2() - pOffset2);
	debpBPConstraint6Dof *generic6Dof = NULL;
	
	const decVector linearLowerLimits(dofLinearX.GetLowerLimit(), dofLinearY.GetLowerLimit(), dofLinearZ.GetLowerLimit());
	const decVector linearUpperLimits(dofLinearX.GetUpperLimit(), dofLinearY.GetUpperLimit(), dofLinearZ.GetUpperLimit());
	const decVector angularLowerLimits(dofAngularX.GetLowerLimit(), dofAngularY.GetLowerLimit(), dofAngularZ.GetLowerLimit());
	const decVector angularUpperLimits(dofAngularX.GetUpperLimit(), dofAngularY.GetUpperLimit(), dofAngularZ.GetUpperLimit());
	
	// juggle the axes around if the setup matches bad behaving cases
	decVector alinearLowerLimits(linearLowerLimits);
	decVector alinearUpperLimits(linearUpperLimits);
	decVector aangularLowerLimits(angularLowerLimits);
	decVector aangularUpperLimits(angularUpperLimits);
	decMatrix axisMatrix;
	int indexMotorX = 0;
	int indexMotorY = 1;
	int indexMotorZ = 2;
	
	if(pTypeInfo.countAngularLocks == 2){
		// considering angular locks only this constraint behaves like a hinge constraint.
		// if the free axis is the y-axis the constraint is known to explode. If the x-axis
		// or z-axis is the free axis the cosstraint is working well.
		if(pTypeInfo.typeAngularY != eatLocked){
			axisMatrix.a11 = 0.0f; axisMatrix.a12 = 0.0f; axisMatrix.a13 = 1.0f;
			axisMatrix.a21 = 1.0f; axisMatrix.a22 = 0.0f; axisMatrix.a23 = 0.0f;
			axisMatrix.a31 = 0.0f; axisMatrix.a32 = 1.0f; axisMatrix.a33 = 0.0f;
			alinearLowerLimits.Set(linearLowerLimits.y, linearLowerLimits.z, linearLowerLimits.x);
			alinearUpperLimits.Set(linearUpperLimits.y, linearUpperLimits.z, linearUpperLimits.x);
			aangularLowerLimits.Set(angularLowerLimits.y, angularLowerLimits.z, angularLowerLimits.x);
			aangularUpperLimits.Set(angularUpperLimits.y, angularUpperLimits.z, angularUpperLimits.x);
			indexMotorX = 2;
			indexMotorY = 0;
			indexMotorZ = 1;
		}
	}
	
	position1 = axisMatrix * position1;
	position2 = axisMatrix * position2;
	orientation1 = axisMatrix.ToQuaternion() * orientation1;
	orientation2 = axisMatrix.ToQuaternion() * orientation2;
	
	// create a constraint for two bodies if phy body 2 is not NULL
	if(pPhyBody2){
		/*
		pBullet.LogInfoFormat("generic6dof %i: (%g,%g,%g) (%g,%g,%g)\n", pConstraintType, position1.x, position1.y, position1.z, position2.x, position2.y, position2.z);
		const btTransform &rb1 = pPhyBody1->GetRigidBody()->getCenterOfMassTransform();
		const btTransform &rb2 = pPhyBody2->GetRigidBody()->getCenterOfMassTransform();
		pBullet.LogInfoFormat(">> rb1 (%g,%g,%g) (%g,%g,%g,%g)\n", rb1.getOrigin().getX(), rb1.getOrigin().getY(), rb1.getOrigin().getZ(), rb1.getRotation().getX(), rb1.getRotation().getY(), rb1.getRotation().getZ(), rb1.getRotation().getW());
		pBullet.LogInfoFormat(">> rb2 (%g,%g,%g) (%g,%g,%g,%g)\n", rb2.getOrigin().getX(), rb2.getOrigin().getY(), rb2.getOrigin().getZ(), rb2.getRotation().getX(), rb2.getRotation().getY(), rb2.getRotation().getZ(), rb2.getRotation().getW());
		pBullet.LogInfoFormat(">> rp1 (%g,%g,%g)\n", pPhyBody1->GetPosition().x, pPhyBody1->GetPosition().y, pPhyBody1->GetPosition().z);
		pBullet.LogInfoFormat(">> rp2 (%g,%g,%g)\n", pPhyBody2->GetPosition().x, pPhyBody2->GetPosition().y, pPhyBody2->GetPosition().z);
		*/
		generic6Dof = new debpBPConstraint6Dof(*this,
			*pPhyBody1->GetRigidBody(), *pPhyBody2->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
		
	// otherwise create a single body constraint
	}else{
		generic6Dof = new debpBPConstraint6Dof(*this,
			*pPhyBody1->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
	}
	pBpConstraint = generic6Dof;
	
	// set motor indices for proper remapping
	generic6Dof->SetMotorIndices(indexMotorX, indexMotorY, indexMotorZ);
	
	// the limits in the 6dof constraint do not follow the same convention
	// as the limits in the game engine. in the game engine angles are winded
	// the mathematical way hence counter clock wise. in bullet though the
	// non-mathematical winding is used hence clock wise. therefore the angular
	// limits have to be negated to obtain the correct result. furthermore
	// the lower limits become the upper limits this way. the same holds true
	// for the linear locks.
	generic6Dof->setAngularLowerLimit(btVector3(-aangularUpperLimits.x, -aangularUpperLimits.y, -aangularUpperLimits.z));
	generic6Dof->setAngularUpperLimit(btVector3(-aangularLowerLimits.x, -aangularLowerLimits.y, -aangularLowerLimits.z));
	generic6Dof->setLinearLowerLimit(btVector3(-alinearUpperLimits.x, -alinearUpperLimits.y, -alinearUpperLimits.z));
	generic6Dof->setLinearUpperLimit(btVector3(-alinearLowerLimits.x, -alinearLowerLimits.y, -alinearLowerLimits.z));
	
	// NOTE: some angles can cause explosions even if they are simple and not
	// causing any singularities. one solution would be to center the limits
	// so they swing evenly in both directions. this would reduce the potential
	// explosion situations but requires some careful testing as bullet
	// constraints are notoriously ill-behaving with transformed coordinate
	// frames.
	
	// the default settings of the 6dof constraint are rather relaxed producing
	// rubber constraints which can be violated way too much. currently they
	// are set to be very rigid to avoid the rubber-band effect.
	
	// ERP
	//   specifies what proportion of the joint error will be fixed during the next simulation step.
	//   If ERP=0 then no correcting force is applied and the bodies will eventually drift apart as the simulation proceeds.
	//   If ERP=1 then the simulation will attempt to fix all joint error during the next time step. However, setting ERP=1
	//   is not recommended, as the joint error will not be completely fixed due to various internal approximations.
	//   A value of ERP=0.1 to 0.8 is recommended (0.2 is the default).
	// 
	// CFM
	//   If CFM is set to zero (which is the default value), the constraint will be hard. If CFM is set to a positive value,
	//   it will be possible to violate the constraint by "pushing on it" (for example, for contact constraints by forcing
	//   the two contacting objects together). In other words the constraint will be soft, and the softness will increase as
	//   CFM increases. What is actually happening here is that the constraint is allowed to be violated by an amount
	//   proportional to CFM times the restoring force that is needed to enforce the constraint. Note that setting CFM to a
	//   negative value can have undesirable bad effects, such as instability. Don't do it.
	
	// the damping and softness is useless. bullet doesn't use them unless motors are set to produce force.
	// even if the motors are enabled the damping force is clamped against the max motor force
	// which reduces it to 0 all time (because higher max force would cause unwanted effects)
	const bool body1Dynamic = pPhyBody1 && pPhyBody1->GetResponseType() == debpPhysicsBody::ertDynamic;
	const bool body2Dynamic = pPhyBody2 && pPhyBody2->GetResponseType() == debpPhysicsBody::ertDynamic;
	bool requiresJointFeedback = false;
	
	float cfm = 0.0f; // default 0.0
	float erp = 0.2f; //0.2f; // default 0.2
	float linearDamping = pLinearDamping(); // default 1.0
	float linearSoftness = 0.7f; // default 0.7
	float linearRestitution = 0.5f; // default 0.5
	float angularDamping = pAngularDamping(); //1.0f; // default 1.0
	float angularSoftness = 0.5f; // default 0.5
	float angularRestitution = 0.0f; // default 0.0
	
	if(!body1Dynamic && !body2Dynamic){
		erp = 0.8f;
		linearDamping = 1.0f;
		linearSoftness = 1.0f;
		linearRestitution = 1.0f;
	}
	
	btTranslationalLimitMotor &motorLinear = *generic6Dof->getTranslationalLimitMotor();
	int l;
	
	for(l=0; l<3; l++){ // linear limits
		generic6Dof->setParam(BT_CONSTRAINT_CFM, cfm, l); // default 0.0f
		generic6Dof->setParam(BT_CONSTRAINT_STOP_CFM, cfm, l); // default 0.0f
		generic6Dof->setParam(BT_CONSTRAINT_STOP_ERP, erp, l); // default 0.2f
	}
	
	motorLinear.m_damping = linearDamping; // Damping for linear limit: default 1.0
	motorLinear.m_limitSoftness = linearSoftness; // Softness for linear limit: default 0.7
	motorLinear.m_restitution = linearRestitution; // Bounce parameter for linear limit: default 0.5
	
	for(l=0; l<3; l++){ // angular limits
		generic6Dof->setParam(BT_CONSTRAINT_CFM, cfm, 3 + l); // Constraint force mixing factor: default 0.0
		generic6Dof->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 3 + l); // Constraint force mixing factor when joint is at limit: default 0.0
		generic6Dof->setParam(BT_CONSTRAINT_STOP_ERP, erp, 3 + l); // Error tolerance factor when joint is at limit: default 0.2
		
		btRotationalLimitMotor &motorAngular = *generic6Dof->getRotationalLimitMotor(l);
		motorAngular.m_damping = angularDamping; // Damping: default 1.0
		motorAngular.m_limitSoftness = angularSoftness; // Relaxation factor: default 0.5
		motorAngular.m_bounce = angularRestitution; // restitution factor: default 0.0
	}
	
	// damping
	generic6Dof->SetDamping(linearDamping);
	
	// constraint breaking
	if(pConstraint.GetBreakingThreshold() > 0.001f){
		generic6Dof->setBreakingImpulseThreshold(pConstraint.GetBreakingThreshold());
	}
	
	// joint friction
	if(dofLinearX.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearX.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		generic6Dof->SetHasJointFrictionLinear(indexMotorX, true);
		motorLinear.m_enableMotor[indexMotorX] = true;
		//motorLinear.m_maxMotorForce[ indexMotorX ] = SIMD_INFINITY; // disable clamping
		requiresJointFeedback = true;
	}
	
	if(dofLinearY.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearY.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		generic6Dof->SetHasJointFrictionLinear(indexMotorY, true);
		motorLinear.m_enableMotor[indexMotorY] = true;
		//motorLinear.m_maxMotorForce[ indexMotorY ] = SIMD_INFINITY; // disable clamping
		requiresJointFeedback = true;
	}
	
	if(dofLinearZ.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofLinearZ.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		generic6Dof->SetHasJointFrictionLinear(indexMotorZ, true);
		motorLinear.m_enableMotor[indexMotorZ] = true;
		//motorLinear.m_maxMotorForce[ indexMotorZ ] = SIMD_INFINITY; // disable clamping
		requiresJointFeedback = true;
	}
	
	if(dofAngularX.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularX.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		generic6Dof->SetHasJointFrictionAngular(indexMotorX, true);
		generic6Dof->getRotationalLimitMotor(indexMotorX)->m_enableMotor = true;
		//generic6Dof->getRotationalLimitMotor( indexMotorX )->m_maxMotorForce = SIMD_INFINITY; // disable clamping
		requiresJointFeedback = true;
	}
	
	if(dofAngularY.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularY.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		generic6Dof->SetHasJointFrictionAngular(indexMotorY, true);
		generic6Dof->getRotationalLimitMotor(indexMotorY)->m_enableMotor = true;
		//generic6Dof->getRotationalLimitMotor( indexMotorY )->m_maxMotorForce = SIMD_INFINITY; // disable clamping
		requiresJointFeedback = true;
	}
	
	if(dofAngularZ.GetStaticFriction() > FLOAT_SAFE_EPSILON
	|| dofAngularZ.GetKinematicFriction() > FLOAT_SAFE_EPSILON){
		generic6Dof->SetHasJointFrictionAngular(indexMotorZ, true);
		generic6Dof->getRotationalLimitMotor(indexMotorZ)->m_enableMotor = true;
		//generic6Dof->getRotationalLimitMotor( indexMotorZ )->m_maxMotorForce = SIMD_INFINITY; // disable clamping
		requiresJointFeedback = true;
	}
	
	// save limits for special use if required
	generic6Dof->SaveLimits();
	
	// create and assign joint feedback if required
	if(requiresJointFeedback){
		generic6Dof->setJointFeedback(new btJointFeedback);
		memset(generic6Dof->getJointFeedback(), '\0', sizeof(btJointFeedback));
	}
}

void debpColliderConstraint::pCreateGenericSpringConstraint(){
	const deColliderConstraintDof &dofLinearX = pConstraint.GetDofLinearX();
	const deColliderConstraintDof &dofLinearY = pConstraint.GetDofLinearY();
	const deColliderConstraintDof &dofLinearZ = pConstraint.GetDofLinearZ();
	const deColliderConstraintDof &dofAngularX = pConstraint.GetDofAngularX();
	const deColliderConstraintDof &dofAngularY = pConstraint.GetDofAngularY();
	const deColliderConstraintDof &dofAngularZ = pConstraint.GetDofAngularZ();
	decQuaternion orientation1(pConstraint.GetOrientation1());
	decQuaternion orientation2(pConstraint.GetOrientation2());
	decVector position1(pConstraint.GetPosition1() - pOffset1);
	decVector position2(pConstraint.GetPosition2() - pOffset2);
	debpBPConstraint6DofSpring *generic6Dof = NULL;
	float springDamping = pSpringDamping();
	
	const decVector linearLowerLimits = decVector(dofLinearX.GetLowerLimit(), dofLinearY.GetLowerLimit(), dofLinearZ.GetLowerLimit());
	const decVector linearUpperLimits = decVector(dofLinearX.GetUpperLimit(), dofLinearY.GetUpperLimit(), dofLinearZ.GetUpperLimit());
	const decVector angularLowerLimits = decVector(dofAngularX.GetLowerLimit(), dofAngularY.GetLowerLimit(), dofAngularZ.GetLowerLimit());
	const decVector angularUpperLimits = decVector(dofAngularX.GetUpperLimit(), dofAngularY.GetUpperLimit(), dofAngularZ.GetUpperLimit());
	const decVector linearSpringStiffness = decVector(dofLinearX.GetSpringStiffness(), dofLinearY.GetSpringStiffness(), dofLinearZ.GetSpringStiffness());
	const decVector angularSpringStiffness = decVector(dofAngularX.GetSpringStiffness(), dofAngularY.GetSpringStiffness(), dofAngularZ.GetSpringStiffness());
	
	// juggle the axes around if the setup matches bad behaving cases
	decVector alinearLowerLimits(linearLowerLimits);
	decVector alinearUpperLimits(linearUpperLimits);
	decVector aangularLowerLimits(angularLowerLimits);
	decVector aangularUpperLimits(angularUpperLimits);
	decVector alinearSpringStiffness(linearSpringStiffness);
	decVector aangularSpringStiffness(angularSpringStiffness);
	decMatrix axisMatrix;
	int indexMotorX = 0;
	int indexMotorY = 1;
	int indexMotorZ = 2;
	
	if(pTypeInfo.countAngularLocks == 2){
		// considering angular locks only this constraint behaves like a hinge constraint.
		// if the free axis is the y-axis the constraint is known to explode. If the x-axis
		// or z-axis is the free axis the cosstraint is working well.
		if(pTypeInfo.typeAngularY != eatLocked){
			axisMatrix.a11 = 0.0f; axisMatrix.a12 = 0.0f; axisMatrix.a13 = 1.0f;
			axisMatrix.a21 = 1.0f; axisMatrix.a22 = 0.0f; axisMatrix.a23 = 0.0f;
			axisMatrix.a31 = 0.0f; axisMatrix.a32 = 1.0f; axisMatrix.a33 = 0.0f;
			alinearLowerLimits.Set(linearLowerLimits.y, linearLowerLimits.z, linearLowerLimits.x);
			alinearUpperLimits.Set(linearUpperLimits.y, linearUpperLimits.z, linearUpperLimits.x);
			aangularLowerLimits.Set(angularLowerLimits.y, angularLowerLimits.z, angularLowerLimits.x);
			aangularUpperLimits.Set(angularUpperLimits.y, angularUpperLimits.z, angularUpperLimits.x);
			alinearSpringStiffness.Set(linearSpringStiffness.y, linearSpringStiffness.z, linearSpringStiffness.x);
			aangularSpringStiffness.Set(angularSpringStiffness.y, angularSpringStiffness.z, angularSpringStiffness.x);
			indexMotorX = 2;
			indexMotorY = 0;
			indexMotorZ = 1;
		}
	}
	
	position1 = axisMatrix * position1;
	position2 = axisMatrix * position2;
	orientation1 = axisMatrix.ToQuaternion() * orientation1;
	orientation2 = axisMatrix.ToQuaternion() * orientation2;
	
	// create a constraint for two bodies if phy body 2 is not NULL
	if(pPhyBody2){
		generic6Dof = new debpBPConstraint6DofSpring(*this,
			*pPhyBody1->GetRigidBody(), *pPhyBody2->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
		
	// otherwise create a single body constraint
	}else{
		generic6Dof = new debpBPConstraint6DofSpring(*this,
			*pPhyBody1->GetRigidBody(),
			btTransform(
				btQuaternion(orientation1.x, orientation1.y, orientation1.z, orientation1.w),
				btVector3(position1.x, position1.y, position1.z)),
			btTransform(
				btQuaternion(orientation2.x, orientation2.y, orientation2.z, orientation2.w),
				btVector3(position2.x, position2.y, position2.z)));
	}
	
	pBpConstraint = generic6Dof;
	
	// set motor indices for proper remapping
	generic6Dof->SetMotorIndices(indexMotorX, indexMotorY, indexMotorZ);
	
	// the limits in the 6dof constraint do not follow the same convention
	// as the limits in the game engine. in the game engine angles are winded
	// the mathematical way hence counter clock wise. in bullet though the
	// non-mathematical winding is used hence clock wise. therefore the angular
	// limits have to be negated to obtain the correct result. furthermore
	// the lower limits become the upper limits this way. the same holds true
	// for the linear locks.
	generic6Dof->setAngularLowerLimit(btVector3(-aangularUpperLimits.x, -aangularUpperLimits.y, -aangularUpperLimits.z));
	generic6Dof->setAngularUpperLimit(btVector3(-aangularLowerLimits.x, -aangularLowerLimits.y, -aangularLowerLimits.z));
	generic6Dof->setLinearLowerLimit(btVector3(-alinearUpperLimits.x, -alinearUpperLimits.y, -alinearUpperLimits.z));
	generic6Dof->setLinearUpperLimit(btVector3(-alinearLowerLimits.x, -alinearLowerLimits.y, -alinearLowerLimits.z));
	
	// NOTE: some angles can cause explosions even if they are simple and not
	// causing any singularities. one solution would be to center the limits
	// so they swing evenly in both directions. this would reduce the potential
	// explosion situations but requires some careful testing as bullet
	// constraints are notoriously ill-behaving with transformed coordinate
	// frames.
	
	// the default settings of the 6dof constraint are rather relaxed producing
	// rubber constraints which can be violated way too much. currently they
	// are set to be very rigid to avoid the rubber-band effect.
	const bool body1Dynamic = pPhyBody1 && pPhyBody1->GetResponseType() == debpPhysicsBody::ertDynamic;
	const bool body2Dynamic = pPhyBody2 && pPhyBody2->GetResponseType() == debpPhysicsBody::ertDynamic;
	
	float cfm = 0.0f; // default 0.0
	float erp = 0.2f; // default 0.2
	float linearDamping = pLinearDamping(); // default 1.0
	float linearSoftness = 1.0f; // default 0.7
	float linearRestitution = 1.0f; // default 0.5
	float angularDamping = pAngularDamping(); // default 1.0
	float angularSoftness = 1.0f; // default 0.5
	float angularRestitution = 0.0f; // default 0.0
	
	if(!body1Dynamic && !body2Dynamic){
		erp = 0.8f;
		linearDamping = 1.0f;
		linearSoftness = 1.0f;
		linearRestitution = 1.0f;
	}
	
	btTranslationalLimitMotor &motorLinear = *generic6Dof->getTranslationalLimitMotor();
	int l;
	
	for(l=0; l<3; l++){ // linear limits
		generic6Dof->setParam(BT_CONSTRAINT_CFM, cfm, l); // default 0.0f
		generic6Dof->setParam(BT_CONSTRAINT_STOP_CFM, cfm, l); // default 0.0f
		generic6Dof->setParam(BT_CONSTRAINT_STOP_ERP, erp, l); // default 0.2f
	}
	
	motorLinear.m_damping = linearDamping; // Damping for linear limit: default 1.0
	motorLinear.m_limitSoftness = linearSoftness; // Softness for linear limit: default 0.7
	motorLinear.m_restitution = linearRestitution; // Bounce parameter for linear limit: default 0.5
	
	for(l=0; l<3; l++){ // angular limits
		generic6Dof->setParam(BT_CONSTRAINT_CFM, cfm, 3 + l); // Constraint force mixing factor: default 0.0
		generic6Dof->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 3 + l); // Constraint force mixing factor when joint is at limit: default 0.0
		generic6Dof->setParam(BT_CONSTRAINT_STOP_ERP, erp, 3 + l); // Error tolerance factor when joint is at limit: default 0.2
		
		btRotationalLimitMotor &motorAngular = *generic6Dof->getRotationalLimitMotor(l);
		motorAngular.m_damping = angularDamping; // Damping: default 1.0
		motorAngular.m_limitSoftness = angularSoftness; // Relaxation factor: default 0.5
		motorAngular.m_bounce = angularRestitution; // restitution factor: default 0.0
	}
	
	// set up the spring parameters for the appropriate degrees of freedom. a degree of freedom
	// has spring enabled if the stiffness is not near zero or below.
	if(alinearSpringStiffness.x > 1e-5f){
		generic6Dof->setStiffness(indexMotorX, alinearSpringStiffness.x);
		generic6Dof->setDamping(indexMotorX, springDamping);
		generic6Dof->enableSpring(indexMotorX, true);
	}
	
	if(alinearSpringStiffness.y > 1e-5f){
		generic6Dof->setStiffness(indexMotorY, alinearSpringStiffness.y);
		generic6Dof->setDamping(indexMotorY, springDamping);
		generic6Dof->enableSpring(indexMotorY, true);
	}
	
	if(alinearSpringStiffness.z > 1e-5f){
		generic6Dof->setStiffness(indexMotorZ, alinearSpringStiffness.z);
		generic6Dof->setDamping(indexMotorZ, springDamping);
		generic6Dof->enableSpring(indexMotorZ, true);
	}
	
	if(aangularSpringStiffness.x > 1e-5f){ // not sure if this is going to work as expected
		generic6Dof->setStiffness(3 + indexMotorX, aangularSpringStiffness.x);
		generic6Dof->setDamping(3 + indexMotorX, springDamping);
		generic6Dof->enableSpring(3 + indexMotorX, true);
	}
	
	if(aangularSpringStiffness.y > 1e-5f){ // not sure if this is going to work as expected
		generic6Dof->setStiffness(3 + indexMotorY, aangularSpringStiffness.y);
		generic6Dof->setDamping(3 + indexMotorY, springDamping);
		generic6Dof->enableSpring(3 + indexMotorY, true);
	}
	
	if(aangularSpringStiffness.z > 1e-5f){ // not sure if this is going to work as expected
		generic6Dof->setStiffness(3 + indexMotorZ, aangularSpringStiffness.z);
		generic6Dof->setDamping(3 + indexMotorZ, springDamping);
		generic6Dof->enableSpring(3 + indexMotorZ, true);
	}
	
	if(pConstraint.GetBreakingThreshold() > 0.001f){
		generic6Dof->setBreakingImpulseThreshold(pConstraint.GetBreakingThreshold());
	}
	
	//generic6Dof->setEquilibriumPoint(); // not sure if this is going to work as expected
}

btScalar debpColliderConstraint::pLinearDamping() const {
	// linear damping coefficient
	return pConstraint.GetLinearDamping();
}

btScalar debpColliderConstraint::pAngularDamping() const {
	// angular damping coefficient
	return pConstraint.GetAngularDamping();
}

btScalar debpColliderConstraint::pSpringDamping() const {
	// spring damping coefficient. bullet calculates (1/timestep)*damping as damping value to
	// use where timestep is 1/60 (60Hz) by default. Hence bullet uses 60*damping. this requires
	// dividing damping by fps
	return pConstraint.GetSpringDamping() * pDynWorld->GetWorld().GetSimulationTimeStep();
}
