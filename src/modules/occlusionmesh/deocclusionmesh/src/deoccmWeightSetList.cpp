/* 
 * Drag[en]gine Occlusion Mesh Module
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoccmWeightSet.h"
#include "deoccmWeightSetList.h"

#include <dragengine/common/exceptions.h>



// Class deoccmWeightSetList
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoccmWeightSetList::deoccmWeightSetList(){
}

deoccmWeightSetList::~deoccmWeightSetList(){
	RemoveAll();
}



// Management
///////////////

int deoccmWeightSetList::GetCount() const{
	return pList.GetCount();
}

deoccmWeightSet *deoccmWeightSetList::GetAt( int index ) const{
	return ( deoccmWeightSet* )pList.GetAt( index );
}

void deoccmWeightSetList::Add( deoccmWeightSet *weightSet ){
	if( ! weightSet ){
		DETHROW( deeInvalidParam );
	}
	
	pList.Add( weightSet );
}

void deoccmWeightSetList::RemoveAll(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoccmWeightSet* )pList.GetAt( i );
	}
	
	pList.RemoveAll();
}

int deoccmWeightSetList::IndexOfEqual( const deoccmWeightSet &weightSet ) const{
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoccmWeightSet* )pList.GetAt( i ) )->Equals( weightSet ) ){
			return i;
		}
	}
	
	return -1;
}

int deoccmWeightSetList::GetLargestWeightCount() const{
	const int count = pList.GetCount();
	int weightCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		const deoccmWeightSet &weightSet = *( ( deoccmWeightSet* )pList.GetAt( i ) );
		
		if( weightSet.GetCount() > weightCount ){
			weightCount = weightSet.GetCount();
		}
	}
	
	return weightCount;
}
