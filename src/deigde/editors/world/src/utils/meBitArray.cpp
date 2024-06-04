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
