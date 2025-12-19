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
#include "cePlaybackActionStackEntry.h"
#include "cePlaybackProcessAction.h"
#include "cePlaybackEvaluateCondition.h"
#include "../ceConversation.h"
#include "../action/ceConversationAction.h"
#include "../actor/ceConversationActor.h"
#include "../actor/speechAnimation/ceSpeechAnimation.h"
#include "../actor/speechAnimation/ceSAWord.h"
#include "../actor/speechAnimation/ceSAPhoneme.h"
#include "../camerashot/ceCameraShot.h"
#include "../gesture/ceGesture.h"
#include "../facepose/ceFacePose.h"
#include "../file/ceConversationFile.h"
#include "../playerChoiceBox/cePlayerChoiceBox.h"
#include "../textbox/ceTextBox.h"
#include "../textbox/ceTextBoxText.h"
#include "../topic/ceConversationTopic.h"
#include "../infobox/ceConversationInfoBox.h"
#include "../condition/ceConversationCondition.h"
#include "../strip/ceStrip.h"

#include <deigde/gui/igdeCamera.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSound.h>



// Class cePlayback
/////////////////////

// Constructor, destructor
////////////////////////////

cePlayback::cePlayback(ceConversation &conversation) :
pConversation(conversation),
pRunning(false),
pPaused(false),
pAutoAdvanceCommands(true),
pCameraHandling(echFree),
pCamera(cePlaybackCamera::Ref::New(*this)),
pMainActionStack(cePlaybackActionStack::Ref::New()),
pActiveActionStack(pMainActionStack){
}

cePlayback::~cePlayback(){
	ClearTextBoxText();
}



// Management
///////////////

void cePlayback::SetTopic(ceConversationTopic *topic){
	if(topic != pTopic){
		pTopic = topic;
		Rewind();
		
		pConversation.NotifyPlaybackChanged();
	}
}

void cePlayback::SetRunning(bool running){
	if(running == pRunning){
		return;
	}
	
	pRunning = running;
	
	if(running){
		//SetCameraHandling( echConversation );
		pConversation.GetInfoBox()->Clear();
		ClearTextBoxText();
		//ResetCamera();
		
	}else{
		pTestActionList.RemoveAll();
		
		pConversation.GetInfoBox()->SetBackgroundColor(decColor(1.0f, 0.0f, 0.0f, 0.5f));
		pConversation.GetInfoBox()->SetText("Conversation Finished");
	}
	
	pConversation.NotifyPlaybackChanged();
}

void cePlayback::SetPaused(bool paused){
	if(paused == pPaused){
		return;
	}
	
	pPaused = paused;
	pConversation.NotifyPlaybackChanged();
}

void cePlayback::SetCameraHandling(eCameraHandling handling){
	pCameraHandling = handling;
}

void cePlayback::SetAutoAdvanceCommands(bool autoAdvance){
	if(autoAdvance == pAutoAdvanceCommands){
		return;
	}
	
	pAutoAdvanceCommands = autoAdvance;
	pConversation.NotifyPlaybackChanged();
}

void cePlayback::AddSideActionStack(const cePlaybackActionStack::Ref &stack){
	pSideActionStacks.Add(stack);
	const cePlaybackActionStack::Ref restoreStack(pActiveActionStack);
	pActiveActionStack = stack;
	pProcessActions(0.0f);
	pActiveActionStack = restoreStack;
}

void cePlayback::Rewind(){
	pSideActionStacks.RemoveAll();
	pMainActionStack->Clear();
	pActiveActionStack = pMainActionStack;
	SetLastPlayedAction(nullptr, nullptr);
	
	pActors.Visit([](cePlaybackActor &actor){
		actor.Reset();
	});
	
	pConversation.GetInfoBox()->Clear();
	pConversation.GetPlayerChoiceBox()->Clear();
	ClearTextBoxText();
	//ResetCamera();
	
	AdvanceToNextAction();
}

