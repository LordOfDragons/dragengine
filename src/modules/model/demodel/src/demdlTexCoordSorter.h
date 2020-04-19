/* 
 * Drag[en]gine Model Module
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
	void Resize( int faceCount, int texCoordSetCount );
	
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
	int GetFaceCornerAt( int face, int corner ) const;
	
	/** \brief Set face corner texture coordinate index. */
	void SetFaceCornerAt( int face, int corner, int texCoord );
	
	/** \brief Face texture coordinate. */
	const decVector2 &GetFaceTexCoordAt( int face, int corner, int texCoordSet ) const;
	
	/** \brief Set face texture coordinate. */
	void SetFaceTexCoordAt( int face, int corner, int texCoordSet, const decVector2 &texCoord );
	
	/** \brief Number of texture coordinates. */
	inline int GetTexCoordCount() const{ return pTexCoordCount; }
	
	/** \brief Texture coordinate. */
	const decVector2 &GetTexCoordAt( int index, int texCoordSet ) const;
	/*@}*/
	
	
	
private:
	int HashTexCoords( const decVector2 *texCoords ) const;
	bool TexCoordsAreEqual( const decVector2 *texCoords1, const decVector2 *texCoords2 ) const;
};

#endif
