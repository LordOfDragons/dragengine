/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalRayTraceHitElement.h"
#include "deoalRayTraceHitElementList.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayTraceHitElementList
//////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayTraceHitElementList::deoalRayTraceHitElementList() :
pElements( NULL ),
pCount( 0 ),
pSize( 0 ){
}

deoalRayTraceHitElementList::~deoalRayTraceHitElementList(){
	if( pElements ){
		delete [] pElements;
	}
}



// Manegement
///////////////

const deoalRayTraceHitElement &deoalRayTraceHitElementList::GetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pElements[ index ];
}

void deoalRayTraceHitElementList::Add( const deoalRayTraceHitElement &element ){
	pBareAdd() = element;
}

void deoalRayTraceHitElementList::AddComponentFace( float distance, const decDVector &point,
const decDVector &normal, deoalAComponent *component, int face, bool frontFacing ){
	pBareAdd().SetComponentFace( distance, point, normal, component, face, frontFacing );
}

void deoalRayTraceHitElementList::RemoveAll(){
	pCount = 0;
}



// Private Functions
//////////////////////

deoalRayTraceHitElement &deoalRayTraceHitElementList::pBareAdd(){
	if( pCount == pSize ){
		const int newSize = pSize + 10;
		deoalRayTraceHitElement * const newArray = new deoalRayTraceHitElement[ newSize ];
		if( pElements ){
			memcpy( newArray, pElements, sizeof( deoalRayTraceHitElement ) * pSize );
			delete [] pElements;
		}
		pElements = newArray;
		pSize = newSize;
	}
	
	return pElements[ pCount++ ];
}
