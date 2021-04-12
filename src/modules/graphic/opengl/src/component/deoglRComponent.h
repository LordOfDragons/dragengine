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

class deoglComponentListener;
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
 * Render component.
 */
class deoglRComponent : public deObject, public deoglOcclusionTestListener{
public:
	/** Render modes. */
	enum eRenderModes{
		/** Render as static model. */
		ermStatic,
		
		/** Render as dynamic model. */
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
	bool pStaticTextures;
	
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
	bool pOutlineSolid;
	
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
	
	decObjectOrderedSet pListeners;
	int pListenerIndex;
	
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
	/** Create render component. */
	deoglRComponent( deoglRenderThread &renderThread );
	
	/** Clean up render component. */
	virtual ~deoglRComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Parent world or \em NULL if not part of a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** Octree node or \em NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * Set octree node or \em NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by deoglWorldOctree.
	 */
	void SetOctreeNode( deoglWorldOctree *octreeNode );
	
	/** Update octree position. */
	void UpdateOctreeNode();
	
	
	
	/** Component is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set component is visible. */
	void SetVisible( bool visible );
	
	/** Movement hint. */
	inline deComponent::eMovementHints GetMovementHint() const{ return pMovementHint; }
	
	/** Set movement hint. */
	void SetMovementHint( deComponent::eMovementHints hint );
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** Update. */
	void Update( float elapsed );
	
	/** Update VBO. */
	void UpdateVBO();
	
	
	
	/** Set renderables dirty. */
	void SetDirtyRendereables();
	
	/** Update renderables in the component if existing */
	void UpdateRenderables( deoglRenderPlan &plan );
	
	/** Add plans for renderables in the component if existing and requiring one. */
	void AddSkinStateRenderPlans( deoglRenderPlan &plan );
	
	
	
	/** Transformation matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Inverse transformation matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	
	/** Set matrices. */
	void SetMatrix( const decDMatrix &matrix );
	
	
	
	/** Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/**
	 * Update extends of dirty.
	 * \warning Requires UpdateBoneMatrices() to be called first.
	 */
	void UpdateExtends( deComponent &component );
	
	
	
	/** Marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** Set marked flag. */
	inline void SetMarked( bool marked ){ pMarked = marked; }
	
	/** Sort distance. */
	inline float GetSortDistance() const{ return pSortDistance; }
	
	/** Set sort distance. */
	void SetSortDistance( float distance );
	
	
	
	/** Model or \em NULL if not set. */
	inline deoglRModel *GetModel() const{ return pModel; }
	
	/** Set model or \em NULL if not set. */
	void SetModel( deoglRModel *model );
	
	/** Skin or \em NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin or \em NULL if not set. */
	void SetSkin( deoglRSkin *skin );
	
	/** Rig changed. */
	void RigChanged();
	
	/** Dynamic skin or \em NULL if not set. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or \em NULL if not set. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** Occlusion mesh or \em NULL if not set. */
	inline deoglROcclusionMesh *GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** Set occlusion mesh or \em NULL if not set. */
	void SetOcclusionMesh( deoglROcclusionMesh *occlusionMesh );
	
	/** Dynamic occlusion mesh or \em NULL if not set. */
	inline deoglDynamicOcclusionMesh *GetDynamicOcclusionMesh() const{ return pDynamicOcclusionMesh; }
	
	/** Occlusion mesh shared shader parameter block element. */
	deoglSharedSPBElement *GetOccMeshSharedSPBElement();
	
	/** Shared SPB render task instance group. */
	deoglSharedSPBRTIGroup &GetOccMeshSharedSPBRTIGroup( bool doubleSided );
	
	/** Mesh changed. */
	void MeshChanged();
	
	
	
	/** Model skin to mappings. */
	inline const decIntList &GetModelSkinMappings() const{ return pModelSkinMappings; }
	
	/** Skin state. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** Skin rendered. */
	inline deoglSkinRendered &GetSkinRendered(){ return pSkinRendered; }
	inline const deoglSkinRendered &GetSkinRendered() const{ return pSkinRendered; }
	
	/** Init skin state calculated properties. */
	void InitSkinStateCalculatedProperties( const deComponent &component );
	
	/** Update skin state calculated properties bone information. */
	void UpdateSkinStateCalculatedPropertiesBones( const deComponent &component );
	
	
	
	/** Shader parameter block for a occlusion meshes (both static and dynamic). */
	deoglSPBlockUBO *GetParamBlockOccMesh();
	
