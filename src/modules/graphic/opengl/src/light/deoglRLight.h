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

#ifndef _DEOGLRLIGHT_H_
#define _DEOGLRLIGHT_H_

#include "pipeline/deoglLightPipelines.h"
#include "../component/deoglComponentSet.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../world/deoglWorldComputeElement.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/light/deLight.h>

class deoglCollideList;
class deoglLightShaderConfig;
class deoglLightVolume;
class deoglOcclusionTest;
class deoglOptimizerLight;
class deoglRCanvasView;
class deoglRComponent;
class deoglRDynamicSkin;
class deoglRenderPlan;
class deoglRenderPlanMasked;
class deoglRenderThread;
class deoglRSkin;
class deoglRWorld;
class deoglShadowCaster;
class deoglSkinState;
class deoglSkinTexture;
class deoglWorldOctree;

class decConvexVolumeList;
class deoglDCollisionVolume;
class decShapeBox;



/**
 * Render light.
 */
class deoglRLight : public deObject{
private:
	/** World compute element. */
	class WorldComputeElement: public deoglWorldComputeElement{
		deoglRLight &pLight;
	public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRLight> Ref;


		WorldComputeElement(deoglRLight &light);
		void UpdateData(sDataElement &data) const override;
	};
	
	
	
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	deoglWorldComputeElement::Ref pWorldComputeElement;
	
	bool pActive;
	deLight::eLightTypes pLightType;
	bool pCastShadows;
	float pSpotAngle;
	float pSpotRatio;
	float pSpotSmoothness;
	float pSpotExponent;
	deLight::eMovementHints pHintMovement;
	int pHintShadowImportance;
	float pIntensity;
	float pAmbientRatio;
	decColor pColor;
	decLayerMask pLayerMask;
	decLayerMask pLayerMaskShadow;
	decObjectSet pShadowIgnoreComponents;
	
	deoglRSkin::Ref pLightSkin;
	deoglRCanvasView::Ref pLightCanvas;
	deoglRDynamicSkin::Ref pDynamicSkin;
	decTexMatrix2 pTransform;
	
	deoglSkinState *pSkinState;
	deoglSkinTexture *pUseSkinTexture;
	bool pDirtyPrepareSkinStateRenderables;
	bool pDirtyRenderSkinStateRenderables;
	
	bool pDirtyPrepareLightCanvas;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	decDVector pFullMinExtend;
	decDVector pFullMaxExtend;
	bool pDirtyExtends;
	bool pDirtyFullExtends;
	
	float pAttenCoeff;
	float pRange;
	float pDampCoeff;
	float pDampThreshold;
	
	decConvexVolumeList *pConvexVolumeList;
	deoglOptimizerLight *pOptimizer;
	
	deoglShadowCaster *pShadowCaster;
	bool pDirtyStaticShadows;
	bool pDirtyDynamicShadows;
	
	deoglComponentSet pStaticComponentList;
	deoglComponentSet pDynamicComponentList;
	deoglCollideList *pStaticCollideList;
	deoglCollideList *pDynamicCollideList;
	bool pDirtyCollideLists;
	
	decDMatrix pMatrix;
	decDMatrix pInverseMatrix;
	
	deoglLightVolume *pLightVolume;
	decShapeBox *pLightVolumeCropBox;
	bool pDirtyConvexVolumeList;
	
	decVector2 pTrackAngles;
	
	deoglDCollisionVolume *pColVol;
	bool pDirtyColVol;
	
	bool pDirtyTouching;
	
	bool pMarked;
	
	bool pUpdateOnRemoveComponent;
	
	deoglLightPipelines::Ref pPipelines;
	deoglSPBlockUBO::Ref pParamBlockLight;
	deoglSPBlockUBO::Ref pParamBlockInstance;
	deoglSPBlockUBO::Ref pParamBlockOccQuery;
	
	uint32_t pCSOctreeIndex;
	
	bool pWorldMarkedRemove;
	
	deoglRLight *pLLWorldPrev;
	deoglRLight *pLLWorldNext;
	
	decPointerLinkedList::cListEntry pLLPrepareForRenderWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render light. */
	deoglRLight(deoglRenderThread &renderThread);
	
	/** Clean up render light. */
	virtual ~deoglRLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL if not in a world. */
	void SetParentWorld(deoglRWorld *parentWorld);
	
	/** Octree node or \em NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * Set octree node or \em NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by the deoglWorldOctree only.
	 */
	void SetOctreeNode(deoglWorldOctree *octreeNode);
	
	/** Update octree node. */
	void UpdateOctreeNode();
	
	/** Update light skin if used. */
	void UpdateSkin(float elapsed);
	
	
	
	/** Light is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** Set light is active. */
	void SetActive(bool active);
	
	/** Light type. */
	inline deLight::eLightTypes GetLightType() const{ return pLightType; }
	
	/** Set light type. */
	void SetLightType(deLight::eLightTypes type);
	
	/** Light casts shadows. */
	inline bool GetCastShadows() const{ return pCastShadows; }
	
	/** Set if light casts shadows. */
	void SetCastShadows(bool castShadows);
	
