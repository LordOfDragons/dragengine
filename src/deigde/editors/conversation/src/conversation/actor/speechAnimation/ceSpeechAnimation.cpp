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
#include <stdlib.h>
#include <string.h>

#include "ceSpeechAnimation.h"
#include "ceSAWord.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>



// Class ceSpeechAnimation
////////////////////////////

// Constructor, destructor
////////////////////////////

ceSpeechAnimation::ceSpeechAnimation(deEngine *engine){
	if(!engine) DETHROW(deeInvalidParam);
	
	pEngine = engine;
	
	pEngAnimator = nullptr;
	pEngAnimatorInstance = nullptr;
	
	pSpeakLength = 0.0f;
	pSpeakPos = -1;
	pSpeakElapsed = 0.0f;
	pSpeaking = false;
	
	try{
		pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator(pEngAnimator);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

ceSpeechAnimation::~ceSpeechAnimation(){
	pCleanUp();
}



// Management
///////////////

void ceSpeechAnimation::SetNeutralMoveName(const char *name){
	pNeutralMoveName = name;
}

void ceSpeechAnimation::RemoveAllSpeakPhonemes(){
	pSpeakPhonemes.RemoveAll();
	pSpeakLength = 0.0f;
	pSpeakPos = -1;
	pSpeakElapsed = 0.0f;
	pSpeaking = false;
}

void ceSpeechAnimation::AddSpeakPhoneme(int ipa, float length){
	DEASSERT_TRUE(ipa >= -1)
	
	pSpeakPhonemes.Add(cSpeakPhoneme::Ref::New(ipa, length));
	
	pSpeakLength += length;
	pSpeakPos = -1;
	pSpeakElapsed = 0.0f;
	pSpeaking = true;
}



void ceSpeechAnimation::CreateAnimator(){
	// clear the engine controllers of all phonemes
	pPhonemes.Visit([](int, ceSAPhoneme &phoneme){
		phoneme.SetEngineController(-1);
	});
	
	// remove the animator from the instance just for safety
	pEngAnimatorInstance->SetAnimator(nullptr);
	
	// rebuild the animator. for each animation move used in all phonemes
	// one controller and one animation rule is added. these are considered
	// the used visemes of all phonemes. the controller is used to set the
	// blend strength of each viseme. The matching controllers are then
	// assigned to each phoneme. Multiple phonemes can therefore use the
	// same controller.
	pEngAnimator->RemoveAllRules();
	pEngAnimator->RemoveAllLinks();
	pEngAnimator->RemoveAllControllers();
	
	// use the existing animation state
	const deAnimatorRuleStateSnapshot::Ref ruleReset(deAnimatorRuleStateSnapshot::Ref::New());
	ruleReset->SetUseLastState(true);
	pEngAnimator->AddRule(ruleReset);
	
	// add rule for neutral vertex position sets
	if(pNeutralVertexPositionSets.GetCount() > 0){
		const deAnimatorRuleStateManipulator::Ref rule(deAnimatorRuleStateManipulator::Ref::New());
		rule->SetEnableRotation(false);
		rule->GetListVertexPositionSets() = pNeutralVertexPositionSets;
		pEngAnimator->AddRule(rule);
	}
	
	// add all unique visemes used by all phonemes
	pPhonemes.Visit([this](int, const ceSAPhoneme &phoneme){
		if(phoneme.GetEngineController() != -1){
			return;
		}
		
		const decString &moveName = phoneme.GetMoveName();
		
		// add a controller for the new viseme
		const int controllerIndex = pEngAnimator->GetControllerCount();
		deAnimatorController * const controller = new deAnimatorController;
		controller->SetName(moveName);
		pEngAnimator->AddController(controller);
		
		// add a link for the new viseme
		const int linkIndex = pEngAnimator->GetLinkCount();
		deAnimatorLink * const engLink = new deAnimatorLink;
		engLink->SetController(controllerIndex);
		pEngAnimator->AddLink(engLink);
		
		// add an animation rule for the new viseme
		if(!phoneme.GetVertexPositionSet().IsEmpty()){
			const deAnimatorRuleStateManipulator::Ref rule(deAnimatorRuleStateManipulator::Ref::New());
			rule->GetListVertexPositionSets().Add(phoneme.GetVertexPositionSet());
			rule->SetEnableRotation(false);
			rule->SetMaximumVertexPositionSet(1.0f);
			rule->GetTargetVertexPositionSet().AddLink(linkIndex);
			pEngAnimator->AddRule(rule);
			
		}else if(!phoneme.GetMoveName().IsEmpty()){
			const deAnimatorRuleAnimation::Ref rule(deAnimatorRuleAnimation::Ref::New());
			rule->SetMoveName(phoneme.GetMoveName());
			rule->SetBlendMode(deAnimatorRule::ebmOverlay);
			rule->GetTargetBlendFactor().AddLink(linkIndex);
			pEngAnimator->AddRule(rule);
		}
		
		// assign the controller to all phonemes using the same move name
		pPhonemes.Visit([&moveName, controllerIndex](int, ceSAPhoneme &phoneme2){
			if(phoneme2.GetMoveName() == moveName){
				phoneme2.SetEngineController(controllerIndex);
			}
		});
	});
	
	// reassign the animator
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
}

void ceSpeechAnimation::Update(float elapsed){
	if(pSpeaking){
		const int controllerCount = pEngAnimatorInstance->GetControllerCount();
		const float wordGapTime = 2.0f;
		const float windUpTime = 0.1f;
		float blendFactor = 1.0f;
		int controller1 = -1;
		int controller2 = -1;
		float phonemeLen;
		int i;
		
		// determine which phoneme to display and the blending
		pSpeakElapsed += elapsed;
		
		while(true){
			const ceSAPhoneme *phoneme1 = nullptr;
			const ceSAPhoneme *phoneme2 = nullptr;
			
			if(pSpeakPos >= 0 && pSpeakPos < pSpeakPhonemes.GetCount() && pSpeakPhonemes.GetAt(pSpeakPos)->ipa != -1){
				phoneme1 = pPhonemes.GetAtOrDefault(pSpeakPhonemes.GetAt(pSpeakPos)->ipa);
			}
			
			if(pSpeakPos + 1 < pSpeakPhonemes.GetCount() && pSpeakPhonemes.GetAt(pSpeakPos + 1)->ipa != -1){
				phoneme2 = pPhonemes.GetAtOrDefault(pSpeakPhonemes.GetAt(pSpeakPos + 1)->ipa);
			}
			
			if(pSpeakPos == -1){
				phonemeLen = windUpTime;
				
			}else if(pSpeakPos == pSpeakPhonemes.GetCount()){
				phonemeLen = wordGapTime;
				
			}else{
				phonemeLen = pSpeakPhonemes.GetAt(pSpeakPos)->length;
			}
			
			if(pSpeakElapsed < phonemeLen){
				//if( pSpeakElapsed < phonemeLen - windUpTime ){
				//	blendFactor = 1.0f;
				//	
				//}else{
					//blendFactor = ( phonemeLen - pSpeakElapsed ) / windUpTime;
					//blendFactor = 1.0f - pSpeakElapsed / phonemeLen;
					blendFactor = 0.5f + cosf((pSpeakElapsed / phonemeLen) * PI) * 0.5f;
				//}
				
				if(phoneme1){
					controller1 = phoneme1->GetEngineController();
				}
				
				if(phoneme2){
					controller2 = phoneme2->GetEngineController();
				}
				
				if(controller1 == controller2){
					controller2 = -1;
					blendFactor = 1.0f;
				}
				
				break;
			}
			
			if(pSpeakPos < pSpeakPhonemes.GetCount()){
				pSpeakPos++;
				
			}else{
				//pSpeakPos = -1;
				pSpeaking = false;
				controller1 = -1;
				controller2 = -1;
				break;
			}
			
			pSpeakElapsed -= phonemeLen;
		}
		
		// update the controllers
		if(controller1 != -1 && controller2 != -1){
			pEngAnimatorInstance->SetBlendFactor(1.0f);
			
		}else if(controller1 != -1){
			pEngAnimatorInstance->SetBlendFactor(blendFactor);
			blendFactor = 1.0f;
			
		}else if(controller2 != -1){
			pEngAnimatorInstance->SetBlendFactor(1.0f - blendFactor);
			blendFactor = 1.0f;
			
		}else{
			pEngAnimatorInstance->SetBlendFactor(0.0f);
		}
		
		for(i=0; i<controllerCount; i++){
			if(i == controller1){
				pEngAnimatorInstance->GetControllerAt(i).SetCurrentValue(blendFactor);
				
			}else if(i == controller2){
				pEngAnimatorInstance->GetControllerAt(i).SetCurrentValue(1.0f - blendFactor);
				
			}else{
				pEngAnimatorInstance->GetControllerAt(i).SetCurrentValue(0.0f);
			}
			
			pEngAnimatorInstance->NotifyControllerChangedAt(i);
		}
	}
}

void ceSpeechAnimation::Clear(){
	pEngAnimatorInstance->SetAnimator(nullptr);
	pEngAnimator->RemoveAllRules();
	pEngAnimator->RemoveAllLinks();
	pEngAnimator->RemoveAllControllers();
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
	
	RemoveAllSpeakPhonemes();
	pWords.RemoveAll();
	pPhonemes.RemoveAll();
	pNeutralMoveName = "";
	pNeutralVertexPositionSets.RemoveAll();
}



// Private Functions
//////////////////////

void ceSpeechAnimation::pCleanUp(){
	if(pEngAnimatorInstance){
		pEngAnimatorInstance->SetAnimator(nullptr);
		pEngAnimatorInstance->SetComponent(nullptr);
	}
	if(pEngAnimator){
		pEngAnimator->SetRig(nullptr);
	}
}
