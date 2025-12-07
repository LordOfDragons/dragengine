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

#ifndef _DEMDLTEXCOORDSORTER_H_
#define _DEMDLTEXCOORDSORTER_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Textue coordinates sorter.
 * 
 * Older models store texture coordinates for face corners. The Drag[en]gine
 * expects though face corners to index into a list of texture coordinates.
 * This allows graphic modules to optimize and reduce size.
 * 
 * This class builds the minimal list of texture coordinates from the face
 * corners and assigns them the texture coordinate index.
 */
class demdlTexCoordSorter{
private:
	struct sBucketEntry{
		const decVector2 *texCoords;
		int index;
		sBucketEntry *next;
	};
	
	int pFaceCount;
	int pTexCoordSetCount;
	int *pFaceCorners;
	decVector2 *pFaceTexCoords;
	
	sBucketEntry **pTCBuckets;
	sBucketEntry *pTCBucketEntries;
	const decVector2 **pTexCoords;
	int pTexCoordCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create texture coordinates sorter. */
	demdlTexCoordSorter();
	
	/** \brief Clean up texture coordinates sorter. */
	~demdlTexCoordSorter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set number of faces and texture coordinate sets. */
	void Resize(int faceCount, int texCoordSetCount);
	
	/**
	 * \brief Sort texture coordinates.
	 * 
	 * Replaces the list of texture coordinates with the smallest list of texture
	 * coordinates containing no duplicates and assigns the matching indices to
	 * the face corners.
	 */
	void Sort();
	
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Number of texture coordinate sets. */
	inline int GetTexCoordSetCount() const{ return pTexCoordSetCount; }
	
	/** \brief Face corner texture coordinate index. */
	int GetFaceCornerAt(int face, int corner) const;
	
	/** \brief Set face corner texture coordinate index. */
	void SetFaceCornerAt(int face, int corner, int texCoord);
	
	/** \brief Face texture coordinate. */
	const decVector2 &GetFaceTexCoordAt(int face, int corner, int texCoordSet) const;
	
	/** \brief Set face texture coordinate. */
	void SetFaceTexCoordAt(int face, int corner, int texCoordSet, const decVector2 &texCoord);
	
	/** \brief Number of texture coordinates. */
	inline int GetTexCoordCount() const{ return pTexCoordCount; }
	
	/** \brief Texture coordinate. */
	const decVector2 &GetTexCoordAt(int index, int texCoordSet) const;
	/*@}*/
	
	
	
private:
	int HashTexCoords(const decVector2 *texCoords) const;
	bool TexCoordsAreEqual(const decVector2 *texCoords1, const decVector2 *texCoords2) const;
};

#endif
