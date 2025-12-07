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

#include "deoglRenderGeometry.h"
#include "deoglRenderTranspCounting.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideList.h"
#include "../component/deoglRComponent.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/debugSnapshot.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../occquery/deoglOcclusionQueryManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../texture/deoglTextureStageManager.h"
#include "../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPTraCountMaxCount{
	sptcmcClampTC,
	sptcmcOffsets1,
	sptcmcOffsets2,
	sptcmcOffsets3,
	sptcmcOffsets4
};



// #define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer dtimer;

#define DEBUG_RESET_TIMER \
	dtimer.Reset();
#define DEBUG_PRINT_TIMER(what) \
	renderThread.GetLogger().LogInfoFormat("World %s = %iys", what, \
		(int)(dtimer.GetElapsedTime() * 1000000.0));\
	dtimer.Reset();
#else
#define DEBUG_RESET_TIMER
#define DEBUG_PRINT_TIMER(what)
#endif



// Class deoglRenderTranspCounting
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTranspCounting::deoglRenderTranspCounting(deoglRenderThread &renderThread) :
deoglRenderBase(renderThread),
pOccQuery(NULL),
pHasCount(false),
pCount(0)
{
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	renderThread.GetShader().SetCommonDefines(commonDefines);
	
	
	// count max count
	pipconf.Reset();
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen Transparency Max Count");
	defines.SetDefines("NO_POSTRANSFORM", "NO_TCTRANSFORM");
	pAsyncGetPipeline(pPipelineTraCountMaxCount, pipconf, sources, defines);
	
	// count max count stereo
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("DefRen Transparency Max Count Stereo");
	}
	pAsyncGetPipeline(pPipelineTraCountMaxCountStereo, pipconf, sources, defines);
	
	
	// count get count
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen Transparency Get Count");
	defines.SetDefines("NO_POSTRANSFORM", "NO_TEXCOORD");
	pAsyncGetPipeline(pPipelineTraCountGetCount, pipconf, sources, defines);
	
	// count get count stereo
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("DefRen Transparency Get Count Stereo");
	}
	pAsyncGetPipeline(pPipelineTraCountGetCountStereo, pipconf, sources, defines);
	
	
	pOccQuery = new deoglOcclusionQuery(renderThread);
}

deoglRenderTranspCounting::~deoglRenderTranspCounting(){
	pCleanUp();
}



// Rendering
//////////////

#define DO_QUICK_DEBUG 1

