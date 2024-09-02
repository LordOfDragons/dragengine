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

#ifndef _DEOGLMODELFACELIST_H_
#define _DEOGLMODELFACELIST_H_

class deoglModelFace;



/**
 * Model Face List.
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
