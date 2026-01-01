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

#include "cePlayback.h"
#include "cePlaybackActor.h"
#include "cePlaybackCamera.h"
#include "cePlaybackActionStack.h"
#include "cePlaybackActionStackEntry.h"
#include "cePlaybackProcessAction.h"
#include "cePlaybackEvaluateCondition.h"
#include "../ceConversation.h"
#include "../action/ceCACameraShot.h"
#include "../action/ceCAActorSpeak.h"
#include "../action/ceCASnippet.h"
#include "../action/ceCAStopConversation.h"
#include "../action/ceCAIfElse.h"
#include "../action/ceCAPlayerChoice.h"
#include "../action/ceCAPlayerChoiceOption.h"
#include "../action/ceConversationAction.h"
#include "../action/ceCASetVariable.h"
#include "../action/ceCASetActorParameter.h"
#include "../action/ceCAActorCommand.h"
#include "../action/ceCAGameCommand.h"
#include "../action/ceCAWait.h"
#include "../action/ceCATrigger.h"
#include "../action/ceCAActorAdd.h"
#include "../action/ceCAActorRemove.h"
#include "../action/ceCACoordSystemAdd.h"
#include "../action/ceCACoordSystemRemove.h"
#include "../action/ceCAIfElseCase.h"
#include "../actor/ceConversationActor.h"
#include "../actor/speechAnimation/ceSpeechAnimation.h"
#include "../actor/speechAnimation/ceSAWord.h"
#include "../actor/speechAnimation/ceSAPhoneme.h"
#include "../camerashot/ceCameraShot.h"
#include "../gesture/ceGesture.h"
#include "../facepose/ceFacePose.h"
#include "../file/ceConversationFile.h"
#include "../playerChoiceBox/cePlayerChoiceBox.h"
#include "../playerChoiceBox/cePCBOption.h"
#include "../target/ceTarget.h"
#include "../textbox/ceTextBox.h"
#include "../textbox/ceTextBoxText.h"
#include "../topic/ceConversationTopic.h"
#include "../infobox/ceConversationInfoBox.h"
#include "../strip/ceStrip.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSound.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>



// Class cePlaybackProcessAction
//////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackProcessAction::cePlaybackProcessAction() :
pMissingPhonemeLength(0.08f){
}

cePlaybackProcessAction::~cePlaybackProcessAction(){
}



// Management
///////////////

void cePlaybackProcessAction::ProcessAction(ceConversation &conversation, ceConversationAction *action){
	const int actionType = action->GetType();
	
	if(actionType == ceConversationAction::eatCameraShot){
		ProcessCameraShot(conversation, *((ceCACameraShot*)action));
		
	}else if(actionType == ceConversationAction::eatActorSpeak){
		ProcessActorSpeak(conversation, *((ceCAActorSpeak*)action));
		
	}else if(actionType == ceConversationAction::eatIfElse){
		ProcessIfElse(conversation, (ceCAIfElse*)action);
		
	}else if(actionType == ceConversationAction::eatPlayerChoice){
		ProcessPlayerChoice(conversation, (ceCAPlayerChoice*)action);
		
	}else if(actionType == ceConversationAction::eatStopConversation){
		ProcessStopConversation(conversation);
		
	}else if(actionType == ceConversationAction::eatStopTopic){
		ProcessStopTopic(conversation);
		
	}else if(actionType == ceConversationAction::eatSnippet){
		ProcessSnippet(conversation, (ceCASnippet*)action);
		
	}else if(actionType == ceConversationAction::eatSetVariable){
		ProcessSetVariable(conversation, *((ceCASetVariable*)action));
		
	}else if(actionType == ceConversationAction::eatSetActorParameter){
		ProcessSetActorParameter(conversation, *((ceCASetActorParameter*)action));
		
	}else if(actionType == ceConversationAction::eatActorCommand){
		ProcessActorCommand(conversation, *((ceCAActorCommand*)action));
		
	}else if(actionType == ceConversationAction::eatGameCommand){
		ProcessGameCommand(conversation, *((ceCAGameCommand*)action));
		
	}else if(actionType == ceConversationAction::eatWait){
		ProcessWait(conversation, (ceCAWait*)action);
		
	}else if(actionType == ceConversationAction::eatTrigger){
		ProcessTrigger(conversation, *((ceCATrigger*)action));
		
	}else if(actionType == ceConversationAction::eatActorAdd){
		ProcessActorAdd(conversation, *((ceCAActorAdd*)action));
		
	}else if(actionType == ceConversationAction::eatActorRemove){
		ProcessActorRemove(conversation, *((ceCAActorRemove*)action));
		
	}else if(actionType == ceConversationAction::eatCoordSystemAdd){
		ProcessCoordSystemAdd(conversation, *((ceCACoordSystemAdd*)action));
		
	}else if(actionType == ceConversationAction::eatCoordSystemRemove){
		ProcessCoordSystemRemove(conversation, *((ceCACoordSystemRemove*)action));
		
	}else if(actionType == ceConversationAction::eatComment){
		conversation.GetPlayback()->AdvanceToNextAction();
		
	}else{
		conversation.GetPlayback()->SetRunning(false);
	}
}



