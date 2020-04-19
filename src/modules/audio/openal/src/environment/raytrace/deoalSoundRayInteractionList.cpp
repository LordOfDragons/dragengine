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

#include "deoalSoundRayInteraction.h"
#include "deoalSoundRayInteractionList.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRayInteractionList
///////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalSoundRayInteractionList::deoalSoundRayInteractionList() :
pRays( NULL ),
pCount( 0 ),
pSize( 0 ){
}

deoalSoundRayInteractionList::~deoalSoundRayInteractionList(){
	if( pRays ){
		delete [] pRays;
	}
}



// Manegement
///////////////

const deoalSoundRayInteraction &deoalSoundRayInteractionList::GetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pRays[ index ];
}

deoalSoundRayInteraction &deoalSoundRayInteractionList::Add(){
	if( pCount == pSize ){
		const int newSize = pSize + 10;
		deoalSoundRayInteraction * const newArray = new deoalSoundRayInteraction[ newSize ];
		if( pRays ){
			memcpy( newArray, pRays, sizeof( deoalSoundRayInteraction ) * pSize );
			delete [] pRays;
		}
		pRays = newArray;
		pSize = newSize;
	}
	
	return pRays[ pCount++ ];
}

void deoalSoundRayInteractionList::RemoveAll(){
	pCount = 0;
}

void deoalSoundRayInteractionList::ReserveSize( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( size <= pSize ){
		return;
	}
	
	deoalSoundRayInteraction * const newArray = new deoalSoundRayInteraction[ size ];
	if( pRays ){
		memcpy( newArray, pRays, sizeof( deoalSoundRayInteraction ) * pSize );
		delete [] pRays;
	}
	pRays = newArray;
	pSize = size;
}



// Operators
//////////////

deoalSoundRayInteractionList &deoalSoundRayInteractionList::operator=(
const deoalSoundRayInteractionList &list ){
	RemoveAll();
	return operator+=( list );
}

deoalSoundRayInteractionList &deoalSoundRayInteractionList::operator+=(
const deoalSoundRayInteractionList &list ){
	ReserveSize( pCount + list.pCount );
	
	int i;
	for( i=0; i<list.pCount; i++ ){
		Add() = list.pRays[ i ];
	}
	return *this;
}
