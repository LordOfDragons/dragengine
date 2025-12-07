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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRParticleEmitter.h"
#include "deoglRParticleEmitterType.h"
#include "../configuration/deoglConfiguration.h"
#include "../light/shader/deoglLightShaderConfig.h"
#include "../light/shader/deoglLightShader.h"
#include "../light/shader/deoglLightShaderManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../skin/deoglRSkin.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>



// Class deoglRParticleEmitterType
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRParticleEmitterType::deoglRParticleEmitterType(deoglRParticleEmitter &emitter) :
pEmitter(emitter),

pParameterSamples(NULL),

pTextureSamples(NULL),

pSkin(NULL),

pEmitLight(false)
{
	LEAK_CHECK_CREATE(emitter.GetRenderThread(), ParticleEmitterType);
}

deoglRParticleEmitterType::~deoglRParticleEmitterType(){
	LEAK_CHECK_FREE(pEmitter.GetRenderThread(), ParticleEmitterType);
	
	if(pParameterSamples){
		delete [] pParameterSamples;
	}
	
	if(pSkin){
		pSkin->FreeReference();
	}
	
	if(pTextureSamples){
		delete pTextureSamples;
	}
}



// Management
///////////////

void deoglRParticleEmitterType::SetSimulationType(deParticleEmitterType::eSimulationTypes simulationType){
	pSimulationType = simulationType;
}



void deoglRParticleEmitterType::UpdateParameterSamples(const deParticleEmitterType &type){
	pParamFactorLinVelo = type.GetMaxLinearVelocity() / 255.0f;
	pParamFactorAngVelo = type.GetMaxAngularVelocity() / 127.0f;
	
	// create array if not existing yet. the array contain the sample values for
	// 6 parameter curves sampled at 256 positions. this is enough since the particle
	// data given to us is contained in byte values so precise sampling does not
	// yield more data than this pre-sampling can yield
	if(! pParameterSamples){
		pParameterSamples = new float[ESC_COUNT * 256]; // 3'072 entries = 12'288 bytes
	}
	
	// sample the parameter curves into the array
	SampleParameters(escSizeProgress, escSizeBeam, type.GetParameter(deParticleEmitterType::epSize));
	SampleParameters(escRedProgress, escRedBeam, type.GetParameter(deParticleEmitterType::epRed));
	SampleParameters(escGreenProgress, escGreenBeam, type.GetParameter(deParticleEmitterType::epGreen));
	SampleParameters(escBlueProgress, escBlueBeam, type.GetParameter(deParticleEmitterType::epBlue));
	SampleParameters(escTransparencyProgress, escTransparencyBeam, type.GetParameter(deParticleEmitterType::epTransparency));
	SampleParameters(escEmissivityProgress, escEmissivityBeam, type.GetParameter(deParticleEmitterType::epEmissivity));
	
	// create samples texture
	const float * const samplesSizeProgress = pParameterSamples + escSizeProgress * 256;
	const float * const samplesRedProgress = pParameterSamples + escRedProgress * 256;
	const float * const samplesGreenProgress = pParameterSamples + escGreenProgress * 256;
	const float * const samplesBlueProgress = pParameterSamples + escBlueProgress * 256;
	const float * const samplesTranspProgress = pParameterSamples + escTransparencyProgress * 256;
	const float * const samplesEmissiveProgress = pParameterSamples + escEmissivityProgress * 256;
	
	const float * const samplesSizeBeam = pParameterSamples + escSizeBeam * 256;
	const float * const samplesRedBeam = pParameterSamples + escRedBeam * 256;
	const float * const samplesGreenBeam = pParameterSamples + escGreenBeam * 256;
	const float * const samplesBlueBeam = pParameterSamples + escBlueBeam * 256;
	const float * const samplesTranspBeam = pParameterSamples + escTransparencyBeam * 256;
	const float * const samplesEmissiveBeam = pParameterSamples + escEmissivityBeam * 256;
	
	pPixelBufferSamples.TakeOver(new deoglPixelBuffer(deoglPixelBuffer::epfFloat3, 256, 4, 1));
	deoglPixelBuffer::sFloat3 *pbdata = pPixelBufferSamples->GetPointerFloat3();
	int i;
	
	for(i=0; i<256; i++){
		pbdata->r = samplesRedProgress[i];
		pbdata->g = samplesGreenProgress[i];
		pbdata->b = samplesBlueProgress[i];
		pbdata++;
	}
	for(i=0; i<256; i++){
		pbdata->r = samplesTranspProgress[i];
		pbdata->g = samplesEmissiveProgress[i];
		pbdata->b = samplesSizeProgress[i];
		pbdata++;
	}
	
	for(i=0; i<256; i++){
		pbdata->r = samplesRedBeam[i];
		pbdata->g = samplesGreenBeam[i];
		pbdata->b = samplesBlueBeam[i];
		pbdata++;
	}
	for(i=0; i<256; i++){
		pbdata->r = samplesTranspBeam[i];
		pbdata->g = samplesEmissiveBeam[i];
		pbdata->b = samplesSizeBeam[i];
		pbdata++;
	}
	
	if(! pTextureSamples){
		pTextureSamples = new deoglTexture(pEmitter.GetRenderThread());
		pTextureSamples->SetSize(256, 4);
		pTextureSamples->SetMapingFormat(3, true, false);
		pTextureSamples->SetMipMapped(false);
	}
	
	// update has transparency value
	const deParticleEmitterParameter &paramTransp =
		type.GetParameter(deParticleEmitterType::epTransparency);
	
	pHasTransparency = fabsf(1.0f - paramTransp.GetValue()) > 0.001f
		|| fabsf(paramTransp.GetSpread()) > 0.001f
		|| paramTransp.GetCurveValue().GetPointCount() > 0
		|| paramTransp.GetCurveSpread().GetPointCount() > 0
		|| paramTransp.GetCurveProgress().GetPointCount() > 0
		|| paramTransp.GetCurveBeam().GetPointCount() > 0;
}

