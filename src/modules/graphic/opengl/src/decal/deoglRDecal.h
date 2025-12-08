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

#ifndef _DEOGLRDECAL_H_
#define _DEOGLRDECAL_H_

#include "../skin/deoglSkinTexture.h"
#include "../skin/pipeline/deoglSkinTexturePipelines.h"
#include "../world/deoglWorldComputeElement.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decPointerLinkedList.h>

class deoglGIBVHLocal;
class deoglGIBVHDynamic;
class deoglRenderPlan;
class deoglRComponent;
class deoglRDynamicSkin;
class deoglRenderThread;
class deoglRSkin;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglSharedVBOBlock;
class deoglSharedSPBElement;
class deoglShaderParameterBlock;
class deoglSkinShader;
class deoglSkinState;
class deoglTexUnitsConfig;
class deoglVAO;
class deoglRenderTaskSharedInstance;
class deoglRenderPlanMasked;
class deoglDecalListener;



/**
 * Render decal.
 */
class deoglRDecal : public deObject{
private:
	/** World compute element. */
	class WorldComputeElement: public deoglWorldComputeElement{
		deoglRDecal &pDecal;
	public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRDecal> Ref;


		WorldComputeElement(deoglRDecal &decal);
		void UpdateData(sDataElement &data) const override;
		void UpdateDataGeometries(sDataElementGeometry *data) const override;
	};
	
	
	
	deoglRenderThread &pRenderThread;
	
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pSize;
	decTexMatrix2 pTransform;
	bool pVisible;
	
	deoglRSkin::Ref pSkin;
	deoglRDynamicSkin::Ref pDynamicSkin;
	deoglSkinState *pSkinState;
	
	deoglRSkin *pUseSkin;
	int pUseTextureNumber;
	deoglSkinTexture *pUseSkinTexture;
	deoglRDynamicSkin *pUseDynamicSkin;
	deoglSkinState *pUseSkinState;
	
	bool pDirtyPrepareSkinStateRenderables;
	bool pDirtyRenderSkinStateRenderables;
	
	deoglSharedVBOBlock::Ref pVBOBlock;
	int pPointCount;
	
	bool pDirtyVBO;
	bool pDirtyUseTexture;
	
	deoglRComponent *pParentComponent;
	bool pComponentMarkedRemove;
	deoglWorldComputeElement::Ref pWorldComputeElement;
	
	deoglSharedSPBElement::Ref pSharedSPBElement;
	deoglRenderTaskSharedInstance *pRTSInstance;
	
	deoglTexUnitsConfig *pTUCGeometry, *pTUCDepth, *pTUCCounter, *pTUCShadow, *pTUCEnvMap;
	
	bool pDirtySharedSPBElement;
	bool pDirtyTUCs;
	
	deoglGIBVHLocal *pGIBVHLocal;
	deoglGIBVHDynamic *pGIBVHDynamic;
	bool pDirtyGIBVHLocal;
	bool pDirtyGIBVHDynamic;
	bool pStaticTexture;
	
	unsigned int pUniqueKey;
	
	decObjectOrderedSet pListeners;
	int pListenerIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render decal. */
	deoglRDecal(deoglRenderThread &renderThread);
	
	/** Clean up render decal. */
	virtual ~deoglRDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition(const decVector &position);
	
	/** Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** Set size. */
	void SetSize(const decVector &size);
	
	/** Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set texture coordinate transformation matrix. */
	void SetTransform(const decTexMatrix2 &matrix);
	
	/** Decal is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set decal is visible. */
	void SetVisible(bool visible);
	
	
	
	/** Update skin. */
	void UpdateSkin(float elapsed);
	
	/** Set vbo dirty. */
	void SetDirtyVBO();
	
	
	
	/** Skin or \em NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin or \em NULL if not set. */
	void SetSkin(deoglRSkin *skin);
	
	/** Dynamic skin or \em NULL if not set. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or \em NULL if not set. */
	void SetDynamicSkin(deoglRDynamicSkin *dynamicSkin);
	
	/** Retrieves the skin state or NULL if there is none. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/**
	 * Set skin state or \em NULL if there is none.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetSkinState(deoglSkinState *skinState);
	
	/** Retrieves the actual skin to use. */
	inline deoglRSkin *GetUseSkin() const{ return pUseSkin; }
	/** Retrieves the actual skin texture number to use. */
	inline int GetUseTextureNumber() const{ return pUseTextureNumber; }
	/** Retrieves the actual skin texture to use. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	/** Retrieves the actual dynamic skin to use. */
	inline deoglRDynamicSkin *GetUseDynamicSkin() const{ return pUseDynamicSkin; }
	/** Retrieves the actual skin state to use. */
	inline deoglSkinState *GetUseSkinState() const{ return pUseSkinState; }
	
	/**
	 * Update skin state depending on skin and dynamic skin.
	 * \warning Only call from main thread during synchronization.
	 */
	void UpdateSkinState();
	
