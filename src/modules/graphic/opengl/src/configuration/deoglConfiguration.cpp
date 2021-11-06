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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglConfiguration.h"

#include <dragengine/dragengine_configuration.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoglConfiguration
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConfiguration::deoglConfiguration() :
pDirty( false ),

pLogLevel( ellInfo ),

pMapFaceSplitThreshold( 5.0f ), // largest allowed face edge in meters
pSkyBodyDensity( 1.0f ), // render 100% sky bodies
pReducedLighting( false ),
pDebugShowCB( false ),
pDebugWireframe( false ),
pDebugUVSplitter( true ),
pDebugPrintSkyUpdate( false ),
pShowLightCB( false ),

pShadowQuality( esqHigh ),
pShadowMapOffsetScale( 1.1f ),
pShadowMapOffsetBias( 4.0f ),
pShadowCubePCFSize( 1.0f ),
pOcclusionReduction( 1 ),

pDistShadowScale( 1.0f ),
pDistShadowBias( 0.001f ), //4.0f ), // 24-bit: 1 step is 1.192093e-7

pTextOffsetU( 0.0f ),
pTextOffsetV( 0.0f ),
pUseDepthPass( true ),
pLightCutOffIntensity( 0.01f ),
pUseTextureCompression( true ),
pTranspLayerLimit( 4 ),

pUseOneFBO( false ),
pUseEncodeDepth( false ),
pDisableStencil( false ),
pStencilOnlyOnRB( false ),

pDefRenEncDepth( false ),
pDefRenUsePOTs( false ),
pDefRenSizeLimit( 0 ),
pUseHDRR( true ),
pHDRRMaxIntensity( 1.5f ),
pHDRRSceneKey( 0.18f ),
pRenderDownScale( 1 ),
pUseInverseDepth( true ),

pGammaCorrection( 1.0f ),
pContrast( 1.0f ),
pBrightness( 0.0f ),

pDecalOffsetScale( -1.5f ), //-0.1f;
pDecalOffsetBias( -8.0f ), //-1.0f;

pLODMaxPixelError( 2 ),

pNormalRoughnessCorrectionStrength( 0.75f ), // 0.5f;

pSSREnable( true ),
pSSRStepCount( 20 ),
pSSRMaxRayLength( 1.0f ),
pSSRReduction( 1 ), //2
pSSRCoverageEdgeSize( 0.1f ),
pSSRCoveragePowerEdge( 1.0f ),
pSSRCoveragePowerRayLength( 8.0f ),
pSSRMethod( 1 ), // method 2 makes no sense anymore

pSSAOEnable( true ),
pSSAOTapCount( 18 ), // 9-18
pSSAOTurnCount( 7.0f ),
pSSAOSelfOcclusionAngle( 10.0f ), // 10-30
pSSAORadius( 0.5f ), // 0.5m
pSSAORadiusLimit( 0.5f ), // 50% screen size
pSSAOMipMapBase( 8.0f ), // 4-16 = 2^2 - 2^4
pSSAOEdgeBlurThreshold( 5.0f ), // 2dm difference on 1m

pAOSelfShadowEnable( true ),
pAOSelfShadowSmoothAngle( 10.0f ), //6.0f;

pSSSSSEnable( true ),

pEnvMapEnable( true ),
pEnvMapUseEqui( false ),
pEnvMapSize( 256 ),
pEnvMapMethod( eemmMultipleBoxProjection ),

pFrameRateLimit( 0 ), // 0 means use display refresh rate
pAsyncRenderSkipSyncTimeRatio( 0.5 ),

pDebugContext( false ),
pAsyncRendering( true ),
pEnableRetainImageOptimization( true ),

pBugNo2ComponentFBOTex( false ),

pDebugNoCulling ( false ),
pOcclusionTestMode( eoctmTransformFeedback ),

pQuickDebug( 0 ),

pDebugSnapshot( 0 ),

pDisableCubeMapLinearFiltering( false ),

pMaxSPBIndexCount( 10000 ),

pGIQuality( egiqHigh ),
pGIUpdateSpeed( egiusMedium ),

