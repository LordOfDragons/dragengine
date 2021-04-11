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

#ifndef _DEOGLGIBVHDYNAMIC_H_
#define _DEOGLGIBVHDYNAMIC_H_

#include "../deoglBasics.h"

#include <dragengine/deObjectReference.h>
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
	void UpdateVertices( const oglVector *positions, int count );
	
	
	
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
	void pWriteNodeExtend( int index, const decVector &minExtend, const decVector &maxExtend );
};

#endif
