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

#ifndef _DEOGLCONFIGURATION_H_
#define _DEOGLCONFIGURATION_H_

#include <dragengine/common/string/decStringSet.h>


// opengl configuration
/////////////////////////

class deoglConfiguration{
public:
	enum eLogLevels{
		ellError,
		ellWarning,
		ellInfo,
		ellDebug
	};
	
	/** Occlusion Test Mode. */
	enum eOcclusionTestModes{
		/** No occlusion testing. */
		eoctmNone,
		/** VBO to texture occlusion testing. */
		eoctmVBOToTexture,
		/** Transform feedback occlusion testing. */
		eoctmTransformFeedback
	};
	
	/** Environment map method. */
	enum eEnvironmentMapMethods{
		/** Single pre-mixed environment map. */
		eemmSingle,
		/** Multiple per-pixel mixed environment maps without parallax correction. */
		eemmMultipleNoParallax,
		/** Multiple per-pixel mixed environment maps with box projection parallax correction. */
		eemmMultipleBoxProjection
	};
	
	enum eShadowQuality{
		esqOff,
		esqVeryLow,
		esqLow,
		esqMedium,
		esqHigh,
		esqVeryHigh
	};
	
	enum eGIQuality{
		egiqOff,
		egiqVeryLow,
		egiqLow,
		egiqMedium,
		egiqHigh,
		egiqVeryHigh
	};
	
	enum eGIUpdateSpeed{
		egiusOff,
		egiusVeryLow,
		egiusLow,
		egiusMedium,
		egiusHigh,
		egiusVeryHigh
	};
	
	
	
private:
	bool pDirty;
	
	eLogLevels pLogLevel;
	
	float pMapFaceSplitThreshold;
	float pSkyBodyDensity;
	bool pReducedLighting;
	bool pDebugShowCB;
	bool pDebugWireframe;
	bool pDebugUVSplitter;
	bool pDebugPrintSkyUpdate;
	bool pShowLightCB;
	
	eShadowQuality pShadowQuality;
	float pShadowMapOffsetScale;
	float pShadowMapOffsetBias;
	float pShadowCubePCFSize;
	int pOcclusionReduction;
	
	float pDistShadowScale;
	float pDistShadowBias;
	
	float pTextOffsetU;
	float pTextOffsetV;
	bool pUseDepthPass;
	float pLightCutOffIntensity;
	bool pUseTextureCompression;
	int pTranspLayerLimit;
	
	bool pUseOneFBO;
	bool pUseEncodeDepth;
	bool pDisableStencil;
	bool pStencilOnlyOnRB;
	
	bool pDefRenEncDepth;
	bool pDefRenUsePOTs;
	int pDefRenSizeLimit;
	bool pUseHDRR;
	float pHDRRMaxIntensity;
	float pHDRRSceneKey;
	int pRenderDownScale;
	bool pUseInverseDepth;
	
	float pGammaCorrection;
	float pContrast;
	float pBrightness;
	
	float pDecalOffsetScale;
	float pDecalOffsetBias;
	
	int pLODMaxPixelError;
	
	float pNormalRoughnessCorrectionStrength;
	
	bool pSSREnable;
	int pSSRStepCount;
	float pSSRMaxRayLength;
	int pSSRReduction;
	float pSSRCoverageEdgeSize;
	float pSSRCoveragePowerEdge;
	float pSSRCoveragePowerRayLength;
	int pSSRMethod;
	
	bool pSSAOEnable;
	int pSSAOTapCount;
	float pSSAOTurnCount;
	float pSSAOSelfOcclusionAngle;
	float pSSAORadius;
	float pSSAORadiusLimit;
	float pSSAOMipMapBase;
	float pSSAOEdgeBlurThreshold;
	
	bool pAOSelfShadowEnable;
	float pAOSelfShadowSmoothAngle;
	
	bool pSSSSSEnable;
	
	bool pEnvMapEnable;
	bool pEnvMapUseEqui;
	int pEnvMapSize;
	eEnvironmentMapMethods pEnvMapMethod;
	
