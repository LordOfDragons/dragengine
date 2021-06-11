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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRComponent.h"
#include "deoglComponentSet.h"

#include <dragengine/common/exceptions.h>



// Class deoglComponentSet
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponentSet::deoglComponentSet(){
	pComponents = NULL;
	pComponentCount = 0;
	pComponentSize = 0;
}

deoglComponentSet::~deoglComponentSet(){
	if( pComponents ){
		delete [] pComponents;
	}
}



// Management
///////////////

deoglRComponent *deoglComponentSet::GetAt( int index ) const{
	if( index < 0 || index >= pComponentCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pComponents[ index ];
}

int deoglComponentSet::IndexOfComponent( deoglRComponent *component ) const{
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		if( component == pComponents[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool deoglComponentSet::Has( deoglRComponent *component ) const{
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		if( component == pComponents[ i ] ){
			return true;
		}
	}
	
	return false;
}

void deoglComponentSet::Add( deoglRComponent *component ){
	if( Has( component ) ){
		DETHROW( deeInvalidParam );
	}
	
	pAddComponent( component );
}

bool deoglComponentSet::AddIfMissing( deoglRComponent *component ){
	if( Has( component ) ){
		return false;
	}
	
	pAddComponent( component );
	return true;
}

void deoglComponentSet::Remove( deoglRComponent *component ){
	const int index = IndexOfComponent( component );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pComponentCount - 1 ){
		pComponents[ index ] = pComponents[ pComponentCount - 1 ];
	}
	pComponentCount--;
}

bool deoglComponentSet::RemoveIfExisting( deoglRComponent *component ){
	const int index = IndexOfComponent( component );
	
	if( index == -1 ){
		return false;
	}
	
	if( index < pComponentCount - 1 ){
		pComponents[ index ] = pComponents[ pComponentCount - 1 ];
	}
	pComponentCount--;
	return true;
}

void deoglComponentSet::RemoveFrom( int index ){
	if( index < 0 || index >= pComponentCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pComponentCount - 1 ){
		pComponents[ index ] = pComponents[ pComponentCount - 1 ];
	}
	pComponentCount--;
}

void deoglComponentSet::RemoveAll(){
	pComponentCount = 0;
}



void deoglComponentSet::MarkAll( bool mark ){
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		pComponents[ i ]->SetMarked( mark );
	}
}

void deoglComponentSet::RemoveAllMarked( bool mark ){
	int last = 0;
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		if( pComponents[ i ]->GetMarked() != mark ){
			if( i != last ){
				deoglRComponent * const swap = pComponents[ last ];
				pComponents[ last ] = pComponents[ i ];
				pComponents[ i ] = swap;
			}
			last++;
		}
	}
	pComponentCount = last;
}



// Private Functions
//////////////////////

void deoglComponentSet::pAddComponent( deoglRComponent *component ){
	if( pComponentCount == pComponentSize ){
		int newSize = pComponentCount + 10; // * 3 / 2 + 1;
		deoglRComponent **newArray = new deoglRComponent*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		if( pComponents ){
			memcpy( newArray, pComponents, sizeof( deoglRComponent* ) * pComponentSize );
			delete [] pComponents;
		}
		
		pComponents = newArray;
		pComponentSize = newSize;
	}
	
	pComponents[ pComponentCount++ ] = component;
}
