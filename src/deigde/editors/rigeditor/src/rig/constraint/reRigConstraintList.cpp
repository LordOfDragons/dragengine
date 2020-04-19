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
#include "reRigConstraintList.h"
#include "dragengine/common/exceptions.h"



// Class reRigConstraintList
/////////////////////////

// Constructor, destructor
////////////////////////////

reRigConstraintList::reRigConstraintList(){
	pConstraints = NULL;
	pConstraintCount = 0;
	pConstraintSize = 0;
}

reRigConstraintList::~reRigConstraintList(){
	RemoveAllConstraints();
	if( pConstraints ) delete [] pConstraints;
}



// Management
///////////////

reRigConstraint *reRigConstraintList::GetConstraintAt( int index ) const{
	if( index < 0 || index >= pConstraintCount ) DETHROW( deeInvalidParam );
	
	return pConstraints[ index ];
}

int reRigConstraintList::IndexOfConstraint( reRigConstraint *constraint ) const{
	if( ! constraint ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pConstraintCount; i++ ){
		if( constraint == pConstraints[ i ] ) return i;
	}
	
	return -1;
}

bool reRigConstraintList::HasConstraint( reRigConstraint *constraint ) const{
	if( ! constraint ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pConstraintCount; i++ ){
		if( constraint == pConstraints[ i ] ) return true;
	}
	
	return false;
}

void reRigConstraintList::AddConstraint( reRigConstraint *constraint ){
	if( HasConstraint( constraint ) ) DETHROW( deeInvalidParam );
	
	if( pConstraintCount == pConstraintSize ){
		int newSize = pConstraintSize * 3 / 2 + 1;
		reRigConstraint **newArray = new reRigConstraint*[ newSize ];
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
}

void reRigConstraintList::RemoveConstraint( reRigConstraint *constraint ){
	int i, index = IndexOfConstraint( constraint );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pConstraintCount; i++ ){
		pConstraints[ i - 1 ] = pConstraints[ i ];
	}
	pConstraintCount--;
	
	constraint->FreeReference();
}

void reRigConstraintList::RemoveAllConstraints(){
	while( pConstraintCount > 0 ){
		pConstraintCount--;
		pConstraints[ pConstraintCount ]->FreeReference();
	}
}