	int pFrameRateLimit;
	float pAsyncRenderSkipSyncTimeRatio;
	
	bool pDebugContext;
	bool pAsyncRendering;
	bool pEnableRetainImageOptimization;
	
	bool pBugNo2ComponentFBOTex;
	
	bool pDebugNoCulling;
	eOcclusionTestModes pOcclusionTestMode;
	
	int pQuickDebug;
	
	int pDebugSnapshot;
	
	bool pDisableCubeMapLinearFiltering;
	
	int pMaxSPBIndexCount;
	
	eGIQuality pGIQuality;
	eGIUpdateSpeed pGIUpdateSpeed;
	
	decStringSet pDisableExtensions;
	
	float pVRRenderScale;
	int pVRForceFrameRate;
	
	
	
public:
	// constructor, destructor
	deoglConfiguration();
	~deoglConfiguration();
	// parmeters
	
	/** Configuration changed. */
	inline bool GetDirty() const{ return pDirty; }
	
	/** Set configuration changed. */
	void SetDirty( bool dirty );
	
	
	
	/** Log level. */
	inline eLogLevels GetLogLevel() const{ return pLogLevel; }
	
	/** Helper method testing if warnings have to be logged. */
	inline bool GetDoLogWarn() const{ return pLogLevel >= ellWarning; }
	
	/** Helper method testing if info have to be logged. */
	inline bool GetDoLogInfo() const{ return pLogLevel >= ellInfo; }
	
	/** Helper method testing if debug have to be logged. */
	inline bool GetDoLogDebug() const{ return pLogLevel >= ellDebug; }
	
	/** Set log level. */
	void SetLogLevel( eLogLevels logLevel );
	
	/** Shadow quality. */
	inline eShadowQuality GetShadowQuality() const{ return pShadowQuality; }
	
	/** Set shadow quality. */
	void SetShadowQuality( eShadowQuality quality );
	
	inline float GetSkyBodyDensity() const{ return pSkyBodyDensity; }
	inline bool GetUseReducedLighting() const{ return pReducedLighting; }
	inline bool GetDebugShowCB() const{ return pDebugShowCB; }
	inline bool GetDebugWireframe() const{ return pDebugWireframe; }
	inline bool GetDebugUVSplitter() const{ return pDebugUVSplitter; }
	inline bool GetShowLightCB() const{ return pShowLightCB; }
	inline float GetMapFaceSplitThreshold() const{ return pMapFaceSplitThreshold; }
	inline float GetShadowMapOffsetScale() const{ return pShadowMapOffsetScale; }
	inline float GetShadowMapOffsetBias() const{ return pShadowMapOffsetBias; }
	inline float GetTextOffsetU() const{ return pTextOffsetU; }
	inline float GetTextOffsetV() const{ return pTextOffsetV; }
	inline bool GetUseDepthPass() const{ return pUseDepthPass; }
	inline float GetLightCutOffIntensity() const{ return pLightCutOffIntensity; }
	inline bool GetUseTextureCompression() const{ return pUseTextureCompression; }
	void SetSkyBodyDensity( float density );
	void SetUseReducedLighting( bool reduced );
	void SetDebugShowCB( bool show );
	void SetDebugWireframe( bool wireframe );
	void SetDebugUVSplitter( bool uvSplitter );
	void SetShowLightCB( bool show );
	void SetMapFaceSplitThreshold( float threshold );
	void SetShadowMapOffsetScale( float scale );
	void SetShadowMapOffsetBias( float bias );
	void SetTextOffsetU( float offset );
	void SetTextOffsetV( float offset );
	void SetUseDepthPass( bool useDepthPass );
	void SetLightCutOffIntensity( float cutOffIntensity );
	void SetUseTextureCompression( bool useTextureCompression );
	
	/** Transparency layer limit. */
	inline int GetTranspLayerLimit() const{ return pTranspLayerLimit; }
	
	/** Set transparency layer limit. */
	void SetTranspLayerLimit( int limit );
	
