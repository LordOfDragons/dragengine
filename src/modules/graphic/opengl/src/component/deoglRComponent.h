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

#ifndef _DEOGLRCOMPONENT_H_
#define _DEOGLRCOMPONENT_H_

#include "../deoglBasics.h"
#include "../light/deoglLightList.h"
#include "../model/deoglRModel.h"
#include "../skin/rendered/deoglSkinRendered.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../world/deoglWorldComputeElement.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/component/deComponent.h>

class deoglComponent;
class deoglComponentListener;
class deoglDynamicOcclusionMesh;
class deoglEnvironmentMap;
class deoglRCamera;
class deoglRComponentLOD;
class deoglRComponentTexture;
class deoglRDecal;
class deoglRDynamicSkin;
class deoglROcclusionMesh;
class deoglRSkin;
class deoglRWorld;
class deoglRenderPlan;
class deoglRenderThread;
class deoglSPBlockUBO;
class deoglShaderParameterBlock;
class deoglSharedSPBElement;
class deoglSkinState;
class deoglVAO;
class deoglVBOLayout;
class deoglWorldOctree;
class deoglRenderPlanMasked;

class deComponent;


/**
 * Render component.
 */
class deoglRComponent : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRComponent> Ref;


public:
	/** Render modes. */
	enum eRenderModes{
		/** Render as static model. */
		ermStatic,
		
		/** Render as dynamic model. */
		ermDynamic
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	deoglWorldComputeElement::Ref pWorldComputeElement;
	bool pHasEnteredWorld;
	
	
	bool pVisible;
	deComponent::eMovementHints pMovementHint;
	decLayerMask pLayerMask;
	int pGIImportance;
	
	decIntList pModelSkinMappings;
	decIntList pModelRigMappings;
	deoglRModel::Ref pModel;
	deoglRSkin::Ref pSkin;
	deoglRDynamicSkin::Ref pDynamicSkin;
	bool pStaticTextures;
	bool pDirtyModelVBOs;
	
	deoglROcclusionMesh::Ref pOcclusionMesh;
	bool pDirtyOccMeshVBO;
	deoglDynamicOcclusionMesh *pDynamicOcclusionMesh;
	bool pDynOccMeshRequiresPrepareForRender;
	deoglSharedSPBElement::Ref pOccMeshSharedSPBElement;
	bool pValidOccMeshSharedSPBElement;
	bool pDirtyOccMeshSharedSPBElement;
	deoglSharedSPBRTIGroup::Ref pOccMeshSharedSPBDoubleSided;
	deoglSharedSPBRTIGroup::Ref pOccMeshSharedSPBSingleSided;
	
	decObjectList pLODs;
	float pLODErrorScaling;
	bool pDirtyLODVBOs;
	bool pDirtyLODRenderTaskConfigs;
	
	// dynamic model data
	oglMatrix3x4 *pBoneMatrices;
	int pBoneMatrixCount;
	
	// for world
	bool pLit;
	bool pOccluded;
	bool pDirtyModelRigMappings;
	
	bool pFirstRender;
	bool pRenderStatic;
	float pStaticSince;
	
	bool pMarked;
	
	bool pSolid;
	bool pXRaySolid;
	bool pOutlineSolid;
	bool pDirtySolid;
	
	eRenderModes pRenderMode;
	float pSortDistance;
	
	deoglSkinState *pSkinState;
	deoglSkinRendered pSkinRendered;
	bool pDirtyPrepareSkinStateRenderables;
	bool pDirtyRenderSkinStateRenderables;
	
	decObjectList pTextures;
	bool pDirtyTextureTUCs;
	bool pDirtyTextureParamBlocks;
	int pOutlineTextureCount;
	
	decObjectList pDecals;
	bool pDirtyDecals;
	bool pDirtyDecalsRenderRenderables;
	
	decDMatrix pMatrix;
	decDMatrix pInverseMatrix;
	
	decVector pLocalMinExtend;
	decVector pLocalMaxExtend;
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	decDVector pCullSphereCenter;
	float pCullSphereRadius;
	bool pDirtyCulling;
	
	deoglLightList pLightList;
	
	float *pVertexPositionSetWeights;
	int pVertexPositionSetCount;
	
	unsigned int pUniqueKey;
	
	decObjectOrderedSet pListeners;
	int pListenerIndex;
	
	deoglEnvironmentMap::Ref pRenderEnvMap;
	deoglEnvironmentMap::Ref pRenderEnvMapFade;
	float pRenderEnvMapFadePerTime;
	float pRenderEnvMapFadeFactor;
	bool pDirtyRenderEnvMap;
	
	deoglEnvironmentMap::Ref pEnvMap;
	
	uint32_t pCSOctreeIndex;
	
	bool pWorldMarkedRemove;
	
	deoglRComponent *pLLWorldPrev;
	deoglRComponent *pLLWorldNext;
	
	decPointerLinkedList::cListEntry pLLPrepareForRenderWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render component. */
	deoglRComponent(deoglRenderThread &renderThread);
	
	/** Clean up render component. */
	virtual ~deoglRComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Parent world or NULL if not part of a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world. */
	void SetParentWorld(deoglRWorld *parentWorld);
	
	/** Component has entered world. */
	void HasEnteredWorld();
	
	/** Octree node or NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * Set octree node or NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by deoglWorldOctree.
	 */
	void SetOctreeNode(deoglWorldOctree *octreeNode);
	
	/**
	 * Update octree position.
	 * \warning Called during synchronization from main thread.
	 */
	void UpdateOctreeNode();
	
	
	
	/** Component is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set component is visible. */
	void SetVisible(bool visible);
	
	/** Movement hint. */
	inline deComponent::eMovementHints GetMovementHint() const{ return pMovementHint; }
	
	/** Set movement hint. */
	void SetMovementHint(deComponent::eMovementHints hint);
	
	inline int GetGIImportance() const{ return pGIImportance; }
	void SetGIImportance(int importance);
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	
	
	/** Update. */
	void Update(float elapsed);
	
	
	
	/** Add plans for renderables in the component if existing and requiring one. */
	void AddSkinStateRenderPlans(deoglRenderPlan &plan);
	
	
	
	/** Transformation matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Inverse transformation matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	
	/** Set matrices. */
	void SetMatrix(const decDMatrix &matrix);
	
	
	
	/** Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/**
	 * Update extends of dirty.
	 * \warning Requires UpdateBoneMatrices() to be called first.
	 */
	void UpdateExtends(deComponent &component);
	
	
	
	/** Marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** Set marked flag. */
	inline void SetMarked(bool marked){pMarked = marked;}
	
	/** Sort distance. */
	inline float GetSortDistance() const{ return pSortDistance; }
	
	/** Set sort distance. */
	void SetSortDistance(float distance);
	
	
	
	/** Model. */
	inline const deoglRModel::Ref &GetModel() const{ return pModel; }
	
	/** Set model or NULL if not set. */
	void SetModel(deoglRModel *model);
	
	/** Skin or NULL if not set. */
	inline const deoglRSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** Set skin or NULL if not set. */
	void SetSkin(deoglRSkin *skin);
	
	/** Rig changed. */
	void RigChanged();
	
	/** Dynamic skin or NULL if not set. */
	inline const deoglRDynamicSkin::Ref &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/**
	 * Set dynamic skin or NULL if not set.
	 * \note Called from main thread during synchronization.
	 */
	void SetDynamicSkin(deoglComponent &component, deoglRDynamicSkin *dynamicSkin);
	
	/** Occlusion mesh or NULL if not set. */
	inline const deoglROcclusionMesh::Ref &GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** Set occlusion mesh or NULL if not set. */
	void SetOcclusionMesh(deoglROcclusionMesh *occlusionMesh);
	
	/** Dynamic occlusion mesh or NULL if not set. */
	inline deoglDynamicOcclusionMesh *GetDynamicOcclusionMesh() const{ return pDynamicOcclusionMesh; }
	
	/** Dynamic occlusion mesh requires prepare for render. */
	void DynOccMeshRequiresPrepareForRender();
	
	/** Occlusion mesh shared shader parameter block element. */
	inline const deoglSharedSPBElement::Ref &GetOccMeshSharedSPBElement() const{ return pOccMeshSharedSPBElement; }
	
	/** Shared SPB render task instance group. */
	deoglSharedSPBRTIGroup &GetOccMeshSharedSPBRTIGroup(bool doubleSided) const;
	
	/** Invalidate occlusion mesh shared SPB render task instance group. */
	void InvalidateOccMeshSharedSPBRTIGroup();
	
	/** Mesh changed. */
	void MeshChanged();
	
	
	
	/** Model skin to mappings. */
	inline const decIntList &GetModelSkinMappings() const{ return pModelSkinMappings; }
	
	/** Skin state. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** Skin rendered. */
	inline deoglSkinRendered &GetSkinRendered(){ return pSkinRendered; }
	inline const deoglSkinRendered &GetSkinRendered() const{ return pSkinRendered; }
	
	void InitSkinStateStates(const deComponent &component);
	void UpdateSkinStateBones(const deComponent &component);
	void UpdateSkinStateStates();
	
	void DirtyPrepareSkinStateRenderables();
	
	
	
	/** Mark occlusion mesh shader parameter block dirty. */
	void MarkOccMeshParamBlockDirty();
	
	/** Update occlusion mesh instance parameter block. */
	void UpdateOccmeshInstanceParamBlock(deoglShaderParameterBlock &paramBlock, int element);
	
	
	
	/** Bone matrices. */
	inline oglMatrix3x4 *GetBoneMatrices() const{ return pBoneMatrices; }
	
	/** Number of bone matrices. */
	inline int GetBoneMatrixCount() const{ return pBoneMatrixCount; }
	
	/** Update the bone matrices if required. */
	void UpdateBoneMatrices(const deComponent &component);
	
	
	
	/** Vertex position sets. */
	inline float *GetVertexPositionSets() const{ return pVertexPositionSetWeights; }
	
	/** Vertex position set count. */
	inline int GetVertexPositionSetCount() const{ return pVertexPositionSetCount; }
	
	/** Update vertex position sets. */
	void UpdateVertexPositionSets(const deComponent &component);
	
	
	
	/** Point offset. */
	int GetPointOffset(int lodLevel) const;
	
	/** Index offset. */
	int GetIndexOffset(int lodLevel) const;
	
	/** VAO for the vertices of the given lod level. */
	deoglVAO *GetVAO(int lodLevel) const;
	
	/** Invalidate VAO. */
	void InvalidateVAO();
	
	
	
	/** Update skin. */
	void UpdateSkin(float elapsed);
	
	
	
	/** Lit. */
	inline bool GetLit() const{ return pLit; }
	
	/** Set lit. */
	void SetLit(bool lit);
	
	/** Occluded. */
	inline bool GetOccluded() const{ return pOccluded; }
	
	/** Set occluded. */
	void SetOccluded(bool occluded);
	
	
	
	
	/** Render mode. */
	inline eRenderModes GetRenderMode() const{ return pRenderMode; }
	
	/** Set render mode. */
	void SetRenderMode(eRenderModes renderMode);
	
	/** Component has no transparent faces. */
	inline bool GetSolid() const{ return pSolid; }
	
	/** Component has no transparent XRay faces. */
	inline bool GetXRaySolid() const{ return pXRaySolid; }
	
	/** Component has no transparent outline. */
	inline bool GetOutlineSolid() const{ return pOutlineSolid; }
	
	/** Mark solid flags dirty. */
	void DirtySolid();
	
	/** Component is render static right now. */
	inline bool GetRenderStatic() const{ return pRenderStatic; }
	
	/** Set render static. */
	void SetRenderStatic(bool isStatic);
	
	/** Reset render static. */
	void ResetRenderStatic();
	
	/** GI static. */
	bool IsGIStatic() const;
	
	/** Light list. */
	inline deoglLightList &GetLightList(){ return pLightList; }
	inline const deoglLightList &GetLightList() const{ return pLightList; }
	
	
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	
	/** Compute shader octree index. */
	inline uint32_t GetCSOctreeIndex() const{ return pCSOctreeIndex; }
	void SetCSOctreeIndex(uint32_t index){pCSOctreeIndex = index;}
	
	
	
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
	/*@}*/
	
	
	
	/** \name LODs */
	/*@{*/
	/** Number of lods. */
	int GetLODCount() const;
	
	/** LOD by index. Negative index counts from the end */
	deoglRComponentLOD &GetLODAt(int index) const;
	
	/** Remove all LODs. */
	void RemoveAllLODs();
	
	/** Add LOD. */
	void AddLOD(deoglRComponentLOD *lod);
	
	/** LOD error scaling factor. */
	inline float GetLODErrorScaling() const{ return pLODErrorScaling; }
	
	/** Set LOD error scaling factor. */
	void SetLODErrorScaling(float errorScaling);
	
	/** Mark LOD VBOs dirty requiring preparing. */
	void DirtyLODVBOs();
	
	/** Mark LOD render task configurations dirty requiring preparing. */
	void DirtyLODRenderTaskConfigs();
	
	/** Update render target shared instances. */
	void UpdateRTSInstances();
	/*@}*/
	
	
	
	/** \name Textures */
	/*@{*/
	/** Number of textures. */
	int GetTextureCount() const;
	
	/** Texture at index. */
	deoglRComponentTexture &GetTextureAt(int index) const;
	
	/** Remove all textures. */
	void RemoveAllTextures();
	
	/** Add texture. */
	void AddTexture(deoglRComponentTexture *texture);
	
	/** Invalidate parameter blocks of all textures. */
	void InvalidateAllTexturesParamBlocks();
	
	/** Mark parameter blocks of all textures dirty. */
	void MarkAllTexturesParamBlocksDirty();
	
	/** Marks texture units configurations of all textures dirty. */
	void MarkAllTexturesTUCsDirty();
	
	/** Marks env-map using texture units configurations of all textures dirty. */
	void MarkAllTexturesTUCsDirtyEnvMapUse();
	
	/** Update static textures. */
	void UpdateStaticTextures();
	
	/** Dynamic skin renderables changed. */
	void DynamicSkinRenderablesChanged();
	void TextureDynamicSkinRenderablesChanged(deoglRComponentTexture &texture);
	
	void UpdateRenderableMapping();
	void UpdateTexturesUseSkin();
	void DirtyTextureTUCs();
	void DirtyTextureParamBlocks();
	
	/** Count of textures with outline properties. */
	inline int GetOutlineTextureCount() const{ return pOutlineTextureCount; }
	
	/** Textures are static. */
	inline bool GetStaticTextures() const{ return pStaticTextures; }
	/*@}*/
	
	
	
	/** \name Decals */
	/*@{*/
	/** Number of decals. */
	int GetDecalCount() const;
	
	/** Decal at index. */
	deoglRDecal *GetDecalAt(int index) const;
	
	/**
	 * Synchronize decal references.
	 * 
	 * Updates the decal list to match the decal list of \em component. Calls
	 * SetParentComponent() only on decals not part of component already. This avoids
	 * triggering re-building of all decals when decals are added or removed.
	 * 
	 * \note Call this from main thread only during synchronization.
	 */
	void SyncDecalReferences(const deComponent &engComponent);
	
	/** Mark parameter blocks of all attached decals dirty. */
	void MarkAllDecalTexturesParamBlocksDirty();
	
	/** Decal requires prepare for render. */
	void DecalRequiresPrepareForRender();
	/*@}*/
	
	
	
	/** \name Culling */
	/*@{*/
	/**  Cull sphere center. */
	const decDVector &GetCullSphereCenter();
	
	/** Cull sphere radius. */
	float GetCullSphereRadius();
	
	/** Set culling parameters dirty. */
	void SetDirtyCulling();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener(deoglComponentListener *listener);
	
	/** Remove listener if existing. */
	void RemoveListener(deoglComponentListener *listener);
	
	/** Notify all that the boundaries changed. */
	void NotifyBoundariesChanged();
	
	/** Notify all that this component has been destroyed. */
	void NotifyComponentDestroyed();
	
	/** Notify all component changed parent world. */
	void NotifyParentWorldChanged();
	
	/** Notify all component changed layer mask. */
	void NotifyLayerMaskChanged();
	
	/** Notify all component changed render static. */
	void NotifyRenderStaticChanged();
	
	/** Notify all occlusion mesh geometry changed. */
	void NotifyOcclusionMeshChanged();
	
	/** Notify all textures changed. */
	void NotifyTexturesChanged();
	
	/** Notify all TUC changed. */
	void NotifyTUCChanged();
	
	/** Notify all movement hint changed. */
	void NotifyMovementHintChanged();
	
	void NotifyGIImportanceChanged();
	
	/** Notify all lights that their light volume has to be updated. */
	void NotifyLightsDirtyLightVolume();
	
	/** Notify skies about changes potentially requiring an update. */
	void NotifySkiesUpdateStatic();
	
	/** Notify visibility changed. */
	void NotifyVisibilityChanged();
	
	/** Notify model changed. */
	void NotifyModelChanged();
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
	
	
	
	/** Linked list world previous. */
	inline deoglRComponent *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** Set linked list world previous. */
	void SetLLWorldPrev(deoglRComponent *component);
	
	/** Linked list world next. */
	inline deoglRComponent *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** Set linked list world next. */
	void SetLLWorldNext(deoglRComponent *component);
	
	/** World prepare for render linked list. */
	inline decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld(){ return pLLPrepareForRenderWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld() const{ return pLLPrepareForRenderWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pInvalidateLODVBOs();
	
	void pUpdateModelSkinMappings();
	void pResizeModelSkinMappings();
	void pUpdateModelRigMappings(const deComponent &component);
	void pResizeModelRigMappings();
	void pCheckRenderModifier(deoglRCamera *oglCamera);
	void pUpdateRenderMode();
	void pUpdateCullSphere();
	
	void pPrepareSolidity();
	void pPrepareModelVBOs();
	void pPrepareLODVBOs();
	void pPrepareLODRenderTaskConfigs();
	void pPrepareRenderEnvMap();
	void pPrepareSkinStateRenderables(const deoglRenderPlanMasked *mask);
	void pRenderSkinStateRenderables(const deoglRenderPlanMasked *mask);
	void pPrepareSkinStateConstructed();
	void pPrepareTextureTUCs();
	void pPrepareParamBlocks();
	void pPrepareTextureParamBlocks();
	void pPrepareDecals(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	void pPrepareDecalsRenderRenderables(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	void pPrepareOccMeshVBO();
	void pPrepareOccMeshRTSInstances();
	void pPrepareDynOccMesh();
	
	void pResizeBoneMatrices();
	
	void pRemoveFromAllLights();
	
	void pRequiresPrepareForRender();
};

#endif
