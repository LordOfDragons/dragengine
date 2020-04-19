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
#include "deAnimationKeyframe.h"
#include "deAnimationKeyframeList.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/exceptions.h"



// class deAnimationKeyframeList
//////////////////////////////////

// constructor, destructor
////////////////////////////

deAnimationKeyframeList::deAnimationKeyframeList(){
	pKeyframes = NULL;
	pKeyframeCount = 0;
	pKeyframeSize = 0;
}

deAnimationKeyframeList::~deAnimationKeyframeList(){
	int i;
	if( pKeyframes ){
		for( i=0; i<pKeyframeCount; i++ ){
			if( pKeyframes[ i ] ) delete pKeyframes[ i ];
		}
		delete [] pKeyframes;
	}
}



// management
///////////////

deAnimationKeyframe *deAnimationKeyframeList::GetKeyframe( int index ) const{
	if( index < 0 || index >= pKeyframeCount ) DETHROW( deeOutOfBoundary );
	return pKeyframes[ index ];
}

void deAnimationKeyframeList::AddKeyframe( deAnimationKeyframe *keyframe ){
	if( ! keyframe ) DETHROW( deeInvalidParam );
	if( pKeyframeCount == pKeyframeSize ){
		int i, newSize = pKeyframeCount * 3 / 2 + 1;
		deAnimationKeyframe **newArray = new deAnimationKeyframe*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pKeyframes ){
			for( i=0; i<pKeyframeCount; i++ ) newArray[ i ] = pKeyframes[ i ];
			delete [] pKeyframes;
		}
		pKeyframes = newArray;
		pKeyframeSize = newSize;
	}
	pKeyframes[ pKeyframeCount ] = keyframe;
	pKeyframeCount++;
}

int deAnimationKeyframeList::GetLeadingKeyframe( int startkf, float time ) const{
	int leadkf = 0;
	if( startkf > 0 && time >= pKeyframes[ startkf ]->GetTime() ){
		leadkf = startkf;
	}
	while( leadkf < pKeyframeCount && pKeyframes[ leadkf ]->GetTime() < time ) leadkf++;
	return leadkf > 0 ? leadkf - 1 : 0;
}
