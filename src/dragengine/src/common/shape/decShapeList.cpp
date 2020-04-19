/* 
 * Drag[en]gine Game Engine
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
#include <string.h>

#include "decShape.h"
#include "decShapeList.h"
#include "decShapeVisitor.h"
#include "../exceptions.h"



// Class decShapeList
///////////////////////

// Constructors, destructors
//////////////////////////////

decShapeList::decShapeList() :
pShapes( NULL ),
pCount( 0 ),
pSize( 0 ){
}

decShapeList::decShapeList( const decShapeList &copy ) :
pShapes( NULL ),
pCount( 0 ),
pSize( 0 )
{
	if( copy.pCount == 0 ){
		return;
	}
	
	try{
		pShapes = new decShape*[ copy.pCount ];
		pSize = copy.pCount;
		
		for( pCount=0; pCount<copy.pCount; pCount++ ){
			pShapes[ pCount ] = copy.pShapes[ pCount ]->Copy();
		}
		
	}catch( ... ){
		pCleanUp();
		throw;
	}
}

decShapeList::~decShapeList(){
	pCleanUp();
}



// Management
///////////////

decShape *decShapeList::GetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pShapes[ index ];
}

int decShapeList::IndexOf( decShape *shape ) const{
	if( ! shape ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pCount; i++ ){
		if( shape == pShapes[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool decShapeList::Has( decShape *shape ) const{
	if( ! shape ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pCount; i++ ){
		if( shape == pShapes[ i ] ){
			return true;
		}
	}
	
	return false;
}

void decShapeList::SetAt( int index, decShape *shape ){
	if( ! shape || index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( shape == pShapes[ index ] ){
		return;
	}
	
	delete pShapes[ index ];
	pShapes[ index ] = shape;
}

void decShapeList::Add( decShape *shape ){
	if( Has( shape ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pCount == pSize ){
		const int newSize = pSize * 3 / 2 + 1;
		decShape ** const newArray = new decShape*[ newSize ];
		if( pShapes ){
			memcpy( newArray, pShapes, sizeof( decShape* ) * pSize );
			delete [] pShapes;
		}
		pShapes = newArray;
		pSize = newSize;
	}
	
	pShapes[ pCount ] = shape;
	pCount++;
}

void decShapeList::Remove( decShape *shape ){
	const int index = IndexOf( shape );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=index+1; i<pCount; i++ ){
		pShapes[ i - 1 ] = pShapes[ i ];
	}
	pCount--;
	
	delete shape;
}

void decShapeList::RemoveFrom( int index ){
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	delete pShapes[ index ];
	
	for( i=index+1; i<pCount; i++ ){
		pShapes[ i - 1 ] = pShapes[ i ];
	}
	pCount--;
}

void decShapeList::RemoveAll(){
	while( pCount > 0 ){
		pCount--;
		delete pShapes[ pCount ];
	}
}



void decShapeList::Visit( decShapeVisitor &visitor ){
	int i;
	for( i=0; i<pCount; i++ ){
		pShapes[ i ]->Visit( visitor );
	}
}



// Operators
//////////////

decShapeList &decShapeList::operator=( const decShapeList& list ){
	RemoveAll();
	
	while( pCount < list.pCount ){
		Add( list.pShapes[ pCount ]->Copy() );
	}
	
	return *this;
}



// Private Functions
//////////////////////

void decShapeList::pCleanUp(){
	RemoveAll();
	if( pShapes ){
		delete [] pShapes;
	}
}
