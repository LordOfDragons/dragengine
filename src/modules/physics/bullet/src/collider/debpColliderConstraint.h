/* 
 * Drag[en]gine Bullet Physics Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEBPCOLLIDERCONSTRAINT_H_
#define _DEBPCOLLIDERCONSTRAINT_H_

#include <dragengine/common/math/decMath.h>

#include "BulletDynamics/ConstraintSolver/btTypedConstraint.h"

class deColliderConstraint;
class deColliderConstraintDof;
class btTypedConstraint;
class debpCollisionWorld;
class debpPhysicsBody;
class dePhysicsBullet;



/**
 * 
 * Stores information about an collider constraint.
 */
class debpColliderConstraint{
public:
	/** Type of the constraint. */
	enum eConstraintType{
		/** Static. */
		ectStatic,
		
		/** Ball-Socket. */
		ectBallSocket,
		
		/** Hinge. */
		ectHinge,
		
		/** Cone-Twist. */
		ectConeTwist,
		
		/** Slider. */
		ectSlider,
		
		/** Generic. */
		ectGeneric,
		
		/** Generic Spring. */
		ectGenericSpring
	};
	
	
	
private:
	dePhysicsBullet &pBullet;
	deColliderConstraint &pConstraint;
	int pRigBoneConstraintIndex;
	
	debpCollisionWorld *pDynWorld;
	btTypedConstraint *pBpConstraint;
	debpPhysicsBody *pPhyBody1;
	debpPhysicsBody *pPhyBody2;
	decVector pOffset1;
	decVector pOffset2;
	
	eConstraintType pConstraintType;
	bool pEnabled;
	bool pHasBroken;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collider constraint. */
	debpColliderConstraint( dePhysicsBullet &bullet, deColliderConstraint &constraint );
	
	/** Clean up collider constraint. */
	~debpColliderConstraint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Engine constraint. */
	inline deColliderConstraint &GetConstraint() const{ return pConstraint; }
	
	/** Rig bone constraint index or -1 if not set. */
	inline int GetRigBoneConstraintIndex() const{ return pRigBoneConstraintIndex; }
	
	/** Set rig bone constraint index or -1 if not set. */
	void SetRigBoneConstraintIndex( int index );
	
	/** Dynamics world. */
	inline debpCollisionWorld *GetDynamicsWorld() const{ return pDynWorld; }
	
	/** Set dynamics world. */
	void SetDynamicsWorld( debpCollisionWorld *dynWorld );
	
	/** Bullet constraint or nullptr. */
	inline btTypedConstraint *GetBpConstraint() const{ return pBpConstraint; }
	
	/** First physics body or nullptr. */
	inline debpPhysicsBody *GetFirstBody() const{ return pPhyBody1; }
	
	/** Set first physics body or nullptr. */
	void SetFirstBody( debpPhysicsBody *body );
	
	/** Second physics body or nullptr. */
	inline debpPhysicsBody *GetSecondBody() const{ return pPhyBody2; }
	
	/** Set second physics body or nullptr. */
	void SetSecondBody( debpPhysicsBody *body );
	
	/** Offset for first physics body. */
	inline const decVector &GetFirstOffset() const{ return pOffset1; }
	
	/** Set offset for first physics body. */
	void SetFirstOffset( const decVector &offset );
	
	/** Offset for second physics body. */
	inline const decVector &GetSecondOffset() const{ return pOffset2; }
	
	/** Set offset for second physics body. */
	void SetSecondOffset( const decVector &offset );
	
	/** Type of constraint. */
	inline eConstraintType GetConstraintType() const{ return pConstraintType; }
	
	/** Constraint can be broken. */
	bool IsBreakable() const;
	
	/** Constraint is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Set if constraint is enabled. */
	void SetEnabled( bool enabled );
	
	/** Constraint has broken due to physics simulation. */
	inline bool GetHasBroken() const{ return pHasBroken; }
	
	/** Set if constraint has broken due to physics simulation. */
	void SetHasBroken( bool hasBroken );
	
	/**
	 * Prepare constraint for next detection step.
	 * \details Required to deal with advanced features like joint frictions.
	 */
	void PrepareForStep();
	
	/**
	 * Check if constraint has broken due to physics simulation since the last time.
	 * 
	 * Returns true if the constraint has not been marked broken but is now broken.
	 * After this point false is returned.
	 */
	bool CheckHasBroken();
	
	
	
	/** Notify constraint changed. */
	void ConstraintChanged();
	
	/** Notify rigid body has been created. */
	void RigidBodyCreated( debpPhysicsBody *body );
	
	/** Notify rigid body is about to be destroyed. */
	void RigidBodyDestroy( debpPhysicsBody *body );
	
	/**
	 * Notify physics body is about to be destroyed.
	 * 
	 * This should never happen if we unregistered ourself correctly. If this does
	 * happen we have to invalidate the given body without unregistering ourself first.
	 */
	void PhysicsBodyDestroy( debpPhysicsBody *body );
	/*@}*/
	
	
	
	/** \name Internal use only */
	/*@{*/
	/** Calculate motor velocity for angular joint friction. */
	decVector AngularJointFrictionVelocity( float timeStep ) const;
	
	/** Calculate motor velocity for linear joint friction. */
	decVector LinearJointFrictionVelocity( float timeStep ) const;
	/*@}*/
	
	
	
private:
	enum eAxisTypes {
		eatLocked,
		eatLimited,
		eatFree
	};
	
	struct sTypeInfo{
		bool hasAngularSprings;
		bool hasLinearSprings;
		bool hasAngularJointFriction;
		bool hasLinearJointFriction;
		eAxisTypes typeLinearX;
		eAxisTypes typeLinearY;
		eAxisTypes typeLinearZ;
		eAxisTypes typeAngularX;
		eAxisTypes typeAngularY;
		eAxisTypes typeAngularZ;
		int countAngularLocks;
		int countLinearLocks;
	} pTypeInfo;
	
	
	
	void pCleanUp();
	
	void pDetectConstraintType();
	
	void pFreeConstraint();
	void pCreateConstraint();
	
	void pCreateStaticConstraint();
	void pCreateBallSocketConstraint();
	void pCreateHingeConstraint();
	void pCreateConeTwistConstraint();
	void pCreateSliderConstraint();
	void pCreateGenericConstraint();
	void pCreateGenericSpringConstraint();
};

#endif
