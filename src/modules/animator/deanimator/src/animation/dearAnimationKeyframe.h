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

#ifndef _DEARANIMATIONKEYFRAME_H_
#define _DEARANIMATIONKEYFRAME_H_

#include <dragengine/common/math/decMath.h>

class deAnimationKeyframe;


/**
 * Animation move keyframe.
 * \details Stores information about a keyframe and the change up to the next keyframe.
 *          if existing. The step values are stored as valueDifference/timeDifference.
 *          This way the interpolated value can be quickly retrieved as
 *          value + step * (t - time).
 */
class dearAnimationKeyframe{
private:
	float pTime;
	float pTimeStep;
	decVector pPosition;
	decVector pPositionStep;
	decQuaternion pRotation;
	decQuaternion pRotationStep;
	decVector pScaling;
	decVector pScalingStep;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new animation move keyframe. */
	dearAnimationKeyframe();
	/** Cleans up the animation move keyframe. */
	~dearAnimationKeyframe();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the time in seconds. */
	inline float GetTime() const{ return pTime; }
	/** Retrieves the time step. */
	inline float GetTimeStep() const{ return pTimeStep; }
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Retrieves the position step. */
	inline const decVector &GetPositionStep() const{ return pPositionStep; }
	/** Retrieves the quaternion rotation. */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	/** Retrieves the quaternion rotation step. */
	inline const decQuaternion &GetRotationStep() const{ return pRotationStep; }
	/** Retrieves the scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	/** Retrieves the scaling step. */
	inline const decVector &GetScalingStep() const{ return pScalingStep; }
	
	/**
	 * Retrieve interpolated position.
	 * \details Time has to be interpolationTime - keyframeTime. No range checking is done.
	 */
	decVector InterpolatePosition( float time ) const;
	/**
	 * Retrieve interpolated rotation.
	 * \details Time has to be interpolationTime - keyframeTime. No range checking is done.
	 */
	decQuaternion InterpolateRotation( float time ) const;
	/**
	 * Retrieve interpolated scaling.
	 * \details Time has to be interpolationTime - keyframeTime. No range checking is done.
	 */
	decVector InterpolateScaling( float time ) const;
	
	/** Set keyframe without interpolation data. */
	void Set( const deAnimationKeyframe &keyframe, bool &negate );
	/** Set keyframe with interpolation data. */
	void Set( const deAnimationKeyframe &keyframe, const deAnimationKeyframe &nextKeyframe, bool &negate );
	/*@}*/
};

#endif
