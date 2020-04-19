/* 
 * Drag[en]gine Android Launcher
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

#include "decUTF8Decoder.h"
#include "../../exceptions.h"



// class decUTF8Decoder
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

decUTF8Decoder::decUTF8Decoder(){
	pString = "";
	pLength = 0;
	pPosition = 0;
}

decUTF8Decoder::~decUTF8Decoder(){
}



// Accessors and Mutators
///////////////////////////

bool decUTF8Decoder::HasReachedEnd() const{
	return pPosition == pLength;
}

void decUTF8Decoder::SetString( const char *string ){
	if( ! string ) DETHROW( deeInvalidParam );
	pString = string;
	pLength = strlen( string );
	pPosition = 0;
}

void decUTF8Decoder::SetPosition( int position ){
	if( position < 0 || position > pLength ) DETHROW( deeInvalidParam );
	pPosition = position;
}

int decUTF8Decoder::DecodeNextCharacter(){
	int followBytes;
	int character;
	
	// check for end of pString
	if( pPosition == pLength ) return -1;
	
	// decode start byte
	if( pString[ pPosition ] & 0x80 ){ // 1xxxxxxx
		
		if( pString[ pPosition ] & 0x40 ){ // 11xxxxxx
			
			if( pString[ pPosition ] & 0x20 ){ // 111xxxxx
				
				if( pString[ pPosition ] & 0x10 ){ // 1111xxxx
					
					if( pString[ pPosition ] & 0x08 ){ // 11111xxx not allowed
						pPosition++;
						return -1;
						
					}else{ // 11110xxx
						character = pString[ pPosition++ ] & 0x07;
						followBytes = 3;
					}
					
				}else{ // 1110xxxx
					character = pString[ pPosition++ ] & 0x0f;
					followBytes = 2;
				}
				
			}else{ // 110xxxxx
				character = pString[ pPosition++ ] & 0x1f;
				followBytes = 1;
			}
			
		}else{ // 10xxxxxx is not allowed
			pPosition++;
			return -1;
		}
		
	}else{ // 0xxxxxxx
		return pString[ pPosition++ ];
	}
	
	// decode follow bytes
	while( followBytes > 0 ){
		
		if( pPosition == pLength ) return -1;
		
		if( pString[ pPosition ] & 0x80 ){ // 1xxxxxxx
			
			if( pString[ pPosition ] & 0x40 ){ // 11xxxxxx is not allowed
				//pPosition++; <= nah, this is a new start byte so don't skip it!
				return -1;
				
			}else{ // 10xxxxxx
				character = ( character << 6 ) | ( pString[ pPosition++ ] & 0x3f );
				followBytes--;
			}
			
		}else{ // 0xxxxxxx is not allowed
			pPosition++;
			return -1;
		}
		
	}
	
	// return the character
	return character;
}
