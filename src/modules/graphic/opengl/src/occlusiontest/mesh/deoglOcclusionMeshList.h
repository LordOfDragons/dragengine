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
