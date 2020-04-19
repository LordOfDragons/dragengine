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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglCapsTextureFormat.h"
#include "deoglCapsTextureFormatList.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapsTextureFormatList
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapsTextureFormatList::deoglCapsTextureFormatList() :
pFormats( NULL ),
pFormatCount( 0 ),
pFormatSize( 0 ){
}

deoglCapsTextureFormatList::~deoglCapsTextureFormatList(){
	if( pFormats ){
		while( pFormatCount > 0 ){
			pFormatCount--;
			delete pFormats[ pFormatCount ];
		}
		delete [] pFormats;
	}
}



// Management
///////////////

const deoglCapsTextureFormat *deoglCapsTextureFormatList::GetFormatAt( int index ) const{
	if( index < 0 || index >= pFormatCount ){
		DETHROW( deeInvalidParam );
	}
	return pFormats[ index ];
}

const deoglCapsTextureFormat *deoglCapsTextureFormatList::GetFormatWith( GLint format ) const{
	int i;
	
	for( i=0; i<pFormatCount; i++ ){
		if( pFormats[ i ]->GetFormat() == format ){
			return pFormats[ i ];
		}
	}
	
	return NULL;
}

void deoglCapsTextureFormatList::AddFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
int bitsPerPixel, bool isDepth, bool isDepthFloat, bool isCompressed, const char *name ){
	if( pFormatCount == pFormatSize ){
		const int newSize = pFormatSize * 3 / 2 + 1;
		deoglCapsTextureFormat ** const newArray = new deoglCapsTextureFormat*[ newSize ];
		
		if( pFormats ){
			memcpy( newArray, pFormats, sizeof( deoglCapsTextureFormat* ) * pFormatSize );
			delete [] pFormats;
		}
		
		pFormats = newArray;
		pFormatSize = newSize;
	}
	
	pFormats[ pFormatCount ] = new deoglCapsTextureFormat( format, pixelFormat, pixelType,
		bitsPerPixel, isDepth, isDepthFloat, isCompressed, name );
	pFormatCount++;
}
