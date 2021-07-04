/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLGIBVHLOCAL_H_
#define _DEOGLGIBVHLOCAL_H_

#include "../utils/bvh/deoglBVH.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/math/decMath.h>

class deoglModelFace;
class deoglRenderThread;
class deoglDynamicTBOFloat32;
class deoglDynamicTBOUInt16;
class deoglDynamicTBOFloat16;
class deoglDynamicTBOBlock;

struct oglModelPosition;
struct oglModelVertex;


/**
 * Global illumination Local BVH. Stores pre-calculated local BVH data that can be copied
 * to deoglGIBVH directly or transformed.
 */
class deoglGIBVHLocal{
protected:
	deoglRenderThread &pRenderThread;
	deoglBVH pBVH;
	
	deoglDynamicTBOFloat32 *pTBONodeBox;
	deoglDynamicTBOUInt16 *pTBOIndex;
	deoglDynamicTBOUInt16 *pTBOFace;
	deoglDynamicTBOFloat32 *pTBOVertex;
	deoglDynamicTBOFloat16 *pTBOTexCoord;
	
	deObjectReference pBlockNode;
	deObjectReference pBlockFace;
	deObjectReference pBlockVertex;
	
	int pBlockUsageCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination local BVH. */
	deoglGIBVHLocal( deoglRenderThread &renderThread );
	
	/** Clean up global illumination local BVH. */
	~deoglGIBVHLocal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread(){ return pRenderThread; }
	inline const deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** TBO for BVH node boundaries. */
	inline deoglDynamicTBOFloat32 *GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** TBO for BVH node indices. */
	inline deoglDynamicTBOUInt16 *GetTBOIndex() const{ return pTBOIndex; }
	
	/** TBO for mesh faces. */
	inline deoglDynamicTBOUInt16 *GetTBOFace() const{ return pTBOFace; }
	
	/** TBO for mesh vertices. */
	inline deoglDynamicTBOFloat32 *GetTBOVertex() const{ return pTBOVertex; }
	
	/** TBO for mesh texture coordinates. */
	inline deoglDynamicTBOFloat16 *GetTBOTexCoord() const{ return pTBOTexCoord; }
	
	/** BVH. */
	inline deoglBVH &GetBVH(){ return pBVH; }
	inline const deoglBVH &GetBVH() const{ return pBVH; }
	
	/** Clear. */
	void Clear();
	
	/**
	 * Build tree. List of build primitives has to contain the boundary information for each
	 * primitive in the same order the primitives are indexed. The array can be deleted after
	 * build. BVH has to be present before faces can be added.
	 */
	void BuildBVH( const deoglBVH::sBuildPrimitive *primitives, int primitiveCount, int maxDepth = 12 );
	
	/**
	 * Recalculate BVH node extends. Keeps the BVH structure but adjusts to changing vertex
	 * positions. This is not the optimal BVH for the new configuration but still a valid one.
	 * Not changing the BVH structure allows to update an animated model in place without
	 * changing the required node count nor indices into TBO data. This is suitable to update
	 * animated models quickly.
	 * 
	 * Requires TBO Vertices and Faces to be present before calling this method.
	 */
	void UpdateBVHExtends();
	
	/** Add vertex to TBO. */
	void TBOAddVertex( const decVector &position );
	
	/** Add vertices to TBO. */
	void TBOAddVertices( const oglModelPosition *positions, int count );
	
	/** Add face to TBO. */
	void TBOAddFace( int vertex1, int vertex2, int vertex3, int material,
		const decVector2 &texCoord1, const decVector2 &texCoord2,
		const decVector2 &texCoord3 );
	
	/** Add faces to TBO. Requires BVH to be build first because it uses primitives as faces. */
	void TBOAddFaces( const deoglModelFace *faces, const oglModelVertex *vertices,
		const decVector2 *texCoords );
	
	/** Add BVH to TBO. */
	void TBOAddBVH();
	
	/** Update TBO nodes extends from BVH node extends. */
	void TBOBVHUpdateNodeExtends();
	
	
	
	/** Get block allocating it if absent. */
	deoglDynamicTBOBlock *GetBlockNode();
	deoglDynamicTBOBlock *GetBlockFace();
	deoglDynamicTBOBlock *GetBlockVertex();
	
	/** Drop blocks. */
	void DropBlocks();
	
	/**
	 * Add block usage. A usage count larger than 0 does not guarantee blocks to be present.
	 * Blocks are created if absent using the GetBlock*() calls. The usage count is only
	 * used to drop blocks if nobody uses them anymore.
	 */
	void AddBlockUsage();
	
	/** Remove block usage calling DropBlocks() if reaching 0. */
	void RemoveBlockUsage();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateBVHExtends( deoglBVHNode &node );
};

#endif
