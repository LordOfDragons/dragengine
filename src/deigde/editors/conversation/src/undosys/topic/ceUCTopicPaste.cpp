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

#include "ceUCTopicPaste.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTopicPaste
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTopicPaste::ceUCTopicPaste(ceConversationFile *file, const ceConversationTopicList &topics) :
pFile(NULL){
	if(! file || topics.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	const int count = topics.GetCount();
	ceConversationTopic *newTopic = NULL;
	int i;
	
	if(count == 1){
		SetShortInfo("Paste topic");
		
	}else{
		SetShortInfo("Paste topics");
	}
	
	try{
		for(i=0; i<count; i++){
			newTopic = new ceConversationTopic(*topics.GetAt(i));
			decString topicID(newTopic->GetID());
			int newNameIndex = 1;
			
			while(file->GetTopicList().HasWithID(topicID)){
				newNameIndex++;
				topicID.Format("%s_%i", newTopic->GetID().GetString(), newNameIndex);
			}
			
			newTopic->SetID(topicID);
			
			pTopics.Add(newTopic);
			newTopic->FreeReference();
			newTopic = NULL;
		}
		
	}catch(const deException &){
		if(newTopic){
			newTopic->FreeReference();
		}
		throw;
	}
	
	pFile = file;
	file->AddReference();
}

ceUCTopicPaste::~ceUCTopicPaste(){
	pTopics.RemoveAll();
	if(pFile){
		pFile->FreeReference();
	}
}



// Management
///////////////

void ceUCTopicPaste::Undo(){
	const int count = pTopics.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pFile->RemoveTopic(pTopics.GetAt(i));
	}
}

void ceUCTopicPaste::Redo(){
	const int count = pTopics.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		pFile->AddTopic(pTopics.GetAt(i));
	}
	pFile->SetActiveTopic(pTopics.GetAt(count - 1));
}
