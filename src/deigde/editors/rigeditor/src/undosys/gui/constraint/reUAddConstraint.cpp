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

#include "reUAddConstraint.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reSelectionConstraints.h"

#include <dragengine/common/exceptions.h>



// Class reUAddConstraint
///////////////////////////

// Constructor, destructor
////////////////////////////

reUAddConstraint::reUAddConstraint( reRig *rig, reRigBone *bone, reRigConstraint *constraint ){
	if( ! constraint || ( ! rig && ! bone ) ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	if( rig ) rig->AddReference();
	
	pBone = bone;
	if( bone ) bone->AddReference();
	
	pConstraint = constraint;
	constraint->AddReference();
	
	try{
		if( bone ){
			SetShortInfo( "Add Bone Constraint" );
			
		}else{
			SetShortInfo( "Add Rig Constraint" );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reUAddConstraint::~reUAddConstraint(){
	pCleanUp();
}



// Management
///////////////

void reUAddConstraint::Undo(){
	reRig *rig = pGetRig();
	
	reSelectionConstraints *selection = rig->GetSelectionConstraints();
	
	if( pConstraint->GetSelected() ){
		selection->RemoveConstraint( pConstraint );
	}
	
	if( pBone ){
		pBone->RemoveConstraint( pConstraint );
		
	}else{
		rig->RemoveConstraint( pConstraint );
	}
}

void reUAddConstraint::Redo(){
	reRig *rig = pGetRig();
	
	reSelectionConstraints *selection = rig->GetSelectionConstraints();
	
	if( pBone ){
		pBone->AddConstraint( pConstraint );
		
	}else{
		rig->AddConstraint( pConstraint );
	}
	
	selection->RemoveAllConstraints();
	selection->AddConstraint( pConstraint );
}



// Private Functions
//////////////////////

void reUAddConstraint::pCleanUp(){
	if( pConstraint ) pConstraint->FreeReference();
	if( pBone ) pBone->FreeReference();
	if( pRig ) pRig->FreeReference();
}

reRig *reUAddConstraint::pGetRig(){
	if( pRig ) return pRig;
	
	reRig *rig = pBone->GetRig();
	if( ! rig ) DETHROW( deeInvalidParam );
	
	return rig;
}
