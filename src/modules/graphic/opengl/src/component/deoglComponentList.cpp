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
#include "deoglComponentList.h"

#include <dragengine/common/exceptions.h>



// Class deoglComponentList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponentList::deoglComponentList(){
	pComponents = NULL;
	pComponentCount = 0;
	pComponentSize = 0;
}

deoglComponentList::~deoglComponentList(){
	if( pComponents ){
		delete [] pComponents;
	}
}



// Management
///////////////

deoglRComponent *deoglComponentList::GetAt( int index ) const{
	if( index < 0 || index >= pComponentCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pComponents[ index ];
}

int deoglComponentList::IndexOfComponent( deoglRComponent *component ) const{
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

bool deoglComponentList::Has( deoglRComponent *component ) const{
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

void deoglComponentList::Add( deoglRComponent *component ){
	if( Has( component ) ){
		DETHROW( deeInvalidParam );
	}
	
	pAddComponent( component );
}

bool deoglComponentList::AddIfMissing( deoglRComponent *component ){
	if( Has( component ) ){
		return false;
	}
	
	pAddComponent( component );
	return true;
}

void deoglComponentList::Remove( deoglRComponent *component ){
	const int index = IndexOfComponent( component );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pComponentCount - 1 ){
		pComponents[ index ] = pComponents[ pComponentCount - 1 ];
	}
	pComponentCount--;
}

bool deoglComponentList::RemoveIfExisting( deoglRComponent *component ){
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

void deoglComponentList::RemoveFrom( int index ){
	if( index < 0 || index >= pComponentCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pComponentCount - 1 ){
		pComponents[ index ] = pComponents[ pComponentCount - 1 ];
	}
	pComponentCount--;
}

void deoglComponentList::RemoveAll(){
	pComponentCount = 0;
}



void deoglComponentList::MarkAll( bool mark ){
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		pComponents[ i ]->SetMarked( mark );
	}
}

void deoglComponentList::RemoveAllMarked( bool mark ){
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

void deoglComponentList::pAddComponent( deoglRComponent *component ){
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
