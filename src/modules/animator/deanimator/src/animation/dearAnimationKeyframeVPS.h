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
