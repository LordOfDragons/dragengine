/* 
 * Drag[en]gine Game Engine
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deRigConstraint.h"
#include "../../common/exceptions.h"



// Class deRigConstraint
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deRigConstraint::deRigConstraint(){
	pDampingLinear = 1.0f;
	pDampingAngular = 1.0f;
	pDampingSpring = 1.0f;
	
	pIsRope = false;
	pBreakingThreshold = 0.0f;
	pParentBone = -1;
}

deRigConstraint::~deRigConstraint(){
}



// Management
///////////////

void deRigConstraint::SetReferencePosition( const decVector &position ){
	pRefPosition = position;
}

void deRigConstraint::SetReferenceOrientation( const decQuaternion &orientation ){
	pRefOrientation = orientation;
}

void deRigConstraint::SetBoneOffset( const decVector &offset ){
	pBoneOffset = offset;
}



deColliderConstraintDof &deRigConstraint::GetDof( deColliderConstraint::eDegreesOfFreedom dof ){
	if( dof < deColliderConstraint::edofLinearX || dof > deColliderConstraint::edofAngularZ ){
		DETHROW( deeInvalidParam );
	}
	
	return pDof[ dof ];
}

const deColliderConstraintDof &deRigConstraint::GetDof( deColliderConstraint::eDegreesOfFreedom dof ) const{
	if( dof < deColliderConstraint::edofLinearX || dof > deColliderConstraint::edofAngularZ ){
		DETHROW( deeInvalidParam );
	}
	
	return pDof[ dof ];
}



void deRigConstraint::SetLinearDamping( float damping ){
	pDampingLinear = damping;
}

void deRigConstraint::SetAngularDamping( float damping ){
	pDampingAngular = damping;
}

void deRigConstraint::SetSpringDamping( float damping ){
	pDampingSpring = damping;
}



void deRigConstraint::SetIsRope( bool isRope ){
	pIsRope = isRope;
}

void deRigConstraint::SetBreakingThreshold( float impulseThreshold ){
	if( impulseThreshold < 0.0f ){
		impulseThreshold = 0.0f;
	}
	pBreakingThreshold = impulseThreshold;
}

void deRigConstraint::SetParentBone( int bone ){
	if( bone < -1 ){
		DETHROW( deeInvalidParam );
	}
	pParentBone = bone;
}
