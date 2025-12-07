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

#include "debpParticleEmitterType.h"
#include "../debpCommon.h"
#include "../debpSkin.h"
#include "../component/debpModel.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/skin/deSkin.h>



#ifndef random
#define random rand
#endif

// Class debpParticleEmitterType
//////////////////////////////////

// Constructor, destructor
////////////////////////////

debpParticleEmitterType::debpParticleEmitterType() :
pParamFactorLinVelo(1.0f),
pParamFactorAngVelo(1.0f)
{
	pEmitter = NULL,
	pType = 0;
	
	pSkin = NULL;
	pModel = NULL;
	pModelSkin = NULL;
	
	pParameterValueSamples = NULL;
	pParameterSpreadSamples = NULL;
	pParameterProgressSamples = NULL;
}

debpParticleEmitterType::~debpParticleEmitterType(){
	if(pParameterProgressSamples){
		delete [] pParameterProgressSamples;
	}
	if(pParameterSpreadSamples){
		delete [] pParameterSpreadSamples;
	}
	if(pParameterValueSamples){
		delete [] pParameterValueSamples;
	}
}



// Management
///////////////

void debpParticleEmitterType::SetEmitter(deParticleEmitter *emitter){
	pEmitter = emitter;
}

void debpParticleEmitterType::SetType(int type){
	pType = type;
}

void debpParticleEmitterType::SetParamFactorLinVelo(float factor){
	pParamFactorLinVelo = factor;
}

void debpParticleEmitterType::SetParamFactorAngVelo(float factor){
	pParamFactorAngVelo = factor;
}



void debpParticleEmitterType::UpdateType(){
	const deParticleEmitterType &type = pEmitter->GetTypeAt(pType);
	
	deModel *model;
	deSkin *skin;
	
	skin = type.GetSkin();
	if(skin){
		pSkin = (debpSkin*)skin->GetPeerPhysics();
		
	}else{
		pSkin = NULL;
	}
	
	model = type.GetModel();
	if(model){
		pModel = (debpModel*)model->GetPeerPhysics();
		
	}else{
		pModel = NULL;
	}
	
	skin = type.GetModelSkin();
	if(skin){
		pModelSkin = (debpSkin*)skin->GetPeerPhysics();
		
	}else{
		pModelSkin = NULL;
	}
}



