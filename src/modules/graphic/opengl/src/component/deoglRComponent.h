/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRCOMPONENT_H_
#define _DEOGLRCOMPONENT_H_

#include "../deoglBasics.h"
#include "../light/deoglLightList.h"
#include "../occlusiontest/deoglOcclusionTestListener.h"
#include "../skin/rendered/deoglSkinRendered.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/component/deComponent.h>

class deoglComponentNotifier;
class deoglDynamicOcclusionMesh;
class deoglEnvironmentMap;
class deoglRCamera;
class deoglRComponentLOD;
class deoglRComponentTexture;
class deoglRDecal;
class deoglRDynamicSkin;
class deoglRModel;
class deoglROcclusionMesh;
class deoglRSkin;
class deoglRWorld;
class deoglRenderPlan;
class deoglRenderThread;
class deoglSPBlockUBO;
class deoglShaderParameterBlock;
class deoglSharedSPBElement;
class deoglSharedSPBRTIGroup;
class deoglSkinState;
class deoglVAO;
class deoglVBOLayout;
class deoglWorldOctree;

class deComponent;


/**
 * \brief Render component.
 */
class deoglRComponent : public deObject, public deoglOcclusionTestListener{
public:
	/** \brief Render modes. */
	enum eRenderModes{
		/** \brief Render as static model. */
		ermStatic,
		
		/** \brief Render as dynamic model. */
		ermDynamic
	};
	
public:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	
	bool pVisible;
	deComponent::eMovementHints pMovementHint;
	decLayerMask pLayerMask;
	
	decIntList pModelSkinMappings;
	decIntList pModelRigMappings;
	deoglRModel *pModel;
	deoglRSkin *pSkin;
	deoglRDynamicSkin *pDynamicSkin;
	
	deoglROcclusionMesh *pOcclusionMesh;
	deoglDynamicOcclusionMesh *pDynamicOcclusionMesh;
	deoglSharedSPBElement *pOccMeshSharedSPBElement;
	bool pDirtyOccMeshSharedSPBElement;
	deoglSharedSPBRTIGroup *pOccMeshSharedSPBDoubleSided;
	deoglSharedSPBRTIGroup *pOccMeshSharedSPBSingleSided;
	
	decObjectList pLODs;
	float pLODErrorScaling;
	
	deoglSPBlockUBO *pParamBlockOccMesh;
	bool pDirtyParamBlockOccMesh;
	
	deoglSPBlockUBO *pParamBlockSpecial;
	bool pCubeFaceVisible[ 6 ];
	int pSpecialFlags;
	
	// dynamic model data
	oglMatrix3x4 *pBoneMatrices;
	int pBoneMatrixCount;
	
	// for world
	bool pLit;
	bool pOccluded;
	bool pDirtyRenderables;
	bool pDirtyTextureUseSkin;
	bool pDirtyModelRigMappings;
	
	bool pCameraInside;
	bool pRenderVisible;
	
	bool pFirstRender;
	bool pRenderStatic;
	float pStaticSince;
	
	bool pMarked;
	
	bool pSolid;
	
	eRenderModes pRenderMode;
	float pSortDistance;
	
	deoglSkinState *pSkinState;
	deoglSkinRendered pSkinRendered;
	
	decObjectList pTextures;
	decObjectList pDecals;
	
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
	
	int pSkyShadowSplitMask;
	
	decObjectOrderedSet pNotifiers;
	int pNotifierIndex;
	
	deoglEnvironmentMap *pRenderEnvMap;
	deoglEnvironmentMap *pRenderEnvMapFade;
	float pRenderEnvMapFadePerTime;
	float pRenderEnvMapFadeFactor;
	bool pDirtyRenderEnvMap;
	
	deoglEnvironmentMap *pEnvMap;
	
	bool pWorldMarkedRemove;
	
	deoglRComponent *pLLWorldPrev;
	deoglRComponent *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render component. */
	deoglRComponent( deoglRenderThread &renderThread );
	
	/** \brief Clean up render component. */
	virtual ~deoglRComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Parent world or \em NULL if not part of a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** \brief Octree node or \em NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * \brief Set octree node or \em NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by deoglWorldOctree.
	 */
	void SetOctreeNode( deoglWorldOctree *octreeNode );
	
	/** \brief Update octree position. */
	void UpdateOctreeNode();
	
	
	
	/** \brief Component is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set component is visible. */
	void SetVisible( bool visible );
	
