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
class btDynamicsWorld;
class debpPhysicsBody;
class dePhysicsBullet;



/**
 * 
 * Stores information about an collider constraint.
 */
class debpColliderConstraint{
public:
	/** \brief Type of the constraint. */
	enum eConstraintType{
		/** \brief Static. */
		ectStatic,
		
		/** \brief Ball-Socket. */
		ectBallSocket,
		
		/** \brief Hinge. */
		ectHinge,
		
		/** \brief Cone-Twist. */
		ectConeTwist,
		
		/** \brief Slider. */
		ectSlider,
		
		/** \brief Generic. */
		ectGeneric,
		
		/** \brief Generic Spring. */
		ectGenericSpring
	};
	
	
	
private:
	dePhysicsBullet &pBullet;
	deColliderConstraint &pConstraint;
	int pRigBoneConstraintIndex;
	
	btDynamicsWorld *pDynWorld;
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
	/** \brief Create collider constraint. */
	debpColliderConstraint( dePhysicsBullet &bullet, deColliderConstraint &constraint );
	
	/** \brief Clean up collider constraint. */
	~debpColliderConstraint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine constraint. */
	inline deColliderConstraint &GetConstraint() const{ return pConstraint; }
	
	/** \brief Rig bone constraint index or -1 if not set. */
	inline int GetRigBoneConstraintIndex() const{ return pRigBoneConstraintIndex; }
	
	/** \brief Set rig bone constraint index or -1 if not set. */
	void SetRigBoneConstraintIndex( int index );
	
	/** \brief Dynamics world. */
	inline btDynamicsWorld *GetDynamicsWorld() const{ return pDynWorld; }
	
	/** \brief Set dynamics world. */
	void SetDynamicsWorld( btDynamicsWorld *dynWorld );
	
	/** \brief Bullet constraint or \em NULL. */
	inline btTypedConstraint *GetBpConstraint() const{ return pBpConstraint; }
	
	/** \brief First physics body or \em NULL. */
	inline debpPhysicsBody *GetFirstBody() const{ return pPhyBody1; }
	
	/** \brief Set first physics body or \em NULL. */
	void SetFirstBody( debpPhysicsBody *body );
	
	/** \brief Second physics body or \em NULL. */
	inline debpPhysicsBody *GetSecondBody() const{ return pPhyBody2; }
	
	/** \brief Set second physics body or \em NULL. */
	void SetSecondBody( debpPhysicsBody *body );
	
	/** \brief Offset for first physics body. */
	inline const decVector &GetFirstOffset() const{ return pOffset1; }
	
	/** \brief Set offset for first physics body. */
	void SetFirstOffset( const decVector &offset );
	
	/** \brief Offset for second physics body. */
	inline const decVector &GetSecondOffset() const{ return pOffset2; }
	
	/** \brief Set offset for second physics body. */
	void SetSecondOffset( const decVector &offset );
	
	/** \brief Type of constraint. */
	inline eConstraintType GetConstraintType() const{ return pConstraintType; }
	
	/** \brief Constraint can be broken. */
	bool IsBreakable() const;
	
	/** \brief Constraint is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if constraint is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Constraint has broken due to physics simulation. */
	inline bool GetHasBroken() const{ return pHasBroken; }
	
	/** \brief Set if constraint has broken due to physics simulation. */
	void SetHasBroken( bool hasBroken );
	
	/**
	 * \brief Prepare constraint for next detection step.
	 * \details Required to deal with advanced features like joint frictions.
	 */
	void PrepareForStep();
	
	/**
	 * \brief Check if constraint has broken due to physics simulation since the last time.
	 * 
	 * Returns true if the constraint has not been marked broken but is now broken.
	 * After this point false is returned.
	 */
	bool CheckHasBroken();
	
	
	
	/** \brief Notify constraint changed. */
	void ConstraintChanged();
	
	/** \brief Notify rigid body has been created. */
	void RigidBodyCreated( debpPhysicsBody *body );
	
	/** \brief Notify rigid body is about to be destroyed. */
	void RigidBodyDestroy( debpPhysicsBody *body );
	
	/**
	 * \brief Notify physics body is about to be destroyed.
	 * 
	 * This should never happen if we unregistered ourself correctly. If this does
	 * happen we have to invalidate the given body without unregistering ourself first.
	 */
	void PhysicsBodyDestroy( debpPhysicsBody *body );
	/*@}*/
	
	
	
	/** \name Internal use only */
	/*@{*/
	/** \brief Calculate motor velocity for angular joint friction. */
	decVector AngularJointFrictionVelocity( float timeStep ) const;
	
	/** \brief Calculate motor velocity for linear joint friction. */
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
