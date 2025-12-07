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

#include "meUObjectTextureCopyToSelected.h"
#include "meUndoDataObjectTexture.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectTextureCopyToSelected
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectTextureCopyToSelected::meUObjectTextureCopyToSelected(const meObjectList &list, meObjectTexture *texture){
	const char *textureName = texture->GetName().GetString();
	meUndoDataObjectTexture *undoData = NULL;
	const int count = list.GetCount();
	meObject *object;
	int i;
	
	if(! texture || count < 1){
		DETHROW(deeInvalidParam);
	}
	
	pTexture = NULL;
	
	try{
		SetShortInfo("Copy object texture to selected");
		
		for(i=0; i<count; i++){
			object = list.GetAt(i);
			if(! object->GetWorld()){
				DETHROW(deeInvalidParam);
			}
			
			undoData = new meUndoDataObjectTexture(object);
			undoData->SetOldTexture(object->GetTextureNamed(textureName));
			
			if(undoData->GetOldTexture() != texture){
				pList.Add(undoData);
			}
			undoData->FreeReference();
			undoData = NULL;
		}
		
	}catch(const deException &){
		if(undoData){
			undoData->FreeReference();
		}
		pCleanUp();
		throw;
	}
	
	pTexture = texture;
	texture->AddReference();
}

meUObjectTextureCopyToSelected::~meUObjectTextureCopyToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUObjectTextureCopyToSelected::Undo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	meObject *object;
	int i;
	
	for(i=0; i<count; i++){
		undoData = (meUndoDataObjectTexture*)pList.GetAt(i);
		object = undoData->GetObject();
		
		if(undoData->GetNewTexture()){
			object->RemoveTexture(undoData->GetNewTexture());
		}
		if(undoData->GetOldTexture()){
			object->AddTexture(undoData->GetOldTexture());
		}
	}
}

void meUObjectTextureCopyToSelected::Redo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	meObjectTexture *texture = NULL;
	meObject *object;
	int i;
	
	for(i=0; i<count; i++){
		undoData = (meUndoDataObjectTexture*)pList.GetAt(i);
		object = undoData->GetObject();
		
		if(! undoData->GetNewTexture()){
			try{
				texture = new meObjectTexture(*pTexture);
				undoData->SetNewTexture(texture);
				texture->FreeReference();
				texture = NULL;
				
			}catch(const deException &){
				if(texture){
					texture->FreeReference();
				}
				throw;
			}
		}
		
		if(undoData->GetOldTexture()){
			object->RemoveTexture(undoData->GetOldTexture());
		}
		object->AddTexture(undoData->GetNewTexture());
	}
}



// Private Functions
//////////////////////

void meUObjectTextureCopyToSelected::pCleanUp(){
	pList.RemoveAll();
}
