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

#include "deoglRenderLight.h"
#include "deoglRenderLightSpot.h"
#include "deoglRenderLightSky.h"
#include "deoglRenderLightPoint.h"
#include "deoglRenderLightParticles.h"
#include "deoglRenderGI.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglAddToRenderTask.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../gi/deoglGIState.h"
#include "../../gi/deoglGICascade.h"
#include "../../light/deoglRLight.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shadow/deoglShadowMapper.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/deoglImageStageManager.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eShaderParameterSSAO{
	espssaoTCTransform,
	espssaoSCTransform
};

enum eSPAOBlur{
	spaobTCTransform,
	spaobTCTransformAlt,
	spaobOffsets1,
	spaobOffsets2,
	spaobOffsets3,
	spaobOffsets4,
	spaobOffsets5,
	spaobOffsets6,
	spaobOffsets7,
	spaobWeights1,
	spaobWeights2,
	spaobClamp,
	spaobDepthDifferenceThreshold
};

enum eShaderParameterSSAOBlur{
	espssaobTCDataToDepth,
	espssaobOffsetRead,
	espssaobOffsetWrite,
	espssaobClamp,
	espssaobDepthDifferenceThreshold
};

enum eShaderParameterSSAOUpscale{
	espssaousTCTransform
};

enum pSPDebugAO{
	spdaoPosTransform,
	spdaoTCTransform,
	spdaoColor,
	spdaoGamma,
	spdaoLevel,
	spdaoLayer
};

enum eSPCopyDepth{
	spcdQuadParams
};



// Class deoglRenderLight
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLight::deoglRenderLight(deoglRenderThread &renderThread,
deoglRTRenderers &renderers) :
deoglRenderLightBase(renderThread),

pRenderLightSpot(NULL),
pRenderLightSky(NULL),
pRenderLightPoint(NULL),
pRenderLightParticles(NULL),
pRenderGI(NULL),

