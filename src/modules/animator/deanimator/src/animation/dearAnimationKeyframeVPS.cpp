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

#include <stdio.h>
#include <string.h>

#include "dearAnimationKeyframeVPS.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationKeyframeVertexPositionSet.h>



// Class dearAnimationKeyframeVPS
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationKeyframeVPS::dearAnimationKeyframeVPS() :
pTime( 0.0f ),
pTimeStep( 1.0f ),
pWeight( 0.0f ),
pWeightStep( 1.0f ){
}

dearAnimationKeyframeVPS::~dearAnimationKeyframeVPS(){
}



// Management
///////////////

float dearAnimationKeyframeVPS::InterpolateWeight( float time ) const{
	return pWeight + pWeightStep * time;
}

void dearAnimationKeyframeVPS::Set( const deAnimationKeyframeVertexPositionSet &keyframe ){
	pTime = keyframe.GetTime();
	pTimeStep = 1.0f;
	pWeight = keyframe.GetWeight();
	pWeightStep = 0.0f;
}

void dearAnimationKeyframeVPS::Set( const deAnimationKeyframeVertexPositionSet &keyframe,
const deAnimationKeyframeVertexPositionSet &nextKeyframe ){
	if( nextKeyframe.GetTime() <= keyframe.GetTime() ){
		Set( keyframe );
		return;
	}
	
	pTime = keyframe.GetTime();
	pTimeStep = 1.0f / ( nextKeyframe.GetTime() - keyframe.GetTime() );
	pWeight = keyframe.GetWeight();
	pWeightStep = ( nextKeyframe.GetWeight() - keyframe.GetWeight() ) * pTimeStep;
}