	/** Determines if debug messages are printed concerning sky updating and env-map updating. */
	inline bool GetDebugPrintSkyUpdate() const{ return pDebugPrintSkyUpdate; }
	/** Sets if debug messages are printed concerning sky updating and env-map updating. */
	void SetDebugPrintSkyUpdate( bool enable );
	
	/** Retrieves the shadow cube pcf size. */
	inline float GetShadowCubePCFSize() const{ return pShadowCubePCFSize; }
	/** Sets the shadow cube pcf size. */
	void SetShadowCubePCFSize( float pcfSize );
	/** Retrieves the occlusion reduction factor. */
	inline int GetOcclusionReduction() const{ return pOcclusionReduction; }
	/** Sets the occlusion reduction factor. */
	void SetOcclusionReduction( int reductionFactor );
	
	/** Retrieves the offset scaling for distance based shadow mapping. */
	inline float GetDistShadowScale() const{ return pDistShadowScale; }
	/** Sets the offset scaling for distance based shadow mapping. */
	void SetDistShadowScale( float scale );
	/** Retrieves the offset bias for distance based shadow mapping. */
	inline float GetDistShadowBias() const{ return pDistShadowBias; }
	/** Sets the offset bias for distance based shadow mapping. */
	void SetDistShadowBias( float bias );
	
	/** Determines if only one FBO is used ( broken hardware fix ). */
	inline bool GetUseOneFBO() const{ return pUseOneFBO; }
	/** Sets if only one FBO is used ( broken hardware fix ). */
	void SetUseOneFBO( bool useOneFBO );
	/** Determines if depth has to be encoded ( broken hardware fix ). */
	inline bool GetUseEncodeDepth() const{ return pUseEncodeDepth; }
	/** Sets if depth has to be encoded ( broken hardware fix ). */
	void SetUseEncodeDepth( bool useEncodeDepth );
	/** Determines if the stencil buffer is disabled ( broken hardware fix ). */
	inline bool GetDisableStencil() const{ return pDisableStencil; }
	/** Sets if stencil buffer is disabled ( broken hardware fix ). */
	void SetDisableStencil( bool disableStencil );
	/** Determines if the stencil buffer is only used on renderbuffers ( broken hardware fix ). */
	inline bool GetStencilOnlyOnRB() const{ return pStencilOnlyOnRB; }
	/** Sets if stencil buffer is used only on renderbuffers ( broken hardware fix ). */
	void SetStencilOnlyOnRB( bool stencilOnlyOnRB );
	
	/** Determines if depth has to be encoded for deferred rendering. */
	inline bool GetDefRenEncDepth() const{ return pDefRenEncDepth; }
	/** Sets if depth has to be encoded for deferred rendering. */
	void SetDefRenEncDepth( bool useEncDepth );
	/** Determines if power of two (POT) textures should be used for deferred rendering instead of non-POTs. */
	inline bool GetDefRenUsePOTs() const{ return pDefRenUsePOTs; }
	/** Sets if power of two (POT) textures should be used for deferred rendering instead of non-POTs. */
	void SetDefRenUsePOTs( bool usePOTs );
	/** Retrieves the size limit for rendering using deferred rendering or 0 for no limit. */
	inline int GetDefRenSizeLimit() const{ return pDefRenSizeLimit; }
	/** Sets the size limit for rendering using deferred rendering or 0 for no limit. */
	void SetDefRenSizeLimit( int size );
	/** Determines if hdrr is to be used. */
	inline bool GetUseHDRR() const{ return pUseHDRR; }
	/** Sets if hdrr is to be used. */
	void SetUseHDRR( bool useHDRR );
	/** Retrieves the maximum intensity for the hdrr tone mapper. */
	inline float GetHDRRMaximumIntensity() const{ return pHDRRMaxIntensity; }
	/** Sets the maximum intensity for the hdrr tone mapper. */
	void SetHDRRMaximumIntensity( float maximumIntensity );
	
	/** High definition range rendering scene key. */
	inline float GetHDRRSceneKey() const{ return pHDRRSceneKey; }
	