void cePlaybackProcessAction::ProcessCameraShot(ceConversation &conversation, const ceCACameraShot &action){
	ceCameraShot * const cameraShot = conversation.GetCameraShotNamed(action.GetName());
	cePlayback &playback = *conversation.GetPlayback();
	
	if(cameraShot){
		const int actorCount = playback.GetActorCount();
		cePlaybackCamera &camera = *playback.GetCamera();
		ceTarget *cameraTarget = nullptr;
		ceTarget *lookAtTarget = nullptr;
		int cameraActor = -1;
		int lookAtActor = -1;
		
		// determine targets
		if(!action.GetCameraTarget().IsEmpty()){
			cameraTarget = conversation.GetTargetNamed(action.GetCameraTarget());
			
			if(cameraTarget){
				cameraActor = conversation.GetActorList().IndexWithIDOrAliasID(cameraTarget->GetActor());
				if(cameraActor == -1){
					ErrorNoActorWithID(playback, cameraTarget->GetActor());
					return;
				}
				
			}else{
				ErrorTargetNotFound(playback, action.GetCameraTarget());
				return;
			}
		}
		
		if(!action.GetLookAtTarget().IsEmpty()){
			lookAtTarget = conversation.GetTargetNamed(action.GetLookAtTarget());
			
			if(lookAtTarget){
				lookAtActor = conversation.GetActorList().IndexWithIDOrAliasID(lookAtTarget->GetActor());
				if(lookAtActor == -1){
					ErrorNoActorWithID(playback, lookAtTarget->GetActor());
					return;
				}
				
			}else{
				ErrorTargetNotFound(playback, action.GetLookAtTarget());
				return;
			}
		}
		
		// set the coordinate system (temporary solution)
		const ceConversationActorList &list = conversation.GetActorList();
		
		if(actorCount == 1){
			camera.SetCoordSys1Actor(*list.GetAt(0));
			
		}else if(actorCount > 1){
			if(cameraShot->GetActorCount() > 1){
				camera.SetCoordSys2Actors(*list.GetAt(0), *list.GetAt(1));
				
			}else{
				camera.SetCoordSys1Actor(*list.GetAt(0));
			}
		}
		
		// set the other parameters straight into the camera
		camera.SetCameraTarget(cameraTarget);
		camera.SetLookAtTarget(lookAtTarget);
		camera.SetOffsetCameraFrom(cameraShot->GetOffsetCameraFrom());
		camera.SetOffsetCameraTo(cameraShot->GetOffsetCameraTo());
		camera.SetCameraOrbitFrom(cameraShot->GetCameraOrbitFrom());
		camera.SetCameraOrbitTo(cameraShot->GetCameraOrbitTo());
		camera.SetCameraDistanceFrom(cameraShot->GetCameraDistanceFrom());
		camera.SetCameraDistanceTo(cameraShot->GetCameraDistanceTo());
		camera.SetLookAtFrom(cameraShot->GetOffsetLookAtFrom());
		camera.SetLookAtTo(cameraShot->GetOffsetLookAtTo());
		camera.SetPositionFrom(cameraShot->GetPositionFrom());
		camera.SetPositionTo(cameraShot->GetPositionTo());
		camera.SetRotationFrom(cameraShot->GetRotationFrom());
		camera.SetRotationTo(cameraShot->GetRotationTo());
		camera.SetTiltFrom(cameraShot->GetTiltFrom());
		camera.SetTiltTo(cameraShot->GetTiltTo());
		camera.SetFovFrom(cameraShot->GetFovFrom());
		camera.SetFovTo(cameraShot->GetFovTo());
		camera.SetLockUpAxis(cameraShot->GetLockUpAxis());
		camera.SetAlignTargets(cameraShot->GetAlignTargets());
		camera.SetRelativeToLookAt(cameraShot->GetRelativeToLookAt());
		camera.SetLockCameraTarget(cameraShot->GetLockCameraTarget());
		camera.SetLockLookAtTarget(cameraShot->GetLockLookAtTarget());
		/*
		camera.SetParameterCurve(cePlaybackCamera::epPositionX, cameraShot->GetParameterCurve(ceCameraShot::epPositionX));
		camera.SetParameterCurve(cePlaybackCamera::epPositionY, cameraShot->GetParameterCurve(ceCameraShot::epPositionY));
		camera.SetParameterCurve(cePlaybackCamera::epPositionZ, cameraShot->GetParameterCurve(ceCameraShot::epPositionZ));
		camera.SetParameterCurve(cePlaybackCamera::epLookAtX, cameraShot->GetParameterCurve(ceCameraShot::epLookAtX));
		camera.SetParameterCurve(cePlaybackCamera::epLookAtY, cameraShot->GetParameterCurve(ceCameraShot::epLookAtY));
		camera.SetParameterCurve(cePlaybackCamera::epLookAtZ, cameraShot->GetParameterCurve(ceCameraShot::epLookAtZ));
		camera.SetParameterCurve(cePlaybackCamera::epTilt, cameraShot->GetParameterCurve(ceCameraShot::epTilt));
		camera.SetParameterCurve(cePlaybackCamera::epFov, cameraShot->GetParameterCurve(ceCameraShot::epFov));
		*/
		
		camera.SetDuration(action.GetDuration());
		camera.Rewind();
	}
	
	playback.AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessActorSpeak(ceConversation &conversation, ceCAActorSpeak &action){
	const bool useSpeechAnimation = action.GetUseSpeechAnimation();
	const ceStrip::List &words = action.GetWords();
	cePlayback &playback = *conversation.GetPlayback();
	const decString &actorID = action.GetActor();
	const int wordCount = words.GetCount();
	deSound *engSound = nullptr;
	
	float speechLength = action.GetMinSpeechTime();
	
	// determine the actor to use
	const int actorIndex = conversation.GetActorList().IndexWithIDOrAliasID(actorID);
	if(actorIndex == -1){
		ErrorNoActorWithID(playback, actorID);
		return;
	}
	
	ceConversationActor &conversationActor = *conversation.GetActorList().GetAt(actorIndex);
	cePlaybackActor &playbackActor = playback.GetActorAt(actorIndex);
	
	// determine the length of the sound file if existing
	engSound = action.GetEngineSound();
	
	if(engSound){
		conversationActor.PlayVoiceAudio(engSound);
		
		if(engSound->GetPlayTime() > speechLength){
			speechLength = engSound->GetPlayTime();
		}
	}
	
	// prepare the speech
	if(wordCount > 0){
		if(conversationActor.GetSpeechAnimation()){
			ceSpeechAnimation &speechAnimation = *conversationActor.GetSpeechAnimation();
			const ceSAPhoneme::Map &saPhonemeList = speechAnimation.GetPhonemes();
			int i, j, length2;
			ceSAWord *saWord;
			float scaling;
			
			speechAnimation.RemoveAllSpeakPhonemes();
			
			for(i=0; i<wordCount; i++){
				const ceStrip &word = *words.GetAt(i);
				const float pauseLen = word.GetPause();
				
				if(pauseLen > FLOAT_SAFE_EPSILON){
					// add pause. if the pause is added as single empty phoneme a long pause
					// leads to strange gaping motion. this is due to blending from phoneme
					// to phoneme. this is counteded by adding short empty phoneme at the
					// appropriate end to protect the long pause
					if(i == 0){
						if(pauseLen > 0.15f){
							// protection at end of pause
							speechAnimation.AddSpeakPhoneme(-1, pauseLen - 0.1f);
							speechAnimation.AddSpeakPhoneme(-1, 0.1f);
							
						}else{
							speechAnimation.AddSpeakPhoneme(-1, pauseLen);
						}
						
					}else if(i == wordCount - 1){
						if(pauseLen > 0.15f){
							// protection at beginning of pause
							speechAnimation.AddSpeakPhoneme(-1, 0.1f);
							speechAnimation.AddSpeakPhoneme(-1, pauseLen - 0.1f);
							
						}else{
							speechAnimation.AddSpeakPhoneme(-1, pauseLen);
						}
						
					}else{
						if(pauseLen > 0.25f){
							// protection at both ends of pause
							speechAnimation.AddSpeakPhoneme(-1, 0.1f);
							speechAnimation.AddSpeakPhoneme(-1, pauseLen - 0.2f);
							speechAnimation.AddSpeakPhoneme(-1, 0.1f);
							
						}else{
							speechAnimation.AddSpeakPhoneme(-1, pauseLen);
						}
					}
				}
				
				if(word.GetID().IsEmpty()){
					saWord = nullptr;
					
				}else{
					saWord = speechAnimation.GetWordList().FindOrDefault([&](const ceSAWord::Ref &w){
						return w->GetName() == word.GetID();
					});
				}
				
				if(saWord){
					const decUnicodeString &phonetics = saWord->GetPhonetics();
					
					length2 = phonetics.GetLength();
					
					scaling = 0.0f;
					for(j=0; j<length2; j++){
						const ceSAPhoneme::Ref *foundPhoneme = nullptr;
						if(saPhonemeList.GetAt(phonetics.GetAt(j), foundPhoneme)){
							scaling += (*foundPhoneme)->GetLength();
							
						}else{
							scaling += pMissingPhonemeLength;
						}
					}
					if(scaling > 1e-5f){
						scaling = word.GetDuration() / scaling;
					}
					
					for(j=0; j<length2; j++){
						const ceSAPhoneme::Ref *foundPhoneme = nullptr;
						if(saPhonemeList.GetAt(phonetics.GetAt(j), foundPhoneme)){
							speechAnimation.AddSpeakPhoneme((*foundPhoneme)->GetIPA(),
								(*foundPhoneme)->GetLength() * scaling);
							
						}else{
							speechAnimation.AddSpeakPhoneme(-1, pMissingPhonemeLength * scaling);
						}
					}
					
				}else{
					if(useSpeechAnimation && !word.GetID().IsEmpty()){
						playback.GetMissingWords().Add(word.GetID());
						conversation.NotifyPlaybackMissingWordsChanged();
					}
					
					speechAnimation.AddSpeakPhoneme(-1, word.GetDuration());
				}
			}
			
			if(speechAnimation.GetSpeakLength() + 0.1f > speechLength){
				speechLength = speechAnimation.GetSpeakLength() + 0.1f;
			}
			
			// remove speech animation if requested. this is done instead of avoiding adding the speech
			// animation in the first place since we need the speech animation to be set to know how
			// long the speaking takes. thus the actual speech animation is cleared afterwards
			if(!useSpeechAnimation){
				speechAnimation.RemoveAllSpeakPhonemes();
			}
			
		}else{
			float duration = 0.0f;
			int i;
			
			for(i=0; i<wordCount; i++){
				duration += words.GetAt(i)->GetPause() + words.GetAt(i)->GetDuration();
			}
			
			if(duration > speechLength){
				speechLength = duration;
			}
		}
	}
	
	// update the sub title
	const decUnicodeString &textBoxText = action.ResolveTextBoxText(conversation);
	if(textBoxText.GetLength() > 0){
		ceTextBoxText::Ref text;
		
		try{
			text = ceTextBoxText::Ref::New();
			text->SetName(conversationActor.GetTextBoxName());
			text->SetText(textBoxText);
			// TODO text style 
			playbackActor.SetTextBoxText(text);
		}catch(const deException &){
			throw;
		}
	}
	
	// update the gesture to play
	if(action.GetGestures().IsNotEmpty()){
		conversationActor.RemoveAllPlayGestures();
		action.GetGestures().Visit([&](const ceStrip &s){
			conversationActor.AddPlayGesture(conversation.GetGestureNamed(s.GetID()), s.GetPause(), s.GetDuration());
		});
	}
	
	// update the face pose to play
	if(action.GetFacePoses().IsNotEmpty()){
		conversationActor.RemoveAllPlayFacePoses();
		action.GetFacePoses().Visit([&](const ceStrip &s){
			conversationActor.AddPlayFacePose(conversation.GetFacePoseNamed(s.GetID()), s.GetPause(), s.GetDuration());
		});
	}
	
	// update the head look-at to play
	if(action.GetHeadLookAts().IsNotEmpty()){
		conversationActor.RemoveAllPlayHeadLookAts();
		action.GetHeadLookAts().Visit([&](const ceStrip &s){
			conversationActor.AddPlayHeadLookAt(conversation.GetTargetNamed(s.GetID()), s.GetPause(), s.GetDuration());
		});
	}
	
	// update the eyes look-at to play. uses head look-ats if empty
	const ceStrip::List &caEyesLAList = action.GetEyesLookAts().IsNotEmpty() ? action.GetEyesLookAts() : action.GetHeadLookAts();
	if(caEyesLAList.GetCount() > 0){
		conversationActor.RemoveAllPlayEyesLookAts();
		caEyesLAList.Visit([&](const ceStrip &s){
			conversationActor.AddPlayEyesLookAt(conversation.GetTargetNamed(s.GetID()), s.GetPause(), s.GetDuration());
		});
	}
	
	// set the speech length if used
	if(speechLength > 0.0f){
		playbackActor.SetSpeechLength(speechLength);
		playbackActor.SetElapsedTime(0.0f);
	}
	
	// advance to the next action
	playback.AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessSnippet(ceConversation &conversation, ceCASnippet *action){
	ceConversationTopic * const topic = conversation.GetTopicWithID(action->GetFile(), action->GetTopic());
	cePlayback &playback = *conversation.GetPlayback();
	
	if(topic){
		if(action->GetCreateSideLane()){
			const cePlaybackActionStack::Ref stack(cePlaybackActionStack::Ref::New());
			stack->Push(topic, action, &topic->GetActions(), 0);
			playback.AddSideActionStack(stack);
			playback.AdvanceToNextAction();
			
		}else{
			playback.GetMainActionStack()->Push(topic, action, &topic->GetActions(), 0);
		}
		
	}else{
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessStopConversation(ceConversation& conversation){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActiveActionStack();
	
	while(actionStack.GetCount() > 1){
		actionStack.Pop();
	}
	
	actionStack.GetTop().ForwardEnd();
	
	// show information
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	
	infobox.SetBackgroundColor(decColor(1.0f, 0.0f, 0.0f, 0.5f));
	infobox.SetText("Conversation Stop");
}

void cePlaybackProcessAction::ProcessStopTopic(ceConversation& conversation){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActiveActionStack();
	
	while(actionStack.GetCount() > 0 && !actionStack.GetTop().GetParentTopic()){
		actionStack.Pop();
	}
	
	if(actionStack.GetCount() == 0){
		return;
	}
	
	actionStack.GetTop().ForwardLast();
	playback.AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessIfElse(ceConversation &conversation, ceCAIfElse *action){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActiveActionStack();
	cePlaybackEvaluateCondition evalCondition;
	
	// if cases
	for(const auto &ifcase : action->GetCases()){
		const ceConversationCondition * const condition = ifcase->GetCondition();
		
		if(condition && evalCondition.EvaluateCondition(conversation, *condition)){
			if(ifcase->GetActions().IsNotEmpty()){
				actionStack.Push(nullptr, action, &ifcase->GetActions(), 0);
				
			}else{
				playback.AdvanceToNextAction();
			}
			
			return;
		}
	}
	
	// else case
	if(action->GetElseActions().IsNotEmpty()){
		actionStack.Push(nullptr, action, &action->GetElseActions(), 0);
		
	}else{
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessPlayerChoice(ceConversation &conversation, ceCAPlayerChoice *action){
	cePlayerChoiceBox &pcbox = conversation.GetPlayerChoiceBox();
	cePCBOption::List &pcbOptionList = pcbox.GetOptionList();
	cePlaybackEvaluateCondition evalCondition;
	
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActiveActionStack();
	actionStack.Push(nullptr, action, &action->GetActions(), 0);
	actionStack.GetTop().SetLooping(true);
	
	pcbox.SetPlaybackStackDepth(actionStack.GetCount());
	
	pcbOptionList.RemoveAll();
	
	action->GetOptions().Visit([&](ceCAPlayerChoiceOption *o){
		if(o->GetCondition() && !evalCondition.EvaluateCondition(conversation, *o->GetCondition())){
			return;
		}
		
		cePCBOption::Ref pcbOption(cePCBOption::Ref::New());
		pcbOption->SetText(o->GetText());
		pcbOption->SetActionOption(action, o);
		pcbOptionList.Add(pcbOption);
	});
	
	pcbox.UpdateCanvas();
	
	if(pcbOptionList.GetCount() == 0){
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessSetVariable(ceConversation &conversation, const ceCASetVariable &action){
	const decString &name = action.GetName();
	
	if(name.IsEmpty()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	cePlayback::VariableMap &variables = conversation.GetPlayback()->GetVariables();
	const ceCASetVariable::eOperators op = action.GetOperator();
	
	int value = action.GetValue();
	if(!action.GetValueVariable().IsEmpty()){
		value = variables.GetAtOrDefault(action.GetValueVariable(), value);
	}
	
	switch(op){
	case ceCASetVariable::eopSet:
		variables.SetAt(name, value);
		break;
		
	case ceCASetVariable::eopRandom:
		variables.SetAt(name, decMath::random(0, value));
		break;
		
	case ceCASetVariable::eopIncrement:
	case ceCASetVariable::eopDecrement:{
		int currentValue = variables.GetAtOrDefault(name, 0);
		
		if(op == ceCASetVariable::eopIncrement){
			variables.SetAt(name, currentValue + value);
			
		}else{
			variables.SetAt(name, currentValue - value);
		}
		}break;
		
	default:
		variables.SetAt(name, value);
	}
	
	conversation.NotifyPlaybackVarListChanged();
	conversation.GetPlayback()->AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessSetActorParameter(ceConversation &conversation,
const ceCASetActorParameter &action){
	cePlayback &playback = *conversation.GetPlayback();
	const decString &actorID = action.GetActor();
	
	ceConversationActor * const conversationActor = conversation.GetActorList().GetWithAliasID(actorID);
	const decString &name = action.GetName();
	if(name.IsEmpty() || !conversationActor){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationActor::ParameterMap &parameters = conversationActor->GetParameters();
	const ceCASetActorParameter::eOperators op = action.GetOperator();
	
	int value = action.GetValue();
	if(!action.GetValueVariable().IsEmpty()){
		value = playback.GetVariables().GetAtOrDefault(action.GetValueVariable(), value);
	}
	
	switch(op){
	case ceCASetActorParameter::eopSet:
		parameters.SetAt(name, value);
		break;
		
	case ceCASetActorParameter::eopRandom:
		parameters.SetAt(name, decMath::random(0, value));
		break;
		
	case ceCASetActorParameter::eopIncrement:
	case ceCASetActorParameter::eopDecrement:{
		int currentValue = parameters.GetAtOrDefault(name, 0);
		
		if(op == ceCASetActorParameter::eopIncrement){
			parameters.SetAt(name, currentValue + value);
			
		}else if(op == ceCASetActorParameter::eopDecrement){
			parameters.SetAt(name, currentValue - value);
			
		}
		}break;
		
	default:
		parameters.SetAt(name, value);
	}
	
	conversation.NotifyActorParametersChanged(conversationActor);
	conversation.GetPlayback()->AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessActorCommand(ceConversation &conversation, const ceCAActorCommand &action){
	if(conversation.GetPlayback()->GetAutoAdvanceCommands()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	const decString &actorID = action.GetActor();
	
	// show information
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format("Action Send Command to actor\n%s: %s", actorID.GetString(), action.GetCommand().GetString());
	
	infobox.SetBackgroundColor(decColor(1.0f, 1.0f, 0.0f, 0.5f));
	infobox.SetText(text);
}

void cePlaybackProcessAction::ProcessGameCommand(ceConversation &conversation, const ceCAGameCommand &action){
	if(conversation.GetPlayback()->GetAutoAdvanceCommands()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format("Action Send Command to game\n%s", action.GetCommand().GetString());
	
	infobox.SetBackgroundColor(decColor(1.0f, 1.0f, 0.0f, 0.5f));
	infobox.SetText(text);
}

void cePlaybackProcessAction::ProcessWait(ceConversation &conversation, ceCAWait *action){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActiveActionStack();
	
	if(action->GetCondition()){
		actionStack.Push(nullptr, action, &action->GetActions(), 0);
		actionStack.GetTop().SetLoopCondition(action->GetCondition());
		actionStack.GetTop().SetLooping(true);
		
	}else{
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessTrigger(ceConversation &conversation, const ceCATrigger &action){
	cePlayback &playback = *conversation.GetPlayback();
	igdeTriggerTargetList &triggerTable = playback.GetTriggerTable();
	igdeTriggerTarget &target = *triggerTable.GetNamedAddIfMissing(action.GetName());
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	if(action.GetAction() == ceCATrigger::eaFire){
		target.Fire();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format("Action Trigger firing trigger '%s'", action.GetName().GetString());
		
	}else if(action.GetAction() == ceCATrigger::eaReset){
		target.Reset();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format("Action Trigger resetting trigger '%s'", action.GetName().GetString());
		
	}else if(action.GetAction() == ceCATrigger::eaPulse){
		target.Fire();
		target.Reset();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format("Action Trigger pulsing trigger '%s'", action.GetName().GetString());
		
	}else{ // ceCATrigger::eaFullReset
		target.FullReset();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format("Action Trigger fully resetting trigger '%s'", action.GetName().GetString());
	}
	
	if(conversation.GetPlayback()->GetAutoAdvanceCommands()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	infobox.SetBackgroundColor(decColor(1.0f, 0.5f, 0.0f, 0.5f));
	infobox.SetText(text);
}

void cePlaybackProcessAction::ProcessActorAdd(ceConversation &conversation, const ceCAActorAdd &action){
	if(conversation.GetPlayback()->GetAutoAdvanceCommands()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	if(action.GetAliasID().IsEmpty()){
		text.Format("Action Add Actor '%s'", action.GetID().GetString());
		
	}else{
		text.Format("Action Add Actor '%s' with alias '%s'",
			action.GetID().GetString(), action.GetAliasID().GetString());
	}
	
	infobox.SetBackgroundColor(decColor(0.5f, 1.0f, 0.0f, 0.5f));
	infobox.SetText(text);
}

void cePlaybackProcessAction::ProcessActorRemove(ceConversation &conversation, const ceCAActorRemove &action){
	if(conversation.GetPlayback()->GetAutoAdvanceCommands()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format("Action Remove Actor '%s'", action.GetActor().GetString());
	
	infobox.SetBackgroundColor(decColor(0.5f, 1.0f, 0.0f, 0.5f));
	infobox.SetText(text);
}

void cePlaybackProcessAction::ProcessCoordSystemAdd(ceConversation &conversation, const ceCACoordSystemAdd &action){
	if(conversation.GetPlayback()->GetAutoAdvanceCommands()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	if(action.GetAliasID().IsEmpty()){
		text.Format("Action Add Coordinate System '%s'", action.GetCoordSystemID().GetString());
		
	}else{
		text.Format("Action Add Coordinate System '%s' with alias '%s'",
			action.GetCoordSystemID().GetString(), action.GetAliasID().GetString());
	}
	
	infobox.SetBackgroundColor(decColor(0.5f, 1.0f, 1.0f, 0.5f));
	infobox.SetText(text);
}

void cePlaybackProcessAction::ProcessCoordSystemRemove(ceConversation &conversation, const ceCACoordSystemRemove &action){
	if(conversation.GetPlayback()->GetAutoAdvanceCommands()){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format("Action Remove Coordinate System '%s'", action.GetCoordSystemID().GetString());
	
	infobox.SetBackgroundColor(decColor(0.5f, 1.0f, 1.0f, 0.5f));
	infobox.SetText(text);
}



int cePlaybackProcessAction::GetTargetActor(ceConversation &conversation, const decString &targetName){
	int actor = -1;
	
	if(!targetName.IsEmpty()){
		const ceTarget * const target = conversation.GetTargetNamed(targetName);
		cePlayback &playback = *conversation.GetPlayback();
		
		if(target){
			actor = conversation.GetActorList().IndexWithIDOrAliasID(target->GetActor());
			if(actor == -1){
				ErrorNoActorWithID(playback, target->GetActor());
			}
			
		}else{
			ErrorTargetNotFound(playback, targetName);
		}
	}
	
	return actor;
}



void cePlaybackProcessAction::ErrorNoActorWithID(cePlayback &playback, const char *id){
	decString text;
	
	text.Format("No actor with ID '%s' exists.", id);
	playback.SetTextBoxText(decUnicodeString::NewFromUTF8(text));
	
	playback.SetRunning(false);
}

void cePlaybackProcessAction::ErrorTargetNotFound(cePlayback &playback, const char *name){
	decString text;
	
	text.Format("No named target '%s' exists.", name);
	playback.SetTextBoxText(decUnicodeString::NewFromUTF8(text));
	
	playback.SetRunning(false);
}
