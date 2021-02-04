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

#include "deoglRenderGeometry.h"
#include "deoglRenderOcclusion.h"
#include "debug/deoglRenderDebug.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlanDebug.h"
#include "plan/deoglRenderPlan.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "task/deoglRenderTaskShader.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../light/deoglRLight.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../extensions/deoglExtensions.h"
#include "../occlusiontest/deoglOcclusionMap.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occlusiontest/deoglOcclusionTracing.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
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



//#define DO_TIMING

#ifdef DO_TIMING
static decTimer dtimer;

#define DEBUG_RESET_TIMERS dtimer.Reset();
#define DEBUG_PRINT_TIMER(what) renderThread.GetLogger().LogInfoFormat("Occlusion-Test %s = %iys",\
	what, ( int )( dtimer.GetElapsedTime() * 1000000.0 ) ); dtimer.Reset()
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#endif



//#define DO_TIMING2

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
	sptFrustumTestMul
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
	spttfbFrustumTestMul
};

enum eSPTracingGenRays{
	sptgrRaysPerProbe,
	sptgrProbeCount,
	sptgrProbeSpacing,
	sptgrProbesPerLine,
	sptgrRandomOrientation
};

enum eSPTracing{
	sptBVHInstanceRootNode
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

pShaderOccMap( NULL ),
pShaderOccMapOrtho( NULL ),
pShaderOccMapDownSample( NULL ),
pShaderOccTest( NULL ),
pShaderOccTestDual( NULL ),
pShaderOccTestSun( NULL ),
pShaderOccTestTFB( NULL ),
pShaderOccTestTFBDual( NULL ),
pShaderOccTestTFBSun( NULL ),
pShaderOccMapCube( NULL ),
pShaderOccTracingGenRays( NULL ),
pShaderOccTracingTraceRays( NULL ),
pShaderOccTracingDebug( NULL ),
pShaderOccTracingUpdateOcclusion( NULL ),
pShaderOccTracingUpdateDistance( NULL ),

pRenderParamBlock( NULL ),
pOccMapFrustumParamBlock( NULL ),
pRenderTask( NULL ),
pAddToRenderTask( NULL )
{
	const bool indirectMatrixAccessBug = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap" );
		AddOccMapDefines( defines );
		pShaderOccMapOrtho = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "PERSPECTIVE_TO_LINEAR", "1" );
		pShaderOccMap = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap Down-Sample" );
		pShaderOccMapDownSample = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion Test" );
		defines.AddDefine( "ENSURE_MIN_SIZE", "1" );
		pShaderOccTest = shaderManager.GetProgramWith( sources, defines );
		defines.AddDefine( "DUAL_OCCMAP", "1" );
		pShaderOccTestDual = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		defines.AddDefine( "ENSURE_MIN_SIZE", "1" );
		defines.AddDefine( "FRUSTUM_TEST", "1" );
		pShaderOccTestSun = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion Test TFB" );
		defines.AddDefine( "ENSURE_MIN_SIZE", "1" );
		pShaderOccTestTFB = shaderManager.GetProgramWith( sources, defines );
		defines.AddDefine( "DUAL_OCCMAP", "1" );
		pShaderOccTestTFBDual = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		defines.AddDefine( "ENSURE_MIN_SIZE", "1" );
		defines.AddDefine( "FRUSTUM_TEST", "1" );
		pShaderOccTestTFBSun = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
		const bool useGSRenderCube = renderThread.GetExtensions().SupportsGeometryShader()
			&& ! bugClearEntireCubeMap;
		
