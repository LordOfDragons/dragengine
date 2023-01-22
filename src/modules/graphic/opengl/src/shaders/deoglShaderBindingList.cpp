/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglShaderBindingList.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderBindingList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderBindingList::deoglShaderBindingList() :
pBindings( nullptr ),
pCount( 0 ),
pSize( 0 ){
}

deoglShaderBindingList::~deoglShaderBindingList(){
	RemoveAll();
	if( pBindings ){
		delete [] pBindings;
	}
}



// Management
///////////////

bool deoglShaderBindingList::HasNamed( const char *name ){
	int i;
	for( i=0; i<pCount; i++ ){
		if( pBindings[ i ].name == name ){
			return true;
		}
	}
	
	return false;
}

int deoglShaderBindingList::IndexOfNamed( const char *name ) const{
	int i;
	for( i=0; i<pCount; i++ ){
		if( pBindings[ i ].name == name ){
			return i;
		}
	}
	
	return -1;
}

const decString &deoglShaderBindingList::GetNameAt( int index ) const{
	DEASSERT_TRUE( index >= 0  )
	DEASSERT_TRUE( index < pCount )
	
	return pBindings[ index ].name;
}

int deoglShaderBindingList::GetTargetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pBindings[ index ].target;
}

void deoglShaderBindingList::Add( const char *name, int target ){
	DEASSERT_FALSE( HasNamed( name ) )
	
	if( pCount == pSize ){
		const int newSize = pSize * 3 / 2 + 1;
		sBinding * const newArray = new sBinding[ newSize ];
		if( pBindings ){
			int i;
			for( i=0; i<pSize; i++ ){
				newArray[ i ] = pBindings[ i ];
			}
			delete [] pBindings;
		}
		pBindings = newArray;
		pSize = newSize;
	}
	
	pBindings[ pCount ].name = name;
	pBindings[ pCount ].target = target;
	pCount++;
}

void deoglShaderBindingList::RemoveAll(){
	pCount = 0;
}
