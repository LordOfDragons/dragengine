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

#include "ceUCGestureSetName.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/gesture/ceGesture.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>



// Class ceUCGestureSetName
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCGestureSetName::ceUCGestureSetName(ceGesture *gesture, const char *newName){
	if(!gesture || !newName) DETHROW(deeInvalidParam);
	
	const ceConversationFileList &fileList = gesture->GetConversation()->GetFileList();
	const int fileCount = fileList.GetCount();
	ceConversationTopic *topic;
	int e, t;
	
	pGesture = NULL;
	
	SetShortInfo("Gesture Set Name");
	
	pOldName = gesture->GetName();
	pNewName = newName;
	
	try{
		pGesture = gesture;
		for(e=0; e<fileCount; e++){
			const ceConversationFile &file = *fileList.GetAt(e);
			const ceConversationTopicList &topicList = file.GetTopicList();
			const int topicCount = topicList.GetCount();
			
			for(t=0; t<topicCount; t++){
				topic = topicList.GetAt(t);
				
				pAddActions(topic, topic->GetActionList());
			}
		}
		
	}catch(const deException &){
		pActionList.RemoveAll();
		throw;
	}
}

ceUCGestureSetName::~ceUCGestureSetName(){
	pActionList.RemoveAll();
}



// Management
///////////////

void ceUCGestureSetName::Undo(){
	pSetName(pNewName.GetString(), pOldName.GetString());
}

void ceUCGestureSetName::Redo(){
	pSetName(pOldName.GetString(), pNewName.GetString());
}



// Private Functions
//////////////////////

void ceUCGestureSetName::pSetName(const char *oldName, const char *newName){
	const int count = pActionList.GetCount();
	ceConversationAction *action;
	int i, j, gestureCount;
	
	pGesture->SetName(newName);
	
	for(i=0; i<count; i++){
		const ceUndoCAction &undoCAction = *pActionList.GetAt(i);
		
		action = undoCAction.GetAction();
		
		const ceCAActorSpeak &actorSpeak = *((ceCAActorSpeak*)action);
		const ceStripList &gestureList = actorSpeak.GetGestureList();
		
		gestureCount = gestureList.GetCount();
		for(j=0; j<gestureCount; j++){
			ceStrip &gesture = *gestureList.GetAt(j);
			
			if(gesture.GetID() == oldName){
				gesture.SetID(newName);
			}
		}
		
		undoCAction.GetTopic()->NotifyActionChanged(action);
	}
}

void ceUCGestureSetName::pAddActions(ceConversationTopic *topic, const ceConversationActionList &list){
	const int count = list.GetCount();
	ceUndoCAction::Ref undoCAction = NULL;
	ceConversationAction *action;
	int i, j, gestureCount;
	
	try{
		for(i=0; i<count; i++){
			action = list.GetAt(i);
			
			if(action->GetType() == ceConversationAction::eatActorSpeak){
				const ceCAActorSpeak &actorSpeak = *((ceCAActorSpeak*)action);
				const ceStripList &gestureList = actorSpeak.GetGestureList();
				
				gestureCount = gestureList.GetCount();
				for(j=0; j<gestureCount; j++){
					if(gestureList.GetAt(j)->GetID() == pOldName){
						undoCAction.TakeOver(new ceUndoCAction(action, topic));
						pActionList.Add(undoCAction);
						undoCAction = NULL;
						break;
					}
				}
				
			}else if(action->GetType() == ceConversationAction::eatIfElse){
				const ceCAIfElse &ifElse = *((ceCAIfElse*)action);
				const ceCAIfElseCaseList &caseList = ifElse.GetCases();
				const int caseCount = caseList.GetCount();
				
				for(j=0; j<caseCount; j++){
					pAddActions(topic, caseList.GetAt(j)->GetActions());
				}
				pAddActions(topic, ifElse.GetElseActions());
				
			}else if(action->GetType() == ceConversationAction::eatPlayerChoice){
				const ceCAPlayerChoice &playerChoice = *((ceCAPlayerChoice*)action);
				const ceCAPlayerChoiceOptionList &optionList = playerChoice.GetOptions();
				const int optionCount = optionList.GetCount();
				
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
		throw;
	}
}