		if( useGSRenderCube ){
			sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap Cube" );
			AddOccMapDefines( defines );
			defines.AddDefine( "PERSPECTIVE_TO_LINEAR", "1" );
			
			defines.AddDefine( "GS_RENDER_CUBE", "1" );
			defines.AddDefine( "GS_RENDER_CUBE_CULLING", "1" );
			if( renderThread.GetExtensions().SupportsGSInstancing() ){
				defines.AddDefine( "GS_RENDER_CUBE_INSTANCING", "1" );
			}
			
			pShaderOccMapCube = shaderManager.GetProgramWith( sources, defines );
			defines.RemoveAllDefines();
		}
		
// 		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion Tracing Generate Rays" );
// 		pShaderOccTracingGenRays = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion Tracing Trace Rays" );
		pShaderOccTracingTraceRays = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "DEBUG_TRACING", "1" );
		pShaderOccTracingDebug = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion Tracing Update Maps" );
		defines.AddDefine( "MAP_OCCLUSION", "1" );
		pShaderOccTracingUpdateOcclusion = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		defines.AddDefine( "MAP_DISTANCE", "1" );
		pShaderOccTracingUpdateDistance = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		pRenderParamBlock = new deoglSPBlockUBO( renderThread );
		pRenderParamBlock->SetRowMajor( ! indirectMatrixAccessBug );
		pRenderParamBlock->SetParameterCount( 4 );
		pRenderParamBlock->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 4, 6 ); // mat4 pMatrixVP[ 6 ]
		pRenderParamBlock->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 6 ); // mat4x3 pMatrixV[ 6 ]
		pRenderParamBlock->GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 6 ); // vec4 pTransformZ[ 6 ]
		pRenderParamBlock->GetParameterAt( 3 ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2 pZToDepth
		pRenderParamBlock->MapToStd140();
		pRenderParamBlock->SetBindingPoint( deoglSkinShader::eubRenderParameters );
		
		pOccMapFrustumParamBlock = new deoglSPBlockUBO( renderThread);
		pOccMapFrustumParamBlock->SetParameterCount( 1 );
		pOccMapFrustumParamBlock->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3 pMatrixModel
		pOccMapFrustumParamBlock->MapToStd140();
		pOccMapFrustumParamBlock->SetBindingPoint( deoglSkinShader::eubInstanceParameters );
		
		pRenderTask = new deoglRenderTask;
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
	
	if( deoglSkinShader::USE_SHARED_SPB ){
		const deoglRenderThread &renderThread = GetRenderThread();
		const deoglRTBufferObject &bo = renderThread.GetBufferObject();
		decString value;
		
		defines.AddDefine( "SHARED_SPB", "1" );
		
		if( renderThread.GetChoices().GetSharedSPBUseSSBO() ){
			defines.AddDefine( "SHARED_SPB_USE_SSBO", "1" );
			
			if( bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() >= 16 ){
				value.SetValue( bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() / 16 );
				defines.AddDefine( "SHARED_SPB_PADDING", value );
			}
			
		}else{
			// NOTE UBO requires array size to be constant, SSBO does not
			if( bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() > 0 ){
				value.SetValue( bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() );
				defines.AddDefine( "SHARED_SPB_ARRAY_SIZE", value );
			}
			
			if( bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() >= 16 ){
				value.SetValue( bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() / 16 );
				defines.AddDefine( "SHARED_SPB_PADDING", value );
			}
		}
		
		if( bo.GetInstanceArraySizeUBO() > 0 ){
			value.SetValue( bo.GetInstanceArraySizeUBO() );
			defines.AddDefine( "SPB_INSTANCE_ARRAY_SIZE", value );
		}
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

The convertionof z position to depth value has to be removed and projective depth
written out by the fixed function pipeline.

In the test shader max() calls are used for filtering down. For inverse depth this
has to be changed to min() calls.

The same is true for the downsampling. It needs to use min() instead of max() too
if inverse depth is used.

Depth compare direction has to change from <= to >= if inverse depth is used.
*/

void deoglRenderOcclusion::RenderTestsCamera( deoglRenderPlan &plan ){
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	
	if( config.GetOcclusionTestMode() == deoglConfiguration::eoctmNone ){
		return;
	}
	
	deoglRenderPlanDebug * const renderPlanDebug = plan.GetDebug();
	decMatrix matrixProjection = plan.GetFrustumMatrix(); // occlusion uses linear depth: always non-infinite projection matrix 
	decMatrix matrixCamera = plan.GetRefPosCameraMatrix();
	
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
	DEBUG_PRINT_TIMER( "Entering Render Occlusion Map" );
	pRenderParamBlock->MapBuffer();
	try{
		// 0: pMatrixVP[ 0 ]
		// 1: pMatrixV[ 0 ]
		// 2: pTransformZ[ 0 ]
		// 3: pZToDepth
		pRenderParamBlock->SetParameterDataMat4x4( 0, matrixCamera * matrixProjection );
		pRenderParamBlock->SetParameterDataMat4x3( 1, matrixCamera );
		pRenderParamBlock->SetParameterDataVec4( 2, matrixCamera.a31,
			matrixCamera.a32, matrixCamera.a33, matrixCamera.a34 );
		pRenderParamBlock->SetParameterDataVec2( 3, zscale, zoffset + occmapResolution * occmapBias );
		
	}catch( const deException & ){
		pRenderParamBlock->UnmapBuffer();
		throw;
	}
	pRenderParamBlock->UnmapBuffer();
	DEBUG_PRINT_TIMER( "Update Param Block" );
	
	// render occlusion map
	if( renderPlanDebug ){
		renderPlanDebug->IncrementOccMapObjects( plan.GetCollideList().GetComponentCount() );
		RenderOcclusionMap( plan );
		renderPlanDebug->IncrementOccMapTriangles( pRenderTask->GetTotalPointCount() / 3 );
		//glFinish();
		renderPlanDebug->SetOccMapRenderTime( timer.GetElapsedTime() );
		
	}else{
		RenderOcclusionMap( plan );
	}
	
	// render visibility tests
	matrixProjection.a33 = zscale * 2.0f;
	matrixProjection.a34 = zoffset * 2.0f - 1.0f;
	matrixCamera = matrixCamera.GetRotationMatrix() * matrixProjection;
	plan.SetOcclusionTestMatrix( matrixCamera );
	
	RenderOcclusionTests( *plan.GetOcclusionMap(), plan.GetOcclusionMapBaseLevel(), -1.0f, matrixCamera );
	plan.GetCollideList().RemoveVisibleComponents( false );
	plan.GetCollideList().RemoveVisibleLights( false );
	DEBUG_PRINT_TIMER( "End tests and remove invisible" );
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
deoglRSkyInstanceLayer &skyLayer, deoglCollideList &collideList,
const decVector &minExtend, const decVector &maxExtend ){
	DEBUG_RESET_TIMERS;
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	
	if( config.GetOcclusionTestMode() == deoglConfiguration::eoctmNone ){
		return;
	}
	
	deoglOcclusionMap &occmap = *plan.GetOcclusionMap();
	const int baselevel = plan.GetOcclusionMapBaseLevel();
	
	const decDVector &referencePosition = plan.GetWorld()->GetReferencePosition();
	const decDVector &camPos = plan.GetCameraPosition();
	decMatrix matrixCamera, matrixCamera2;
	decVector position, scale;
	
	// calculate the camera matrix fitting around all splits
	const decMatrix matLig = decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * skyLayer.GetMatrix();
	
	position.x = ( minExtend.x + maxExtend.x ) * 0.5f;
	position.y = ( minExtend.y + maxExtend.y ) * 0.5f;
	position.z = ( minExtend.z + maxExtend.z ) * 0.5f;
	position = ( camPos - referencePosition ).ToVector() + ( matLig * position );
	
	scale.x = 2.0f / ( maxExtend.x - minExtend.x );
	scale.y = 2.0f / ( maxExtend.y - minExtend.y );
	scale.z = 2.0f / ( maxExtend.z - minExtend.z );
	
	matrixCamera = decMatrix::CreateCamera( position, matLig.TransformView(), matLig.TransformUp() ) * decMatrix::CreateScale( scale );
	matrixCamera2 = matrixCamera.Invert() * decMatrix::CreateTranslation( ( referencePosition - camPos ).ToVector() ) * plan.GetOcclusionTestMatrix();
	DEBUG_PRINT_TIMER( "Prepare" );
	
	RenderOcclusionTestsSun( occmap, baselevel, -100.0f, matrixCamera, -1.0f, matrixCamera2, plan );
	collideList.RemoveVisibleComponents( false );
	DEBUG_PRINT_TIMER( "End tests and remove invisible" );
}

void deoglRenderOcclusion::RenderOcclusionMap( deoglRenderPlan &plan ){
	deoglOcclusionMap &occmap = *plan.GetOcclusionMap();
	const int baselevel = plan.GetOcclusionMapBaseLevel();
	const deoglCollideList &collideList = plan.GetCollideList();
	const decMatrix *renderFrustumPlanesMatrix = NULL;
	const bool perspective = true;

	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderGeometry &rengeom = GetRenderThread().GetRenderers().GetGeometry();
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
	DEBUG_PRINT_TIMER( "Set States" );
	
	// prepare fbo
	renderThread.GetFramebuffer().Activate( occmap.GetFBOAt( 0 ) );
	DEBUG_PRINT_TIMER( "Activate FBO" );
	
	width = occmap.GetWidth();
	height = occmap.GetHeight();
	for( i=1; i<baselevel; i++ ){
		width >>= 1;
		height >>= 1;
	}
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	DEBUG_PRINT_TIMER( "Viewport" );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	DEBUG_PRINT_TIMER( "Scissor Test" );
	
	// clear the occlusion map
	const GLfloat clearDepth = ( GLfloat )1.0f; // occlusion uses linear depth
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	DEBUG_PRINT_TIMER( "Clear" );
	
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
		if( perspective ){
			renderThread.GetShader().ActivateShader( pShaderOccMap );
			shader = pShaderOccMap->GetCompiled();
			
		}else{
			renderThread.GetShader().ActivateShader( pShaderOccMapOrtho );
			shader = pShaderOccMapOrtho->GetCompiled();
		}
		
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
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) ); // left
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 4, 4 ) ); // top
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 8, 4 ) ); // right
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 12, 4 ) ); // bottom
		//OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, 36 ) );
		
		pglBindVertexArray( 0 );
		DEBUG_PRINT_TIMER( "Render Frustum Planes" );
	}
	
	// render solid content
	pRenderTask->Clear();
	pRenderTask->SetRenderParamBlock( pRenderParamBlock );
	
	pAddToRenderTask->Reset();
	pAddToRenderTask->SetSolid( true );
	pAddToRenderTask->SetNoRendered( plan.GetNoRenderedOccMesh() );
	
	if( perspective ){
		pAddToRenderTask->SetEnforceShader( pShaderOccMap );
		
	}else{
		pAddToRenderTask->SetEnforceShader( pShaderOccMapOrtho );
	}
	
	pAddToRenderTask->AddOcclusionMeshes( collideList );
	DEBUG_PRINT_TIMER( "Build Render Task" );
	
	DEBUG_PRINT_TIMER2( "Time until before render occmap" );
	pRenderTask->PrepareForRender( renderThread );
	rengeom.RenderTask( *pRenderTask );
	DEBUG_PRINT_TIMER( "Render Render Task" );
	DEBUG_PRINT_TIMER2( "Time until after render occmap" );
	
	// create mipmap level (z-pyramid)
	renderThread.GetShader().ActivateShader( pShaderOccMapDownSample );
	DEBUG_PRINT_TIMER( "Activate Down-Sample Shader" );
	shader = pShaderOccMapDownSample->GetCompiled();
	
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) ); // ati bug, disabling depth testing gets you in hell's kitchen
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	DEBUG_PRINT_TIMER( "Set Down-Sample State" );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	DEBUG_PRINT_TIMER( "Bind Full-Screen VAO" );
	
	tsmgr.EnableTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
	DEBUG_PRINT_TIMER( "Enable OccMap Texture" );
	
	for( i=baselevel+1; i<levelCount; i++ ){
		width >>= 1;
		height >>= 1;
		
		renderThread.GetFramebuffer().Activate( occmap.GetFBOAt( i ) );
		DEBUG_PRINT_TIMER( "Activate Level FBO" );
		OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
		DEBUG_PRINT_TIMER( "Level Viewport" );
		
		shader->SetParameterInt( spomdsLevel, i - 1 );
		DEBUG_PRINT_TIMER( "Level Set Uniform" );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		DEBUG_PRINT_TIMER( "Level Draw Quad" );
		#ifdef DO_TIMING2
		if( i == baselevel+1 ){
			DEBUG_PRINT_TIMER2( "Time until first MipMap level" );
		}else if( i == levelCount-1 ){
			DEBUG_PRINT_TIMER2( "Time until last MipMap level" );
		}
		#endif
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	DEBUG_PRINT_TIMER( "Disable VAO" );
}



