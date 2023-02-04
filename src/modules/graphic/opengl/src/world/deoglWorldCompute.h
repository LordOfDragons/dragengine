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

#ifndef _DEOGLWORLDOCOMPUTE_H_
#define _DEOGLWORLDOCOMPUTE_H_

#include "../shaders/paramblock/deoglSPBlockSSBO.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>

class deoglRWorld;
class decLayerMask;


/**
 * World compute.
 */
class deoglWorldCompute : public deObject{
public:
	typedef deTObjectReference<deoglWorldCompute> Ref;
	
	/** Element flags. */
	enum eElementFlags{
		eefComponent = 0x1,
		eefComponentDynamic = 0x2,
		eefBillboard = 0x4,
		eefParticleEmitter = 0x8,
		eefLight = 0x10,
		eefPropField = 0x20,
		eefHTSector = 0x40
	};
	
	/** Shader element parameters. */
	enum eShaderParamsElement{
		espeMinExtend,
		espeFlags,
		espeMaxExtend,
		espeUpdateIndex,
		espeLayerMask,
	};
	
	/** Data element. */
	struct sDataElement{
		float minExtendX, minExtendY, minExtendZ;
		uint32_t flags;
		
		float maxExtendX, maxExtendY, maxExtendZ;
		uint32_t updateIndex; // for use by updating only
		
		uint32_t layerMask[ 2 ]; // 0=upper 32 bits, 1=lower 32 bits
		uint32_t padding[ 2 ];
		
		void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
		void SetLayerMask( const decLayerMask &layerMask );
		void SetEmptyLayerMask();
	};
	
	/** Compute shader element types. */
	enum eElementTypes{
		eetComponent,
		eetBillboard,
		eetParticleEmitter,
		eetLight,
		eetPropField,
		eetHTSector
	};
	
	/** Element. */
	class Element : public deObject{
	public:
		typedef deTObjectReference<Element> Ref;
		
	private:
		const eElementTypes pType;
		const void * const pOwner;
		int pIndex;
		bool pUpdateRequired;
		
	public:
		/** Create element. */
		Element( eElementTypes type, const void *owner );
		
	protected:
		/** Clean up element. */
		virtual ~Element();
		
	public:
		/** Element type. */
		inline eElementTypes GetType() const{ return pType; }
		
		/** Owner. */
		inline const void *GetOwner() const{ return pOwner; }
		
		/** Index. */
		inline int GetIndex() const{ return pIndex; }
		
		/** Set index. For use by deoglWorldCompute only. */
		void SetIndex( int index );
		
		/** Update required. For use by deoglWorldCompute only. */
		inline bool GetUpdateRequired() const{ return pUpdateRequired; }
		
		/** Set if update is required. For use by deoglWorldCompute only. */
		void SetUpdateRequired( bool updateRequired );
		
		/** Update data element. */
		virtual void UpdateData( const deoglWorldCompute &worldCompute, sDataElement &data ) = 0;
	};
	
	
	
private:
	deoglRWorld &pWorld;
	
	deoglSPBlockSSBO::Ref pSSBOElements;
	decObjectList pElements;
	
	decObjectList pUpdateElements;
	int pFullUpdateLimit;
	float pFullUpdateFactor;
	int pUpdateElementCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world compute. */
	deoglWorldCompute( deoglRWorld &world );
	
protected:
	/** Clean up world compute. */
	virtual ~deoglWorldCompute();
	/*@}*/
	
	
	
public:
	/** \name Visiting */
	/*@{*/
	/** World. */
	inline deoglRWorld &GetWorld() const{ return pWorld; }
	
	
	
	/** Prepare. */
	void Prepare();
	
	
	
	/** SSBO elements. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOElements() const{ return pSSBOElements; }
	
	/** Count of elements. */
	int GetElementCount() const;
	
	/** Element at index. */
	Element &GetElementAt( int index ) const;
	
	/** Add element. */
	void AddElement( Element *element );
	
	/** Update element. */
	void UpdateElement( Element *element );
	
	/** Remove element. */
	void RemoveElement( Element *element );
	
	/** Update element count. */
	inline int GetUpdateElementCount() const{ return pUpdateElementCount; }
	/*@}*/
	
	
	
private:
	void pUpdateSSBOElements();
	void pFullUpdateSSBOElements();
	void pUpdateFullUpdateLimit();
};

#endif
