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

#include "reURemoveConstraint.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reRigConstraintList.h"
#include "../../../rig/constraint/reSelectionConstraints.h"

#include <dragengine/common/exceptions.h>



// Class reURemoveConstraint
//////////////////////////////

// Constructor, destructor
////////////////////////////

reURemoveConstraint::reURemoveConstraint( reRigConstraintList &list ){
	int constraintCount = list.GetConstraintCount();
	reRigConstraint *constraint;
	reRigBone *bone;
	
	if( constraintCount == 0 ) DETHROW( deeInvalidParam );
	
	pRig = list.GetConstraintAt( 0 )->GetRig();
	if( ! pRig ) DETHROW( deeInvalidParam );
	pRig->AddReference();
	
	pEntries = NULL;
	pEntryCount = 0;
	
	try{
		pEntries = new sEntry[ constraintCount ];
		if( ! pEntries ) DETHROW( deeOutOfMemory );
		
		while( pEntryCount < constraintCount ){
			constraint = list.GetConstraintAt( pEntryCount );
			bone = constraint->GetRigBone();
			
			if( constraint->GetRig() != pRig ) DETHROW( deeInvalidParam );
			
			pEntries[ pEntryCount ].constraint = constraint;
			constraint->AddReference();
			
			pEntries[ pEntryCount ].bone = bone;
			if( bone ) bone->AddReference();
			
			pEntryCount++;
		}
		
		SetShortInfo( "Remove Constraints" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reURemoveConstraint::~reURemoveConstraint(){
	pCleanUp();
}



// Management
///////////////

void reURemoveConstraint::Undo(){
	reSelectionConstraints *selection = pRig->GetSelectionConstraints();
	int e;
	
	selection->RemoveAllConstraints();
	
	for( e=0; e<pEntryCount; e++ ){
		if( pEntries[ e ].bone ){
			pEntries[ e ].bone->AddConstraint( pEntries[ e ].constraint );
			
		}else{
			pRig->AddConstraint( pEntries[ e ].constraint );
		}
		
		selection->AddConstraint( pEntries[ e ].constraint );
	}
	
}

void reURemoveConstraint::Redo(){
	reSelectionConstraints *selection = pRig->GetSelectionConstraints();
	int e;
	
	for( e=0; e<pEntryCount; e++ ){
		if( pEntries[ e ].constraint->GetSelected() ){
			selection->RemoveConstraint( pEntries[ e ].constraint );
		}
		
		if( pEntries[ e ].bone ){
			pEntries[ e ].bone->RemoveConstraint( pEntries[ e ].constraint );
			
		}else{
			pRig->RemoveConstraint( pEntries[ e ].constraint );
		}
	}
}



// Private Functions
//////////////////////

void reURemoveConstraint::pCleanUp(){
	if( pEntries ){
		while( pEntryCount > 0 ){
			pEntryCount--;
			
			if( pEntries[ pEntryCount ].bone ){
				pEntries[ pEntryCount ].bone->FreeReference();
			}
			pEntries[ pEntryCount ].constraint->FreeReference();
		}
		
		delete [] pEntries;
	}
	
	if( pRig ){
		pRig->FreeReference();
	}
}
