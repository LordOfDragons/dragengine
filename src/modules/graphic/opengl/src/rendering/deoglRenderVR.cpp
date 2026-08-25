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

#include "deoglRenderVR.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/deoglTextureStageManager.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vr/deoglVR.h"
#include "../world/deoglRCamera.h"

#include <dragengine/common/exceptions.h>


// Class deoglRenderVR
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderVR::deoglRenderVR(deoglRenderThread &renderThread) :
deoglRenderBase(renderThread),
pVBOHud(0),
pHudVertexCount(0)
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const bool vsRenderLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	deoglPipelineConfiguration pipconf, pipconf2;
	deoglShaderDefines defines, commonDefines;
	const deoglShaderSources *sources;
	
	try{
		renderThread.GetShader().SetCommonDefines(commonDefines);
		
		// hidden area
		pipconf.Reset();
		pipconf.SetMasks(false, false, false, false, false);
		pipconf.EnableDepthTestAlways();
		pipconf.SetEnableScissorTest(true);
		pipconf.SetEnableStencilTest(true);
		pipconf.SetStencil(GL_ALWAYS, 0x0, 0x01, 0x01);
		pipconf.SetStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
		pipconf2 = pipconf;
		pipconf2.SetDepthMask(true);
		pipconf2.SetDepthFunc(renderThread.GetChoices().GetDepthCompareFuncRegular());
		pipconf2.SetEnableStencilTest(false);
		pipconf2.SetClipControl(useInverseDepth);
		
		defines = commonDefines;
		if(vsRenderLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		sources = shaderManager.GetSourcesNamed(vsRenderLayer ? "VR Hidden Area" : "VR Hidden Area Stereo");
		pAsyncGetPipeline(pPipelineHiddenAreaClearMask, pipconf, sources, defines);
		pAsyncGetPipeline(pPipelineHiddenAreaDepth, pipconf2, sources, defines);
		
		// hidden area stereo left
		defines.SetDefines("SPLIT_LAYERS");
		defines.SetDefine("RENDER_PASS", 0);
		pAsyncGetPipeline(pPipelineHiddenAreaClearMaskStereoLeft, pipconf, sources, defines);
		pAsyncGetPipeline(pPipelineHiddenAreaDepthStereoLeft, pipconf2, sources, defines);
		
		// hidden area stereo right
		defines.SetDefine("RENDER_PASS", 1);
		pAsyncGetPipeline(pPipelineHiddenAreaClearMaskStereoRight, pipconf, sources, defines);
		pAsyncGetPipeline(pPipelineHiddenAreaDepthStereoRight, pipconf2, sources, defines);
		
		
		// hud
		pipconf.Reset();
		pipconf.SetMasks(true, true, true, true, false);
		pipconf.SetEnableScissorTest(true);
		pipconf.EnableBlendBlend();
		
		defines = commonDefines;
		pAsyncGetPipeline(pPipelineHud, pipconf, "VR Hud", defines);
		
		pCreateHudVAO();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglRenderVR::~deoglRenderVR(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderVR::RenderHiddenArea(deoglRenderPlan &plan, bool clearMask){
	if(!plan.GetCamera() || !plan.GetCamera()->GetVR()){
		return;
	}
	
	const deoglVR &vr = *plan.GetCamera()->GetVR();
	deoglRModel *model = nullptr;
	deoglRModel *modelLeft = nullptr;
	deoglRModel *modelRight = nullptr;
	
	switch(plan.GetRenderVR()){
	case deoglRenderPlan::ervrLeftEye:
		model = vr.GetLeftEye().GetHiddenMesh();
		break;
		
	case deoglRenderPlan::ervrRightEye:
		model = vr.GetRightEye().GetHiddenMesh();
		break;
		
	case deoglRenderPlan::ervrStereo:
		modelLeft = vr.GetLeftEye().GetHiddenMesh();
		modelRight = vr.GetRightEye().GetHiddenMesh();
		break;
		
	case deoglRenderPlan::ervrNone:
		return;
	}
	
	if(!model && !modelLeft && !modelRight){
		return;
	}
	
	if(model){
		deoglModelLOD &lod = model->GetLODAt(0);
		lod.PrepareVBOBlock();
		const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
		if(vboBlock){
			(clearMask ? pPipelineHiddenAreaClearMask : pPipelineHiddenAreaDepth)->Activate();
			pRenderHiddenArea(*vboBlock);
		}
		
	}else{
		if(modelLeft){
			deoglModelLOD &lod = modelLeft->GetLODAt(0);
			lod.PrepareVBOBlock();
			const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
			if(vboBlock){
				(clearMask ? pPipelineHiddenAreaClearMaskStereoLeft : pPipelineHiddenAreaDepthStereoLeft)->Activate();
				pRenderHiddenArea(*vboBlock);
			}
		}
		
		if(modelRight){
			deoglModelLOD &lod = modelRight->GetLODAt(0);
			lod.PrepareVBOBlock();
			const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
			if(vboBlock){
				(clearMask ? pPipelineHiddenAreaClearMaskStereoRight : pPipelineHiddenAreaDepthStereoRight)->Activate();
				pRenderHiddenArea(*vboBlock);
			}
		}
	}
	
	pglBindVertexArray(0);
}

void deoglRenderVR::RenderHud(deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const auto &canvas = renderThread.GetCanvasVRHudOverlay();
	if(!canvas || canvas->HasNoChildren() || !plan.GetCamera()->GetVR()){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace(renderThread, "Hud.Render");
	
	pPipelineHud->Activate();
	auto &shader = pPipelineHud->GetShader();
	
	renderThread.GetFramebuffer().Activate(plan.GetFBOTarget());
	
	const int viewportWidth = plan.GetViewportWidth();
	const int viewportHeight = plan.GetViewportHeight();
	OGL_CHECK(renderThread, glViewport(0, 0, viewportWidth, viewportHeight));
	OGL_CHECK(renderThread, glScissor(0, 0, viewportWidth, viewportHeight));
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.DisableAllStages();
	tsmgr.EnableTexture(0, *canvas->GetRenderTarget()->GetTexture(), GetSamplerClampNearest());
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	float gamma = OGL_RENDER_GAMMA;
	if(plan.GetUseHdrOutput() || plan.GetCamera()->GetVR()->GetLeftEye().GetUseGammaCorrection()){
		gamma = 1.0f;
	}
	shader.SetParameterFloat(0, gamma, gamma, gamma);
	
	OGL_CHECK(renderThread, pglBindVertexArray(pVAOHud->GetVAO()));
	
	OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLES,
		plan.GetRenderVR() == deoglRenderPlan::ervrLeftEye ? 0 : pHudVertexCount,
		pHudVertexCount));
	
	OGL_CHECK(renderThread, pglBindVertexArray(0));
	
	tsmgr.DisableAllStages();
}


// Private Functions
//////////////////////

void deoglRenderVR::pCleanUp(){
	if(pVBOHud){
		GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer(pVBOHud);
		pVBOHud = 0;
	}
}

void deoglRenderVR::pRenderHiddenArea(const deoglSharedVBOBlock &vboBlock){
	deoglRenderThread &renderThread = GetRenderThread();
	
	const deoglVAO &vao = *vboBlock.GetVBO()->GetVAO();
	pglBindVertexArray(vao.GetVAO());
	
	if(renderThread.GetChoices().GetSharedVBOUseBaseVertex()){
		OGL_CHECK(renderThread, pglDrawElementsBaseVertex(GL_TRIANGLES,
			vboBlock.GetIndexCount(), vao.GetIndexGLType(),
			(GLvoid*)(intptr_t)(vao.GetIndexSize() * vboBlock.GetIndexOffset()),
			vboBlock.GetOffset()));
		
	}else{
		OGL_CHECK(renderThread, glDrawElements(GL_TRIANGLES,
			vboBlock.GetIndexCount(), vao.GetIndexGLType(),
			(GLvoid*)(intptr_t)(vao.GetIndexSize() * vboBlock.GetIndexOffset())));
	}
}

void deoglRenderVR::pCreateHudVAO(){
	deoglRenderThread &renderThread = GetRenderThread();
	const int quadCount = 10;
	
	pHudVertexCount = quadCount * quadCount * 6; // 2 triangles per quad
	const int totalVertexCount = pHudVertexCount * 2; // two layers
	
	// create VBO with interleaved vertex data. not using a pre-allocated array since gcc
	// out-of-bounds check fails to understand the loop is correct
	OGL_CHECK(renderThread, pglGenBuffers(1, &pVBOHud));
	OGL_CHECK(renderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBOHud));
	
	struct sVertex{
		float x, y, u, v;
		int eye;
	};
	decTList<sVertex> vertices(totalVertexCount);
	const float factorPos = 2.0f / (float)quadCount;
	const float factorTc = 1.0f / (float)quadCount;
	
	for(int e=0; e<2; e++){
		for(int y=0; y<quadCount; y++){
			for(int x=0; x<quadCount; x++){
				const float x0 = (float)x * factorPos - 1.0f;
				const float x1 = (float)(x + 1) * factorPos - 1.0f;
				const float y0 = (float)y * factorPos - 1.0f;
				const float y1 = (float)(y + 1) * factorPos - 1.0f;
				
				const float u0 = (float)x * factorTc;
				const float u1 = (float)(x + 1) * factorTc;
				const float v0 = 1.0f - (float)y * factorTc;
				const float v1 = 1.0f - (float)(y + 1) * factorTc;
				
				// triangle 1: top left, top right, bottom right
				vertices.Add({x0, y0, u0, v0, e});
				vertices.Add({x1, y0, u1, v0, e});
				vertices.Add({x1, y1, u1, v1, e});
				
				// triangle 2: top left, bottom right, bottom left
				vertices.Add({x0, y0, u0, v0, e});
				vertices.Add({x1, y1, u1, v1, e});
				vertices.Add({x0, y1, u0, v1, e});
			}
		}
	}
	
	OGL_CHECK(renderThread, pglBufferData(GL_ARRAY_BUFFER,
		totalVertexCount * sizeof(sVertex), vertices.GetArrayPointer(), GL_STATIC_DRAW));
	
	// create VAO
	pVAOHud = deTUniqueReference<deoglVAO>::New(renderThread);
	OGL_CHECK(renderThread, pglBindVertexArray(pVAOHud->GetVAO()));
	
	OGL_CHECK(renderThread, pglEnableVertexAttribArray(0));
	OGL_CHECK(renderThread, pglVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 20, (const GLvoid *)0));
	
	OGL_CHECK(renderThread, pglEnableVertexAttribArray(1));
	OGL_CHECK(renderThread, pglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (const GLvoid *)8));
	
	OGL_CHECK(renderThread, pglEnableVertexAttribArray(2));
	OGL_CHECK(renderThread, pglVertexAttribIPointer(2, 1, GL_INT, 20, (const GLvoid *)16));
}
