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

#ifndef _DEOGLRBILLBOARD_H_
#define _DEOGLRBILLBOARD_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/rendered/deoglSkinRendered.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoglWorldOctree;
class deoglRWorld;
class deoglRenderThread;
class deoglRSkin;
class deoglSkinState;
class deoglRDynamicSkin;
class deoglEnvironmentMap;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglShaderParameterBlock;
class deoglSharedSPBElement;
class deoglSharedSPBRTIGroup;
class deoglRenderPlan;

class deBillboard;



/**
 * \brief Render billboard.
 */
class deoglRBillboard : public deObject, public deoglOcclusionTestListener{
public:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	
	deoglRSkin *pSkin;
	deoglSkinTexture *pUseSkinTexture;
	deoglRDynamicSkin *pDynamicSkin;
	
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
	
	deoglEnvironmentMap *pRenderEnvMap;
	deoglEnvironmentMap *pRenderEnvMapFade;
	float pRenderEnvMapFadePerTime;
	float pRenderEnvMapFadeFactor;
	bool pDirtyRenderEnvMap;
	
	deoglSPBlockUBO *pParamBlockDepth;
	deoglSPBlockUBO *pParamBlockGeometry;
	deoglSPBlockUBO *pParamBlockEnvMap;
	deoglSharedSPBElement *pSharedSPBElement;
	deoglSharedSPBRTIGroup *pSharedSPBRTIGroup;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCCounter;
	deoglTexUnitsConfig *pTUCEnvMap;
	
	bool pValidParamBlockDepth;
	bool pValidParamBlockGeometry;
	bool pValidParamBlockEnvMap;
	bool pDirtyParamBlockDepth;
	bool pDirtyParamBlockGeometry;
	bool pDirtyParamBlockEnvMap;
	bool pDirtySharedSPBElement;
	
	bool pDirtyTUCDepth;
	bool pDirtyTUCGeometry;
	bool pDirtyTUCCounter;
	bool pDirtyTUCEnvMap;
	
	deoglSPBlockUBO *pParamBlockSpecial;
	bool pCubeFaceVisible[ 6 ];
	int pSpecialFlags;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	decDVector pCullSphereCenter;
	float pCullSphereRadius;
	bool pDirtyCulling;
	bool pRenderVisible;
	
	bool pMarked;
	
	bool pWorldMarkedRemove;
	
	deoglRBillboard *pLLWorldPrev;
	deoglRBillboard *pLLWorldNext;
	
	decPointerLinkedList::cListEntry pLLPrepareForRenderWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render decal. */
	deoglRBillboard( deoglRenderThread &renderThread );
	
	/** \brief Clean up render decal. */
	virtual ~deoglRBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Parent world or NULL if not part of a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** \brief Octree node or NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * \brief Set octree node or NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by deoglWorldOctree.
	 */
	void SetOctreeNode( deoglWorldOctree *octreeNode );
	
	/** \brief Update octree position. */
	void UpdateOctreeNode();
	
	
	
	/** \brief Skin or NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin( deoglRSkin *skin );
	
	/** \brief Skin texture to use or NULL if not present. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** \brief Dynamic skin or NULL if not set. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or NULL if not set. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** \brief Update skin. */
	void UpdateSkin( float elapsed );
	
	/** \brief Skin state. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** \brief Init skin state calculated properties. */
	void InitSkinStateCalculatedProperties();
	void UpdateSkinStateCalculatedProperties();
	
	/** \brief Skin rendered. */
	inline deoglSkinRendered &GetSkinRendered(){ return pSkinRendered; }
	inline const deoglSkinRendered &GetSkinRendered() const{ return pSkinRendered; }
	
	void DirtyPrepareSkinStateRenderables();
	void PrepareSkinStateRenderables();
	void DynamicSkinRenderablesChanged();
	void UpdateRenderableMapping();
	
	
	
	/** \brief Add plans for renderables in the component if existing and requiring one. */
	void AddSkinStateRenderPlans( deoglRenderPlan &plan );
	
	
	
