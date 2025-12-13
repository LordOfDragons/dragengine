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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderWorld.h"
#include "deoglRenderCompute.h"
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
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideListLight.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugInformation.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../light/deoglRLight.h"
#include "../light/volume/deoglLightVolume.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../extensions/deoglExtensions.h"
#include "../occlusiontest/deoglOcclusionMap.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../occquery/deoglOcclusionQuery.h"
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
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../sky/deoglRSkyLayer.h"
#include "../sky/deoglRSkyInstanceLayer.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/deoglImageStageManager.h"
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
	what, (int)(dtimer.GetElapsedTime() * 1000000.0)); dtimer.Reset()
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
	(int)(dtimer.GetElapsedTime() * 1000000.0)); dtimer.Reset()
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

enum eSPTestTFB{
	spttfInputDataCount,
	spttfbMatrix,
	spttfbScaleSize,
	spttfbBaseTopLevel,
	spttfbClipNear,
	spttfbMatrix2,
	spttfbScaleSize2,
	spttfbBaseTopLevel2,
	spttfbClipNear2,
	spttfbFrustumNormal1,
	spttfbFrustumNormal2,
	spttfbFrustumNormal3,
	spttfbFrustumNormal4,
	spttfbFrustumTestAdd,
	spttfbFrustumTestMul,
	spttfbMatrixStereo,
	spttfbMatrix2Stereo,
	spttfbCullFilter
};



// Class deoglRenderOcclusion
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderOcclusion::deoglRenderOcclusion(deoglRenderThread &renderThread) :
deoglRenderBase(renderThread),

pRenderTask(NULL),
pAddToRenderTask(NULL)
{
	const bool indirectMatrixAccess = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines, commonDefines, commonOccMapDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	int modifiers;
	
	try{
		renderThread.GetShader().SetCommonDefines(commonDefines);
		
		
		// occlusion map
		commonOccMapDefines = commonDefines;
		AddSharedSPBDefines(commonOccMapDefines);
		
		for(modifiers=0; modifiers<epmSingle<<1; modifiers++){
			pipconf.Reset();
			pipconf.SetMasks(false, false, false, false, true);
			pipconf.EnableDepthTestLessEqual();
			pipconf.SetSPBInstanceIndexBase(0);
			
			defines = commonOccMapDefines;
			
			if(modifiers & epmStereo){
				defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
				if(renderFSQuadStereoVSLayer){
					sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap");
					defines.SetDefines("VS_RENDER_LAYER");
					pipconf.SetDrawIDOffset(1);
					
				}else{
					sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap Stereo");
				}
				
			}else{
				sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap");
			}
			
			if(modifiers & epmClipPlane){
				defines.SetDefines("USE_CLIP_PLANE");
			}
			
			if(!(modifiers & epmOrtho)){
				defines.SetDefines("PERSPECTIVE_TO_LINEAR");
			}
			
			if(modifiers & epmSingle){
				pipconf.EnableCulling(false);
			}
			
			pAsyncGetPipeline(pPipelinesOccMap[modifiers], pipconf, sources, defines, true);
		}
		
		
		// occlusion queries
		for(modifiers=0; modifiers<epmOrtho<<1; modifiers++){
			pipconf.Reset();
			pipconf.DisableMasks();
			pipconf.EnableDepthTestLessEqual();
			pipconf.SetDepthClamp(true); // prevent wrong results
			// pipconf.EnableCulling( false ); // less errors if disabled
			
			defines = commonDefines;
			
			if(modifiers & epmStereo){
				defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
				if(renderFSQuadStereoVSLayer){
					sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap");
					defines.SetDefines("VS_RENDER_LAYER");
					pipconf.SetDrawIDOffset(1);
					
				}else{
					sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap Stereo");
				}
				
			}else{
				sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap");
			}
			
			if(modifiers & epmClipPlane){
				defines.SetDefines("USE_CLIP_PLANE");
			}
			
			if(!(modifiers & epmOrtho)){
				defines.SetDefines("PERSPECTIVE_TO_LINEAR");
			}
			
			pAsyncGetPipeline(pPipelinesOccQuery[modifiers], pipconf, sources, defines, true);
		}
		
		
		// occlusion map downsample
		pipconf.Reset();
		pipconf.SetMasks(false, false, false, false, true);
		pipconf.EnableDepthTestAlways();
		pipconf.SetClipControl(false);
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap Down-Sample");
		pAsyncGetPipeline(pPipelineOccMapDownSample, pipconf, sources, defines);
		
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if(!renderThread.GetChoices().GetRenderFSQuadStereoVSLayer()){
			sources = shaderManager.GetSourcesNamed("DefRen Occlusion OccMap Down-Sample Stereo");
		}
		pAsyncGetPipeline(pPipelineOccMapDownSampleStereo, pipconf, sources, defines);
		
		
		// occlusion test
		pipconf.Reset();
		pipconf.SetType(deoglPipelineConfiguration::etCompute);
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed("DefRen Occlusion Test");
		defines.SetDefines("ENSURE_MIN_SIZE");
		pAsyncGetPipeline(pPipelineOccTest, pipconf, sources, defines);
		
		defines = commonDefines;
		defines.SetDefines("ENSURE_MIN_SIZE", "WITH_COMPUTE_RENDER_TASK");
		pAsyncGetPipeline(pPipelineOccTestComputeRT, pipconf,
			"DefRen Occlusion Test Compute RT", defines);
		
		// occlusion test transform feedback dual
		defines = commonDefines;
		defines.SetDefines("ENSURE_MIN_SIZE", "DUAL_OCCMAP");
		pAsyncGetPipeline(pPipelineOccTestDual, pipconf, sources, defines);
		
		// occlusion test transform feedback sun
		defines = commonDefines;
		defines.SetDefines("ENSURE_MIN_SIZE", "FRUSTUM_TEST");
		pAsyncGetPipeline(pPipelineOccTestSun, pipconf, sources, defines);
		
		defines = commonDefines;
		defines.SetDefines("ENSURE_MIN_SIZE", "FRUSTUM_TEST", "WITH_COMPUTE_RENDER_TASK");
		pAsyncGetPipeline(pPipelineOccTestSunComputeRT, pipconf, sources, defines);
		
		
		
		
		deoglSPBlockUBO::Ref ubo(deoglSPBlockUBO::Ref::New(renderThread));
		ubo->SetRowMajor(indirectMatrixAccess);
		ubo->SetParameterCount(5);
		ubo->GetParameterAt(0).SetAll(deoglSPBParameter::evtFloat, 4, 4, 6); // mat4 pMatrixVP[6]
		ubo->GetParameterAt(1).SetAll(deoglSPBParameter::evtFloat, 4, 3, 6); // mat4x3 pMatrixV[6]
		ubo->GetParameterAt(2).SetAll(deoglSPBParameter::evtFloat, 4, 1, 6); // vec4 pTransformZ[6]
		ubo->GetParameterAt(3).SetAll(deoglSPBParameter::evtFloat, 2, 1, 1); // vec2 pZToDepth
		ubo->GetParameterAt(4).SetAll(deoglSPBParameter::evtFloat, 4, 1, 2); // vec4 pClipPlane[2]
		ubo->MapToStd140();
		ubo->SetBindingPoint(deoglSkinShader::eubRenderParameters);
		pRenderParamBlockSingleUse = deoglSPBSingleUse::Ref::New(renderThread, ubo);
		
		ubo = deoglSPBlockUBO::Ref::New(renderThread);
		ubo->SetParameterCount(1);
		ubo->GetParameterAt(0).SetAll(deoglSPBParameter::evtFloat, 4, 3, 1); // mat4x3 pMatrixModel
		ubo->MapToStd140();
		ubo->SetBindingPoint(deoglSkinShader::eubInstanceParameters);
		pOccMapFrustumParamBlockSingleUse = deoglSPBSingleUse::Ref::New(renderThread, ubo);
		
		pRenderTask = new deoglRenderTask(renderThread);
		pAddToRenderTask = new deoglAddToRenderTask(renderThread, *pRenderTask);
		
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
		const GLfloat pointsFrustumPlanes[48] = {
			-1.0f,  1.0f, 1.0f,    1.0f,  1.0f, -1.0f,    1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, 1.0f, // left frustum plane quad
			1.0f,  1.0f, 1.0f,    1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, 1.0f, // top frustum plane quad
			1.0f, -1.0f, 1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,    1.0f,  1.0f, 1.0f, // right frustum plane quad
			-1.0f, -1.0f, 1.0f,   -1.0f,  1.0f, -1.0f,    1.0f,  1.0f, -1.0f,    1.0f, -1.0f, 1.0f, // bottom frustum plane quad
		};
		
		OGL_CHECK(renderThread, pglGenVertexArrays(1, &pVAOFrustumPlanes));
		if(!pVAOFrustumPlanes){
			DETHROW(deeOutOfMemory);
		}
		OGL_CHECK(renderThread, pglBindVertexArray(pVAOFrustumPlanes));
		
		OGL_CHECK(renderThread, pglGenBuffers(1, &pVBOFrustumPlanes));
		if(!pVBOFrustumPlanes){
			DETHROW(deeOutOfMemory);
		}
		OGL_CHECK(renderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBOFrustumPlanes));
		OGL_CHECK(renderThread, pglBufferData(GL_ARRAY_BUFFER, sizeof(pointsFrustumPlanes), (const GLvoid *)&pointsFrustumPlanes, GL_STATIC_DRAW));
		
		OGL_CHECK(renderThread, pglEnableVertexAttribArray(0));
		OGL_CHECK(renderThread, pglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0));
		
		OGL_CHECK(renderThread, pglBindBuffer(GL_ARRAY_BUFFER, 0));
		OGL_CHECK(renderThread, pglBindVertexArray(0));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglRenderOcclusion::~deoglRenderOcclusion(){
	pCleanUp();
}



