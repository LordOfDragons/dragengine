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

#include "deoalSoundRay.h"
#include "deoalSoundRaySegment.h"
#include "deoalSoundRayList.h"

#include <dragengine/common/exceptions.h>



// Class deoalSoundRayList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalSoundRayList::deoalSoundRayList() :
pRays( NULL ),
pRayCount( 0 ),
pRaySize( 0 ),

pSegments( NULL ),
pSegmentCount( 0 ),
pSegmentSize( 0 ),

pTransmittedRays( NULL ),
pTransmittedRayCount( 0 ),
pTransmittedRaySize( 0 ){
}

deoalSoundRayList::~deoalSoundRayList(){
	if( pTransmittedRays ){
		delete [] pTransmittedRays;
	}
	if( pSegments ){
		delete [] pSegments;
	}
	if( pRays ){
		delete [] pRays;
	}
}



// Manegement
///////////////

deoalSoundRay &deoalSoundRayList::GetRayAt( int index ){
	if( index < 0 || index >= pRayCount ){
		DETHROW( deeInvalidParam );
	}
	return pRays[ index ];
}

const deoalSoundRay &deoalSoundRayList::GetRayAt( int index ) const{
	if( index < 0 || index >= pRayCount ){
		DETHROW( deeInvalidParam );
	}
	return pRays[ index ];
}

int deoalSoundRayList::AddRay(){
	if( pRayCount == pRaySize ){
		const int newSize = pRaySize + 10;
		deoalSoundRay * const newArray = new deoalSoundRay[ newSize ];
		if( pRays ){
			memcpy( newArray, pRays, sizeof( deoalSoundRay ) * pRaySize );
			delete [] pRays;
		}
		pRays = newArray;
		pRaySize = newSize;
	}
	
	deoalSoundRay &ray = pRays[ pRayCount ];
	ray.SetFirstSegment( pSegmentCount );
	ray.SetSegmentCount( 0 );
	ray.SetFirstTransmittedRay( pTransmittedRayCount );
	ray.SetTransmittedRayCount( 0 );
	ray.SetOutside( false );
	return pRayCount++;
}

void deoalSoundRayList::RemoveAllRays(){
	pRayCount = 0;
	pSegmentCount = 0;
	pTransmittedRayCount = 0;
}

deoalSoundRaySegment &deoalSoundRayList::GetSegmentAt( int index ){
	if( index < 0 || index >= pSegmentCount ){
		DETHROW( deeInvalidParam );
	}
	return pSegments[ index ];
}

const deoalSoundRaySegment &deoalSoundRayList::GetSegmentAt( int index ) const{
	if( index < 0 || index >= pSegmentCount ){
		DETHROW( deeInvalidParam );
	}
	return pSegments[ index ];
}

int deoalSoundRayList::AddSegment(){
	if( pSegmentCount == pSegmentSize ){
		const int newSize = pSegmentSize + 10;
		deoalSoundRaySegment * const newArray = new deoalSoundRaySegment[ newSize ];
		if( pSegments ){
			memcpy( newArray, pSegments, sizeof( deoalSoundRaySegment ) * pSegmentSize );
			delete [] pSegments;
		}
		pSegments = newArray;
		pSegmentSize = newSize;
	}
	
	return pSegmentCount++;
}

deoalSoundRay &deoalSoundRayList::GetTransmittedRayAt( int index ){
	if( index < 0 || index >= pTransmittedRayCount ){
		DETHROW( deeInvalidParam );
	}
	return pTransmittedRays[ index ];
}

const deoalSoundRay &deoalSoundRayList::GetTransmittedRayAt( int index ) const{
	if( index < 0 || index >= pTransmittedRayCount ){
		DETHROW( deeInvalidParam );
	}
	return pTransmittedRays[ index ];
}

