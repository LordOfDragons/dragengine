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

// include only once
#ifndef _DEARANIMATIONSTATE_H_
#define _DEARANIMATIONSTATE_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions



/**
 * @brief Animation state class.
 * Stores the state of an animation frame.
 */
class dearAnimationState{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new bone state object. */
	dearAnimationState();
	/** Cleans up the bone state. */
	~dearAnimationState();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decVector &position );
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** Sets the orientation. */
	void SetOrientation( const decQuaternion &orientation );
	/** Retrieves the size. */
	inline const decVector &GetSize() const{ return pSize; }
	/** Sets the size. */
	void SetSize( const decVector &size );
	/** Reset. */
	void Reset();
	/*@}*/
};

// end of include only once
#endif
