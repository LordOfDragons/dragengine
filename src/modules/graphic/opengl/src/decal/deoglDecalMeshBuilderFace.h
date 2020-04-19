/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLDECALMESHBUILDERFACE_H_
#define _DEOGLDECALMESHBUILDERFACE_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief OpenGL decal mesh builder face.
 */
class deoglDecalMeshBuilderFace{
public:
	int pPoints[ 3 ];
	decVector pNormal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decal mesh builder face. */
	deoglDecalMeshBuilderFace();
	
	/** \brief Clean up decal mesh builder face. */
	~deoglDecalMeshBuilderFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Point by index. */
	int GetPointAt( int index ) const;
	
	/** \brief First point. */
	inline int GetPoint1() const{ return pPoints[ 0 ]; }
	
	/** \brief Set first point. */
	void SetPoint1( int point );
	
	/** \brief Second point. */
	inline int GetPoint2() const{ return pPoints[ 1 ]; }
	
	/** \brief Set second point. */
	void SetPoint2( int point );
	
	/** \brief Third point. */
	inline int GetPoint3() const{ return pPoints[ 2 ]; }
	
	/** \brief Set third point. */
	void SetPoint3( int point );
	
	/** \brief Face normal. */
	inline const decVector &GetFaceNormal() const{ return pNormal; }
	
	/** \brief Set face normal. */
	void SetFaceNormal( const decVector &normal );
	/*@}*/
};

// end of include only once
#endif
