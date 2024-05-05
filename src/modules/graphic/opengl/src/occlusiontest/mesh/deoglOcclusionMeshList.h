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

#ifndef _DEOGLOCCLUSIONMESHLIST_H_
#define _DEOGLOCCLUSIONMESHLIST_H_

class deoglOcclusionMesh;



/**
 * Occlusion Mesh List.
 * List of pointers to occlusion meshes. The ordering of the occlusion meshes
 * in the list is not defined and can change when occlusion meshes are removed.
 * This allows to remove elements with constant time requirements.
 */
class deoglOcclusionMeshList{
private:
	deoglOcclusionMesh **pOcclusionMeshes;
	int pOcclusionMeshCount;
	int pOcclusionMeshSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new occlusion mesh list. */
	deoglOcclusionMeshList();
	/** Cleans up the occlusion mesh list. */
	~deoglOcclusionMeshList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of occlusion meshes. */
	inline int GetCount() const{ return pOcclusionMeshCount; }
	/** Retrieves the index of the occlusion mesh or -1 if not in the list. */
	int IndexOfOcclusionMesh( deoglOcclusionMesh *occlusionMesh ) const;
	/** Determines if the occlusion mesh exists. */
	bool Has( deoglOcclusionMesh *occlusionMesh ) const;
	/** Retrieves the occlusion mesh at the given index. */
	deoglOcclusionMesh *GetAt( int index ) const;
	/** Adds a occlusion mesh. */
	void Add( deoglOcclusionMesh *occlusionMesh );
	/** Adds a occlusion mesh if missing in the list. Returns true if added. */
	bool AddIfMissing( deoglOcclusionMesh *occlusionMesh );
	/** Removes occlusion mesh. */
	void Remove( deoglOcclusionMesh *occlusionMesh );
	/** Removes occlusion mesh if existing in the list. Returns true if removed. */
	bool RemoveIfExisting( deoglOcclusionMesh *occlusionMesh );
	/** Removes occlusion mesh from the given index. */
	void RemoveFrom( int index );
	/** Removes all occlusion meshes. */
	void RemoveAll();
	/*@}*/
	
private:
	void pAddOcclusionMesh( deoglOcclusionMesh *occlusionMesh );
};

#endif
