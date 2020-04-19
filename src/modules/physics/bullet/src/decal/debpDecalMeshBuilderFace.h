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
#ifndef _DEBPDECALMESHBUILDERFACE_H_
#define _DEBPDECALMESHBUILDERFACE_H_

// includes
#include "dragengine/common/math/decMath.h"



/**
 * @brief Bullet Decal Mesh Builder Face.
 * Face used by the mesh builder.
 */
class debpDecalMeshBuilderFace{
public:
	int pPoints[ 3 ];
	int pFaceIndex;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new decal mesh builder face. */
	debpDecalMeshBuilderFace();
	/** Cleans up the decal mesh builder face. */
	~debpDecalMeshBuilderFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the first point. */
	inline int GetPoint1() const{ return pPoints[ 0 ]; }
	/** Sets the first point. */
	void SetPoint1( int point );
	/** Retrieves the second point. */
	inline int GetPoint2() const{ return pPoints[ 1 ]; }
	/** Sets the second point. */
	void SetPoint2( int point );
	/** Retrieves the thirdpoint. */
	inline int GetPoint3() const{ return pPoints[ 2 ]; }
	/** Sets the third point. */
	void SetPoint3( int point );
	/** Retrieves the face index. */
	inline int GetFaceIndex() const{ return pFaceIndex; }
	/** Sets the face index. */
	void SetFaceIndex( int faceIndex );
	/*@}*/
};

// end of include only once
#endif
