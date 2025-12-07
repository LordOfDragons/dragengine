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

#ifndef _DEOGLWORLDOCOMPUTE_H_
#define _DEOGLWORLDOCOMPUTE_H_

#include "deoglWorldComputeElement.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/shared/deoglSharedBlockSPB.h"

#include <stdint.h>
#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>

class deoglRWorld;
class deoglWorldComputeElement;



/**
 * World compute.
 */
class deoglWorldCompute : public deObject{
public:
	typedef deTObjectReference<deoglWorldCompute> Ref;
	
	
	/** Element flags. */
	enum eElementFlags{
		eefComponent = 0x1,
		eefBillboard = 0x2,
		eefParticleEmitter = 0x4,
		eefLight = 0x8,
		eefPropFieldCluster = 0x10,
		eefHeightTerrainSectorCluster = 0x20,
		eefDecal = 0x40,
		eefStatic = 0x80,
		eefDynamic = 0x100,
		eefGIStatic = 0x200,
		eefGIDynamic = 0x400
	};
	
	static const int ElementFlagsAllTypes = eefComponent | eefBillboard | eefParticleEmitter
		| eefLight | eefPropFieldCluster | eefHeightTerrainSectorCluster | eefDecal;
	
	static const int ElementFlagsAllDynamics = eefStatic | eefDynamic | eefGIStatic | eefGIDynamic;
	
	static const int ElementFlagsAll = ElementFlagsAllTypes | ElementFlagsAllDynamics;
	
	/** Shader element parameters. */
	enum eShaderParamsElement{
		espeMinExtend,
		espeFlags,
		espeMaxExtend,
		espeUpdateIndex,
		espeLayerMask,
		espeFirstGeometry,
		espeGeometryCount,
		espeLodFactors,
		espeHighestLod,
		espeCullResult,
		espeLodIndex
	};
	
	/** Shader element geometry parameters. */
	enum eShaderParamsElementGeometry{
		espegElement,
		espegLod,
		espegRenderFilter,
		espegSkinTexture,
		espegPipelineBase,
		espegVao,
		espegInstance,
		espegSPBInstance,
		espegTUCs
	};
	
	
	
private:
	struct sClearGeometries{
		int first, count;
	};
	
	deoglRWorld &pWorld;
	
	deoglSPBlockSSBO::Ref pSSBOElements;
	decObjectList pElements;
	
	decObjectList pUpdateElements;
	int pFullUpdateLimit;
	float pFullUpdateFactor;
	int pUpdateElementCount;
	bool pForceFullUpdate;
	
	decObjectList pUpdateElementGeometries;
	int pFullUpdateGeometryLimit;
	float pFullUpdateGeometryFactor;
	int pUpdateElementGeometryCount;
	bool pForceFullUpdateGeometry;
	
	sClearGeometries *pClearGeometries;
	int pClearGeometriesCount;
	int pClearGeometriesSize;
	int pClearGeometryCount;
	
	deoglSPBlockSSBO::Ref pSSBOElementGeometries;
	deoglSharedBlockSPB::Ref pSharedSPBGeometries;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world compute. */
	deoglWorldCompute(deoglRWorld &world);
	
protected:
	/** Clean up world compute. */
	virtual ~deoglWorldCompute();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** World. */
	inline deoglRWorld &GetWorld() const{return pWorld;}
	
	
	
	/** Prepare. */
	void Prepare();
	void PrepareGeometries();
	
	
	
	/** SSBO elements. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOElements() const{return pSSBOElements;}
	
	/** Count of elements. */
	int GetElementCount() const;
	
	/** Element at index. */
	deoglWorldComputeElement &GetElementAt(int index) const;
	
	/** Add element. */
	void AddElement(deoglWorldComputeElement *element);
	
	/** Update element. */
	void UpdateElement(deoglWorldComputeElement *element);
	
	/** Remove element. */
	void RemoveElement(deoglWorldComputeElement *element);
	
	/** Update element count. */
	inline int GetUpdateElementCount() const{return pUpdateElementCount;}
	
	/** Count of element geometries. */
	int GetElementGeometryCount() const;
	
	/** Update element geometries. */
	void UpdateElementGeometries(deoglWorldComputeElement *element);
	
	/** Update element geometry count. */
	inline int GetUpdateElementGeometryCount() const{return pUpdateElementGeometryCount;}
	
	/** Clear element geometry count. */
	inline int GetClearGeometryCount() const{return pClearGeometryCount;}
	
	
	
	/** SSBO element geometries. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOElementGeometries() const{return pSSBOElementGeometries;}
	
	/** Shared SPB element geometries. */
	inline const deoglSharedBlockSPB::Ref &GetSharedSPBGeometries() const{return pSharedSPBGeometries;}
	/*@}*/
	
	
	
private:
	void pUpdateSSBOElements();
	void pFullUpdateSSBOElements();
	void pUpdateSSBOElement(deoglWorldComputeElement &element, deoglWorldComputeElement::sDataElement &data);
	void pCheckElementGeometryCount(deoglWorldComputeElement &element, deoglWorldComputeElement::sDataElement &data);
	
	void pUpdateSSBOElementGeometries();
	void pFullUpdateSSBOElementGeometries();
	
	void pUpdateSSBOClearGeometries();
	
	void pUpdateFullUpdateLimits();
	void pUpdateFullUpdateGeometryLimits();
	
	void pDebugPrintElements();
	void pDebugPrintUpdateElements();
	void pDebugPrintUpdateGeometries();
	void pDebugPrintClearGeometries();
};

#endif
