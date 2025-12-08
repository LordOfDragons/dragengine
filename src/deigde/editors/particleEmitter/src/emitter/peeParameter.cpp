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

#include "peeType.h"
#include "peeEmitter.h"
#include "peeParameter.h"
#include "peeController.h"

#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Class peeParameter
///////////////////////

// Constructor, destructor
////////////////////////////

peeParameter::peeParameter(peeType &type, deParticleEmitterType::eParameters index,
	float value, float spread) :
pType(type),
pIndex(index),
pValue(value),
pSpread(spread),
pActive(false){
}

peeParameter::~peeParameter(){
}



// Management
///////////////

void peeParameter::SetValue(float value){
	if(fabsf(value - pValue) > 1e-5f){
		pValue = value;
		
		if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
			
			engType.GetParameter(pIndex).SetValue(value);
			engEmitter.NotifyTypeChangedAt(pType.GetIndex());
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetSpread(float spread){
	if(fabsf(spread - pSpread) > 1e-5f){
		pSpread = spread;
		
		if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
			
			engType.GetParameter(pIndex).SetSpread(spread);
			engEmitter.NotifyTypeChangedAt(pType.GetIndex());
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetControllerValue(peeController::Ref controller){
	if(controller != pControllerValue){
		pControllerValue = controller;
		if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
			
			engType.GetParameter(pIndex).SetControllerValue(
				pType.GetEmitter()->GetControllers().IndexOf(controller));
			engEmitter.NotifyTypeChangedAt(pType.GetIndex());
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetCurveValue(const decCurveBezier &curve){
	pCurveValue = curve;
	
	if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
		
		engType.GetParameter(pIndex).GetCurveValue() = curve;
		engEmitter.NotifyTypeChangedAt(pType.GetIndex());
	}
	
	NotifyChanged();
}

void peeParameter::SetControllerSpread(peeController::Ref controller){
	if(controller != pControllerSpread){
		pControllerSpread = controller;
		if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
			
			engType.GetParameter(pIndex).SetControllerSpread(
				pType.GetEmitter()->GetControllers().IndexOf(controller));
			engEmitter.NotifyTypeChangedAt(pType.GetIndex());
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetCurveSpread(const decCurveBezier &curve){
	pCurveSpread = curve;
	
	if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
		
		engType.GetParameter(pIndex).GetCurveSpread() = curve;
		engEmitter.NotifyTypeChangedAt(pType.GetIndex());
	}
	
	NotifyChanged();
}

void peeParameter::SetCurveProgress(const decCurveBezier &curve){
	pCurveProgress = curve;
	
	if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
		
		engType.GetParameter(pIndex).GetCurveProgress() = curve;
		engEmitter.NotifyTypeChangedAt(pType.GetIndex());
	}
	
	NotifyChanged();
}

void peeParameter::SetCurveBeam(const decCurveBezier &curve){
	pCurveBeam = curve;
	
	if(pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter()){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt(pType.GetIndex());
		
		engType.GetParameter(pIndex).GetCurveBeam() = curve;
		engEmitter.NotifyTypeChangedAt(pType.GetIndex());
	}
	
	NotifyChanged();
}



void peeParameter::SetActive(bool active){
	pActive = active;
}



void peeParameter::UpdateEngineParameter(deParticleEmitterParameter &parameter){
	const peeControllerList &list = pType.GetEmitter()->GetControllers();
	parameter.SetValue(pValue);
	parameter.SetSpread(pSpread);
	parameter.GetCurveValue() = pCurveValue;
	parameter.GetCurveSpread() = pCurveSpread;
	parameter.GetCurveProgress() = pCurveProgress;
	parameter.GetCurveBeam() = pCurveBeam;
	parameter.SetControllerValue(list.IndexOf(pControllerValue));
	parameter.SetControllerSpread(list.IndexOf(pControllerSpread));
}



void peeParameter::NotifyChanged(){
	if(pType.GetEmitter()){
		pType.GetEmitter()->NotifyTypeParameterChanged(&pType, this);
	}
}
