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

#ifndef _DEOGLRENDERERPLAN_H_
#define _DEOGLRENDERERPLAN_H_

#include "../../collidelist/deoglCollideList.h"
#include "../../envmap/deoglEnvMapFader.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/string/decString.h>

#define OGL_RENDER_GAMMA		2.2f
#define OGL_RENDER_INVGAMMA		( 1.0f / 2.2f )

class deoglRenderThread;
class deoglRCamera;
class deoglFramebuffer;
class deoglGraphicContext;
class deoglHTView;
class deoglOcclusionMap;
class deoglRenderPlanEnvMap;
class deoglPlanVisitorCullElements;
class deoglRenderCacheLight;
class deoglRenderCacheLightShadow;
class deoglRenderPlanDebug;
class deoglRenderPlanLight;
class deoglRenderPlanMasked;
class deoglRenderPlanSkyLight;
class deoglTexture;
class deoglRWorld;
class deoglRSkyInstance;
class deoglRSkyInstanceLayer;



/**
 * \brief Render Plan.
 * 
 * Stores the render plan used to build an efficient method for rendering
 * a particular shot of a world. Furthermore a render plan stores information
 * to use frame to frame coherency where possible. The render plan is self
 * sustaining not requiring a camera for storing the render parameters.
 * Instead all camera attributes are located in the render plan itself. This
 * way indirect rendering or other forms of intermediate rendering are
 * possible where there exists no explicite camera.
 */
class deoglRenderPlan{
private:
	deoglRenderThread &pRenderThread;
	deoglRWorld *pWorld;
	
	bool pIsRendering;
	
	int pLevel;
	
	deoglRCamera *pCamera;
	decDVector pCameraPosition;
	decDMatrix pCameraMatrix;
	decDMatrix pCameraInverseMatrix;
	decDMatrix pProjectionMatrix;
	decDMatrix pFrustumMatrix;
	decDMatrix pRefPosCameraMatrix;
	float pCameraFov;
	float pCameraFovRatio;
	float pCameraImageDistance;
	float pCameraViewDistance;
	decVector4 pDepthToPosition;
	int pViewportX;
	int pViewportY;
	int pViewportWidth;
	int pViewportHeight;
	float pAspectRatio;
	int pUpscaleWidth;
	int pUpscaleHeight;
	bool pUseUpscaling;
	bool pUpsideDown;
	bool pUseToneMap;
	bool pIgnoreDynamicComponents;
	bool pRenderDebugPass;
	bool pNoReflections;
	
	bool pUseLayerMask;
	decLayerMask pLayerMask;
	
	deoglFramebuffer *pFBOTarget;
	bool pFBOTargetCopyDepth;
	
	deoglDCollisionFrustum pCustomFrustum;
	bool pUseCustomFrustum;
	decBoundary pCustomFrustumScreenArea;
	
	bool pDirtyProjMat;
	
	deoglCollideList pCollideList;
	deoglPlanVisitorCullElements *pVisitorCullElements;
	
	bool pNoRenderedOccMesh;
	bool pFlipCulling;
	
	bool pDisableLights;
	bool pRescaleShadowMaps;
	int pShadowMapSize;
	int pShadowCubeSize;
	int pShadowSkySize;
	float pDominance;
	int pForceShadowMapSize;
	
	deoglRenderPlanEnvMap *pEnvMaps;
	int pEnvMapCount;
	
	deoglEnvMapFader pDirectEnvMapFader;
	
	deoglRenderPlanLight **pLights;
	int pLightCount;
	int pLightSize;
	
	deoglRenderPlanSkyLight **pSkyLights;
	int pSkyLightCount;
	int pSkyLightSize;
	
	deoglRenderPlanMasked **pMaskedPlans;
	int pMaskedPlanCount;
	int pMaskedPlanSize;
	
	decPointerList pSkyInstances;
	decColor pSkyBgColor;
	
	deoglHTView *pHTView;
	
	bool pEmptyPass;
	bool pSkyVisible;
	bool pHasTransparency;
	bool pClearStencilPassBits;
	bool pClearColor;
	
	int pCurTransparencyLayer;
	int pTransparencyLayerCount;
	int pRenderPassNumber;
	int pStencilRefValue;
	int pStencilPrevRefValue;
	int pStencilWriteMask;
	
	deoglOcclusionMap *pOcclusionMap;
	int pOcclusionMapBaseLevel;
	decMatrix pOcclusionTestMatrix;
	