	/** Retrieves the vbo block. */
	inline deoglSharedVBOBlock *GetVBOBlock() const{ return pVBOBlock; }
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	
	
	void DirtyPrepareSkinStateRenderables();
	void DynamicSkinRenderablesChanged();
	void UpdateRenderableMapping();
	
	
	
	/** Parent component. */
	inline deoglRComponent *GetParentComponent() const{ return pParentComponent; }
	deoglRComponent &GetParentComponentRef() const;
	
	/**
	 * Set parent component or \em NULL.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetParentComponent(deoglRComponent *component);
	
	bool IsParentComponentSolid() const;
	
	/**
	 * Marked for removal.
	 * \details For use by deoglComponent only. Non-thread safe.
	 */
	inline bool GetComponentMarkedRemove() const{ return pComponentMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglComponent only. Non-thread safe.
	 */
	void SetComponentMarkedRemove(bool marked);
	
	/** Add to world compute. */
	void AddToWorldCompute(deoglWorldCompute &worldCompute);
	
	/** Update world compute. */
	void UpdateWorldCompute();
	
	/** Remove from world compute. */
	void RemoveFromWorldCompute();
	
	
	
	/** Shared shader parameter block element. */
	inline deoglSharedSPBElement *GetSharedSPBElement() const{ return pSharedSPBElement; }
	
	/** Render task shared instance or NULL. */
	inline deoglRenderTaskSharedInstance *GetRTSInstance() const{ return pRTSInstance; }
	
	/** Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForPipelineType(deoglSkinTexturePipelines::eTypes type) const;
	
	/**
	 * Texture units configuration for geometry type shaders or NULL if empty.
	 * This texture units configuration works for the shader type estComponentGeometry.
	 */
	inline deoglTexUnitsConfig *GetTUCGeometry() const{ return pTUCGeometry; }
	
	inline deoglTexUnitsConfig *GetTUCDepth() const{ return pTUCDepth; }
	inline deoglTexUnitsConfig *GetTUCCounter() const{ return pTUCCounter; }
	
	/**
	 * Texture units configuration for shadow type shaders or NULL if empty.
	 * This texture units configuration works for these shader types:
	 * - estComponentShadowProjection
	 * - estComponentShadowOrthogonal
	 * - estComponentShadowOrthogonalCascaded
	 * - estComponentShadowDistance
	 */
	inline deoglTexUnitsConfig *GetTUCShadow() const{ return pTUCShadow; }
	
	/**
	 * Texture units configuration for the environment map shader or NULL if empty.
	 * This texture units configuration works for the shader type estEnvMap.
	 */
	inline deoglTexUnitsConfig *GetTUCEnvMap() const{ return pTUCEnvMap; }
	
	/** Obtain texture units configuration for a shader type. Bare call not to be used directly. */
	deoglTexUnitsConfig *BareGetTUCFor(deoglSkinTexturePipelines::eTypes type) const;
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	
	
	/** Prepare for render. Called by owner deoglRComponent if registered previously. */
	void PrepareForRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	
	/** Prepare for render. Called by owner deoglRComponent if registered previously. */
	void PrepareForRenderRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	
	/** Prepare for quick disposal of decal. */
	void PrepareQuickDispose();
	
	
	
	/** GI Local BVH or NULL. */
	inline deoglGIBVHLocal *GetGIBVHLocal() const{ return pGIBVHLocal; }
	
	/** Prepare GI Local BVH if not build yet. */
	void PrepareGILocalBVH();
	
	/** Set GI BVHs dirty. */
	void SetDirtyGIBVH();
	
	/** Texture is static. */
	inline bool GetStaticTexture() const{ return pStaticTexture; }
	
	/** Update static texture. */
	void UpdateStaticTexture();
	
	
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener(deoglDecalListener *listener);
	
	/** Remove listener if existing. */
	void RemoveListener(deoglDecalListener *listener);
	
	/** Notify all geometry changed. */
	void NotifyGeometryChanged();
	
	/** Notify all decal has been destroyed. */
	void NotifyDecalDestroyed();
	
	/** Notify all texture changed. */
	void NotifyTextureChanged();
	
	/** Notify all TUC changed. */
	void NotifyTUCChanged();
	/*@}*/
	
	
	
private:
	void pCreateMeshComponent();
	void pPrepareVBO();
	void pUpdateUseSkin();
	void pPrepareTUCs();
	void pPrepareParamBlocks();
	void pPrepareSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	void pRenderSkinStateRenderables(const deoglRenderPlanMasked *renderPlanMask);
	void pPrepareSkinStateConstructed();
	void pUpdateRTSInstance();
	void pUpdateInstanceParamBlock(deoglShaderParameterBlock &paramBlock,
		int element, deoglSkinShader &skinShader);
	
	void pRequiresPrepareForRender();
};

#endif
