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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigConstraint.h"
#include "reRigConstraintDof.h"
#include "../reRig.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deColliderComponent.h>



// Class reRigConstraintDof
/////////////////////////////

// Constructor, destructor
////////////////////////////

reRigConstraintDof::reRigConstraintDof( reRigConstraint &constraint,
	deColliderConstraint::eDegreesOfFreedom dofIndex ) :
pRigConstraint( constraint ),
pDofIndex( dofIndex ),
pLowerLimit( 0.0f ),
pUpperLimit( 0.0f ),
pStaticFriction( 0.0f ),
pKinematicFriction( 0.0f ),
pSpringStiffness( 0.0f ){
}

reRigConstraintDof::~reRigConstraintDof(){
}



// Management
///////////////

float reRigConstraintDof::GetThresholdLimits() const{
	switch( pDofIndex ){
	case deColliderConstraint::edofLinearX:
	case deColliderConstraint::edofLinearY:
	case deColliderConstraint::edofLinearZ:
		return FLOAT_SAFE_EPSILON;
		
	case deColliderConstraint::edofAngularX:
	case deColliderConstraint::edofAngularY:
	case deColliderConstraint::edofAngularZ:
	default:
		return 1e-3f;
	}
}

void reRigConstraintDof::SetLowerLimit( float lowerLimit ){
	if( fabsf( lowerLimit - pLowerLimit ) < GetThresholdLimits() ){
		return;
	}
	
	pLowerLimit = lowerLimit;
	
	if( pRigConstraint.GetEngineConstraint() ){
		switch( pDofIndex ){
		case deColliderConstraint::edofLinearX:
		case deColliderConstraint::edofLinearY:
		case deColliderConstraint::edofLinearZ:
			pRigConstraint.GetEngineConstraint()->GetDof( pDofIndex ).SetLowerLimit( lowerLimit );
			break;
			
		case deColliderConstraint::edofAngularX:
		case deColliderConstraint::edofAngularY:
		case deColliderConstraint::edofAngularZ:
			pRigConstraint.GetEngineConstraint()->GetDof( pDofIndex ).SetLowerLimit( lowerLimit * DEG2RAD );
			break;
		}
	}
	
	NotifyEngineConstraintChanged();
	NotifyAllConstraintDofChanged();
}

void reRigConstraintDof::SetUpperLimit( float upperLimit ){
	if( fabsf( upperLimit - pUpperLimit ) < GetThresholdLimits() ){
		return;
	}
	
	pUpperLimit = upperLimit;
	
	if( pRigConstraint.GetEngineConstraint() ){
		switch( pDofIndex ){
		case deColliderConstraint::edofLinearX:
		case deColliderConstraint::edofLinearY:
		case deColliderConstraint::edofLinearZ:
			pRigConstraint.GetEngineConstraint()->GetDof( pDofIndex ).SetUpperLimit( upperLimit );
			break;
			
		case deColliderConstraint::edofAngularX:
		case deColliderConstraint::edofAngularY:
		case deColliderConstraint::edofAngularZ:
		default:
			pRigConstraint.GetEngineConstraint()->GetDof( pDofIndex ).SetUpperLimit( upperLimit * DEG2RAD );
		}
	}
	
	NotifyEngineConstraintChanged();
	NotifyAllConstraintDofChanged();
}



void reRigConstraintDof::SetStaticFriction( float friction ){
	if( friction < 0.0f ){
		friction = 0.0;
	}
	
	if( fabsf( friction - pStaticFriction ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pStaticFriction = friction;
	
	if( pRigConstraint.GetEngineConstraint() ){
		pRigConstraint.GetEngineConstraint()->GetDof( pDofIndex ).SetStaticFriction( friction );
	}
	
	NotifyEngineConstraintChanged();
	NotifyAllConstraintDofChanged();
}

void reRigConstraintDof::SetKinematicFriction( float friction ){
	if( friction < 0.0f ){
		friction = 0.0;
	}
	
	if( fabsf( friction - pKinematicFriction ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pKinematicFriction = friction;
	
	if( pRigConstraint.GetEngineConstraint() ){
		pRigConstraint.GetEngineConstraint()->GetDof( pDofIndex ).SetKinematicFriction( friction );
	}
	
	NotifyEngineConstraintChanged();
	NotifyAllConstraintDofChanged();
}



void reRigConstraintDof::SetSpringStiffness( float stiffness ){
	if( fabsf( stiffness - pSpringStiffness ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSpringStiffness = stiffness;
	
	if( pRigConstraint.GetEngineConstraint() ){
		pRigConstraint.GetEngineConstraint()->GetDof( pDofIndex ).SetSpringStiffness( stiffness );
	}
	
	NotifyEngineConstraintChanged();
	NotifyAllConstraintDofChanged();
}



void reRigConstraintDof::UpdateEngineDof( deColliderConstraintDof &engDof ) const{
	switch( pDofIndex ){
	case deColliderConstraint::edofLinearX:
	case deColliderConstraint::edofLinearY:
	case deColliderConstraint::edofLinearZ:
		engDof.SetLowerLimit( pLowerLimit );
		engDof.SetUpperLimit( pUpperLimit );
		break;
		
	case deColliderConstraint::edofAngularX:
	case deColliderConstraint::edofAngularY:
	case deColliderConstraint::edofAngularZ:
		engDof.SetLowerLimit( pLowerLimit * DEG2RAD );
		engDof.SetUpperLimit( pUpperLimit * DEG2RAD );
	}
	
	engDof.SetStaticFriction( pStaticFriction );
	engDof.SetKinematicFriction( pKinematicFriction );
	
	engDof.SetSpringStiffness( pSpringStiffness );
}

void reRigConstraintDof::SetFromEngineDof( const deColliderConstraintDof &engDof ){
	switch( pDofIndex ){
	case deColliderConstraint::edofLinearX:
	case deColliderConstraint::edofLinearY:
	case deColliderConstraint::edofLinearZ:
		pLowerLimit = engDof.GetLowerLimit();
		pUpperLimit = engDof.GetUpperLimit();
		break;
		
	case deColliderConstraint::edofAngularX:
	case deColliderConstraint::edofAngularY:
	case deColliderConstraint::edofAngularZ:
		pLowerLimit = engDof.GetLowerLimit() * RAD2DEG;
		pUpperLimit = engDof.GetUpperLimit() * RAD2DEG;
	}
	
	pStaticFriction = engDof.GetStaticFriction();
	pKinematicFriction = engDof.GetKinematicFriction();
	
	pSpringStiffness = engDof.GetSpringStiffness();
}



void reRigConstraintDof::SetParametersFrom( const reRigConstraintDof &dof ){
	pLowerLimit = dof.pLowerLimit;
	pUpperLimit = dof.pUpperLimit;
	
	pStaticFriction = dof.pStaticFriction;
	pKinematicFriction = dof.pKinematicFriction;
	
	pSpringStiffness = dof.pSpringStiffness;
}



void reRigConstraintDof::NotifyAllConstraintDofChanged(){
	if( pRigConstraint.GetRig() ){
		pRigConstraint.GetRig()->NotifyAllConstraintDofChanged( &pRigConstraint, pDofIndex );
	}
}

void reRigConstraintDof::NotifyEngineConstraintChanged(){
	if( ! pRigConstraint.GetEngineConstraint() ){
		return;
	}
	
	deColliderComponent &engSimCollider = *pRigConstraint.GetRig()->GetEngineSimulationCollider();
	engSimCollider.NotifyConstraintChanged( engSimCollider.IndexOfConstraint( pRigConstraint.GetEngineConstraint() ) );
	pRigConstraint.InvalidatePositions();
}
