/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLFRSHADOWMANAGER_H_
#define _DEOGLFRSHADOWMANAGER_H_

#include "../deoglBasics.h"
#include "../light/deoglRLight.h"
#include "../texture/arraycubemap/deoglArrayCubeMap.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../shaders/paramblock/deoglSPBParameter.h"

#include <dragengine/deTObjectReference.h>
#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglShadowCaster;
class deoglRenderPlan;
class deoglRenderPlanLight;
class deoglRenderPlanSkyLight;
class deoglTextureStageManager;
class deoglShaderParameterBlock;


/**
 * Forward rendering shadow texture manager.
 * 
 * Manages shared shadow array textures used during low fill rate forward rendering.
 * One 2D depth array texture with 16 layers is used for spot lights and one cube map
 * array with 16 layers is used for point lights. Layers 0-7 store static shadow maps
 * and layers 8-15 store dynamic shadow maps. Up to 8 spot and 8 point lights can thus
 * be rendered with shadows simultaneously.
 */
class deoglFRShadowManager{
public:
	/** Count of shadow slots per texture type. */
	static constexpr int SlotCount = 8;
	
	/** Layer count per light: static, dynamic. */
	static constexpr int LayerPerLight = 2;
	
	/** Maximum count of SSBO lights: sky + 8 spot/point. */
	static constexpr int MaxLights = 1 + SlotCount;
	
	/** sFRLight SSBO parameters. */
	enum eFRLightParam{
		eflpType,             //!< int: light type (LightMode*)
		eflpArrayIndex,       //!< int: shadow array index
		eflpPcfMode,          //!< int: PcfMode*
		eflpHasTextureShadow, //!< bvec2: x=shadow1.solid, y=shadow2.solid
		eflpLight,            //!< sFRLightParameters: light
		eflpInstance,         //!< sFRInstanceParameters: instance
	};
	
	
private:
	deoglRenderThread &pRenderThread;
	deTUniqueReference<deoglArrayTexture> pSpotShadowTexture;
	deTUniqueReference<deoglArrayCubeMap> pPointShadowTexture;
	int pSpotShadowSize, pPointShadowSize;
	
	deoglRenderPlanSkyLight *pSkyLight;
	deoglRLight::Ref pLightSlot[SlotCount], pSpotSlot[SlotCount], pPointSlot[SlotCount];
	deoglRenderPlanLight *pLightSlotPlans[SlotCount];
	int pLightCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create forward rendering shadow array manager. */
	explicit deoglFRShadowManager(deoglRenderThread &renderThread);
	
	/** Clean up forward rendering shadow array manager. */
	~deoglFRShadowManager();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Spot shadow 2D array texture. */
	const deTUniqueReference<deoglArrayTexture> &GetSpotShadowArray();
	
	/** Point shadow cube map array texture. */
	const deTUniqueReference<deoglArrayCubeMap> &GetPointShadowArray();
	
	/** Create light parameters SSBO shader parameters. */
	static void SetSPBParameterLightParams(deoglSPBParameter &parameter);
	
	/**
	 * Collects first sky light and up to 8 spot/point lights from render plan and assigns
	 * shadow array layers.
	 */
	void PrepareForRender(deoglRenderPlan &plan);
	
	/** Light count. */
	int GetLightCount() const { return pLightCount; }
	/*@}*/
	
	
private:
	void pCreateSpotShadowArray();
	void pCreatePointShadowArray();
};

#endif
