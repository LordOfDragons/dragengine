/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "dearAnimationKeyframeVPSList.h"
#include "dearAnimationKeyframeVPS.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationKeyframeVertexPositionSetList.h>



// Class dearAnimationKeyframeVPSList
///////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationKeyframeVPSList::dearAnimationKeyframeVPSList(
	const deAnimationKeyframeVertexPositionSetList &list ) :
pKeyframes( nullptr ),
pKeyframeCount( 0 )
{
	try{
		pCreateKeyframes( list );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dearAnimationKeyframeVPSList::~dearAnimationKeyframeVPSList(){
	pCleanUp();
}



// Management
///////////////

dearAnimationKeyframeVPS &dearAnimationKeyframeVPSList::GetAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pKeyframeCount )
	
	return pKeyframes[ index ];
}

dearAnimationKeyframeVPS *dearAnimationKeyframeVPSList::GetWithTime( float time ) const{
	if( pKeyframeCount == 0 ){
		return nullptr;
	}
	
	if( time <= pKeyframes[ 0 ].GetTime() ){
		return pKeyframes;
	}
	
	int i;
	for( i=1; i<pKeyframeCount; i++ ){
		if( time < pKeyframes[ i ].GetTime() ){
			return pKeyframes + ( i - 1 );
		}
	}
	
	return pKeyframes + ( pKeyframeCount - 1 );
}



// Private functions
//////////////////////

void dearAnimationKeyframeVPSList::pCleanUp(){
	if( pKeyframes ){
		delete [] pKeyframes;
	}
}

void dearAnimationKeyframeVPSList::pCreateKeyframes(
const deAnimationKeyframeVertexPositionSetList &list ){
	const int count = list.GetKeyframeCount();
	if( count == 0 ){
		return;
	}
	
	pKeyframes = new dearAnimationKeyframeVPS[ count ];
	
	while( pKeyframeCount < count ){
		if( pKeyframeCount < count - 1 ){
			pKeyframes[ pKeyframeCount ].Set( *list.GetKeyframe( pKeyframeCount ),
				*list.GetKeyframe( pKeyframeCount + 1 ) );
			
		}else{
			pKeyframes[ pKeyframeCount ].Set( *list.GetKeyframe( pKeyframeCount ) );
		}
		
		pKeyframeCount++;
	}
}