pVRRenderScale( 1.0f ),
pVRForceFrameRate( 0 )
{
	#ifdef OS_ANDROID
	// android is too weak a platform right now to support advanced features out of the
	// box. to avoid devices bogging down until users have a chance to change options
	// set the defaults to reasonably weak settings in the mean time. later on this
	// will be fixed with the obligatory initial performance testing mode of launchers.
	// 
	// the weakest point on android is fill-rate compared. parameters for pc are geared
	// for devices with regular pixel size. android has a much higher pixel density.
	// to reduce fill-rate the resolution of off-screen textures and rendering can be
	// reduced a lot and still obtain satifactory results. furthermore upscaled
	// rendering is used. this reduces pixel work a lot while upscaling is rendered
	// unimportant by the high pixel density. hardware scaler could be used but this
	// would affect also 2D graphics. the default should only cut down on the heavy
	// processing of 3D graphics while keeping 2D graphics crisp if possible
	pShadowQuality = esqVeryLow;
	pShadowMapSize = 256; //1024
	pShadowCubeSize = 256; //1024
	pOcclusionReduction = 2; //1
	pRenderDownScale = 2; //1
	
	// disable advanced render effects requiring a strong GPU with good fill-rate.
	// the user can enable these options if he really wants them at his own risk.
	// as reference speed the GPU timed render effect pass is compared to a GPU timed
	// reference value of sky render pass which is a pure fill-rate operation using
	// a simple texture-only shader.
	// - sky render pass: 2.5ms
	
	// ssao pass consumes roughly 9ms. this is roughly 3.5 times the sky render pass.
	// enabling this effect is reasonable unless performance turns really bad
	pSSAOEnable = true;
	
	// screen space sub surface scattering consumes roughly 2.5ms . this value is
	// though with no pixels to shader and should be higher if actually used. kept
	// enabled until further performance values are available
	//pSSSSSEnable = true;
	pSSSSSEnable = false; // disabled since 4 draw buffers are not enough to use it
	
	// screen space reflection consumes toughly 40ms. this is a major speed killed
	// on android platforms. disabled and not recommended to be used
	pSSREnable = false; //true
	
	// enable logging by default
	pLogLevel = ellDebug;
	
	// make sure retain image memory optimization is enabled since memory is scarce
	pEnableRetainImageOptimization = true;
	
	// debug
	pDebugContext = true;
	pLogLevel = ellDebug;
	
	// disable asynchronous rendering for debug purpose
	//pAsyncRendering = false;
	#endif
}

deoglConfiguration::~deoglConfiguration(){
}



// Parmeters
//////////////

void deoglConfiguration::SetDirty( bool dirty ){
	pDirty = dirty;
}



void deoglConfiguration::SetLogLevel( eLogLevels logLevel ){
	if( logLevel == pLogLevel ){
		return;
	}
	pLogLevel = logLevel;
	pDirty = true;
}

