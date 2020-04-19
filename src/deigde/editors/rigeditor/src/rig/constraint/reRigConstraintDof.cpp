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

reRigConstraintDof::reRigConstraintDof( reRigConstraint *constraint, deColliderConstraint::eDegreesOfFreedom dofIndex ){
	if( ! constraint || dofIndex < deColliderConstraint::edofLinearX
	|| dofIndex > deColliderConstraint::deColliderConstraint::edofAngularZ ){
		DETHROW( deeInvalidParam );
	}
	
	pRigConstraint = constraint;
	pDofIndex = dofIndex;
	
	pLowerLimit = 0.0f;
	pUpperLimit = 0.0f;
	
	pStaticFriction = 0.0f;
	pKinematicFriction = 0.0f;
	
	pSpringStiffness = 0.0f;
}

reRigConstraintDof::~reRigConstraintDof(){
	pCleanUp();
}



// Management
///////////////

void reRigConstraintDof::SetLowerLimit( float lowerLimit ){
	if( fabsf( lowerLimit - pLowerLimit ) > FLOAT_SAFE_EPSILON ){
		pLowerLimit = lowerLimit;
		
		if( pRigConstraint->GetEngineConstraint() ){
			if( pDofIndex == deColliderConstraint::edofLinearX
			|| pDofIndex == deColliderConstraint::edofLinearY
			|| pDofIndex == deColliderConstraint::edofLinearZ ){
				pRigConstraint->GetEngineConstraint()->GetDof( pDofIndex ).SetLowerLimit( lowerLimit );
				
			}else{
				pRigConstraint->GetEngineConstraint()->GetDof( pDofIndex ).SetLowerLimit( lowerLimit * DEG2RAD );
			}
		}
		
		NotifyEngineConstraintChanged();
		NotifyAllConstraintDofChanged();
	}
}

void reRigConstraintDof::SetUpperLimit( float upperLimit ){
	if( fabsf( upperLimit - pUpperLimit ) > FLOAT_SAFE_EPSILON ){
		pUpperLimit = upperLimit;
		
		if( pRigConstraint->GetEngineConstraint() ){
			if( pDofIndex == deColliderConstraint::edofLinearX
			|| pDofIndex == deColliderConstraint::edofLinearY
			|| pDofIndex == deColliderConstraint::edofLinearZ ){
				pRigConstraint->GetEngineConstraint()->GetDof( pDofIndex ).SetUpperLimit( upperLimit );
				
			}else{
				pRigConstraint->GetEngineConstraint()->GetDof( pDofIndex ).SetUpperLimit( upperLimit * DEG2RAD );
			}
		}
		
		NotifyEngineConstraintChanged();
		NotifyAllConstraintDofChanged();
	}
}



void reRigConstraintDof::SetStaticFriction( float friction ){
	if( friction < 0.0f ){
		friction = 0.0;
	}
	
	if( fabsf( friction - pStaticFriction ) > FLOAT_SAFE_EPSILON ){
		pStaticFriction = friction;
		
		if( pRigConstraint->GetEngineConstraint() ){
			pRigConstraint->GetEngineConstraint()->GetDof( pDofIndex ).SetStaticFriction( friction );
		}
		
		NotifyEngineConstraintChanged();
		NotifyAllConstraintDofChanged();
	}
}

void reRigConstraintDof::SetKinematicFriction( float friction ){
	if( friction < 0.0f ){
		friction = 0.0;
	}
	
	if( fabsf( friction - pKinematicFriction ) > FLOAT_SAFE_EPSILON ){
		pKinematicFriction = friction;
		
		if( pRigConstraint->GetEngineConstraint() ){
			pRigConstraint->GetEngineConstraint()->GetDof( pDofIndex ).SetKinematicFriction( friction );
		}
		
		NotifyEngineConstraintChanged();
		NotifyAllConstraintDofChanged();
	}
}



void reRigConstraintDof::SetSpringStiffness( float stiffness ){
	if( fabsf( stiffness - pSpringStiffness ) > FLOAT_SAFE_EPSILON ){
		pSpringStiffness = stiffness;
		
		if( pRigConstraint->GetEngineConstraint() ){
			pRigConstraint->GetEngineConstraint()->GetDof( pDofIndex ).SetSpringStiffness( stiffness );
		}
		
		NotifyEngineConstraintChanged();
		NotifyAllConstraintDofChanged();
	}
}



void reRigConstraintDof::UpdateEngineDof( deColliderConstraintDof &engDof ) const{
	if( pDofIndex == deColliderConstraint::edofLinearX
	|| pDofIndex == deColliderConstraint::edofLinearY
	|| pDofIndex == deColliderConstraint::edofLinearZ ){
		engDof.SetLowerLimit( pLowerLimit );
		engDof.SetUpperLimit( pUpperLimit );
		
	}else{
		engDof.SetLowerLimit( pLowerLimit * DEG2RAD );
		engDof.SetUpperLimit( pUpperLimit * DEG2RAD );
	}
	
	engDof.SetStaticFriction( pStaticFriction );
	engDof.SetKinematicFriction( pKinematicFriction );
	
	engDof.SetSpringStiffness( pSpringStiffness );
}

void reRigConstraintDof::SetFromEngineDof( const deColliderConstraintDof &engDof ){
	if( pDofIndex == deColliderConstraint::edofLinearX
	|| pDofIndex == deColliderConstraint::edofLinearY
	|| pDofIndex == deColliderConstraint::edofLinearZ ){
		pLowerLimit = engDof.GetLowerLimit();
		pUpperLimit = engDof.GetUpperLimit();
		
	}else{
		pLowerLimit = engDof.GetLowerLimit() / DEG2RAD;
		pUpperLimit = engDof.GetUpperLimit() / DEG2RAD;
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
	if( pRigConstraint->GetRig() ){
		pRigConstraint->GetRig()->NotifyAllConstraintDofChanged( pRigConstraint, pDofIndex );
	}
}

void reRigConstraintDof::NotifyEngineConstraintChanged(){
	if( pRigConstraint->GetEngineConstraint() ){
		deColliderComponent &engSimCollider = *pRigConstraint->GetRig()->GetEngineSimulationCollider();
		engSimCollider.NotifyConstraintChanged( engSimCollider.IndexOfConstraint( pRigConstraint->GetEngineConstraint() ) );
		pRigConstraint->InvalidatePositions();
	}
}



// Operators
//////////////

void reRigConstraintDof::pCleanUp(){
}