pRenderTask(NULL),
pAddToRenderTask(NULL)
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	renderThread.GetShader().SetCommonDefines(commonDefines);
	
	pShadowPBSingleUse = deoglSPBSingleUse::Ref::New(renderThread,
		deoglSkinShader::CreateSPBRender(renderThread));
	pOccMapPBSingleUse = deoglSPBSingleUse::Ref::New(renderThread,
		deoglSkinShader::CreateSPBOccMap(renderThread));
	
	pRenderTask = new deoglRenderTask(renderThread);
	pAddToRenderTask = new deoglAddToRenderTask(renderThread, *pRenderTask);
	
	pRenderLightSky = new deoglRenderLightSky(renderThread);
	pRenderLightSpot = new deoglRenderLightSpot(renderThread, renderers);
	pRenderLightPoint = new deoglRenderLightPoint(renderThread, renderers);
	pRenderLightParticles = new deoglRenderLightParticles(renderThread);
	pRenderGI = new deoglRenderGI(renderThread);
	
	
	
	// sssss
	pipconf.Reset();
	pipconf.SetMasks(true, true, true, false, false);
	pipconf.EnableBlendAdd();
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen ScreenSpace SubSurface Scattering");
	defines.SetDefines("FULLSCREENQUAD", "NO_POSTRANSFORM");
	pAsyncGetPipeline(pPipelineSSSSS, pipconf, sources, defines);
	
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("DefRen ScreenSpace SubSurface Scattering Stereo");
	}
	pAsyncGetPipeline(pPipelineSSSSSStereo, pipconf, sources, defines);
	
	
	
	// ambient occlusion
	pipconf.Reset();
	// pipconf.SetMasks( false, true, false, false, false );
	pipconf.SetMasks(true, true, true, true, false);
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen AmbientOcclusion Local");
	defines.SetDefine("SSAO_RESOLUTION_COUNT", 1); // 1-4
	defines.SetDefines("FULLSCREENQUAD", "FULLSCREENQUAD_TCTRANSFORM",
		"NO_POSTRANSFORM", "FULLSCREENQUAD_SCTRANSFORM");
	pAsyncGetPipeline(pPipelineAOLocal, pipconf, sources, defines);
	
	// ambient occlusion stereo
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("DefRen AmbientOcclusion Local Stereo");
	}
	pAsyncGetPipeline(pPipelineAOLocalStereo, pipconf, sources, defines);
	
	
	
	// ambient occlusion blur phase 1
	pipconf.SetMasks(true, false, false, false, false);
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("Gauss Separable Fixed");
	defines.SetDefine("TAP_COUNT", 9);
	defines.SetDefine("OUT_DATA_SIZE", 1);
	defines.SetDefine("TEX_DATA_SIZE", 1);
	defines.SetDefine("TEX_DATA_SWIZZLE", 1); // gggg
	defines.SetDefines("DEPTH_DIFFERENCE_WEIGHTING", "INPUT_ARRAY_TEXTURES");
	defines.SetDefines("NO_POSTRANSFORM");
	pAsyncGetPipeline(pPipelineAOBlur1, pipconf, sources, defines);
	
	// ambient occlusion blur phase 1 stereo
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("Gauss Separable Fixed Stereo");
	}
	pAsyncGetPipeline(pPipelineAOBlur1Stereo, pipconf, sources, defines);
	
	
	// ambient occlusion blur phase 2
	pipconf.SetMasks(false, true, false, false, false);
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("Gauss Separable Fixed");
	defines.SetDefine("TAP_COUNT", 9);
	defines.SetDefine("OUT_DATA_SIZE", 3);
	defines.SetDefine("OUT_DATA_SWIZZLE", 1); // gggg
	defines.SetDefine("TEX_DATA_SIZE", 1);
	defines.SetDefines("DEPTH_DIFFERENCE_WEIGHTING", "INPUT_ARRAY_TEXTURES");
	defines.SetDefines("NO_POSTRANSFORM");
	pAsyncGetPipeline(pPipelineAOBlur2, pipconf, sources, defines);
	
	// ambient occlusion blur phase 2 stereo
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("Gauss Separable Fixed Stereo");
	}
	pAsyncGetPipeline(pPipelineAOBlur2Stereo, pipconf, sources, defines);
	
	
	// ambient occlusion debug
	pipconf.SetMasks(true, true, true, false, false);
	pipconf.SetEnableBlend(false);
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("Debug Display Texture");
	defines.SetDefines("TEXTURELEVEL");
	defines.SetDefine("OUT_DATA_SIZE", 3);
	defines.SetDefine("TEX_DATA_SWIZZLE", 1); // gggg
	pAsyncGetPipeline(pPipelineDebugAO, pipconf, sources, defines);
	
	
	
	// ssao
	/*
	pipconf.Reset();
	pipconf.SetType(deoglPipelineConfiguration::etCompute);
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen SSAO");
	defines.SetDefine("SSAO_RESOLUTION_COUNT", 1); // 1-4
	pAsyncGetPipeline(pPipelineSSAO, pipconf, sources, defines);
	*/
	
	pipconf.Reset();
	pipconf.SetType(deoglPipelineConfiguration::etCompute);
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen SSAO Blur");
	pAsyncGetPipeline(pPipelineSSAOBlur1, pipconf, sources, defines);
	
	defines.SetDefine("RENDER_PASS", 1);
	pAsyncGetPipeline(pPipelineSSAOBlur2, pipconf, sources, defines);
	
	
	// ssao upscale
	pipconf.Reset();
	pipconf.SetMasks(false, true, false, false, false);
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen SSAO Upscale");
	defines.SetDefines("NO_POSTRANSFORM");
	pAsyncGetPipeline(pPipelineSSAOUpscale, pipconf, sources, defines);
	
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("DefRen SSAO Upscale Stereo");
	}
	pAsyncGetPipeline(pPipelineSSAOUpscaleStereo, pipconf, sources, defines);
	
	
	
	// copy depth
	pipconf.Reset();
	pipconf.SetMasks(false, false, false, false, true);
	pipconf.EnableDepthTestAlways();
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed("DefRen Copy Depth");
	pAsyncGetPipeline(pPipelineCopyDepth, pipconf, sources, defines);
	
	// copy depth stereo
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if(!renderFSQuadStereoVSLayer){
		sources = shaderManager.GetSourcesNamed("DefRen Copy Depth Stereo");
	}
	pAsyncGetPipeline(pPipelineCopyDepthStereo, pipconf, sources, defines);
	
	
	
	// debug information
	const decColor colorText(1.0f, 1.0f, 1.0f, 1.0f);
	const decColor colorBg(0.0f, 0.0f, 0.25f, 0.75f);
	const decColor colorBgSub(0.05f, 0.05f, 0.05f, 0.75f);
	
	pDebugInfoSolid = deoglDebugInformation::Ref::New("Lights Solid", colorText, colorBg);
	
	pDebugInfoSolidCopyDepth = deoglDebugInformation::Ref::New("Copy Depth", colorText, colorBgSub);
	pDebugInfoSolid->GetChildren().Add(pDebugInfoSolidCopyDepth);
	
	pDebugInfoSolid->GetChildren().Add(pRenderLightSky->GetDebugInfoSolid());
	pDebugInfoSolid->GetChildren().Add(pRenderLightPoint->GetDebugInfoSolid());
	pDebugInfoSolid->GetChildren().Add(pRenderLightSpot->GetDebugInfoSolid());
	
	pDebugInfoSolidParticle = deoglDebugInformation::Ref::New("Particle", colorText, colorBgSub);
	pDebugInfoSolid->GetChildren().Add(pDebugInfoSolidParticle);
	
	pDebugInfoSolidSSSSS = deoglDebugInformation::Ref::New("SSSSS", colorText, colorBgSub);
	pDebugInfoSolid->GetChildren().Add(pDebugInfoSolidSSSSS);
	
	
	
	pDebugInfoTransparent = deoglDebugInformation::Ref::New("Lights Transp", colorText, colorBg);
	
	pDebugInfoTransparentCopyDepth = deoglDebugInformation::Ref::New("Copy Depth", colorText, colorBgSub);
	pDebugInfoTransparent->GetChildren().Add(pDebugInfoTransparentCopyDepth);
	
	pDebugInfoTransparent->GetChildren().Add(pRenderLightSky->GetDebugInfoTransparent());
	pDebugInfoTransparent->GetChildren().Add(pRenderLightPoint->GetDebugInfoTransparent());
	pDebugInfoTransparent->GetChildren().Add(pRenderLightSpot->GetDebugInfoTransparent());
	
	pDebugInfoTransparentSSSSS = deoglDebugInformation::Ref::New("SSSSS", colorText, colorBgSub);
	pDebugInfoTransparent->GetChildren().Add(pDebugInfoTransparentSSSSS);
	
	
	
	DevModeDebugInfoChanged();
}