void deoglConfiguration::SetSkyBodyDensity( float density ){
	density = decMath::clamp( density, 0.0f, 1.0f );
	if( fabsf( density - pSkyBodyDensity ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSkyBodyDensity = density;
	pDirty = true;
}

void deoglConfiguration::SetUseReducedLighting( bool reduced ){
	if( reduced == pReducedLighting ){
		return;
	}
	pReducedLighting = reduced;
	pDirty = true;
}

void deoglConfiguration::SetDebugShowCB( bool show ){
	if( show == pDebugShowCB ){
		return;
	}
	pDebugShowCB = show;
	pDirty = true;
}

void deoglConfiguration::SetDebugWireframe( bool wireframe ){
	if( wireframe == pDebugWireframe ){
		return;
	}
	pDebugWireframe = wireframe;
	pDirty = true;
}

void deoglConfiguration::SetDebugUVSplitter( bool uvSplitter ){
	if( uvSplitter == pDebugUVSplitter ){
		return;
	}
	pDebugUVSplitter = uvSplitter;
	pDirty = true;
}

void deoglConfiguration::SetDebugPrintSkyUpdate( bool enable ){
	if( enable == pDebugPrintSkyUpdate ){
		return;
	}
	pDebugPrintSkyUpdate = enable;
	pDirty = true;
}

void deoglConfiguration::SetShowLightCB( bool show ){
	if( show == pShowLightCB ){
		return;
	}
	pShowLightCB = show;
	pDirty = true;
}

void deoglConfiguration::SetMapFaceSplitThreshold( float threshold ){
	threshold = decMath::max( threshold, 0.0f );
	if( fabsf( threshold - pMapFaceSplitThreshold ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pMapFaceSplitThreshold = threshold;
	pDirty = true;
}



void deoglConfiguration::SetShadowQuality( eShadowQuality quality ){
	if( quality == pShadowQuality ){
		return;
	}
	
	pShadowQuality = quality;
	pDirty = true;
}

void deoglConfiguration::SetShadowMapOffsetScale( float scale ){
	if( fabsf( scale - pShadowMapOffsetScale ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pShadowMapOffsetScale = scale;
	pDirty = true;
}

void deoglConfiguration::SetShadowMapOffsetBias( float bias ){
	if( fabsf( bias - pShadowMapOffsetBias ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pShadowMapOffsetBias = bias;
	pDirty = true;
}

void deoglConfiguration::SetShadowCubePCFSize( float pcfSize ){
	pcfSize = decMath::max( pcfSize, 0.0f );
	if( fabsf( pcfSize - pShadowCubePCFSize ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pShadowCubePCFSize = pcfSize;
	pDirty = true;
}

void deoglConfiguration::SetOcclusionReduction( int reductionFactor ){
	reductionFactor = decMath::max( reductionFactor, 1 );
	if( reductionFactor == pOcclusionReduction ){
		return;
	}
	pOcclusionReduction = reductionFactor;
	pDirty = true;
}



void deoglConfiguration::SetDistShadowScale( float scale ){
	if( fabsf( scale - pDistShadowScale ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pDistShadowScale = scale;
	pDirty = true;
}

void deoglConfiguration::SetDistShadowBias( float bias ){
	if( fabsf( bias - pDistShadowBias ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pDistShadowBias = bias;
	pDirty = true;
}



void deoglConfiguration::SetTextOffsetU( float offset ){
	if( fabsf( offset - pTextOffsetU ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pTextOffsetU = offset;
	pDirty = true;
}

void deoglConfiguration::SetTextOffsetV( float offset ){
	if( fabsf( offset - pTextOffsetV ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pTextOffsetV = offset;
	pDirty = true;
}

void deoglConfiguration::SetUseDepthPass( bool useDepthPass ){
	if( useDepthPass == pUseDepthPass ){
		return;
	}
	pUseDepthPass = useDepthPass;
	pDirty = true;
}

void deoglConfiguration::SetLightCutOffIntensity( float cutOffIntensity ){
	cutOffIntensity = decMath::clamp( cutOffIntensity, 1e-4f, 0.5f );
	if( fabsf( cutOffIntensity - pLightCutOffIntensity ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pLightCutOffIntensity = cutOffIntensity;
	pDirty = true;
}

void deoglConfiguration::SetUseTextureCompression( bool useTextureCompression ){
	if( useTextureCompression == pUseTextureCompression ){
		return;
	}
	pUseTextureCompression = useTextureCompression;
	pDirty = true;
}

void deoglConfiguration::SetTranspLayerLimit( int limit ){
	limit = decMath::clamp( limit, 1, 15 );
	if( limit == pTranspLayerLimit ){
		return;
	}
	
	pTranspLayerLimit = limit;
	pDirty = true;
}



void deoglConfiguration::SetUseOneFBO( bool useOneFBO ){
	if( useOneFBO == pUseOneFBO ){
		return;
	}
	pUseOneFBO = useOneFBO;
	pDirty = true;
}

void deoglConfiguration::SetUseEncodeDepth( bool useEncodeDepth ){
	//pUseEncodeDepth = useEncodeDepth;
	//pDirty = true;
}

void deoglConfiguration::SetDisableStencil( bool disableStencil ){
	if( disableStencil == pDisableStencil ){
		return;
	}
	pDisableStencil = disableStencil;
	pDirty = true;
}

void deoglConfiguration::SetStencilOnlyOnRB( bool stencilOnlyOnRB ){
	if( stencilOnlyOnRB == pStencilOnlyOnRB ){
		return;
	}
	pStencilOnlyOnRB = stencilOnlyOnRB;
	pDirty = true;
}



void deoglConfiguration::SetDefRenEncDepth( bool useEncDepth ){
	//pDefRenEncDepth = useEncDepth;
	//pDirty = true;
}

void deoglConfiguration::SetDefRenUsePOTs( bool usePOTs ){
	if( usePOTs == pDefRenUsePOTs ){
		return;
	}
	pDefRenUsePOTs = usePOTs;
	pDirty = true;
}

void deoglConfiguration::SetDefRenSizeLimit( int size ){
	size = decMath::max( size, 0 );
	if( size == pDefRenSizeLimit ){
		return;
	}
	pDefRenSizeLimit = size;
	pDirty = true;
}

void deoglConfiguration::SetUseHDRR( bool useHDRR ){
	if( useHDRR == pUseHDRR ){
		return;
	}
	pUseHDRR = useHDRR;
	pDirty = true;
}

void deoglConfiguration::SetHDRRMaximumIntensity( float maximumIntensity ){
	if( fabsf( maximumIntensity - pHDRRMaxIntensity ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pHDRRMaxIntensity = maximumIntensity;
	pDirty = true;
}

void deoglConfiguration::SetHDRRSceneKey( float key ){
	if( fabsf( key - pHDRRSceneKey ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pHDRRSceneKey = key;
	pDirty = true;
}

void deoglConfiguration::SetRenderDownScale( int downScale ){
	if( downScale < 1 ){
		downScale = 1;
	}
	if( downScale == pRenderDownScale ){
		return;
	}
	pRenderDownScale = downScale;
	pDirty = true;
}

void deoglConfiguration::SetUseInverseDepth( bool useInverseDepth ){
	if( useInverseDepth == pUseInverseDepth ){
		return;
	}
	pUseInverseDepth = useInverseDepth;
	pDirty = true;
}



void deoglConfiguration::SetGammaCorrection( float gamma ){
	gamma = decMath::max( gamma, 0.01f );
	if( fabsf( gamma - pGammaCorrection ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pGammaCorrection = gamma;
	pDirty = true;
}

void deoglConfiguration::SetContrast( float contrast ){
	if( fabsf( contrast - pContrast ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pContrast = contrast;
	pDirty = true;
}

void deoglConfiguration::SetBrightness( float brightness ){
	if( fabsf( brightness - pBrightness ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pBrightness = brightness;
	pDirty = true;
}



void deoglConfiguration::SetDecalOffsetScale( float scale ){
	if( fabsf( scale - pDecalOffsetScale ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pDecalOffsetScale = scale;
	pDirty = true;
}

void deoglConfiguration::SetDecalOffsetBias( float bias ){
	if( fabsf( bias - pDecalOffsetBias ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pDecalOffsetBias = bias;
	pDirty = true;
}



void deoglConfiguration::SetLODMaxPixelError( int maxPixelError ){
	maxPixelError = decMath::max( maxPixelError, 1 );
	if( maxPixelError == pLODMaxPixelError ){
		return;
	}
	pLODMaxPixelError = maxPixelError;
	pDirty = true;
}



void deoglConfiguration::SetNormalRoughnessCorrectionStrength( float strength ){
	strength = decMath::clamp( strength, 0.0f, 1.0f );
	if( fabsf( strength - pNormalRoughnessCorrectionStrength ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pNormalRoughnessCorrectionStrength = strength;
	pDirty = true;
}



void deoglConfiguration::SetSSREnable( bool enable ){
	if( enable == pSSREnable ){
		return;
	}
	pSSREnable = enable;
	pDirty = true;
}

void deoglConfiguration::SetSSRStepCount( int stepCount ){
	stepCount = decMath::max( stepCount, 1 );
	if( stepCount == pSSRStepCount ){
		return;
	}
	pSSRStepCount = stepCount;
	pDirty = true;
}

void deoglConfiguration::SetSSRMaxRayLength( float maxRayLength ){
	maxRayLength = decMath::clamp( maxRayLength, 0.01f, 1.0f );
	if( fabsf( maxRayLength - pSSRMaxRayLength ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSRMaxRayLength = maxRayLength;
	pDirty = true;
}

void deoglConfiguration::SetSSRReduction( int reduction ){
	reduction = decMath::max( reduction, 1 );
	if( reduction == pSSRReduction ){
		return;
	}
	pSSRReduction = reduction;
	pDirty = true;
}

void deoglConfiguration::SetSSRCoverageEdgeSize( float edgeSize ){
	edgeSize = decMath::clamp( edgeSize, 0.01f, 1.0f );
	if( fabsf( edgeSize - pSSRCoverageEdgeSize ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSRCoverageEdgeSize = edgeSize;
	pDirty = true;
}

void deoglConfiguration::SetSSRCoveragePowerEdge( float power ){
	power = decMath::max( power, 1.0f );
	if( fabsf( power - pSSRCoveragePowerEdge ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSRCoveragePowerEdge = power;
	pDirty = true;
}

void deoglConfiguration::SetSSRCoveragePowerRayLength( float power ){
	power = decMath::max( power, 1.0f );
	if( fabsf( power - pSSRCoveragePowerRayLength ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSRCoveragePowerRayLength = power;
	pDirty = true;
}

void deoglConfiguration::SetSSRMethod( int method ){
	if( method == pSSRMethod ){
		return;
	}
	pSSRMethod = method;
	pDirty = true;
}

void deoglConfiguration::SetEnvMapEnable( bool enable ){
	if( enable == pEnvMapEnable ){
		return;
	}
	pEnvMapEnable = enable;
	pDirty = true;
}

void deoglConfiguration::SetEnvMapUseEqui( bool useEqui ){
	if( useEqui == pEnvMapUseEqui ){
		return;
	}
	pEnvMapUseEqui = useEqui;
	pDirty = true;
}

void deoglConfiguration::SetEnvMapSize( int size ){
	size = decMath::max( size, 32 );
	if( size == pEnvMapSize ){
		return;
	}
	pEnvMapSize = size;
	pDirty = true;
}

void deoglConfiguration::SetEnvMapMethod( eEnvironmentMapMethods method ){
	if( method < eemmSingle ){
		method = eemmSingle;
		
	}else if( method > eemmMultipleBoxProjection ){
		method = eemmMultipleBoxProjection;
	}
	
	if( method == pEnvMapMethod ){
		return;
	}
	
	pEnvMapMethod = method;
	pDirty = true;
}



void deoglConfiguration::SetSSAOEnable( bool enable ){
	if( enable == pSSAOEnable ){
		return;
	}
	pSSAOEnable = enable;
	pDirty = true;
}

void deoglConfiguration::SetSSAOTapCount( int tapCount ){
	tapCount = decMath::max( tapCount, 1 );
	if( tapCount == pSSAOTapCount ){
		return;
	}
	pSSAOTapCount = tapCount;
	pDirty = true;
}

void deoglConfiguration::SetSSAOTurnCount( float turnCount ){
	turnCount = decMath::max( turnCount, 1.0f );
	if( fabsf( turnCount - pSSAOTurnCount ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSAOTurnCount = turnCount;
	pDirty = true;
}

void deoglConfiguration::SetSSAOSelfOcclusionAngle( float angle ){
	angle = decMath::clamp( angle, 0.0f, 90.0f );
	if( fabsf( angle - pSSAOSelfOcclusionAngle ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSAOSelfOcclusionAngle = angle;
	pDirty = true;
}

void deoglConfiguration::SetSSAORadius( float radius ){
	radius = decMath::max( radius, 0.0f );
	if( fabsf( radius - pSSAORadius ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSAORadius = radius;
	pDirty = true;
}

void deoglConfiguration::SetSSAORadiusLimit( float limit ){
	limit = decMath::clamp( limit, 0.0f, 1.0f );
	if( fabsf( limit - pSSAORadiusLimit ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSAORadiusLimit = limit;
	pDirty = true;
}

void deoglConfiguration::SetSSAOMipMapBase( float base ){
	base = decMath::max( base, 0.0f );
	if( fabsf( base - pSSAOMipMapBase ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSAOMipMapBase = base;
	pDirty = true;
}

void deoglConfiguration::SetSSAOEdgeBlurThreshold( float threshold ){
	threshold = decMath::max( threshold, 0.0f );
	if( fabsf( threshold - pSSAOEdgeBlurThreshold ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pSSAOEdgeBlurThreshold = threshold;
	pDirty = true;
}



void deoglConfiguration::SetAOSelfShadowEnable( bool enable ){
	if( enable == pAOSelfShadowEnable ){
		return;
	}
	pAOSelfShadowEnable = enable;
	pDirty = true;
}

void deoglConfiguration::SetAOSelfShadowSmoothAngle( float angle ){
	angle = decMath::clamp( angle, 0.1f, 90.0f );
	if( fabsf( angle - pAOSelfShadowSmoothAngle ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pAOSelfShadowSmoothAngle = angle;
	pDirty = true;
}



void deoglConfiguration::SetSSSSSEnable( bool enable ){
	if( enable == pSSSSSEnable ){
		return;
	}
	pSSSSSEnable = enable;
	pDirty = true;
}



void deoglConfiguration::SetFrameRateLimit( int frameRateLimit ){
	if( frameRateLimit == pFrameRateLimit ){
		return;
	}
	if( frameRateLimit < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pFrameRateLimit = frameRateLimit;
	pDirty = true;
}

void deoglConfiguration::SetAsyncRenderSkipSyncTimeRatio( float ratio ){
	ratio = decMath::clamp( ratio, 0.0f, 1.0f );
	if( fabsf( ratio - pAsyncRenderSkipSyncTimeRatio ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	pAsyncRenderSkipSyncTimeRatio = ratio;
	pDirty = true;
}



void deoglConfiguration::SetDebugContext( bool debugContext ){
	if( debugContext == pDebugContext ){
		return;
	}
	pDebugContext = debugContext;
	pDirty = true;
}

void deoglConfiguration::SetAsyncRendering( bool asyncRendering ){
	if( asyncRendering == pAsyncRendering ){
		return;
	}
	pAsyncRendering = asyncRendering;
	pDirty = true;
}

void deoglConfiguration::SetEnableRetainImageOptimization( bool enable ){
	if( enable == pEnableRetainImageOptimization ){
		return;
	}
	pEnableRetainImageOptimization = enable;
	pDirty = true;
}



void deoglConfiguration::SetBugNo2ComponentFBOTex( bool bugNo2ComponentFBOTex ){
	if( bugNo2ComponentFBOTex == pBugNo2ComponentFBOTex ){
		return;
	}
	pBugNo2ComponentFBOTex = bugNo2ComponentFBOTex;
	pDirty = true;
}



void deoglConfiguration::SetQuickDebug( int value ){
	if( value == pQuickDebug ){
		return;
	}
	pQuickDebug = value;
	pDirty = true;
}

void deoglConfiguration::SetDebugNoCulling( bool noCulling ){
	if( noCulling == pDebugNoCulling ){
		return;
	}
	pDebugNoCulling = noCulling;
	pDirty = true;
}

void deoglConfiguration::SetOcclusionTestMode( eOcclusionTestModes mode ){
	if( mode < eoctmNone ){
		mode = eoctmNone;
		
	}else if( mode > eoctmTransformFeedback ){
		mode = eoctmTransformFeedback;
	}
	
	if( mode == pOcclusionTestMode ){
		return;
	}
	
	pOcclusionTestMode = mode;
	pDirty = true;
}

void deoglConfiguration::SetDebugSnapshot( int snapshot ){
	if( snapshot == pDebugSnapshot ){
		return;
	}
	pDebugSnapshot = snapshot;
	pDirty = true;
}

void deoglConfiguration::SetDisableCubeMapLinearFiltering( bool disableCubeMapLinearFiltering ){
	if( disableCubeMapLinearFiltering == pDisableCubeMapLinearFiltering ){
		return;
	}
	pDisableCubeMapLinearFiltering = disableCubeMapLinearFiltering;
	pDirty = true;
}

void deoglConfiguration::SetMaxSPBIndexCount( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	if( count == pMaxSPBIndexCount ){
		return;
	}
	
	pMaxSPBIndexCount = count;
	pDirty = true;
}

void deoglConfiguration::SetGIQuality( eGIQuality quality ){
	if( quality == pGIQuality ){
		return;
	}
	
	pGIQuality = quality;
	pDirty = true;
}

void deoglConfiguration::SetGIUpdateSpeed( eGIUpdateSpeed updateSpeed ){
	if( updateSpeed == pGIUpdateSpeed ){
		return;
	}
	
	pGIUpdateSpeed = updateSpeed;
	pDirty = true;
}

void deoglConfiguration::SetVRRenderScale( float scale ){
	scale = floorf( ( scale / 0.05f ) + 0.5f ) * 0.05f;
	
	if( fabsf( scale - pVRRenderScale ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pVRRenderScale = scale;
	pDirty = true;
}

void deoglConfiguration::SetVRForceFrameRate( int framerate ){
	switch( framerate ){
	case 90:
	case 45:
	case 30:
	case 15:
		break;
		
	default:
		framerate = 0;
	}
	
	if( framerate == pVRForceFrameRate ){
		return;
	}
	
	pVRForceFrameRate = framerate;
	pDirty = true;
}