	/** \brief Movement hint. */
	inline deComponent::eMovementHints GetMovementHint() const{ return pMovementHint; }
	
	/** \brief Set movement hint. */
	void SetMovementHint( deComponent::eMovementHints hint );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Update VBO. */
	void UpdateVBO();
	
	
	
	/** \brief Set renderables dirty. */
	void SetDirtyRendereables();
	
	/** \brief Update renderables in the component if existing */
	void UpdateRenderables( deoglRenderPlan &plan );
	
	/** \brief Add plans for renderables in the component if existing and requiring one. */
	void AddSkinStateRenderPlans( deoglRenderPlan &plan );
	
	
	
	/** \brief Transformation matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse transformation matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	
	/** \brief Set matrices. */
	void SetMatrix( const decDMatrix &matrix );
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/**
	 * \brief Update extends of dirty.
	 * \warning Requires UpdateBoneMatrices() to be called first.
	 */
	void UpdateExtends( deComponent &component );
	
	
	
	/** \brief Marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** \brief Set marked flag. */
	inline void SetMarked( bool marked ){ pMarked = marked; }
	
	/** \brief Sort distance. */
	inline float GetSortDistance() const{ return pSortDistance; }
	
	/** \brief Set sort distance. */
	void SetSortDistance( float distance );
	
	
	
	/** \brief Model or \em NULL if not set. */
	inline deoglRModel *GetModel() const{ return pModel; }
	
	/** \brief Set model or \em NULL if not set. */
	void SetModel( deoglRModel *model );
	
	/** \brief Skin or \em NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or \em NULL if not set. */
	void SetSkin( deoglRSkin *skin );
	
	/** \brief Rig changed. */
	void RigChanged();
	
	/** \brief Dynamic skin or \em NULL if not set. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or \em NULL if not set. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** \brief Occlusion mesh or \em NULL if not set. */
	inline deoglROcclusionMesh *GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** \brief Set occlusion mesh or \em NULL if not set. */
	void SetOcclusionMesh( deoglROcclusionMesh *occlusionMesh );
	
	/** \brief Dynamic occlusion mesh or \em NULL if not set. */
	inline deoglDynamicOcclusionMesh *GetDynamicOcclusionMesh() const{ return pDynamicOcclusionMesh; }
	
	/** \brief Occlusion mesh shared shader parameter block element. */
	deoglSharedSPBElement *GetOccMeshSharedSPBElement();
	
	/** \brief Shared SPB render task instance group. */
	deoglSharedSPBRTIGroup &GetOccMeshSharedSPBRTIGroup( bool doubleSided );
	
	/** \brief Mesh changed. */
	void MeshChanged();
	
	
	
	/** \brief Model skin to mappings. */
	inline const decIntList &GetModelSkinMappings() const{ return pModelSkinMappings; }
	
	/** \brief Skin state. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** \brief Skin rendered. */
	inline deoglSkinRendered &GetSkinRendered(){ return pSkinRendered; }
	inline const deoglSkinRendered &GetSkinRendered() const{ return pSkinRendered; }
	
	/** \brief Init skin state calculated properties. */
	void InitSkinStateCalculatedProperties( const deComponent &component );
	
	/** \brief Update skin state calculated properties bone information. */
	void UpdateSkinStateCalculatedPropertiesBones( const deComponent &component );
	
	
	
	/** \brief Shader parameter block for a occlusion meshes (both static and dynamic). */
	deoglSPBlockUBO *GetParamBlockOccMesh();
	
	/** \brief Mark occlusion mesh shader parameter block dirty. */
	void MarkOccMeshParamBlockDirty();
	
	/** \brief Update occlusion mesh instance parameter block. */
	void UpdateOccmeshInstanceParamBlock( deoglShaderParameterBlock &paramBlock, int element );
	
	
	
	/**
	 * \brief Special shader parameter block.
	 * 
	 * Has to be updated by caller.
	 */
	deoglSPBlockUBO *GetParamBlockSpecial();
	
	/**
	 * \brief Update cube face visibility.
	 * 
	 * Calculates for each face of a cube map if the object is potentially visible.
	 * Used to optimize rendering cube maps by not rendering objects on cube map faces
	 * where the object is not visible.
	 */
	void UpdateCubeFaceVisibility( const decDVector &cubePosition );
	
	/**
	 * \brief Object is visible in cube map face.
	 * 
	 * Only valid after UpdateCubeFaceVisibility() has been called.
	 */
	bool GetCubeFaceVisible( int cubeFace ) const;
	
