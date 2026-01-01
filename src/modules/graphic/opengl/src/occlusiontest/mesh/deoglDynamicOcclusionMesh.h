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

#ifndef _DEOGLDYNAMICOCCLUSIONMESH_H_
#define _DEOGLDYNAMICOCCLUSIONMESH_H_

#include "deoglROcclusionMesh.h"
#include "../../deoglBasics.h"

#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>

class deoglRComponent;
class deoglSharedVBOBlock;
class deoglRenderThread;
class deoglVAO;
class deoglBVH;

class deComponent;


/**
 * Occlusion Dynamic Mesh.
 */
class deoglDynamicOcclusionMesh{
public:
	deoglRenderThread &pRenderThread;
	deoglROcclusionMesh::Ref pOcclusionMesh;
	deoglRComponent *pComponent;
	
	decIntList pBoneMappings;
	
	oglMatrix3x4 *pWeights;
	int pWeightCount;
	
	decVector *pVertices;
	
	GLuint pVBO;
	deoglVAO *pVAO;
	deoglVBOp *pVBOData;
	int pVBOVertexCount;
	int pVBOVertexSize;
	
	bool pDirtyOccMesh;
	bool pDirtyVBO;
	
	deoglBVH *pBVH;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new dynamic occlusion mesh object. */
	deoglDynamicOcclusionMesh(deoglRenderThread &renderThread,
		deoglROcclusionMesh *occlusionMesh, deoglRComponent *component);
	
	/** Cleans up the peer. */
	~deoglDynamicOcclusionMesh();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	/** Retrieves the occlusion mesh. */
	inline const deoglROcclusionMesh::Ref &GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** VBO. */
	inline GLuint GetVBO() const{ return pVBO; }
	
	/** VAO. */
	deoglVAO *GetVAO() const;
	
	/** Invalidates the VAO. */
	void InvalidateVAO();
	
	/** Component state changed. */
	void ComponentStateChanged();
	
	/** Update bone mappings. */
	void UpdateBoneMappings(const deComponent &component);
	
	/** Prepare for rendering. */
	void PrepareForRender();
	
	/** Direct access to vertices. */
	inline const decVector *GetVertices() const{ return pVertices; }
	
	
	
	/** BVH or NULL. */
	inline deoglBVH *GetBVH() const{ return pBVH; }
	
	/** Build BVH if not build yet. */
	void PrepareBVH();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pBuildArrays();
	void pCalculateWeights();
	void pTransformVertices();
	void pWriteVBOData();
	void pBuildVBO();
	void pUpdateVAO();
};

#endif