#ifdef DO_QUICK_DEBUG
#define QUICK_DEBUG_START(lower, upper) \
	if(renderThread.GetConfiguration().GetQuickDebug() > upper \
	|| renderThread.GetConfiguration().GetQuickDebug() < lower){
#define QUICK_DEBUG_END}
#else
#define QUICK_DEBUG_START(lower, upper)
#define QUICK_DEBUG_END
#endif



//#define ENABLE_DEBUG_ENTER_EXIT 1
#ifdef OS_ANDROID
// 	#define ENABLE_DEBUG_ENTER_EXIT 1
#endif

#ifdef ENABLE_DEBUG_ENTER_EXIT
#define DBG_ENTER(x) GetRenderThread().GetLogger().LogInfo("RenderWorld." x ": ENTER");
#define DBG_ENTER_PARAM(x,pt,pv) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt "]", pv);
#define DBG_ENTER_PARAM2(x,pt1,pv1,pt2,pv2) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt1 "," pt2 "]", pv1, pv2);
#define DBG_ENTER_PARAM3(x,pt1,pv1,pt2,pv2,pt3,pv3) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt1 "," pt2 "," pt3 "]", pv1, pv2, pv3);
#define DBG_EXIT(x) GetRenderThread().GetLogger().LogInfo("RenderWorld." x ": EXIT");
#else
#define DBG_ENTER(x)
#define DBG_ENTER_PARAM(x,pt,pv)
#define DBG_ENTER_PARAM2(x,pt1,pv1,pt2,pv2)
#define DBG_ENTER_PARAM3(x,pt1,pv1,pt2,pv2,pt3,pv3)
#define DBG_EXIT(x)
#endif



void deoglRenderTranspCounting::CountTransparency(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask){
DBG_ENTER_PARAM("deoglRenderTranspCounting::CountTransparency", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "TranspCounting.CountTransparency");
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	int realWidth = defren.GetWidth();
	int realHeight = defren.GetHeight();
	int curWidth, curHeight;
	bool useTexture1;
	int nextSize;
	
	
	// drop cached count
	pHasCount = false;
	pCount = 0;
	
	
	// attach the first counter texture to store the count of layers per pixel
	pPipelineClearBuffers->Activate();
	defren.ActivateFBODiffuse(true);
	SetViewport(plan);
	
	// clear the counter texture to 0
	const GLfloat clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	OGL_CHECK(renderThread, pglClearBufferfv(GL_COLOR, 0, &clearColor[0]));
	
	// render transparent scene elements
	deoglSkinTexturePipelines::eTypes pipelineType;
	if(mask && mask->GetUseClipPlane()){
		pipelineType = deoglSkinTexturePipelines::etCounterClipPlane;
		
	}else{
		pipelineType = deoglSkinTexturePipelines::etCounter;
	}
	
	int pipelineModifier = 0;
	if(plan.GetFlipCulling()){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if(plan.GetRenderStereo()){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock(renworld.GetRenderPB());
	renderTask.SetRenderVSStereo(plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid(false);
	addToRenderTask.SetNoNotReflected(plan.GetNoReflections());
	addToRenderTask.SetSkinPipelineType(pipelineType);
	addToRenderTask.SetSkinPipelineModifier(pipelineModifier);
	
	addToRenderTask.AddComponents(collideList);
	addToRenderTask.AddBillboards(collideList);
	
	if(renderThread.GetChoices().GetRealTransparentParticles()){
		addToRenderTask.AddParticles(collideList);
	}
	
	// render
	renderTask.PrepareForRender();
	rengeom.RenderTask(renderTask);
	
	
	// outline
	renderTask.Clear();
	renderTask.SetRenderParamBlock(renworld.GetRenderPB());
	renderTask.SetRenderVSStereo(plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer());
	
	addToRenderTask.Reset();
	addToRenderTask.SetOutline(true);
	addToRenderTask.SetFilterDecal(true);
	addToRenderTask.SetDecal(false);
	addToRenderTask.SetSolid(false);
	addToRenderTask.SetNoNotReflected(plan.GetNoReflections());
	addToRenderTask.SetSkinPipelineType(pipelineType);
	addToRenderTask.SetSkinPipelineModifier(pipelineModifier);
	
	addToRenderTask.AddComponents(collideList);
	
	if(renderTask.GetPipelineCount() > 0){
		renderTask.PrepareForRender();
		rengeom.RenderTask(renderTask);
	}
	
	
	// calculate the maximum layer count. uses ping pong between diffuse and reflectivity buffer.
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineTraCountMaxCountStereo : *pPipelineTraCountMaxCount;
	pipeline.Activate();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	OGL_CHECK(renderThread, pglBindVertexArray(defren.GetVAOFullScreenQuad()->GetVAO()));
	
	useTexture1 = true;
	curWidth = realWidth;
	curHeight = realHeight;
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	while(curWidth > 1 || curHeight > 1){
		// reduce in x direction
		if(curWidth > 1){
			nextSize = (curWidth - 1) / 8 + 1;
			
			if(useTexture1){
				defren.ActivateFBOReflectivity(false);
				tsmgr.EnableArrayTexture(0, *defren.GetTextureDiffuse(), GetSamplerClampNearest());
				
			}else{
				defren.ActivateFBODiffuse(false);
				tsmgr.EnableArrayTexture(0, *defren.GetTextureReflectivity(), GetSamplerClampNearest());
			}
			SetViewport(nextSize, curHeight);
			
			OGL_CHECK(renderThread, pglClearBufferfv(GL_COLOR, 0, &clearColor[0]));
			
			shader.SetParameterInt(sptcmcClampTC, 0, 0, curWidth - 1, curHeight - 1);
			shader.SetParameterInt(sptcmcOffsets1, 1, 0, 2, 0);
			shader.SetParameterInt(sptcmcOffsets2, 3, 0, 4, 0);
			shader.SetParameterInt(sptcmcOffsets3, 5, 0, 6, 0);
			shader.SetParameterInt(sptcmcOffsets4, 7, 0, 8, 1);
			
			RenderFullScreenQuad(plan);
			
			useTexture1 = !useTexture1;
			curWidth = nextSize;
		}
		
		// reduce in y direction
		if(curHeight > 1){
			nextSize = (curHeight - 1) / 8 + 1;
			
			if(useTexture1){
				defren.ActivateFBOReflectivity(false);
				tsmgr.EnableArrayTexture(0, *defren.GetTextureDiffuse(), GetSamplerClampNearest());
				
			}else{
				defren.ActivateFBODiffuse(false);
				tsmgr.EnableArrayTexture(0, *defren.GetTextureReflectivity(), GetSamplerClampNearest());
			}
			SetViewport(curWidth, nextSize);
			
			OGL_CHECK(renderThread, pglClearBufferfv(GL_COLOR, 0, &clearColor[0]));
			
			shader.SetParameterInt(sptcmcClampTC, 0, 0, curWidth - 1, curHeight - 1);
			shader.SetParameterInt(sptcmcOffsets1, 0, 1, 0, 2);
			shader.SetParameterInt(sptcmcOffsets2, 0, 3, 0, 4);
			shader.SetParameterInt(sptcmcOffsets3, 0, 5, 0, 6);
			shader.SetParameterInt(sptcmcOffsets4, 0, 7, 1, 8);
			
			RenderFullScreenQuad(plan);
			
			useTexture1 = !useTexture1;
			curHeight = nextSize;
		}
	}
	
	
	// start the occlusion query to determine the count. occlusion queries always have a little
	// delay so we fetch the result after the solid pass
	#ifdef WITH_OPENGLES
		// OpenGL ES does not support counting queries only any samples passed type queries.
		// we are forced to do a slower glReadPixels on the last set frame buffer attachment
		GLbyte queryResult[4];
		OGL_CHECK(renderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
		OGL_CHECK(renderThread, glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&queryResult));
		
		plan.SetTransparencyLayerCount(queryResult[0]);
		
	#else
		if(useTexture1){
			defren.ActivateFBOReflectivity(false);
			tsmgr.EnableArrayTexture(0, *defren.GetTextureDiffuse(), GetSamplerClampNearest());
			
		}else{
			defren.ActivateFBODiffuse(false);
			tsmgr.EnableArrayTexture(0, *defren.GetTextureReflectivity(), GetSamplerClampNearest());
		}
		SetViewport(100, 1);
		
		(plan.GetRenderStereo() ? pPipelineTraCountGetCountStereo : pPipelineTraCountGetCount)->Activate();
		
		renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
		
		const GLfloat clearColor2[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		OGL_CHECK(renderThread, pglClearBufferfv(GL_COLOR, 0, &clearColor2[0]));
		
		pOccQuery->BeginQuery(deoglOcclusionQuery::eqtCount);
		RenderFullScreenQuad(plan);
		pOccQuery->EndQuery();
		
		OGL_CHECK(renderThread, pglBindVertexArray(0));
	#endif
	
	// invalidate buffer. it is not needed anymore
	renderThread.GetFramebuffer().GetActive()->InvalidateColor(0);
DBG_EXIT("deoglRenderTranspCounting::CountTransparency")
}



int deoglRenderTranspCounting::GetCount(){
	if(!pHasCount){
		pCount = pOccQuery->GetResult();
		pHasCount = true;
	}
	
	return pCount;
}



// Private Functions
//////////////////////

void deoglRenderTranspCounting::pCleanUp(){
	if(pOccQuery){
		delete pOccQuery;
	}
}