	deoglRenderPlanDebug *pDebug;
	bool pDebugTiming;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render plan. */
	deoglRenderPlan( deoglRenderThread &renderThread );
	
	/** \brief Clean up render plan. */
	~deoglRenderPlan();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief World. */
	inline deoglRWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( deoglRWorld *world );
	
	/** \brief Level. */
	inline int GetLevel() const{ return pLevel; }
	
	/** \brief Set level. */
	void SetLevel( int level );
	
	
	
	/** \brief All lights are disabled. */
	inline bool GetDisableLights() const{ return pDisableLights; }
	
	/** \brief Shadow map sizes are rescaled relative to the viewport size. */
	inline bool GetRescaleShadowMaps() const{ return pRescaleShadowMaps; }
	
	/** \brief Set if shadow map sizes are rescaled relative to the viewport size. */
	void SetRescaleShadowMaps( bool rescale );
	
	/** \brief Shadow map size. */
	inline int GetShadowMapSize() const{ return pShadowMapSize; }
	
	/** \brief Shadow cube size. */
	inline int GetShadowCubeSize() const{ return pShadowCubeSize; }
	
	/** \brief Shadow sky size. */
	inline int GetShadowSkySize() const{ return pShadowSkySize; }
	
	/** \brief Dominance. */
	inline float GetDominance() const{ return pDominance; }
	
	/** \brief Forced shadow map size or 0 to use the default shadow map size. */
	inline int GetForceShadowMapSize() const{ return pForceShadowMapSize; }
	
	/** \brief Set forced shadow map size or 0 to use the default shadow map size. */
	void SetForceShadowMapSize( int forcedSize );
	
	
	
	/** \brief Prepare plan for rendering. */
	void PrepareRender();
	
	/**
	 * \brief Prepare whatever can be prepared for a one turn of rendering.
	 * 
	 * The PrepareRender function is used to prepare whatever stays
	 * the same for an entire frame update. This function on the
	 * other hand is used to prepare what can change between rendering
	 * using different cameras. This is done since most of the rendering
	 * takes place in 2 or more steps ( first depth then rendering with
	 * optionally additional transparency passes ). As long as the same
	 * camera is rendering these passes the same prepared values can
	 * be used. Once the camera changes this funciton is called another
	 * time to setup for the next batch of rendering.
	 */
	void PrepareRenderOneTurn();
	
	/**
	 * \brief Plan transparency.
	 * \details Called after transparency counter is ready and planing is possible.
	 * \param[in] layerCount Number of transparent layers. How this is calculated is the
	 *                       responsibility of the caller.
	 */
	void PlanTransparency( int layerCount );
	
	/** \brief Render using the previously prepared plan. */
	void Render();
	
	/** \brief Clean up after rendering. */
	void CleanUp();
	
	
	
	/** \brief Camera or \em NULL. */
	inline deoglRCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or \em NULL. */
	void SetCamera( deoglRCamera *camera );
	
	/** \brief Camera matrix. */
	inline const decDMatrix &GetCameraMatrix() const{ return pCameraMatrix; }
	
	/** \brief Inverse camera matrix. */
	inline const decDMatrix &GetInverseCameraMatrix() const{ return pCameraInverseMatrix; }
	
	/** \brief Set camera matrix including the inverse one. */
	void SetCameraMatrix( const decDMatrix &matrix );
	
	/** \brief Projection matrix. */
	inline const decDMatrix &GetProjectionMatrix() const{ return pProjectionMatrix; }
	
	/** \brief Frustum matrix. */
	inline const decDMatrix &GetFrustumMatrix() const{ return pFrustumMatrix; }
	
	/** \brief Reference position camera matrix. */
	inline const decDMatrix &GetRefPosCameraMatrix() const{ return pRefPosCameraMatrix; }
	
	/** \brief Camera position in world space. */
	inline const decDVector &GetCameraPosition() const{ return pCameraPosition; }
	
	/** \brief Camera field of view. */
	inline float GetCameraFov() const{ return pCameraFov; }
	
	/** \brief Camera field of view ratio. */
	inline float GetCameraFovRatio() const{ return pCameraFovRatio; }
	
	/** \brief Camera image distance. */
	inline float GetCameraImageDistance() const{ return pCameraImageDistance; }
	
	/** \brief Camera view distance. */
	inline float GetCameraViewDistance() const{ return pCameraViewDistance; }
	