	/** Set high definition range rendering scene key. */
	void SetHDRRSceneKey( float key );
	
	/** Determines if rendering takes place with halve the resolution and then upscaled. */
	inline int GetRenderDownScale() const{ return pRenderDownScale; }
	/** Sets if rendering takes place with halve the resolution and then upscaled. */
	void SetRenderDownScale( int downScale );
	
	/** Enable inverse depth using floating point depth buffer if supported. */
	inline bool GetUseInverseDepth() const{ return pUseInverseDepth; }
	
	/** Set if inverse depth using floating point depth buffer is enabled if supported. */
	void SetUseInverseDepth( bool useInverseDepth );
	
	
	
	/** Retrieves the gamma correction. */
	inline float GetGammaCorrection() const{ return pGammaCorrection; }
	/** Sets the gamma correction. */
	void SetGammaCorrection( float gamma );
	/** Retrieves the contrast. */
	inline float GetContrast() const{ return pContrast; }
	/** Sets the contrast. */
	void SetContrast( float contrast );
	/** Retrieves the brightness. */
	inline float GetBrightness() const{ return pBrightness; }
	/** Sets the brightness. */
	void SetBrightness( float brightness );
	
	/** Retrieves the decal offset scale. */
	inline float GetDecalOffsetScale() const{ return pDecalOffsetScale; }
	/** Sets the decal offset scale. */
	void SetDecalOffsetScale( float scale );
	/** Retrieves the decal offset bias. */
	inline float GetDecalOffsetBias() const{ return pDecalOffsetBias; }
	/** Sets the decal offset bias. */
	void SetDecalOffsetBias( float bias );
	
	/** Retrieves the maximum pixel error allowed for LOD calculation. */
	inline int GetLODMaxPixelError() const{ return pLODMaxPixelError; }
	/** Sets the maximum pixel error allowed for LOD calculation. */
	void SetLODMaxPixelError( int maxPixelError );
	
	/** Retrieves the normal roughness correction strength. */
	inline float GetNormalRoughnessCorrectionStrength() const{ return pNormalRoughnessCorrectionStrength; }
	/** Sets the normal roughness correction strength. */
	void SetNormalRoughnessCorrectionStrength( float strength );
	
	/** Determines if screen space reflections are enabled. */
	inline bool GetSSREnable() const{ return pSSREnable; }
	/** Sets if screen space reflections are enabled. */
	void SetSSREnable( bool enable );
	/** Retrieves the screen space reflection step count. */
	inline int GetSSRStepCount() const{ return pSSRStepCount; }
	/** Sets the screen space reflection step count. */
	void SetSSRStepCount( int stepCount );
	/** Retrieves the screen space reflection maximum ray length. */
	inline float GetSSRMaxRayLength() const{ return pSSRMaxRayLength; }
	/** Sets the screen space reflection maximum ray length. */
	void SetSSRMaxRayLength( float maxRayLength );
	/** Retrieves the screen space reflection reduction. */
	inline int GetSSRReduction() const{ return pSSRReduction; }
	/** Sets the screen space reflection reduction. */
	void SetSSRReduction( int reduction );
	/** Retrieves the screen space reflection coverage edge size. */
	inline float GetSSRCoverageEdgeSize() const{ return pSSRCoverageEdgeSize; }
	/** Sets the screen space reflection coverage edge size. */
	void SetSSRCoverageEdgeSize( float edgeSize );
	/** Retrieves the screen space reflection power factor for edge coverage. */
	inline float GetSSRCoveragePowerEdge() const{ return pSSRCoveragePowerEdge; }
	/** Sets the screen space reflection power factor for edge coverage. */
	void SetSSRCoveragePowerEdge( float power );
	/** Retrieves the screen space reflection power factor for ray length coverage. */
	inline float GetSSRCoveragePowerRayLength() const{ return pSSRCoveragePowerRayLength; }
	/** Sets the screen space reflection power factor for ray length coverage. */
	void SetSSRCoveragePowerRayLength( float power );
	/** Retrieves the ssr method to use. */
	inline int GetSSRMethod() const{ return pSSRMethod; }
	/** Sets the ssr method to use. */
	void SetSSRMethod( int method );
	
