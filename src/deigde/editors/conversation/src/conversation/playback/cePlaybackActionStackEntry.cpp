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

#include "cePlaybackActionStackEntry.h"
#include "../action/ceConversationAction.h"
#include "../action/ceConversationActionList.h"
#include "../topic/ceConversationTopic.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class cePlaybackActionStackEntry
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackActionStackEntry::cePlaybackActionStackEntry() :
pParentTopic(NULL),
pParentList(NULL),

pLoopCondition(NULL),
pLooping(false),

pNextIndex(0){
}

cePlaybackActionStackEntry::~cePlaybackActionStackEntry(){
	Clear();
}



// Management
///////////////

void cePlaybackActionStackEntry::SetParentTopic(ceConversationTopic *topic){
	if(pParentTopic == topic){
		return;
	}
	
	
	pParentTopic = topic;
}

void cePlaybackActionStackEntry::SetParentAction(ceConversationAction *action){
	pParentAction = action;
}

void cePlaybackActionStackEntry::SetParentList(const ceConversationActionList *list){
	pParentList = list;
}



void cePlaybackActionStackEntry::SetLoopCondition(ceConversationCondition *condition){
	if(pLoopCondition == condition){
		return;
	}
	
	
	pLoopCondition = condition;
}

void cePlaybackActionStackEntry::SetLooping(bool looping){
	pLooping = looping;
}



void cePlaybackActionStackEntry::SetNextIndex(int index){
	if(!pParentList){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	if(index < 0 || index > pParentList->GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pNextIndex = index;
	
	if(pNextIndex < pParentList->GetCount()){
		pNextAction = pParentList->GetAt(pNextIndex);
		
	}else{
		pNextAction = NULL;
	}
}

void cePlaybackActionStackEntry::AdvanceIndex(){
	if(!pParentList){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	const int nextIndex = pNextIndex + 1;
	
	if(nextIndex < pParentList->GetCount()){
		SetNextIndex(nextIndex);
		
	}else if(pLooping){
		SetNextIndex(0);
		
	}else{
		SetNextIndex(pParentList->GetCount());
	}
}

bool cePlaybackActionStackEntry::HasNextAction() const{
	return pNextAction;
}

void cePlaybackActionStackEntry::Clear(){
	pNextAction = NULL;
	pNextIndex = 0;
	
	SetLoopCondition(NULL);
	SetLooping(false);
	
	SetParentList(NULL);
	SetParentAction(NULL);
	SetParentTopic(NULL);
}

void cePlaybackActionStackEntry::ForwardLast(){
	if(!pParentList){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	if(pParentList->GetCount() == 0){
		SetNextIndex(0);
		
	}else{
		SetNextIndex(pParentList->GetCount() - 1);
	}
}

void cePlaybackActionStackEntry::ForwardEnd(){
	if(!pParentList){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	SetNextIndex(pParentList->GetCount());
}

void cePlaybackActionStackEntry::CancelLooping(){
	SetLooping(false);
	SetLoopCondition(NULL);
	ForwardEnd();
}
