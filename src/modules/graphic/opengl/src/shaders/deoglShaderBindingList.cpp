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

deoglShaderBindingList::deoglShaderBindingList(){
	pBindings = NULL;
	pCount = 0;
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
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pCount; i++ ){
		if( strcmp( name, pBindings[ i ].name ) == 0 ){
			return true;
		}
	}
	
	return false;
}

int deoglShaderBindingList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pCount; i++ ){
		if( strcmp( name, pBindings[ i ].name ) == 0 ){
			return i;
		}
	}
	
	return -1;
}

const char *deoglShaderBindingList::GetNameAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return ( const char * )pBindings[ index ].name;
}

int deoglShaderBindingList::GetTargetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pBindings[ index ].target;
}

void deoglShaderBindingList::Add( const char *name, int target ){
	if( HasNamed( name ) ){
		DETHROW( deeInvalidParam );
	}
	
	sBinding * const newArray = new sBinding[ pCount + 1 ];
	if( pBindings ){
		memcpy( newArray, pBindings, sizeof( sBinding ) * pCount );
		delete [] pBindings;
	}
	pBindings = newArray;
	
	pBindings[ pCount ].name = new char[ strlen( name ) + 1 ];
	strcpy( pBindings[ pCount ].name, name );
	pBindings[ pCount ].target = target;
	pCount++;
}

void deoglShaderBindingList::RemoveAll(){
	if( pBindings ){
		while( pCount > 0 ){
			pCount--;
			delete [] pBindings[ pCount ].name;
		}
		
		delete [] pBindings;
		pBindings = NULL;
	}
}
