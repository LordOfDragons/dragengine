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
#ifndef _DEBPDMBCONVEXVOLUMEFACE_H_
#define _DEBPDMBCONVEXVOLUMEFACE_H_

// includes
#include "dragengine/common/math/decConvexVolumeFace.h"
#include "dragengine/common/math/decMath.h"



/**
 * @brief OpenGL Decal Mesh Builder Convex Volume Face.
 * Spezialized version of the convex volume face used by the
 * decal mesh builder.
 */
class debpDMBConvexVolumeFace : public decConvexVolumeFace{
public:
	bool pDecalFace;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new decal mesh builder convex volume face. */
	debpDMBConvexVolumeFace();
	/** Cleans up the decal mesh builder convex volume face. */
	virtual ~debpDMBConvexVolumeFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if this face is a decal face. */
	inline bool GetDecalFace() const{ return pDecalFace; }
	/** Sets if this face is a decal face. */
	void SetDecalFace( bool decalFace );
	/*@}*/
};

// end of include only once
#endif
