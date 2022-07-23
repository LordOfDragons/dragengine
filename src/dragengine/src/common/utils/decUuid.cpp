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
#include <stdlib.h>
#include <string.h>

#include "decUuid.h"
#include "decPRNG.h"
#include "../math/decMath.h"
#include "../exceptions.h"



// Class decUuid
//////////////////

// Constructor, destructor
////////////////////////////

decUuid::decUuid(){
	memset( pValues, 0, sizeof( pValues ) );
}

decUuid::decUuid( const uint8_t values[ 16 ] ){
	int i;
	for( i=0; i<16; i++ ){
		pValues[ i ] = values[ i ];
	}
}

decUuid::decUuid( const char *string, bool condensed ){
	SetFromHexString( string, condensed );
}

decUuid::decUuid( const decUuid &id ){
	int i;
	for( i=0; i<16; i++ ){
		pValues[ i ] = id.pValues[ i ];
	}
}

decUuid::~decUuid(){
}

decUuid decUuid::Random(){
	decUuid uuid;
	decPRNG prng;
	int i;
	for( i=0; i<16; i++ ){
		uuid.pValues[ i ] = ( uint8_t )prng.RandomInt( 0, 255 );
	}
	return uuid;
}



// Management
///////////////

uint8_t decUuid::GetValueAt( int index ) const{
	if( index < 0 || index > 15 ){
		DETHROW( deeInvalidParam );
	}
	return pValues[ index ];
}

void decUuid::SetValueAt( int index, uint8_t value ){
	if( index < 0 || index > 15 ){
		DETHROW( deeInvalidParam );
	}
	pValues[ index ] = value;
}

decString decUuid::ToHexString( bool condensed ) const{
	int position = 0;
	int index = 0;
	int shift = 4;
	int i;
	
	decString string;
	string.Set( '0', condensed ? 32 : 36 );
	
	for( i=0; i<32; i++ ){
		if( ! condensed && ( i == 8 || i == 12 || i == 16 || i == 20 ) ){
			string[ position++ ] = '-';
		}
		
		const int value = ( pValues[ index ] >> shift ) & 0xf;
		
		if( value < 10 ){
			string[ position++ ] = ( char )( '0' + value );
			
		}else{
			string[ position++ ] = ( char )( 'a' + ( value - 10 ) );
		}
		
		if( shift == 0 ){
			index++;
			shift = 4;
			
		}else{
			shift = 0;
		}
	}
	
	return string;
}

void decUuid::SetFromHexString( const char *string, bool condensed ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = ( int )strlen( string );
	if( ( condensed && len != 32 ) || ( ! condensed && len != 36 ) ){
		DETHROW( deeInvalidFormat );
	}
	
	int shift = 4;
	int index = 0;
	int i;
	
	memset( pValues, 0, sizeof( pValues ) );
	
	for( i=0; i<len; i++ ){
		if( ! condensed && ( i == 8 || i == 13 || i == 18 || i == 23 ) ){
			if( string[ i ] != '-' ){
				DETHROW( deeInvalidFormat );
			}
			i++;
		}
		
		if( string[ i ] >= '0' && string[ i ] <= '9' ){
			pValues[ index ] |= ( string[ i ] - '0' ) << shift;
			
		}else if( string[ i ] >= 'a' && string[ i ] <= 'z' ){
			pValues[ index ] |= ( string[ i ] - 'a' + 10 ) << shift;
			
		}else if( string[ i ] >= 'A' && string[ i ] <= 'Z' ){
			pValues[ index ] |= ( string[ i ] - 'A' + 10 ) << shift;
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
		if( shift == 0 ){
			index++;
			shift = 4;
			
		}else{
			shift = 0;
		}
	}
}

void decUuid::Clear(){
	memset( pValues, 0, sizeof( pValues ) );
}



// Operators
//////////////

bool decUuid::operator=( const decUuid &id ){
	memcpy( pValues, id.pValues, 16 );
	return *this;
}

bool decUuid::operator==( const decUuid &id ) const{
	int i;
	for( i=0; i<16; i++ ){
		if( id.pValues[ i ] != pValues[ i ] ){
			return false;
		}
	}
	return true;
}

bool decUuid::operator!=( const decUuid &id ) const{
	return ! ( *this == id );
}

bool decUuid::operator!() const{
	return ! operator bool();
}

decUuid::operator bool() const{
	int i;
	for( i=0; i<16; i++ ){
		if( ! pValues[ i ] ){
			return false;
		}
	}
	return true;
}
