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
#include <stdlib.h>
#include <string.h>

#include "deNavigatorPath.h"
#include "../../../common/exceptions.h"



// Class deNavigatorPath
//////////////////////////

// Constructor, destructor
////////////////////////////

deNavigatorPath::deNavigatorPath() :
pPoints( NULL ),
pCount( 0 ),
pSize( 0 ){
}

deNavigatorPath::deNavigatorPath( const deNavigatorPath &path ) :
pPoints( NULL ),
pCount( 0 ),
pSize( 0 )
{
	if( path.pCount == 0 ){
		return;
	}
	
	pPoints = new decDVector[ path.pCount ];
	memcpy( pPoints, path.pPoints, sizeof( decDVector ) * path.pCount );
	pCount = path.pCount;
	pSize = path.pCount;
}

deNavigatorPath::~deNavigatorPath(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decDVector &deNavigatorPath::GetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return pPoints[ index ];
}

void deNavigatorPath::SetAt( int index, const decDVector &position ){
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	pPoints[ index ] = position;
}

void deNavigatorPath::Add( const decDVector &point ){
	if( pCount == pSize ){
		const int newSize = pCount + 10;
		decDVector * const newArray = new decDVector[ newSize ];
		if( pPoints ){
			memcpy( newArray, pPoints, sizeof( decDVector ) * pCount );
			delete [] pPoints;
		}
		pPoints = newArray;
		pSize = newSize;
	}
	
	pPoints[ pCount++ ] = point;
}

void deNavigatorPath::AddPath( const deNavigatorPath &path ){
	int i;
	for( i=0; i<path.pCount; i++ ){
		Add( path.pPoints[ i ] );
	}
}

void deNavigatorPath::RemoveFrom( int index ){
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=index+1; i<pCount; i++ ){
		pPoints[ i - 1 ] = pPoints[ i ];
	}
	
	pCount--;
}

void deNavigatorPath::RemoveAll(){
	pCount = 0;
}



// Operators
//////////////

deNavigatorPath &deNavigatorPath::operator=( const deNavigatorPath &path ){
	RemoveAll();
	AddPath( path );
	return *this;
}