void cePlayback::ResetCamera(){
	pCamera->Reset();
	
	if(pActors.GetCount() == 1){
		pCamera->SetCoordSys1Actor(*pConversation.GetActorList().GetAt(0));
		pCamera->SetOffsetCameraFrom(decVector(0.0f, 1.65f, 0.0f));
		pCamera->SetCameraDistanceFrom(0.5f);
		/*
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionX, 0.0f);
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionY, 1.65f);
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionZ, -0.5f);
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtX, 0.0f);
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtY, 1.65f);
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtZ, 0.0f);
		*/
		
	}else if(pActors.GetCount() > 1){
		const ceConversationActorList &list = pConversation.GetActorList();
		ceConversationActor &actor1 = *list.GetAt(0);
		ceConversationActor &actor2 = *list.GetAt(1);
		float distance = (actor1.GetPosition() - actor2.GetPosition()).Length();
		
		pCamera->SetCoordSys2Actors(actor1, actor2);
		pCamera->SetOffsetCameraFrom(decVector(0.0f, 1.5f, 0.0f));
		pCamera->SetCameraOrbitFrom(decVector(0.0f, -90.0f, 0.0f));
		pCamera->SetCameraDistanceFrom(distance * 1.2f);
		/*
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionX, distance * -1.2f);
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionY, 1.5f);
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionZ, 0.0f);
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtX, 0.0f);
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtY, 1.65f);
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtZ, 0.0f);
		*/
	}
	
	pConversation.NotifyCameraChanged();
}

void cePlayback::PlaySingleAction(ceConversationAction *action, float time){
	Rewind();
	SetRunning(false);
	SetPaused(false);
	
	pTestActionList.RemoveAll();
	
	if(action){//&& action->GetType() == ceConversationAction::eatActorSpeak){
		pTestActionList.Add(action);
		
		if(action->GetType() != ceConversationAction::eatActorSpeak){
			time = 0.0f;
		}
		
		//time += action->GetDelay();
	}
	
	if(pTestActionList.IsNotEmpty()){
		const float timeStep = 0.1f;
		
		AdvanceToNextAction();
		SetRunning(true);
		pConversation.Update(0.0f);
		
		while(time > 1e-5f){
			if(time > timeStep){
				pConversation.Update(timeStep);
				
			}else{
				pConversation.Update(time);
			}
			
			time -= timeStep;
		}
	}
}



int cePlayback::GetActorCount() const{
	return pActors.GetCount();
}

void cePlayback::SetActorCount(int count){
	if(count == pActors.GetCount()){
		return;
	}
	
	pActors.RemoveAll();
	for(int i=0; i<count; i++){
		pActors.Add(cePlaybackActor::Ref::New());
		pActors.GetAt(i)->SetTextBox(pConversation.GetTextBox());
	}
}

cePlaybackActor &cePlayback::GetActorAt(int index) const{
	return pActors.GetAt(index);
}



void cePlayback::Update(float elapsed){
	if((pTopic || pTestActionList.GetCount() > 0) && pRunning && !pPaused){
		pActors.Visit([&](cePlaybackActor &a){
			a.Update(elapsed);
		});
		ProcessActions(elapsed);
	}
	
	UpdateCamera(elapsed);
}