	/**
	 * \brief Special flags for use with render task.
	 * 
	 * Call appropriate flags update method to update the special flags before use.
	 */
	inline int GetSpecialFlags() const{ return pSpecialFlags; }
	
	/** \brief Set special flags from cube map visibility. */
	void SetSpecialFlagsFromFaceVisibility();
	
	/**
	 * \brief Update special shader parameter block.
	 * 
	 * Depending on the usage these calls are required before using this call.
	 * - UpdateCubeFaceVisibility(): for use by geometry shader cube rendering.
	 */
	void UpdateSpecialSPBCubeRender();
	
	
	
	/** \brief Bone matrices. */
	inline oglMatrix3x4 *GetBoneMatrices() const{ return pBoneMatrices; }
	
	/** \brief Number of bone matrices. */
	inline int GetBoneMatrixCount() const{ return pBoneMatrixCount; }
	
	/** \brief Update the bone matrices if required. */
	void UpdateBoneMatrices( deComponent &component );
	
	
	
	/** \brief Point offset. */
	int GetPointOffset( int lodLevel ) const;
	
	/** \brief Index offset. */
	int GetIndexOffset( int lodLevel ) const;
	
	/** \brief VAO for the vertices of the given lod level. */
	deoglVAO *GetVAO( int lodLevel );
	
	/** \brief Invalidate VAO. */
	void InvalidateVAO();
	
	
	
	/** \brief Update skin. */
	void UpdateSkin( float elapsed );
	
	
	
	/** \brief Lit. */
	inline bool GetLit() const{ return pLit; }
	
	/** \brief Set lit. */
	void SetLit( bool lit );
	
	/** \brief Occluded. */
	inline bool GetOccluded() const{ return pOccluded; }
	
	/** \brief Set occluded. */
	void SetOccluded( bool occluded );
	
	/** \brief Camera is inside component boundaries. */
	inline bool GetCameraInside() const{ return pCameraInside; }
	
	/** \brief Test if camera is inside component boundaries. */
	void TestCameraInside( const decDVector &position );
	
	
	
	
	/** \brief Render mode. */
	inline eRenderModes GetRenderMode() const{ return pRenderMode; }
	
	/** \brief Component has no transparent faces. */
	inline bool GetSolid() const{ return pSolid; }
	
	/** \brief Component is render static right now. */
	inline bool GetRenderStatic() const{ return pRenderStatic; }
	
	/** \brief Component is render visible. */
	inline bool GetRenderVisible() const{ return pRenderVisible; }
	
	/** \brief Set if component is render visible. */
	void SetRenderVisible( bool visible );
	
	/** \brief Set render static. */
	void SetRenderStatic( bool isStatic );
	
	/** \brief Reset render static. */
	void ResetRenderStatic();
	
	/** \brief Light list. */
	inline deoglLightList &GetLightList(){ return pLightList; }
	inline const deoglLightList &GetLightList() const{ return pLightList; }
	
	
	
	/** \brief Sky shadow split mask. */
	inline int GetSkyShadowSplitMask() const{ return pSkyShadowSplitMask; }
	
	/** \brief Set sky shadow split mask. */
	void SetSkyShadowSplitMask( int mask );
	
	
	
	/** \brief Render environment map or NULL if not used. */
	inline deoglEnvironmentMap *GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** \brief Set render environment map or NULL if not assigned yet. */
	void SetRenderEnvMap( deoglEnvironmentMap *envmap );
	
	/** \brief Fading render environment map or NULL if not used. */
	inline deoglEnvironmentMap *GetRenderEnvMapFade() const{ return pRenderEnvMapFade; }
	
	/** \brief Set fading render environment map or NULL if not used. */
	void SetRenderEnvMapFade( deoglEnvironmentMap *envmap );
	
	/** \brief Render environment map fade per time. */
	inline float GetRenderEnvMapFadePerTime() const{ return pRenderEnvMapFadePerTime; }
	
	/** \brief Set render environment map fade per time. */
	void SetRenderEnvMapFadePerTime( float fadePerTime );
	
	/** \brief Render environment map fade factor. */
	inline float GetRenderEnvMapFadeFactor() const{ return pRenderEnvMapFadeFactor; }
	
	/** \brief Set render environment map fade factor. */
	void SetRenderEnvMapFadeFactor( float factor );
	
	/** \brief World environment map layout changed. */
	void WorldEnvMapLayoutChanged();
	
	/** \brief Update render environment map. */
	void UpdateRenderEnvMap();
	
