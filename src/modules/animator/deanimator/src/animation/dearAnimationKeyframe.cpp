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

#include "dearAnimationKeyframe.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>



// Class dearAnimationKeyframe
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationKeyframe::dearAnimationKeyframe() :
pTime( 0.0f ),
pTimeStep( 1.0f ),
pScaling( 1.0f, 1.0f, 1.0f ){
}

dearAnimationKeyframe::~dearAnimationKeyframe(){
}



// Management
///////////////

decVector dearAnimationKeyframe::InterpolatePosition( float time ) const{
	return pPosition + pPositionStep * time;
}

decQuaternion dearAnimationKeyframe::InterpolateRotation( float time ) const{
	return pRotation + pRotationStep * time;
}

decVector dearAnimationKeyframe::InterpolateScaling( float time ) const{
	return pScaling + pScalingStep * time;
}



void dearAnimationKeyframe::Set( const deAnimationKeyframe &keyframe, bool &negate ){
	pTime = keyframe.GetTime();
	pTimeStep = 1.0f;
	pPosition = keyframe.GetPosition();
	pPositionStep.SetZero();
	pRotation.SetFromEuler( keyframe.GetRotation() );
	if( negate ){
		pRotation = -pRotation;
	}
	pRotationStep.Set( 0.0f, 0.0f, 0.0f, 0.0f );
	pScaling = keyframe.GetScale();
	pScalingStep.SetZero();
}

void dearAnimationKeyframe::Set( const deAnimationKeyframe &keyframe, const deAnimationKeyframe &nextKeyframe, bool &negate ){
	if( nextKeyframe.GetTime() <= keyframe.GetTime() ){
		Set( keyframe, negate );
		return;
	}
	
	pTime = keyframe.GetTime();
	pTimeStep = 1.0f / ( nextKeyframe.GetTime() - keyframe.GetTime() );
	pPosition = keyframe.GetPosition();
	pPositionStep = ( nextKeyframe.GetPosition() - keyframe.GetPosition() ) * pTimeStep;
	pScaling = keyframe.GetScale();
	pScalingStep = ( nextKeyframe.GetScale() - keyframe.GetScale() ) * pTimeStep;
	
	// protect against uncontrolled flipping due to converting euler angles to quaternions. if the dot
	// product with the previous quaterion is negative the conversion would cause the quaternion to
	// animated the long way instead of the short way. negate the quaternion to fix this. the negation
	// propagates forward until a quaternion is not required to be negated anymore
	pRotation.SetFromEuler( keyframe.GetRotation() );
	if( negate ){
		pRotation = -pRotation;
	}
	
	decQuaternion nextRotation( decQuaternion::CreateFromEuler( nextKeyframe.GetRotation() ) );
	negate = ( pRotation.Dot( nextRotation ) < 0.0f );
	if( negate ){
		nextRotation = -nextRotation;
	}
	
	pRotationStep = ( nextRotation - pRotation ) * pTimeStep;
}
