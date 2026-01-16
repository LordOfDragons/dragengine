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

#include "debnMessage.h"
#include "debnMessageManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/network/deNetworkMessage.h>



// Class debnMessageManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

debnMessageManager::debnMessageManager(){
}

debnMessageManager::~debnMessageManager(){
}



// Management
///////////////

debnMessage *debnMessageManager::GetMessageAt(int index) const{
	return pMessages.GetAt(index);
}

int debnMessageManager::IndexOfMessageWithNumber(int number) const{
	return pMessages.IndexOfMatching([&](const debnMessage &m){
		return m.GetNumber() == number;
	});
}

int debnMessageManager::IndexOfMessage(debnMessage *message) const{
	DEASSERT_NOTNULL(message)
	return pMessages.IndexOf(message);
}

bool debnMessageManager::HasMessage(debnMessage *message) const{
	DEASSERT_NOTNULL(message)
	return pMessages.Has(message);
}

void debnMessageManager::AddMessage(debnMessage::Ref &&message){
	pMessages.Add(std::move(message));
}

void debnMessageManager::RemoveMessage(debnMessage *message){
	pMessages.Remove(message);
}

void debnMessageManager::RemoveMessageAt(int index){
	pMessages.RemoveFrom(index);
}

void debnMessageManager::RemoveAllMessages(){
	pMessages.RemoveAll();
}