// Rendering
//////////////

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

void deoglRenderOcclusion::RenderTestsCamera(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
	const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Occlusion.RenderTestsCamera");
	deoglRenderPlanDebug * const renderPlanDebug = plan.GetDebug();
	
	decTimer timer;
	if(renderPlanDebug){
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
	
	const float zscale = 1.0f / (zfar - znear);
	const float zoffset = -(znear * zscale);
	
	// calculate a small offset to avoid problems of similar depth.
	// occmap resolution is between 1/(1 << 24) and 1/(1 << 32). with 24bit the resolution on
	// 300m view distance and 0.01m image distance this is 1.78e-5 (18ym). with 32bit the
	// resolution is 6.98e-8 (70nm). with a huge view distance of 30km and 0.01m image distance
	// this would be 1.78e-3 (2mm) for 24bit and 6.98e-6 (7ym) for 32bit. for typical view
	// distances the resolution is below micro-meter range. even for the huge view distance
	// the resolution is in the range of a few micro-meters (or 2mm for ancient systems).
	// this is very small and has no real influence on the result. still a small offset is
	// kept just in case
	const float occmapResolution = 1.0f / (float)((1 << 24) - 1);
	const float occmapBias = 1.0f; // bias by 1 or 2 depth values
	
	// update render parameter block
	const decDMatrix &matrixCamera = plan.GetRefPosCameraMatrix();
	
	decDMatrix matrixCameraStereo(matrixCamera);
	if(plan.GetRenderStereo()){
		matrixCameraStereo *= plan.GetCameraStereoMatrix();
	}
	
	// linear depth: use non-infinite projection matrix
	const decDMatrix &matrixProjection = plan.GetFrustumMatrix();
	pRenderParamBlock = (deoglSPBlockUBO*)pRenderParamBlockSingleUse->Next();
	{
		const deoglSPBMapBuffer mapped(pRenderParamBlock);
		// 0: pMatrixVP[ 0 ]
		// 1: pMatrixV[ 0 ]
		// 2: pTransformZ[ 0 ]
		pRenderParamBlock->SetParameterDataArrayMat4x4(0, 0, matrixCamera * matrixProjection);
		pRenderParamBlock->SetParameterDataArrayMat4x3(1, 0, matrixCamera);
		pRenderParamBlock->SetParameterDataArrayVec4(2, 0, (float)matrixCamera.a31 * zscale,
			(float)matrixCamera.a32 * zscale, (float)matrixCamera.a33 * zscale,
			(float)matrixCamera.a34 * zscale + zoffset + occmapResolution * occmapBias);
		pRenderParamBlock->SetParameterDataVec2(3, zscale, zoffset + occmapResolution * occmapBias);
		
		if(mask && mask->GetUseClipPlane()){
			pRenderParamBlock->SetParameterDataArrayVec4(4, 0, mask->GetClipNormal(), mask->GetClipDistance());
			
		}else{
			pRenderParamBlock->SetParameterDataArrayVec4(4, 0, 0.0f, 0.0f, 0.0f, 0.0f); // pClipPlane
		}
		
		if(plan.GetRenderStereo()){
			pRenderParamBlock->SetParameterDataArrayMat4x4(0, 1, matrixCameraStereo * matrixProjection);
			pRenderParamBlock->SetParameterDataArrayMat4x3(1, 1, matrixCameraStereo);
			pRenderParamBlock->SetParameterDataArrayVec4(2, 1, (float)matrixCameraStereo.a31 * zscale,
				(float)matrixCameraStereo.a32 * zscale, (float)matrixCameraStereo.a33 * zscale,
				(float)matrixCameraStereo.a34 * zscale + zoffset + occmapResolution * occmapBias);
			
			if(mask && mask->GetUseClipPlane()){
				pRenderParamBlock->SetParameterDataArrayVec4(4, 1, mask->GetClipNormalStereo(), mask->GetClipDistanceStereo());
				
			}else{
				pRenderParamBlock->SetParameterDataArrayVec4(4, 1, 0.0f, 0.0f, 0.0f, 0.0f); // pClipPlane
			}
		}
	}
	DEBUG_PRINT_TIMER("Update Param Block");
	
	// render occlusion map
	if(renderPlanDebug){
		renderPlanDebug->IncrementOccMapObjects(plan.GetCollideList().GetComponentCount());
		RenderOcclusionMap(plan, mask);
		renderPlanDebug->IncrementOccMapTriangles(pRenderTask->GetTotalPointCount() / 3);
		renderPlanDebug->SetOccMapRenderTime(timer.GetElapsedTime());
		
	}else{
		RenderOcclusionMap(plan, mask);
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
	
	decDMatrix testMatrix, testMatrixStereo;
	
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		// for compute render task the offset has to be kept since the world element data is
		// relative to the world reference position not the camera position
		
		testMatrix = matrixCamera * matrixProjection;
		testMatrix.a34 = matrixCamera.a34 * zscale + zoffset; // no bias while sampling
		
		testMatrixStereo = matrixCameraStereo * matrixProjection;
		testMatrixStereo.a34 = matrixCameraStereo.a34 * zscale + zoffset; // no bias while sampling
		
	}else{
		testMatrix = matrixCamera.GetRotationMatrix() * matrixProjection;
		testMatrix.a34 = zoffset; // no bias while sampling
		
		testMatrixStereo = matrixCameraStereo.GetRotationMatrix() * matrixProjection;
		testMatrixStereo.a34 = zoffset; // no bias while sampling
	}
	
	testMatrix.a31 = matrixCamera.a31 * zscale;
	testMatrix.a32 = matrixCamera.a32 * zscale;
	testMatrix.a33 = matrixCamera.a33 * zscale;
	
	testMatrixStereo.a31 = matrixCameraStereo.a31 * zscale;
	testMatrixStereo.a32 = matrixCameraStereo.a32 * zscale;
	testMatrixStereo.a33 = matrixCameraStereo.a33 * zscale;
	
	plan.SetOcclusionTestMatrix(testMatrix);
	plan.SetOcclusionTestMatrixStereo(testMatrixStereo);
	
	RenderOcclusionTests(plan, *plan.GetOcclusionTest(), *plan.GetOcclusionMap(),
		plan.GetOcclusionMapBaseLevel(), -1.0f, testMatrix, testMatrixStereo);
	if(renderPlanDebug){
		renderPlanDebug->SetOccTestTime(timer.GetElapsedTime());
	}
}


void deoglRenderOcclusion::RenderTestsSkyLayer(deoglRenderPlanSkyLight &planSkyLight){
	DEBUG_RESET_TIMERS;
	// calculate the camera matrix fitting around all splits. the camera matrix transform
	// the points into the range [-1..1] for all axes
	const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Occlusion.RenderTestsSkyLayer");
	const deoglRenderPlan &plan = planSkyLight.GetPlan();
	const decDVector &referencePosition = plan.GetWorld()->GetReferencePosition();
	const decVector &minExtend = planSkyLight.GetFrustumBoxMinExtend();
	const decVector &maxExtend = planSkyLight.GetFrustumBoxMaxExtend();
	const decDVector &camPos = plan.GetCameraPosition();
	
	const decDMatrix matLig(decDMatrix::CreateRotation(0.0, PI, 0.0)
		* decDMatrix( planSkyLight.GetLayer()->GetMatrix() ) );
	
	const decDVector boxCenter((minExtend + maxExtend) * 0.5f);
	const decDVector boxSize(maxExtend - minExtend);
	
	const decDVector position(camPos - referencePosition + matLig * boxCenter);
	const decDVector scale(2.0 / boxSize.x, 2.0 / boxSize.y, 2.0 / boxSize.z);
	
	// test input valus use world reference position as reference position instead of
	// camera position as the camera tests do
	
	// the test shader expects z coordinate to be transformed into the range [0..1]
	// and x/y/z in the range [-w..w]. this requires adding a scale of z=0.5 and a
	// translation of z=0.5
	const decDMatrix matrixCamera(
		decDMatrix::CreateCamera(position, matLig.TransformView(), matLig.TransformUp())
		* decDMatrix::CreateScale( scale ) * decDMatrix::CreateScale( 1.0, 1.0, 0.5 )
		* decDMatrix::CreateTranslation( 0.0, 0.0, 0.5 ) );
	
	// the result of the first occlusion test has to be transformed back into occlusion
	// map space. this is done by first reverting the matrixCamera which transforms back
	// into world space relative to world reference position. then the position has to
	// be transformed into world space relative to camera position. now the position is
	// in the same space as the camera tests used. then the occlusion matrix is applied
	// the same way as camera tests did
	decMatrix matrixLightToCamera(matrixCamera.Invert());
	
	if(!pRenderThread.GetChoices().GetUseComputeRenderTask()){
		// with compute render task test matrix includes camera position otherwise not
		matrixLightToCamera *= decDMatrix::CreateTranslation(referencePosition - camPos);
	}
	
	const decMatrix matrixCamera2(matrixLightToCamera * plan.GetOcclusionTestMatrix());
	const decMatrix matrixCamera2Stereo(matrixLightToCamera * plan.GetOcclusionTestMatrixStereo());
	DEBUG_PRINT_TIMER("Prepare");
	
	RenderOcclusionTestsSun(planSkyLight, *planSkyLight.GetOcclusionTest(), *plan.GetOcclusionMap(),
		plan.GetOcclusionMapBaseLevel(), -100.0f, matrixCamera, -1.0f, matrixCamera2, matrixCamera2Stereo);
	DEBUG_PRINT_TIMER("Render");
}


const deoglPipeline *deoglRenderOcclusion::GetRenderOcclusionMapRTS(const deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool perspective, bool singleSided) const{
	int modifiers = 0;
	
	if(!perspective){
		modifiers |= epmOrtho;
	}
	if(mask && mask->GetUseClipPlane()){
		modifiers |= epmClipPlane;
	}
	if(plan.GetRenderStereo()){
		modifiers |= epmStereo;
	}
	if(singleSided){
		modifiers |= epmSingle;
	}
	
	return pPipelinesOccMap[modifiers];
}

void deoglRenderOcclusion::RenderOcclusionMap(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
	if(GetRenderThread().GetChoices().GetUseComputeRenderTask()){
		RenderOcclusionMap(plan, nullptr, plan.GetCompute()->GetRTOcclusion());
		return;
	}
	
	pRenderTask->Clear();
	
	pAddToRenderTask->Reset();
	pAddToRenderTask->SetSolid(true);
	pAddToRenderTask->SetNoRendered(plan.GetNoRenderedOccMesh());
	
	pAddToRenderTask->AddOcclusionMeshes(plan.GetComponentsOccMap(),
		GetRenderOcclusionMapRTS(plan, mask, true, true),
		GetRenderOcclusionMapRTS(plan, mask, true, false));
	DEBUG_PRINT_TIMER("RenderOcclusionMap Build RT");
	
	RenderOcclusionMap(plan, pRenderTask, nullptr);
}

void deoglRenderOcclusion::RenderOcclusionMap(deoglRenderPlan &plan,
deoglRenderTask *renderTask, deoglComputeRenderTask *computeRenderTask){
	deoglOcclusionMap &occmap = *plan.GetOcclusionMap();
	const int baselevel = plan.GetOcclusionMapBaseLevel();
	const decMatrix *renderFrustumPlanesMatrix = NULL;
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Occlusion.RenderOcclusionMap");
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	const int levelCount = occmap.GetLevelCount();
	int i, width, height;
	
	// prepare fbo
	pPipelineClearBuffers->Activate();
	renderThread.GetFramebuffer().Activate(occmap.GetFBOAt(0));
	
	width = occmap.GetWidth();
	height = occmap.GetHeight();
	for(i=1; i<baselevel; i++){
		width >>= 1;
		height >>= 1;
	}
	
	SetViewport(width, height);
	
	// clear the occlusion map
	const GLfloat clearDepth = (GLfloat)1.0f; // occlusion uses linear depth
	OGL_CHECK(renderThread, pglClearBufferfv(GL_DEPTH, 0, &clearDepth));
	DEBUG_PRINT_TIMER("RenderOcclusionMap Clear");
	
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
	if(renderFrustumPlanesMatrix){// && false){
		pOccMapFrustumParamBlock = (deoglSPBlockUBO*)pOccMapFrustumParamBlockSingleUse->Next();
		{
			const deoglSPBMapBuffer mapped(pOccMapFrustumParamBlock);
			// 0: pMatrixModel
			pOccMapFrustumParamBlock->SetParameterDataMat4x3(0, *renderFrustumPlanesMatrix);
		}
		
		pRenderParamBlock->Activate();
		pOccMapFrustumParamBlock->Activate();
		pglBindVertexArray(pVAOFrustumPlanes);
		
		if(plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer()){
			const GLint first[] = {0, 0, 4, 4, 8, 8, 12, 12};
			const GLsizei count[] = {4, 4, 4, 4, 4, 4, 4, 4};
			OGL_CHECK(renderThread, pglMultiDrawArrays(GL_TRIANGLE_FAN, first, count, 2)); // left
			OGL_CHECK(renderThread, pglMultiDrawArrays(GL_TRIANGLE_FAN, first + 2, count + 2, 2)); // top
			OGL_CHECK(renderThread, pglMultiDrawArrays(GL_TRIANGLE_FAN, first + 4, count + 4, 2)); // right
			OGL_CHECK(renderThread, pglMultiDrawArrays(GL_TRIANGLE_FAN, first + 6, count + 6, 2)); // bottom
			
		}else{
			OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLE_FAN, 0, 4)); // left
			OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLE_FAN, 4, 4)); // top
			OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLE_FAN, 8, 4)); // right
			OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLE_FAN, 12, 4)); // bottom
			//OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, 36 ) );
		}
		
		pglBindVertexArray(0);
		DEBUG_PRINT_TIMER("RenderOcclusionMap Render Frustum Planes");
	}
	
	// render occlusion map
	if(computeRenderTask){
		if(computeRenderTask->GetStepCount() > 0){
			computeRenderTask->SetRenderParamBlock(pRenderParamBlock);
			computeRenderTask->SetRenderVSStereo(plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer());
			computeRenderTask->Render();
		}
		
	}else{
		renderTask->SetRenderParamBlock(pRenderParamBlock);
		renderTask->SetRenderVSStereo(plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer());
		renderTask->PrepareForRender();
		rengeom.RenderTask(*renderTask);
	}
	DEBUG_PRINT_TIMER("RenderOcclusionMap Render");
	
	// create mipmap level (z-pyramid)
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineOccMapDownSampleStereo : *pPipelineOccMapDownSample;
	pipeline.Activate();
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	OGL_CHECK(renderThread, pglBindVertexArray(defren.GetVAOFullScreenQuad()->GetVAO()));
	
	// not GetSamplerClampNearestMipMap() or AMD windows drivers dies
	tsmgr.EnableArrayTexture(0, *occmap.GetTexture(), GetSamplerClampNearest());
	
	for(i=baselevel+1; i<levelCount; i++){
		width >>= 1;
		height >>= 1;
		
		renderThread.GetFramebuffer().Activate(occmap.GetFBOAt(i));
		OGL_CHECK(renderThread, glViewport(0, 0, width, height));
		
		shader.SetParameterInt(spomdsLevel, i - 1);
		
		RenderFullScreenQuad(plan);
	}
	
	OGL_CHECK(renderThread, pglBindVertexArray(0));
	DEBUG_PRINT_TIMER("RenderOcclusionMap Downsample");
}