deoglRenderLight::~deoglRenderLight(){
	pCleanUp();
}



// Rendering
//////////////

const deoglSPBlockUBO::Ref &deoglRenderLight::NextShadowPB(){
	pShadowPB = (deoglSPBlockUBO*)pShadowPBSingleUse->Next();
	return pShadowPB;
}

const deoglSPBlockUBO::Ref & deoglRenderLight::NextOccMapPB(){
	pOccMapPB = (deoglSPBlockUBO*)pOccMapPBSingleUse->Next();
	return pOccMapPB;
}

void deoglRenderLight::RenderLights(deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, solid
		? (mask ? "Light.RenderLights(Solid,Masked)" : "Light.RenderLights(Solid)")
		: (mask ? "Light.RenderLights(Transparent,Masked)" : "Light.RenderLights(Transparent)"));
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const bool sssssEnable = config.GetSSSSSEnable();
	
	DebugTimersReset(plan, false);
	
	// copy the depth. this is only required if we are not using encoded depth
	/*
	if(renderThread.GetDebug().GetDeveloperMode().GetDebugEnableLightDepthStencil()){
		renderThread.GetDeferredRendering().CopyFirstDepthToSecond(true, true);
		
		if(solid){
			DebugTimer2Sample(plan, *pDebugInfoSolidCopyDepth, true);
			
		}else{
			DebugTimer2Sample(plan, *pDebugInfoTransparentCopyDepth, true);
		}
	}*/
	
	// renderThread.GetDeferredRendering().CopyFirstDepthToSecond( true, false );
	CopyDepth1ToDepth3(plan);
	
	if(solid){
		DebugTimer2Sample(plan, *pDebugInfoSolidCopyDepth, true);
		
	}else{
		DebugTimer2Sample(plan, *pDebugInfoTransparentCopyDepth, true);
	}
	
	// render lights
	const bool hasGIStateUpdate = plan.GetUpdateGIState() != NULL;
	const bool hasGIStateRender = plan.GetRenderGIState() != NULL;
	
	if(solid && !mask && !xray && hasGIStateUpdate){
		pRenderGI->ClearProbes(plan);
	}
	
	RestoreFBO(plan);
	
	if(sssssEnable){
		pPipelineClearBuffers->Activate();
		renderThread.GetDeferredRendering().ActivateFBOTemporary2(false);
		
		GLfloat clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		OGL_CHECK(renderThread, pglClearBufferfv(GL_COLOR, 0, &clearColor[0]));
	}
	
	if(hasGIStateRender){
		pRenderGI->PrepareUBORenderLight(plan);
	}
	
	pRenderLightPoint->RenderLights(plan, solid, mask);
	pRenderLightSpot->RenderLights(plan, solid, mask);
	
	// sky light requires large render tasks that can be expensive to build. to do this as
	// fast as possible the render task building is done using parallel tasks. by rendering
	// sky light as last light type before GI reduces the time waiting for the parallel
	// tasks to finish
	pRenderLightSky->RenderLights(plan, solid, mask, xray);
	
	if(solid && !mask && hasGIStateUpdate){
		if(hasGIStateRender){
			pRenderGI->RenderLightGIRay(plan);
		}
		
		// required or update probes compute shaders accessing light texture written by
		// lighting (including RenderLightGIRay above) can cause garbage to be read
		OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
			| GL_TEXTURE_FETCH_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT));
		
		pRenderGI->UpdateProbes(plan);
		SetViewport(plan);
	}
	
	if(hasGIStateRender){
		pRenderGI->RenderLight(plan, solid);
	}
	
	DebugTimer2Reset(plan, false);
	
	if(solid){
		pRenderLightParticles->RenderLights(plan);
		DebugTimer2Sample(plan, *pDebugInfoSolidParticle, true);
	}
	
	// apply screen space sub surface scattering
	if(sssssEnable){
		RenderSSSSS(plan, solid);
		if(solid){
			DebugTimer2Sample(plan, *pDebugInfoSolidSSSSS, true);
			
		}else{
			DebugTimer2Sample(plan, *pDebugInfoTransparentSSSSS, true);
		}
	}
	
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	shadowMapper.DropTextures();
	shadowMapper.DropCubeMaps();
	
	// do not remove this line. without the rendering of upcoming light passes
	// (even across frames) is seriously broken
	renderThread.GetTexture().GetStages().DisableAllStages();
	
	if(solid){
		DebugTimer1Sample(plan, *pDebugInfoSolid, true);
		
	}else{
		DebugTimer1SampleCount(plan, *pDebugInfoTransparent, 1, true);
	}
}



