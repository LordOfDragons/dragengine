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

#include "ceConversationFile.h"
#include "../ceConversation.h"
#include "../topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceConversationFile
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationFile::ceConversationFile(const char *id) :
pConversation(nullptr),
pID(id){
}

ceConversationFile::ceConversationFile(const ceConversationFile &file) :
pConversation(nullptr),
pID(file.pID)
{
	file.pTopics.Visit([&](const ceConversationTopic &t){
		AddTopic(ceConversationTopic::Ref::New(t));
	});
}

ceConversationFile::~ceConversationFile(){
	RemoveAllTopics();
}



// Management
///////////////

void ceConversationFile::SetConversation(ceConversation *conversation){
	pConversation = conversation;
}

void ceConversationFile::SetID(const char *id){
	if(!id){
		DETHROW(deeInvalidParam);
	}
	
	if(pID.Equals(id)){
		return;
	}
	
	if(pConversation){
		DEASSERT_FALSE(pConversation->GetFiles().HasMatching([&](const ceConversationFile &f){ return f.pID == id; }))
	}
	
	pID = id;
	
	if(pConversation){
		pConversation->NotifyFileChanged(this);
	}
}



// Topics
///////////

void ceConversationFile::AddTopic(ceConversationTopic *topic){
	DEASSERT_NOTNULL(topic)
	DEASSERT_FALSE(pTopics.HasMatching([&](const ceConversationTopic &t){ return t.GetID() == topic->GetID();}))
	DEASSERT_NULL(topic->GetFile())
	
	pTopics.Add(topic);
	topic->SetFile(this);
	
	if(pConversation){
		pConversation->NotifyTopicStructureChanged(this);
	}
	
	if(!pActiveTopic){
		SetActiveTopic(topic);
	}
}

void ceConversationFile::RemoveTopic(ceConversationTopic *topic){
	const ceConversationTopic::Ref guard(topic);
	pTopics.Remove(topic);
	
	if(topic == pActiveTopic){
		if(pTopics.GetCount() == 1){
			SetActiveTopic(nullptr);
			
		}else{
			if(pTopics.First() == topic){
				SetActiveTopic(pTopics.GetAt(1));
				
			}else{
				SetActiveTopic(pTopics.First());
			}
		}
	}
	
	topic->SetFile(nullptr);
	
	if(pConversation){
		pConversation->NotifyTopicStructureChanged(this);
	}
}

void ceConversationFile::RemoveAllTopics(){
	SetActiveTopic(nullptr);
	
	pTopics.Visit([&](ceConversationTopic &t){
		t.SetFile(nullptr);
	});
	pTopics.RemoveAll();
	
	if(pConversation){
		pConversation->NotifyTopicStructureChanged(this);
	}
}

void ceConversationFile::SetActiveTopic(ceConversationTopic *topic){
	if(topic == pActiveTopic){
		return;
	}
	pActiveTopic = topic;
	if(pConversation){
		pConversation->NotifyActiveTopicChanged(this);
	}
}
