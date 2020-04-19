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

#ifndef _DEOCCLUSIONMESH_H_
#define _DEOCCLUSIONMESH_H_

#include "../deFileResource.h"
#include "../../common/math/decMath.h"

class deOcclusionMeshBone;
class deOcclusionMeshVertex;
class deOcclusionMeshWeight;
class deOcclusionMeshManager;
class deWorld;
class deBaseGraphicOcclusionMesh;


/**
 * \brief Occlusion Mesh.
 * 
 * Defines an occlusion mesh used to help graphic modules to improve occlusion testing
 * if supported. Occlusion meshes are optional but potentially improve performance.
 * Occlusion meshes are similar to models in that they support bone weights but they
 * do not support LOD nor textures or texture coordinate sets. Faces are classified
 * in single sided and double sided. Single sided is useful to define occlusion only
 * in the direction in front of a faces. In contrary to models an occlusion mesh face
 * can have more than 3 corners as long as they are all coplanar and convex. corners
 * are the vertices used in the faces and are stored as a continuous array of vertex
 * indices. Each face defines the number of corners it contains in clockwise order.
 */
class deOcclusionMesh : public deFileResource{
private:
	deOcclusionMeshBone *pBones;
	deOcclusionMeshWeight *pWeights;
	int *pWeightGroups;
	deOcclusionMeshVertex *pVertices;
	unsigned short *pCorners;
	unsigned short *pFaces;
	
	int pBoneCount;
	int pWeightCount;
	int pWeightGroupCount;
	int pVertexCount;
	int pCornerCount;
	int pFaceCount;
	int pDoubleSidedFaceCount;
	
	deBaseGraphicOcclusionMesh *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new occlusion mesh with the given resource manager. */
	deOcclusionMesh( deOcclusionMeshManager *manager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up occlusion mesh.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deOcclusionMesh();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Verify the mesh. */
	bool Verify() const;
	
	/** \brief Prepare mesh for later use. */
	void Prepare();
	/*@}*/
	
	
	
	/** \name Bones */
	/*@{*/
	/** \brief Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** \brief Set number of bones. */
	void SetBoneCount( int count );
	
	/** \brief Retrieves a bone by index. */
	deOcclusionMeshBone &GetBoneAt( int index ) const;
	
	/** \brief Pointer to the bones. */
	inline deOcclusionMeshBone *GetBones() const{ return pBones; }
	
	/** \brief Index of the bone with the given name or -1 if not found. */
	int IndexOfBoneNamed( const char *name ) const;
	
	/** \brief Determiens if a bone with the given name exists. */
	bool HasBoneNamed( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Weight Sets */
	/*@{*/
	/** \brief Number of weights. */
	inline int GetWeightCount() const{ return pWeightCount; }
	
	/** \brief Set number of weights. */
	void SetWeightCount( int count );
	
	/** \brief Weight at the given position. */
	deOcclusionMeshWeight &GetWeightAt( int index ) const;
	
	/** \brief Pointer to the weights. */
	inline deOcclusionMeshWeight *GetWeights() const{ return pWeights; }
	
	/** \brief Number of weight groups. */
	inline int GetWeightGroupCount() const{ return pWeightGroupCount; }
	
	/** \brief Set number of weight groups. */
	void SetWeightGroupCount( int count );
	
	/** \brief Weight group at the given position. */
	int GetWeightGroupAt( int index ) const;
	
	/** \brief Set weight group at the given position. */
	void SetWeightGroupAt( int index, int weightSetCount ) const;
	
	/** \brief Pointer to the weight groups. */
	inline int *GetWeightGroups() const{ return pWeightGroups; }
	/*@}*/
	
	
	
	/** \name Vertices */
	/*@{*/
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Set number of vertices. */
	void SetVertexCount( int count );
	
	/** \brief Vertex at the given position. */
	deOcclusionMeshVertex &GetVertexAt( int index ) const;
	
	/** \brief Pointer to the vertices. */
	inline deOcclusionMeshVertex *GetVertices() const{ return pVertices; }
	/*@}*/
	
	
	
	/** \name Corners */
	/*@{*/
	/** \brief Number of corners. */
	inline int GetCornerCount() const{ return pCornerCount; }
	
	/** \brief Set number of corners. */
	void SetCornerCount( int count );
	
	/** \brief Corner at the given position. */
	unsigned short GetCornerAt( int index ) const;
	
	/** \brief Set corner at the given position. */
	void SetCornerAt( int index, unsigned short vertexIndex );
	
	/** \brief Pointer to the corners. */
	inline unsigned short *GetCorners() const{ return pCorners; }
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Set number of faces. */
	void SetFaceCount( int count );
	
	/** \brief Number of faces at the end of the face list that are double sided not single sided. */
	inline int GetDoubleSidedFaceCount() const{ return pDoubleSidedFaceCount; }
	
	/** \brief Set number of faces at the end of the face list that are double sided not single sided. */
	void SetDoubleSidedFaceCount( int count );
	
	/** \brief Corner count for the face at the given position. */
	unsigned short GetFaceAt( int index ) const;
	
	/** \brief Set corner count for the face at the given position. */
	void SetFaceAt( int index, unsigned short cornerCount );
	
	/** \brief Pointer to the face corner counts. */
	inline unsigned short *GetFaces() const{ return pFaces; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicOcclusionMesh *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicOcclusionMesh *peer );
	/*@}*/
};

#endif