void deoglRenderLight::RenderAO(deoglRenderPlan &plan, bool solid){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, solid ? "Light.RenderAO(Solid)" : "Light.RenderAO(Transparent)");
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	if(!config.GetSSAOEnable() || plan.GetDisableLights()){
		return;
	}
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglImageStageManager &ismgr = renderThread.GetTexture().GetImageStages();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	// deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	deoglShaderCompiled *shader;
	
	const int width = defren.GetWidth();
	const int height = defren.GetHeight();
	const float pixelSizeU = defren.GetPixelSizeU();
	const float pixelSizeV = defren.GetPixelSizeV();
	
	const int reduction = plan.GetRenderStereo() ? 3 : 2;
	const int reducedWidth = width / reduction;
	const int reducedHeight = height / reduction;
	const float tcshift = ((reduction - 1) % 2) == 1 ? -0.5f : 0.0f;
	const float inttcshift = ((reduction - 1) % 2) == 1 ? 0.5f : 0.0f;
	
	if(reducedWidth < 2 || reducedHeight < 2){
		return;
	}
	
	
	// performance:
	//
	// SSAO for every pixel with best possible quality:
	// - monitor 1920x1080: ssao=948 blur1=334 blur2=317: total 1623
	// - vr hmd 2468x2740: ssao=4560 blur1=2080 blur2=1985: total 8631
	// 
	// this is already expensive for monitor and gets very expensive for hmd. SSAO though
	// is for of a global effect so calculating it at lower scale has slight quality loss
	// but improves performance considerable.
	// 
	// reduction 2:
	// - monitor 1920x1080: ssao=326 blur1=56 blur2=78 upscale=44: total 480 [~30%, -1.14ms]
	// - vr hmd 2468x2740: ssao=1418 blur1=268 blur2=330 upscale=237 : total 2254 [~26%, -6.38ms]
	// 
	// reduction 3:
	// - monitor 1920x1080: ssao=244 blur1=33 blur2=40 upscale=44: total 363 [~23%, -1.26ms]
	// - vr hmd 2468x2740: ssao=865 blur1=154 blur2=212 upscale=230 : total 1463 [~17%, -7.17ms]
	// 
	// for monitor reduction 2 is enough. reduction 3 is not reducing render time that much
	// but the quality degrades noticeable.
	// 
	// for VR reduction of 3 is required to get better performance. due to the higher resolution
	// the quality loss is more acceptable in contrary to monitor.
	
	
	// new ssao
	/*
	const int workGroupSizeZ = plan.GetRenderStereo() ? 2 : 1;
	
	pPipelineSSAO->Activate();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	tsmgr.EnableArrayTexture(0, *defren.GetDepthTexture1(), GetSamplerClampNearestMipMap());
	ismgr.Enable(0, *defren.GetTextureDiffuse(), 0, deoglImageStageManager::eaRead);
	ismgr.Enable(1, *defren.GetTextureNormal(), 0, deoglImageStageManager::eaRead);
	ismgr.Enable(2, *defren.GetTextureTemporary3(), 0, deoglImageStageManager::eaWrite);
	
	OGL_CHECK(renderThread, pglDispatchCompute((width - 1) / 64 + 1, height, workGroupSizeZ));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
		| GL_TEXTURE_FETCH_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT));
	*/
	
	
	// render SSAO into green channel. if reduction is even sampling the full size depth
	// texture ends up right at the middle of 4 pixels. to avoid problems the texture
	// coordinates are shifted by half a pixels into the negative direction. this way the
	// sampling ends up in the middle of the lower left pixel of the 2x2 pixel group.
	// it doesn't matter which of the 4 pixels in the group we choose just that both
	// texture and screen coordinates are shifted to the same pixel. the TCTransform
	// and SCTransform shader parameter takes care of this. SCTransform uses the
	// same scale value as erutFSTexCoordToScreenCoord in deoglRenderWorld to simulate
	// calling fsquadScreenCoordToTexCoord() as in screenspace.glsl
	const deoglPipeline *pipeline = plan.GetRenderStereo() ? pPipelineAOLocalStereo : pPipelineAOLocal;
	pipeline->Activate();
	
	// defren.ActivateFBOAOSolidity( false );
	defren.ActivateFBOTemporary3();
	SetViewport(reducedWidth, reducedHeight);
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	tsmgr.EnableArrayTexture(0, *defren.GetDepthTexture1(), GetSamplerClampNearest());
	tsmgr.EnableArrayTexture(1, *defren.GetTextureDiffuse(), GetSamplerClampNearest());
	tsmgr.EnableArrayTexture(2, *defren.GetTextureNormal(), GetSamplerClampNearest());
	
	shader = &pipeline->GetShader();
	shader->SetParameterFloat(espssaoTCTransform, 1.0f, 1.0f,
		pixelSizeU * tcshift, pixelSizeV * tcshift);
	shader->SetParameterFloat(espssaoSCTransform, 1.0f, 1.0f,
		(2.0f / defren.GetScalingU()) * (pixelSizeU * tcshift),
		(2.0f / defren.GetScalingV()) * (pixelSizeV * tcshift));
	
	OGL_CHECK(renderThread, pglBindVertexArray(defren.GetVAOFullScreenQuad()->GetVAO()));
	
	RenderFullScreenQuad(plan);
	
	
	// blur passes
	const int workGroupSizeZ = plan.GetRenderStereo() ? 2 : 1;
	const float edgeBlurThreshold = config.GetSSAOEdgeBlurThreshold();
	const float dataToDepthScaleU = defren.GetScalingU() / (float)reducedWidth;
	const float dataToDepthScaleV = defren.GetScalingV() / (float)reducedHeight;
	const float dataToDepthOffsetU = defren.GetPixelSizeU() * inttcshift;
	const float dataToDepthOffsetV = defren.GetPixelSizeV() * inttcshift;
	
	pPipelineSSAOBlur1->Activate();
	shader = &pPipelineSSAOBlur1->GetShader();
	
	tsmgr.EnableArrayTexture(0, *defren.GetDepthTexture1(), GetSamplerClampNearest());
	ismgr.Enable(0, *defren.GetTextureTemporary3(), 0, deoglImageStageManager::eaReadWrite);
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	shader->SetParameterFloat(espssaobTCDataToDepth,
		dataToDepthScaleU, dataToDepthScaleV, dataToDepthOffsetU, dataToDepthOffsetV);
	shader->SetParameterInt(espssaobOffsetRead, 0, 0);
	shader->SetParameterInt(espssaobOffsetWrite, reducedWidth, 0);
	shader->SetParameterInt(espssaobClamp, reducedWidth - 1);
	shader->SetParameterFloat(espssaobDepthDifferenceThreshold, edgeBlurThreshold);
	
	OGL_CHECK(renderThread, pglDispatchCompute(
		(reducedWidth - 1) / 64 + 1, reducedHeight, workGroupSizeZ));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
	
	
	pPipelineSSAOBlur2->Activate();
	shader = &pPipelineSSAOBlur2->GetShader();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	shader->SetParameterFloat(espssaobTCDataToDepth,
		dataToDepthScaleU, dataToDepthScaleV, dataToDepthOffsetU, dataToDepthOffsetV);
	shader->SetParameterInt(espssaobOffsetRead, reducedWidth, 0);
	shader->SetParameterInt(espssaobOffsetWrite, 0, 0);
	shader->SetParameterInt(espssaobClamp, reducedWidth - 1);
	shader->SetParameterFloat(espssaobDepthDifferenceThreshold, edgeBlurThreshold);
	
	OGL_CHECK(renderThread, pglDispatchCompute(
		(reducedHeight - 1) / 64 + 1, reducedWidth, workGroupSizeZ));
	OGL_CHECK(renderThread, pglMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT));
	
	
	// upscale
	pipeline = plan.GetRenderStereo() ? pPipelineSSAOUpscaleStereo : pPipelineSSAOUpscale;
	pipeline->Activate();
	
	defren.ActivateFBOAOSolidity(false);
	SetViewport(plan);
	
	tsmgr.EnableArrayTexture(0, *defren.GetTextureTemporary3(), GetSamplerClampLinear());
	
	shader = &pipeline->GetShader();
	defren.SetShaderParamFSQuad(*shader, espssaousTCTransform, reducedWidth, reducedHeight);
	
	RenderFullScreenQuad(plan);
	
	
	
