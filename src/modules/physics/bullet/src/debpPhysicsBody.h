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

#ifndef _DEBPPHYSICSBODY_H_
#define _DEBPPHYSICSBODY_H_

#include "debpCollisionObject.h"

#include <dragengine/common/math/decMath.h>

class debpMotionState;
class debpColliderConstraint;
class debpCollisionWorld;
class debpBulletShape;
class btRigidBody;



/**
 * \brief Bullet physics body.
 *
 * Implements a body in the bullet physics system. This body takes care of creating the
 * bullet physics objects and keeping track of their state as well as changes in those
 * states. Furthermore a reference is kept to the dragengine object owning this physics
 * body. A dragengine object can own more than one physics body.
 */
class debpPhysicsBody : public debpCollisionObject{
public:
	/** \brief Response types. */
	enum eResponseTypes{
		/** \brief Rigid body is static and does never move. */
		ertStatic,
		
		/** \brief Rigid body is driven by kinematics. */
		ertKinematic,
		
		/** \brief Rigid body is driven by dynamics. */
		ertDynamic
	};
	
	
	
private:
	debpCollisionWorld *pDynWorld;
	btRigidBody *pRigidBody;
	debpMotionState *pMotionState;
	debpBulletShape *pShape;
	float pShapeSurface;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pLinVelo;
	decVector pAngVelo;
	decVector pGravity;
	decDMatrix pMatrix;
	float pMass;
	eResponseTypes pResponseType;
	bool pEnabled;
	bool pDirtyMatrix;
	bool pHasGravity;
	bool pHasLinVelo;
	bool pHasAngVelo;
	bool pPreventUpdate;
	bool pStateChanged;
	
	float pCcdThreshold;
	float pCcdRadius;
	
	debpColliderConstraint **pConstraints;
	int pConstraintCount;
	int pConstraintSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create physics body. */
	debpPhysicsBody();
	
	/** \brief Clean up physics body. */
	virtual ~debpPhysicsBody();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet rigid body. */
	inline btRigidBody *GetRigidBody() const{ return pRigidBody; }
	
	/** \brief Dynamic world or \em NULL. */
	inline debpCollisionWorld *GetDynamicsWorld() const{ return pDynWorld; }
	
	/** \brief Dynamic world or \em NULL. */
	void SetDynamicsWorld( debpCollisionWorld *dynWorld );
	
	/** \brief Collision shape or \em NULL. */
	inline debpBulletShape *GetShape() const{ return pShape; }
	
	/** \brief Set collision shape or \em NULL. */
	void SetShape( debpBulletShape *shape );
	
	/** \brief Shape surface. */
	inline float GetShapeSurface() const{ return pShapeSurface; }
	
	/** \brief Set shape surface. */
	void SetShapeSurface( float surface );
	
	
	
	/** \brief Activate body in case it is deactivated. */
	void Activate();
	
	/** \brief Update collider from physics body. */
	bool UpdateFromBody();
	
	/** \brief Collision filter or ignore colliders changed. */
	void CollisionFilteringChanged();
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Linear velocity. */
	inline const decVector &GetLinearVelocity() const{ return pLinVelo; }
	
	/** \brief Set linear velocity. */
	void SetLinearVelocity( const decVector &linVelo );
	
	/** \brief Angular velocity. */
	inline const decVector &GetAngularVelocity() const{ return pAngVelo; }
	
	/** \brief Set anglular velocity. */
	void SetAngularVelocity( const decVector &angVelo );
	
	/** \brief Gravity. */
	inline const decVector &GetGravity() const{ return pGravity; }
	
	/** \brief Set gravity. */
	void SetGravity( const decVector &gravity );
	
	/** \brief Mass. */
	inline float GetMass() const{ return pMass; }
	
	/** \brief Set mass. */
	void SetMass( float mass );
	
	/** \brief Response type. */
	inline eResponseTypes GetResponseType() const{ return pResponseType; }
	
	/** \brief Set response type. */
	void SetResponseType( eResponseTypes responseType );
	
	/** \brief Reset interpolation states if the rigid body is kinematic or static. */
	void ResetKinematicInterpolation();
	
	/** \brief Enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if physics body is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Physics body is active. */
	bool GetIsActive() const;
	
	/** \brief Matrix. */
	const decDMatrix &GetMatrix();
	
	/** \brief State changed. */
	inline bool GetStateChanged() const{ return pStateChanged; }
	
	
	
	/** \brief Ccd threshold. */
	inline float GetCcdThreshold() const{ return pCcdThreshold; }
	
	/** \brief Ccd radius. */
	inline float GetCcdRadius() const{ return pCcdRadius; }
	
	/** \brief Set ccd parameters. */
	void SetCcdParameters( float threshold, float radius );
	
	
	
	/** \brief Apply gravity to linear velocity. */
	void ApplyGravity( float elapsed );
	
	/** \brief Update dynamic world AABB if dirty. */
	void UpdateAABB();
	/*@}*/
	
	
	
	/** \name Force and Impuls */
	/*@{*/
	/** \brief Apply an impuls at the center mass point. */
	void ApplyImpuls( const decVector &impuls );
	
	/** \brief Apply an impuls relative to the body position. */
	void ApplyImpulsAt( const decVector &impuls, const decVector &position );
	
	/** \brief Apply a torque impuls at the center mass point. */
	void ApplyTorqueImpuls( const decVector &torqueImpuls );
	
	/** \brief Apply a force at the center mass point. */
	void ApplyForce( const decVector &force );
	
	/** \brief Apply a force relative to the body position. */
	void ApplyForceAt( const decVector &force, const decVector &position );
	
	/** \brief Apply a torque force at the center mass point. */
	void ApplyTorque( const decVector &torque );
	/*@}*/
	
	
	
	/** \name Linked Constraints */
	/*@{*/
	/** \brief Number of linked constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	
	/** \brief Linked constraint at index. */
	debpColliderConstraint *GetConstraintAt( int index ) const;
	
	/** \brief Index of constraint or -1 if absent. */
	int IndexOfConstraint( debpColliderConstraint *constraint ) const;
	
	/** \brief Constraint is present. */
	bool HasConstraint( debpColliderConstraint *constraint ) const;
	
	/** \brief Add linked constraint. */
	void AddConstraint( debpColliderConstraint *constraint );
	
	/** \brief Remove linked constraint. */
	void RemoveConstraint( debpColliderConstraint *constraint );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateRigidBody();
	void pFreeRigidBody();
	void pUpdateTransform();

};

#endif
