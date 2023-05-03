/* 
 * Drag[en]gine Game Engine
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