	/** \brief Invalidate render environment map. */
	void InvalidateRenderEnvMap();
	
	/** \brief Invalidate render environment map. */
	void InvalidateRenderEnvMapIf( deoglEnvironmentMap *envmap );
	
	/** \brief The world reference point changed. */
	void WorldReferencePointChanged();
	
	
	
	/** \brief Prepare for quick disposal of component. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name LODs */
	/*@{*/
	/** \brief Number of lods. */
	int GetLODCount() const;
	
	/** \brief LOD by index. */
	deoglRComponentLOD &GetLODAt( int index ) const;
	
	/** \brief Remove all LODs. */
	void RemoveAllLODs();
	
	/** \brief Add LOD. */
	void AddLOD( deoglRComponentLOD *lod );
	
	/** \brief LOD error scaling factor. */
	inline float GetLODErrorScaling() const{ return pLODErrorScaling; }
	
	/** \brief Set LOD error scaling factor. */
	void SetLODErrorScaling( float errorScaling );
	/*@}*/
	
	
	
	/** \name Textures */
	/*@{*/
	/** \brief Number of textures. */
	int GetTextureCount() const;
	
	/** \brief Texture at index. */
	deoglRComponentTexture &GetTextureAt( int index );
	
	/** \brief Remove all textures. */
	void RemoveAllTextures();
	
	/** \brief Add texture. */
	void AddTexture( deoglRComponentTexture *texture );
	
	/** \brief Mark texture use skins dirty. */
	void MarkTextureUseSkinDirty();
	
	/** \brief Invalidate parameter blocks of all textures. */
	void InvalidateAllTexturesParamBlocks();
	
	/** \brief Mark parameter blocks of all textures dirty. */
	void MarkAllTexturesParamBlocksDirty();
	
	/** \brief Marks texture units configurations of all textures dirty. */
	void MarkAllTexturesTUCsDirty();
	/*@}*/
	
	
	
	/** \name Decals */
	/*@{*/
	/** \brief Number of decals. */
	int GetDecalCount() const;
	
	/** \brief Decal at index. */
	deoglRDecal *GetDecalAt( int index );
	
	/**
	 * \brief Synchronize decal references.
	 * 
	 * Updates the decal list to match the decal list of \em component. Calls
	 * SetParentComponent() only on decals not part of component already. This avoids
	 * triggering re-building of all decals when decals are added or removed.
	 * 
	 * \note Call this from main thread only during synchronization.
	 */
	void SyncDecalReferences( const deComponent &engComponent );
	
	/** \brief Mark parameter blocks of all attached decals dirty. */
	void MarkAllDecalTexturesParamBlocksDirty();
	/*@}*/
	
	
	
	/** \name Culling */
	/*@{*/
	/** \brief  Cull sphere center. */
	const decDVector &GetCullSphereCenter();
	
	/** \brief Cull sphere radius. */
	float GetCullSphereRadius();
	
	/** \brief Set culling parameters dirty. */
	void SetDirtyCulling();
	
	/** \brief Start occlusion test. */
	void StartOcclusionTest( const decDVector &cameraPosition );
	
	/** \brief Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Add a notifier. */
	void AddNotifier( deoglComponentNotifier *notifier );
	
	/** \brief Remove notifier if existing. */
	void RemoveNotifier( deoglComponentNotifier *notifier );
	
	/** \brief Notify all that the boundaries changed. */
	void NotifyBoundariesChanged();
	
	/** \brief Notify all that this component has been destroyed. */
	void NotifyComponentDestroyed();
	
	/** \brief Notify all lights that their light volume has to be updated. */
	void NotifyLightsDirtyLightVolume();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	
	
	
	/** \brief Linked list world previous. */
	inline deoglRComponent *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set linked list world previous. */
	void SetLLWorldPrev( deoglRComponent *component );
	
	/** \brief Linked list world next. */
	inline deoglRComponent *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set linked list world next. */
	void SetLLWorldNext( deoglRComponent *component );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pInvalidateLODVBOs();
	
	void pUpdateModelSkinMappings();
	void pResizeModelSkinMappings();
	void pUpdateModelRigMappings( deComponent &component );
	void pResizeModelRigMappings();
	void pCheckRenderModifier( deoglRCamera *oglCamera );
	void pUpdateRenderMode();
	void pUpdateRenderables();
	void pUpdateCullSphere();
	void pUpdateSolid();
	
	void pResizeBoneMatrices();
	
	void pRemoveFromAllLights();
};

#endif
