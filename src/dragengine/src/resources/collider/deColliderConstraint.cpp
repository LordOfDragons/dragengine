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
#include "deColliderConstraint.h"
#include "deCollider.h"
#include "../../common/exceptions.h"



// Class deColliderConstraint
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderConstraint::deColliderConstraint() :
pDampingLinear( 1.0f ),
pDampingAngular( 1.0f ),
pDampingSpring( 1.0f ),
pIsRope( false ),
pBreakingThreshold( 0.0f ),
pBone( -1 )
{
	LockAll();
}

deColliderConstraint::deColliderConstraint( const deColliderConstraint &constraint ) :
pTargetCollider( constraint.pTargetCollider ),
pTargetBone( constraint.pTargetBone ),
pPosition1( constraint.pPosition1 ),
pOrientation1( constraint.pOrientation1 ),
pPosition2( constraint.pPosition2 ),
pOrientation2( constraint.pOrientation2 ),
pDampingLinear( constraint.pDampingLinear ),
pDampingAngular( constraint.pDampingAngular ),
pDampingSpring( constraint.pDampingSpring ),
pIsRope( constraint.pIsRope ),
pBreakingThreshold( constraint.pBreakingThreshold ),
pBone( constraint.pBone )
{
	int i;
	for( i=0; i<6; i++ ){
		pDof[ i ] = constraint.pDof[ i ];
	}
}

deColliderConstraint::~deColliderConstraint(){
}



// Management
///////////////

void deColliderConstraint::SetTargetCollider( deCollider *collider ){
	pTargetCollider = collider;
}

void deColliderConstraint::SetTargetBone( const char *bone ){
	pTargetBone = bone;
}



void deColliderConstraint::SetPosition1( const decVector &position ){
	pPosition1 = position;
}

void deColliderConstraint::SetOrientation1( const decQuaternion &orientation ){
	pOrientation1 = orientation;
}

void deColliderConstraint::SetPosition2( const decVector &position ){
	pPosition2 = position;
}

void deColliderConstraint::SetOrientation2( const decQuaternion &orientation ){
	pOrientation2 = orientation;
}



deColliderConstraintDof &deColliderConstraint::GetDof( eDegreesOfFreedom dof ){
	if( dof < edofLinearX || dof > edofAngularZ ){
		DETHROW( deeInvalidParam );
	}
	
	return pDof[ dof ];
}

const deColliderConstraintDof &deColliderConstraint::GetDof( eDegreesOfFreedom dof ) const{
	if( dof < edofLinearX || dof > edofAngularZ ){
		DETHROW( deeInvalidParam );
	}
	
	return pDof[ dof ];
}



void deColliderConstraint::SetLinearDamping( float damping ){
	pDampingLinear = damping;
}

void deColliderConstraint::SetAngularDamping( float damping ){
	pDampingAngular = damping;
}

void deColliderConstraint::SetSpringDamping( float damping ){
	pDampingSpring = damping;
}



void deColliderConstraint::LockAll(){
	pDof[ edofLinearX ].SetLocked( 0.0f );
	pDof[ edofLinearY ].SetLocked( 0.0f );
	pDof[ edofLinearZ ].SetLocked( 0.0f );
	
	pDof[ edofAngularX ].SetLocked( 0.0f );
	pDof[ edofAngularY ].SetLocked( 0.0f );
	pDof[ edofAngularZ ].SetLocked( 0.0f );
}

void deColliderConstraint::SetToBallJoint(){
	pDof[ edofLinearX ].SetLocked( 0.0f );
	pDof[ edofLinearY ].SetLocked( 0.0f );
	pDof[ edofLinearZ ].SetLocked( 0.0f );
	
	pDof[ edofAngularX ].SetFree();
	pDof[ edofAngularY ].SetFree();
	pDof[ edofAngularZ ].SetFree();
}

void deColliderConstraint::SetToHingeJoint( eDegreesOfFreedom dof, float lowerLimit, float upperLimit ){
	if( dof < edofAngularX || dof > edofAngularZ ){
		DETHROW( deeInvalidParam );
	}
	
	LockAll();
	
	pDof[ dof ].SetLimited( lowerLimit, upperLimit );
}

void deColliderConstraint::SetToPistonJoint( eDegreesOfFreedom dof, float lowerLimit, float upperLimit ){
	if( dof < edofLinearX || dof > edofLinearZ ){
		DETHROW( deeInvalidParam );
	}
	
	LockAll();
	
	pDof[ dof ].SetLimited( lowerLimit, upperLimit );
}

void deColliderConstraint::SetSpringLinear1D( eDegreesOfFreedom dof, float stiffness, float damping, float bendStiffness ){
	if( dof < edofLinearX || dof > edofLinearZ ){
		DETHROW( deeInvalidParam );
	}
	
	pDof[ edofLinearX ].SetFree();
	pDof[ edofLinearY ].SetFree();
	pDof[ edofLinearZ ].SetFree();
	
	pDof[ edofAngularX ].SetLocked( 0.0f );
	pDof[ edofAngularY ].SetLocked( 0.0f );
	pDof[ edofAngularZ ].SetLocked( 0.0f );
	
	pDof[ edofLinearX ].SetSpringStiffness( bendStiffness );
	pDof[ edofLinearY ].SetSpringStiffness( bendStiffness );
	pDof[ edofLinearZ ].SetSpringStiffness( bendStiffness );
	
	pDof[ dof ].SetSpringStiffness( stiffness );
}



void deColliderConstraint::SetIsRope( bool isRope ){
	pIsRope = isRope;
}

void deColliderConstraint::SetBreakingThreshold( float impulseThreshold ){
	if( impulseThreshold < 0.0f ){
		impulseThreshold = 0.0f;
	}
	pBreakingThreshold = impulseThreshold;
}

void deColliderConstraint::SetBone( int bone ){
	if( bone < -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pBone = bone;
}



// Operators
//////////////

deColliderConstraint &deColliderConstraint::operator=( const deColliderConstraint &constraint ){
	pTargetCollider = constraint.pTargetCollider;
	pTargetBone = constraint.pTargetBone;
	
	pPosition1 = constraint.pPosition1;
	pOrientation1 = constraint.pOrientation1;
	pPosition2 = constraint.pPosition2;
	pOrientation2 = constraint.pOrientation2;
	
	pDof[ edofLinearX ] = constraint.GetDofLinearX();
	pDof[ edofLinearY ] = constraint.GetDofLinearY();
	pDof[ edofLinearZ ] = constraint.GetDofLinearZ();
	pDof[ edofAngularX ] = constraint.GetDofAngularX();
	pDof[ edofAngularY ] = constraint.GetDofAngularY();
	pDof[ edofAngularZ ] = constraint.GetDofAngularZ();
	
	pDampingLinear = constraint.pDampingLinear;
	pDampingAngular = constraint.pDampingAngular;
	pDampingSpring = constraint.pDampingSpring;
	
	pIsRope = constraint.pIsRope;
	
	pBreakingThreshold = constraint.pBreakingThreshold;
	
	pBone = constraint.pBone;
	
	return *this;
}
