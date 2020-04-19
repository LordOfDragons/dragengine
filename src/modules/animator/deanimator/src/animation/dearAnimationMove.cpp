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

#include "dearAnimationMove.h"
#include "dearAnimationKeyframeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Class dearAnimationMove
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationMove::dearAnimationMove( const deAnimationMove &move ){
	pPlaytime = move.GetPlaytime();
	pName = move.GetName();
	
	pKeyframeLists = NULL;
	pKeyframeListCount = 0;
	
	try{
		pCreateKeyframeLists( move );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dearAnimationMove::~dearAnimationMove(){
	pCleanUp();
}



// Management
///////////////

dearAnimationKeyframeList *dearAnimationMove::GetKeyframeListAt( int index ) const{
	if( index < 0 || index >= pKeyframeListCount ){
		DETHROW( deeInvalidParam );
	}
	return pKeyframeLists[ index ];
}



// Private functions
//////////////////////

void dearAnimationMove::pCleanUp(){
	if( pKeyframeLists ){
		int i;
		for( i=0; i<pKeyframeListCount; i++ ){
			delete pKeyframeLists[ i ];
		}
		delete [] pKeyframeLists;
	}
}



void dearAnimationMove::pCreateKeyframeLists( const deAnimationMove &move ){
	const int count = move.GetKeyframeListCount();
	if( count == 0 ){
		return;
	}
	
	pKeyframeLists = new dearAnimationKeyframeList*[ count ];
	
	while( pKeyframeListCount < count ){
		pKeyframeLists[ pKeyframeListCount ] = new dearAnimationKeyframeList( *move.GetKeyframeList( pKeyframeListCount ) );
		pKeyframeListCount++;
	}
}
