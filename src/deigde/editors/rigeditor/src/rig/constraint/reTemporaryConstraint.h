/* 
 * Drag[en]gine IGDE Rig Editor
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

// include only once
#ifndef _RETEMPORARYCONSTRAINT_H_
#define _RETEMPORARYCONSTRAINT_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class reRig;
class reRigBone;
class deColliderComponent;
class deColliderConstraint;
class deEngine;



/**
 * @brief Temporary Constraint.
 *
 * Temporary Constraint.
 */
class reTemporaryConstraint{
private:
	deColliderComponent *pEngSimCollider;
	deColliderConstraint *pEngConstraint;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pLinearLowerLimits;
	decVector pLinearUpperLimits;
	decVector pAngularLowerLimits;
	decVector pAngularUpperLimits;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new temporary constraint. */
	reTemporaryConstraint( reRig *rig, reRigBone *bone, const decDVector &position, const decQuaternion &orientation );
	/** Cleans up the temporary constraint. */
	~reTemporaryConstraint();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the engine constraint. */
	inline deColliderConstraint *GetEngineConstraint() const{ return pEngConstraint; }
	
	/** Retrieves the constraint position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the constraint position. */
	void SetPosition( const decDVector &position );
	/** Retrieves the constraint orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** Sets the constraint orientation. */
	void SetOrientation( const decQuaternion &orientation );
	/** Retrieves the lower linear limits. */
	inline const decVector &GetLinearLowerLimits() const{ return pLinearLowerLimits; }
	/** Sets the lower linear limits. */
	void SetLinearLowerLimits( const decVector &lowerLimits );
	/** Retrieves the upper linear limits. */
	inline const decVector &GetLinearUpperLimits() const{ return pLinearUpperLimits; }
	/** Sets the upper linear limits. */
	void SetLinearUpperLimits( const decVector &upperLimits );
	/** Retrieves the lower angular limits. */
	inline const decVector &GetAngularLowerLimits() const{ return pAngularLowerLimits; }
	/** Sets the lower angular limits. */
	void SetAngularLowerLimits( const decVector &lowerLimits );
	/** Retrieves the upper angular limits. */
	inline const decVector &GetAngularUpperLimits() const{ return pAngularUpperLimits; }
	/** Sets the upper angular limits. */
	void SetAngularUpperLimits( const decVector &upperLimits );
	/*@}*/
	
private:
	void pNotifyConstraintChanged();
};

// end of include only once
#endif