void deoglRenderOcclusion::RenderOcclusionTests( deoglOcclusionMap &occmap, int baselevel,
float clipNear, const decMatrix &matrixCamera ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglOcclusionTest &occtest = renderThread.GetOcclusionTest();
	
	// render tests if required
	if( occtest.GetInputDataCount() == 0 ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const int resultHeight = occtest.GetResultHeight();
	const int resultWidth = occtest.GetResultWidth();
	deoglShaderCompiled *shader;
	
	DEBUG_PRINT_TIMER( "Entering Render Occlusion Tests" );
	
	if( occtest.GetVBOResult() ){ // valid deoglConfiguration::eoctmTransformFeedback
		renderThread.GetShader().ActivateShader( pShaderOccTestTFB );
		shader = pShaderOccTestTFB->GetCompiled();
		DEBUG_PRINT_TIMER( "Activate FBO" );
		
		shader->SetParameterMatrix4x4( spttfbMatrix, matrixCamera );
		shader->SetParameterFloat( spttfbScaleSize, ( float )occmap.GetWidth(), ( float )occmap.GetHeight() );
		shader->SetParameterFloat( spttfbBaseLevel, ( float )baselevel );
		shader->SetParameterFloat( spttfbClipNear, clipNear );
		DEBUG_PRINT_TIMER( "Set Uniforms" );
		
		tsmgr.EnableTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
		DEBUG_PRINT_TIMER( "Set Texture" );
		
		if( renderThread.GetCapabilities().GetRasterizerDiscard().Broken() ){
			OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
			OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		}
		
		OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, occtest.GetVBOResult() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( occtest.GetVAO() ) );
		OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occtest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglEndTransformFeedback() );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
		DEBUG_PRINT_TIMER( "Draw with Feedback" );
		DEBUG_PRINT_TIMER2( "Time until render tests" );
		
	}else{ // invalid deoglConfiguration::eoctmTransformFeedback or deoglConfiguration::eoctmVBOToTexture
		renderThread.GetFramebuffer().Activate( occtest.GetFBOResult() );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, resultWidth, resultHeight ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		
		renderThread.GetShader().ActivateShader( pShaderOccTest );
		shader = pShaderOccTest->GetCompiled();
		
		// x:(0..1)=>(-1..1) , y:(0..height)=>(-1..1)
		shader->SetParameterFloat( sptPosTransform, 2.0f, 2.0f / ( float )( resultHeight - 1 ), -1.0f, -1.0f );
		shader->SetParameterFloat( sptInvWidth, 1.0f / ( float )resultWidth );
		shader->SetParameterDMatrix4x4( sptMatrix, matrixCamera );
		shader->SetParameterFloat( sptScaleSize, ( float )occmap.GetWidth(), ( float )occmap.GetHeight() );
		shader->SetParameterFloat( sptBaseLevel, ( float )baselevel );
		
		tsmgr.EnableTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
		
		OGL_CHECK( renderThread, pglBindVertexArray( occtest.GetVAO() ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occtest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	}
	
	// read back results
	occtest.UpdateResults();
	DEBUG_PRINT_TIMER( "Update Results" );
	DEBUG_PRINT_TIMER2( "Time until update results" );
}

void deoglRenderOcclusion::RenderOcclusionTestsSun( deoglOcclusionMap &occmap, int baselevel, float clipNear,
const decMatrix &matrixCamera, float clipNear2, const decMatrix &matrixCamera2, deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglOcclusionTest &occtest = renderThread.GetOcclusionTest();
	
	if( occtest.GetInputDataCount() == 0 ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const int resultHeight = occtest.GetResultHeight();
	const int resultWidth = occtest.GetResultWidth();
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
	
	if( occtest.GetVBOResult() ){ // valid deoglConfiguration::eoctmTransformFeedback
		renderThread.GetShader().ActivateShader( pShaderOccTestTFBSun );
		shader = pShaderOccTestTFBSun->GetCompiled();
		DEBUG_PRINT_TIMER( "Activate FBO" );
		
		shader->SetParameterMatrix4x4( spttfbMatrix, matrixCamera );
		shader->SetParameterFloat( spttfbScaleSize, ( float )occmap.GetWidth(), ( float )occmap.GetHeight() );
		shader->SetParameterFloat( spttfbBaseLevel, ( float )baselevel );
		shader->SetParameterFloat( spttfbClipNear, clipNear );
		shader->SetParameterMatrix4x4( spttfbMatrix2, matrixCamera2 );
		shader->SetParameterFloat( spttfbClipNear2, clipNear2 );
		shader->SetParameterVector3( spttfbFrustumNormal1, frustumPlaneNormal[ 0 ] );
		shader->SetParameterVector3( spttfbFrustumNormal2, frustumPlaneNormal[ 1 ] );
		shader->SetParameterVector3( spttfbFrustumNormal3, frustumPlaneNormal[ 2 ] );
		shader->SetParameterVector3( spttfbFrustumNormal4, frustumPlaneNormal[ 3 ] );
		shader->SetParameterFloat( spttfbFrustumTestAdd, frustumFactorAdd[ 0 ], frustumFactorAdd[ 1 ], frustumFactorAdd[ 2 ], frustumFactorAdd[ 3 ] );
		shader->SetParameterFloat( spttfbFrustumTestMul, frustumFactorMul[ 0 ], frustumFactorMul[ 1 ], frustumFactorMul[ 2 ], frustumFactorMul[ 3 ] );
		DEBUG_PRINT_TIMER( "Set Uniforms" );
		
		tsmgr.EnableTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
		DEBUG_PRINT_TIMER( "Set Texture" );
		
		if( renderThread.GetCapabilities().GetRasterizerDiscard().Broken() ){
			OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
			OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		}
		
		OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, occtest.GetVBOResult() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( occtest.GetVAO() ) );
		OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occtest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglEndTransformFeedback() );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
		DEBUG_PRINT_TIMER( "Draw with Feedback" );
		
	}else{ // invalid deoglConfiguration::eoctmTransformFeedback or deoglConfiguration::eoctmVBOToTexture
		renderThread.GetFramebuffer().Activate( occtest.GetFBOResult() );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, resultWidth, resultHeight ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		
		renderThread.GetShader().ActivateShader( pShaderOccTestSun );
		shader = pShaderOccTestSun->GetCompiled();
		
		// x:(0..1)=>(-1..1) , y:(0..height)=>(-1..1)
		shader->SetParameterFloat( sptPosTransform, 2.0f, 2.0f / ( float )( resultHeight - 1 ), -1.0f, -1.0f );
		shader->SetParameterFloat( sptInvWidth, 1.0f / ( float )resultWidth );
		shader->SetParameterDMatrix4x4( sptMatrix, matrixCamera );
		shader->SetParameterFloat( sptScaleSize, ( float )occmap.GetWidth(), ( float )occmap.GetHeight() );
		shader->SetParameterFloat( sptBaseLevel, ( float )baselevel );
		shader->SetParameterDMatrix4x4( sptMatrix2, matrixCamera2 );
		shader->SetParameterVector3( sptFrustumNormal1, frustumPlaneNormal[ 0 ] );
		shader->SetParameterVector3( sptFrustumNormal2, frustumPlaneNormal[ 1 ] );
		shader->SetParameterVector3( sptFrustumNormal3, frustumPlaneNormal[ 2 ] );
		shader->SetParameterVector3( sptFrustumNormal4, frustumPlaneNormal[ 3 ] );
		shader->SetParameterFloat( sptFrustumTestAdd, frustumFactorAdd[ 0 ], frustumFactorAdd[ 1 ], frustumFactorAdd[ 2 ], frustumFactorAdd[ 3 ] );
		shader->SetParameterFloat( sptFrustumTestMul, frustumFactorMul[ 0 ], frustumFactorMul[ 1 ], frustumFactorMul[ 2 ], frustumFactorMul[ 3 ] );
		
		tsmgr.EnableTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
		
		OGL_CHECK( renderThread, pglBindVertexArray( occtest.GetVAO() ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occtest.GetInputDataCount() ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	}
	
	// read back results
	occtest.UpdateResults();
	DEBUG_PRINT_TIMER( "Update Results" );
}

void deoglRenderOcclusion::RenderOcclusionTestsDual( deoglOcclusionMap &occmap, int baselevel, float clipNear,
const decMatrix &matrixCamera, deoglOcclusionMap &occmap2, int baselevel2, float clipNear2, const decMatrix& matrixCamera2 ){
	deoglRenderThread &renderThread = GetRenderThread();
	//const deoglConfiguration &config = *ogl.GetConfiguration();
	deoglOcclusionTest &occtest = renderThread.GetOcclusionTest();
	
	// render tests if required
	if( occtest.GetInputDataCount() > 0 ){
		deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
		const int resultHeight = occtest.GetResultHeight();
		const int resultWidth = occtest.GetResultWidth();
		deoglShaderCompiled *shader;
		
		DEBUG_PRINT_TIMER( "Entering Render Occlusion Tests" );
		
		if( occtest.GetVBOResult() ){ // valid deoglConfiguration::eoctmTransformFeedback
			renderThread.GetShader().ActivateShader( pShaderOccTestTFBDual );
			shader = pShaderOccTestTFBDual->GetCompiled();
			DEBUG_PRINT_TIMER( "Activate FBO" );
			
			shader->SetParameterMatrix4x4( spttfbMatrix, matrixCamera );
			shader->SetParameterFloat( spttfbScaleSize, ( float )occmap.GetWidth(), ( float )occmap.GetHeight() );
			shader->SetParameterFloat( spttfbBaseLevel, ( float )baselevel );
			shader->SetParameterFloat( spttfbClipNear, clipNear );
			shader->SetParameterMatrix4x4( spttfbMatrix2, matrixCamera2 );
			shader->SetParameterFloat( spttfbScaleSize2, ( float )occmap2.GetWidth(), ( float )occmap2.GetHeight() );
			shader->SetParameterFloat( spttfbBaseLevel2, ( float )baselevel2 );
			shader->SetParameterFloat( spttfbClipNear2, clipNear2 );
			DEBUG_PRINT_TIMER( "Set Uniforms" );
			
			tsmgr.EnableTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
			tsmgr.EnableTexture( 1, *occmap2.GetTexture(), GetSamplerClampNearestMipMap() );
			DEBUG_PRINT_TIMER( "Set Texture" );
			
			OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
			OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, occtest.GetVBOResult() ) );
			OGL_CHECK( renderThread, pglBindVertexArray( occtest.GetVAO() ) );
			OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
			OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occtest.GetInputDataCount() ) );
			OGL_CHECK( renderThread, pglEndTransformFeedback() );
			OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
			OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
			DEBUG_PRINT_TIMER( "Draw with Feedback" );
			
		}else{ // invalid deoglConfiguration::eoctmTransformFeedback or deoglConfiguration::eoctmVBOToTexture
			renderThread.GetFramebuffer().Activate( occtest.GetFBOResult() );
			
			OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
			OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
			OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
			OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
			
			OGL_CHECK( renderThread, glViewport( 0, 0, resultWidth, resultHeight ) );
			OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
			
			const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
			
			renderThread.GetShader().ActivateShader( pShaderOccTestDual );
			shader = pShaderOccTestDual->GetCompiled();
			
			// x:(0..1)=>(-1..1) , y:(0..height)=>(-1..1)
			shader->SetParameterFloat( sptPosTransform, 2.0f, 2.0f / ( float )( resultHeight - 1 ), -1.0f, -1.0f );
			shader->SetParameterFloat( sptInvWidth, 1.0f / ( float )resultWidth );
			shader->SetParameterDMatrix4x4( sptMatrix, matrixCamera );
			shader->SetParameterFloat( sptScaleSize, ( float )occmap.GetWidth(), ( float )occmap.GetHeight() );
			shader->SetParameterFloat( sptBaseLevel, ( float )baselevel );
			shader->SetParameterDMatrix4x4( sptMatrix2, matrixCamera2 );
			shader->SetParameterFloat( sptScaleSize2, ( float )occmap2.GetWidth(), ( float )occmap2.GetHeight() );
			shader->SetParameterFloat( sptBaseLevel2, ( float )baselevel2 );
			
			tsmgr.EnableTexture( 0, *occmap.GetTexture(), GetSamplerClampNearestMipMap() );
			tsmgr.EnableTexture( 1, *occmap2.GetTexture(), GetSamplerClampNearestMipMap() );
			
			OGL_CHECK( renderThread, pglBindVertexArray( occtest.GetVAO() ) );
			OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, occtest.GetInputDataCount() ) );
			OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		}
		
		// read back results
		occtest.UpdateResults();
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
	int i;
	
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		deoglRComponent &component = *collideList.GetComponentAt( i )->GetComponent();
		if( component.GetOcclusionMesh() ){
			component.UpdateCubeFaceVisibility( position );
			if( deoglSkinShader::USE_SHARED_SPB ){
				component.SetSpecialFlagsFromFaceVisibility();
			}
		}
	}
	
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
				pRenderParamBlock->SetParameterDataVec4( 2, matrixCamera.a31,
					matrixCamera.a32, matrixCamera.a33, matrixCamera.a34 ); // pTransformZ
				pRenderParamBlock->SetParameterDataVec2( 3, zscale, zoffset ); // pZToDepth
			}
			
		}catch( const deException & ){
			pRenderParamBlock->UnmapBuffer();
			throw;
		}
		pRenderParamBlock->UnmapBuffer();
		
		// update object render cube face special parameter. optimizes rendering by skipping
		// object faces on cube map faces they are not visible on
		if( ! deoglSkinShader::USE_SHARED_SPB ){
			for( i=0; i<componentCount; i++ ){
				deoglRComponent &component = *collideList.GetComponentAt( i )->GetComponent();
				if( component.GetOcclusionMesh() ){
					component.UpdateSpecialSPBCubeRender();
				}
			}
		}
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
		pAddToRenderTask->SetEnforceShader( pShaderOccMapCube );
	}else{
		pAddToRenderTask->SetEnforceShader( pShaderOccMap );
	}
	pAddToRenderTask->SetSolid( true );
	pAddToRenderTask->SetWithHoles( false );
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
			
			pRenderTask->PrepareForRender( renderThread );
			rengeom.RenderTask( *pRenderTask );
			
		}else{
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, position, vCubeFaces[ cmf ] );
				
				pRenderParamBlock->MapBuffer();
				try{
					pRenderParamBlock->SetParameterDataMat4x4( 0, matrixCamera * matrixProjection );
					pRenderParamBlock->SetParameterDataMat4x3( 1, matrixCamera );  // unused
					pRenderParamBlock->SetParameterDataVec4( 2, matrixCamera.a31,
						matrixCamera.a32, matrixCamera.a33, matrixCamera.a34 );
					pRenderParamBlock->SetParameterDataVec2( 3, zscale, zoffset ); // pZToDepth
					
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
				
				pRenderTask->PrepareForRender( renderThread );
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

void deoglRenderOcclusion::RenderOcclusionTraceProbes( deoglOcclusionTracing &tracing ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	
	// set common states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	
	// calculate rays to use for probe updating
	/*
	const int maxUpdateProbeCount = tracing.GetTextureRayOrigin().GetHeight();
	const int raysPerProbe = tracing.GetTextureRayOrigin().GetWidth();
	const decPoint3 &probeCount = tracing.GetProbeCount();
	
	renderThread.GetFramebuffer().Activate( &tracing.GetFBORay() );
	OGL_CHECK( renderThread, glViewport( 0, 0, raysPerProbe, maxUpdateProbeCount ) );
	tsmgr.DisableAllStages();
	
	renderThread.GetShader().ActivateShader( pShaderOccTracingGenRays );
	deoglShaderCompiled &shaderGenRay = *pShaderOccTracingGenRays->GetCompiled();
	
	shaderGenRay.SetParameterInt( sptgrRaysPerProbe, raysPerProbe );
	shaderGenRay.SetParameterPoint3( sptgrProbeCount, probeCount );
	shaderGenRay.SetParameterDVector3( sptgrProbeSpacing, tracing.GetProbeSpacing() );
	shaderGenRay.SetParameterInt( sptgrProbesPerLine, tracing.GetProbesPerLine() );
	shaderGenRay.SetParameterMatrix3x3( sptgrRandomOrientation, decMatrix::CreateRotation(
		decMath::random( -PI, PI ), decMath::random( -PI, PI ), decMath::random( -PI, PI ) ) );
	
	tsmgr.DisableAllStages();
	
	const GLfloat clearRayOrigin[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearRayDirection[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearRayOrigin[ 0 ] ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 1, &clearRayDirection[ 0 ] ) );
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	*/
	
	
	// trace rays
	const decPoint &sampleImageSize = tracing.GetSampleImageSize();
	
	defren.Resize( sampleImageSize.x, sampleImageSize.y );
	defren.ActivateFBOColor( false, false );
	
// 	OGL_CHECK( renderThread, glViewport( 0, 0, 256, 256 ) );
	OGL_CHECK( renderThread, glViewport( 0, 0, sampleImageSize.x, sampleImageSize.y ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, sampleImageSize.x, sampleImageSize.y ) );
	
	tsmgr.EnableTBO( 0, tracing.GetTBONodeBox().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 1, tracing.GetTBOIndex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 2, tracing.GetTBOInstance().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 3, tracing.GetTBOMatrix().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 4, tracing.GetTBOFace().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 5, tracing.GetTBOVertex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 5 );
	
// 	renderThread.GetShader().ActivateShader( pShaderOccTracingDebug );
	renderThread.GetShader().ActivateShader( pShaderOccTracingTraceRays );
	tracing.GetUBOTracing()->Activate();
	
	const GLfloat clearPosition[ 4 ] = { 0.0f, 0.0f, 0.0f, 10000.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearPosition[ 0 ] ) );
	
// 	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, tracing.GetUpdateProbeCount() ) );
	
	
	// update probes: occlusion map
	renderThread.GetFramebuffer().Activate( &tracing.GetFBOProbeOcclusion() );
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glViewport( 0, 0, tracing.GetTextureProbeOcclusion().GetWidth(),
		tracing.GetTextureProbeOcclusion().GetHeight() ) );
	
	tsmgr.EnableTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 0 );
	
	renderThread.GetShader().ActivateShader( pShaderOccTracingUpdateOcclusion );
	tracing.GetUBOTracing()->Activate();
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, tracing.GetUpdateProbeCount() ) );
	
	
	// update probes: distance map
	renderThread.GetFramebuffer().Activate( &tracing.GetFBOProbeDistance() );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, tracing.GetTextureProbeDistance().GetWidth(),
		tracing.GetTextureProbeDistance().GetHeight() ) );
	
	renderThread.GetShader().ActivateShader( pShaderOccTracingUpdateDistance );
	tracing.GetUBOTracing()->Activate();
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, tracing.GetUpdateProbeCount() ) );
	
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
}