#if 0
	// gauss coefficients: gauss(i,s) = exp(-i*i/(2*s*s)), gaussNorm(s) = 1/(2*pi*s*s)
	// py ['{:e}'.format(gauss(x,0.9)/gaussNorm(0.9)) for x in [0,1,2,3,4]]
	// s typically 1.
	// 
	// normalization is not used here since the shader normalizes. this is required
	// since depth thresholding changes the actual weights used.
	// 
	// here used similar to ['{:e}'.format(gauss(x,1.1)*0.1963806) for x in [0,1,2,3,4]]:
	// [1.963806e-1, 1.299086e-1, 3.760594e-2, 4.763803e-3, 2.640767e-4]
	// 
	// alternate version:
	// s=1.0: [1.0, 6.065307e-1, 1.353353e-1, 1.110900e-2, 3.354626e-4]
	// s=1.5: [1.0, 8.007374e-1, 4.111123e-1, 1.353353e-1, 2.856550e-2]
	// s=1.2: [1.0, 7.066483e-1, 2.493522e-1, 4.393693e-2, 3.865920e-3]
	// s=1.1: [1.0, 6.615147e-1, 1.914952e-1, 2.425801e-2, 1.344719e-3]
	// 
	// for reduced version s=1 is used to sharpen the AO a bit since during upscaling
	// the sharpness will be reduced a bit. furthermore the values are modified to smear
	// out the ssao a bit to combine better with texture ao
	// 
	// gaussian blur with 9 taps (17 pixels size): pass 1
	// const float blurOffsets[ 4 ] = { 1.411765f, 3.294118f, 5.176471f, 7.058824f };
	const float blurOffsets[4] = {1.0f, 2.0f, 3.0f, 4.0f};
	// const float blurWeights[ 4 ] = { 2.967529e-1f, 9.442139e-2f, 1.037598e-2f, 2.593994e-4f };
	// const float blurWeight0 = 0.1963806f;
	const float blurWeights[5] = {1.0f, 6.065307e-1f, 1.353353e-1f, 1.110900e-2f, 3.354626e-4f};
	//const float blurWeightMod[ 5 ] = { 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };
	const float blurWeightMod[5] = {0.8f, 0.85f, 0.9f, 0.95f, 1.0f};
	const float edgeBlurThreshold = config.GetSSAOEdgeBlurThreshold();
	
	pipeline = plan.GetRenderStereo() ? pPipelineAOBlur1Stereo : pPipelineAOBlur1;
	pipeline->Activate();
	
	defren.ActivateFBOTemporary3();
	SetViewport(reducedWidth, reducedHeight);
	
	const GLfloat clearColor[4] = {1.0f, 1.0f, 1.0f, 0.0f};
	OGL_CHECK(renderThread, pglClearBufferfv(GL_COLOR, 0, &clearColor[0]));
	
	shader = &pipeline->GetGlShader();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	tsmgr.EnableArrayTexture(0, *defren.GetTextureAOSolidity(),
		reduction == 1 ? GetSamplerClampLinear() : GetSamplerClampNearest());
	tsmgr.EnableArrayTexture(1, *defren.GetDepthTexture1(),
		reduction == 1 ? GetSamplerClampLinear() : GetSamplerClampNearest());
	
	defren.SetShaderParamFSQuad(*shader, spaobTCTransform, reducedWidth, reducedHeight);
	shader->SetParameterFloat(spaobTCTransformAlt, (float)reduction, (float)reduction,
		pixelSizeU * tcshift, pixelSizeV * tcshift);
	shader->SetParameterFloat(spaobClamp, pixelSizeU * ((float)reducedWidth - 0.5f),
		pixelSizeV * ((float)reducedHeight - 0.5f));
	shader->SetParameterFloat(spaobDepthDifferenceThreshold, edgeBlurThreshold);
	
	shader->SetParameterFloat(spaobWeights1, blurWeights[0] * blurWeightMod[0],
		blurWeights[1] * blurWeightMod[1], blurWeights[2] * blurWeightMod[2],
		blurWeights[3] * blurWeightMod[3]);
	shader->SetParameterFloat(spaobWeights2, blurWeights[4] * blurWeightMod[4], 0.0f, 0.0f, 0.0f);
	
	shader->SetParameterFloat(spaobOffsets1,
		blurOffsets[0] * pixelSizeU, 0.0f, -blurOffsets[0] * pixelSizeU, 0.0f);
	shader->SetParameterFloat(spaobOffsets2,
		blurOffsets[1] * pixelSizeU, 0.0f, -blurOffsets[1] * pixelSizeU, 0.0f);
	shader->SetParameterFloat(spaobOffsets3,
		blurOffsets[2] * pixelSizeU, 0.0f, -blurOffsets[2] * pixelSizeU, 0.0f);
	shader->SetParameterFloat(spaobOffsets4,
		blurOffsets[3] * pixelSizeU, 0.0f, -blurOffsets[3] * pixelSizeU, 0.0f);
	
	RenderFullScreenQuad(plan);
	
	
	
	// gaussian blur with 9 taps (17 pixels size): pass 2
	pipeline = plan.GetRenderStereo() ? pPipelineAOBlur2Stereo : pPipelineAOBlur2;
	pipeline->Activate();
	
	defren.ActivateFBOAOSolidity(false);
	SetViewport(reducedWidth, reducedHeight);
	
	shader = &pipeline->GetGlShader();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	tsmgr.EnableArrayTexture(0, *defren.GetTextureTemporary3(),
		reduction == 1 ? GetSamplerClampLinear() : GetSamplerClampNearest());
	
	defren.SetShaderParamFSQuad(*shader, spaobTCTransform, reducedWidth, reducedHeight);
	shader->SetParameterFloat(spaobTCTransformAlt, (float)reduction, (float)reduction,
		pixelSizeU * tcshift, pixelSizeV * tcshift);
	
	shader->SetParameterFloat(spaobClamp,
		pixelSizeU * ((float)reducedWidth - 0.5f), pixelSizeV * ((float)reducedHeight - 0.5f));
	shader->SetParameterFloat(spaobDepthDifferenceThreshold, edgeBlurThreshold);
	
	shader->SetParameterFloat(spaobWeights1, blurWeights[0] * blurWeightMod[0],
		blurWeights[1] * blurWeightMod[1], blurWeights[2] * blurWeightMod[2],
		blurWeights[3] * blurWeightMod[3]);
	shader->SetParameterFloat(spaobWeights2, blurWeights[4] * blurWeightMod[4], 0.0f, 0.0f, 0.0f);
	
	shader->SetParameterFloat(spaobOffsets1,
		0.0f, blurOffsets[0] * pixelSizeV, 0.0f, -blurOffsets[0] * pixelSizeV);
	shader->SetParameterFloat(spaobOffsets2,
		0.0f, blurOffsets[1] * pixelSizeV, 0.0f, -blurOffsets[1] * pixelSizeV);
	shader->SetParameterFloat(spaobOffsets3,
		0.0f, blurOffsets[2] * pixelSizeV, 0.0f, -blurOffsets[2] * pixelSizeV);
	shader->SetParameterFloat(spaobOffsets4,
		0.0f, blurOffsets[3] * pixelSizeV, 0.0f, -blurOffsets[3] * pixelSizeV);
	
	RenderFullScreenQuad(plan);
	
	// debug output
	if(devmode.GetShowSSAO()){
		pPipelineDebugAO->Activate();
		
		renderThread.GetFramebuffer().Activate(devmode.GetFBODebugImageWith(width, height));
		SetViewport(plan);
		
		shader = &pPipelineDebugAO->GetGlShader();
		
		tsmgr.EnableArrayTexture(0, *defren.GetTextureAOSolidity(), GetSamplerClampNearest());
		
		shader->SetParameterFloat(spdaoPosTransform, 1.0f, 1.0f, 0.0f, 0.0f);
		defren.SetShaderParamFSQuad(*shader, spdaoTCTransform);
		
		shader->SetParameterFloat(spdaoColor, 1.0f, 1.0f, 1.0f, 1.0f);
		shader->SetParameterFloat(spdaoGamma, 2.2f, 2.2f, 2.2f, 1.0f);
		shader->SetParameterFloat(spdaoLevel, 0.0f);
		
		OGL_CHECK(renderThread, glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
	}
#endif
}