void debpParticleEmitterType::UpdateParameters(){
	const deParticleEmitterType &type = pEmitter->GetTypeAt(pType);
	
	// create array if not existing yet. the array contain the sample values for
	// 11 parameter curves sampled at 256 positions. this is enough since the
	// particles do not life for long so precise sampling does not yield much
	// more accuracy than this pre-sampling does
	if(! pParameterValueSamples){
		pParameterValueSamples = new float[ESC_COUNT_CAST * 256]; // 21 curves: 5376 entries = 21504 bytes
	}
	if(! pParameterSpreadSamples){
		pParameterSpreadSamples = new float[ESC_COUNT_CAST * 256]; // 21 curves: 5376 entries = 21504 bytes
	}
	if(! pParameterProgressSamples){
		pParameterProgressSamples = new float[ESC_COUNT_PROGRESS * 256]; // 13 curves: 3328 entries = 13312 bytes
	}
	
	// sample the parameter curves into the array
	pSampleParameters(escParticleCount, type.GetParameter(deParticleEmitterType::epParticleCount));
	pSampleParameters(escMass, type.GetParameter(deParticleEmitterType::epMass));
	pSampleParameters(escRotation, type.GetParameter(deParticleEmitterType::epRotation));
	pSampleParameters(escLinearVelocity, type.GetParameter(deParticleEmitterType::epLinearVelocity));
	pSampleParameters(escAngularVelocity, type.GetParameter(deParticleEmitterType::epAngularVelocity));
	pSampleParameters(escBrown, type.GetParameter(deParticleEmitterType::epBrownMotion));
	pSampleParameters(escDamp, type.GetParameter(deParticleEmitterType::epDamping));
	pSampleParameters(escDrag, type.GetParameter(deParticleEmitterType::epDrag));
	pSampleParameters(escGravityX, type.GetParameter(deParticleEmitterType::epGravityX));
	pSampleParameters(escGravityY, type.GetParameter(deParticleEmitterType::epGravityY));
	pSampleParameters(escGravityZ, type.GetParameter(deParticleEmitterType::epGravityZ));
	pSampleParameters(escLocalGravity, type.GetParameter(deParticleEmitterType::epLocalGravity));
	pSampleParameters(escForceFieldDirect, type.GetParameter(deParticleEmitterType::epForceFieldDirect));
	pSampleParameters(escForceFieldSurface, type.GetParameter(deParticleEmitterType::epForceFieldSurface));
	pSampleParameters(escForceFieldVolume, type.GetParameter(deParticleEmitterType::epForceFieldMass));
	pSampleParameters(escForceFieldSpeed, type.GetParameter(deParticleEmitterType::epForceFieldSpeed));
	pSampleParameters(escElasticity, type.GetParameter(deParticleEmitterType::epElasticity));
	pSampleParameters(escRoughness, type.GetParameter(deParticleEmitterType::epRoughness));
	pSampleParameters(escEmitDirection, type.GetParameter(deParticleEmitterType::epEmitDirection));
	
	pSampleParameters(escTimeToLive, type.GetParameter(deParticleEmitterType::epTimeToLive));
	pSampleParameters(escInterval, type.GetParameter(deParticleEmitterType::epInterval));
	pSampleParameters(escCastAngleX, type.GetParameter(deParticleEmitterType::epCastAngleX));
	pSampleParameters(escCastAngleY, type.GetParameter(deParticleEmitterType::epCastAngleY));
	pSampleParameters(escSize, type.GetParameter(deParticleEmitterType::epSize));
	pSampleParameters(escRed, type.GetParameter(deParticleEmitterType::epRed));
	pSampleParameters(escGreen, type.GetParameter(deParticleEmitterType::epGreen));
	pSampleParameters(escBlue, type.GetParameter(deParticleEmitterType::epBlue));
	pSampleParameters(escTransparency, type.GetParameter(deParticleEmitterType::epTransparency));
	pSampleParameters(escEmissivity, type.GetParameter(deParticleEmitterType::epEmissivity));
}

float debpParticleEmitterType::EvaluateProgressParameter(int curve, float lifetime) const{
	const int sampleIndex = (int)(lifetime * 255.0f);
	
	if(sampleIndex < 0){
		return pParameterProgressSamples[curve * 256];
		
	}else if(sampleIndex > 255){
		return pParameterProgressSamples[curve * 256 + 255];
		
	}else{
		return pParameterProgressSamples[curve * 256 + sampleIndex];
	}
}

float debpParticleEmitterType::EvaluateCastParameter(const deParticleEmitterInstance &instance,
int curve, deParticleEmitterType::eParameters parameter) const{
	float value;
	
	value = EvaluateValueParameter(instance, curve, parameter);
	value += EvaluateSpreadParameter(instance, curve, parameter) * (((float)random() / (float)RAND_MAX) - 0.5f);
	
	return value;
}

float debpParticleEmitterType::EvaluateValueParameter(const deParticleEmitterInstance &instance,
int curve, deParticleEmitterType::eParameters parameter) const{
	const deParticleEmitterParameter &engParameter = pEmitter->GetTypeAt(pType).GetParameter(parameter);
	const int controllerIndex = engParameter.GetControllerValue();
	float value = engParameter.GetValue();
	
	if(controllerIndex >= 0 && controllerIndex < pEmitter->GetControllerCount()){
		const deParticleEmitterController &controller = instance.GetControllerAt(controllerIndex);
		const float lower = controller.GetLower();
		const float range = controller.GetUpper() - lower;
		float curveValue = 0.0f;
		int sampleIndex;
		
		if(range > 1e-5f){
			curveValue = (controller.GetValue() - lower) / range;
		}
		
		sampleIndex = (int)(curveValue * 255.0f);
		
		if(sampleIndex < 0){
			value *= pParameterValueSamples[curve * 256];
			
		}else if(sampleIndex > 255){
			value *= pParameterValueSamples[curve * 256 + 255];
			
		}else{
			value *= pParameterValueSamples[curve * 256 + sampleIndex];
		}
	}
	
	return value;
}

