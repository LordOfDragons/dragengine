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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderWorld.h"
#include "deoglRenderGeometry.h"
#include "deoglRenderOcclusion.h"
#include "debug/deoglRenderDebug.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlanDebug.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanSkyLight.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "task/deoglRenderTaskShader.h"
#include "task/shared/deoglRenderTaskSharedShader.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugInformation.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../light/deoglRLight.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../extensions/deoglExtensions.h"
#include "../occlusiontest/deoglOcclusionMap.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../sky/deoglRSkyLayer.h"
#include "../sky/deoglRSkyInstanceLayer.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"
#include "../world/deoglRWorld.h"
#include "../utils/collision/deoglCollisionFrustum.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



// #define DO_TIMING

#ifdef DO_TIMING
static decTimer dtimer;

#define DEBUG_RESET_TIMERS dtimer.Reset();
#define DEBUG_PRINT_TIMER(what) GetRenderThread().GetLogger().LogInfoFormat("Occlusion-Test %s = %iys",\
	what, ( int )( dtimer.GetElapsedTime() * 1000000.0 ) ); dtimer.Reset()
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#endif



// #define DO_TIMING2

#ifdef DO_TIMING2
#include <dragengine/common/utils/decTimer.h>
static decTimer dtimer;

#define DEBUG_RESET_TIMERS2 dtimer.Reset();
#define DEBUG_PRINT_TIMER2(what) ogl.LogInfoFormat("Occlusion-Test %s = %iys", what, \
	( int )( dtimer.GetElapsedTime() * 1000000.0 ) ); dtimer.Reset()
#else
#define DEBUG_RESET_TIMERS2
#define DEBUG_PRINT_TIMER2(what)
#endif



// Definitions
////////////////

enum eSPOccMap{
	spomMatrixMVP,
	spomMatrixMV,
	spomZTransform,
	spomMatrixP
};

enum eSPOccMapDownSample{
	spomdsLevel
};

enum eSPTest{
	sptPosTransform,
	sptInvWidth,
	sptMatrix,
	sptScaleSize,
	sptBaseLevel,
	sptClipNear,
	sptMatrix2,
	sptScaleSize2,
	sptBaseLevel2,
	sptClipNear2,
	sptFrustumNormal1,
	sptFrustumNormal2,
	sptFrustumNormal3,
	sptFrustumNormal4,
	sptFrustumTestAdd,
	sptFrustumTestMul,
	sptMatrixStereo,
	sptMatrix2Stereo
};

enum eSPTestTFB{
	spttfbMatrix,
	spttfbScaleSize,
	spttfbBaseLevel,
	spttfbClipNear,
	spttfbMatrix2,
	spttfbScaleSize2,
	spttfbBaseLevel2,
	spttfbClipNear2,
	spttfbFrustumNormal1,
	spttfbFrustumNormal2,
	spttfbFrustumNormal3,
	spttfbFrustumNormal4,
	spttfbFrustumTestAdd,
	spttfbFrustumTestMul,
	spttfbMatrixStereo,
	spttfbMatrix2Stereo
};

static const int vCubeFaces[] = {
	deoglCubeMap::efPositiveX, deoglCubeMap::efNegativeX,
	deoglCubeMap::efPositiveY, deoglCubeMap::efNegativeY,
	deoglCubeMap::efPositiveZ, deoglCubeMap::efNegativeZ
};



// Class deoglRenderOcclusion
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderOcclusion::deoglRenderOcclusion( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),

pRenderParamBlock( NULL ),
pOccMapFrustumParamBlock( NULL ),
pRenderTask( NULL ),
pAddToRenderTask( NULL )
{
	const bool indirectMatrixAccessBug = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	int drawIDOffset;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		AddOccMapDefines( commonDefines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap" );
		pShaderOccMapOrtho = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMapOrtho->EnsureRTSShader();
		pShaderOccMapOrtho->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		
		defines.SetDefines( "USE_CLIP_PLANE" );
		pShaderOccMapOrthoClipPlane = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMapOrthoClipPlane->EnsureRTSShader();
		pShaderOccMapOrthoClipPlane->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		defines.RemoveDefine( "USE_CLIP_PLANE" );
		
		defines.SetDefines( "PERSPECTIVE_TO_LINEAR" );
		pShaderOccMap = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMap->EnsureRTSShader();
		pShaderOccMap->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		
		defines.SetDefines( "USE_CLIP_PLANE" );
		pShaderOccMapClipPlane = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMapClipPlane->EnsureRTSShader();
		pShaderOccMapClipPlane->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		
		
		
		defines = commonDefines;
		if( renderThread.GetChoices().GetRenderStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			drawIDOffset = 1;
			
		}else{
			defines.SetDefines( "GS_RENDER_STEREO" );
			sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap Stereo" );
			drawIDOffset = -1;
		}
		pShaderOccMapOrthoStereo = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMapOrthoStereo->EnsureRTSShader();
		pShaderOccMapOrthoStereo->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		pShaderOccMapOrthoStereo->GetRTSShader()->SetDrawIDOffset( drawIDOffset );
		
		defines.SetDefines( "USE_CLIP_PLANE" );
		pShaderOccMapOrthoClipPlaneStereo = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMapOrthoClipPlaneStereo->EnsureRTSShader();
		pShaderOccMapOrthoClipPlaneStereo->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		pShaderOccMapOrthoClipPlaneStereo->GetRTSShader()->SetDrawIDOffset( drawIDOffset );
		defines.RemoveDefine( "USE_CLIP_PLANE" );
		
		defines.SetDefines( "PERSPECTIVE_TO_LINEAR" );
		pShaderOccMapStereo = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMapStereo->EnsureRTSShader();
		pShaderOccMapStereo->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		pShaderOccMapStereo->GetRTSShader()->SetDrawIDOffset( drawIDOffset );
		
		defines.SetDefines( "USE_CLIP_PLANE" );
		pShaderOccMapClipPlaneStereo = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMapClipPlaneStereo->EnsureRTSShader();
		pShaderOccMapClipPlaneStereo->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		pShaderOccMapClipPlaneStereo->GetRTSShader()->SetDrawIDOffset( drawIDOffset );
		
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap Down-Sample" );
		pShaderOccMapDownSample = shaderManager.GetProgramWith( sources, defines );
		
		if( renderThread.GetChoices().GetRenderStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			defines.SetDefines( "GS_RENDER_STEREO" );
			sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap Down-Sample Stereo" );
		}
		pShaderOccMapDownSampleStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion Test" );
		defines.SetDefine( "ENSURE_MIN_SIZE", true );
		pShaderOccTest = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "DUAL_OCCMAP", true );
		pShaderOccTestDual = shaderManager.GetProgramWith( sources, defines );
		
		defines = commonDefines;
		defines.SetDefine( "ENSURE_MIN_SIZE", true );
		defines.SetDefine( "FRUSTUM_TEST", true );
		pShaderOccTestSun = shaderManager.GetProgramWith( sources, defines );
		
		defines = commonDefines;
		defines.SetDefine( "ENSURE_MIN_SIZE", true );
		if( renderThread.GetChoices().GetRenderStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			defines.SetDefines( "GS_RENDER_STEREO" );
		}
		pShaderOccTestStereo = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "DUAL_OCCMAP", true );
		defines.SetDefine( "DUAL_OCCMAP_STEREO", true );
		pShaderOccTestDualStereo = shaderManager.GetProgramWith( sources, defines );
		
		defines = commonDefines;
		defines.SetDefine( "ENSURE_MIN_SIZE", true );
		defines.SetDefine( "FRUSTUM_TEST", true );
		defines.SetDefine( "FRUSTUM_TEST_STEREO", true );
		pShaderOccTestSunStereo = shaderManager.GetProgramWith( sources, defines );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion Test TFB" );
		defines.SetDefine( "ENSURE_MIN_SIZE", true );
		pShaderOccTestTFB = shaderManager.GetProgramWith( sources, defines );
		defines.SetDefine( "DUAL_OCCMAP", true );
		pShaderOccTestTFBDual = shaderManager.GetProgramWith( sources, defines );
		
		defines = commonDefines;
		defines.SetDefine( "ENSURE_MIN_SIZE", true );
		defines.SetDefine( "FRUSTUM_TEST", true );
		pShaderOccTestTFBSun = shaderManager.GetProgramWith( sources, defines );
		