	/** \brief Set camera parameters. */
	void SetCameraParameters( float fov, float fovRatio, float imageDistance, float viewDistance );
	
	/** \brief Copy camera parameters from another render plan. */
	void CopyCameraParametersFrom( const deoglRenderPlan &plan );
	
	/** \brief Update reference position camera matrix. */
	void UpdateRefPosCameraMatrix();
	
	
	
	/** \brief Depth to position transformation factors. */
	inline const decVector4 &GetDepthToPosition() const{ return pDepthToPosition; }
	
	
	
	/** \brief Vierwport x. */
	inline int GetViewportX() const{ return pViewportX; }
	
	/** \brief Vierwport y. */
	inline int GetViewportY() const{ return pViewportY; }
	
	/** \brief Vierwport width. */
	inline int GetViewportWidth() const{ return pViewportWidth; }
	
	/** \brief Viewport height. */
	inline int GetViewportHeight() const{ return pViewportHeight; }
	
	/** \brief Ratio between the width and the height. */
	inline float GetAspectRatio() const{ return pAspectRatio; }
	
	/** \brief Set viewport parameters. */
	void SetViewport( int x, int y, int width, int height );
	
	
	
	/** \brief Target framebuffer object or \em NULL to use the one of the active graphics context. */
	inline deoglFramebuffer *GetFBOTarget() const{ return pFBOTarget; }
	
	/** \brief Set target framebuffer object or \em NULL to use the one of the active graphics context. */
	void SetFBOTarget( deoglFramebuffer *fbo );
	
	/** \brief Depth is copied to the target framebuffer object if not \em NULL. */
	inline bool GetFBOTargetCopyDepth() const{ return pFBOTargetCopyDepth; }
	
	/** \brief Set if depth is copied to the target framebuffer object if not \em NULL. */
	void SetFBOTargetCopyDepth( bool copyDepth );
	
	
	
	/** \brief Upscale width. */
	inline int GetUpscaleWidth() const{ return pUpscaleWidth; }
	
	/** \brief Upscale height. */
	inline int GetUpscaleHeight() const{ return pUpscaleHeight; }
	
	/** \brief Set upscale size. */
	void SetUpscaleSize( int width, int height );
	
	/** \brief Upscaling has to be used. */
	inline bool GetUseUpscaling() const{ return pUseUpscaling; }
	
	/** \brief Set if upscaling has to be used. */
	void SetUseUpscaling( bool useUpscaling );
	
	/** \brief Upside down rendering has to be used. */
	inline bool GetUpsideDown() const{ return pUpsideDown; }
	
	/** \brief Set if upside down rendering has to be used. */
	void SetUpsideDown( bool upsideDown );
	
	/** \brief Tone mapping is used. */
	inline bool GetUseToneMap() const{ return pUseToneMap; }
	
	/** \brief Set if tone mapping is used. */
	void SetUseToneMap( bool useToneMap );
	
	/** \brief Static components are ignored. */
	inline bool GetIgnoreStaticComponents() const{ return pIgnoreDynamicComponents; }
	
	/** \brief Set if static components are ignored. */
	void SetIgnoreStaticComponents( bool ignoreStaticComponents );
	
	/** \brief Debug pass is rendered. */
	inline bool GetRenderDebugPass() const{ return pRenderDebugPass; }
	
	/** \brief Set if debug pass is rendered. */
	void SetRenderDebugPass( bool render );
	
	/** \brief No reflections are used. */
	inline bool GetNoReflections() const{ return pNoReflections; }
	
	/** \brief Set if no reflections are used. */
	void SetNoReflections( bool noReflections );
	
	
	
	/** \brief Layer mask is used for culling. */
	inline bool GetUseLayerMask() const{ return pUseLayerMask; }
	
	/** \brief Set if layer mask is used for culling. */
	void SetUseLayerMask( bool useLayerMask );
	
