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

#include "deoalRayCacheRay.h"
#include "deoalRayCacheRayHit.h"
#include "../../environment/raytrace/deoalRayTraceResult.h"
#include "../../environment/raytrace/deoalRayTraceHitElement.h"
#include "../../environment/raytrace/deoalRayTraceHitElementList.h"
#include "../../component/deoalAComponent.h"
#include "../../model/deoalAModel.h"
#include "../../model/deoalModelFace.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayCacheRay
////////////////////////

// Constructor, destructor
////////////////////////////

deoalRayCacheRay::deoalRayCacheRay( const decVector &origin, const decVector &direction,
	float length, const deoalRayTraceHitElementList &elements ) :
pOrigin( origin ),
pDirection( direction ),
pLength( length ),
pHits( NULL ),
pHitCount( 0 ),
pOctreeNode( NULL ),
pLLPrev( NULL ),
pLLNext( NULL )
{
	const int elementCount = elements.GetCount();
	if( elementCount == 0 ){
		return;
	}
	
	pHits = new deoalRayCacheRayHit[ elementCount ];
	
	for( pHitCount=0; pHitCount<elementCount; pHitCount++ ){
		const deoalRayTraceHitElement &element = elements.GetAt( pHitCount );
		deoalRayCacheRayHit &hit = pHits[ pHitCount ];
		
		hit.SetDistance( element.GetDistance() );
		hit.SetFaceIndex( element.GetComponentFace() );
		hit.SetForwardFacing( element.GetForwardFacing() );
		hit.SetPoint( element.GetPoint() );
		hit.SetNormal( element.GetNormal() );
	}
}

deoalRayCacheRay::deoalRayCacheRay( const decVector &origin, const decVector &direction,
float length, const deoalRayTraceResult &result ) :
pOrigin( origin ),
pDirection( direction ),
pLength( length ),
pHits( NULL ),
pHitCount( 0 ),
pOctreeNode( NULL ),
pLLPrev( NULL ),
pLLNext( NULL )
{
	const int count = result.GetElementCount();
	if( count == 0 ){
		return;
	}
	
	pHits = new deoalRayCacheRayHit[ count ];
	
	for( pHitCount=0; pHitCount<count; pHitCount++ ){
		const deoalRayTraceHitElement &element = result.GetElementAt( pHitCount );
		deoalRayCacheRayHit &hit = pHits[ pHitCount ];
		
		hit.SetDistance( element.GetDistance() );
		hit.SetFaceIndex( element.GetComponentFace() );
		hit.SetForwardFacing( element.GetForwardFacing() );
		hit.SetPoint( element.GetPoint() );
		hit.SetNormal( element.GetNormal() );
	}
}

deoalRayCacheRay::deoalRayCacheRay( const decVector &origin, const decVector &direction,
	float length, const deoalRayTraceResult &result, int elementCount ) :
pOrigin( origin ),
pDirection( direction ),
pLength( length ),
pHits( NULL ),
pHitCount( 0 ),
pOctreeNode( NULL ),
pLLPrev( NULL ),
pLLNext( NULL )
{
	if( elementCount == 0 ){
		return;
	}
	
	pHits = new deoalRayCacheRayHit[ elementCount ];
	
	for( pHitCount=0; pHitCount<elementCount; pHitCount++ ){
		const deoalRayTraceHitElement &element = result.GetElementAt( pHitCount );
		deoalRayCacheRayHit &hit = pHits[ pHitCount ];
		
		hit.SetDistance( element.GetDistance() );
		hit.SetFaceIndex( element.GetComponentFace() );
		hit.SetForwardFacing( element.GetForwardFacing() );
		hit.SetPoint( element.GetPoint() );
		hit.SetNormal( element.GetNormal() );
	}
}

deoalRayCacheRay::~deoalRayCacheRay(){
	if( pHits ){
		delete [] pHits;
	}
}



// Management
///////////////

const deoalRayCacheRayHit &deoalRayCacheRay::GetHitAt( int index ) const{
	if( index < 0 || index >= pHitCount ){
		DETHROW( deeInvalidParam );
	}
	return pHits[ index ];
}



void deoalRayCacheRay::SetOctreeNode( deoalRayCacheOctree *node ){
	pOctreeNode = node;
}

void deoalRayCacheRay::SetLLPrev( deoalRayCacheRay *ray ){
	pLLPrev = ray;
}

void deoalRayCacheRay::SetLLNext( deoalRayCacheRay *ray) {
	pLLNext = ray;
}
