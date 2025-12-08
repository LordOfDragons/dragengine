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
pConversation(NULL),
pID(id){
}

ceConversationFile::ceConversationFile(const ceConversationFile &file) :
pConversation(NULL),
pID(file.pID)
{
	const int count = file.pTopics.GetCount();
	ceConversationTopic::Ref topic = NULL;
	int i;
	
	try{
		for(i=0; i<count; i++){
			topic.TakeOver(new ceConversationTopic(*file.pTopics.GetAt(i)));
			AddTopic(topic);
			topic = NULL;
		}
		
	}catch(const deException &){
		RemoveAllTopics();
		throw;
	}
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
	
	if(pConversation && pConversation->GetFileList().HasWithID(id)){
		DETHROW(deeInvalidParam);
	}
	
	pID = id;
	
	if(pConversation){
		pConversation->NotifyFileChanged(this);
	}
}



// Topics
///////////

void ceConversationFile::AddTopic(ceConversationTopic *topic){
	if(!topic || pTopics.HasWithID(topic->GetID().GetString()) || topic->GetFile()){
		DETHROW(deeInvalidParam);
	}
	
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
	if(!topic || !pTopics.Has(topic)){
		DETHROW(deeInvalidParam);
	}
	
	if(topic == pActiveTopic){
		if(pTopics.GetCount() == 1){
			SetActiveTopic(NULL);
			
		}else{
			if(pTopics.GetAt(0) == topic){
				SetActiveTopic(pTopics.GetAt(1));
				
			}else{
				SetActiveTopic(pTopics.GetAt(0));
			}
		}
	}
	
	topic->SetFile(NULL);
	pTopics.Remove(topic);
	
	if(pConversation){
		pConversation->NotifyTopicStructureChanged(this);
	}
}

void ceConversationFile::RemoveAllTopics(){
	const int count = pTopics.GetCount();
	int i;
	
	SetActiveTopic(NULL);
	
	for(i=0; i<count; i++){
		pTopics.GetAt(i)->SetFile(NULL);
	}
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