void cePlayback::UpdateCamera(float elapsed){
	switch(pCameraHandling){
	case echConversation:
		pCamera->Update(*pConversation.GetCamera(), elapsed, true);
		pConversation.NotifyCameraChanged();
		break;
		
	case echCameraShot:{
		if(!pConversation.GetActiveCameraShot()){
			break;
		}
		
		const ceCameraShot &cameraShot = *pConversation.GetActiveCameraShot();
		const ceConversationActorList &list = pConversation.GetActorList();
		
		if(pActors.GetCount() == 1){
			pCamera->SetCoordSys1Actor(*list.GetAt(0));
			
		}else if(pActors.GetCount() > 1){
			if(cameraShot.GetActorCount() > 1){
				pCamera->SetCoordSys2Actors(*list.GetAt(0), *list.GetAt(1));
				
			}else{
				pCamera->SetCoordSys1Actor(*list.GetAt(0));
			}
		}
		
		// set the other parameters straight into the camera
		if(cameraShot.GetCameraTarget().IsEmpty()){
			pCamera->SetCameraTarget(nullptr);
			
		}else{
			pCamera->SetCameraTarget(pConversation.GetTargetNamed(cameraShot.GetCameraTarget()));
		}
		
		if(cameraShot.GetLookAtTarget().IsEmpty()){
			pCamera->SetLookAtTarget(nullptr);
			
		}else{
			pCamera->SetLookAtTarget(pConversation.GetTargetNamed(cameraShot.GetLookAtTarget()));
		}
		
		pCamera->SetOffsetCameraFrom(cameraShot.GetOffsetCameraFrom());
		pCamera->SetOffsetCameraTo(cameraShot.GetOffsetCameraTo());
		pCamera->SetCameraOrbitFrom(cameraShot.GetCameraOrbitFrom());
		pCamera->SetCameraOrbitTo(cameraShot.GetCameraOrbitTo());
		pCamera->SetCameraDistanceFrom(cameraShot.GetCameraDistanceFrom());
		pCamera->SetCameraDistanceTo(cameraShot.GetCameraDistanceTo());
		pCamera->SetLookAtFrom(cameraShot.GetOffsetLookAtFrom());
		pCamera->SetLookAtTo(cameraShot.GetOffsetLookAtTo());
		pCamera->SetPositionFrom(cameraShot.GetPositionFrom());
		pCamera->SetPositionTo(cameraShot.GetPositionTo());
		pCamera->SetRotationFrom(cameraShot.GetRotationFrom());
		pCamera->SetRotationTo(cameraShot.GetRotationTo());
		pCamera->SetTiltFrom(cameraShot.GetTiltFrom());
		pCamera->SetTiltTo(cameraShot.GetTiltTo());
		pCamera->SetFovFrom(cameraShot.GetFovFrom());
		pCamera->SetFovTo(cameraShot.GetFovTo());
		pCamera->SetLockUpAxis(cameraShot.GetLockUpAxis());
		pCamera->SetAlignTargets(cameraShot.GetAlignTargets());
		pCamera->SetRelativeToLookAt(cameraShot.GetRelativeToLookAt());
		pCamera->SetLockCameraTarget(cameraShot.GetLockCameraTarget());
		pCamera->SetLockLookAtTarget(cameraShot.GetLockLookAtTarget());
		/*
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionX, cameraShot.GetParameterCurve(ceCameraShot::epPositionX));
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionY, cameraShot.GetParameterCurve(ceCameraShot::epPositionY));
		pCamera->SetParameterCurve(cePlaybackCamera::epPositionZ, cameraShot.GetParameterCurve(ceCameraShot::epPositionZ));
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtX, cameraShot.GetParameterCurve(ceCameraShot::epLookAtX));
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtY, cameraShot.GetParameterCurve(ceCameraShot::epLookAtY));
		pCamera->SetParameterCurve(cePlaybackCamera::epLookAtZ, cameraShot.GetParameterCurve(ceCameraShot::epLookAtZ));
		pCamera->SetParameterCurve(cePlaybackCamera::epTilt, cameraShot.GetParameterCurve(ceCameraShot::epTilt));
		pCamera->SetParameterCurve(cePlaybackCamera::epFov, cameraShot.GetParameterCurve(ceCameraShot::epFov));
		*/
		
		pCamera->SetDuration(3.0f);
		pCamera->Update(*pConversation.GetCamera(), elapsed, false);
		pConversation.NotifyCameraChanged();
		}break;
		
	case echFree:{
		igdeCamera &in = *pConversation.GetCameraFree();
		igdeCamera &out = *pConversation.GetCamera();
		out.SetPosition(in.GetPosition());
		out.SetOrientation(in.GetOrientation());
		out.SetFov(in.GetFov());
		out.SetFovRatio(in.GetFovRatio());
		out.SetImageDistance(in.GetImageDistance());
		out.SetViewDistance(in.GetViewDistance());
		out.SetExposure(in.GetExposure());
		out.SetLowestIntensity(in.GetLowestIntensity());
		out.SetHighestIntensity(in.GetHighestIntensity());
		out.SetAdaptionTime(in.GetAdaptionTime());
		out.SetDistance(in.GetDistance());
		}break;
	}
}