void deoglRenderOcclusion::RenderOcclusionQueries(deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool perspective){
	deoglOcclusionMap &occmap = *plan.GetOcclusionMap();
	const int baselevel = plan.GetOcclusionMapBaseLevel();
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Occlusion.RenderOcclusionQueries");
	int i, width, height;
	
	// prepare fbo
	renderThread.GetFramebuffer().Activate(occmap.GetFBOAt(0));
	
	width = occmap.GetWidth();
	height = occmap.GetHeight();
	for(i=1; i<baselevel; i++){
		width >>= 1;
		height >>= 1;
	}
	
	SetViewport(width, height);
	
	// activate pipeline
	int modifiers = 0;
	
	if(!perspective){
		modifiers |= epmOrtho;
	}
	if(mask && mask->GetUseClipPlane()){
		modifiers |= epmClipPlane;
	}
	if(plan.GetRenderStereo()){
		modifiers |= epmStereo;
	}
	
	pPipelinesOccQuery[modifiers]->Activate();
	
	// update occlusion parameter blocks. this has to come first since unmapping
	// parameter blocks needs to temporarily bind them. this process can potentially
	// fumble up binding points so we have to make sure all bindings required for
	// rendering are done after all parameter blocks have been updated
	const decDVector &referencePosition = plan.GetWorld()->GetReferencePosition();
	const deoglCollideList &clist = plan.GetCollideList();
	const int lightCount = clist.GetLightCount();
	
	for(i=0; i<lightCount; i++){
		const deoglCollideListLight &cllight = *clist.GetLightAt(i);
		if(cllight.GetCameraInside()){
			continue;
		}
		
		deoglRLight &light = *cllight.GetLight();
		
		deoglSPBlockUBO &spbOccQuery = light.GetOccQueryParameterBlock();
		const deoglSPBMapBuffer mapped(spbOccQuery);
		
		decDMatrix matrix(light.GetMatrix());
		matrix.a14 -= referencePosition.x;
		matrix.a24 -= referencePosition.y;
		matrix.a34 -= referencePosition.z;
		
		spbOccQuery.SetParameterDataMat4x3(0, matrix);
	}
	
	// render occlusion queries. now it is safe to bind parameter blocks
	const bool renderVSStereo = plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer();
	
	pRenderParamBlock->Activate();
	
	for(i=0; i<lightCount; i++){
		deoglCollideListLight &cllight = *clist.GetLightAt(i);
		cllight.SetOcclusionQueryValid(false);
		
		if(cllight.GetCulled() || cllight.GetCameraInside() || cllight.GetCameraInsideOccQueryBox()){
			continue;
		}
		
		deoglRLight &light = *cllight.GetLight();
		
		light.GetOccQueryParameterBlock()->Activate();
		
		const deoglLightVolume &lvolume = *light.GetLightVolume();
		pglBindVertexArray(lvolume.GetVAO());
		
		deoglOcclusionQuery &query = cllight.GetOcclusionQuery();
		query.BeginQuery(deoglOcclusionQuery::eqtAny);
		
		if(renderVSStereo){
			const GLint first[2] = {0, 0};
			const GLsizei count[2] = {lvolume.GetPointCount(), lvolume.GetPointCount()};
			OGL_CHECK(renderThread, pglMultiDrawArrays(GL_TRIANGLES, first, count, 2));
			
		}else{
			OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLES, 0, lvolume.GetPointCount()));
		}
		
		query.EndQuery();
		cllight.SetOcclusionQueryValid(true);
	}
	pglBindVertexArray(0);
	DEBUG_PRINT_TIMER("RenderOcclusionQueries Render");
}