void deoglRParticleEmitterType::SampleParameters(eSampleCurves curveProgress,
eSampleCurves curveBeam, const deParticleEmitterParameter& parameter){
	float * const samplesProgress = pParameterSamples + curveProgress * 256;
	float * const samplesBeam = pParameterSamples + curveBeam * 256;
	const float factor = 1.0f / 255.0f;
	int i;
	
	// cast and spread parameter in case no float inputs are used
	pParamFactorMultiply[curveProgress] = parameter.GetSpread() * 2.0f * factor;
	pParamFactorAdd[curveProgress] = parameter.GetValue() - parameter.GetSpread();
	
	// progress curve
	if(parameter.GetCurveProgress().GetPointCount() == 0){
		for(i=0; i<256; i++){
			samplesProgress[i] = 1.0f; // this way the cast value is used
		}
		
	}else{
		decCurveBezierEvaluator evalCurve(parameter.GetCurveProgress());
		
		for(i=0; i<256; i++){
			samplesProgress[i] = evalCurve.EvaluateAt((float)i * factor);
			
			// prevent values less than 0 since this causes problems with opengl
			if(samplesProgress[i] < 0.0f){
				samplesProgress[i] = 0.0f;
			}
		}
	}
	
	// beam curve
	if(parameter.GetCurveBeam().GetPointCount() == 0){
		for(i=0; i<256; i++){
			samplesBeam[i] = 1.0f; // this way the cast value is used
		}
		
	}else{
		decCurveBezierEvaluator evalCurve(parameter.GetCurveBeam());
		
		for(i=0; i<256; i++){
			samplesBeam[i] = evalCurve.EvaluateAt((float)i * factor);
			
			// prevent values less than 0 since this causes problems with opengl
			if(samplesBeam[i] < 0.0f){
				samplesBeam[i] = 0.0f;
			}
		}
	}
}

float deoglRParticleEmitterType::GetSampledParameter(eSampleCurves curve, float location) const{
	if(curve < 0 || curve >= ESC_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	const float indexLocation = location * 255.0f;
	
	if(indexLocation <= 0.0f){
		return pParameterSamples[curve * 256];
		
	}else if(indexLocation >= 255.0f){
		return pParameterSamples[curve * 256 + 255];
		
	}else{
		float indexLocationIntegral;
		float indexLocationFractional = modff(indexLocation, &indexLocationIntegral);
		const int baseLocation = curve * 256 + (int)indexLocationIntegral;
		const float value1 = pParameterSamples[baseLocation];
		const float value2 = pParameterSamples[baseLocation + 1];
		
		return value1 * (1.0f - indexLocationFractional) + value2 * indexLocationFractional;
	}
}

void deoglRParticleEmitterType::PrepareForRender(){
	if(pPixelBufferSamples){
		if(pTextureSamples){
			pTextureSamples->SetPixels(pPixelBufferSamples);
		}
		pPixelBufferSamples = nullptr;
	}
}



void deoglRParticleEmitterType::SetSkin(deoglRSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	if(pSkin){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if(skin){
		skin->AddReference();
	}
}

void deoglRParticleEmitterType::CheckEmitLight(const deParticleEmitterType &type){
	const deParticleEmitterParameter &emissivity = type.GetParameter(deParticleEmitterType::epEmissivity);
	
	pEmitLight = false;
	
	if(emissivity.GetValue() > 1e-5f || emissivity.GetSpread() > 1e-5f){
		if(pSkin){
			pEmitLight = true;
		}
	}
}



deoglLightPipelines &deoglRParticleEmitterType::GetPipelines(){
	if(pPipelines){
		return pPipelines;
	}
	
	pPipelines.TakeOver(new deoglLightPipelinesParticle(*this));
	
	deoglBatchedShaderLoading batched(pEmitter.GetRenderThread(), 1000.0f, true);
	try{
		pPipelines->Prepare(batched);
		
	}catch(const deException &){
		batched.WaitAllCompileFinished();
		throw;
	}
	
	batched.WaitAllCompileFinished();
	return pPipelines;
}

deoglSPBlockUBO &deoglRParticleEmitterType::GetLightParameterBlock(){
	if(! pParamBlockLight){
		pParamBlockLight = GetPipelines().GetWithRef(deoglLightPipelines::etNoShadow, 0)
			.GetShader()->CreateSPBLightParam();
	}
	return pParamBlockLight;
}

void deoglRParticleEmitterType::DropPipelines(){
	pParamBlockLight = nullptr;
	pPipelines = nullptr;
}