	/** Determines if screen space ambient occlusion is enabled. */
	inline bool GetSSAOEnable() const{ return pSSAOEnable; }
	/** Sets if screen space ambient occlusion is enabled. */
	void SetSSAOEnable( bool enable );
	/** Retrieves the screen space ambient occlusion tap count. */
	inline int GetSSAOTapCount() const{ return pSSAOTapCount; }
	/** Sets the screen space ambient occlusion tap count. */
	void SetSSAOTapCount( int tapCount );
	/** Retrieves the screen space ambient occlusion turn count. */
	inline float GetSSAOTurnCount() const{ return pSSAOTurnCount; }
	/** Sets the screen space ambient occlusion turn count. */
	void SetSSAOTurnCount( float turnCount );
	/** Retrieves the screen space ambient occlusion self occlusion angle. */
	inline float GetSSAOSelfOcclusionAngle() const{ return pSSAOSelfOcclusionAngle; }
	/** Sets the screen space ambient occlusion self occlusion angle. */
	void SetSSAOSelfOcclusionAngle( float angle );
	/** Retrieves the screen space ambient occlusion world radius. */
	inline float GetSSAORadius() const{ return pSSAORadius; }
	/** Sets the screen space ambient occlusion world radius. */
	void SetSSAORadius( float radius );
	/** Retrieves the screen space ambient occlusion screen relative radius limit. */
	inline float GetSSAORadiusLimit() const{ return pSSAORadiusLimit; }
	/** Sets the screen space ambient occlusion screen relative radius limit. */
	void SetSSAORadiusLimit( float limit );
	/** Retrieves the screen space ambient occlusion mip map base. */
	inline float GetSSAOMipMapBase() const{ return pSSAOMipMapBase; }
	/** Sets the screen space ambient occlusion mip map base. */
	void SetSSAOMipMapBase( float base );
	/** Retrieves the screen space ambient occlusion edge blur threshold. */
	inline float GetSSAOEdgeBlurThreshold() const{ return pSSAOEdgeBlurThreshold; }
	/** Sets the screen space ambient occlusion edge blur threshold. */
	void SetSSAOEdgeBlurThreshold( float threshold );
	
	/** Determines if ambient occlusion self shadowing enabled. */
	inline bool GetAOSelfShadowEnable() const{ return pAOSelfShadowEnable; }
	/** Sets if ambient occlusion self shadowing enabled. */
	void SetAOSelfShadowEnable( bool enable );
	/** Retrieves the ambient occlusion self shadow smoothing angle. */
	inline float GetAOSelfShadowSmoothAngle() const{ return pAOSelfShadowSmoothAngle; }
	/** Sets the ambient occlusion self shadow smoothing angle. */
	void SetAOSelfShadowSmoothAngle( float angle );
	
	/** Determines if screen space subsurface scattering is enabled. */
	inline bool GetSSSSSEnable() const{ return pSSSSSEnable; }
	/** Sets if screen space subsurface scattering is enabled. */
	void SetSSSSSEnable( bool enable );
	
	/** Determines if in-game rendered environment maps are enabled. */
	inline bool GetEnvMapEnable() const{ return pEnvMapEnable; }
	/** Sets if in-game rendered environment maps are enabled. */
	void SetEnvMapEnable( bool enable );
	/** Determines if equirectangular environment maps are used. */
	inline bool GetEnvMapUseEqui() const{ return pEnvMapUseEqui; }
	/** Sets if equirectangular environment maps are used. */
	void SetEnvMapUseEqui( bool useEqui );
	/** Retrieves the environment map size. */
	inline int GetEnvMapSize() const{ return pEnvMapSize; }
	/** Sets the environment map size. */
	void SetEnvMapSize( int size );
	/** Retrieves the environment map method. */
	inline eEnvironmentMapMethods GetEnvMapMethod() const{ return pEnvMapMethod; }
	/** Sets the environment map method. */
	void SetEnvMapMethod( eEnvironmentMapMethods method );
	