int deoalSoundRayList::AddTransmittedRay(){
	if( pTransmittedRayCount == pTransmittedRaySize ){
		const int newSize = pTransmittedRaySize + 10;
		deoalSoundRay * const newArray = new deoalSoundRay[ newSize ];
		if( pTransmittedRays ){
			memcpy( newArray, pTransmittedRays, sizeof( deoalSoundRay ) * pTransmittedRaySize );
			delete [] pTransmittedRays;
		}
		pTransmittedRays = newArray;
		pTransmittedRaySize = newSize;
	}
	
	deoalSoundRay &ray = pTransmittedRays[ pTransmittedRayCount ];
	ray.SetFirstSegment( pSegmentCount );
	ray.SetSegmentCount( 0 );
	ray.SetFirstTransmittedRay( pTransmittedRayCount );
	ray.SetTransmittedRayCount( 0 );
	ray.SetOutside( false );
	return pTransmittedRayCount++;
}

void deoalSoundRayList::ReserveSize( int rays, int segments, int transmittedRays ){
	if( rays < 0 || segments < 0 || transmittedRays < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( rays > pRaySize ){
		deoalSoundRay * const newArray = new deoalSoundRay[ rays ];
		if( pRays ){
			memcpy( newArray, pRays, sizeof( deoalSoundRay ) * pRaySize );
			delete [] pRays;
		}
		pRays = newArray;
		pRaySize = rays;
	}
	
	if( segments > pSegmentSize ){
		deoalSoundRaySegment * const newArray = new deoalSoundRaySegment[ segments ];
		if( pSegments ){
			memcpy( newArray, pSegments, sizeof( deoalSoundRaySegment ) * pSegmentSize );
			delete [] pSegments;
		}
		pSegments = newArray;
		pSegmentSize = segments;
	}
	
	if( transmittedRays > pTransmittedRaySize ){
		deoalSoundRay * const newArray = new deoalSoundRay[ transmittedRays ];
		if( pTransmittedRays ){
			memcpy( newArray, pTransmittedRays, sizeof( deoalSoundRay ) * pTransmittedRaySize );
			delete [] pTransmittedRays;
		}
		pTransmittedRays = newArray;
		pTransmittedRaySize = transmittedRays;
	}
}



// Operators
//////////////

deoalSoundRayList &deoalSoundRayList::operator=(
const deoalSoundRayList &list ){
	pRayCount = 0;
	pSegmentCount = 0;
	pTransmittedRayCount = 0;
	return operator+=( list );
}

deoalSoundRayList &deoalSoundRayList::operator+=(
const deoalSoundRayList &list ){
	ReserveSize( pRayCount + list.pRayCount, pSegmentCount + list.pSegmentCount,
		pTransmittedRayCount + list.pTransmittedRayCount );
	const int segmentOffset = pSegmentCount;
	const int rayOffset = pRayCount;
	const int transmittedRayOffset = pTransmittedRayCount;
	int i;
	
	for( i=0; i<list.pTransmittedRayCount; i++ ){
		const deoalSoundRay &raySrc = list.pTransmittedRays[ i ];
		deoalSoundRay &ray = pTransmittedRays[ transmittedRayOffset + i ];
		ray.SetFirstSegment( raySrc.GetFirstSegment() + segmentOffset );
		ray.SetSegmentCount( raySrc.GetSegmentCount() );
		ray.SetFirstTransmittedRay( raySrc.GetFirstTransmittedRay() + transmittedRayOffset );
		ray.SetTransmittedRayCount( raySrc.GetTransmittedRayCount() );
		ray.SetOutside( raySrc.GetOutside() );
	}
	pTransmittedRayCount += list.pTransmittedRayCount;
	
	for( i=0; i<list.pSegmentCount; i++ ){
		pSegments[ segmentOffset + i ] = list.pSegments[ i ];
	}
	pSegmentCount += list.pSegmentCount;
	
	for( i=0; i<list.pRayCount; i++ ){
		const deoalSoundRay &raySrc = list.pRays[ i ];
		deoalSoundRay &ray = pRays[ rayOffset + i ];
		ray.SetFirstSegment( raySrc.GetFirstSegment() + segmentOffset );
		ray.SetSegmentCount( raySrc.GetSegmentCount() );
		ray.SetFirstTransmittedRay( raySrc.GetFirstTransmittedRay() + transmittedRayOffset );
		ray.SetTransmittedRayCount( raySrc.GetTransmittedRayCount() );
		ray.SetOutside( raySrc.GetOutside() );
	}
	pRayCount += list.pRayCount;
	
	return *this;
}
