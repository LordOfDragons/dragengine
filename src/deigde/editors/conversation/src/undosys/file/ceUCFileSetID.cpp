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

#include "ceUCFileSetID.h"
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



// Class ceUCFileSetID
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCFileSetID::ceUCFileSetID(const ceConversation &conversation,
ceConversationFile *file, const char *newID) :
pNewID(newID)
{
	if(!file || !newID){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("File Set ID");
	
	pOldID = file->GetID();
	
	const decString &matchGroupID = file->GetID();
	
	conversation.GetFiles().Visit([&](const ceConversationFile &f){
		f.GetTopics().Visit([&](ceConversationTopic *t){
			pAddSnippets(t, matchGroupID, t->GetActions());
		});
	});
	
	pFile = file;
}

ceUCFileSetID::~ceUCFileSetID(){
}



// Management
///////////////

void ceUCFileSetID::Undo(){
	pSetID(pOldID);
}

void ceUCFileSetID::Redo(){
	pSetID(pNewID);
}



// Private Functions
//////////////////////

void ceUCFileSetID::pSetID(const char *id){
	pFile->SetID(id);
	
	pSnippets.Visit([&](const ceUndoCAction &a){
		((ceCASnippet&)*a.GetAction()).SetFile(id);
		a.GetTopic()->NotifyActionChanged(a.GetAction());
	});
}

void ceUCFileSetID::pAddSnippets(ceConversationTopic *topic, const char *matchGroupID,
const ceConversationAction::List &actions){
	actions.Visit([&](ceConversationAction *a){
		switch(a->GetType()){
		case ceConversationAction::eatIfElse:{
			const ceCAIfElse &ifElse = *((const ceCAIfElse*)a);
			ifElse.GetCases().Visit([&](const ceCAIfElseCase &c){
				pAddSnippets(topic, matchGroupID, c.GetActions());
			});
			pAddSnippets(topic, matchGroupID, ifElse.GetElseActions());
			}break;
			
		case ceConversationAction::eatPlayerChoice:{
			const ceCAPlayerChoice &playerChoice = *((const ceCAPlayerChoice*)a);
			playerChoice.GetOptions().Visit([&](const ceCAPlayerChoiceOption &o){
				pAddSnippets(topic, matchGroupID, o.GetActions());
			});
			}break;
			
		case ceConversationAction::eatWait:
			pAddSnippets(topic, matchGroupID, ((const ceCAWait*)a)->GetActions());
			break;
			
		case ceConversationAction::eatSnippet:{
			ceCASnippet * const snippet = (ceCASnippet*)a;
			if(snippet->GetFile() == matchGroupID){
				pSnippets.Add(ceUndoCAction::Ref::New(a, topic));
			}
			}break;
			
		default:
			break;
		}
	});
}
