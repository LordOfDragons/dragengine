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

#include "cePlaybackActionStack.h"
#include "cePlaybackActionStackEntry.h"
#include "../action/ceConversationAction.h"
#include "../action/ceConversationActionList.h"

#include <dragengine/common/exceptions.h>



// Class cePlaybackActionStack
////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackActionStack::cePlaybackActionStack() :
pActionWaiting(false),
pActionTime(0.0f)
{
	pEntries = NULL;
	pEntryCount = 0;
	pEntrySize = 0;
	
	SetStackSize(100);
}

cePlaybackActionStack::~cePlaybackActionStack(){
	if(pEntries){
		Clear();
		delete [] pEntries;
	}
}

void cePlaybackActionStack::SetActionWaiting(bool actionWaiting){
	pActionWaiting = actionWaiting;
}

void cePlaybackActionStack::SetActionTime(float time){
	pActionTime = time;
}



// Management
///////////////

void cePlaybackActionStack::SetStackSize(int size){
	if(size < 1){
		DETHROW(deeInvalidParam);
	}
	
	Clear();
	
	cePlaybackActionStackEntry *newArray = new cePlaybackActionStackEntry[size];
	if(pEntries){
		delete [] pEntries;
	}
	pEntries = newArray;
	pEntrySize = size;
}

cePlaybackActionStackEntry &cePlaybackActionStack::GetTop() const{
	if(pEntryCount == 0){
		DETHROW(deeStackEmpty);
	}
	
	return pEntries[pEntryCount - 1];
}

cePlaybackActionStackEntry &cePlaybackActionStack::GetAt(int position) const{
	if(position < 0 || position >= pEntryCount){
		DETHROW(deeInvalidParam);
	}
	
	return pEntries[pEntryCount - 1 - position];
}

void cePlaybackActionStack::Push(ceConversationTopic *topic,
ceConversationAction *action, const ceConversationActionList *list, int index){
	if((!topic && !action) || !list || index < 0){
		DETHROW(deeInvalidParam);
	}
	if(pEntryCount == pEntrySize){
		DETHROW(deeStackOverflow);
	}
	
	pEntries[pEntryCount].SetParentTopic(topic);
	pEntries[pEntryCount].SetParentAction(action);
	pEntries[pEntryCount].SetParentList(list);
	pEntries[pEntryCount].SetNextIndex(index);
	pEntryCount++;
}

void cePlaybackActionStack::Pop(){
	if(pEntryCount == 0){
		DETHROW(deeStackEmpty);
	}
	
	pEntryCount--;
	pEntries[pEntryCount].Clear();
}

bool cePlaybackActionStack::HasNextAction() const{
	if(pEntryCount == 0){
		return NULL;
		
	}else{
		return pEntries[pEntryCount - 1].HasNextAction();
	}
}

void cePlaybackActionStack::Clear(){
	while(pEntryCount > 0){
		pEntryCount--;
		pEntries[pEntryCount].Clear();
	}
	
	pActionTime = 0.0f;
	pActionWaiting = true;
}
