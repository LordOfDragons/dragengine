/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
