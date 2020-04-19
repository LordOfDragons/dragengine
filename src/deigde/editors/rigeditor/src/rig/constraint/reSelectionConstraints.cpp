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
#include <string.h>
#include "reSelectionConstraints.h"
#include "reRigConstraint.h"
#include "reRigConstraintList.h"
#include "../reRig.h"
#include "dragengine/common/exceptions.h"



// Class reSelectionConstraints
/////////////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionConstraints::reSelectionConstraints( reRig *rig ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	
	pConstraints = NULL;
	pConstraintCount = 0;
	pConstraintSize = 0;
	pActiveConstraint = NULL;
}

reSelectionConstraints::~reSelectionConstraints(){
	Reset();
	if( pConstraints ) delete [] pConstraints;
}



// Management
///////////////

reRigConstraint *reSelectionConstraints::GetConstraintAt( int index ) const{
	if( index < 0 || index >= pConstraintCount ) DETHROW( deeOutOfBoundary );
	
	return pConstraints[ index ];
}

bool reSelectionConstraints::HasConstraint( reRigConstraint *constraint ) const{
	if( ! constraint ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pConstraintCount; i++ ){
		if( constraint == pConstraints[ i ] ){
			return true;
		}
	}
	
	return false;
}
	
int reSelectionConstraints::IndexOfConstraint( reRigConstraint *constraint ) const{
	if( ! constraint ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pConstraintCount; i++ ){
		if( constraint == pConstraints[ i ] ){
			return i;
		}
	}
	
	return -1;
}

int reSelectionConstraints::IndexOfConstraintWith( deColliderVolume *collider ) const{
	if( ! collider ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pConstraintCount; i++ ){
		if( collider == pConstraints[ i ]->GetCollider() ){
			return i;
		}
	}
	
	return -1;
}

void reSelectionConstraints::AddConstraint( reRigConstraint *constraint ){
	if( HasConstraint( constraint ) ) DETHROW( deeInvalidParam );
	
	if( pConstraintCount == pConstraintSize ){
		int newSize = pConstraintSize * 3 / 2 + 1;
		reRigConstraint **newArray = new reRigConstraint*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pConstraints ){
			memcpy( newArray, pConstraints, sizeof( reRigConstraint* ) * pConstraintSize );
			delete [] pConstraints;
		}
		pConstraints = newArray;
		pConstraintSize = newSize;
	}
	
	pConstraints[ pConstraintCount ] = constraint;
	pConstraintCount++;
	
	constraint->AddReference();
	
	constraint->SetSelected( true );
	
	pRig->NotifyConstraintSelectedChanged( constraint );
	
	if( pActiveConstraint == NULL ){
		SetActiveConstraint( constraint );
	}
}

void reSelectionConstraints::RemoveConstraint( reRigConstraint *constraint ){
	int i, index = IndexOfConstraint( constraint );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pConstraintCount; i++ ){
		pConstraints[ i - 1 ] = pConstraints[ i ];
	}
	pConstraints[ pConstraintCount - 1 ] = NULL;
	pConstraintCount--;
	
	constraint->SetSelected( false );
	
	if( constraint == pActiveConstraint ){
		if( pConstraintCount > 0 ){
			SetActiveConstraint( pConstraints[ 0 ] );
			
		}else{
			SetActiveConstraint( NULL );
		}
	}
	
	pRig->NotifyConstraintSelectedChanged( constraint );
	
	constraint->FreeReference();
}

void reSelectionConstraints::RemoveAllConstraints(){
	SetActiveConstraint( NULL );
	
	pRig->NotifyAllConstraintsDeselected();
	
	while( pConstraintCount > 0 ){
		pConstraintCount--;
		
		pConstraints[ pConstraintCount ]->SetSelected( false );
		pConstraints[ pConstraintCount ]->FreeReference();
	}
}



bool reSelectionConstraints::HasActiveConstraint() const{
	return pActiveConstraint != NULL;
}

void reSelectionConstraints::SetActiveConstraint( reRigConstraint *constraint ){
	if( constraint != pActiveConstraint ){
		if( constraint && ! HasConstraint( constraint ) ) DETHROW( deeInvalidParam );
		
		if( pActiveConstraint ){
			pActiveConstraint->SetActive( false );
		}
		
		pActiveConstraint = constraint;
		
		if( constraint ){
			constraint->SetActive( true );
		}
		
		pRig->NotifyActiveConstraintChanged();
	}
}

void reSelectionConstraints::Reset(){
	RemoveAllConstraints();
}

void reSelectionConstraints::AddVisibleConstraintsTo( reRigConstraintList &list ) const{
	int c;
	
	for( c=0; c<pConstraintCount; c++ ){
		if( pConstraints[ c ]->IsVisible() ){
			list.AddConstraint( pConstraints[ c ] );
		}
	}
}
