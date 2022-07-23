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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deAnimation.h"
#include "deAnimationMove.h"
#include "deAnimationKeyframeList.h"
#include "../../common/file/decBaseFileReader.h"
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
pListSize( 0 ){
}

deAnimationMove::~deAnimationMove(){
	int i;
	if( pLists ){
		for( i=0; i<pListCount; i++ ){
			if( pLists[ i ] ) delete pLists[ i ];
		}
		delete [] pLists;
	}
}



// management
///////////////

void deAnimationMove::SetName( const char *name ){
	pName = name;
}

void deAnimationMove::SetPlaytime( float playtime ){
	if( playtime < 0 ) DETHROW( deeInvalidParam );
	pPlaytime = playtime;
}

void deAnimationMove::SetFPS( float fps ){
	pFPS = decMath::max( fps, 1.0f );
}

deAnimationKeyframeList *deAnimationMove::GetKeyframeList( int index ) const{
	if( index < 0 || index >= pListCount ) DETHROW( deeOutOfBoundary );
	return pLists[ index ];
}

void deAnimationMove::AddKeyframeList( deAnimationKeyframeList *list ){
	if( ! list ) DETHROW( deeInvalidParam );
	if( pListCount == pListSize ){
		int i, newSize = pListCount * 3 / 2 + 1;
		deAnimationKeyframeList **newArray = new deAnimationKeyframeList*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLists ){
			for( i=0; i<pListCount; i++ ) newArray[ i ] = pLists[ i ];
			delete [] pLists;
		}
		pLists = newArray;
		pListSize = newSize;
	}
	pLists[ pListCount ] = list;
	pListCount++;
}
