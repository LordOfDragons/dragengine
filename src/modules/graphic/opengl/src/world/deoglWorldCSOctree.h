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

#ifndef _DEOGLWORLDOCSCTREE_H_
#define _DEOGLWORLDOCSCTREE_H_

#include <stdint.h>

#include "../shaders/paramblock/deoglSPBlockSSBO.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoglWorldOctree;
class decLayerMask;


/**
 * World compute shader octree.
 */
class deoglWorldCSOctree : public deObject{
public:
	typedef deTObjectReference<deoglWorldCSOctree> Ref;
	
	/** Compute shader element flags. */
	enum eCSElementFlags{
		ecsefComponent = 0x1,
		ecsefComponentDynamic = 0x2,
		ecsefBillboard = 0x4,
		ecsefParticleEmitter = 0x8,
		ecsefLight = 0x10
	};
	
	/** Compute shader node parameters. */
	enum eCSNodeParams{
		ecsnpMinExtend,
		ecsnpFirstNode,
		ecsnpMaxExtend,
		ecsnpChildNodeCount,
		ecsnpElementCount
	};
	
	/** Compute shader node. Aligned to (u)vec4[3]. */
	struct sCSNode{
		float minExtendX, minExtendY, minExtendZ;
		uint32_t firstNode;
		
		float maxExtendX, maxExtendY, maxExtendZ;
		uint32_t childNodeCount;
		
		uint32_t firstElement;
		uint32_t elementCount;
		uint32_t padding[ 2 ];
		
		void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
	};
	
	/** Compute shader element parameters. */
	enum eCSElementParams{
		ecsepMinExtend,
		ecsepElementIndex,
		ecsepMaxExtend,
		ecsepFlags,
		ecsepLayerMask
	};
	
	/** Compute shader element. Aligned to (u)vec4[3]. */
	struct sCSElement{
		float minExtendX, minExtendY, minExtendZ;
		uint32_t elementindex;
		
		float maxExtendX, maxExtendY, maxExtendZ;
		uint32_t flags;
		
		uint32_t layerMask[ 2 ]; // 64-bit layer mask
		uint32_t padding[ 2 ];
		
		void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
		void SetLayerMask( const decLayerMask &layerMask );
	};
	
	/** Compute shader element types. */
	enum eCSElementTypes{
		ecsetComponent = 0,
		ecsetBillboard = 1,
		ecsetParticleEmitter = 2,
		ecsetLight = 3
	};
	
	/** Link. */
	struct sElementLink{
		eCSElementTypes type;
		const void *element;
	};
	
	
private:
	deoglRenderThread &pRenderThread;
	decDVector pReferencePosition;
	
	deoglSPBlockSSBO::Ref pSSBONodes;
	deoglSPBlockSSBO::Ref pSSBOElements;
	sCSNode *pPtrNode;
	sCSElement *pPtrElement;
	
	int pNodeCount;
	int pElementCount;
	
	int pNextNode;
	int pNextElement;
	
	sElementLink *pElementLinks;
	int pElementLinkSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create octree. */
	deoglWorldCSOctree( deoglRenderThread &renderThread );
	
protected:
	/** Clean up octree. */
	virtual ~deoglWorldCSOctree();
	/*@}*/
	
	
	
public:
	/** \name Visiting */
	/*@{*/
	/** World reference position. */
	inline const decDVector &GetReferencePosition() const{ return pReferencePosition; }
	
	/** Set world reference position. */
	void SetReferencePosition( const decDVector &position );
	
	
	
	/** SSBO compute shader nodes. */
	inline const deoglSPBlockSSBO::Ref &GetSSBONodes() const{ return pSSBONodes; }
	
	/** SSBO compute shader elements. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOElements() const{ return pSSBOElements; }
	
	/** Count of nodes in SSBO data. */
	inline int GetNodeCount() const{ return pNodeCount; }
	
	/** Count of elements in SSBO elments. */
	inline int GetElementCount() const{ return pElementCount; }
	
	/** Clear octree. */
	void Clear();
	
	/** Begin writing octree. */
	void BeginWriting( int nodeCount, int elementCount );
	
	/** End writing octree. */
	void EndWriting();
	
	
	
	/** Pointer to SSBO node write buffer or nullptr. */
	inline sCSNode *GetPtrNode() const{ return pPtrNode; }
	
	/** SSBO node write buffer at index or throws exception if not writing. */
	sCSNode &GetNodeAt( int index ) const;
	
	/** Index of next unused SSBO node. */
	inline int GetNextNode() const{ return pNextNode; }
	
	/** Index of next unused SSBO node and advances index by one. */
	int NextNode();
	
	/** Reference to next unused SSBO node and advances index by one. */
	sCSNode &NextNodeRef();
	
	/** Advance index of next unused SSBO data. */
	void AdvanceNextData( int amount );
	
	
	
	/** Pointer to SSBO element write buffer or nullptr. */
	inline sCSElement *GetPtrElement() const{ return pPtrElement; }
	
	/** SSBO element write buffer at index or throws exception if not writing. */
	sCSElement &GetElementAt( int index ) const;
	
	/** Index of next unused SSBO element. */
	inline int GetNextElement() const{ return pNextElement; }
	
	/** Index of next unused SSBO element and advances index by one. */
	int NextElement( eCSElementTypes type, const void *link );
	
	/** Reference to next unused SSBO element and advances index by one. */
	sCSElement &NextElementRef( eCSElementTypes type, const void *link );
	
	/** Element link at index. */
	const sElementLink &GetLinkAt( int index ) const;
	/*@}*/
};

#endif
