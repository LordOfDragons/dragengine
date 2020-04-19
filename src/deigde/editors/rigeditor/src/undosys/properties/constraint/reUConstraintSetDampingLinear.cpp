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

#include "reUConstraintSetDampingLinear.h"

#include "../../../rig/reRig.h"
#include "../../../rig/constraint/reRigConstraint.h"

#include <dragengine/common/exceptions.h>



// Class reUConstraintSetDampingLinear
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

reUConstraintSetDampingLinear::reUConstraintSetDampingLinear( reRigConstraint *constraint, float newDamping ){
	if( ! constraint || ! constraint->GetRig() ){
		DETHROW( deeInvalidParam );
	}
	
	pConstraint = constraint;
	
	pOldDamping = constraint->GetLinearDamping();
	pNewDamping = newDamping;
	
	SetShortInfo( "Constraint set linear damping" );
	
	pConstraint->AddReference();
}

reUConstraintSetDampingLinear::~reUConstraintSetDampingLinear(){
	if( pConstraint ){
		pConstraint->FreeReference();
	}
}



// Management
///////////////

void reUConstraintSetDampingLinear::Undo(){
	pConstraint->SetLinearDamping( pOldDamping );
}

void reUConstraintSetDampingLinear::Redo(){
	pConstraint->SetLinearDamping( pNewDamping );
}