float debpParticleEmitterType::EvaluateSpreadParameter(const deParticleEmitterInstance &instance,
int curve, deParticleEmitterType::eParameters parameter) const{
	const deParticleEmitterParameter &engParameter = pEmitter->GetTypeAt(pType).GetParameter(parameter);
	const int controllerIndex = engParameter.GetControllerSpread();
	float spread = engParameter.GetSpread();
	
	if(controllerIndex >= 0 && controllerIndex < pEmitter->GetControllerCount()){
		const deParticleEmitterController &controller = instance.GetControllerAt(controllerIndex);
		const float lower = controller.GetLower();
		const float range = controller.GetUpper() - lower;
		float curveValue = 0.0f;
		int sampleIndex;
		
		if(range > 1e-5f){
			curveValue = (controller.GetValue() - lower) / range;
		}
		
		sampleIndex = (int)(curveValue * 255.0f);
		
		if(sampleIndex < 0){
			spread *= pParameterSpreadSamples[curve * 256];
			
		}else if(sampleIndex > 255){
			spread *= pParameterSpreadSamples[curve * 256 + 255];
			
		}else{
			spread *= pParameterSpreadSamples[curve * 256 + sampleIndex];
		}
	}
	
	return spread;
}



// Private Functions
//////////////////////

void debpParticleEmitterType::pSampleParameters(int curve, const deParticleEmitterParameter &parameter){
	float *spreadSamples = pParameterSpreadSamples + curve * 256;
	float *valueSamples = pParameterValueSamples + curve * 256;
	const float factor = 1.0f / 255.0f;
	int i;
	
	// sample value curve
	if(parameter.GetCurveValue().GetPointCount() == 0){ // actually not required since curve is not used in this case
		for(i=0; i<256; i++){
			valueSamples[i] = 1.0f;
		}
		
	}else{
		decCurveBezierEvaluator evalCurve(parameter.GetCurveValue());
		
		for(i=0; i<256; i++){
			//if( curve == escInterval && i < 10 )
			//	printf( "sample %i=%g(%g)\n", i, evalCurve.EvaluateAt( ( float )i * factor ), ( float )i * factor );
			valueSamples[i] = evalCurve.EvaluateAt((float)i * factor);
		}
	}
	
	// sample spread curve
	if(parameter.GetCurveSpread().GetPointCount() == 0){ // actually not required since curve is not used in this case
		for(i=0; i<256; i++){
			spreadSamples[i] = 1.0f;
		}
		
	}else{
		decCurveBezierEvaluator evalCurve(parameter.GetCurveSpread());
		
		for(i=0; i<256; i++){
			spreadSamples[i] = evalCurve.EvaluateAt((float)i * factor);
		}
	}
	
	// sample progress curve if required
	if(curve < ESC_COUNT_PROGRESS){
		float *progressSamples = pParameterProgressSamples + curve * 256;
		
		if(parameter.GetCurveProgress().GetPointCount() == 0){
			for(i=0; i<256; i++){
				progressSamples[i] = 1.0f;
			}
			
		}else{
			decCurveBezierEvaluator evalCurve(parameter.GetCurveProgress());
			const float factor = 1.0f / 255.0f;
			
			for(i=0; i<256; i++){
				progressSamples[i] = evalCurve.EvaluateAt((float)i * factor);
			}
		}
	}
}