void deoglRenderOcclusion::RenderOcclusionTests(deoglRenderPlan &plan,
deoglOcclusionTest &occlusionTest, deoglOcclusionMap &occlusionMap, int baselevel,
float clipNear, const decMatrix &matrixCamera, const decMatrix &matrixCameraStereo){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	if(renderThread.GetChoices().GetUseComputeRenderTask()){
		const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Occlusion.RenderOcclusionTests");
		deoglRenderCompute &renderCompute = renderThread.GetRenderers().GetCompute();
		const deoglWorldCompute &worldCompute = plan.GetWorld()->GetCompute();
		deoglRenderPlanCompute &planCompute = plan.GetCompute();
		
		int cullFilter = ~0;
		if(plan.GetUpdateGIState()){
			// we can not cull lights outside frustum if inside GI cascade detection box.
			// light renderers will take care of not rendering lights if light is culled
			cullFilter &= ~deoglWorldCompute::eefLight;
		}
		
		renderCompute.GetSSBOCounters()->CopyData(planCompute.GetSSBOCounters(),
			deoglRenderCompute::ecTempCounter, 1, 0);
		planCompute.GetSSBOCounters()->ClearDataUInt(0, 1, 0, 1, 1, 0);
		
		pPipelineOccTestComputeRT->Activate();
		deoglShaderCompiled &shader = pPipelineOccTestComputeRT->GetShader();
		
		shader.SetParameterMatrix4x4(spttfbMatrix, matrixCamera);
		shader.SetParameterFloat(spttfbScaleSize,
			(float)occlusionMap.GetWidth(), (float)occlusionMap.GetHeight());
		shader.SetParameterFloat(spttfbBaseTopLevel,
			(float)baselevel, (float)(occlusionMap.GetLevelCount() - 1));
		shader.SetParameterFloat(spttfbClipNear, clipNear);
		if(plan.GetRenderStereo()){
			shader.SetParameterMatrix4x4(spttfbMatrixStereo, matrixCameraStereo);
		}
		shader.SetParameterUInt(spttfbCullFilter, cullFilter);
		
		ClearRenderDocDebugTexture();
		tsmgr.EnableArrayTexture(0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap());
		
		worldCompute.GetSSBOElements()->Activate(0);
		planCompute.GetSSBOVisibleElements()->Activate(1);
		renderCompute.GetSSBOCounters()->Activate(2);
		planCompute.GetSSBOVisibleElements2()->Activate(3);
		planCompute.GetSSBOCounters()->ActivateAtomic(0);
		renderThread.GetTexture().GetImageStages().EnableRenderDocDebug(4, deoglRTChoices::RenderDocDebugFlagOccTest);
		
		renderCompute.GetSSBOCounters()->ActivateDispatchIndirect();
		OGL_CHECK(renderThread, pglDispatchComputeIndirect(
			renderCompute.CounterDispatchOffset(deoglRenderCompute::ecTempCounter)));
		OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
			| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
		renderCompute.GetSSBOCounters()->DeactivateDispatchIndirect();
		
		planCompute.GetSSBOVisibleElements2()->GPUFinishedWriting();
		planCompute.GetSSBOCounters()->GPUFinishedWriting();
		planCompute.GetSSBOCounters()->GPUReadToCPU(1);
		
		planCompute.SwapVisibleElements();
		
	}else{
		if(occlusionTest.GetInputDataCount() == 0){
			return;
		}
		
		const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Occlusion.RenderOcclusionTests");
		const int inputDataCount = occlusionTest.GetInputDataCount();
		
		pPipelineOccTest->Activate();
		deoglShaderCompiled &shader = pPipelineOccTest->GetShader();
		
		shader.SetParameterUInt(spttfInputDataCount, inputDataCount);
		shader.SetParameterMatrix4x4(spttfbMatrix, matrixCamera);
		shader.SetParameterFloat(spttfbScaleSize,
			(float)occlusionMap.GetWidth(), (float)occlusionMap.GetHeight());
		shader.SetParameterFloat(spttfbBaseTopLevel,
			(float)baselevel, (float)(occlusionMap.GetLevelCount() - 1));
		shader.SetParameterFloat(spttfbClipNear, clipNear);
		if(plan.GetRenderStereo()){
			shader.SetParameterMatrix4x4(spttfbMatrixStereo, matrixCameraStereo);
		}
		
		tsmgr.EnableArrayTexture(0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap());
		
		occlusionTest.GetSSBOInput()->Activate();
		occlusionTest.GetSSBOResult()->Activate();
		
		OGL_CHECK(renderThread, pglDispatchCompute((inputDataCount - 1) / 64 + 1, 1, 1));
		
		occlusionTest.GetSSBOResult()->GPUFinishedWriting();
		occlusionTest.GetSSBOResult()->GPUReadToCPU((inputDataCount - 1) / 4 + 1);
	}
	DEBUG_PRINT_TIMER("RenderOcclusionTests Compute");
}

