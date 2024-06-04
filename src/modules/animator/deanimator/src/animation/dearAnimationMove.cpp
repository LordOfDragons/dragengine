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

#include <stdio.h>
#include <string.h>

#include "dearAnimationMove.h"
#include "dearAnimationKeyframeList.h"
#include "dearAnimationKeyframeVPSList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Class dearAnimationMove
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationMove::dearAnimationMove( const deAnimationMove &move ) :
pName( move.GetName() ),
pPlaytime( move.GetPlaytime() ),
pKeyframeLists( nullptr ),
pKeyframeListCount( 0 ),
pKeyframeVPSLists( nullptr ),
pKeyframeVPSListCount( 0 )
{
	try{
		pCreateKeyframeLists( move );
		pCreateKeyframeVPSLists( move );
		
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
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pKeyframeListCount )
	
	return pKeyframeLists[ index ];
}

dearAnimationKeyframeVPSList *dearAnimationMove::GetKeyframeVPSListAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pKeyframeVPSListCount )
	
	return pKeyframeVPSLists[ index ];
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
	
	if( pKeyframeVPSLists ){
		int i;
		for( i=0; i<pKeyframeVPSListCount; i++ ){
			delete pKeyframeVPSLists[ i ];
		}
		delete [] pKeyframeVPSLists;
	}
}



void dearAnimationMove::pCreateKeyframeLists( const deAnimationMove &move ){
	const int count = move.GetKeyframeListCount();
	if( count == 0 ){
		return;
	}
	
	pKeyframeLists = new dearAnimationKeyframeList*[ count ];
	
	while( pKeyframeListCount < count ){
		pKeyframeLists[ pKeyframeListCount ] = new dearAnimationKeyframeList(
			*move.GetKeyframeList( pKeyframeListCount ) );
		pKeyframeListCount++;
	}
}

void dearAnimationMove::pCreateKeyframeVPSLists( const deAnimationMove &move ){
	const int count = move.GetVertexPositionSetKeyframeListCount();
	if( count == 0 ){
		return;
	}
	
	pKeyframeVPSLists = new dearAnimationKeyframeVPSList*[ count ];
	
	while( pKeyframeVPSListCount < count ){
		pKeyframeVPSLists[ pKeyframeVPSListCount ] = new dearAnimationKeyframeVPSList(
			*move.GetVertexPositionSetKeyframeList( pKeyframeVPSListCount ) );
		pKeyframeVPSListCount++;
	}
}
