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

#include "deoglRenderPlanTasks.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../component/deoglComponentList.h"
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
class deoglOcclusionTest;
class deoglGIState;
class deoglRenderPlanEnvMap;
class deoglRenderPlanDebug;
class deoglRenderPlanLight;
class deoglRenderPlanMasked;
class deoglRenderPlanSkyLight;
class deoglTexture;
class deoglRWorld;
class deoglRSkyInstance;
class deoglRSkyInstanceLayer;
class deoglRPTFindContent;



/**
 * Render Plan.
 * 
 * Stores the render plan used to build an efficient method for rendering
 * a particular shot of a world. Furthermore a render plan stores information
 * to use frame to frame coherency where possible. The render plan is self
 * sustaining not requiring a camera for storing the render parameters.
 * Instead all camera attributes are located in the render plan itself. This
 * way indirect rendering or other forms of intermediate rendering are
 * possible where there exists no explicite camera.
 * 
 * \note It is not possible to store the owner deoglRenderPlanMasked pointer since a render
 *       plan can be visible by multiple other plans and thus can be part of multiple masked
 *       render plans.
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
	decDMatrix pCameraMatrixNonMirrored;
	decDMatrix pProjectionMatrix;
	decDMatrix pFrustumMatrix;
	decDMatrix pRefPosCameraMatrix;
	decDMatrix pRefPosCameraMatrixNonMirrored;
	float pCameraFov;
	float pCameraFovRatio;
	float pCameraImageDistance;
	float pCameraViewDistance;
	float pCameraAdaptedIntensity;
	decVector4 pDepthToPosition;
	decVector2 pDepthSampleOffset;
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
	bool pUseGIState;
	deoglGIState *pUseConstGIState;
	
	bool pUseLayerMask;
	decLayerMask pLayerMask;
	
	deoglFramebuffer *pFBOTarget;
	bool pFBOTargetCopyDepth;
	
	deoglDCollisionFrustum pCustomFrustum;
	bool pUseCustomFrustum;
	decBoundary pCustomFrustumScreenArea;
	deoglDCollisionFrustum *pUseFrustum;
	deoglDCollisionFrustum pCameraFrustum;
	
	bool pDirtyProjMat;
	
	deoglCollideList pCollideList;
	deoglComponentList pComponentsOccMap;
	
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
	
	decPointerList pSkyLights;
	int pSkyLightCount;
	
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
	deoglOcclusionTest *pOcclusionTest;
	int pOcclusionMapBaseLevel;
	decMatrix pOcclusionTestMatrix;
	deoglGIState *pGIState;
	
	deoglRenderPlanTasks pTasks;
	
	deoglRPTFindContent *pTaskFindContent;
	
	deoglRenderPlanDebug *pDebug;
	bool pDebugTiming;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan. */
	deoglRenderPlan( deoglRenderThread &renderThread );
	
	/** Clean up render plan. */
	~deoglRenderPlan();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** World. */
	inline deoglRWorld *GetWorld() const{ return pWorld; }
	
	/** Set world. */
	void SetWorld( deoglRWorld *world );
	
	/** Level. */
	inline int GetLevel() const{ return pLevel; }
	
	/** Set level. */
	void SetLevel( int level );
	
	
	
	/** All lights are disabled. */
	inline bool GetDisableLights() const{ return pDisableLights; }
	
	/** Shadow map sizes are rescaled relative to the viewport size. */
	inline bool GetRescaleShadowMaps() const{ return pRescaleShadowMaps; }
	
	/** Set if shadow map sizes are rescaled relative to the viewport size. */
	void SetRescaleShadowMaps( bool rescale );
	
	/** Shadow map size. */
	inline int GetShadowMapSize() const{ return pShadowMapSize; }
	
	/** Shadow cube size. */
	inline int GetShadowCubeSize() const{ return pShadowCubeSize; }
	
	/** Shadow sky size. */
	inline int GetShadowSkySize() const{ return pShadowSkySize; }
	
	/** Dominance. */
	inline float GetDominance() const{ return pDominance; }
	
	/** Forced shadow map size or 0 to use the default shadow map size. */
	inline int GetForceShadowMapSize() const{ return pForceShadowMapSize; }
	
	/** Set forced shadow map size or 0 to use the default shadow map size. */
	void SetForceShadowMapSize( int forcedSize );
	
	
	
	/** Prepare plan for rendering. */
	void PrepareRender( const deoglRenderPlanMasked *mask );
	
	/**
	 * Prepare whatever can be prepared for a one turn of rendering.
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
	 * Plan transparency.
	 * \details Called after transparency counter is ready and planing is possible.
	 * \param[in] layerCount Number of transparent layers. How this is calculated is the
	 *                       responsibility of the caller.
	 */
	void PlanTransparency( int layerCount );
	
	/** Render using the previously prepared plan. */
	void Render();
	
	/** Clean up after rendering. */
	void CleanUp();
	
	
	
	/** Camera or \em NULL. */
	inline deoglRCamera *GetCamera() const{ return pCamera; }
	
	/** Set camera or \em NULL. */
	void SetCamera( deoglRCamera *camera );
	
	/** Camera matrix. */
	inline const decDMatrix &GetCameraMatrix() const{ return pCameraMatrix; }
	
	/** Inverse camera matrix. */
	inline const decDMatrix &GetInverseCameraMatrix() const{ return pCameraInverseMatrix; }
	
	/** Mirror free camera matrix. */
	inline const decDMatrix &GetCameraMatrixNonMirrored() const{ return pCameraMatrixNonMirrored; }
	
	/** Set camera matrix including the inverse one. */
	void SetCameraMatrix( const decDMatrix &matrix );
	
	/** Set mirror free camera matrix. */
	void SetCameraMatrixNonMirrored( const decDMatrix &matrix );
	
	/** Projection matrix. */
	inline const decDMatrix &GetProjectionMatrix() const{ return pProjectionMatrix; }
	
	/** Frustum matrix. */
	inline const decDMatrix &GetFrustumMatrix() const{ return pFrustumMatrix; }
	
	/** Reference position camera matrix. */
	inline const decDMatrix &GetRefPosCameraMatrix() const{ return pRefPosCameraMatrix; }
	
	/** Reference position camera matrix mirror free. */
	inline const decDMatrix &GetRefPosCameraMatrixNonMirrored() const{ return pRefPosCameraMatrixNonMirrored; }
	
	/** Camera position in world space. */
	inline const decDVector &GetCameraPosition() const{ return pCameraPosition; }
	
	/** Camera field of view. */
	inline float GetCameraFov() const{ return pCameraFov; }
	
	/** Camera field of view ratio. */
	inline float GetCameraFovRatio() const{ return pCameraFovRatio; }
	
	/** Camera image distance. */
	inline float GetCameraImageDistance() const{ return pCameraImageDistance; }
	
	/** Camera view distance. */
	inline float GetCameraViewDistance() const{ return pCameraViewDistance; }
	
	/** Set camera parameters. */
	void SetCameraParameters( float fov, float fovRatio, float imageDistance, float viewDistance );
	
	/** Camera adapted intensity if camera is NULL. */
	inline float GetCameraAdaptedIntensity() const{ return pCameraAdaptedIntensity; }
	
	/** Set camera adapted intensity if camera is NULL. */
	void SetCameraAdaptedIntensity( float intensity );
	
	/** Copy camera parameters from another render plan. */
	void CopyCameraParametersFrom( const deoglRenderPlan &plan );
	
	/** Update reference position camera matrix. */
	void UpdateRefPosCameraMatrix();
	
	
	
	/** Depth to position transformation factors. */
	inline const decVector4 &GetDepthToPosition() const{ return pDepthToPosition; }
	
	/** Depth sample offset. */
	inline const decVector2 &GetDepthSampleOffset() const{ return pDepthSampleOffset; }
	
	
	
	/** Vierwport x. */
	inline int GetViewportX() const{ return pViewportX; }
	
	/** Vierwport y. */
	inline int GetViewportY() const{ return pViewportY; }
	
	/** Vierwport width. */
	inline int GetViewportWidth() const{ return pViewportWidth; }
	
	/** Viewport height. */
	inline int GetViewportHeight() const{ return pViewportHeight; }
	
	/** Ratio between the width and the height. */
	inline float GetAspectRatio() const{ return pAspectRatio; }
	
	/** Set viewport parameters. */
	void SetViewport( int x, int y, int width, int height );
	
	
	
	/** Target framebuffer object or \em NULL to use the one of the active graphics context. */
	inline deoglFramebuffer *GetFBOTarget() const{ return pFBOTarget; }
	
	/** Set target framebuffer object or \em NULL to use the one of the active graphics context. */
	void SetFBOTarget( deoglFramebuffer *fbo );
	
	/** Depth is copied to the target framebuffer object if not \em NULL. */
	inline bool GetFBOTargetCopyDepth() const{ return pFBOTargetCopyDepth; }
	
	/** Set if depth is copied to the target framebuffer object if not \em NULL. */
	void SetFBOTargetCopyDepth( bool copyDepth );
	
	
	
	/** Upscale width. */
	inline int GetUpscaleWidth() const{ return pUpscaleWidth; }
	
	/** Upscale height. */
	inline int GetUpscaleHeight() const{ return pUpscaleHeight; }
	
	/** Set upscale size. */
	void SetUpscaleSize( int width, int height );
	
	/** Upscaling has to be used. */
	inline bool GetUseUpscaling() const{ return pUseUpscaling; }
	
	/** Set if upscaling has to be used. */
	void SetUseUpscaling( bool useUpscaling );
	
	/** Upside down rendering has to be used. */
	inline bool GetUpsideDown() const{ return pUpsideDown; }
	
	/** Set if upside down rendering has to be used. */
	void SetUpsideDown( bool upsideDown );
	
	/** Tone mapping is used. */
	inline bool GetUseToneMap() const{ return pUseToneMap; }
	
	/** Set if tone mapping is used. */
	void SetUseToneMap( bool useToneMap );
	
	/** Dynamic components are ignored. */
	inline bool GetIgnoreDynamicComponents() const{ return pIgnoreDynamicComponents; }
	
	/** Set if dynamic components are ignored. */
	void SetIgnoreDynamicComponents( bool ignoreStaticComponents );
	
	/** Debug pass is rendered. */
	inline bool GetRenderDebugPass() const{ return pRenderDebugPass; }
	
	/** Set if debug pass is rendered. */
	void SetRenderDebugPass( bool render );
	
	/** No reflections are used. */
	inline bool GetNoReflections() const{ return pNoReflections; }
	
	/** Set if no reflections are used. */
	void SetNoReflections( bool noReflections );
	
	/** Use GI state. */
	inline bool GetUseGIState() const{ return pUseGIState; }
	
	/** Set use GI state. */
	void SetUseGIState( bool useGIState );
	
	/** Use constant GI state. */
	inline deoglGIState *GetUseConstGIState() const{ return pUseConstGIState; }
	
	/** Set use const GI state. */
	void SetUseConstGIState( deoglGIState *giState );
	
	
	
	/** Layer mask is used for culling. */
	inline bool GetUseLayerMask() const{ return pUseLayerMask; }
	
	/** Set if layer mask is used for culling. */
	void SetUseLayerMask( bool useLayerMask );
	
	/** Layer mask. */
	const decLayerMask &GetLayerMask(){ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** Set custom frustum. */
	void SetCustomFrustumBoundaries( const decDVector &position, const decDVector &topLeft,
		const decDVector &topRight, const decDVector &bottomLeft, const decDVector &bottomRight,
		double near, double far );
	
	/** Custom frustum is used. */
	inline bool GetUseCustomFrustum() const{ return pUseCustomFrustum; }
	
	/** Set if custom frustum is used. */
	void SetUseCustomFrustum( bool useCustomFrustum );
	
	/** Custom frustum screen area. */
	inline const decBoundary &GetCustomFrustumScreenArea() const{ return pCustomFrustumScreenArea; }
	
	/** Set custom frustum screen area. */
	void SetCustomFrustumScreenArea( const decBoundary &area );
	
	/** Frustum to use. */
	inline deoglDCollisionFrustum *GetUseFrustum() const{ return pUseFrustum; }
	
	
	
	/** Height terrain view. */
	inline deoglHTView *GetHeightTerrainView() const{ return pHTView; }
	
	/** Collider list. */
	inline deoglCollideList &GetCollideList(){ return pCollideList; }
	
	/** Occlusion map component list. */
	inline deoglComponentList &GetComponentsOccMap(){ return pComponentsOccMap; }
	
	
	
	/** Ignore occlusion meshes if owner component has rendered skins. */
	inline bool GetNoRenderedOccMesh(){ return pNoRenderedOccMesh; }
	
	/** Set to ignore occlusion meshes if owner component has rendered skins. */
	void SetNoRenderedOccMesh( bool noRenderedOccMesh );
	
	/** Culling flags have to be flipped. */
	inline bool GetFlipCulling() const{ return pFlipCulling; }
	
	/** Set if culling flags have to be flipped. */
	void SetFlipCulling( bool flipCulling );
	
	
	
	/** Pass is empty. */
	inline bool GetEmptyPass() const{ return pEmptyPass; }
	
	/** Set if pass is empty. */
	void SetEmptyPass( bool emptyPass );
	
	
	
	/** Sky is visible. */
	inline bool GetSkyVisible() const{ return pSkyVisible; }
	
	/** There are transparent objects. */
	inline bool GetHasTransparency() const{ return pHasTransparency; }
	
	/** Stencil pass bits have to be cleared. */
	inline bool GetClearStencilPassBits() const{ return pClearStencilPassBits; }
	
	/** Set if stencil pass bits have to be cleared. */
	void SetClearStencilPassBits( bool clear );
	
	/** Clear color target. */
	inline bool GetClearColor() const{ return pClearColor; }
	
	/** Set if color target has to be cleared. */
	void SetClearColor( bool clear );
	
	
	
	/** Number of transparency layers. */
	inline int GetTransparencyLayerCount() const{ return pTransparencyLayerCount; }
	
	/** Set number of transparency layers. */
	void SetTransparencyLayerCount( int count );
	
	/** Current transparency layer number. */
	inline int GetCurrentTransparencyLayer() const{ return pCurTransparencyLayer; }
	
	/** Set current transparency layer number. */
	void SetCurrentTransparencyLayer( int layer );
	
	/** Render pass number. */
	inline int GetRenderPassNumber() const{ return pRenderPassNumber; }
	
	/** Set render pass number. */
	void SetRenderPassNumber( int number );
	
	/** Stencil reference value. */
	inline int GetStencilRefValue() const{ return pStencilRefValue; }
	
	/** Set stencil reference value. */
	void SetStencilRefValue( int refValue );
	
	/** Previous stencil reference value. */
	inline int GetStencilPrevRefValue() const{ return pStencilPrevRefValue; }
	
	/** Set previous stencil reference value. */
	void SetStencilPrevRefValue( int refValue );
	
	/** Stencil write mask. */
	inline int GetStencilWriteMask() const{ return pStencilWriteMask; }
	
	/** Set stencil write mask. */
	void SetStencilWriteMask( int writeMask );
	
	
	
	/** Occlusion map. */
	inline deoglOcclusionMap *GetOcclusionMap() const{ return pOcclusionMap; }
	
	/** Set occlusion map. */
	void SetOcclusionMap( deoglOcclusionMap *occlusionMap );
	
	/** Occlusion test. */
	inline deoglOcclusionTest *GetOcclusionTest() const{ return pOcclusionTest; }
	
	/** Set occlusion test. */
	void SetOcclusionTest( deoglOcclusionTest *occlusionTest );
	
	/** Occlusion map base level. */
	inline int GetOcclusionMapBaseLevel() const{ return pOcclusionMapBaseLevel; }
	
	/** Set occlusion map base level. */
	void SetOcclusionMapBaseLevel( int level );
	
	/** Occlusion test matrix. */
	inline const decMatrix &GetOcclusionTestMatrix() const{ return pOcclusionTestMatrix; }
	
	/** Set occlusion test matrix. */
	void SetOcclusionTestMatrix( const decMatrix &matrix );
	
	
	
	/** Global illumination state or NULL. */
	inline deoglGIState *GetGIState() const{ return pGIState; }
	
	/** GI state to update or NULL. */
	deoglGIState *GetUpdateGIState() const;
	
	/** GI state to render or NULL. */
	deoglGIState *GetRenderGIState() const;
	
	
	
	/** Tasks. */
	inline deoglRenderPlanTasks &GetTasks(){ return pTasks; }
	
	
	
	/** Debug object or \em NULL if not existing. */
	inline deoglRenderPlanDebug *GetDebug() const{ return pDebug; }
	
	/** Debug timing information is printed. */
	inline bool GetDebugTiming() const{ return pDebugTiming; }
	
	/** Set if debug timing information is printed. */
	void SetDebugTiming( bool debugTiming );
	/*@}*/
	
	
	
	/** \name Environment Maps */
	/*@{*/
	/** Number of plan environment maps. */
	inline int GetEnvMapCount() const{ return pEnvMapCount; }
	
	/** Plan environment map at the given index. */
	deoglRenderPlanEnvMap &GetEnvMapAt( int index ) const;
	
	/** Remove environment map from the entire render plan without calling any methods on it. */
	void RemoveEnvMap( deoglEnvironmentMap *envmap );
	
	/** Direct environment map fader. */
	inline deoglEnvMapFader &GetDirectEnvMapFader(){ return pDirectEnvMapFader; }
	inline const deoglEnvMapFader &GetDirectEnvMapFader() const{ return pDirectEnvMapFader; }
	/*@}*/
	
	
	
	/** \name Lights (deprecated) */
	/*@{*/
	/** Number of plan lights. */
	inline int GetLightCount() const{ return pLightCount; }
	
	/** Plan light at the given index. */
	deoglRenderPlanLight *GetLightAt( int index ) const;
	
	/** Plan light for the given light. If no plan light exists yet a new one is created. */
	deoglRenderPlanLight *GetLightFor( deoglRLight *light );
	
	/** Remove all plan lights. */
	void RemoveAllLights();
	/*@}*/
	
	
	
	/** \name Sky lights */
	/*@{*/
	/** Count of sky lights. */
	inline int GetSkyLightCount() const{ return pSkyLightCount; }
	
	/** Sky light at index. */
	deoglRenderPlanSkyLight *GetSkyLightAt( int index ) const;
	
	/** Remove sky lights. */
	void RemoveAllSkyLights();
	
	/** Sky lights start building render tasks. */
	void SkyLightsStartBuildRT();
	/*@}*/
	
	
	
	/** \name Sky instances */
	/*@{*/
	/** Count of sky instances. */
	int GetSkyInstanceCount() const;
	
	/** Sky instance at index. */
	deoglRSkyInstance *GetSkyInstanceAt( int index ) const;
	
	/** Remove all sky instances. */
	void RemoveAllSkyInstances();
	
	/** Sky background color. */
	inline const decColor &GetSkyBgColor() const{ return pSkyBgColor; }
	
	/** Set sky background color. */
	void SetSkyBgColor( const decColor &color );
	/*@}*/
	
	
	
	/** \name Sub Plans */
	/*@{*/
	/** Number of masked plans. */
	inline int GetMaskedPlanCount() const{ return pMaskedPlanCount; }
	
	/** Masked plan at the given index. */
	deoglRenderPlanMasked *GetMaskedPlanAt( int index ) const;
	
	/** Add masked plan for the given render plan. */
	deoglRenderPlanMasked *AddMaskedPlanFor( deoglRenderPlan *plan );
	
	/** Remove all masked plans. */
	void RemoveAllMaskedPlans();
	/*@}*/
	
	
	
private:
	void pBarePrepareRender( const deoglRenderPlanMasked *mask );
	void pPlanCamera();
	void pPlanSky();
	void pPlanSkyLight();
	void pPlanDominance();
	void pPlanShadowCasting();
	void pStartFindContent();
	void pWaitFinishedFindContent();
	void pPlanGI();
	void pUpdateGI();
	void pPlanLODLevels();
	void pPlanEnvMaps();
	void pRenderOcclusionTests( const deoglRenderPlanMasked *mask );
	void pFinishOcclusionTests( const deoglRenderPlanMasked *mask );
	
	void pDebugPrepare();
	void pDebugVisibleNoCull();
	void pDebugVisibleCulled();
	
	int pIndexOfLightWith( deoglRLight *light ) const;
	
	void pCheckTransparency();
	void pBuildRenderPlan();
	void pBuildLightPlan();
	
	void pUpdateHTView();
	void pUpdateHTViewRTSInstances();
	
	void pCheckOutsideVisibility();
	
	void pDropLightsDynamic();
};

#endif
