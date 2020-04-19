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

#include "deoglOcclusionMesh.h"
#include "deoglOcclusionMeshList.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionMeshList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionMeshList::deoglOcclusionMeshList(){
	pOcclusionMeshes = NULL;
	pOcclusionMeshCount = 0;
	pOcclusionMeshSize = 0;
}

deoglOcclusionMeshList::~deoglOcclusionMeshList(){
	if( pOcclusionMeshes ){
		delete [] pOcclusionMeshes;
	}
}



// Management
///////////////

deoglOcclusionMesh *deoglOcclusionMeshList::GetAt( int index ) const{
	if( index < 0 || index >= pOcclusionMeshCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pOcclusionMeshes[ index ];
}

int deoglOcclusionMeshList::IndexOfOcclusionMesh( deoglOcclusionMesh *occlusionMesh ) const{
	if( ! occlusionMesh ){
		DETHROW( deeInvalidParam );
	}
	
	int l;
	
	for( l=0; l<pOcclusionMeshCount; l++ ){
		if( occlusionMesh == pOcclusionMeshes[ l ] ){
			return l;
		}
	}
	
	return -1;
}

bool deoglOcclusionMeshList::Has( deoglOcclusionMesh *occlusionMesh ) const{
	if( ! occlusionMesh ){
		DETHROW( deeInvalidParam );
	}
	
	int l;
	
	for( l=0; l<pOcclusionMeshCount; l++ ){
		if( occlusionMesh == pOcclusionMeshes[ l ] ){
			return true;
		}
	}
	
	return false;
}

void deoglOcclusionMeshList::Add( deoglOcclusionMesh *occlusionMesh ){
	if( Has( occlusionMesh ) ){
		DETHROW( deeInvalidParam );
	}
	
	pAddOcclusionMesh( occlusionMesh );
}

bool deoglOcclusionMeshList::AddIfMissing( deoglOcclusionMesh *occlusionMesh ){
	if( Has( occlusionMesh ) ){
		return false;
	}
	
	pAddOcclusionMesh( occlusionMesh );
	
	return true;
}

void deoglOcclusionMeshList::Remove( deoglOcclusionMesh *occlusionMesh ){
	const int index = IndexOfOcclusionMesh( occlusionMesh );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pOcclusionMeshCount - 1 ){
		pOcclusionMeshes[ index ] = pOcclusionMeshes[ pOcclusionMeshCount - 1 ];
	}
	pOcclusionMeshCount--;
}

bool deoglOcclusionMeshList::RemoveIfExisting( deoglOcclusionMesh *occlusionMesh ){
	const int index = IndexOfOcclusionMesh( occlusionMesh );
	
	if( index == -1 ){
		return false;
	}
	
	if( index < pOcclusionMeshCount - 1 ){
		pOcclusionMeshes[ index ] = pOcclusionMeshes[ pOcclusionMeshCount - 1 ];
	}
	pOcclusionMeshCount--;
	
	return true;
}

void deoglOcclusionMeshList::RemoveFrom( int index ){
	if( index < 0 || index >= pOcclusionMeshCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pOcclusionMeshCount - 1 ){
		pOcclusionMeshes[ index ] = pOcclusionMeshes[ pOcclusionMeshCount - 1 ];
	}
	pOcclusionMeshCount--;
}

void deoglOcclusionMeshList::RemoveAll(){
	pOcclusionMeshCount = 0;
}



// Private Functions
//////////////////////

void deoglOcclusionMeshList::pAddOcclusionMesh( deoglOcclusionMesh *occlusionMesh ){
	if( pOcclusionMeshCount == pOcclusionMeshSize ){
		const int newSize = pOcclusionMeshCount + 10; // * 3 / 2 + 1;
		deoglOcclusionMesh ** const newArray = new deoglOcclusionMesh*[ newSize ];
		
		if( pOcclusionMeshes ){
			memcpy( newArray, pOcclusionMeshes, sizeof( deoglOcclusionMesh* ) * pOcclusionMeshSize );
			delete [] pOcclusionMeshes;
		}
		
		pOcclusionMeshes = newArray;
		pOcclusionMeshSize = newSize;
	}
	
	pOcclusionMeshes[ pOcclusionMeshCount++ ] = occlusionMesh;
}
