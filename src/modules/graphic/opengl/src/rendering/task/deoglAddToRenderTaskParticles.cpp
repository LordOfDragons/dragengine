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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglAddToRenderTaskParticles.h"
#include "deoglRenderTaskParticles.h"
#include "deoglRenderTaskParticlesStep.h"
#include "../defren/deoglDeferredRendering.h"
#include "../task/shared/deoglRenderTaskSharedInstance.h"
#include "../task/shared/deoglRenderTaskSharedTexture.h"
#include "../task/shared/deoglRenderTaskSharedVAO.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../particle/deoglRParticleEmitterInstanceType.h"
#include "../../particle/deoglRParticleEmitterType.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/combinedTexture/deoglCombinedTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../skin/state/deoglSkinStateRenderable.h"
#include "../../target/deoglRenderTarget.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglAddToRenderTaskParticles
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglAddToRenderTaskParticles::deoglAddToRenderTaskParticles(
deoglRenderThread &renderThread, deoglRenderTaskParticles *renderTask) :
pRenderThread(renderThread),
pRenderTask(renderTask),
pSkinPipelineType(deoglSkinTexturePipelines::etGeometry),
pSkinPipelineModifier(0){
}

deoglAddToRenderTaskParticles::~deoglAddToRenderTaskParticles(){
}



// Management
///////////////

void deoglAddToRenderTaskParticles::SetSkinPipelineType(deoglSkinTexturePipelines::eTypes type){
	pSkinPipelineType = type;
}

void deoglAddToRenderTaskParticles::SetSkinPipelineModifier(int modifier){
	pSkinPipelineModifier = modifier;
}



void deoglAddToRenderTaskParticles::AddParticles(const deoglParticleEmitterInstanceList &list){
	const int count = list.GetCount();
	int e, p;
	
	for(e=0; e<count; e++){
		deoglRParticleEmitterInstance &instance = *list.GetAt(e);
		const deoglRParticleEmitterInstance::sParticle * const particles = instance.GetParticles();
		const int particleCount = instance.GetParticleCount();
		
		for(p=0; p<particleCount; p++){
			AddParticle(instance, particles + p);
		}
	}
}

void deoglAddToRenderTaskParticles::AddParticle(deoglRParticleEmitterInstance &emitterInstance,
const deoglRParticleEmitterInstance::sParticle *particle){
	// fetch parameters
	deoglRParticleEmitterInstanceType &itype = emitterInstance.GetTypeAt(particle->type);
	const deoglRParticleEmitterType &etype = emitterInstance.GetEmitter()->GetTypeAt(particle->type);
	const bool simulationRibbon = (etype.GetSimulationType() == deParticleEmitterType::estRibbon);
	const bool simulationBeam = (etype.GetSimulationType() == deParticleEmitterType::estBeam);
	
	const deoglRSkin * const skin = itype.GetUseSkin();
	const deoglRDynamicSkin * const dynamicSkin = itype.GetDynamicSkin();
	const deoglSkinTexture * const skinTexture = itype.GetUseSkinTexture();
	
	// if there is no texture we can skip the geometry attached to it
	if(!skinTexture){
		return;
	}
	
	// if the emitter is ribbon or beam type and the particle is the first skip it
	if((simulationRibbon || simulationBeam) && particle->ribbonLine[0] == 0
	&& particle->ribbonLine[1] == 0){
		return;
	}
	
	// retrieve the shader and texture units configuration to use
	const deoglSkinTexturePipeline &pipeline = itype.GetUseSkinPipelines().
		GetWithRef(pSkinPipelineType, pSkinPipelineModifier);
	
	const deoglTexUnitsConfig *tuc = itype.GetTUCForPipelineType (pSkinPipelineType);
	if(!tuc){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	// determine if the last step has the same texture
	const deoglVAO * const vao = emitterInstance.GetVAO();
	deoglRenderTaskParticlesStep *rtps = nullptr;
	
	if(pRenderTask->GetStepCount() > 0){
		rtps = pRenderTask->GetStepAt(pRenderTask->GetStepCount() - 1);
		
		if(rtps->GetVAO() != vao
		|| rtps->GetPipeline() != pipeline.GetPipeline()
		|| rtps->GetSkin() != skin
		|| rtps->GetDynamicSkin() != dynamicSkin){
			rtps = nullptr;
		}
	}
	
	// if there exists no matching step create a new one
	if(!rtps){
		rtps = pRenderTask->AddStep();
		rtps->SetPipeline(pipeline.GetPipeline());
		rtps->SetTUC(tuc);
		rtps->SetVAO(vao);
		rtps->SetSkin(skin);
		rtps->SetDynamicSkin(dynamicSkin);
		
		rtps->SetFirstIndex(emitterInstance.GetIBOUsedIndexCount());
		
		if(simulationRibbon || simulationBeam){
			rtps->SetPrimitiveType(GL_LINES_ADJACENCY);
			
		}else{
			rtps->SetPrimitiveType(GL_POINTS);
		}
		
		rtps->SetParameterBlockTexture(skinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock());
		rtps->SetParameterBlockInstance(itype.GetParamBlock());
	}
	
	// add an instance
	rtps->AddInstance().particle = particle;
	
	// update IBO
	if(simulationRibbon || simulationBeam){
		emitterInstance.AddIBOEntries(
			particle->particle + particle->ribbonLine[0],
			particle->particle + particle->ribbonLine[1],
			particle->particle,
			particle->particle + particle->ribbonLine[2]);
		rtps->IncrementIndexCount(4);
		
	}else{
		emitterInstance.AddIBOEntry(particle->particle);
		rtps->IncrementIndexCount(1);
	}
}