void cePlayback::ProcessActions(float elapsed){
	cePlaybackActionStack::List(pSideActionStacks).Visit([&](const cePlaybackActionStack::Ref &s){
		pActiveActionStack = s;
		pProcessActions(elapsed);
	});
	
	pActiveActionStack = pMainActionStack;
	pProcessActions(elapsed);
}

void cePlayback::AdvanceToNextAction(){
	if(!pTopic && pTestActionList.IsEmpty()){
		SetRunning(false);
		return;
	}
	
	if(pActiveActionStack->IsEmpty()){
		if(pTestActionList.IsNotEmpty()){
			pActiveActionStack->Push(nullptr, pTestActionList.First(), &pTestActionList, 0);
			
		}else{
			pActiveActionStack->Push(pTopic, nullptr, &pTopic->GetActions(), 0);
		}
		return;
	}
	
	while(true){
		cePlaybackActionStackEntry &stackEntry = pActiveActionStack->GetTop();
		
		stackEntry.AdvanceIndex();
		
		if(stackEntry.HasNextAction()){
			pActiveActionStack->SetActionWaiting(true);
			pActiveActionStack->SetActionTime(0.0f);
			break;
			
		}else{
			if(pActiveActionStack->GetCount() > 1){
				pActiveActionStack->Pop();
				pActiveActionStack->SetActionWaiting(true);
				pActiveActionStack->SetActionTime(0.0f);
				
			}else{
				break;
			}
		}
	}
}

void cePlayback::FastForwardSpeaking(){
	if(pTopic && pTestActionList.GetCount() == 0 && pRunning && !pPaused){
		float timeToForward = pActors.Inject(0.0f, [](float t, const cePlaybackActor &a){
			if(!a.IsSpeechDone()){
				t = decMath::max(t, a.GetSpeechLength() - a.GetElapsedTime());
			}
			return t;
		});
		
		const float timeStep = 0.1f;
		while(timeToForward > 1e-5f){
			pConversation.Update(decMath::min(timeStep, timeToForward));
			timeToForward -= timeStep;
		}
	}
}

void cePlayback::CancelLoopingLayer(int stackDepth){
	if(stackDepth < 0 || stackDepth > pActiveActionStack->GetCount()){
		return;
	}
	
	while(pActiveActionStack->GetCount() > stackDepth){
		pActiveActionStack->Pop();
	}
	
	pActiveActionStack->GetTop().CancelLooping();
	pActiveActionStack->SetActionTime(0.0f);
	pActiveActionStack->SetActionWaiting(true);
}



void cePlayback::SetTextBoxText(const decUnicodeString &text){
	ClearTextBoxText();
	
	if(!pTextBoxText){
		pTextBoxText = ceTextBoxText::Ref::New();
		pTextBoxText->SetName(decUnicodeString::NewFromUTF8("Playback:"));
	}
	
	pTextBoxText->SetText(text);
	
	pConversation.GetTextBox()->GetTexts().Add(pTextBoxText);
	pConversation.GetTextBox()->UpdateCanvas();
}

void cePlayback::ClearTextBoxText(){
	if(pTextBoxText){
		ceTextBox &textBox = *pConversation.GetTextBox();
		if(textBox.GetTexts().RemoveIfPresent(pTextBoxText)){
			textBox.UpdateCanvas();
		}
		pTextBoxText->SetText(decUnicodeString());
	}
}



// Private Functions
//////////////////////