void deoglRenderOcclusion::RenderOcclusionTestsSun(deoglRenderPlanSkyLight &planSkyLight,
deoglOcclusionTest &occlusionTest, deoglOcclusionMap &occlusionMap, int baselevel, float clipNear,
const decMatrix &matrixCamera, float clipNear2, const decMatrix &matrixCamera2,
const decMatrix &matrixCamera2Stereo){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	decVector frustumPlaneNormal[4]; // left, top, right, bottom
	float frustumFactorAdd[4]; // left, top, right, bottom
	float frustumFactorMul[4]; // left, top, right, bottom
	
	// calculate frustum from plan if required.
	// TODO it is also possible to cull the other end of the shadow shaft. this could potentially help in cases
	//      where the sun is behind the player and the object is between the frustum and the sun. this culling
	//      would have to be done with the other planes (those facing away from the sun).
	const deoglRenderPlan &plan = planSkyLight.GetPlan();
	const float fov = plan.GetCameraFov();
	const float fovRatio = plan.GetCameraFovRatio();
	const float zfar = plan.GetCameraViewDistance();
	const float xfar = tanf(fov * 0.5f) * zfar;
	const float yfar = tanf(fov * 0.5f * fovRatio) * zfar;
	
	const decMatrix frustumMatrix = plan.GetRefPosCameraMatrix().Invert() * matrixCamera;
	decVector fpts[5];
	float denom;
	int i;
	
	fpts[0] = frustumMatrix.GetPosition();
	frustumMatrix.Transform(fpts[1], -xfar,  yfar, zfar); // top-left
	frustumMatrix.Transform(fpts[2],  xfar,  yfar, zfar); // top-right
	frustumMatrix.Transform(fpts[3],  xfar, -yfar, zfar); // bottom-right
	frustumMatrix.Transform(fpts[4], -xfar, -yfar, zfar); // bottom-left
	
	for(i=0; i<4; i++){
		frustumPlaneNormal[i] = ((fpts[1 + i] - fpts[0])
			% (fpts[1 + ((3 + i) % 4)] - fpts[0])).Normalized();
		
		if(plan.GetFlipCulling()){
			// mirroring causes normals to point outside instead of inside
			frustumPlaneNormal[i] = -frustumPlaneNormal[i];
		}
		
		if(frustumPlaneNormal[i].z < 0.0f){
			denom = 1.0f / frustumPlaneNormal[i].z; // 1/(sunDir*planeNormal) = 1/((0,0,01)*planeNormal) = 1/planeNormal.z
			frustumFactorAdd[i] = (fpts[0] * frustumPlaneNormal[i]) * denom;
			frustumFactorMul[i] = -denom;
			
		}else{
			frustumPlaneNormal[i].SetZero();
			frustumFactorMul[i] = 0.0f;
			frustumFactorAdd[i] = 1.0f;
		}
	}
	
	DEBUG_PRINT_TIMER("RenderOcclusionTestsSun Entering");
	
	if(renderThread.GetChoices().GetUseComputeRenderTask()){
		const deoglDebugTraceGroup debugTrace(renderThread, "World.RenderOcclusionTestsSun");
		deoglRenderCompute &renderCompute = renderThread.GetRenderers().GetCompute();
		const deoglWorldCompute &worldCompute = plan.GetWorld()->GetCompute();
		
		renderCompute.GetSSBOCounters()->CopyData(planSkyLight.GetSSBOCounters(),
			deoglRenderCompute::ecTempCounter, 1, 0);
		planSkyLight.GetSSBOCounters()->ClearDataUInt(0, 1, 0, 1, 1, 0);
		
		pPipelineOccTestSunComputeRT->Activate();
		deoglShaderCompiled &shader = pPipelineOccTestSunComputeRT->GetShader();
		
		shader.SetParameterMatrix4x4(spttfbMatrix, matrixCamera);
		shader.SetParameterFloat(spttfbScaleSize,
			(float)occlusionMap.GetWidth(), (float)occlusionMap.GetHeight());
		shader.SetParameterFloat(spttfbBaseTopLevel,
			(float)baselevel, (float)(occlusionMap.GetLevelCount() - 1));
		shader.SetParameterFloat(spttfbClipNear, clipNear);
		shader.SetParameterMatrix4x4(spttfbMatrix2, matrixCamera2);
		shader.SetParameterFloat(spttfbClipNear2, clipNear2);
		shader.SetParameterVector3(spttfbFrustumNormal1, frustumPlaneNormal[0]);
		shader.SetParameterVector3(spttfbFrustumNormal2, frustumPlaneNormal[1]);
		shader.SetParameterVector3(spttfbFrustumNormal3, frustumPlaneNormal[2]);
		shader.SetParameterVector3(spttfbFrustumNormal4, frustumPlaneNormal[3]);
		shader.SetParameterFloat(spttfbFrustumTestAdd, frustumFactorAdd[0],
			frustumFactorAdd[1], frustumFactorAdd[2], frustumFactorAdd[3]);
		shader.SetParameterFloat(spttfbFrustumTestMul, frustumFactorMul[0],
			frustumFactorMul[1], frustumFactorMul[2], frustumFactorMul[3]);
		if(plan.GetRenderStereo()){
			shader.SetParameterMatrix4x4(spttfbMatrix2Stereo, matrixCamera2Stereo);
		}
		shader.SetParameterUInt(spttfbCullFilter, ~0);
		
		tsmgr.EnableArrayTexture(0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap());
		
		worldCompute.GetSSBOElements()->Activate(0);
		planSkyLight.GetSSBOVisibleElements()->Activate(1);
		renderCompute.GetSSBOCounters()->Activate(2);
		planSkyLight.GetSSBOVisibleElements2()->Activate(3);
		planSkyLight.GetSSBOCounters()->ActivateAtomic(0);
		
		renderCompute.GetSSBOCounters()->ActivateDispatchIndirect();
		OGL_CHECK(renderThread, pglDispatchComputeIndirect(
			renderCompute.CounterDispatchOffset(deoglRenderCompute::ecTempCounter)));
		OGL_CHECK(renderThread, pglMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT
			| GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT));
		renderCompute.GetSSBOCounters()->DeactivateDispatchIndirect();
		
		planSkyLight.SwapVisibleElements();
		
	}else{
		if(occlusionTest.GetInputDataCount() == 0){
			return;
		}
		
		const deoglDebugTraceGroup debugTrace(renderThread, "World.RenderOcclusionTestsSun");
		const int inputDataCount = occlusionTest.GetInputDataCount();
		
		pPipelineOccTestSun->Activate();
		deoglShaderCompiled &shader = pPipelineOccTestSun->GetShader();
		DEBUG_PRINT_TIMER("Activate FBO");
		
		shader.SetParameterUInt(spttfInputDataCount, inputDataCount);
		shader.SetParameterMatrix4x4(spttfbMatrix, matrixCamera);
		shader.SetParameterFloat(spttfbScaleSize,
			(float)occlusionMap.GetWidth(), (float)occlusionMap.GetHeight());
		shader.SetParameterFloat(spttfbBaseTopLevel,
			(float)baselevel, (float)(occlusionMap.GetLevelCount() - 1));
		shader.SetParameterFloat(spttfbClipNear, clipNear);
		shader.SetParameterMatrix4x4(spttfbMatrix2, matrixCamera2);
		shader.SetParameterFloat(spttfbClipNear2, clipNear2);
		shader.SetParameterVector3(spttfbFrustumNormal1, frustumPlaneNormal[0]);
		shader.SetParameterVector3(spttfbFrustumNormal2, frustumPlaneNormal[1]);
		shader.SetParameterVector3(spttfbFrustumNormal3, frustumPlaneNormal[2]);
		shader.SetParameterVector3(spttfbFrustumNormal4, frustumPlaneNormal[3]);
		shader.SetParameterFloat(spttfbFrustumTestAdd, frustumFactorAdd[0],
			frustumFactorAdd[1], frustumFactorAdd[2], frustumFactorAdd[3]);
		shader.SetParameterFloat(spttfbFrustumTestMul, frustumFactorMul[0],
			frustumFactorMul[1], frustumFactorMul[2], frustumFactorMul[3]);
		if(plan.GetRenderStereo()){
			shader.SetParameterMatrix4x4(spttfbMatrix2Stereo, matrixCamera2Stereo);
		}
		DEBUG_PRINT_TIMER("Set Uniforms");
		
		tsmgr.EnableArrayTexture(0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap());
		DEBUG_PRINT_TIMER("Set Texture");
		
		occlusionTest.GetSSBOInput()->Activate();
		occlusionTest.GetSSBOResult()->Activate();
		
		OGL_CHECK(renderThread, pglDispatchCompute((inputDataCount - 1) / 64 + 1, 1, 1));
		
		occlusionTest.GetSSBOResult()->GPUFinishedWriting();
		occlusionTest.GetSSBOResult()->GPUReadToCPU((inputDataCount - 1) / 4 + 1);
	}
	DEBUG_PRINT_TIMER("Compute");
}

