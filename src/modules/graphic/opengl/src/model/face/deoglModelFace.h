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

#ifndef _DEOGLMODELFACE_H_
#define _DEOGLMODELFACE_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Model Face.
 */
class deoglModelFace{
private:
	int pVertices[ 3 ];
	decVector pFaceNormal;
	decVector pMinExtend;
	decVector pMaxExtend;
	decVector pCenter;
	int pTexture;
	int pIndex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model face. */
	deoglModelFace();
	/** Cleans up the model face. */
	~deoglModelFace();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the first vertex. */
	inline int GetVertex1() const{ return pVertices[ 0 ]; }
	/** Sets the first vertex. */
	void SetVertex1( int vertex );
	/** Retrieves the second vertex. */
	inline int GetVertex2() const{ return pVertices[ 1 ]; }
	/** Sets the second vertex. */
	void SetVertex2( int vertex );
	/** Retrieves the third vertex. */
	inline int GetVertex3() const{ return pVertices[ 2 ]; }
	/** Sets the third vertex. */
	void SetVertex3( int vertex );
	/** Retrieves the given vertex. */
	inline int GetVertexAt( int index ) const{ return pVertices[ index ]; }
	/** Sets the given vertex. */
	void SetVertexAt( int index, int vertex );
	
	/** Retrieves the face normal. */
	inline const decVector &GetFaceNormal() const{ return pFaceNormal; }
	/** Sets the face normal. */
	void SetFaceNormal( const decVector &normal );
	/** Retrieves the minimum extend. */
	inline const decVector &GetMinExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decVector &GetMaxExtend() const{ return pMaxExtend; }
	/** Sets the extends. */
	void SetExtends( const decVector &minExtend, const decVector &maxExtend );
	/** Retrieves the center. */
	inline const decVector &GetCenter() const{ return pCenter; }
	/** Sets the center. */
	void SetCenter( const decVector &center );
	
	/** Retrieves the texture. */
	inline int GetTexture() const{ return pTexture; }
	/** Sets the texture. */
	void SetTexture( int texture );
	/** Retrieves the index. */
	inline int GetIndex() const{ return pIndex; }
	/** Sets the index. */
	void SetIndex( int index );
	/*@}*/
};

#endif