	/** Frame rate limit for render thread. */
	inline int GetFrameRateLimit() const{ return pFrameRateLimit; }
	
	/** Set frame rate limit for render thread. */
	void SetFrameRateLimit( int frameRateLimit );
	
	/** Ratio between game time and render time required to skip synchronization. */
	inline float GetAsyncRenderSkipSyncTimeRatio() const{ return pAsyncRenderSkipSyncTimeRatio; }
	
	/** Set ratio between game time and render time required to skip synchronization. */
	void SetAsyncRenderSkipSyncTimeRatio( float ratio );
	
	
	
	/** Create debug context. */
	inline bool GetDebugContext() const{ return pDebugContext; }
	
	/** Set debug context. */
	void SetDebugContext( bool debugContext );
	
	/** Asynchronous rendering. */
	inline bool GetAsyncRendering() const{ return pAsyncRendering; }
	
	/** Set asynchronous rendering. */
	void SetAsyncRendering( bool asyncRendering );
	
	/** Enable memory optimization using retained image data. */
	inline bool GetEnableRetainImageOptimization() const{ return pEnableRetainImageOptimization; }
	
	/** Set if memory optimization using retained image data is enabled. */
	void SetEnableRetainImageOptimization( bool enable );
	
	
	
	/** Determines if 2-component fbo textures are disabled to counter an nVidia driver bug. */
	inline bool GetBugNo2ComponentFBOTex() const{ return pBugNo2ComponentFBOTex; }
	/** Sets if 2-component fbo textures are disabled to counter an nVidia driver bug. */
	void SetBugNo2ComponentFBOTex( bool bugNo2ComponentFBOTex );
	
	/** Retrieves the quick debug value. */
	inline int GetQuickDebug() const{ return pQuickDebug; }
	/** Sets the quick debug value. */
	void SetQuickDebug( int value );
	/** Determines if software culling is disabled. */
	inline bool GetDebugNoCulling() const{ return pDebugNoCulling; }
	/** Sets if software culling is disabled. */
	void SetDebugNoCulling( bool noCulling );
	/** Retrieves the occlusion test mode. */
	inline eOcclusionTestModes GetOcclusionTestMode() const{ return pOcclusionTestMode; }
	/** Sets the occlusion test mode. */
	void SetOcclusionTestMode( eOcclusionTestModes mode );
	
	/** Retrieves the debug snapshot value. */
	inline int GetDebugSnapshot() const{ return pDebugSnapshot; }
	/** Sets the debug snapshot value. */
	void SetDebugSnapshot( int snapshot );
	
	/** Determines if linear filtering for cube mapping has to be disabled. */
	inline bool GetDisableCubeMapLinearFiltering() const{ return pDisableCubeMapLinearFiltering; }
	/** Set if linear filtering for cube mapping has to be disabled. */
	void SetDisableCubeMapLinearFiltering( bool disableCubeMapLinearFiltering );
	
	/** Maximum shared parameter buffer index count. */
	inline int GetMaxSPBIndexCount() const{ return pMaxSPBIndexCount; }
	
	/** Set maximum shared parameter buffer index count. */
	void SetMaxSPBIndexCount( int count );
	
	inline eGIQuality GetGIQuality() const{ return pGIQuality; }
	void SetGIQuality( eGIQuality quality );
	
	inline eGIUpdateSpeed GetGIUpdateSpeed() const{ return pGIUpdateSpeed; }
	void SetGIUpdateSpeed( eGIUpdateSpeed updateSpeed );
	
	/** OpenGL extensions to disable. */
	inline decStringSet &GetDisableExtensions(){ return pDisableExtensions; }
	inline const decStringSet &GetDisableExtensions() const{ return pDisableExtensions; }
	
	inline float GetVRRenderScale() const{ return pVRRenderScale; }
	void SetVRRenderScale( float scale );
	
	inline int GetVRForceFrameRate() const{ return pVRForceFrameRate; }
	void SetVRForceFrameRate( int framerate );
};

#endif
