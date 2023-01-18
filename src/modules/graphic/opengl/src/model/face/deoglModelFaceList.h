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

#ifndef _DEOGLMODELFACELIST_H_
#define _DEOGLMODELFACELIST_H_

class deoglModelFace;



/**
 * @brief Model Face List.
 * List of pointers to model faces. The ordering of the faces in the
 * list is not defined and can change when faces are removed. This
 * allows to remove faces with constant time requirements. This list
 * is supposed to stored pointers to faces from more than one model
 * and therefore is designed for classes required to keep track of
 * the model faces they belong to.
 */
class deoglModelFaceList{
private:
	deoglModelFace **pFaces;
	int pFaceCount;
	int pFaceSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new face list. */
	deoglModelFaceList();
	/** Cleans up the face list. */
	~deoglModelFaceList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of faces. */
	inline int GetCount() const{ return pFaceCount; }
	/** Retrieves the index of the face or -1 if not in the list. */
	int IndexOfFace( deoglModelFace *face ) const;
	/** Determines if the face exists. */
	bool Has( deoglModelFace *face ) const;
	/** Retrieves the face at the given index. */
	deoglModelFace *GetAt( int index ) const;
	/** Adds a face. */
	void Add( deoglModelFace *face );
	/** Adds a face if missing in the list. Returns true if added. */
	bool AddIfMissing( deoglModelFace *face );
	/** Removes face. */
	void Remove( deoglModelFace *face );
	/** Removes face if existing in the list. Returns true if removed. */
	bool RemoveIfExisting( deoglModelFace *face );
	/** Removes face from the given index. */
	void RemoveFrom( int index );
	/** Removes all faces. */
	void RemoveAll();
	/** Sorts the faces by their index. */
	void SortByIndex();
	/*@}*/
	
private:
	void pAddFace( deoglModelFace *face );
};

// end of include only once
#endif
