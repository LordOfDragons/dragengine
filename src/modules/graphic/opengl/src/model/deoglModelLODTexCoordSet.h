/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLMODELLODTEXCOORDSET_H_
#define _DEOGLMODELLODTEXCOORDSET_H_

#include <dragengine/common/math/decMath.h>

struct oglModelVertex;



/**
 * Model LOD mesh texture coordinate set.
 */
class deoglModelLODTexCoordSet{
public:
	decVector2 *pTexCoords;
	int pTexCoordCount;
	
	decVector *pTangents;
	bool *pNegateTangents;
	int pTangentCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model lod texture coordinate set. */
	deoglModelLODTexCoordSet();
	/** Cleans up the model lod texture coordinate set. */
	~deoglModelLODTexCoordSet();
	/*@}*/
	
	/** \name Management */
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
