/* 
 * Drag[en]gine Rig Module
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
#include "dermNameList.h"
#include "dragengine/common/exceptions.h"



// Class dermNameList
///////////////////////

// Constructor, destructor
////////////////////////////

dermNameList::dermNameList(){
	pNames = nullptr;
	pNameCount = 0;
	pNameSize = 0;
}

dermNameList::~dermNameList(){
	if( pNames ){
		delete [] pNames;
	}
}



// Management
///////////////

const decString &dermNameList::GetNameAt( int index ) const{
	if( index < 0 || index >= pNameCount ) DETHROW( deeInvalidParam );
	
	return pNames[ index ].name;
}

int dermNameList::GetNameNumberAt( int index ) const{
	if( index < 0 || index >= pNameCount ) DETHROW( deeInvalidParam );
	
	return pNames[ index ].number;
}

void dermNameList::SetNameNumberAt( int index, int number ){
	if( index < 0 || index >= pNameCount ) DETHROW( deeInvalidParam );
	
	pNames[ index ].number = number;
}

int dermNameList::IndexOfName( const char *name ) const{
	DEASSERT_NOTNULL( name )
	int n;
	
	for( n=0; n<pNameCount; n++ ){
		if( pNames[ n ].name == name ){
			return n;
		}
	}
	
	return -1;
}

bool dermNameList::HasName( const char *name ) const{
	DEASSERT_NOTNULL( name )
	int n;
	
	for( n=0; n<pNameCount; n++ ){
		if( pNames[ n ].name == name ){
			return true;
		}
	}
	
	return false;
}

int dermNameList::AddName( const char *name ){
	int index = IndexOfName( name );
	if( index != -1 ){
		return index;
	}
	
	if( pNameCount == pNameSize ){
		int i, newSize = pNameSize * 3 / 2 + 1;
		sName * const newArray = new sName[ newSize ];
		if( pNames ){
			for( i=0; i<pNameSize; i++ ){
				newArray[ i ] = pNames[ i ];
			}
			delete [] pNames;
		}
		pNames = newArray;
		pNameSize = newSize;
	}
	
	pNames[ pNameCount ].name = name;
	pNames[ pNameCount ].number = -1;
	pNameCount++;
	
	return pNameCount - 1;
}
