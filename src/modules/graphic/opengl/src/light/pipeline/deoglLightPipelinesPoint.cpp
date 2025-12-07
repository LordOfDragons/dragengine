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

#include "deoglLightPipelinesPoint.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../light/deoglRLight.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/channel/deoglSkinChannel.h"

#include <dragengine/common/exceptions.h>


// Class deoglLightPipelinesPoint
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightPipelinesPoint::deoglLightPipelinesPoint(const deoglRLight &light) :
deoglLightPipelinesRanged(light){
}

deoglLightPipelinesPoint::~deoglLightPipelinesPoint(){
}



// Management
///////////////

const char *deoglLightPipelinesPoint::GetDebugName() const{
	return "deoglLightPipelinesPoint";
}



// Protected Functions
/////////////////////////

void deoglLightPipelinesPoint::pPreparePipelines(deoglBatchedShaderLoading &batched){
	// base pipeline configuration
	deoglPipelineConfiguration basePipelineConfig;
	pBasePipelineConfig(pLight.GetRenderThread(), basePipelineConfig);
	basePipelineConfig.EnableCulling(false);
	
	deoglPipelineConfiguration basePipelineConfigGI;
	pBasePipelineConfigGI(basePipelineConfigGI);
	
	// base light shader configuration
	deoglLightShaderConfig baseShaderConfig;
	baseShaderConfig.Reset();
	
	// create pipelines for each type and all valid modifications
	pPrepareNoShadow(basePipelineConfig, baseShaderConfig, batched);
	pPrepareSolid1(basePipelineConfig, baseShaderConfig, batched);
	pPrepareSolid1Transp1(basePipelineConfig, baseShaderConfig, batched);
	pPrepareSolid2(basePipelineConfig, baseShaderConfig, batched);
	pPrepareSolid2Transp1(basePipelineConfig, baseShaderConfig, batched);
	pPrepareSolid2Transp2(basePipelineConfig, baseShaderConfig, batched);
	// pPrepareLumSolid1( basePipelineConfig, baseShaderConfig, batched ); // unusued
	// pPrepareLumSolid2( basePipelineConfig, baseShaderConfig, batched ); // unusued
	
	pPrepareGIRayNoShadow(basePipelineConfigGI, baseShaderConfig, batched);
	pPrepareGIRaySolid1(basePipelineConfigGI, baseShaderConfig, batched);
	pPrepareGIRaySolid2(basePipelineConfigGI, baseShaderConfig, batched);
}



// Private Functions
//////////////////////

void deoglLightPipelinesPoint::pPrepareNoShadow(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseNoShadow(shaconf);
	pSetPoint(shaconf);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etNoShadow, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside, batched);
}

void deoglLightPipelinesPoint::pPrepareSolid1(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseSolid1(shaconf);
	pSetPoint(shaconf);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside, batched);
}

void deoglLightPipelinesPoint::pPrepareSolid1Transp1(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseSolid1Transp1(shaconf);
	pSetPoint(shaconf);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1Transp1, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside, batched);
}

void deoglLightPipelinesPoint::pPrepareSolid2(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseSolid2(shaconf);
	pSetPoint(shaconf);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid2, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside, batched);
}

void deoglLightPipelinesPoint::pPrepareSolid2Transp1(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseSolid2Transp1(shaconf);
	pSetPoint(shaconf);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid2Transp1, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside, batched);
}

void deoglLightPipelinesPoint::pPrepareSolid2Transp2(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseSolid2Transp2(shaconf);
	pSetPoint(shaconf);
	
	// TODO too many texture targets
// 	pCreatePipelines( pLight.GetRenderThread(), basePipelineConfig, shaconf,
// 		etSolid2Transp2, emNoAmbient | emStereo | emTransparent | emFlipCullFace | emCameraInside, batched );
}

void deoglLightPipelinesPoint::pPrepareLumSolid1(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseLumSolid1(shaconf);
	pSetPoint(shaconf);
	shaconf.SetShadowTapMode(deoglLightShaderConfig::estmSingle);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etLumSolid1, emNoAmbient | emTransparent, batched);
}

void deoglLightPipelinesPoint::pPrepareLumSolid2(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseLumSolid2(shaconf);
	pSetPoint(shaconf);
	shaconf.SetShadowTapMode(deoglLightShaderConfig::estmSingle);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf,
		etSolid1, emNoAmbient | emTransparent, batched);
}

void deoglLightPipelinesPoint::pPrepareGIRayNoShadow(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseGIRayNoShadow(shaconf);
	pSetPoint(shaconf);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRayNoShadow, 0, batched);
}

void deoglLightPipelinesPoint::pPrepareGIRaySolid1(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseGIRaySolid1(shaconf);
	pSetPoint(shaconf);
	shaconf.SetShadowTapMode(deoglLightShaderConfig::estmSingle);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid1, 0, batched);
}

void deoglLightPipelinesPoint::pPrepareGIRaySolid2(deoglPipelineConfiguration &basePipelineConfig,
deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched){
	deoglLightShaderConfig shaconf(baseShaderConfig);
	
	pSetBaseGIRaySolid2(shaconf);
	pSetPoint(shaconf);
	shaconf.SetShadowTapMode(deoglLightShaderConfig::estmSingle);
	
	pCreatePipelines(pLight.GetRenderThread(), basePipelineConfig, shaconf, etGIRaySolid2, 0, batched);
}



void deoglLightPipelinesPoint::pSetPoint(deoglLightShaderConfig &shaconf){
	deoglSkinTexture * const texture = pLight.GetUseSkinTexture();
	
	shaconf.SetLightMode(deoglLightShaderConfig::elmPoint);
	shaconf.SetShadowMappingAlgorithm1(deoglLightShaderConfig::esmaCube);
	shaconf.SetShadowMappingAlgorithm2(deoglLightShaderConfig::esmaCube);
	shaconf.SetHWDepthCompare(true);
	shaconf.SetDecodeInShadow(false);
	shaconf.SetShadowMatrix2EqualsMatrix1(true);
	
	shaconf.SetShadowTapMode(deoglLightShaderConfig::estmPcf9);
	//shaconf.SetShadowTapMode( deoglLightShaderConfig::estmPcfVariableTap );
	shaconf.SetTextureNoise(false);
	
	if(pLight.GetLightCanvas()){
		// right now canvas can not have depth. once it has light canvas can only be used
		// as cube map texture. since this is not possible right now equirect is used.
		shaconf.SetTextureColorOmnidirEquirect(true);
		
	}else if(texture){
		if(texture->IsChannelEnabled(deoglSkinChannel::ectColorOmnidirCube)){
			shaconf.SetTextureColorOmnidirCube(true);
			
		}else if(texture->IsChannelEnabled(deoglSkinChannel::ectColorOmnidirEquirect)){
			shaconf.SetTextureColorOmnidirEquirect(true);
			
		}else if(texture->GetMaterialPropertyAt(deoglSkinTexture::empColorOmnidirCube).GetRenderable() != -1){
			shaconf.SetTextureColorOmnidirCube(true);
			
		}else if(texture->GetMaterialPropertyAt(deoglSkinTexture::empColorOmnidirEquirect).GetRenderable() != -1){
			shaconf.SetTextureColorOmnidirEquirect(true);
		}
	}
}
