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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debnMessage.h"
#include "debnMessageManager.h"
#include "dragengine/resources/network/deNetworkMessage.h"
#include "dragengine/common/exceptions.h"



// Class debnMessageManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

debnMessageManager::debnMessageManager(){
	pMessages = NULL;
	pMessageCount = 0;
	pMessageSize = 0;
}

debnMessageManager::~debnMessageManager(){
	RemoveAllMessages();
	if(pMessages) delete [] pMessages;
}



// Management
///////////////

debnMessage *debnMessageManager::GetMessageAt(int index) const{
	if(index < 0 || index >= pMessageCount) DETHROW(deeInvalidParam);
	
	return pMessages[index];
}

int debnMessageManager::IndexOfMessageWithNumber(int number) const{
	int i;
	
	for(i=0; i<pMessageCount; i++){
		if(number == pMessages[i]->GetNumber()) return i;
	}
	
	return -1;
}

int debnMessageManager::IndexOfMessage(debnMessage *message) const{
	if(! message) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pMessageCount; i++){
		if(message == pMessages[i]) return i;
	}
	
	return -1;
}

bool debnMessageManager::HasMessage(debnMessage *message) const{
	if(! message) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pMessageCount; i++){
		if(message == pMessages[i]) return true;
	}
	
	return false;
}

void debnMessageManager::AddMessage(debnMessage *message){
	if(! message) DETHROW(deeInvalidParam);
	
	if(pMessageCount == pMessageSize){
		int newSize = pMessageSize * 3 / 2 + 1;
		debnMessage **newArray = new debnMessage*[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		if(pMessages){
			memcpy(newArray, pMessages, sizeof(debnMessage*) * pMessageSize);
			delete [] pMessages;
		}
		pMessages = newArray;
		pMessageSize = newSize;
	}
	
	pMessages[pMessageCount] = message;
	pMessageCount++;
}

void debnMessageManager::RemoveMessage(debnMessage *message){
	RemoveMessageAt(IndexOfMessage(message));
}

void debnMessageManager::RemoveMessageAt(int index){
	if(index < 0 || index >= pMessageCount) DETHROW(deeInvalidParam);
	debnMessage *message = pMessages[index];
	int i;
	
	for(i=index+1; i<pMessageCount; i++){
		pMessages[i - 1] = pMessages[i];
	}
	pMessageCount--;
	
	delete message;
}

void debnMessageManager::RemoveAllMessages(){
	while(pMessageCount > 0){
		pMessageCount--;
		delete pMessages[pMessageCount];
	}
}