	/** Outer spot Angle. */
	inline float GetSpotAngle() const{ return pSpotAngle; }
	
	/** Set outer spot angle. */
	void SetSpotAngle(float angle);
	
	/** Spot ratio as height divided by width. */
	inline float GetSpotRatio() const{ return pSpotRatio; }
	
	/** Set spot ratio as height divided by width. */
	void SetSpotRatio(float ratio);
	
	/** Spot smoothness. */
	inline float GetSpotSmoothness() const{ return pSpotSmoothness; }
	
	/** Set spot smoothness. */
	void SetSpotSmoothness(float smoothness);
	
	/** Spot exponent. */
	inline float GetSpotExponent() const{ return pSpotExponent; }
	
	/** Set spot exponent. */
	void SetSpotExponent(float exponent);
	
	/** Movement hint. */
	inline deLight::eMovementHints GetHintMovement() const{ return pHintMovement; }
	
	/** Set movement hint. */
	void SetHintMovement(deLight::eMovementHints movement);
	
	/** Shadow importance. */
	inline int GetHintShadowImportance() const{ return pHintShadowImportance; }
	
	/** Set shadow importance. */
	void SetHintShadowImportance(int importance);
	
	/** Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** Set intensity. */
	void SetIntensity(float intensity);
	
	/** Ambient intensity ratio. */
	inline float GetAmbientRatio() const{ return pAmbientRatio; }
	
	/** Set ambient intensity ratio. */
	void SetAmbientRatio(float ratio);
	
	/** Light canvas or \em NULL if not used. */
	inline const deoglRCanvasView::Ref &GetLightCanvas() const{ return pLightCanvas; }
	
	/** Set light canvas or \em NULL if not used. */
	void SetLightCanvas(deoglRCanvasView *canvas);
	
	/** Light color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Set light color. */
	void SetColor(const decColor &color);
	
	/** Camera layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set camera layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** Shadow layer mask. */
	inline const decLayerMask &GetLayerMaskShadow() const{ return pLayerMaskShadow; }
	
	/** Set shadow layer mask. */
	void SetLayerMaskShadow(const decLayerMask &layerMask);
	
	/** Remove all shadow ignore components. */
	void RemoveAllShadowIgnoreComponents();
	
	/** Add shadow ignore component. */
	void AddShadowIgnoreComponent(deoglRComponent *component);
	
	/** Has shadow ignore component. */
	bool HasShadowIgnoreComponent(deoglRComponent *component) const;
	
	/**
	 * Check if camera layer mask matches light/shadow layer masks.
	 * 
	 * Static shadow maps are only valid if this method returns true.
	 */
	bool StaticMatchesCamera(const decLayerMask &layerMask) const;
	
	
	
	/** Light skin or \em NULL if not used. */
	inline const deoglRSkin::Ref &GetLightSkin() const{ return pLightSkin; }
	
	/** Set light skin or \em NULL if not used. */
	void SetLightSkin(deoglRSkin *skin);
	
	/** Dynamic skin or \em NULL if not used. */
	inline const deoglRDynamicSkin::Ref &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or \em NULL if not used. */
	void SetDynamicSkin(deoglRDynamicSkin *dynamicSkin);
	
	/** Light texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set light texture coordinate transformation matrix. */
	void SetTransform(const decTexMatrix2 &matrix);
	
	/** Skin state or \em NULL if not used. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** Skin texture to use or \em NULL. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	void InitSkinStateStates();
	void UpdateSkinStateStates();
	
	void DirtyPrepareSkinStateRenderables();
	void PrepareSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	void RenderSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	void PrepareSkinStateConstructed();
	
	void DynamicSkinRenderablesChanged();
	void UpdateRenderableMapping();
	
	void DirtyPrepareLightCanvas();
	
	
	
	/** Transformation matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Inverse transformation matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	
	/** Set matrices. */
	void SetMatrix(const decDMatrix &matrix);
	
	
	
	/** Full minimum extend. */
	inline const decDVector &GetFullMinExtend() const{ return pFullMinExtend; }
	
	/** Full maximum extend. */
	inline const decDVector &GetFullMaxExtend() const{ return pFullMaxExtend; }
	
	/** Mark full extends dirty. */
	void SetDirtyFullExtends();
	
	/** Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Mark extends dirty. */
	void SetDirtyExtends();
	
	
	
	/** Marked. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** Set marked. */
	inline void SetMarked(bool marked){pMarked = marked;}
	
	
	
	/** Convex light volume. */
	inline decConvexVolumeList *GetConvexVolumeList() const{ return pConvexVolumeList; }
	
	/** Has extends. */
	bool HasExtends() const;
	
	/** Collision volume. */
	inline deoglDCollisionVolume *GetCollisionVolume() const{ return pColVol; }
	
	/** Mark collision volume dirty. */
	void SetDirtyCollisionVolume();
	
	
	
	/** Static component list. */
	inline deoglComponentSet &GetStaticComponentList(){ return pStaticComponentList; }
	inline const deoglComponentSet &GetStaticComponentList() const{ return pStaticComponentList; }
	
