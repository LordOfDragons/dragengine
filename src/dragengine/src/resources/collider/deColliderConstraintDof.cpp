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

#include "deColliderConstraintDof.h"
#include "../../common/exceptions.h"



// Class deColliderConstraintDof
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderConstraintDof::deColliderConstraintDof(){
	pLowerLimit = 0.0f;
	pUpperLimit = 0.0f;
	
	pStaticFriction = 0.0f;
	pKinematicFriction = 0.0f;
	
	pSpringStiffness = 0.0f;
}

deColliderConstraintDof::deColliderConstraintDof( const deColliderConstraintDof &dof ){
	*this = dof;
}

deColliderConstraintDof::~deColliderConstraintDof(){
}



// Management
///////////////

void deColliderConstraintDof::SetLowerLimit( float lowerLimit ){
	pLowerLimit = lowerLimit;
}

void deColliderConstraintDof::SetUpperLimit( float upperLimit ){
	pUpperLimit = upperLimit;
}

void deColliderConstraintDof::SetLocked( float value ){
	pLowerLimit = value;
	pUpperLimit = value;
}

void deColliderConstraintDof::SetLimited( float lower, float upper ){
	pLowerLimit = lower;
	
	if( upper > lower ){
		pUpperLimit = upper;
		
	}else{
		pUpperLimit = lower;
	}
}

void deColliderConstraintDof::SetFree(){
	pLowerLimit = 1.0f;
	pUpperLimit = 0.0f;
}



void deColliderConstraintDof::SetStaticFriction( float friction ){
	if( friction < 0.0f ){
		friction = 0.0;
	}
	
	pStaticFriction = friction;
}

void deColliderConstraintDof::SetKinematicFriction( float friction ){
	if( friction < 0.0f ){
		friction = 0.0;
	}
	
	pKinematicFriction = friction;
}



void deColliderConstraintDof::SetSpringStiffness( float stiffness ){
	pSpringStiffness = stiffness;
}



// Operators
//////////////

deColliderConstraintDof &deColliderConstraintDof::operator=( const deColliderConstraintDof &dof ){
	pLowerLimit = dof.pLowerLimit;
	pUpperLimit = dof.pUpperLimit;
	
	pStaticFriction = dof.pStaticFriction;
	pKinematicFriction = dof.pKinematicFriction;
	
	pSpringStiffness = dof.pSpringStiffness;
	
	return *this;
}
