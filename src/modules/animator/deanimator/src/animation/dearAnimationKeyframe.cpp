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
