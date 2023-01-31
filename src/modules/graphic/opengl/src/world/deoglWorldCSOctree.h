/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLWORLDOCSCTREE_H_
#define _DEOGLWORLDOCSCTREE_H_

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
		ecsefBillboard = 0x2,
		ecsefParticleEmitter = 0x4,
		ecsefLight = 0x8,
		ecsefStatic = 0x10
	};
	
	/** Compute shader node. Aligned to (u)vec4[3]. */
	struct sCSNode{
		float minExtendX, minExtendY, minExtendZ, reserved1;
		float maxExtendX, maxExtendY, maxExtendZ, reserved2;
		uint32_t firstNode;
		uint32_t childNodeCount;
		uint32_t elementCount;
		uint32_t reserved3;
		
		void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
	};
	
	/** Compute shader element. Aligned to (u)vec4[3]. */
	struct sCSElement{
		float minExtendX, minExtendY, minExtendZ, reserved1;
		float maxExtendX, maxExtendY, maxExtendZ, reserved2;
		uint32_t elementindex;
		uint32_t flags;
		uint32_t layerMaskUpper, layerMaskLower; // 64-bit layer mask
		
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
