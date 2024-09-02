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

#ifndef _DEARANIMATIONKEYFRAMEVPS_H_
#define _DEARANIMATIONKEYFRAMEVPS_H_

#include <dragengine/common/math/decMath.h>

class deAnimationKeyframeVertexPositionSet;


/**
 * Animation move vertex position set keyframe.
 */
class dearAnimationKeyframeVPS{
private:
	float pTime;
	float pTimeStep;
	float pWeight;
	float pWeightStep;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create keyframe. */
	dearAnimationKeyframeVPS();
	
	/** Clean up keyframe. */
	~dearAnimationKeyframeVPS();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Time in seconds. */
	inline float GetTime() const{ return pTime; }
	
	/** Time step. */
	inline float GetTimeStep() const{ return pTimeStep; }
	
	/** Weight. */
	inline float GetWeight() const{ return pWeight; }
	
	/** Weight step. */
	inline float GetWeightStep() const{ return pWeightStep; }
	
	/** Interpolated position. Has to be interpolationTime - keyframeTime. No range checking is done. */
	float InterpolateWeight( float time ) const;
	
	/** Set keyframe without interpolation data. */
	void Set( const deAnimationKeyframeVertexPositionSet &keyframe );
	
	/** Set keyframe with interpolation data. */
	void Set( const deAnimationKeyframeVertexPositionSet &keyframe,
		const deAnimationKeyframeVertexPositionSet &nextKeyframe );
	/*@}*/
};

#endif
