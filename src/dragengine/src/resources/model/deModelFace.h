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

#ifndef _DEMODELFACE_H_
#define _DEMODELFACE_H_

#include "../../common/math/decMath.h"


/**
 * \brief Model Triangular Face.
 */
class DE_DLL_EXPORT deModelFace{
private:
	int pTexture;
	int pVertex[ 3 ];
	int pNormal[ 3 ];
	int pTangent[ 3 ];
	int pTextureCoordinates[ 3 ];
	decVector pFaceNormal;
	decVector pFaceTangent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model face. */
	deModelFace();
	
	/** \brief Clean up model face. */
	~deModelFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of the texture. */
	inline int GetTexture() const{ return pTexture; }
	
	/** \brief Set index of the texture. */
	void SetTexture( int texture );
	
	/** \brief Index of the first vertex. */
	inline int GetVertex1() const{ return pVertex[ 0 ]; }
	
	/** \brief Set index of the first vertex. */
	void SetVertex1( int vertex );
	
	/** \brief Index of the second vertex. */
	inline int GetVertex2() const{ return pVertex[ 1 ]; }
	
	/** \brief Set index of the second vertex. */
	void SetVertex2( int vertex );
	
	/** \brief Index of the third vertex. */
	inline int GetVertex3() const{ return pVertex[ 2 ]; }
	
	/** \brief Set index of the third vertex. */
	void SetVertex3( int vertex );
	
	/** \brief Index of the given vertex. */
	int GetVertexAt( int corner ) const;
	
	/** \brief Set index of the given vertex. */
	void SetVertexAt( int corner, int vertex );
	
	/** \brief Index of the first normal. */
	inline int GetNormal1() const{ return pNormal[ 0 ]; }
	
	/** \brief Set index of the first normal. */
	void SetNormal1( int normal );
	
	/** \brief Index of the second normal. */
	inline int GetNormal2() const{ return pNormal[ 1 ]; }
	
	/** \brief Set index of the second normal. */
	void SetNormal2( int normal );
	
	/** \brief Index of the third normal. */
	inline int GetNormal3() const{ return pNormal[ 2 ]; }
	
	/** \brief Set index of the third normal. */
	void SetNormal3( int normal );
	
	/** \brief Index of the given normal. */
	int GetNormalAt( int corner ) const;
	
	/** \brief Set index of the given normal. */
	void SetNormalAt( int corner, int normal );
	
	/** \brief Index of the first tangent. */
	inline int GetTangent1() const{ return pTangent[ 0 ]; }
	
	/** \brief Set index of the first tangent. */
	void SetTangent1( int tangent );
	
	/** \brief Index of the second tangent. */
	inline int GetTangent2() const{ return pTangent[ 1 ]; }
	
	/** \brief Set index of the second tangent. */
	void SetTangent2( int tangent );
	
	/** \brief Index of the third tangent. */
	inline int GetTangent3() const{ return pTangent[ 2 ]; }
	
	/** \brief Set index of the third tangent. */
	void SetTangent3( int tangent );
	
	/** \brief Index of the given tangent. */
	int GetTangentAt( int corner ) const;
	
	/** \brief Set index of the given tangent. */
	void SetTangentAt( int corner, int tangent );
	
	/** \brief First texture coordinates index. */
	inline int GetTextureCoordinates1() const{ return pTextureCoordinates[ 0 ]; }
	
	/** \brief Set first texture coordinates index. */
	void SetTextureCoordinates1( int textureCoordinates );
	
	/** \brief Second texture coordinates index. */
	inline int GetTextureCoordinates2() const{ return pTextureCoordinates[ 1 ]; }
	
	/** \brief Set second texture coordinates index. */
	void SetTextureCoordinates2( int textureCoordinates );
	
	/** \brief Third texture coordinates index. */
	inline int GetTextureCoordinates3() const{ return pTextureCoordinates[ 2 ]; }
	
	/** \brief Set third texture coordinates index. */
	void SetTextureCoordinates3( int textureCoordinates );
	
	/** \brief Given texture coordinates index. */
	int GetTextureCoordinatesAt( int corner ) const;
	
	/** \brief Set given texture coordinates index. */
	void SetTextureCoordinatesAt( int corner, int textureCoordinates );
	
	/** \brief Face normal. */
	inline const decVector &GetFaceNormal() const{ return pFaceNormal; }
	
	/** \brief Set face normal. */
	void SetFaceNormal( const decVector &normal );
	
	/** \brief Face tangent. */
	inline const decVector &GetFaceTangent() const{ return pFaceTangent; }
	
	/** \brief Set face tangent. */
	void SetFaceTangent( const decVector &tangent );
	/*@}*/
};

#endif