#if 0
		defines = commonDefines;
		const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
		const bool useGSRenderCube = renderThread.GetExtensions().SupportsGeometryShader() && ! bugClearEntireCubeMap;
		
		if( useGSRenderCube ){
			sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap Cube" );
			defines.SetDefines( "PERSPECTIVE_TO_LINEAR" );
			
			defines.SetDefine( "GS_RENDER_CUBE", true );
			defines.SetDefine( "GS_RENDER_CUBE_CULLING", true );
			
			pShaderOccMapCube = shaderManager.GetProgramWith( sources, defines );
			pShaderOccMapCube->EnsureRTSShader();
			pShaderOccMapCube->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		}
#endif
		
		
		
		pRenderParamBlock = new deoglSPBlockUBO( renderThread );
		pRenderParamBlock->SetRowMajor( ! indirectMatrixAccessBug );
		pRenderParamBlock->SetParameterCount( 5 );
		pRenderParamBlock->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 4, 6 ); // mat4 pMatrixVP[ 6 ]
		pRenderParamBlock->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 6 ); // mat4x3 pMatrixV[ 6 ]
		pRenderParamBlock->GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 6 ); // vec4 pTransformZ[ 6 ]
		pRenderParamBlock->GetParameterAt( 3 ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2 pZToDepth
		pRenderParamBlock->GetParameterAt( 4 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 2 ); // vec4 pClipPlane[ 2 ]
		pRenderParamBlock->MapToStd140();
		pRenderParamBlock->SetBindingPoint( deoglSkinShader::eubRenderParameters );
		
		pOccMapFrustumParamBlock = new deoglSPBlockUBO( renderThread);
		pOccMapFrustumParamBlock->SetParameterCount( 1 );
		pOccMapFrustumParamBlock->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3 pMatrixModel
		pOccMapFrustumParamBlock->MapToStd140();
		pOccMapFrustumParamBlock->SetBindingPoint( deoglSkinShader::eubInstanceParameters );
		
		pRenderTask = new deoglRenderTask( renderThread );
		pAddToRenderTask = new deoglAddToRenderTask( renderThread, *pRenderTask );
		
		// create frustum planes VBO and VAO. the frustum planes mesh is actually a reduced frustum composing
		// of the frustum position and the 4 far plane points. the near plane is not required since we are
		// only interested in the left, top, right and bottom planes not the near nor the far plane. for
		// coordinates of the points are build in a way any frustum can be rendered with a properly formatted
		// matrix. the matrix has to look like this:
		// [ farX   0    0  0 ]
		// [   0  farY   0  0 ] * inverseCameraRefPointMatrix
		// [   0    0  farZ 0 ]
		// [   0    0    0  1 ]
		// this replaces the 1-values in the points with the respective far corner values while 0 sticks to
		// the frustum position. the frustum planes are pointing inside hence rendering the frustum planes
		// renders only those frustum planes on the back side of the frustum.
		// 
		// now this would construct a perfect frustum. for the culling to be more efficient though it is
		// better to replace the perfectly matching triangular planes with quadrilaterals located at the
		// frustum position and stretching in both directions. these quads are actually 4 times the size
		// of the corresponding triangle.
		/*const GLfloat pointsFrustumPlanes[ 36 ] = {
			0.0f, 0.0f, 0.0f,   -1.0f, -1.0f, 1.0f,    -1.0f,  1.0f, 1.0f, // left frustum plane triangle
			0.0f, 0.0f, 0.0f,   -1.0f,  1.0f, 1.0f,     1.0f,  1.0f, 1.0f, // top frustum plane triangle
			0.0f, 0.0f, 0.0f,    1.0f,  1.0f, 1.0f,     1.0f, -1.0f, 1.0f, // right frustum plane triangle
			0.0f, 0.0f, 0.0f,    1.0f, -1.0f, 1.0f,    -1.0f, -1.0f, 1.0f  // bottom frustum plane triangle
		};*/
		const GLfloat pointsFrustumPlanes[ 48 ] = {
			-1.0f,  1.0f, 1.0f,    1.0f,  1.0f, -1.0f,    1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, 1.0f, // left frustum plane quad
			1.0f,  1.0f, 1.0f,    1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, 1.0f, // top frustum plane quad
			1.0f, -1.0f, 1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,    1.0f,  1.0f, 1.0f, // right frustum plane quad
			-1.0f, -1.0f, 1.0f,   -1.0f,  1.0f, -1.0f,    1.0f,  1.0f, -1.0f,    1.0f, -1.0f, 1.0f, // bottom frustum plane quad
		};
		
		OGL_CHECK( renderThread, pglGenVertexArrays( 1, &pVAOFrustumPlanes ) );
		if( ! pVAOFrustumPlanes ){
			DETHROW( deeOutOfMemory );
		}
		OGL_CHECK( renderThread, pglBindVertexArray( pVAOFrustumPlanes ) );
		
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOFrustumPlanes ) );
		if( ! pVBOFrustumPlanes ){
			DETHROW( deeOutOfMemory );
		}
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOFrustumPlanes ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( pointsFrustumPlanes ), ( const GLvoid * )&pointsFrustumPlanes, GL_STATIC_DRAW ) );
		
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) );
		OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( const GLvoid * )0 ) );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderOcclusion::~deoglRenderOcclusion(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderOcclusion::AddOccMapDefines( deoglShaderDefines &defines ){
	AddBasicDefines( defines );
	
	const deoglRenderThread &renderThread = GetRenderThread();
	const deoglRTBufferObject &bo = renderThread.GetBufferObject();
	decString value;
	
	defines.SetDefine( "SHARED_SPB", true );
	
	if( renderThread.GetChoices().GetSharedSPBUseSSBO() ){
		defines.SetDefine( "SHARED_SPB_USE_SSBO", true );
		
		if( bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() >= 16 ){
			value.SetValue( bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() / 16 );
			defines.SetDefine( "SHARED_SPB_PADDING", value );
		}
		
	}else{
		// NOTE UBO requires array size to be constant, SSBO does not
		if( bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() > 0 ){
			value.SetValue( bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() );
			defines.SetDefine( "SHARED_SPB_ARRAY_SIZE", value );
		}
		
		if( bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() >= 16 ){
			value.SetValue( bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() / 16 );
			defines.SetDefine( "SHARED_SPB_PADDING", value );
		}
	}
	
	if( bo.GetInstanceArraySizeUBO() > 0 ){
		value.SetValue( bo.GetInstanceArraySizeUBO() );
		defines.SetDefine( "SPB_INSTANCE_ARRAY_SIZE", value );
	}
}

/*
Implementation Note

The current implementation is based on writing linear depth in the fragment shader.
This requires disabling all inverse depth parts. Linear depth has been chosen due
to resolution concerns. Using inverse depth this could be avoided which would
speed up occlusion testing since depth is no more written in the fragment shader.
To realize this though a couple of things have to be change and made dependent on
the use inverse depth mode.

The occlusion texture has to use depthFloat instead of depth if enabled

The convertion of z position to depth value has to be removed and projective depth
written out by the fixed function pipeline.

In the test shader max() calls are used for filtering down. For inverse depth this
has to be changed to min() calls.

The same is true for the downsampling. It needs to use min() instead of max() too
if inverse depth is used.

Depth compare direction has to change from <= to >= if inverse depth is used.
*/

void deoglRenderOcclusion::RenderTestsCamera( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	
	if( config.GetOcclusionTestMode() == deoglConfiguration::eoctmNone ){
		return;
	}
	
	deoglRenderPlanDebug * const renderPlanDebug = plan.GetDebug();
	
	decTimer timer;
	if( renderPlanDebug ){
		timer.Reset();
	}
	DEBUG_RESET_TIMERS;
	DEBUG_RESET_TIMERS2;
	
	// calculate the z transformation required to do linear-z occlusion map
	// z' = (z-zn)/(zf-zn)
	// z' = z/(zf-zn)-zn/(zf-zn)
	// z' = z * [1/(zf-zn)] + [-zn/(zf-zn)]
	const float znear = plan.GetCameraImageDistance();
	const float zfar = plan.GetCameraViewDistance();
	
	const float zscale = 1.0f / ( zfar - znear );
	const float zoffset = -( znear * zscale );
	
	// calculate a small offset to avoid problems of similar depth
	const float occmapResolution = 1.0f / ( float )( ( 1 << 24 ) - 1 ); // 24-bit depth texture
	const float occmapBias = 1.0f; // bias by 1 or 2 depth values. 1 should be enough
	
	// update render parameter block
	const decDMatrix &matrixCamera = plan.GetRefPosCameraMatrix();
	
	decDMatrix matrixCameraStereo( matrixCamera );
	if( plan.GetRenderStereo() ){
		matrixCameraStereo *= plan.GetCameraStereoMatrix();
	}
	
	// linear depth: use non-infinite projection matrix 
	const decDMatrix &matrixProjection = plan.GetFrustumMatrix();
	
	pRenderParamBlock->MapBuffer();
	try{
		// 0: pMatrixVP[ 0 ]
		// 1: pMatrixV[ 0 ]
		// 2: pTransformZ[ 0 ]
		pRenderParamBlock->SetParameterDataArrayMat4x4( 0, 0, matrixCamera * matrixProjection );
		pRenderParamBlock->SetParameterDataArrayMat4x3( 1, 0, matrixCamera );
		pRenderParamBlock->SetParameterDataArrayVec4( 2, 0, ( float )matrixCamera.a31 * zscale,
			( float )matrixCamera.a32 * zscale, ( float )matrixCamera.a33 * zscale,
			( float )matrixCamera.a34 * zscale + occmapResolution * occmapBias );
		pRenderParamBlock->SetParameterDataVec2( 3, zscale, zoffset + occmapResolution * occmapBias );
		
		if( mask && mask->GetUseClipPlane() ){
			pRenderParamBlock->SetParameterDataArrayVec4( 4, 0, mask->GetClipNormal(), mask->GetClipDistance() );
			
		}else{
			pRenderParamBlock->SetParameterDataArrayVec4( 4, 0, 0.0f, 0.0f, 0.0f, 0.0f ); // pClipPlane
		}
		
		if( plan.GetRenderStereo() ){
			pRenderParamBlock->SetParameterDataArrayMat4x4( 0, 1, matrixCameraStereo * matrixProjection );
			pRenderParamBlock->SetParameterDataArrayMat4x3( 1, 1, matrixCameraStereo );
			pRenderParamBlock->SetParameterDataArrayVec4( 2, 1, ( float )matrixCameraStereo.a31 * zscale,
				( float )matrixCameraStereo.a32 * zscale, ( float )matrixCameraStereo.a33 * zscale,
				( float )matrixCameraStereo.a34 * zscale + occmapResolution * occmapBias );
			
			if( mask && mask->GetUseClipPlane() ){
				pRenderParamBlock->SetParameterDataArrayVec4( 4, 1, mask->GetClipNormalStereo(), mask->GetClipDistanceStereo() );
				
			}else{
				pRenderParamBlock->SetParameterDataArrayVec4( 4, 1, 0.0f, 0.0f, 0.0f, 0.0f ); // pClipPlane
			}
		}
		
	}catch( const deException & ){
		pRenderParamBlock->UnmapBuffer();
		throw;
	}
	pRenderParamBlock->UnmapBuffer();
	DEBUG_PRINT_TIMER( "Update Param Block" );
	
	// render occlusion map
	if( renderPlanDebug ){
		renderPlanDebug->IncrementOccMapObjects( plan.GetCollideList().GetComponentCount() );
		RenderOcclusionMap( plan, mask );
		renderPlanDebug->IncrementOccMapTriangles( pRenderTask->GetTotalPointCount() / 3 );
		renderPlanDebug->SetOccMapRenderTime( timer.GetElapsedTime() );
		
	}else{
		RenderOcclusionMap( plan, mask );
	}
	
	// render visibility tests. we use linear depth. the shaders do not apply perspective
	// division to the z coordinate. we thus have to recreate the same transformation applied
	// during rendering of the occlusion map:
	//   vDepth = dot( pTransformZ[ 0 ], position )
	// 
	// we have to keep the x, y and w transformation the same so the perspective division
	// works as intended. the z row in the matrix thus has to be modified to reflect the
	// transformation mentioned above. written out the transformation looks like this:
	//   depth = (m31 * px + m32 * py + m33 * pz + m34) * scale + offset
	//   depth = m31 * px * scale + m32 * py * scale + m33 * pz * scale + m34 * scale + offset
	//   depth = px * (m31 * scale) + py * (m32 * scale) + pz * (m33 * scale) + (m34 * scale + offset)
	// 
	// the modified z row values thus have to look like this:
	//   m31' = m31 * scale
	//   m32' = m32 * scale
	//   m33' = m33 * scale
	//   m34' = m34 * scale + offset
	// 
	// note about the rotation part. the test input data is calculated relative to the camera
	// position. for this reason the transformation part of the camera matrix (this is the
	// camera position) has to be dropped to not apply it twice. since the last row is (0,0,0,1)
	// using the rotation part of the matrix is working out of the box.
	// 
	// for this reason the last line looks different than above because m34 is 0:
	//   m34' = offset
	
	decDMatrix testMatrix( matrixCamera.GetRotationMatrix() * matrixProjection );
	testMatrix.a31 = matrixCamera.a31 * zscale;
	testMatrix.a32 = matrixCamera.a32 * zscale;
	testMatrix.a33 = matrixCamera.a33 * zscale;
	testMatrix.a34 = zoffset; // no bias while sampling
	
	decDMatrix testMatrixStereo( matrixCameraStereo.GetRotationMatrix() * matrixProjection );
	testMatrixStereo.a31 = matrixCameraStereo.a31 * zscale;
	testMatrixStereo.a32 = matrixCameraStereo.a32 * zscale;
	testMatrixStereo.a33 = matrixCameraStereo.a33 * zscale;
	
	plan.SetOcclusionTestMatrix( testMatrix );
	plan.SetOcclusionTestMatrixStereo( testMatrixStereo );
	
	RenderOcclusionTests( plan, *plan.GetOcclusionTest(), *plan.GetOcclusionMap(),
		plan.GetOcclusionMapBaseLevel(), -1.0f, testMatrix, testMatrixStereo );
	if( renderPlanDebug ){
		renderPlanDebug->SetOccTestTime( timer.GetElapsedTime() );
	}
}

#if 0
void deoglRenderOcclusion::4( deoglRenderPlan &plan, deoglRSkyLayer &skyLayer,
deoglCollideList &collideList, const decVector &minExtend, const decVector &maxExtend ){
	const deoglConfiguration &config = *ogl.GetConfiguration();
	
	if( config.GetOcclusionTestMode() == deoglConfiguration::eoctmNone ){
		return;
	}
	
	deoglOcclusionTest &occtest = ogl.GetOcclusionTest();
	deoglOcclusionMap * const occmap2 = occtest.GetOcclisionMap1();
	deoglOcclusionMap &occmap = *occtest.GetOcclisionMap2();
	const int baselevel2 = plan.GetOcclusionMapBaseLevel();
	const int baselevel = 0;
	
	float occmapResolution, occmapBias;
	
	deoglWorld &world = *plan.GetWorld();
	const decDVector &referencePosition = world.GetReferencePosition();
	const decDVector &camPos = plan.GetCameraPosition();
	decMatrix matrixCamera, matrixCamera2;
	decVector position, scale;
	float orgMatrixCameraZ;
	DEBUG_RESET_TIMERS;
	
	// calculate a small offset to avoid problems of similar depth
	occmapResolution = 1.0f / ( float )( ( 1 << 24 ) - 1 ); // 24-bit depth texture
	occmapBias = 1.0f; // bias by 1 or 2 depth values. 1 should be enough
	
	// calculate the camera matrix fitting around all splits
	const decMatrix matLig = decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * skyLayer.GetMatrix().ToMatrix();
	
	position.x = ( minExtend.x + maxExtend.x ) * 0.5f;
	position.y = ( minExtend.y + maxExtend.y ) * 0.5f;
	position.z = ( minExtend.z + maxExtend.z ) * 0.5f;
	position = ( camPos - referencePosition ).ToVector() + ( matLig * position );
	
	scale.x = 1.0f / ( maxExtend.x - minExtend.x );
	scale.y = 1.0f / ( maxExtend.y - minExtend.y );
	scale.z = 1.0f / ( maxExtend.z - minExtend.z );
	
	matrixCamera = decMatrix::CreateCamera( position, matLig.TransformView(), matLig.TransformUp() ) * decMatrix::CreateScale( scale * 2.0f );
	orgMatrixCameraZ = matrixCamera.a34;
	
	// update render parameter block
	matrixCamera.a34 += occmapResolution * occmapBias;
	DEBUG_PRINT_TIMER( "Entering Render Occlusion Map (Sky Layer)" );
	pRenderParamBlock->SetParameterDataMat4x4( 0, matrixCamera ); // pMatrixVP
	DEBUG_PRINT_TIMER( "Set Param Block Data" );
	pRenderParamBlock->Update();
	DEBUG_PRINT_TIMER( "Update Param Block" );
	
	// render occlusion map
	matrixCamera.a34 = orgMatrixCameraZ;
	matrixCamera2 = matrixCamera.Invert() * decMatrix::CreateTranslation( ( referencePosition - camPos ).ToVector() ) * plan.GetOcclusionTestMatrix();
	/*
	const float zf = plan.GetCameraViewDistance();
	const float xf = tanf( plan.GetCameraFov() * 0.5f ) * zf;
	const float yf = tanf( plan.GetCameraFov() * plan.GetCameraFovRatio() * 0.5f ) *zf;
	decMatrix renderFrustumPlanesMatrix = decMatrix::CreateScale( xf, yf, zf ) * plan.GetRefPosCameraMatrix().Invert();
	*/
	RenderOcclusionMap( occmap, baselevel, collideList, false, NULL ); //&renderFrustumPlanesMatrix );
	
	// render visibility tests
	/*
	if( ogl.GetConfiguration()->GetDebugSnapshot() == 120 ){
		printf( "debug snapshot sun light" );
		printf( "extends (%f,%f,%f)-(%f,%f,%f)\n", minExtend.x, minExtend.y, minExtend.z, maxExtend.x, maxExtend.y, maxExtend.z );
		printf( "matrixCamera:\n" );
		printf( "  [%+f , %+f , %+f , %+f]\n", matrixCamera.a11, matrixCamera.a12, matrixCamera.a13, matrixCamera.a14 );
		printf( "  [%+f , %+f , %+f , %+f]\n", matrixCamera.a21, matrixCamera.a22, matrixCamera.a23, matrixCamera.a24 );
		printf( "  [%+f , %+f , %+f , %+f]\n", matrixCamera.a31, matrixCamera.a32, matrixCamera.a33, matrixCamera.a34 );
		printf( "  [%+f , %+f , %+f , %+f]\n", matrixCamera.a41, matrixCamera.a42, matrixCamera.a43, matrixCamera.a44 );
		printf( "refpos (%f,%f,%f)\n", referencePosition.x, referencePosition.y, referencePosition.z );
		
		deoglOcclusionTest &occtest = ogl.GetOcclusionTest();
		const int inputDataCount = occtest.GetInputDataCount();
		struct sInputData{
			oglVector3 minExtend;
			oglVector3 maxExtend;
		} *inputData = ( sInputData* )occtest.GetInputData();
		int i;
		
		for( i=0; i<inputDataCount; i++ ){
			const oglVector3 &idmine = inputData[ i ].minExtend;
			const oglVector3 &idmaxe = inputData[ i ].maxExtend;
			if( idmine.x > -16.1f && idmine.y > 2.9f && idmine.z > 2.9f && idmaxe.x < -15.9f && idmaxe.y < 6.1f && idmaxe.z <= 13.1f ){
				printf( "object at input %i: extends (%f,%f,%f)-(%f,%f,%f)\n", i, idmine.x, idmine.y, idmine.z, idmaxe.x, idmaxe.y, idmaxe.z );
			}
		}
	}
	*/
	RenderOcclusionTestsSun( occmap, baselevel, -100.0f, matrixCamera, occmap2, baselevel2, -1.0f, matrixCamera2, &plan );
	collideList.RemoveVisibleComponents( false );
	DEBUG_PRINT_TIMER( "End tests and remove invisible" );
	/*
	if( ogl.GetConfiguration()->GetDebugSnapshot() == 120 ){
		ogl.GetConfiguration()->SetDebugSnapshot( 0 );
	}
	*/
}
#endif

void deoglRenderOcclusion::RenderTestsSkyLayer( deoglRenderPlan &plan,
deoglRenderPlanSkyLight &planSkyLight ){
	DEBUG_RESET_TIMERS;
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	
	if( config.GetOcclusionTestMode() == deoglConfiguration::eoctmNone ){
		return;
	}
	
	// calculate the camera matrix fitting around all splits. the camera matrix transform
	// the points into the range [-1..1] for all axes
	const decDVector &referencePosition = plan.GetWorld()->GetReferencePosition();
	const decVector &minExtend = planSkyLight.GetFrustumBoxMinExtend();
	const decVector &maxExtend = planSkyLight.GetFrustumBoxMaxExtend();
	const decDVector &camPos = plan.GetCameraPosition();
	
	const decDMatrix matLig( decDMatrix::CreateRotation( 0.0, PI, 0.0 )
		* decDMatrix( planSkyLight.GetLayer()->GetMatrix() ) );
	
	const decDVector boxCenter( ( minExtend + maxExtend ) * 0.5f );
	const decDVector boxSize( maxExtend - minExtend );
	
	const decDVector position( camPos - referencePosition + matLig * boxCenter );
	const decDVector scale( 2.0 / boxSize.x, 2.0 / boxSize.y, 2.0 / boxSize.z );
	
	// test input valus use world reference position as reference position instead of
	// camera position as the camera tests do
	
	// the test shader expects z coordinate to be transformed into the range [0..1]
	// and x/y/z in the range [-w..w]. this requires adding a scale of z=0.5 and a
	// translation of z=0.5
	const decDMatrix matrixCamera(
		decDMatrix::CreateCamera( position, matLig.TransformView(), matLig.TransformUp() )
		* decDMatrix::CreateScale( scale ) * decDMatrix::CreateScale( 1.0, 1.0, 0.5 )
		* decDMatrix::CreateTranslation( 0.0, 0.0, 0.5 ) );
	
	// the result of the first occlusion test has to be transformed back into occlusion
	// map space. this is done by first reverting the matrixCamera which transforms back
	// into world space relative to world reference position. then the position has to
	// be transformed into world space relative to camera position. now the position is
	// in the same space as the camera tests used. then the occlusion matrix is applied
	// the same way as camera tests did
	const decMatrix matrixLightToCamera( matrixCamera.Invert()
		* decDMatrix::CreateTranslation( referencePosition - camPos ) );
	
	const decMatrix matrixCamera2( matrixLightToCamera * plan.GetOcclusionTestMatrix() );
	const decMatrix matrixCamera2Stereo( matrixLightToCamera * plan.GetOcclusionTestMatrixStereo() );
	DEBUG_PRINT_TIMER( "Prepare" );
	
	RenderOcclusionTestsSun( plan, *planSkyLight.GetOcclusionTest(), *plan.GetOcclusionMap(),
		plan.GetOcclusionMapBaseLevel(), -100.0f, matrixCamera, -1.0f, matrixCamera2, matrixCamera2Stereo );
	DEBUG_PRINT_TIMER( "Render" );
}


deoglRenderTaskSharedShader *deoglRenderOcclusion::GetRenderOcclusionMapRTS(
const deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool perspective ) const{
	if( perspective ){
		if( mask && mask->GetUseClipPlane() ){
			return ( plan.GetRenderStereo() ? pShaderOccMapClipPlaneStereo : pShaderOccMapClipPlane )->GetRTSShader();
			
		}else{
			return ( plan.GetRenderStereo() ? pShaderOccMapStereo : pShaderOccMap )->GetRTSShader();
		}
		
	}else{
		if( mask && mask->GetUseClipPlane() ){
			return ( plan.GetRenderStereo() ? pShaderOccMapOrthoClipPlaneStereo : pShaderOccMapOrthoClipPlane )->GetRTSShader();
			
		}else{
			return ( plan.GetRenderStereo() ? pShaderOccMapOrthoStereo : pShaderOccMapOrtho )->GetRTSShader();
		}
	}
}

void deoglRenderOcclusion::RenderOcclusionMap( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
	pRenderTask->Clear();
	
	pAddToRenderTask->Reset();
	pAddToRenderTask->SetSolid( true );
	pAddToRenderTask->SetNoRendered( plan.GetNoRenderedOccMesh() );
	
	pAddToRenderTask->SetEnforceShader( GetRenderOcclusionMapRTS( plan, mask, true ) );
	
	pAddToRenderTask->AddOcclusionMeshes( plan.GetComponentsOccMap() );
	DEBUG_PRINT_TIMER( "RenderOcclusionMap Build RT" );
	
	RenderOcclusionMap( plan, *pRenderTask );
}

void deoglRenderOcclusion::RenderOcclusionMap( deoglRenderPlan &plan, deoglRenderTask &renderTask ){
	deoglOcclusionMap &occmap = *plan.GetOcclusionMap();
	const int baselevel = plan.GetOcclusionMapBaseLevel();
	const decMatrix *renderFrustumPlanesMatrix = NULL;
	const bool perspective = true;

	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	const int levelCount = occmap.GetLevelCount();
	deoglShaderCompiled *shader;
	int i, width, height;
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_LEQUAL ) ); // lequal, occlusion uses linear depth
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( false );
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT,  GL_NEGATIVE_ONE_TO_ONE ); // reset, occlusion uses linear depth
	}
	
	// prepare fbo
	renderThread.GetFramebuffer().Activate( occmap.GetFBOAt( 0 ) );
	
	width = occmap.GetWidth();
	height = occmap.GetHeight();
	for( i=1; i<baselevel; i++ ){
		width >>= 1;
		height >>= 1;
	}
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	// clear the occlusion map
	const GLfloat clearDepth = ( GLfloat )1.0f; // occlusion uses linear depth
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	DEBUG_PRINT_TIMER( "RenderOcclusionMap Clear" );
	
	// render the frustum back facing planes if required. this is an optimization for the sun shadow case.
	// there we use the sun occlusion map to determine the shadow shaft size for elements to test for
	// visibility against the camera occlusion map. if the world does not contain full closed occlusion
	// meshes holes exist causing far away boundary values to stretch the shadow shafts longer than needed.
	// this causes such elements to be included in shadow casting although they are not really affecting
	// the scene slowing down rendering. using a little trick there is a partial cure for this sub optimal
	// situation. the shadow shafts are only interest up to the camera frustum. this ensures shadow shafts
	// do not stick too much beyond the current camera view since the frustum planes are located at the
	// screen borders. if these frustum planes are thus rendered also into the occlusion map the chance
	// of shortening shadow shafts in these situations increases. the rendering is done using a generic
	// frustum planes VBO build in a way any frustum can be created just by using a model matrix build
	// using a scaling matrix filled with the far frustum point coordinates. only the back faces of the
	// frustum are rendered.
	if( renderFrustumPlanesMatrix ){// && false ){
		deoglShaderProgram *program;
		if( perspective ){
			program = plan.GetRenderStereo() ? pShaderOccMapStereo : pShaderOccMap;
			
		}else{
			program = plan.GetRenderStereo() ? pShaderOccMapOrthoStereo : pShaderOccMapOrtho;
		}
		renderThread.GetShader().ActivateShader( program );
		shader = program->GetCompiled();
		
		pOccMapFrustumParamBlock->MapBuffer();
		try{
			// 0: pMatrixModel
			pOccMapFrustumParamBlock->SetParameterDataMat4x3( 0, *renderFrustumPlanesMatrix );
			
		}catch( const deException & ){
			pOccMapFrustumParamBlock->UnmapBuffer();
			throw;
		}
		pOccMapFrustumParamBlock->UnmapBuffer();
		
		pRenderParamBlock->Activate();
		pOccMapFrustumParamBlock->Activate();
		pglBindVertexArray( pVAOFrustumPlanes );
		
		OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
		
		if( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() ){
			const GLint first[] = { 0, 0, 4, 4, 8, 8, 12, 12 };
			const GLsizei count[] = { 4, 4, 4, 4, 4, 4, 4, 4 };
			OGL_CHECK( renderThread, pglMultiDrawArrays( GL_TRIANGLE_FAN, first, count, 2 ) ); // left
			OGL_CHECK( renderThread, pglMultiDrawArrays( GL_TRIANGLE_FAN, first + 2, count + 2, 2 ) ); // top
			OGL_CHECK( renderThread, pglMultiDrawArrays( GL_TRIANGLE_FAN, first + 4, count + 4, 2 ) ); // right
			OGL_CHECK( renderThread, pglMultiDrawArrays( GL_TRIANGLE_FAN, first + 6, count + 6, 2 ) ); // bottom
			
		}else{
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) ); // left
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 4, 4 ) ); // top
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 8, 4 ) ); // right
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 12, 4 ) ); // bottom
			//OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, 36 ) );
		}
		
		pglBindVertexArray( 0 );
		DEBUG_PRINT_TIMER( "RenderOcclusionMap Render Frustum Planes" );
	}
	
	// render occlusion map
	renderTask.SetRenderParamBlock( pRenderParamBlock );
	renderTask.SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
	renderTask.PrepareForRender();
	rengeom.RenderTask( renderTask );
	DEBUG_PRINT_TIMER( "RenderOcclusionMap Render" );
	
	// create mipmap level (z-pyramid)
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderOccMapDownSampleStereo : pShaderOccMapDownSample;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) ); // ati bug, disabling depth testing gets you in hell's kitchen
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	tsmgr.EnableArrayTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
	
	for( i=baselevel+1; i<levelCount; i++ ){
		width >>= 1;
		height >>= 1;
		
		renderThread.GetFramebuffer().Activate( occmap.GetFBOAt( i ) );
		OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
		
		shader->SetParameterInt( spomdsLevel, i - 1 );
		
		RenderFullScreenQuad( plan );
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	DEBUG_PRINT_TIMER( "RenderOcclusionMap Downsample" );
}



