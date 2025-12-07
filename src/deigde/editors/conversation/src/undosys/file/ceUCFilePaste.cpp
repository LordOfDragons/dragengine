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

#include "ceUCFilePaste.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>



// Class ceUCFilePaste
////////////////////////

// Constructor, destructor
////////////////////////////

ceUCFilePaste::ceUCFilePaste(ceConversation *conversation, const ceConversationFileList &files) :
pConversation(NULL){
	if(!conversation || files.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	const int count = files.GetCount();
	ceConversationFile *newFile = NULL;
	int i;
	
	if(count == 1){
		SetShortInfo("Paste file");
		
	}else{
		SetShortInfo("Paste files");
	}
	
	try{
		for(i=0; i<count; i++){
			newFile = new ceConversationFile(*files.GetAt(i));
			decString fileID(newFile->GetID());
			int newNameIndex = 1;
			
			while(conversation->GetFileList().HasWithID(fileID)){
				newNameIndex++;
				fileID.Format("%s_%i", newFile->GetID().GetString(), newNameIndex);
			}
			
			newFile->SetID(fileID);
			
			pFiles.Add(newFile);
			newFile->FreeReference();
			newFile = NULL;
		}
		
	}catch(const deException &){
		if(newFile){
			newFile->FreeReference();
		}
		throw;
	}
	
	pConversation = conversation;
	conversation->AddReference();
}

ceUCFilePaste::~ceUCFilePaste(){
	pFiles.RemoveAll();
}



// Management
///////////////

void ceUCFilePaste::Undo(){
	const int count = pFiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pConversation->RemoveFile(pFiles.GetAt(i));
	}
}

void ceUCFilePaste::Redo(){
	const int count = pFiles.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		pConversation->AddFile(pFiles.GetAt(i));
	}
	pConversation->SetActiveFile(pFiles.GetAt(count - 1));
}
