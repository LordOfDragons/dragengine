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
	inline float GetTime() const{return pTime;}
	/** Retrieves the time step. */
	inline float GetTimeStep() const{return pTimeStep;}
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{return pPosition;}
	/** Retrieves the position step. */
	inline const decVector &GetPositionStep() const{return pPositionStep;}
	/** Retrieves the quaternion rotation. */
	inline const decQuaternion &GetRotation() const{return pRotation;}
	/** Retrieves the quaternion rotation step. */
	inline const decQuaternion &GetRotationStep() const{return pRotationStep;}
	/** Retrieves the scaling. */
	inline const decVector &GetScaling() const{return pScaling;}
	/** Retrieves the scaling step. */
	inline const decVector &GetScalingStep() const{return pScalingStep;}
	
	/**
	 * Retrieve interpolated position.
	 * \details Time has to be interpolationTime - keyframeTime. No range checking is done.
	 */
	decVector InterpolatePosition(float time) const;
	/**
	 * Retrieve interpolated rotation.
	 * \details Time has to be interpolationTime - keyframeTime. No range checking is done.
	 */
	decQuaternion InterpolateRotation(float time) const;
	/**
	 * Retrieve interpolated scaling.
	 * \details Time has to be interpolationTime - keyframeTime. No range checking is done.
	 */
	decVector InterpolateScaling(float time) const;
	
	/** Set keyframe without interpolation data. */
	void Set(const deAnimationKeyframe &keyframe, bool &negate);
	/** Set keyframe with interpolation data. */
	void Set(const deAnimationKeyframe &keyframe, const deAnimationKeyframe &nextKeyframe, bool &negate);
	/*@}*/
};

#endif
