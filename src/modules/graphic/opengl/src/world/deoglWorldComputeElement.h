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
