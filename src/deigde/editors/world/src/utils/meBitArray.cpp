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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meBitArray.h"

#include "dragengine/deEngine.h"
#include "dragengine/common/exceptions.h"


 
// Class meBitArray
/////////////////////

// Constructor, destructor
////////////////////////////

meBitArray::meBitArray( int colons, int rows ){
	if( colons < 1 || rows < 1 ) DETHROW( deeInvalidParam );
	
	pColons = colons;
	pRows = rows;
	
	pByteCount = ( ( colons * rows - 1 ) >> 3 ) + 1;
	
	pBytes = new unsigned char[ pByteCount ];
	if( ! pBytes ) DETHROW( deeOutOfMemory );
	
	memset( pBytes, 255, pByteCount );
}

meBitArray::~meBitArray(){
	if( pBytes ) delete [] pBytes;
}



// Management
///////////////

bool meBitArray::GetValueAt( int x, int y ) const{
	if( x < 0 || x >= pColons || y < 0 || y >= pRows ) DETHROW( deeInvalidParam );
	
	int bitOffset = pColons * y + x;
	
	return ( pBytes[ bitOffset >> 3 ] & ( 1 << ( bitOffset & 0x7 ) ) ) != 0;
}

void meBitArray::SetValueAt( int x, int y, bool value ){
	if( x < 0 || x >= pColons || y < 0 || y >= pRows ) DETHROW( deeInvalidParam );
	
	int bitOffset = pColons * y + x;
	
	if( value ){
		pBytes[ bitOffset >> 3 ] |= ( unsigned char )( 1 << ( bitOffset & 0x7 ) );
		
	}else{
		pBytes[ bitOffset >> 3 ] &= ~( ( unsigned char )( 1 << ( bitOffset & 0x7 ) ) );
	}
}

void meBitArray::CopyTo( meBitArray &bitArray ) const{
	if( bitArray.pColons != pColons || bitArray.pRows != pRows ) DETHROW( deeInvalidParam );
	
	memcpy( bitArray.pBytes, pBytes, pByteCount );
}

void meBitArray::ClearTo( bool value ){
	if( value ){
		memset( pBytes, 255, pByteCount );
		
	}else{
		memset( pBytes, 0, pByteCount );
	}
}