	/** \brief Shader parameter block for a shader type. */
	deoglSPBlockUBO *GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType );
	
	/**
	 * \brief Depth shader parameter block or NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estBillboardDepth
	 *          - deoglSkinTexture::estBillboardDepthClipPlane
	 *          - deoglSkinTexture::estBillboardCounter
	 *          - deoglSkinTexture::estBillboardCounterClipPlane
	 */
	deoglSPBlockUBO *GetParamBlockDepth();
	
	/**
	 * \brief Geometry shader parameter block or NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estBillboardGeometry
	 */
	deoglSPBlockUBO *GetParamBlockGeometry();
	
	/**
	 * \brief Environment map shader parameter block or NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estEnvMap
	 */
	deoglSPBlockUBO *GetParamBlockEnvMap();
	
	/** \brief Shared shader parameter block element. */
	deoglSharedSPBElement *GetSharedSPBElement();
	
	/** \brief Shared SPB render task instance group. */
	deoglSharedSPBRTIGroup &GetSharedSPBRTIGroup();
	
	/** \brief Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType );
	
	/**
	 * \brief Texture units configuration for depth type shaders or NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estBillboardDepth
	 *          - deoglSkinTexture::estBillboardDepthClipPlane
	 */
	deoglTexUnitsConfig *GetTUCDepth();
	
	/**
	 * \brief Texture units configuration for geometry type shaders or NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estBillboardGeometry
	 */
	deoglTexUnitsConfig *GetTUCGeometry();
	
	/**
	 * \brief Texture units configuration for counter type shaders or NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estBillboardCounter
	 *          - deoglSkinTexture::estBillboardCounterClipPlane
	 */
	deoglTexUnitsConfig *GetTUCCounter();
	
	/**
	 * \brief Texture units configuration for the environment map shader or NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estEnvMap
	 */
	deoglTexUnitsConfig *GetTUCEnvMap();
	
	/**
	 * \brief Obtain texture units configuration for a shader type.
	 * \details Bare call not to be used directly.
	 */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const;
	
	/** \brief Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** \brief Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** \brief Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/**
	 * \brief Update instance parameter shader parameter block.
	 * 
	 * Parameter block has to be mapped while calling this method.
	 * Caller is responsible to properly unmap in case of exceptions.
	 */
	void UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
		int element, deoglSkinShader &skinShader );
	
	
	
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
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Axis. */
	inline const decVector &GetAxis() const{ return pAxis; }
	
	/** \brief Set axis. */
	void SetAxis( const decVector &axis );
	
	/** \brief Size. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decVector2 &size );
	
	/** \brief Offset. */
	inline const decVector2 &GetOffset() const{ return pOffset; }
	
	/** Sets the offset. */
	void SetOffset( const decVector2 &offset );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set visible. */
	void SetVisible( bool visible );
	
	/** \brief Billboard is locked. */
	inline bool GetLocked() const{ return pLocked; }
	
	/** \brief Set billboard is locked. */
	void SetLocked( bool locked );
	
	/** \brief Billboard is spherical. */
	inline bool GetSpherical() const{ return pSpherical; }
	
	/** \brief Set if billboard is spherical. */
	void SetSpherical( bool spherical );
	
	/** \brief Billboard size is fixed to the screen size. */
	inline bool GetSizeFixedToScreen() const{ return pSizeFixedToScreen; }
	
	/** \brief Set if billboard size is fixed to the screen size. */
	void SetSizeFixedToScreen( bool sizeFixedToScreen );
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** \brief Update extends. */
	void UpdateExtends( const deBillboard &billboard );
	
	
	
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
	
	/** \brief Render visible. */
	inline bool GetRenderVisible() const{ return pRenderVisible; }
	
	/** \brief Set if render visible. */
	void SetRenderVisible( bool visible );
	
	/** \brief Marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** \brief Set marked flag. */
	inline void SetMarked( bool marked ){ pMarked = marked; }
	
	/** \brief Sort distance. */
	inline float GetSortDistance() const{ return pSortDistance; }
	
	/** \brief Set sort distance. */
	void SetSortDistance( float distance );
	
	/** \brief Occluded. */
	inline bool GetOccluded() const{ return pOccluded; }
	
	/** \brief Set occluded. */
	void SetOccluded( bool occluded );
	
	/** \brief Sky shadow split mask. */
	inline int GetSkyShadowSplitMask() const{ return pSkyShadowSplitMask; }
	
	/** \brief Set sky shadow split mask. */
	void SetSkyShadowSplitMask( int mask );
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
	
	
	
	/** Prepare for render. Called by deoglRWorld if registered previously. */
	void PrepareForRender( deoglRenderPlan &plan );
	
	/** \brief Prepare for quick disposal of component. */
	void PrepareQuickDispose();
	
	
	
	/** \brief Linked list world previous. */
	inline deoglRBillboard *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set linked list world previous. */
	void SetLLWorldPrev( deoglRBillboard *billboard );
	
	/** \brief Linked list world next. */
	inline deoglRBillboard *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set linked list world next. */
	void SetLLWorldNext( deoglRBillboard *billboard );
	
	/** World prepare for render linked list. */
	inline decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld(){ return pLLPrepareForRenderWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld() const{ return pLLPrepareForRenderWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateCullSphere();
	
	void pRequiresPrepareForRender();
};

#endif