	/** Mark occlusion mesh shader parameter block dirty. */
	void MarkOccMeshParamBlockDirty();
	
	/** Update occlusion mesh instance parameter block. */
	void UpdateOccmeshInstanceParamBlock( deoglShaderParameterBlock &paramBlock, int element );
	
	
	
	/**
	 * Special shader parameter block.
	 * 
	 * Has to be updated by caller.
	 */
	deoglSPBlockUBO *GetParamBlockSpecial();
	
	/**
	 * Update cube face visibility.
	 * 
	 * Calculates for each face of a cube map if the object is potentially visible.
	 * Used to optimize rendering cube maps by not rendering objects on cube map faces
	 * where the object is not visible.
	 */
	void UpdateCubeFaceVisibility( const decDVector &cubePosition );
	
	/**
	 * Object is visible in cube map face.
	 * 
	 * Only valid after UpdateCubeFaceVisibility() has been called.
	 */
	bool GetCubeFaceVisible( int cubeFace ) const;
	
	/**
	 * Special flags for use with render task.
	 * 
	 * Call appropriate flags update method to update the special flags before use.
	 */
	inline int GetSpecialFlags() const{ return pSpecialFlags; }
	
	/** Set special flags from cube map visibility. */
	void SetSpecialFlagsFromFaceVisibility();
	
	/**
	 * Update special shader parameter block.
	 * 
	 * Depending on the usage these calls are required before using this call.
	 * - UpdateCubeFaceVisibility(): for use by geometry shader cube rendering.
	 */
	void UpdateSpecialSPBCubeRender();
	
	
	
	/** Bone matrices. */
	inline oglMatrix3x4 *GetBoneMatrices() const{ return pBoneMatrices; }
	
	/** Number of bone matrices. */
	inline int GetBoneMatrixCount() const{ return pBoneMatrixCount; }
	
	/** Update the bone matrices if required. */
	void UpdateBoneMatrices( deComponent &component );
	
	
	
	/** Point offset. */
	int GetPointOffset( int lodLevel ) const;
	
	/** Index offset. */
	int GetIndexOffset( int lodLevel ) const;
	
	/** VAO for the vertices of the given lod level. */
	deoglVAO *GetVAO( int lodLevel );
	
	/** Invalidate VAO. */
	void InvalidateVAO();
	
	
	
	/** Update skin. */
	void UpdateSkin( float elapsed );
	
	
	
	/** Lit. */
	inline bool GetLit() const{ return pLit; }
	
	/** Set lit. */
	void SetLit( bool lit );
	
	/** Occluded. */
	inline bool GetOccluded() const{ return pOccluded; }
	
	/** Set occluded. */
	void SetOccluded( bool occluded );
	
	/** Camera is inside component boundaries. */
	inline bool GetCameraInside() const{ return pCameraInside; }
	
	/** Test if camera is inside component boundaries. */
	void TestCameraInside( const decDVector &position );
	
	
	
	
	/** Render mode. */
	inline eRenderModes GetRenderMode() const{ return pRenderMode; }
	
	/** Component has no transparent faces. */
	inline bool GetSolid() const{ return pSolid; }
	
	/** Component has no transparent outline. */
	inline bool GetOutlineSolid() const{ return pOutlineSolid; }
	
	/** Component is render static right now. */
	inline bool GetRenderStatic() const{ return pRenderStatic; }
	
	/** Component is render visible. */
	inline bool GetRenderVisible() const{ return pRenderVisible; }
	
	/** Set if component is render visible. */
	void SetRenderVisible( bool visible );
	
	/** Set render static. */
	void SetRenderStatic( bool isStatic );
	
	/** Reset render static. */
	void ResetRenderStatic();
	
	/** Light list. */
	inline deoglLightList &GetLightList(){ return pLightList; }
	inline const deoglLightList &GetLightList() const{ return pLightList; }
	
	
	
	/** Sky shadow split mask. */
	inline int GetSkyShadowSplitMask() const{ return pSkyShadowSplitMask; }
	
	/** Set sky shadow split mask. */
	void SetSkyShadowSplitMask( int mask );
	
	
	
	/** Render environment map or NULL if not used. */
	inline deoglEnvironmentMap *GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** Set render environment map or NULL if not assigned yet. */
	void SetRenderEnvMap( deoglEnvironmentMap *envmap );
	
	/** Fading render environment map or NULL if not used. */
	inline deoglEnvironmentMap *GetRenderEnvMapFade() const{ return pRenderEnvMapFade; }
	
	/** Set fading render environment map or NULL if not used. */
	void SetRenderEnvMapFade( deoglEnvironmentMap *envmap );
	
