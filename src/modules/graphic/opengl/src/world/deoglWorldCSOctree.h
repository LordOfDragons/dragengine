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
	
	/** Compute shader element types. */
	enum eCSElementTypes{
		ecsetStaticComponent = 0,
		ecsetDynamicComponent = 1,
		ecsetBillboard = 2,
		ecsetParticleEmitter = 3,
		ecsetLight = 4
	};
	
	/** Compute shader data. Aligned to (u)vec4[3]. */
	struct sCSData{
		float minExtendX, minExtendY, minExtendZ, reserved1;
		float maxExtendX, maxExtendY, maxExtendZ, reserved2;
		uint32_t data1; // firstNode or elementIndex
		uint32_t data2; // 28-bits(elementCount), 4-bits(childNodeCount)
		                // or elementType
		uint32_t layerMaskUpper, layerMaskLower; // 64-bit layer mask
		
		void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
		void SetLayerMask( const decLayerMask &layerMask );
	};
	
	/** Link. */
	struct sElementLink{
		eCSElementTypes type;
		const void *element;
	};
	
	
private:
	deoglRenderThread &pRenderThread;
	decDVector pReferencePosition;
	
	deoglSPBlockSSBO::Ref pSSBOData;
	sCSData *pPtrData;
	
// 	deoglSPBlockSSBO::Ref pSSBOResult;
	
	int pNodeCount;
	int pElementCount;
	
	int pNextData;
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
	
	
	
	/** SSBO compute shader data. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOData() const{ return pSSBOData; }
	
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
	
	
	
	/** Pointer to SSBO data write buffer or nullptr. */
	inline sCSData *GetPtrData() const{ return pPtrData; }
	
	/** SSBO data write buffer at index or throws exception if not writing. */
	sCSData &GetDataAt( int index ) const;
	
	/** Index of next unused SSBO data. */
	inline int GetNextData() const{ return pNextData; }
	
	/** Index of next unused SSBO data and advances index by one. */
	int NextData();
	
	/** Reference to next unused SSBO data and advances index by one. */
	sCSData &NextDataRef();
	
	/** Advance index of next unused SSBO data. */
	void AdvanceNextData( int amount );
	
	/** Index of next unused SSBO element. */
	inline int GetNextElement() const{ return pNextElement; }
	
	/** Index of next unused SSBO element and advances index by one. */
	int NextElement( eCSElementTypes type, const void *link );
	
	/** Element link at index. */
	const sElementLink &GetLinkAt( int index ) const;
	/*@}*/
};

#endif
