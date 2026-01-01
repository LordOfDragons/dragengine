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

#include "ceUCTopicSetID.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCASnippet.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTopicSetID
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTopicSetID::ceUCTopicSetID(const ceConversation &conversation,
ceConversationTopic *topic, const char *newID) :

pNewID(newID)
{
	if(!topic || !topic->GetFile()){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Topic Set ID");
	
	pOldID = topic->GetID();
	
	const decString &matchGroupID = topic->GetFile()->GetID();
	const decString &matchTopicID = topic->GetID();
	
	conversation.GetFiles().Visit([&](const ceConversationFile &f){
		f.GetTopics().Visit([&](ceConversationTopic *t){
			pAddSnippets(t, matchGroupID, matchTopicID, t->GetActions());
		});
	});
	
	pTopic = topic;
}

ceUCTopicSetID::~ceUCTopicSetID(){
}



// Management
///////////////

void ceUCTopicSetID::Undo(){
	pSetID(pOldID);
}

void ceUCTopicSetID::Redo(){
	pSetID(pNewID);
}



// Private Functions
//////////////////////

void ceUCTopicSetID::pSetID(const char *id){
	pTopic->SetID(id);
	
	pSnippets.Visit([&](const ceUndoCAction &a){
		((ceCASnippet&)*a.GetAction()).SetTopic(id);
		a.GetTopic()->NotifyActionChanged(a.GetAction());
	});
}

void ceUCTopicSetID::pAddSnippets(ceConversationTopic *topic, const char *matchGroupID,
const char *matchTopicID, const ceConversationAction::List &actions){
	actions.Visit([&](ceConversationAction *a){
		switch(a->GetType()){
		case ceConversationAction::eatIfElse:{
			const ceCAIfElse &ifElse = *((const ceCAIfElse*)a);
			ifElse.GetCases().Visit([&](const ceCAIfElseCase &c){
				pAddSnippets(topic, matchGroupID, matchTopicID, c.GetActions());
			});
			pAddSnippets(topic, matchGroupID, matchTopicID, ifElse.GetElseActions());
			}break;
			
		case ceConversationAction::eatPlayerChoice:{
			const ceCAPlayerChoice &playerChoice = *((const ceCAPlayerChoice*)a);
			playerChoice.GetOptions().Visit([&](const ceCAPlayerChoiceOption &o){
				pAddSnippets(topic, matchGroupID, matchTopicID, o.GetActions());
			});
			}break;
			
		case ceConversationAction::eatWait:
			pAddSnippets(topic, matchGroupID, matchTopicID, ((const ceCAWait*)a)->GetActions());
			break;
			
		case ceConversationAction::eatSnippet:{
			ceCASnippet * const snippet = (ceCASnippet*)a;
			if(snippet->GetFile() == matchGroupID && snippet->GetTopic() == matchTopicID){
				pSnippets.Add(ceUndoCAction::Ref::New(a, topic));
			}
			}break;
			
		default:
			break;
		}
	});
}