	/** Render environment map fade per time. */
	inline float GetRenderEnvMapFadePerTime() const{ return pRenderEnvMapFadePerTime; }
	
	/** Set render environment map fade per time. */
	void SetRenderEnvMapFadePerTime( float fadePerTime );
	
	/** Render environment map fade factor. */
	inline float GetRenderEnvMapFadeFactor() const{ return pRenderEnvMapFadeFactor; }
	
	/** Set render environment map fade factor. */
	void SetRenderEnvMapFadeFactor( float factor );
	
	/** World environment map layout changed. */
	void WorldEnvMapLayoutChanged();
	
	/** Update render environment map. */
	void UpdateRenderEnvMap();
	
	/** Invalidate render environment map. */
	void InvalidateRenderEnvMap();
	
	/** Invalidate render environment map. */
	void InvalidateRenderEnvMapIf( deoglEnvironmentMap *envmap );
	
	/** The world reference point changed. */
	void WorldReferencePointChanged();
	
	
	
	/** Prepare for quick disposal of component. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name LODs */
	/*@{*/
	/** Number of lods. */
	int GetLODCount() const;
	
	/** LOD by index. Negative index counts from the end */
	deoglRComponentLOD &GetLODAt( int index ) const;
	
	/** Remove all LODs. */
	void RemoveAllLODs();
	
	/** Add LOD. */
	void AddLOD( deoglRComponentLOD *lod );
	
	/** LOD error scaling factor. */
	inline float GetLODErrorScaling() const{ return pLODErrorScaling; }
	
	/** Set LOD error scaling factor. */
	void SetLODErrorScaling( float errorScaling );
	/*@}*/
	
	
	
	/** \name Textures */
	/*@{*/
	/** Number of textures. */
	int GetTextureCount() const;
	
	/** Texture at index. */
	deoglRComponentTexture &GetTextureAt( int index );
	
	/** Remove all textures. */
	void RemoveAllTextures();
	
	/** Add texture. */
	void AddTexture( deoglRComponentTexture *texture );
	
	/** Mark texture use skins dirty. */
	void MarkTextureUseSkinDirty();
	
	/** Invalidate parameter blocks of all textures. */
	void InvalidateAllTexturesParamBlocks();
	
	/** Mark parameter blocks of all textures dirty. */
	void MarkAllTexturesParamBlocksDirty();
	
	/** Marks texture units configurations of all textures dirty. */
	void MarkAllTexturesTUCsDirty();
	
	/** Update static textures. */
	void UpdateStaticTextures();
	
	/** Textures are static. */
	inline bool GetStaticTextures() const{ return pStaticTextures; }
	/*@}*/
	
	
	
	/** \name Decals */
	/*@{*/
	/** Number of decals. */
	int GetDecalCount() const;
	
	/** Decal at index. */
	deoglRDecal *GetDecalAt( int index );
	
	/**
	 * Synchronize decal references.
	 * 
	 * Updates the decal list to match the decal list of \em component. Calls
	 * SetParentComponent() only on decals not part of component already. This avoids
	 * triggering re-building of all decals when decals are added or removed.
	 * 
	 * \note Call this from main thread only during synchronization.
	 */
	void SyncDecalReferences( const deComponent &engComponent );
	
	/** Mark parameter blocks of all attached decals dirty. */
	void MarkAllDecalTexturesParamBlocksDirty();
	/*@}*/
	
	
	
	/** \name Culling */
	/*@{*/
	/**  Cull sphere center. */
	const decDVector &GetCullSphereCenter();
	
	/** Cull sphere radius. */
	float GetCullSphereRadius();
	
	/** Set culling parameters dirty. */
	void SetDirtyCulling();
	
	/** Start occlusion test. */
	void StartOcclusionTest( const decDVector &cameraPosition );
	
	/** Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener( deoglComponentListener *listener );
	
	/** Remove listener if existing. */
	void RemoveListener( deoglComponentListener *listener );
	
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
	
	/** Notify all movement hint changed. */
	void NotifyMovementHintChanged();
	
	/** Notify all lights that their light volume has to be updated. */
	void NotifyLightsDirtyLightVolume();
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
	void SetWorldMarkedRemove( bool marked );
	
	
	
	/** Linked list world previous. */
	inline deoglRComponent *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** Set linked list world previous. */
	void SetLLWorldPrev( deoglRComponent *component );
	
	/** Linked list world next. */
	inline deoglRComponent *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** Set linked list world next. */
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
