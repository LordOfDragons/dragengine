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

#ifndef _DEOGLWORLDOCOMPUTEELEMENT_H_
#define _DEOGLWORLDOCOMPUTEELEMENT_H_

#include "../shaders/paramblock/shared/deoglSharedBlockSPBElement.h"
#include "../skin/pipeline/deoglSkinTexturePipelinesList.h"

#include <stdint.h>
#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>


class deoglWorldCompute;
class decLayerMask;
class deoglSkinTexture;
class deoglTexUnitsConfig;
class deoglVAO;
class deoglRenderTaskSharedInstance;


/**
 * World compute element.
 */
class deoglWorldComputeElement : public deObject{
public:
	typedef deTObjectReference<deoglWorldComputeElement> Ref;
	
	/** Compute shader element types. */
	enum eElementTypes{
		eetComponent,
		eetBillboard,
		eetParticleEmitter,
		eetLight,
		eetPropFieldCluster,
		eetHeightTerrainSectorCluster,
		eetDecal
	};
	
	/** Data element. */
	struct sDataElement{
		float minExtendX, minExtendY, minExtendZ;
		uint32_t flags;
		
		float maxExtendX, maxExtendY, maxExtendZ;
		uint32_t updateIndex; // for use by updating only
		
		uint32_t layerMask[ 2 ]; // 0=upper 32 bits, 1=lower 32 bits
		uint32_t firstGeometry;
		uint32_t geometryCount;
		
		float lodFactors[ 4 ];
		uint32_t highestLod;
		
		uint32_t cullResult;
		uint32_t lodIndex;
		
		uint32_t padding;
		
		void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
		void SetLayerMask( const decLayerMask &layerMask );
		void SetEmptyLayerMask();
	};
	
	/** Data element geometry. */
	struct sDataElementGeometry{
		uint32_t element;
		uint32_t lod;
		uint32_t renderFilter;
		uint32_t skinTexture;
		uint32_t pipelineBase;
		uint32_t vao;
		uint32_t instance;
		uint32_t spbInstance;
		uint32_t tucs[ 4 ];
	};
	
	/** TUC information. */
	struct sInfoTUC{
		const deoglTexUnitsConfig *geometry;
		const deoglTexUnitsConfig *depth;
		const deoglTexUnitsConfig *counter;
		const deoglTexUnitsConfig *shadow;
		const deoglTexUnitsConfig *shadowCube;
		const deoglTexUnitsConfig *envMap;
		const deoglTexUnitsConfig *luminance;
		const deoglTexUnitsConfig *giMaterial;
		sInfoTUC();
	};
	
	static const int RenderFilterOutline = ertfOutline | ertfOutlineSolid;
	
	
	
private:
	const eElementTypes pType;
	const void * const pOwner;
	deoglWorldCompute *pWorldCompute;
	int pIndex;
	bool pUpdateRequired;
	bool pUpdateGeometriesRequired;
	deoglSharedBlockSPBElement::Ref pSPBGeometries;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world compute element. */
	deoglWorldComputeElement( eElementTypes type, const void *owner );
	
protected:
	/** Clean up world compute element. */
	virtual ~deoglWorldComputeElement();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Element type. */
	inline eElementTypes GetType() const{ return pType; }
	
	/** Owner. */
	inline const void *GetOwner() const{ return pOwner; }
	
	/** World compute or nullptr if not added to one. */
	inline deoglWorldCompute *GetWorldCompute() const{ return pWorldCompute; }
	
	/** World compute reference or throws exception if not added to one. */
	deoglWorldCompute &GetWorldComputeRef() const;
	
	/** Index or -1 if not added to a world compute. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** World reference position. Call this only if GetWorldCompute() is not nullptr. */
	const decDVector &GetReferencePosition() const;
	
	/** Update element if added to a world compute. */
	void ComputeUpdateElement();
	
	/** Update element geometries if added to a world compute. */
	void ComputeUpdateElementGeometries();
	
	/** Update element and geometries if added to a world compute. */
	void ComputeUpdateElementAndGeometries();
	
	/** Remove element if added to a world compute. */
	void RemoveFromCompute();
	
	
	
	/** Update data element. */
	virtual void UpdateData( sDataElement &data ) const = 0;
	
	/** Update data element geometries. */
	virtual void UpdateDataGeometries( sDataElementGeometry *data ) const;
	
	
	
	/** Set data element geometry using skin texture. */
	void SetDataGeometry( sDataElementGeometry &data, int lod, int renderFilter,
		deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType,
		int pipelineModifier, const deoglSkinTexture *skinTexture,
		const deoglVAO *vao, const deoglRenderTaskSharedInstance *instance, int spbInstance ) const;
	
	/** Set data element geometry TUCs. */
	void SetDataGeometryTUCs( sDataElementGeometry &data, const sInfoTUC &info ) const;
	
	/** Set data element geometry using no texture. */
	void SetDataGeometry( sDataElementGeometry &data, int renderFilter, const deoglVAO *vao,
		const deoglRenderTaskSharedInstance *instance, int spbInstance ) const;
	/*@}*/
	
	
	
	/** \name deoglWorldCompute */
	/*@{*/
	/** Set world compute. For use by deoglWorldCompute only. */
	void SetWorldCompute( deoglWorldCompute *worldCompute );
	
	/** Set index. For use by deoglWorldCompute only. */
	void SetIndex( int index );
	
	/** Update required. For use by deoglWorldCompute only. */
	inline bool GetUpdateRequired() const{ return pUpdateRequired; }
	
	/** Set if update is required. For use by deoglWorldCompute only. */
	void SetUpdateRequired( bool updateRequired );
	
	/** Update geometries required. For use by deoglWorldCompute only. */
	inline bool GetUpdateGeometriesRequired() const{ return pUpdateGeometriesRequired; }
	
	/** Set if update geometries is required. For use by deoglWorldCompute only. */
	void SetUpdateGeometriesRequired( bool updateRequired );
	
	/** Shader parameter block for geometries. For use by deoglWorldCompute only. */
	inline deoglSharedBlockSPBElement::Ref &GetSPBGeometries(){ return pSPBGeometries; }
	inline const deoglSharedBlockSPBElement::Ref &GetSPBGeometries() const{ return pSPBGeometries; }
	/*@}*/
};

#endif