void deoglRenderLight::RenderSSSSS(deoglRenderPlan &plan, bool solid){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, solid ? "Light.RenderSSSSS(Solid)" : "Light.RenderSSSSS(Transparent)");
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineSSSSSStereo : *pPipelineSSSSS;
	pipeline.Activate();
	
	defren.ActivateFBOColor(false, false);
	SetViewport(plan);
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableArrayTexture(0, *defren.GetDepthTexture1(), GetSamplerClampNearest());
	tsmgr.EnableArrayTexture(1, *defren.GetTextureDiffuse(), GetSamplerClampNearest());
	tsmgr.EnableArrayTexture(2, *defren.GetTextureSubSurface(), GetSamplerClampNearest());
	tsmgr.EnableArrayTexture(3, *defren.GetTextureTemporary2(), GetSamplerClampLinear());
	
	RenderFullScreenQuadVAO(plan);
}



void deoglRenderLight::CopyDepth1ToDepth3(deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "Light.CopyDepth1ToDepth3");
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	defren.GetDepthTexture3()->CopyFrom(*defren.GetDepthTexture1(), false);
	
	/*
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineCopyDepthStereo : *pPipelineCopyDepth;
	pipeline.Activate();
	defren.ActivateFBODepth3();
	
	deoglShaderCompiled &shader = pipeline.GetGlShader();
	
	renderThread.GetTexture().GetStages().EnableArrayTexture(0, *defren.GetDepthTexture1(), GetSamplerClampNearest());
	
	defren.SetShaderParamFSQuad(shader, spcdQuadParams);
	RenderFullScreenQuadVAO(plan);
	*/
}