void deoglRenderOcclusion::RenderOcclusionTests( deoglRenderPlan &plan,
deoglOcclusionTest &occlusionTest, deoglOcclusionMap &occlusionMap, int baselevel,
float clipNear, const decMatrix &matrixCamera, const decMatrix &matrixCameraStereo ){
	deoglRenderThread &renderThread = GetRenderThread();
	
	// render tests if required
	if( occlusionTest.GetInputDataCount() == 0 ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const decPoint &resultSize = occlusionTest.GetResultSize();
	deoglShaderCompiled *shader;
	
	if( occlusionTest.GetVBOResult() ){ // valid deoglConfiguration::eoctmTransformFeedback
		renderThread.GetShader().ActivateShader( pShaderOccTestTFB );
		shader = pShaderOccTestTFB->GetCompiled();
		
		shader->SetParameterMatrix4x4( spttfbMatrix, matrixCamera );
		shader->SetParameterFloat( spttfbScaleSize, ( float )occlusionMap.GetWidth(), ( float )occlusionMap.GetHeight() );
		shader->SetParameterFloat( spttfbBaseLevel, ( float )baselevel );
		shader->SetParameterFloat( spttfbClipNear, clipNear );
		if( plan.GetRenderStereo() ){
			shader->SetParameterMatrix4x4( spttfbMatrixStereo, matrixCameraStereo );
		}
		
		tsmgr.EnableArrayTexture( 0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap() );
		
		if( renderThread.GetCapabilities().GetRasterizerDiscard().Broken() ){
			OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
			OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		}
		
		OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, occlusionTest.GetVBOResult() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( occlusionTest.GetVAO() ) );
		OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occlusionTest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglEndTransformFeedback() );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
		
	}else{ // invalid deoglConfiguration::eoctmTransformFeedback or deoglConfiguration::eoctmVBOToTexture
		renderThread.GetFramebuffer().Activate( occlusionTest.GetFBOResult() );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, resultSize.x, resultSize.y ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		
		deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderOccTestStereo : pShaderOccTest;
		renderThread.GetShader().ActivateShader( program );
		shader = program->GetCompiled();
		
		// x:(0..1)=>(-1..1) , y:(0..height)=>(-1..1)
		shader->SetParameterFloat( sptPosTransform, 2.0f, 2.0f / ( float )( resultSize.y - 1 ), -1.0f, -1.0f );
		shader->SetParameterFloat( sptInvWidth, 1.0f / ( float )resultSize.x );
		shader->SetParameterDMatrix4x4( sptMatrix, matrixCamera );
		shader->SetParameterFloat( sptScaleSize, ( float )occlusionMap.GetWidth(), ( float )occlusionMap.GetHeight() );
		shader->SetParameterFloat( sptBaseLevel, ( float )baselevel );
		if( plan.GetRenderStereo() ){
			shader->SetParameterDMatrix4x4( sptMatrixStereo, matrixCameraStereo );
		}
		
		tsmgr.EnableArrayTexture( 0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap() );
		
		OGL_CHECK( renderThread, pglBindVertexArray( occlusionTest.GetVAO() ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occlusionTest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	}
	DEBUG_PRINT_TIMER( "RenderOcclusionTests Render" );
}

void deoglRenderOcclusion::RenderOcclusionTestsSun( deoglRenderPlan &plan,
deoglOcclusionTest &occlusionTest, deoglOcclusionMap &occlusionMap, int baselevel, float clipNear,
const decMatrix &matrixCamera, float clipNear2, const decMatrix &matrixCamera2,
const decMatrix &matrixCamera2Stereo ){
	deoglRenderThread &renderThread = GetRenderThread();
	
	if( occlusionTest.GetInputDataCount() == 0 ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const decPoint &resultSize = occlusionTest.GetResultSize();
	deoglShaderCompiled *shader;
	
	decVector frustumPlaneNormal[ 4 ]; // left, top, right, bottom
	float frustumFactorAdd[ 4 ]; // left, top, right, bottom
	float frustumFactorMul[ 4 ]; // left, top, right, bottom
	
	// calculate frustum from plan if required.
	// TODO it is also possible to cull the other end of the shadow shaft. this could potentially help in cases
	//      where the sun is behind the player and the object is between the frustum and the sun. this culling
	//      would have to be done with the other planes (those facing away from the sun).
	const float fov = plan.GetCameraFov();
	const float fovRatio = plan.GetCameraFovRatio();
	const float zfar = plan.GetCameraViewDistance();
	const float xfar = tanf( fov * 0.5f ) * zfar;
	const float yfar = tanf( fov * 0.5f * fovRatio ) * zfar;
	
	const decMatrix frustumMatrix = plan.GetRefPosCameraMatrix().Invert() * matrixCamera;
	decVector fpts[ 5 ];
	float denom;
	int i;
	
	fpts[ 0 ] = frustumMatrix.GetPosition();
	frustumMatrix.Transform( fpts[ 1 ], -xfar,  yfar, zfar ); // top-left
	frustumMatrix.Transform( fpts[ 2 ],  xfar,  yfar, zfar ); // top-right
	frustumMatrix.Transform( fpts[ 3 ],  xfar, -yfar, zfar ); // bottom-right
	frustumMatrix.Transform( fpts[ 4 ], -xfar, -yfar, zfar ); // bottom-left
	
	for( i=0; i<4; i++ ){
		frustumPlaneNormal[ i ] = ( ( fpts[ 1 + i ] - fpts[ 0 ] ) % ( fpts[ 1 + ( ( 3 + i ) % 4 ) ] - fpts[ 0 ] ) ).Normalized();
		
		if( plan.GetFlipCulling() ){
			// mirroring causes normals to point outside instead of inside
			frustumPlaneNormal[ i ] = -frustumPlaneNormal[ i ];
		}
		
		if( frustumPlaneNormal[ i ].z < 0.0f ){
			denom = 1.0f / frustumPlaneNormal[ i ].z; // 1/(sunDir*planeNormal) = 1/((0,0,01)*planeNormal) = 1/planeNormal.z
			frustumFactorAdd[ i ] = ( fpts[ 0 ] * frustumPlaneNormal[ i ] ) * denom;
			frustumFactorMul[ i ] = -denom;
			
		}else{
			frustumPlaneNormal[ i ].SetZero();
			frustumFactorMul[ i ] = 0.0f;
			frustumFactorAdd[ i ] = 1.0f;
		}
	}
	
	DEBUG_PRINT_TIMER( "RenderOcclusionTestsSun Entering" );
	
	if( occlusionTest.GetVBOResult() ){ // valid deoglConfiguration::eoctmTransformFeedback
		renderThread.GetShader().ActivateShader( pShaderOccTestTFBSun );
		shader = pShaderOccTestTFBSun->GetCompiled();
		DEBUG_PRINT_TIMER( "Activate FBO" );
		
		shader->SetParameterMatrix4x4( spttfbMatrix, matrixCamera );
		shader->SetParameterFloat( spttfbScaleSize, ( float )occlusionMap.GetWidth(), ( float )occlusionMap.GetHeight() );
		shader->SetParameterFloat( spttfbBaseLevel, ( float )baselevel );
		shader->SetParameterFloat( spttfbClipNear, clipNear );
		shader->SetParameterMatrix4x4( spttfbMatrix2, matrixCamera2 );
		shader->SetParameterFloat( spttfbClipNear2, clipNear2 );
		shader->SetParameterVector3( spttfbFrustumNormal1, frustumPlaneNormal[ 0 ] );
		shader->SetParameterVector3( spttfbFrustumNormal2, frustumPlaneNormal[ 1 ] );
		shader->SetParameterVector3( spttfbFrustumNormal3, frustumPlaneNormal[ 2 ] );
		shader->SetParameterVector3( spttfbFrustumNormal4, frustumPlaneNormal[ 3 ] );
		shader->SetParameterFloat( spttfbFrustumTestAdd, frustumFactorAdd[ 0 ],
			frustumFactorAdd[ 1 ], frustumFactorAdd[ 2 ], frustumFactorAdd[ 3 ] );
		shader->SetParameterFloat( spttfbFrustumTestMul, frustumFactorMul[ 0 ],
			frustumFactorMul[ 1 ], frustumFactorMul[ 2 ], frustumFactorMul[ 3 ] );
		if( plan.GetRenderStereo() ){
			shader->SetParameterMatrix4x4( spttfbMatrix2Stereo, matrixCamera2Stereo );
		}
		DEBUG_PRINT_TIMER( "Set Uniforms" );
		
		tsmgr.EnableArrayTexture( 0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap() );
		DEBUG_PRINT_TIMER( "Set Texture" );
		
		if( renderThread.GetCapabilities().GetRasterizerDiscard().Broken() ){
			OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
			OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		}
		
		OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, occlusionTest.GetVBOResult() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( occlusionTest.GetVAO() ) );
		OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occlusionTest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglEndTransformFeedback() );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
		DEBUG_PRINT_TIMER( "Draw with Feedback" );
		
	}else{ // invalid deoglConfiguration::eoctmTransformFeedback or deoglConfiguration::eoctmVBOToTexture
		renderThread.GetFramebuffer().Activate( occlusionTest.GetFBOResult() );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, resultSize.x, resultSize.y ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		
		deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderOccTestSunStereo : pShaderOccTestSun;
		renderThread.GetShader().ActivateShader( program );
		shader = program->GetCompiled();
		
		// x:(0..1)=>(-1..1) , y:(0..height)=>(-1..1)
		shader->SetParameterFloat( sptPosTransform, 2.0f, 2.0f / ( float )( resultSize.y - 1 ), -1.0f, -1.0f );
		shader->SetParameterFloat( sptInvWidth, 1.0f / ( float )resultSize.x );
		shader->SetParameterDMatrix4x4( sptMatrix, matrixCamera );
		shader->SetParameterFloat( sptScaleSize, ( float )occlusionMap.GetWidth(), ( float )occlusionMap.GetHeight() );
		shader->SetParameterFloat( sptBaseLevel, ( float )baselevel );
		/* frustum test not supported without transform feedback
		shader->SetParameterDMatrix4x4( sptMatrix2, matrixCamera2 );
		shader->SetParameterVector3( sptFrustumNormal1, frustumPlaneNormal[ 0 ] );
		shader->SetParameterVector3( sptFrustumNormal2, frustumPlaneNormal[ 1 ] );
		shader->SetParameterVector3( sptFrustumNormal3, frustumPlaneNormal[ 2 ] );
		shader->SetParameterVector3( sptFrustumNormal4, frustumPlaneNormal[ 3 ] );
		shader->SetParameterFloat( sptFrustumTestAdd, frustumFactorAdd[ 0 ], frustumFactorAdd[ 1 ], frustumFactorAdd[ 2 ], frustumFactorAdd[ 3 ] );
		shader->SetParameterFloat( sptFrustumTestMul, frustumFactorMul[ 0 ], frustumFactorMul[ 1 ], frustumFactorMul[ 2 ], frustumFactorMul[ 3 ] );
		*/
		
		tsmgr.EnableArrayTexture( 0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap() );
		
		OGL_CHECK( renderThread, pglBindVertexArray( occlusionTest.GetVAO() ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occlusionTest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	}
}

void deoglRenderOcclusion::RenderOcclusionTestsDual( deoglRenderPlan &plan, deoglOcclusionTest &occlusionTest,
deoglOcclusionMap &occlusionMap, int baselevel, float clipNear, const decMatrix &matrixCamera,
deoglOcclusionMap &occlusionMap2, int baselevel2, float clipNear2, const decMatrix& matrixCamera2 ){
	deoglRenderThread &renderThread = GetRenderThread();
	//const deoglConfiguration &config = *ogl.GetConfiguration();
	
	// render tests if required
	if( occlusionTest.GetInputDataCount() > 0 ){
		deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
		const decPoint &resultSize = occlusionTest.GetResultSize();
		deoglShaderCompiled *shader;
		
		DEBUG_PRINT_TIMER( "Entering Render Occlusion Tests" );
		
		if( occlusionTest.GetVBOResult() ){ // valid deoglConfiguration::eoctmTransformFeedback
			renderThread.GetShader().ActivateShader( pShaderOccTestTFBDual );
			shader = pShaderOccTestTFBDual->GetCompiled();
			DEBUG_PRINT_TIMER( "Activate FBO" );
			
			shader->SetParameterMatrix4x4( spttfbMatrix, matrixCamera );
			shader->SetParameterFloat( spttfbScaleSize, ( float )occlusionMap.GetWidth(), ( float )occlusionMap.GetHeight() );
			shader->SetParameterFloat( spttfbBaseLevel, ( float )baselevel );
			shader->SetParameterFloat( spttfbClipNear, clipNear );
			shader->SetParameterMatrix4x4( spttfbMatrix2, matrixCamera2 );
			shader->SetParameterFloat( spttfbScaleSize2, ( float )occlusionMap2.GetWidth(), ( float )occlusionMap2.GetHeight() );
			shader->SetParameterFloat( spttfbBaseLevel2, ( float )baselevel2 );
			shader->SetParameterFloat( spttfbClipNear2, clipNear2 );
			DEBUG_PRINT_TIMER( "Set Uniforms" );
			
			tsmgr.EnableArrayTexture( 0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap() );
			tsmgr.EnableArrayTexture( 1, *occlusionMap2.GetTexture(), GetSamplerClampNearestMipMap() );
			DEBUG_PRINT_TIMER( "Set Texture" );
			
			OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
			OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, occlusionTest.GetVBOResult() ) );
			OGL_CHECK( renderThread, pglBindVertexArray( occlusionTest.GetVAO() ) );
			OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
			OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occlusionTest.GetInputDataCount() ) );
			OGL_CHECK( renderThread, pglEndTransformFeedback() );
			OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
			OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
			DEBUG_PRINT_TIMER( "Draw with Feedback" );
			
		}else{ // invalid deoglConfiguration::eoctmTransformFeedback or deoglConfiguration::eoctmVBOToTexture
			renderThread.GetFramebuffer().Activate( occlusionTest.GetFBOResult() );
			
			OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
			OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
			OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
			OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
			
			OGL_CHECK( renderThread, glViewport( 0, 0, resultSize.x, resultSize.y ) );
			OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
			
			const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
			
			deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderOccTestDualStereo : pShaderOccTestDual;
			renderThread.GetShader().ActivateShader( program );
			shader = program->GetCompiled();
			
			// x:(0..1)=>(-1..1) , y:(0..height)=>(-1..1)
			shader->SetParameterFloat( sptPosTransform, 2.0f, 2.0f / ( float )( resultSize.y - 1 ), -1.0f, -1.0f );
			shader->SetParameterFloat( sptInvWidth, 1.0f / ( float )resultSize.x );
			shader->SetParameterDMatrix4x4( sptMatrix, matrixCamera );
			shader->SetParameterFloat( sptScaleSize, ( float )occlusionMap.GetWidth(), ( float )occlusionMap.GetHeight() );
			shader->SetParameterFloat( sptBaseLevel, ( float )baselevel );
			shader->SetParameterDMatrix4x4( sptMatrix2, matrixCamera2 );
			shader->SetParameterFloat( sptScaleSize2, ( float )occlusionMap2.GetWidth(), ( float )occlusionMap2.GetHeight() );
			shader->SetParameterFloat( sptBaseLevel2, ( float )baselevel2 );
			
			tsmgr.EnableArrayTexture( 0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap() );
			tsmgr.EnableArrayTexture( 1, *occlusionMap2.GetTexture(), GetSamplerClampNearestMipMap() );
			
			OGL_CHECK( renderThread, pglBindVertexArray( occlusionTest.GetVAO() ) );
			OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occlusionTest.GetInputDataCount() ) );
			OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		}
		
		// read back results
		occlusionTest.UpdateResults();
		DEBUG_PRINT_TIMER( "Update Results" );
	}
}



