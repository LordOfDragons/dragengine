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

#ifndef _DEOGLRBILLBOARD_H_
#define _DEOGLRBILLBOARD_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/rendered/deoglSkinRendered.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../world/deoglWorldComputeElement.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include "deoglSharedSPBElement.h"
#include "deoglRDynamicSkin.h"
#include "deoglEnvironmentMap.h"
#include "deoglRSkin.h"

class deoglWorldOctree;
class deoglRWorld;
class deoglRenderThread;
class deoglSkinState;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglShaderParameterBlock;
class deoglRenderPlan;
class deoglOcclusionTest;
class deoglRenderPlanMasked;

class deBillboard;



/**
 * Render billboard.
 */
class deoglRBillboard : public deObject, public deoglOcclusionTestListener{
private:
	/** World compute element. */
	class WorldComputeElement: public deoglWorldComputeElement{
		deoglRBillboard &pBillboard;
	public:
		WorldComputeElement(deoglRBillboard &billboard);
		void UpdateData(sDataElement &data) const override;
		void UpdateDataGeometries(sDataElementGeometry *data) const override;
	};
	
	
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	deoglWorldComputeElement::Ref pWorldComputeElement;
	
	deoglRSkin::Ref pSkin;
	deoglSkinTexture *pUseSkinTexture;
	deoglRDynamicSkin::Ref pDynamicSkin;
	
	decDVector pPosition;
	decVector pAxis;
	decVector2 pSize;
	decVector2 pOffset;
	decLayerMask pLayerMask;
	bool pLocked;
	bool pSpherical;
	bool pSizeFixedToScreen;
	bool pVisible;
	
	deoglSkinState *pSkinState;
	deoglSkinRendered pSkinRendered;
	int pSkyShadowSplitMask;
	float pSortDistance;
	bool pOccluded;
	bool pDirtyPrepareSkinStateRenderables;
	bool pDirtyRenderSkinStateRenderables;
	
	deoglEnvironmentMap::Ref pRenderEnvMap;
	deoglEnvironmentMap::Ref pRenderEnvMapFade;
	float pRenderEnvMapFadePerTime;
	float pRenderEnvMapFadeFactor;
	bool pDirtyRenderEnvMap;
	
	deoglSharedSPBElement::Ref pSharedSPBElement;
	deoglSharedSPBRTIGroup::Ref pSharedSPBRTIGroup;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCCounter;
	deoglTexUnitsConfig *pTUCEnvMap;
	
	bool pDirtySharedSPBElement;
	bool pDirtyTUCs;
	
	bool pCubeFaceVisible[6];
	int pSpecialFlags;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	decDVector pCullSphereCenter;
	float pCullSphereRadius;
	bool pDirtyCulling;
	bool pRenderVisible;
	
	bool pMarked;
	
	uint32_t pCSOctreeIndex;
	
	bool pWorldMarkedRemove;
	
	deoglRBillboard *pLLWorldPrev;
	deoglRBillboard *pLLWorldNext;
	
	decPointerLinkedList::cListEntry pLLPrepareForRenderWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render decal. */
	deoglRBillboard(deoglRenderThread &renderThread);
	
	/** Clean up render decal. */
	~deoglRBillboard() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Parent world or NULL if not part of a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world. */
	void SetParentWorld(deoglRWorld *parentWorld);
	
	/** Octree node or NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * Set octree node or NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by deoglWorldOctree.
	 */
	void SetOctreeNode(deoglWorldOctree *octreeNode);
	
	/** Update octree position. */
	void UpdateOctreeNode();
	
	
	
	/** Skin or NULL if not set. */
	inline const deoglRSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** Set skin or NULL if not set. */
	void SetSkin(deoglRSkin *skin);
	
	/** Skin texture to use or NULL if not present. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** Dynamic skin or NULL if not set. */
	inline const deoglRDynamicSkin::Ref &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or NULL if not set. */
	void SetDynamicSkin(deoglRDynamicSkin *dynamicSkin);
	
	/** Update skin. */
	void UpdateSkin(float elapsed);
	
	/** Skin state. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** Init skin state states. */
	void InitSkinStateStates();
	void UpdateSkinStateStates();
	
	/** Skin rendered. */
	inline deoglSkinRendered &GetSkinRendered(){ return pSkinRendered; }
	inline const deoglSkinRendered &GetSkinRendered() const{ return pSkinRendered; }
	