// Private Functions
//////////////////////

void deoglRenderOcclusion::pCleanUp(){
	if( pShaderOccMap ){
		pShaderOccMap->RemoveUsage();
	}
	if( pShaderOccMapOrtho ){
		pShaderOccMapOrtho->RemoveUsage();
	}
	if( pShaderOccMapDownSample ){
		pShaderOccMapDownSample->RemoveUsage();
	}
	if( pShaderOccTest ){
		pShaderOccTest->RemoveUsage();
	}
	if( pShaderOccTestDual ){
		pShaderOccTestDual->RemoveUsage();
	}
	if( pShaderOccTestSun ){
		pShaderOccTestSun->RemoveUsage();
	}
	if( pShaderOccTestTFB ){
		pShaderOccTestTFB->RemoveUsage();
	}
	if( pShaderOccTestTFBDual ){
		pShaderOccTestTFBDual->RemoveUsage();
	}
	if( pShaderOccTestTFBSun ){
		pShaderOccTestTFBSun->RemoveUsage();
	}
	if( pShaderOccMapCube ){
		pShaderOccMapCube->RemoveUsage();
	}
	if( pShaderOccTracingGenRays ){
		pShaderOccTracingGenRays->RemoveUsage();
	}
	if( pShaderOccTracingTraceRays ){
		pShaderOccTracingTraceRays->RemoveUsage();
	}
	if( pShaderOccTracingDebug ){
		pShaderOccTracingDebug->RemoveUsage();
	}
	if( pShaderOccTracingUpdateOcclusion ){
		pShaderOccTracingUpdateOcclusion->RemoveUsage();
	}
	if( pShaderOccTracingUpdateDistance ){
		pShaderOccTracingUpdateDistance->RemoveUsage();
	}
	
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
	
	if( pVAOFrustumPlanes ){
		pglDeleteVertexArrays( 1, &pVAOFrustumPlanes );
	}
	if( pVBOFrustumPlanes ){
		pglDeleteBuffers( 1, &pVBOFrustumPlanes );
	}
}
