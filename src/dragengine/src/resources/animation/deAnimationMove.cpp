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


#include "deAnimation.h"
#include "deAnimationMove.h"
#include "deAnimationKeyframeList.h"
#include "deAnimationKeyframeVertexPositionSetList.h"
#include "../../common/exceptions.h"


// class deAnimationMove
//////////////////////////

// constructor, destructor
////////////////////////////

deAnimationMove::deAnimationMove() :
pPlaytime( 0 ),
pFPS( 25.0f ),
pLists( nullptr ),
pListCount( 0 ),
pListSize( 0 ),
pVertexPositionSetLists( nullptr ),
pVertexPositionSetListCount( 0 ),
pVertexPositionSetListSize( 0 ){
}

deAnimationMove::~deAnimationMove(){
	int i;
	
	if( pLists ){
		for( i=0; i<pListCount; i++ ){
			if( pLists[ i ] ){
				delete pLists[ i ];
			}
		}
		delete [] pLists;
	}
	
	if( pVertexPositionSetLists ){
		for( i=0; i<pVertexPositionSetListCount; i++ ){
			if( pVertexPositionSetLists[ i ] ){
				delete pVertexPositionSetLists[ i ];
			}
		}
		delete [] pVertexPositionSetLists;
	}
}



// management
///////////////

void deAnimationMove::SetName( const char *name ){
	pName = name;
}

void deAnimationMove::SetPlaytime( float playtime ){
	pPlaytime = decMath::max( playtime, 0.0f );
}

void deAnimationMove::SetFPS( float fps ){
	pFPS = decMath::max( fps, 1.0f );
}

deAnimationKeyframeList *deAnimationMove::GetKeyframeList( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pListCount )
	
	return pLists[ index ];
}

void deAnimationMove::AddKeyframeList( deAnimationKeyframeList *list ){
	DEASSERT_NOTNULL( list );
	
	if( pListCount == pListSize ){
		const int newSize = pListCount * 3 / 2 + 1;
		int i;
		deAnimationKeyframeList ** const newArray = new deAnimationKeyframeList*[ newSize ];
		if( pLists ){
			for( i=0; i<pListCount; i++ ){
				newArray[ i ] = pLists[ i ];
			}
			delete [] pLists;
		}
		pLists = newArray;
		pListSize = newSize;
	}
	
	pLists[ pListCount ] = list;
	pListCount++;
}

deAnimationKeyframeVertexPositionSetList *deAnimationMove::GetVertexPositionSetKeyframeList( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pVertexPositionSetListCount )
	
	return pVertexPositionSetLists[ index ];
}

void deAnimationMove::AddVertexPositionSetKeyframeList( deAnimationKeyframeVertexPositionSetList *list ){
	DEASSERT_NOTNULL( list );
	
	if( pVertexPositionSetListCount == pVertexPositionSetListSize ){
		const int newSize = pVertexPositionSetListCount * 3 / 2 + 1;
		int i;
		deAnimationKeyframeVertexPositionSetList ** const newArray =
			new deAnimationKeyframeVertexPositionSetList*[ newSize ];
		if( pVertexPositionSetLists ){
			for( i=0; i<pVertexPositionSetListCount; i++ ){
				newArray[ i ] = pVertexPositionSetLists[ i ];
			}
			delete [] pVertexPositionSetLists;
		}
		pVertexPositionSetLists = newArray;
		pVertexPositionSetListSize = newSize;
	}
	
	pVertexPositionSetLists[ pVertexPositionSetListCount ] = list;
	pVertexPositionSetListCount++;
}
