/* 
 * Drag[en]gine Animator Module
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
#include <string.h>
#include <unistd.h>

#include "dearAnimationKeyframeList.h"
#include "dearAnimationKeyframe.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationKeyframeList.h>



// Class dearAnimationKeyframeList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationKeyframeList::dearAnimationKeyframeList( const deAnimationKeyframeList &list ){
	pKeyframes = NULL;
	pKeyframeCount = 0;
	
	try{
		pCreateKeyframes( list );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dearAnimationKeyframeList::~dearAnimationKeyframeList(){
	pCleanUp();
}



// Management
///////////////

dearAnimationKeyframe &dearAnimationKeyframeList::GetAt( int index ) const{
	if( index < 0 || index >= pKeyframeCount ){
		DETHROW( deeInvalidParam );
	}
	return pKeyframes[ index ];
}

dearAnimationKeyframe *dearAnimationKeyframeList::GetWithTime( float time ) const{
	if( pKeyframeCount == 0 ){
		return NULL;
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

void dearAnimationKeyframeList::pCleanUp(){
	if( pKeyframes ){
		delete [] pKeyframes;
	}
}



void dearAnimationKeyframeList::pCreateKeyframes( const deAnimationKeyframeList &list ){
	const int count = list.GetKeyframeCount();
	if( count == 0 ){
		return;
	}
	
	pKeyframes = new dearAnimationKeyframe[ count ];
	bool negate = false;
	
	while( pKeyframeCount < count ){
		if( pKeyframeCount < count - 1 ){
			pKeyframes[ pKeyframeCount ].Set( *list.GetKeyframe( pKeyframeCount ),
				*list.GetKeyframe( pKeyframeCount + 1 ), negate );
			
		}else{
			pKeyframes[ pKeyframeCount ].Set( *list.GetKeyframe( pKeyframeCount ), negate );
		}
		
		pKeyframeCount++;
	}
}