void deoglRenderOcclusion::DebugOcclusionMap( deoglRenderPlan &plan ){
	if( ! plan.GetDebug() ){
		return;
	}
	
	const deoglCollideList &clist = plan.GetCollideList();
	const int componentCount = clist.GetComponentCount();
	deoglRenderPlanDebug &debug = *plan.GetDebug();
	int i;
	
	debug.IncrementOccMapObjects( componentCount );
	
	for( i=0; i<componentCount; i++ ){
		const deoglCollideListComponent &clistComponent = *clist.GetComponentAt( i );
		const deoglRComponent &component = *clistComponent.GetComponent();
		
		if( ! component.GetModel() ){
			continue;
		}
		
		const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( clistComponent.GetLODLevel() );
		debug.IncrementOccMapTriangles( modelLOD.GetFaceCount() );
	}
}



#if 0
void deoglRenderOcclusion::RenderOcclusionCubeMap( const deoglCollideList &collideList,
deoglCubeMap *cubemap, const decDVector &position, float imageDistance, float viewDistance ){
	if( ! cubemap ){
		DETHROW( deeInvalidParam );
	}
	
	// calculate the z transformation required to do linear-z occlusion map
	// z' = (z-zn)/(zf-zn)
	// z' = z/(zf-zn)-zn/(zf-zn)
	// z' = z * [1/(zf-zn)] + [-zn/(zf-zn)]
	const float zscale = 1.0f / ( viewDistance - imageDistance );
	const float zoffset = -( imageDistance * zscale );
	
	// do the rest
	deoglRenderThread &renderThread = GetRenderThread();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const int size = cubemap->GetSize();
	deoglFramebuffer *fbo = NULL;
	decDMatrix matrixCamera;
	int cmf;
	
	const bool singlePass = false; //true;
	
	const decDMatrix matrixProjection( defren.CreateProjectionDMatrix(
		size, size, DEG2RAD * 90.0f, 1.0f, 0.01f, viewDistance + 0.01f ) );
	
	// calculate object render cube face special parameter. optimizes rendering by skipping
	// object faces on cube map faces they are not visible on
	collideList.UpdateOccMeshCubeFaceMasks( position );
	
	// configuration
	const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
	const bool useGSRenderCube = renderThread.GetExtensions().SupportsGeometryShader()
		&& ! bugClearEntireCubeMap
		&& singlePass;
	
	// setup render parameters
	if( useGSRenderCube ){
		// NOTE Y axis is flipped compared to opengl. pCubeFaces takes care of this
		pRenderParamBlock->MapBuffer();
		try{
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, position, vCubeFaces[ cmf ] );
				
				pRenderParamBlock->SetParameterDataArrayMat4x4( 0, cmf, matrixCamera * matrixProjection );
				pRenderParamBlock->SetParameterDataArrayMat4x3( 1, cmf, matrixCamera );  // unused
				pRenderParamBlock->SetParameterDataVec4( 2, matrixCamera.a31 * zscale,
					matrixCamera.a32 * zscale, matrixCamera.a33 * zscale,
					matrixCamera.a34 * zscale + zoffset ); // pTransformZ
				pRenderParamBlock->SetParameterDataVec2( 3, zscale, zoffset ); // pZToDepth
				pRenderParamBlock->SetParameterDataVec4( 4, 0.0f, 0.0f, 0.0f, 0.0f ); // pClipPlane
			}
			
		}catch( const deException & ){
			pRenderParamBlock->UnmapBuffer();
			throw;
		}
		pRenderParamBlock->UnmapBuffer();
	}
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_LEQUAL ) ); // lequal, occlusion uses linear depth
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( false );
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT,  GL_NEGATIVE_ONE_TO_ONE ); // reset, occlusion uses linear depth
	}
	
	OGL_CHECK( renderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	if( useGSRenderCube ){
		pAddToRenderTask->SetUseSpecialParamBlock( true );
		pAddToRenderTask->SetEnforceShader( pShaderOccMapCube->GetRTSShader() );
	}else{
		pAddToRenderTask->SetEnforceShader( pShaderOccMap->GetRTSShader() );
	}
	pAddToRenderTask->SetSolid( true );
	pAddToRenderTask->SetNoRendered( false );
	pAddToRenderTask->SetNoShadowNone( false );
	
	const GLfloat clearDepth = ( GLfloat )1.0f; // occlusion uses linear depth
	deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
	
	try{
		fbo = renderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
		renderThread.GetFramebuffer().Activate( fbo );
		
		fbo->DetachAllImages();
		
		const GLenum buffers[ 1 ] = { GL_NONE };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
		
		if( ! bugClearEntireCubeMap ){
			fbo->AttachDepthCubeMap( cubemap );
			OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		}
		
		if( useGSRenderCube ){
			// cube map activate already by clear
			
			pRenderTask->Clear();
			pRenderTask->SetRenderParamBlock( pRenderParamBlock );
			pRenderTask->SetUseSPBInstanceFlags( useGSRenderCube );
			
			pAddToRenderTask->AddOcclusionMeshes( collideList );
			
			pRenderTask->PrepareForRender();
			rengeom.RenderTask( *pRenderTask );
			
		}else{
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, position, vCubeFaces[ cmf ] );
				
				pRenderParamBlock->MapBuffer();
				try{
					pRenderParamBlock->SetParameterDataMat4x4( 0, matrixCamera * matrixProjection );
					pRenderParamBlock->SetParameterDataMat4x3( 1, matrixCamera );  // unused
					pRenderParamBlock->SetParameterDataVec4( 2, matrixCamera.a31 * zscale,
						matrixCamera.a32 * zscale, matrixCamera.a33 * zscale,
						matrixCamera.a34 * zscale + zoffset );
					pRenderParamBlock->SetParameterDataVec2( 3, zscale, zoffset ); // pZToDepth
					pRenderParamBlock->SetParameterDataVec4( 4, 0.0f, 0.0f, 0.0f, 0.0f ); // pClipPlane
					
				}catch( const deException & ){
					pRenderParamBlock->UnmapBuffer();
					throw;
				}
				pRenderParamBlock->UnmapBuffer();
				
				fbo->AttachDepthCubeMapFace( cubemap, vCubeFaces[ cmf ] );
				fbo->Verify();
				
				if( bugClearEntireCubeMap ){
					OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
				}
				
				pRenderTask->Clear();
				pRenderTask->SetRenderParamBlock( pRenderParamBlock );
				
				pAddToRenderTask->SetFilterCubeFace( cmf );
				pAddToRenderTask->AddOcclusionMeshes( collideList );
				
				pRenderTask->PrepareForRender();
				rengeom.RenderTask( *pRenderTask );
			}
		}
		
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		
	}catch( const deException & ){
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		throw;
	}
}
#endif



// Private Functions
//////////////////////

void deoglRenderOcclusion::pCleanUp(){
	if( pAddToRenderTask ){
		delete pAddToRenderTask;
	}
	if( pRenderTask ){
		delete pRenderTask;
	}
	if( pOccMapFrustumParamBlock ){
		pOccMapFrustumParamBlock->FreeReference();
	}
	if( pRenderParamBlock ){
		pRenderParamBlock->FreeReference();
	}
	
	deoglDelayedOperations &dops = GetRenderThread().GetDelayedOperations();
	dops.DeleteOpenGLVertexArray( pVAOFrustumPlanes );
	dops.DeleteOpenGLBuffer( pVBOFrustumPlanes );
}
