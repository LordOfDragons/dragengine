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

#include "deoglRBillboard.h"
#include "deoglBillboardList.h"

#include <dragengine/common/exceptions.h>



// Class deoglBillboardList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglBillboardList::deoglBillboardList(){
	pBillboards = NULL;
	pBillboardCount = 0;
	pBillboardSize = 0;
}

deoglBillboardList::~deoglBillboardList(){
	if( pBillboards ){
		delete [] pBillboards;
	}
}



// Management
///////////////

deoglRBillboard *deoglBillboardList::GetAt( int index ) const{
	if( index < 0 || index >= pBillboardCount ) DETHROW( deeInvalidParam );
	
	return pBillboards[ index ];
}

int deoglBillboardList::IndexOf( deoglRBillboard *billboard ) const{
	if( ! billboard ) DETHROW( deeInvalidParam );
	
	int l;
	
	for( l=0; l<pBillboardCount; l++ ){
		if( billboard == pBillboards[ l ] ){
			return l;
		}
	}
	
	return -1;
}

bool deoglBillboardList::Has( deoglRBillboard *billboard ) const{
	if( ! billboard ) DETHROW( deeInvalidParam );
	
	int l;
	
	for( l=0; l<pBillboardCount; l++ ){
		if( billboard == pBillboards[ l ] ){
			return true;
		}
	}
	
	return false;
}

void deoglBillboardList::Add( deoglRBillboard *billboard ){
	if( Has( billboard ) ) DETHROW( deeInvalidParam );
	
	pAddBillboard( billboard );
}

bool deoglBillboardList::AddIfMissing( deoglRBillboard *billboard ){
	if( Has( billboard ) ) return false;
	
	pAddBillboard( billboard );
	return true;
}

void deoglBillboardList::Remove( deoglRBillboard *billboard ){
	int index = IndexOf( billboard );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	if( index < pBillboardCount - 1 ){
		pBillboards[ index ] = pBillboards[ pBillboardCount - 1 ];
	}
	pBillboardCount--;
}

bool deoglBillboardList::RemoveIfExisting( deoglRBillboard *billboard ){
	int index = IndexOf( billboard );
	
	if( index == -1 ) return false;
	
	if( index < pBillboardCount - 1 ){
		pBillboards[ index ] = pBillboards[ pBillboardCount - 1 ];
	}
	pBillboardCount--;
	return true;
}

void deoglBillboardList::RemoveFrom( int index ){
	if( index < 0 || index >= pBillboardCount ) DETHROW( deeInvalidParam );
	
	if( index < pBillboardCount - 1 ){
		pBillboards[ index ] = pBillboards[ pBillboardCount - 1 ];
	}
	pBillboardCount--;
}

void deoglBillboardList::RemoveAll(){
	pBillboardCount = 0;
}



// Private Functions
//////////////////////

void deoglBillboardList::pAddBillboard( deoglRBillboard *billboard ){
	if( pBillboardCount == pBillboardSize ){
		int newSize = pBillboardCount + 10; // * 3 / 2 + 1;
		deoglRBillboard **newArray = new deoglRBillboard*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		if( pBillboards ){
			memcpy( newArray, pBillboards, sizeof( deoglRBillboard* ) * pBillboardSize );
			delete [] pBillboards;
		}
		
		pBillboards = newArray;
		pBillboardSize = newSize;
	}
	
	pBillboards[ pBillboardCount++ ] = billboard;
}
