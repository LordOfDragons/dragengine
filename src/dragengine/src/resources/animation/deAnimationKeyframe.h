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

#ifndef _DEANIMATIONKEYFRAME_H_
#define _DEANIMATIONKEYFRAME_H_

#include "../../common/math/decMath.h"


/**
 * \brief Animation bone keyframe.
 */
class deAnimationKeyframe{
private:
	float pTime;
	decVector pPosition;
	decVector pRotation;
	decVector pScale;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animation bone keyframe. */
	deAnimationKeyframe();
	
	/** \brief Clean up animation bone keyframe. */
	~deAnimationKeyframe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Time in seconds. */
	inline float GetTime() const{ return pTime; }
	
	/** \brief Set time in seconds. */
	void SetTime( float time );
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set rotation. */
	void SetRotation( const decVector &rotation );
	
	/** \brief Scaling. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scaling. */
	void SetScale( const decVector &scale );
	/*@}*/
};

#endif
