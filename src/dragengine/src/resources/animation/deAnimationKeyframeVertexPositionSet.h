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

#ifndef _DEANIMATIONKEYFRAMEVERTEXPOSITIONSET_H_
#define _DEANIMATIONKEYFRAMEVERTEXPOSITIONSET_H_

#include "../../common/math/decMath.h"


/**
 * \brief Animation vertex position set keyframe.
 * \version 1.17
 */
class DE_DLL_EXPORT deAnimationKeyframeVertexPositionSet{
private:
	float pTime;
	float pWeight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation vertex position set keyframe. */
	deAnimationKeyframeVertexPositionSet();
	
	/** \brief Clean up animation vertex position set keyframe. */
	~deAnimationKeyframeVertexPositionSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Time in seconds. */
	inline float GetTime() const{ return pTime; }
	
	/** \brief Set time in seconds. */
	void SetTime( float time );
	
	/** \brief Weight. */
	inline float GetWeight() const{ return pWeight; }
	
	/** \brief Set weight. */
	void SetWeight( float weight );
	/*@}*/
};

#endif
