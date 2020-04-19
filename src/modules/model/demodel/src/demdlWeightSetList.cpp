/* 
 * Drag[en]gine Model Module
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

#include "demdlWeightSet.h"
#include "demdlWeightSetList.h"

#include <dragengine/common/exceptions.h>



// Class demdlWeightSetList
/////////////////////////////

// Constructor, destructor
////////////////////////////

demdlWeightSetList::demdlWeightSetList(){
}

demdlWeightSetList::~demdlWeightSetList(){
	RemoveAll();
}



// Management
///////////////

int demdlWeightSetList::GetCount() const{
	return pList.GetCount();
}

demdlWeightSet *demdlWeightSetList::GetAt( int index ) const{
	return ( demdlWeightSet* )pList.GetAt( index );
}

void demdlWeightSetList::Add( demdlWeightSet *weightSet ){
	if( ! weightSet ){
		DETHROW( deeInvalidParam );
	}
	
	pList.Add( weightSet );
}

void demdlWeightSetList::RemoveAll(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( demdlWeightSet* )pList.GetAt( i );
	}
	
	pList.RemoveAll();
}

int demdlWeightSetList::IndexOfEqual( const demdlWeightSet &weightSet ) const{
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( demdlWeightSet* )pList.GetAt( i ) )->Equals( weightSet ) ){
			return i;
		}
	}
	
	return -1;
}

int demdlWeightSetList::GetLargestWeightCount() const{
	const int count = pList.GetCount();
	int weightCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		const demdlWeightSet &weightSet = *( ( demdlWeightSet* )pList.GetAt( i ) );
		
		if( weightSet.GetCount() > weightCount ){
			weightCount = weightSet.GetCount();
		}
	}
	
	return weightCount;
}
