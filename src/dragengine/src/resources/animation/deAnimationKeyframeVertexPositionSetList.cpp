/* 
 * Drag[en]gine Game Engine
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

#include "deAnimation.h"
#include "deAnimationKeyframeVertexPositionSet.h"
#include "deAnimationKeyframeVertexPositionSetList.h"
#include "../../common/exceptions.h"



// class deAnimationKeyframeVertexPositionSetList
///////////////////////////////////////////////////

// constructor, destructor
////////////////////////////

deAnimationKeyframeVertexPositionSetList::deAnimationKeyframeVertexPositionSetList() :
pKeyframes( nullptr ),
pKeyframeCount( 0 ),
pKeyframeSize( 0 ){
}

deAnimationKeyframeVertexPositionSetList::~deAnimationKeyframeVertexPositionSetList(){
	int i;
	if( pKeyframes ){
		for( i=0; i<pKeyframeCount; i++ ){
			if( pKeyframes[ i ] ){
				delete pKeyframes[ i ];
			}
		}
		delete [] pKeyframes;
	}
}



// Management
///////////////

deAnimationKeyframeVertexPositionSet *deAnimationKeyframeVertexPositionSetList::GetKeyframe( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pKeyframeCount );
	
	return pKeyframes[ index ];
}

void deAnimationKeyframeVertexPositionSetList::AddKeyframe( deAnimationKeyframeVertexPositionSet *keyframe ){
	DEASSERT_NOTNULL( keyframe )
	
	if( pKeyframeCount == pKeyframeSize ){
		const int newSize = pKeyframeCount * 3 / 2 + 1;
		int i;
		deAnimationKeyframeVertexPositionSet ** const newArray = new deAnimationKeyframeVertexPositionSet*[ newSize ];
		if( pKeyframes ){
			for( i=0; i<pKeyframeCount; i++ ){
				newArray[ i ] = pKeyframes[ i ];
			}
			delete [] pKeyframes;
		}
		pKeyframes = newArray;
		pKeyframeSize = newSize;
	}
	
	pKeyframes[ pKeyframeCount ] = keyframe;
	pKeyframeCount++;
}

int deAnimationKeyframeVertexPositionSetList::GetLeadingKeyframe( int startkf, float time ) const{
	int leadkf = 0;
	if( startkf > 0 && time >= pKeyframes[ startkf ]->GetTime() ){
		leadkf = startkf;
	}
	while( leadkf < pKeyframeCount && pKeyframes[ leadkf ]->GetTime() < time ){
		leadkf++;
	}
	return leadkf > 0 ? leadkf - 1 : 0;
}