void cePlayback::pProcessActions(float elapsed){
	ceConversationTopic *actionTopic = pLastPlayedActionTopic;
	ceConversationAction::Ref action;
	int i;
	
	// check if any stack entry upwards contains a condition evaluating to false. in
	// this case all actions up to this level are cancelled. in particular this means
	// if two looping conditions are nested and the outer condition becomes false then
	// control passes outside the outer layer. this also means top-level layer
	// boundaries are skiped so be careful how you nest conditional loops
	cePlaybackEvaluateCondition evaluateCondition;
	
	for(i=0; i<pActiveActionStack->GetCount(); i++){
		cePlaybackActionStackEntry &stackEntry = pActiveActionStack->GetAt(i);
		
		const ceConversationCondition * const condition = stackEntry.GetLoopCondition();
		if(!condition){
			continue;
		}
		
		if(evaluateCondition.EvaluateCondition(pConversation, *condition)){
			continue;
		}
		
		stackEntry.CancelLooping();
		for(; i>0; i--){
			pActiveActionStack->Pop();
		}
		pActiveActionStack->SetActionTime(0.0f);
		pActiveActionStack->SetActionWaiting(true);
		break;
	}
	
	// determine the next action
	if(pActiveActionStack->IsNotEmpty()){
		cePlaybackActionStackEntry &stackEntry = pActiveActionStack->GetTop();
		
		action = stackEntry.GetNextAction();
		if(stackEntry.GetParentTopic()){
			actionTopic = stackEntry.GetParentTopic();
		}
		
		// special case. it is possible the stack entry is looping but contains no actions.
		// in this case skip processing until a change comes from the outside world
		if(!action && stackEntry.GetLooping()){
			return;
		}
	}
	
	// if there is a next action process it
	if(action){
		// check for waiting conditions
		if(pActiveActionStack->GetActionWaiting()){
			const ceConversationActorList &actorList = pConversation.GetActorList();
			
			if(action->GetWaitForActor()){
				const bool useActorWait = !action->GetWaitSpeakOnly();
				
				if(action->GetWaitForActorID().IsEmpty()){
					for(i=0; i<pActors.GetCount(); i++){
						if(!pActors.GetAt(i)->IsSpeechDone()
						|| (useActorWait && actorList.GetAt(i)->GetWaiting())){
							return;
						}
					}
					
				}else{
					const int index = actorList.IndexWithIDOrAliasID(action->GetWaitForActorID());
					if(index != -1){
						if(!pActors.GetAt(index)->IsSpeechDone()
						|| (useActorWait && actorList.GetAt(index)->GetWaiting())){
							return;
						}
					}
				}
			}
			
			pActiveActionStack->SetActionWaiting(false);
		}
		
		// wait the delay time then process the action
		pActiveActionStack->SetActionTime(pActiveActionStack->GetActionTime() + elapsed);
		
		if(pActiveActionStack->GetActionTime() >= action->GetDelay()){
			cePlaybackProcessAction processAction;
			SetLastPlayedAction(actionTopic, action);
			processAction.ProcessAction(pConversation, action);
			pActiveActionStack->SetActionTime(0.0f);
			pActiveActionStack->SetActionWaiting(true);
		}
		
	// if there is no next action this can be either because a looping action has no actions or
	// we actually arrive at the end of the conversation.
	}else{
		if(pActiveActionStack->GetCount() > 1){
			// looping action without actions. advance to the next action in the stack below
			AdvanceToNextAction();
			
		}else if(pActiveActionStack == pMainActionStack){
			// end of conversation. wait for all actors to be done speaking and exit the loop
			for(const auto &a : pActors){
				if(!a->IsSpeechDone()){
					return;
				}
			}
			
			SetRunning(false);
			
		}else{
			pSideActionStacks.Remove(pActiveActionStack);
		}
	}
}

void cePlayback::SetLastPlayedAction(ceConversationTopic *topic, ceConversationAction *action){
	if(topic != pLastPlayedActionTopic){
		pLastPlayedActionTopic = topic;
	}
	
	if(action != pLastPlayedAction){
		pLastPlayedAction = action;
	}
}
