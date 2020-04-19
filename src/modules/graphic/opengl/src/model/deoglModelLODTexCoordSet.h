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

#ifndef _DEOGLMODELLODTEXCOORDSET_H_
#define _DEOGLMODELLODTEXCOORDSET_H_

#include <dragengine/common/math/decMath.h>

struct oglModelVertex;



/**
 * @brief Model LOD mesh texture coordinate set.
 */
class deoglModelLODTexCoordSet{
public:
	decVector2 *pTexCoords;
	int pTexCoordCount;
	
	decVector *pTangents;
	bool *pNegateTangents;
	int pTangentCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new model lod texture coordinate set. */
	deoglModelLODTexCoordSet();
	/** Cleans up the model lod texture coordinate set. */
	~deoglModelLODTexCoordSet();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the texture coordinates. */
	inline decVector2 *GetTextureCoordinates() const{ return pTexCoords; }
	/** Retrieves the number of texture coordinates. */
	inline int GetTextureCoordinateCount() const{ return pTexCoordCount; }
	/** Sets the number of texture coordinates. */
	void SetTextureCoordinateCount( int count, bool copyContent );
	
	/** Retrieves the tangents. */
	inline decVector *GetTangents() const{ return pTangents; }
	/** Retrieves the negate tangents. */
	inline bool *GetNegateTangents() const{ return pNegateTangents; }
	/** Retrieves the tangent count. */
	inline int GetTangentCount() const{ return pTangentCount; }
	/** Sets the tangent count. */
	void SetTangentCount( int count );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