void deoglRenderLight::ResetDebugInfo(){
	pDebugInfoSolid->Clear();
	pDebugInfoSolidCopyDepth->Clear();
	pDebugInfoSolidParticle->Clear();
	pDebugInfoSolidSSSSS->Clear();
	
	pDebugInfoTransparent->Clear();
	pDebugInfoTransparentCopyDepth->Clear();
	pDebugInfoTransparentSSSSS->Clear();
	
	pRenderLightSky->ResetDebugInfo();
	pRenderLightPoint->ResetDebugInfo();
	pRenderLightSpot->ResetDebugInfo();
	pRenderGI->ResetDebugInfo();
}

void deoglRenderLight::AddTopLevelDebugInfo(){
	deoglDebugInformationList &list = GetRenderThread().GetDebug().GetDebugInformationList();
	
	list.Add(pDebugInfoSolid);
	pRenderLightSky->AddTopLevelDebugInfoSolid();
	pRenderLightPoint->AddTopLevelDebugInfoSolid();
	pRenderLightSpot->AddTopLevelDebugInfoSolid();
	
	list.Add(pDebugInfoTransparent);
	pRenderLightSky->AddTopLevelDebugInfoTransparent();
	pRenderLightPoint->AddTopLevelDebugInfoTransparent();
	pRenderLightSpot->AddTopLevelDebugInfoTransparent();
	
	pRenderGI->AddTopLevelDebugInfo();
}

