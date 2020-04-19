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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglModelFace.h"
#include "deoglModelFaceList.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelFaceList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelFaceList::deoglModelFaceList(){
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

deoglModelFaceList::~deoglModelFaceList(){
	if( pFaces ){
		delete [] pFaces;
	}
}



// Management
///////////////

deoglModelFace *deoglModelFaceList::GetAt( int index ) const{
	if( index < 0 || index >= pFaceCount ) DETHROW( deeInvalidParam );
	
	return pFaces[ index ];
}

int deoglModelFaceList::IndexOfFace( deoglModelFace *face ) const{
	if( ! face ) DETHROW( deeInvalidParam );
	
	int l;
	
	for( l=0; l<pFaceCount; l++ ){
		if( face == pFaces[ l ] ){
			return l;
		}
	}
	
	return -1;
}

bool deoglModelFaceList::Has( deoglModelFace *face ) const{
	if( ! face ) DETHROW( deeInvalidParam );
	
	int l;
	
	for( l=0; l<pFaceCount; l++ ){
		if( face == pFaces[ l ] ){
			return true;
		}
	}
	
	return false;
}

void deoglModelFaceList::Add( deoglModelFace *face ){
	if( Has( face ) ) DETHROW( deeInvalidParam );
	
	pAddFace( face );
}

bool deoglModelFaceList::AddIfMissing( deoglModelFace *face ){
	if( Has( face ) ) return false;
	
	pAddFace( face );
	return true;
}

void deoglModelFaceList::Remove( deoglModelFace *face ){
	int index = IndexOfFace( face );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	if( index < pFaceCount - 1 ){
		pFaces[ index ] = pFaces[ pFaceCount - 1 ];
	}
	pFaceCount--;
}

bool deoglModelFaceList::RemoveIfExisting( deoglModelFace *face ){
	int index = IndexOfFace( face );
	
	if( index == -1 ) return false;
	
	if( index < pFaceCount - 1 ){
		pFaces[ index ] = pFaces[ pFaceCount - 1 ];
	}
	pFaceCount--;
	return true;
}

void deoglModelFaceList::RemoveFrom( int index ){
	if( index < 0 || index >= pFaceCount ) DETHROW( deeInvalidParam );
	
	if( index < pFaceCount - 1 ){
		pFaces[ index ] = pFaces[ pFaceCount - 1 ];
	}
	pFaceCount--;
}

void deoglModelFaceList::RemoveAll(){
	pFaceCount = 0;
}

void deoglModelFaceList::SortByIndex(){
	deoglModelFace *temp;
	int i;
	
	for( i=1; i<pFaceCount; i++ ){
		if( pFaces[ i ]->GetIndex() < pFaces[ i - 1 ]->GetIndex() ){
			temp = pFaces[ i - 1 ];
			pFaces[ i - 1 ] = pFaces[ i ];
			pFaces[ i ] = temp;
			
			if( i > 1 ){
				i -= 2;
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglModelFaceList::pAddFace( deoglModelFace *face ){
	if( pFaceCount == pFaceSize ){
		int newSize = pFaceCount + 10; // * 3 / 2 + 1;
		deoglModelFace **newArray = new deoglModelFace*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		if( pFaces ){
			memcpy( newArray, pFaces, sizeof( deoglModelFace* ) * pFaceSize );
			delete [] pFaces;
		}
		
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaces[ pFaceCount++ ] = face;
}
