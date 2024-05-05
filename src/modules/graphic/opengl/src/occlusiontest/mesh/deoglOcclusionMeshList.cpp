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