void deoglRenderLight::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = (details & deoglDeveloperMode::edimLight) == deoglDeveloperMode::edimLight;
	
	pDebugInfoSolid->SetVisible(show);
	pDebugInfoTransparent->SetVisible(show);
	
	pRenderLightSky->DevModeDebugInfoChanged();
	pRenderLightPoint->DevModeDebugInfoChanged();
	pRenderLightSpot->DevModeDebugInfoChanged();
	pRenderGI->DevModeDebugInfoChanged();
}



// Private Functions
//////////////////////

void deoglRenderLight::pCleanUp(){
	if(pRenderGI){
		delete pRenderGI;
	}
	if(pRenderLightParticles){
		delete pRenderLightParticles;
	}
	if(pRenderLightPoint){
		delete pRenderLightPoint;
	}
	if(pRenderLightSpot){
		delete pRenderLightSpot;
	}
	if(pRenderLightSky){
		delete pRenderLightSky;
	}
	
	if(pAddToRenderTask){
		delete pAddToRenderTask;
	}
	if(pRenderTask){
		delete pRenderTask;
	}
	
	deoglDebugInformationList &dilist = GetRenderThread().GetDebug().GetDebugInformationList();
	dilist.RemoveIfPresent(pDebugInfoSolid);
	dilist.RemoveIfPresent(pDebugInfoTransparent);
}
