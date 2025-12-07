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
#include <string.h>
#include <stdlib.h>

#include "ceUCTargetSetName.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/target/ceTarget.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTargetSetName
////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTargetSetName::ceUCTargetSetName(ceTarget *target, const char *newName){
	if(!target || !newName) DETHROW(deeInvalidParam);
	
	const ceCameraShotList &cameraShotList = target->GetConversation()->GetCameraShotList();
	const ceConversationFileList &fileList = target->GetConversation()->GetFileList();
	const int cameraShotCount = cameraShotList.GetCount();
	const int fileCount = fileList.GetCount();
	ceConversationTopic *topic;
	int i, j;
	
	pTarget = NULL;
	
	SetShortInfo("Target Set Name");
	
	pOldName = target->GetName();
	pNewName = newName;
	
	try{
		pTarget = target;
		target->AddReference();
		
		for(i=0; i<fileCount; i++){
			const ceConversationFile &file = *fileList.GetAt(i);
			const ceConversationTopicList &topicList = file.GetTopicList();
			const int topicCount = topicList.GetCount();
			
			for(j=0; j<topicCount; j++){
				topic = topicList.GetAt(j);
				
				pAddActions(topic, topic->GetActionList());
			}
		}
		
		for(i=0; i<cameraShotCount; i++){
			ceCameraShot * const cameraShot = cameraShotList.GetAt(i);
			
			if(cameraShot->GetCameraTarget() == pOldName || cameraShot->GetLookAtTarget() == pOldName){
				pCameraShotList.Add(cameraShot);
			}
		}
		
	}catch(const deException &){
		pActionList.RemoveAll();
		
		if(pTarget){
			pTarget->FreeReference();
		}
		
		throw;
	}
}

ceUCTargetSetName::~ceUCTargetSetName(){
	pCameraShotList.RemoveAll();
	pActionList.RemoveAll();
	
	if(pTarget){
		pTarget->FreeReference();
	}
}



// Management
///////////////

void ceUCTargetSetName::Undo(){
	pSetName(pNewName, pOldName);
}

void ceUCTargetSetName::Redo(){
	pSetName(pOldName, pNewName);
}



// Private Functions
//////////////////////

void ceUCTargetSetName::pSetName(const char *oldName, const char *newName){
	ceConversationAction *action;
	int i, count;
	
	pTarget->SetName(newName);
	
	count = pActionList.GetCount();
	for(i=0; i<count; i++){
		const ceUndoCAction &undoCAction = *pActionList.GetAt(i);
		
		action = undoCAction.GetAction();
		
		if(action->GetType() == ceConversationAction::eatCameraShot){
			ceCACameraShot &actionCShot = *((ceCACameraShot*)action);
			
			if(actionCShot.GetCameraTarget() == oldName){
				actionCShot.SetCameraTarget(newName);
			}
			if(actionCShot.GetLookAtTarget() == oldName){
				actionCShot.SetLookAtTarget(newName);
			}
		}
		
		undoCAction.GetTopic()->NotifyActionChanged(action);
	}
	
	count = pCameraShotList.GetCount();
	for(i=0; i<count; i++){
		ceCameraShot &cameraShot = *pCameraShotList.GetAt(i);
		
		if(cameraShot.GetCameraTarget() == oldName){
			cameraShot.SetCameraTarget(newName);
		}
		if(cameraShot.GetLookAtTarget() == oldName){
			cameraShot.SetLookAtTarget(newName);
		}
	}
}

void ceUCTargetSetName::pAddActions(ceConversationTopic *topic, const ceConversationActionList &list){
	const int count = list.GetCount();
	ceUndoCAction *undoCAction = NULL;
	ceConversationAction *action;
	int i;
	
	try{
		for(i=0; i<count; i++){
			action = list.GetAt(i);
			
			if(action->GetType() == ceConversationAction::eatCameraShot){
				const ceCACameraShot &target = *((ceCACameraShot*)action);
				
				if(target.GetCameraTarget() == pOldName || target.GetLookAtTarget() == pOldName){
					undoCAction = new ceUndoCAction(action, topic);
					pActionList.Add(undoCAction);
					undoCAction->FreeReference();
					undoCAction = NULL;
				}
				
			}else if(action->GetType() == ceConversationAction::eatIfElse){
				const ceCAIfElse &ifElse = *((ceCAIfElse*)action);
				const ceCAIfElseCaseList &caseList = ifElse.GetCases();
				const int caseCount = caseList.GetCount();
				int j;
				
				for(j=0; j<caseCount; j++){
					pAddActions(topic, caseList.GetAt(j)->GetActions());
				}
				pAddActions(topic, ifElse.GetElseActions());
				
			}else if(action->GetType() == ceConversationAction::eatPlayerChoice){
				const ceCAPlayerChoice &playerChoice = *((ceCAPlayerChoice*)action);
				const ceCAPlayerChoiceOptionList &optionList = playerChoice.GetOptions();
				const int optionCount = optionList.GetCount();
				int j;
				
				for(j=0; j<optionCount; j++){
					pAddActions(topic, optionList.GetAt(j)->GetActions());
				}
				
				pAddActions(topic, playerChoice.GetActions());
				
			}else if(action->GetType() == ceConversationAction::eatWait){
				const ceCAWait &wait = *((ceCAWait*)action);
				
				pAddActions(topic, wait.GetActions());
			}
		}
		
	}catch(const deException &){
		if(undoCAction){
			undoCAction->FreeReference();
		}
		throw;
	}
}
