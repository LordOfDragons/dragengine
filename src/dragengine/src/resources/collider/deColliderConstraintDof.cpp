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

deColliderConstraintDof::deColliderConstraintDof(const deColliderConstraintDof &dof){
	*this = dof;
}

deColliderConstraintDof::~deColliderConstraintDof(){
}



// Management
///////////////

void deColliderConstraintDof::SetLowerLimit(float lowerLimit){
	pLowerLimit = lowerLimit;
}

void deColliderConstraintDof::SetUpperLimit(float upperLimit){
	pUpperLimit = upperLimit;
}

void deColliderConstraintDof::SetLocked(float value){
	pLowerLimit = value;
	pUpperLimit = value;
}

void deColliderConstraintDof::SetLimited(float lower, float upper){
	pLowerLimit = lower;
	
	if(upper > lower){
		pUpperLimit = upper;
		
	}else{
		pUpperLimit = lower;
	}
}

void deColliderConstraintDof::SetFree(){
	pLowerLimit = 1.0f;
	pUpperLimit = 0.0f;
}



void deColliderConstraintDof::SetStaticFriction(float friction){
	if(friction < 0.0f){
		friction = 0.0;
	}
	
	pStaticFriction = friction;
}

void deColliderConstraintDof::SetKinematicFriction(float friction){
	if(friction < 0.0f){
		friction = 0.0;
	}
	
	pKinematicFriction = friction;
}



void deColliderConstraintDof::SetSpringStiffness(float stiffness){
	pSpringStiffness = stiffness;
}



// Operators
//////////////

deColliderConstraintDof &deColliderConstraintDof::operator=(const deColliderConstraintDof &dof){
	pLowerLimit = dof.pLowerLimit;
	pUpperLimit = dof.pUpperLimit;
	
	pStaticFriction = dof.pStaticFriction;
	pKinematicFriction = dof.pKinematicFriction;
	
	pSpringStiffness = dof.pSpringStiffness;
	
	return *this;
}