void deoglRenderOcclusion::RenderOcclusionTestsDual(deoglRenderPlan &plan, deoglOcclusionTest &occlusionTest,
deoglOcclusionMap &occlusionMap, int baselevel, float clipNear, const decMatrix &matrixCamera,
deoglOcclusionMap &occlusionMap2, int baselevel2, float clipNear2, const decMatrix& matrixCamera2){
	if(occlusionTest.GetInputDataCount() == 0){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(GetRenderThread(), "Occlusion.RenderOcclusionTestsDual");
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const int inputDataCount = occlusionTest.GetInputDataCount();
	
	DEBUG_PRINT_TIMER("Entering Render Occlusion Tests");
	
	pPipelineOccTestDual->Activate();
	deoglShaderCompiled &shader = pPipelineOccTestDual->GetShader();
	DEBUG_PRINT_TIMER("Activate FBO");
	
	shader.SetParameterUInt(spttfInputDataCount, inputDataCount);
	shader.SetParameterMatrix4x4(spttfbMatrix, matrixCamera);
	shader.SetParameterFloat(spttfbScaleSize, (float)occlusionMap.GetWidth(), (float)occlusionMap.GetHeight());
	shader.SetParameterFloat(spttfbBaseTopLevel,
		(float)baselevel, (float)(occlusionMap.GetLevelCount() - 1));
	shader.SetParameterFloat(spttfbClipNear, clipNear);
	shader.SetParameterMatrix4x4(spttfbMatrix2, matrixCamera2);
	shader.SetParameterFloat(spttfbScaleSize2, (float)occlusionMap2.GetWidth(), (float)occlusionMap2.GetHeight());
	shader.SetParameterFloat(spttfbBaseTopLevel2,
		(float)baselevel2, (float)(occlusionMap2.GetLevelCount() - 1));
	shader.SetParameterFloat(spttfbClipNear2, clipNear2);
	DEBUG_PRINT_TIMER("Set Uniforms");
	
	tsmgr.EnableArrayTexture(0, *occlusionMap.GetTexture(), GetSamplerClampNearestMipMap());
	tsmgr.EnableArrayTexture(1, *occlusionMap2.GetTexture(), GetSamplerClampNearestMipMap());
	DEBUG_PRINT_TIMER("Set Texture");
	
	occlusionTest.GetSSBOInput()->Activate();
	occlusionTest.GetSSBOResult()->Activate();
	
	OGL_CHECK(renderThread, pglDispatchCompute((inputDataCount - 1) / 64 + 1, 1, 1));
	
	occlusionTest.GetSSBOResult()->GPUFinishedWriting();
	occlusionTest.GetSSBOResult()->GPUReadToCPU((inputDataCount - 1) / 4 + 1);
	DEBUG_PRINT_TIMER("Compute");
	
	// read back results
	occlusionTest.UpdateResults();
	DEBUG_PRINT_TIMER("Update Results");
}



void deoglRenderOcclusion::DebugOcclusionMap(deoglRenderPlan &plan){
	if(!plan.GetDebug()){
		return;
	}
	
	const deoglCollideList &clist = plan.GetCollideList();
	const int componentCount = clist.GetComponentCount();
	deoglRenderPlanDebug &debug = *plan.GetDebug();
	int i;
	
	debug.IncrementOccMapObjects(componentCount);
	
	for(i=0; i<componentCount; i++){
		const deoglCollideListComponent &clistComponent = *clist.GetComponentAt(i);
		const deoglRComponent &component = *clistComponent.GetComponent();
		
		if(!component.GetModel()){
			continue;
		}
		
		const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt(clistComponent.GetLODLevel());
		debug.IncrementOccMapTriangles(modelLOD.GetFaceCount());
	}
}



// Private Functions
//////////////////////

void deoglRenderOcclusion::pCleanUp(){
	if(pAddToRenderTask){
		delete pAddToRenderTask;
	}
	if(pRenderTask){
		delete pRenderTask;
	}
	
	deoglDelayedOperations &dops = GetRenderThread().GetDelayedOperations();
	dops.DeleteOpenGLVertexArray(pVAOFrustumPlanes);
	dops.DeleteOpenGLBuffer(pVBOFrustumPlanes);
}
