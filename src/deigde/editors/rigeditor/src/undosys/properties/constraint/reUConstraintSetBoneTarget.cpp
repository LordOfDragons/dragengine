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

#include "reUConstraintSetBoneTarget.h"

#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/constraint/reRigConstraint.h"

#include <dragengine/common/exceptions.h>



// Class reUConstraintSetBoneTarget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

reUConstraintSetBoneTarget::reUConstraintSetBoneTarget( reRigConstraint *constraint, reRigBone *newBone ){
	if( ! constraint || ! constraint->GetRig() ){
		DETHROW( deeInvalidParam );
	}
	
	pConstraint = constraint;
	
	pOldBone = constraint->GetConstraintBone();
	pNewBone = newBone;
	
	SetShortInfo( "Constraint set target bone" );
	
	pConstraint->AddReference();
	if( pOldBone ){
		pOldBone->AddReference();
	}
	if( pNewBone ){
		pNewBone->AddReference();
	}
}

reUConstraintSetBoneTarget::~reUConstraintSetBoneTarget(){
	if( pNewBone ){
		pNewBone->FreeReference();
	}
	if( pOldBone ){
		pOldBone->FreeReference();
	}
	if( pConstraint ){
		pConstraint->FreeReference();
	}
}



// Management
///////////////

void reUConstraintSetBoneTarget::Undo(){
	pConstraint->SetConstraintBone( pOldBone );
}

void reUConstraintSetBoneTarget::Redo(){
	pConstraint->SetConstraintBone( pNewBone );
}
