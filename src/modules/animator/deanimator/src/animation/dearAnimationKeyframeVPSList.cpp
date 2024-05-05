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
