/* 
 * Drag[en]gine Bullet Physics Module
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
#ifndef _DEBPDECALFACE_H_
#define _DEBPDECALFACE_H_

// includes
#include "dragengine/common/math/decMath.h"



/**
 * @brief Bullet Decal Face.
 * Face of a decal.
 */
class debpDecalFace{
public:
	decVector pPoints[ 3 ];
	decVector pNormal;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new decal face. */
	debpDecalFace();
	/** Cleans up the decal face. */
	~debpDecalFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the face normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	/** Sets the face normal. */
	void SetNormal( const decVector &normal );
	
	/** Retrieves the first point. */
	inline const decVector &GetPoint1() const{ return pPoints[ 0 ]; }
	/** Sets the first point. */
	void SetPoint1( const decVector &point );
	/** Retrieves the second point. */
	inline const decVector &GetPoint2() const{ return pPoints[ 1 ]; }
	/** Sets the second point. */
	void SetPoint2( const decVector &point );
	/** Retrieves the third point. */
	inline const decVector &GetPoint3() const{ return pPoints[ 2 ]; }
	/** Sets the third point. */
	void SetPoint3( const decVector &point );
	/*@}*/
};

// end of include only once
#endif
