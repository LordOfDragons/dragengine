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

#ifndef _DEOGLGIBVHDYNAMIC_H_
#define _DEOGLGIBVHDYNAMIC_H_

#include "../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglBVHNode;
class deoglGIBVHLocal;
class deoglDynamicTBOFloat32;
class deoglDynamicTBOBlock;

struct oglModelPosition;
struct oglModelVertex;


/**
 * Global illumination Dynamic BVH. Stores dynamic BVH data relative to a local BVH.
 */
class deoglGIBVHDynamic{
protected:
	deoglGIBVHLocal &pGIBVHLocal;
	
	deoglDynamicTBOFloat32 *pTBONodeBox;
	deoglDynamicTBOFloat32 *pTBOVertex;
	
	deObjectReference pBlockNode;
	deObjectReference pBlockVertex;
	
	decVector pMinExtend;
	decVector pMaxExtend;
	
	int pBlockUsageCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination dynamic BVH. */
	deoglGIBVHDynamic( deoglGIBVHLocal &bvhLocal );
	
	/** Clean up global illumination dynamic BVH. */
	~deoglGIBVHDynamic();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** TBO for BVH node boundaries. */
	inline deoglDynamicTBOFloat32 *GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** TBO for mesh vertices. */
	inline deoglDynamicTBOFloat32 *GetTBOVertex() const{ return pTBOVertex; }
	
	/** Local BVH. */
	inline deoglGIBVHLocal &GetGIBVHLocal(){ return pGIBVHLocal; }
	inline const deoglGIBVHLocal &GetGIBVHLocal() const{ return pGIBVHLocal; }
	
	/** BVH minimum extends. Valid after call to UpdateBVHExtends. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** BVH minimum extends. Valid after call to UpdateBVHExtends. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	
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
	
	/** Update vertex positions. */
	void UpdateVertices( const oglModelPosition *positions, int count );
	
	/** Update vertex positions. */
	void UpdateVertices( const oglVector3 *positions, int count );
	
	
	
	/**
	 * Get block allocating it if absent.
	 * \note Block node shares TBO index with local BVH while TBO node box is replaced.
	 */
	deoglDynamicTBOBlock *GetBlockNode();
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
	void pCalcNodeExtends( const deoglBVHNode &node, decVector &minExtend, decVector &maxExtend );
	void pWriteNodeExtends( int index, const decVector &minExtend, const decVector &maxExtend );
};

#endif