	/** \brief Layer mask. */
	const decLayerMask &GetLayerMask(){ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Set custom frustum. */
	void SetCustomFrustumBoundaries( const decDVector &position, const decDVector &topLeft,
		const decDVector &topRight, const decDVector &bottomLeft, const decDVector &bottomRight,
		double near, double far );
	
	/** \brief Custom frustum is used. */
	inline bool GetUseCustomFrustum() const{ return pUseCustomFrustum; }
	
	/** \brief Set if custom frustum is used. */
	void SetUseCustomFrustum( bool useCustomFrustum );
	
	/** \brief Custom frustum screen area. */
	inline const decBoundary &GetCustomFrustumScreenArea() const{ return pCustomFrustumScreenArea; }
	
	/** \brief Set custom frustum screen area. */
	void SetCustomFrustumScreenArea( const decBoundary &area );
	
	
	
	/** \brief Height terrain view. */
	inline deoglHTView *GetHeightTerrainView() const{ return pHTView; }
	
	/** \brief Collider list. */
	inline deoglCollideList &GetCollideList(){ return pCollideList; }
	
	
	
	/** \brief Ignore occlusion meshes if owner component has rendered skins. */
	inline bool GetNoRenderedOccMesh(){ return pNoRenderedOccMesh; }
	
	/** \brief Set to ignore occlusion meshes if owner component has rendered skins. */
	void SetNoRenderedOccMesh( bool noRenderedOccMesh );
	
	/** \brief Culling flags have to be flipped. */
	inline bool GetFlipCulling() const{ return pFlipCulling; }
	
	/** \brief Set if culling flags have to be flipped. */
	void SetFlipCulling( bool flipCulling );
	
	
	
	/** \brief Pass is empty. */
	inline bool GetEmptyPass() const{ return pEmptyPass; }
	
	/** \brief Set if pass is empty. */
	void SetEmptyPass( bool emptyPass );
	
	
	
	/** \brief Sky is visible. */
	inline bool GetSkyVisible() const{ return pSkyVisible; }
	
	/** \brief There are transparent objects. */
	inline bool GetHasTransparency() const{ return pHasTransparency; }
	
	/** \brief Stencil pass bits have to be cleared. */
	inline bool GetClearStencilPassBits() const{ return pClearStencilPassBits; }
	
	/** \brief Set if stencil pass bits have to be cleared. */
	void SetClearStencilPassBits( bool clear );
	
	/** \brief Clear color target. */
	inline bool GetClearColor() const{ return pClearColor; }
	
	/** \brief Set if color target has to be cleared. */
	void SetClearColor( bool clear );
	
	
	
	/** \brief Number of transparency layers. */
	inline int GetTransparencyLayerCount() const{ return pTransparencyLayerCount; }
	
	/** \brief Set number of transparency layers. */
	void SetTransparencyLayerCount( int count );
	
	/** \brief Current transparency layer number. */
	inline int GetCurrentTransparencyLayer() const{ return pCurTransparencyLayer; }
	
	/** \brief Set current transparency layer number. */
	void SetCurrentTransparencyLayer( int layer );
	
	/** \brief Render pass number. */
	inline int GetRenderPassNumber() const{ return pRenderPassNumber; }
	
	/** \brief Set render pass number. */
	void SetRenderPassNumber( int number );
	
	/** \brief Stencil reference value. */
	inline int GetStencilRefValue() const{ return pStencilRefValue; }
	
	/** \brief Set stencil reference value. */
	void SetStencilRefValue( int refValue );
	
	/** \brief Previous stencil reference value. */
	inline int GetStencilPrevRefValue() const{ return pStencilPrevRefValue; }
	
	/** \brief Set previous stencil reference value. */
	void SetStencilPrevRefValue( int refValue );
	
	/** \brief Stencil write mask. */
	inline int GetStencilWriteMask() const{ return pStencilWriteMask; }
	
	/** \brief Set stencil write mask. */
	void SetStencilWriteMask( int writeMask );
	
	
	
	/** \brief Occlusion map. */
	inline deoglOcclusionMap *GetOcclusionMap() const{ return pOcclusionMap; }
	
	/** \brief Set occlusion map. */
	void SetOcclusionMap( deoglOcclusionMap *map );
	
	/** \brief Occlusion map base level. */
	inline int GetOcclusionMapBaseLevel() const{ return pOcclusionMapBaseLevel; }
	
	/** \brief Set occlusion map base level. */
	void SetOcclusionMapBaseLevel( int level );
	
	/** \brief Occlusion test matrix. */
	inline const decMatrix &GetOcclusionTestMatrix() const{ return pOcclusionTestMatrix; }
	
	/** \brief Set occlusion test matrix. */
	void SetOcclusionTestMatrix( const decMatrix &matrix );
	
	
	
	/** \brief Debug object or \em NULL if not existing. */
	inline deoglRenderPlanDebug *GetDebug() const{ return pDebug; }
	
	/** \brief Debug timing information is printed. */
	inline bool GetDebugTiming() const{ return pDebugTiming; }
	
	/** \brief Set if debug timing information is printed. */
	void SetDebugTiming( bool debugTiming );
	/*@}*/
	
	
	
	/** \name Environment Maps */
	/*@{*/
	/** \brief Number of plan environment maps. */
	inline int GetEnvMapCount() const{ return pEnvMapCount; }
	
	/** \brief Plan environment map at the given index. */
	deoglRenderPlanEnvMap &GetEnvMapAt( int index ) const;
	
	/** \brief Remove environment map from the entire render plan without calling any methods on it. */
	void RemoveEnvMap( deoglEnvironmentMap *envmap );
	
	/** \brief Direct environment map fader. */
	inline deoglEnvMapFader &GetDirectEnvMapFader(){ return pDirectEnvMapFader; }
	inline const deoglEnvMapFader &GetDirectEnvMapFader() const{ return pDirectEnvMapFader; }
	/*@}*/
	
	
	
	/** \name Lights (deprecated) */
	/*@{*/
	/** \brief Number of plan lights. */
	inline int GetLightCount() const{ return pLightCount; }
	
	/** \brief Plan light at the given index. */
	deoglRenderPlanLight *GetLightAt( int index ) const;
	
	/** \brief Plan light for the given light. If no plan light exists yet a new one is created. */
	deoglRenderPlanLight *GetLightFor( deoglRLight *light );
	
	/** \brief Remove all plan lights. */
	void RemoveAllLights();
	/*@}*/
	
	
	
	/** \name Sky lights */
	/*@{*/
	/** \brief Number of sky lights. */
	inline int GetSkyLightCount() const{ return pSkyLightCount; }
	
	/** \brief Sky light at index. */
	deoglRenderPlanSkyLight *GetSkyLightAt( int index ) const;
	
	/** \brief Add sky light. */
	deoglRenderPlanSkyLight *AddSkyLight( deoglRSkyInstanceLayer *layer );
	
	/** \brief Remove sky lights. */
	void RemoveAllSkyLights();
	/*@}*/
	
	
	
	/** \name Sky instances */
	/*@{*/
	/** \brief Count of sky instances. */
	int GetSkyInstanceCount() const;
	
	/** \brief Sky instance at index. */
	deoglRSkyInstance *GetSkyInstanceAt( int index ) const;
	
	/** \brief Remove all sky instances. */
	void RemoveAllSkyInstances();
	
	/** \brief Sky background color. */
	inline const decColor &GetSkyBgColor() const{ return pSkyBgColor; }
	
	/** \brief Set sky background color. */
	void SetSkyBgColor( const decColor &color );
	/*@}*/
	
	
	
	/** \name Sub Plans */
	/*@{*/
	/** \brief Number of masked plans. */
	inline int GetMaskedPlanCount() const{ return pMaskedPlanCount; }
	
	/** \brief Masked plan at the given index. */
	deoglRenderPlanMasked *GetMaskedPlanAt( int index ) const;
	
	/** \brief Add masked plan for the given render plan. */
	deoglRenderPlanMasked *AddMaskedPlanFor( deoglRenderPlan *plan );
	
	/** \brief Remove all masked plans. */
	void RemoveAllMaskedPlans();
	/*@}*/
	
	
	
private:
	void pBarePrepareRender();
	void pPlanSky();
	void pPlanDominance();
	void pPlanShadowCasting();
	void pPlanOcclusionTesting();
	void pPlanCollideList( deoglDCollisionFrustum *frustum );
	void pPlanOcclusionTestInputData();
	void pPlanLODLevels();
	void pPlanEnvMaps();
	void pPlanVisibility( deoglDCollisionFrustum *frustum );
	
	void pDebugPrepare();
	void pDebugVisibleNoCull();
	void pDebugVisibleCulled();
	
	int pIndexOfLightWith( deoglRLight *light ) const;
	
	void pCheckTransparency();
	void pBuildRenderPlan();
	void pBuildSkyLightPlan();
	void pBuildLightPlan();
	void pCalcShadowMemoryConsumption( deoglRenderCacheLight &light,
		deoglRenderCacheLightShadow &shadow, bool withColor );
	void pBuildLightProbes();
	
	void pUpdateHTView();
	
	void pCheckOutsideVisibility();
	
	void pDropLightsDynamic();
};

#endif
