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
class DE_DLL_EXPORT deOcclusionMesh : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deOcclusionMesh> Ref;
	
	
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
	deOcclusionMesh(deOcclusionMeshManager *manager, deVirtualFileSystem *vfs,
		const char *filename, TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up occlusion mesh.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deOcclusionMesh() override;
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
	void SetBoneCount(int count);
	
	/** \brief Retrieves a bone by index. */
	deOcclusionMeshBone &GetBoneAt(int index) const;
	
	/** \brief Pointer to the bones. */
	inline deOcclusionMeshBone *GetBones() const{ return pBones; }
	
	/** \brief Index of the bone with the given name or -1 if not found. */
	int IndexOfBoneNamed(const char *name) const;
	
	/** \brief Determiens if a bone with the given name exists. */
	bool HasBoneNamed(const char *name) const;
	/*@}*/
	
	
	
	/** \name Weight Sets */
	/*@{*/
	/** \brief Number of weights. */
	inline int GetWeightCount() const{ return pWeightCount; }
	
	/** \brief Set number of weights. */
	void SetWeightCount(int count);
	
	/** \brief Weight at the given position. */
	deOcclusionMeshWeight &GetWeightAt(int index) const;
	
	/** \brief Pointer to the weights. */
	inline deOcclusionMeshWeight *GetWeights() const{ return pWeights; }
	
	/** \brief Number of weight groups. */
	inline int GetWeightGroupCount() const{ return pWeightGroupCount; }
	
	/** \brief Set number of weight groups. */
	void SetWeightGroupCount(int count);
	
	/** \brief Weight group at the given position. */
	int GetWeightGroupAt(int index) const;
	
	/** \brief Set weight group at the given position. */
	void SetWeightGroupAt(int index, int weightSetCount) const;
	
	/** \brief Pointer to the weight groups. */
	inline int *GetWeightGroups() const{ return pWeightGroups; }
	/*@}*/
	
	
	
	/** \name Vertices */
	/*@{*/
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Set number of vertices. */
	void SetVertexCount(int count);
	
	/** \brief Vertex at the given position. */
	deOcclusionMeshVertex &GetVertexAt(int index) const;
	
	/** \brief Pointer to the vertices. */
	inline deOcclusionMeshVertex *GetVertices() const{ return pVertices; }
	/*@}*/
	
	
	
	/** \name Corners */
	/*@{*/
	/** \brief Number of corners. */
	inline int GetCornerCount() const{ return pCornerCount; }
	
	/** \brief Set number of corners. */
	void SetCornerCount(int count);
	
	/** \brief Corner at the given position. */
	unsigned short GetCornerAt(int index) const;
	
	/** \brief Set corner at the given position. */
	void SetCornerAt(int index, unsigned short vertexIndex);
	
	/** \brief Pointer to the corners. */
	inline unsigned short *GetCorners() const{ return pCorners; }
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Set number of faces. */
	void SetFaceCount(int count);
	
	/** \brief Number of faces at the end of the face list that are double sided not single sided. */
	inline int GetDoubleSidedFaceCount() const{ return pDoubleSidedFaceCount; }
	
	/** \brief Set number of faces at the end of the face list that are double sided not single sided. */
	void SetDoubleSidedFaceCount(int count);
	
	/** \brief Corner count for the face at the given position. */
	unsigned short GetFaceAt(int index) const;
	
	/** \brief Set corner count for the face at the given position. */
	void SetFaceAt(int index, unsigned short cornerCount);
	
	/** \brief Pointer to the face corner counts. */
	inline unsigned short *GetFaces() const{ return pFaces; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicOcclusionMesh *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic(deBaseGraphicOcclusionMesh *peer);
	/*@}*/
};

#endif