	/** Dynamic component list. */
	inline deoglComponentSet &GetDynamicComponentList(){ return pDynamicComponentList; }
	inline const deoglComponentSet &GetDynamicComponentList() const{ return pDynamicComponentList; }
	
	/**
	 * Static collide list.
	 * \details Updates it if not updated already.
	 */
	const deoglCollideList *GetStaticCollideList();
	
	/**
	 * Retrieves the dynamic collide list.
	 * \details Updates it if not updated already.
	 */
	const deoglCollideList *GetDynamicCollideList();
	
	/** Mark collide lists dirty. */
	void SetDirtyCollideLists();
	
	
	
	/** Mark light volume dirty. */
	void SetLightVolumeDirty();
	
	/** Light volume. */
	inline deoglLightVolume *GetLightVolume() const{ return pLightVolume; }
	
	/** Light volume crop box or \em NULL if not set. */
	inline decShapeBox *GetLightVolumeCropBox() const{ return pLightVolumeCropBox; }
	
	/** Set light volume crop box or NULL if not set. */
	void SetLightVolumeCropBox(decShapeBox *box);
	
	
	
	/** Shadow caster. Update first if dirty. */
	deoglShadowCaster *GetShadowCaster();
	
	/** Set static and dynamic shadows dirty. */
	void SetDirtyShadows();
	
	/** Shadow caster requires neeed future preparing. */
	void ShadowCasterRequiresPrepare();
	
	
	
	
	/** Range. */
	inline float GetRange() const{ return pRange; }
	
	/** Attenuation coefficient. */
	inline float GetAttenuationCoefficient() const{ return pAttenCoeff; }
	
	/** Damping coefficient. */
	inline float GetDampingCoefficient() const{ return pDampCoeff; }
	
	/** Damping threshold. */
	inline float GetDampingThreshold() const{ return pDampThreshold; }
	
	/** Update attenuation parameters. */
	void UpdateAttenuation(float range, float halfIntensityDistance);
	
	
	
	/** Clear registered optimizer. */
	void ClearOptimizer();
	
	
	
	/**
	 * Does preparations required to allow parallel tasks in render plan to run while the
	 * PrepareForRender runs meanwhile on the render thread
	 */
	void EarlyPrepareForRender();
	
	/** Prepare for rendering. */
	void PrepareForRender(const deoglRenderPlanMasked *renderPlanMask);
	
	/** Prepare for rendering render. */
	void PrepareForRenderRender(const deoglRenderPlanMasked *renderPlanMask);
	
	
	
	/** Pipelines. */
	deoglLightPipelines &GetPipelines();
	
	/** Light parameter block. */
	const deoglSPBlockUBO::Ref &GetLightParameterBlock();
	
	/** Instance parameter block. */
	const deoglSPBlockUBO::Ref &GetInstanceParameterBlock();
	
	/** Occlusion query parameter block. */
	const deoglSPBlockUBO::Ref &GetOccQueryParameterBlock();
	
	/** Drop all pipelines and parameter blocks. */
	void DropPipelines();
	
	
	
	/** Set touching dirty. */
	void SetDirtyTouching();
	
	/** Notifiy env maps in the world about light changes. */
	void EnvMapNotifyLightChanged();
	
	
	
	/** Prepare for quick disposal of light. */
	void PrepareQuickDispose();
	
	/** Compute shader octree index. */
	inline uint32_t GetCSOctreeIndex() const{ return pCSOctreeIndex; }
	void SetCSOctreeIndex(uint32_t index){pCSOctreeIndex = index;}
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** Add a component. */
	void AddComponent(deoglRComponent *component);
	
	/** Remove a component. */
	void RemoveComponent(deoglRComponent *component);
	
	/** Remove all components. */
	void RemoveAllComponents();
	
	/**
	 * Test component against this light.
	 * \details If the component is in contact with the bounding box it is added to the
	 *          list of touching component if not already listed. If not touching the
	 *          component is removed if listed.
	 */
	void TestComponent(deoglRComponent *oglComponent);
	/*@}*/
	
	
	
	/** \name Optimizations */
	/*@{*/
	/** Light volume has been improved. */
	void LightVolumeImproved();
	
	/** Replace light volume with an improved version. */
	void ReplaceLightVolume(decConvexVolumeList *list);
	
	/** Replace shadow caster with an improved version. */
	void ReplaceShadowCaster(deoglShadowCaster *shadowCaster);
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove(bool marked);
	
	
	
	inline deoglRLight *GetLLWorldPrev() const{ return pLLWorldPrev; }
	void SetLLWorldPrev(deoglRLight *light);
	inline deoglRLight *GetLLWorldNext() const{ return pLLWorldNext; }
	void SetLLWorldNext(deoglRLight *light);
	
	inline decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld(){ return pLLPrepareForRenderWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld() const{ return pLLPrepareForRenderWorld; }
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateFullExtends();
	void pUpdateExtends();
	void pUpdateCollideLists();
	void pCheckTouching();
	void pUpdateCollisionVolume();
	
	/** \note Can be called indirectly from main thread during synchronization. */
	void pUpdateLightVolume();
	
	void pRequiresPrepareForRender();
};

#endif
