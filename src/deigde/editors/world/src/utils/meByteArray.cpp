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

#include "meByteArray.h"

#include <dragengine/common/exceptions.h>



// Class meByteArray
//////////////////////

// Constructor, destructor
////////////////////////////

meByteArray::meByteArray( int colons, int rows ) :
pColons( colons ),
pRows( rows ),
pValues( NULL )
{
	if( colons < 1 || rows < 1 ) {
		DETHROW( deeInvalidParam );
	}
	
	pValues = new unsigned char[ colons * rows ];
	SetAll( 0 );
}

meByteArray::meByteArray( const meByteArray &array ) :
pColons( array.pColons ),
pRows( array.pRows ),
pValues( NULL )
{
	pValues = new unsigned char[ pColons * pRows ];
	*this = array;
}

meByteArray::~meByteArray(){
	if( pValues ){
		delete [] pValues;
	}
}



// Management
///////////////

int meByteArray::GetValueAt( int x, int y ) const{
	if( x < 0 || x >= pColons || y < 0 || y >= pRows ){
		DETHROW( deeInvalidParam );
	}
	return ( int )pValues[ y * pColons + x ];
}

void meByteArray::SetValueAt( int x, int y, int value ){
	if( x < 0 || x >= pColons || y < 0 || y >= pRows ){
		DETHROW( deeInvalidParam );
	}
	pValues[ y * pColons + x ] = ( unsigned char )value;
}

void meByteArray::SetAll( int value ){
	memset( pValues, value, pColons * pRows );
}



// Operators
//////////////

bool meByteArray::operator==( const meByteArray &array ) const{
	return pColons == array.pColons && pRows == array.pRows
		&& memcmp( pValues, array.pValues, pColons * pRows ) == 0;
}

meByteArray &meByteArray::operator=( const meByteArray &array ){
	if( array.pColons != pColons || array.pRows != pRows ){
		DETHROW( deeInvalidParam );
	}
	memcpy( pValues, array.pValues, pColons * pRows );
	return *this;
}