	void DirtyPrepareSkinStateRenderables();
	void PrepareSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	void RenderSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	void PrepareSkinStateConstructed();
	void DynamicSkinRenderablesChanged();
	void UpdateRenderableMapping();
	
	
	
	/** Add plans for renderables in the component if existing and requiring one. */
	void AddSkinStateRenderPlans(deoglRenderPlan &plan);
	
	
	
	/** Shared shader parameter block element. */
	inline const deoglSharedSPBElement::Ref &GetSharedSPBElement() const{ return pSharedSPBElement; }
	
	/** Shared SPB render task instance group. */
	inline deoglSharedSPBRTIGroup &GetSharedSPBRTIGroup() const{ return *pSharedSPBRTIGroup; }
	
	/** Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForPipelineType(deoglSkinTexturePipelines::eTypes type) const;
	
	/**
	 * Texture units configuration for depth type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estBillboardDepth
	 * - deoglSkinTexture::estBillboardDepthClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCDepth() const{ return pTUCDepth; }
	
	/**
	 * Texture units configuration for geometry type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estBillboardGeometry
	 */
	inline deoglTexUnitsConfig *GetTUCGeometry() const{ return pTUCGeometry; }
	
	/**
	 * Texture units configuration for counter type shaders or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estBillboardCounter
	 * - deoglSkinTexture::estBillboardCounterClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCCounter() const{ return pTUCCounter; }
	
	/**
	 * Texture units configuration for the environment map shader or NULL if empty.
	 * 
	 * This texture units configuration works for the shader types:
	 * - deoglSkinTexture::estEnvMap
	 */
	inline deoglTexUnitsConfig *GetTUCEnvMap() const{ return pTUCEnvMap; }
	
	/**
	 * Obtain texture units configuration for a shader type.
	 * \details Bare call not to be used directly.
	 */
	deoglTexUnitsConfig *BareGetTUCFor(deoglSkinTexturePipelines::eTypes type) const;
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/**
	 * Update instance parameter shader parameter block.
	 * 
	 * Parameter block has to be mapped while calling this method.
	 * Caller is responsible to properly unmap in case of exceptions.
	 */
	void UpdateInstanceParamBlock(deoglShaderParameterBlock &paramBlock,
		int element, deoglSkinShader &skinShader);
	
	
	
	/**
	 * Update cube face visibility.
	 * 
	 * Calculates for each face of a cube map if the object is potentially visible.
	 * Used to optimize rendering cube maps by not rendering objects on cube map faces
	 * where the object is not visible.
	 */
	void UpdateCubeFaceVisibility(const decDVector &cubePosition);
	
	/**
	 * Object is visible in cube map face.
	 * 
	 * Only valid after UpdateCubeFaceVisibility() has been called.
	 */
	bool GetCubeFaceVisible(int cubeFace) const;
	
	/**
	 * Special flags for use with render task.
	 * 
	 * Call appropriate flags update method to update the special flags before use.
	 */
	inline int GetSpecialFlags() const{ return pSpecialFlags; }
	
	/** Set special flags from cube map visibility. */
	void SetSpecialFlagsFromFaceVisibility();
	
	
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition(const decDVector &position);
	
	/** Axis. */
	inline const decVector &GetAxis() const{ return pAxis; }
	
	/** Set axis. */
	void SetAxis(const decVector &axis);
	
	/** Size. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** Set size. */
	void SetSize(const decVector2 &size);
	
	/** Offset. */
	inline const decVector2 &GetOffset() const{ return pOffset; }
	
	/** Sets the offset. */
	void SetOffset(const decVector2 &offset);
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** Visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set visible. */
	void SetVisible(bool visible);
	
	/** Billboard is locked. */
	inline bool GetLocked() const{ return pLocked; }
	
	/** Set billboard is locked. */
	void SetLocked(bool locked);
	
	/** Billboard is spherical. */
	inline bool GetSpherical() const{ return pSpherical; }
	
	/** Set if billboard is spherical. */
	void SetSpherical(bool spherical);
	
	/** Billboard size is fixed to the screen size. */
	inline bool GetSizeFixedToScreen() const{ return pSizeFixedToScreen; }
	
