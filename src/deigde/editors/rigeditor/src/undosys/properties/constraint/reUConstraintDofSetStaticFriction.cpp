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

#include <stdio.h>
#include <stdlib.h>

#include "reUConstraintDofSetStaticFriction.h"

#include "../../../rig/reRig.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reRigConstraintDof.h"

#include <dragengine/common/exceptions.h>



// Class reUConstraintDofSetStaticFriction
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

reUConstraintDofSetStaticFriction::reUConstraintDofSetStaticFriction( reRigConstraint *constraint,
deColliderConstraint::eDegreesOfFreedom dof, float newValue ){
	if( ! constraint || ! constraint->GetRig()
	|| dof < deColliderConstraint::edofLinearX || dof > deColliderConstraint::edofAngularZ ){
		DETHROW( deeInvalidParam );
	}
	
	pConstraint = constraint;
	pDof = dof;
	
	pOldValue = constraint->GetDof( dof ).GetStaticFriction();
	pNewValue = newValue;
	
	SetShortInfo( "Constraint DOF set static friction" );
	
	pConstraint->AddReference();
}

reUConstraintDofSetStaticFriction::~reUConstraintDofSetStaticFriction(){
	if( pConstraint ){
		pConstraint->FreeReference();
	}
}



// Management
///////////////

void reUConstraintDofSetStaticFriction::Undo(){
	pConstraint->GetDof( pDof ).SetStaticFriction( pOldValue );
}

void reUConstraintDofSetStaticFriction::Redo(){
	pConstraint->GetDof( pDof ).SetStaticFriction( pNewValue );
}
