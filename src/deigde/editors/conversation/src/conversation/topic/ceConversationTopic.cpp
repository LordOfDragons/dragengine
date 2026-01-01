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

#include "ceConversationTopic.h"
#include "../ceConversation.h"
#include "../actor/ceConversationActor.h"
#include "../actor/speechAnimation/ceSpeechAnimation.h"
#include "../action/ceConversationAction.h"
#include "../action/ceCAActorSpeak.h"
#include "../action/ceCAIfElse.h"
#include "../action/ceCAIfElseCase.h"
#include "../action/ceCAPlayerChoice.h"
#include "../action/ceCAPlayerChoiceOption.h"
#include "../action/ceCAWait.h"
#include "../file/ceConversationFile.h"
#include "../../conversation/strip/ceStrip.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>



// Class ceConversationTopic
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationTopic::ceConversationTopic(const char *id) :
pFile(nullptr),
pID(id),
pActiveCondition(nullptr){
}

ceConversationTopic::ceConversationTopic(const ceConversationTopic &topic) :
pFile(nullptr),
pID(topic.pID),
pActiveCondition(nullptr)
{
	topic.pActions.Visit([&](const ceConversationAction &a){
		pActions.Add(a.CreateCopy());
	});
}

ceConversationTopic::~ceConversationTopic(){
	SetActive(nullptr, nullptr);
}



// Management
///////////////

void ceConversationTopic::SetFile(ceConversationFile *file){
	pFile = file;
}

void ceConversationTopic::SetID(const char *id){
	if(pID == id){
		return;
	}
	
	if(pFile && pFile->GetTopics().HasMatching([&](const ceConversationTopic *t){ return t != this && t->GetID() == id; })){
		DETHROW(deeInvalidParam);
	}
	
	pID = id;
	
	if(pFile && pFile->GetConversation()){
		pFile->GetConversation()->NotifyTopicChanged(pFile, this);
	}
}

void ceConversationTopic::NotifyActionStructureChanged(ceConversationAction *action){
	if(pFile && pFile->GetConversation()){
		pFile->GetConversation()->NotifyActionStructureChanged(pFile, this, action);
	}
}

void ceConversationTopic::NotifyActionChanged(ceConversationAction *action){
	if(pFile && pFile->GetConversation()){
		pFile->GetConversation()->NotifyActionChanged(pFile, this, action);
	}
}

void ceConversationTopic::SetActive(ceConversationAction *action, ceConversationCondition *condition){
	if(pActiveAction == action && pActiveCondition == condition){
		return;
	}
	
	pActiveAction = action;
	pActiveCondition = condition;
	
	if(pFile && pFile->GetConversation()){
		pFile->GetConversation()->NotifyActiveChanged(pFile, this);
	}
}

void ceConversationTopic::FindMissingWords(decStringSet &missingWords) const{
	FindMissingWords(pActions, missingWords);
}

void ceConversationTopic::FindMissingWords(const ceConversationAction::List &actions,
decStringSet &missingWords) const{
	if(!pFile || !pFile->GetConversation()){
		return;
	}
	
	const ceConversation &conversation = *pFile->GetConversation();
	const ceConversationActorList &actorList = conversation.GetActorList();
	
	actions.Visit([&](const ceConversationAction &a){
		switch(a.GetType()){
		case ceConversationAction::eatActorSpeak:{
			const ceCAActorSpeak &speak = (ceCAActorSpeak&)a;
			if(!speak.GetUseSpeechAnimation() || speak.GetWords().IsEmpty()){
				break;
			}
			
			const ceConversationActor * const conversationActor = actorList.GetWithIDOrAliasID(speak.GetActor());
			if(!conversationActor || !conversationActor->GetSpeechAnimation()){
				break;
			}
			
			const ceSAWord::List &saWordList = conversationActor->GetSpeechAnimation()->GetWordList();
			
			speak.GetWords().Visit([&](const ceStrip &s){
				const decString &word = s.GetID();
				if(!word.IsEmpty() && !saWordList.HasMatching([&](const ceSAWord &w){ return w.GetName() == word; })){
					missingWords.Add(word);
				}
			});
			} break;
			
		case ceConversationAction::eatIfElse:{
			const ceCAIfElse &ifelse = (ceCAIfElse&)a;
			ifelse.GetCases().Visit([&](const ceCAIfElseCase &c){
				FindMissingWords(c.GetActions(), missingWords);
			});
			FindMissingWords(ifelse.GetElseActions(), missingWords);
			} break;
			
		case ceConversationAction::eatPlayerChoice:{
			const ceCAPlayerChoice &playerChoice = (ceCAPlayerChoice&)a;
			FindMissingWords(playerChoice.GetActions(), missingWords);
			playerChoice.GetOptions().Visit([&](const ceCAPlayerChoiceOption &o){
				FindMissingWords(o.GetActions(), missingWords);
			});
			} break;
			
		case ceConversationAction::eatWait:{
			FindMissingWords(((ceCAWait&)a).GetActions(), missingWords);
			} break;
			
		default:
			break;
		}
	});
}



void ceConversationTopic::NotifyConditionChanged(ceConversationAction *action,
ceConversationCondition *condition){
	if(pFile && pFile->GetConversation()){
		pFile->GetConversation()->NotifyConditionChanged(pFile, this, action, condition);
	}
}

void ceConversationTopic::NotifyConditionStructureChanged(ceConversationAction *action){
	if(pFile && pFile->GetConversation()){
		pFile->GetConversation()->NotifyConditionStructureChanged(pFile, this, action);
	}
}