	/** Set if billboard size is fixed to the screen size. */
	void SetSizeFixedToScreen(bool sizeFixedToScreen);
	
	
	
	/** Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Update extends. */
	void UpdateExtends(const deBillboard &billboard);
	
	
	
	/**  Cull sphere center. */
	const decDVector &GetCullSphereCenter();
	
	/** Cull sphere radius. */
	float GetCullSphereRadius();
	
	/** Set culling parameters dirty. */
	void SetDirtyCulling();
	
	/** Start occlusion test. */
	void StartOcclusionTest(deoglOcclusionTest &occlusionTest, const decDVector &cameraPosition);
	
	/** Occlusion test finished with a result of invisible for the element. */
	void OcclusionTestInvisible() override;
	
	/** Render visible. */
	inline bool GetRenderVisible() const{ return pRenderVisible; }
	
	/** Set if render visible. */
	void SetRenderVisible(bool visible);
	
	/** Marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** Set marked flag. */
	inline void SetMarked(bool marked){pMarked = marked;}
	
	/** Sort distance. */
	inline float GetSortDistance() const{ return pSortDistance; }
	
	/** Set sort distance. */
	void SetSortDistance(float distance);
	
	/** Occluded. */
	inline bool GetOccluded() const{ return pOccluded; }
	
	/** Set occluded. */
	void SetOccluded(bool occluded);
	
	/** Sky shadow split mask. */
	inline int GetSkyShadowSplitMask() const{ return pSkyShadowSplitMask; }
	
	/** Set sky shadow split mask. */
	void SetSkyShadowSplitMask(int mask);
	
	/** Compute shader octree index. */
	inline uint32_t GetCSOctreeIndex() const{ return pCSOctreeIndex; }
	void SetCSOctreeIndex(uint32_t index){pCSOctreeIndex = index;}
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
	
	/** Render environment map or NULL if not used. */
	inline const deoglEnvironmentMap::Ref &GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** Set render environment map or NULL if not assigned yet. */
	void SetRenderEnvMap(deoglEnvironmentMap *envmap);
	
	/** Fading render environment map or NULL if not used. */
	inline const deoglEnvironmentMap::Ref &GetRenderEnvMapFade() const{ return pRenderEnvMapFade; }
	
	/** Set fading render environment map or NULL if not used. */
	void SetRenderEnvMapFade(deoglEnvironmentMap *envmap);
	
	/** Render environment map fade per time. */
	inline float GetRenderEnvMapFadePerTime() const{ return pRenderEnvMapFadePerTime; }
	
	/** Set render environment map fade per time. */
	void SetRenderEnvMapFadePerTime(float fadePerTime);
	
	/** Render environment map fade factor. */
	inline float GetRenderEnvMapFadeFactor() const{ return pRenderEnvMapFadeFactor; }
	
	/** Set render environment map fade factor. */
	void SetRenderEnvMapFadeFactor(float factor);
	
	/** World environment map layout changed. */
	void WorldEnvMapLayoutChanged();
	
	/** Update render environment map. */
	void UpdateRenderEnvMap();
	
	/** Invalidate render environment map. */
	void InvalidateRenderEnvMap();
	
	/** Invalidate render environment map. */
	void InvalidateRenderEnvMapIf(deoglEnvironmentMap *envmap);
	
	/** The world reference point changed. */
	void WorldReferencePointChanged();
	
	
	
	/** Prepare for render. Called by deoglRWorld if registered previously. */
	void PrepareForRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	
	/** Prepare for render render. Called by deoglRWorld if registered previously. */
	void PrepareForRenderRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	
	/** Prepare for quick disposal of component. */
	void PrepareQuickDispose();
	
	
	
	/** Linked list world previous. */
	inline deoglRBillboard *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** Set linked list world previous. */
	void SetLLWorldPrev(deoglRBillboard *billboard);
	
	/** Linked list world next. */
	inline deoglRBillboard *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** Set linked list world next. */
	void SetLLWorldNext(deoglRBillboard *billboard);
	
	/** World prepare for render linked list. */
	inline decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld(){ return pLLPrepareForRenderWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld() const{ return pLLPrepareForRenderWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateCullSphere();
	void pPrepareTUCs();
	void pPrepareParamBlocks();
	
	void pRequiresPrepareForRender();
};

#endif
