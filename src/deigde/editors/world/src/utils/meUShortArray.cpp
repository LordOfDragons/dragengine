/* 
 * Drag[en]gine IGDE World Editor
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

#include "meUShortArray.h"

#include <dragengine/common/exceptions.h>



// Class meUShortArray
////////////////////////

// Constructor, destructor
////////////////////////////

meUShortArray::meUShortArray( int colons, int rows ) :
pColons( colons ),
pRows( rows ),
pValues( NULL )
{
	if( colons < 1 || rows < 1 ) {
		DETHROW( deeInvalidParam );
	}
	
	pValues = new unsigned short[ colons * rows ];
	SetAll( 0 );
}

meUShortArray::meUShortArray( const meUShortArray &array ) :
pColons( array.pColons ),
pRows( array.pRows ),
pValues( NULL )
{
	pValues = new unsigned short[ pColons * pRows ];
	*this = array;
}

meUShortArray::~meUShortArray(){
	if( pValues ){
		delete [] pValues;
	}
}



// Management
///////////////

int meUShortArray::GetValueAt( int x, int y ) const{
	if( x < 0 || x >= pColons || y < 0 || y >= pRows ){
		DETHROW( deeInvalidParam );
	}
	return ( int )pValues[ y * pColons + x ];
}

void meUShortArray::SetValueAt( int x, int y, int value ){
	if( x < 0 || x >= pColons || y < 0 || y >= pRows ){
		DETHROW( deeInvalidParam );
	}
	pValues[ y * pColons + x ] = ( unsigned short )value;
}

void meUShortArray::SetAll( int value ){
	const unsigned short rvalue = ( unsigned short )value;
	const int count = pColons * pRows;
	int i;
	
	for( i=0; i<count; i++ ){
		pValues[ i ] = rvalue;
	}
}



// Operators
//////////////

bool meUShortArray::operator==( const meUShortArray &array ) const{
	return pColons == array.pColons && pRows == array.pRows
		&& memcmp( pValues, array.pValues, pColons * pRows ) == 0;
}

meUShortArray &meUShortArray::operator=( const meUShortArray &array ){
	if( array.pColons != pColons || array.pRows != pRows ){
		DETHROW( deeInvalidParam );
	}
	memcpy( pValues, array.pValues, pColons * pRows );
	return *this;
}
