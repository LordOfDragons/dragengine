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

#include "meUObjectCloneTexturesToSelected.h"
#include "meUndoDataObjectTexture.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectCloneTexturesToSelected
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectCloneTexturesToSelected::meUObjectCloneTexturesToSelected(
const meObjectList &list, const meObjectTextureList &textureList){
	int textureCount = textureList.GetTextureCount();
	meUndoDataObjectTexture *undoData = NULL;
	const int objectCount = list.GetCount();
	meObject *object;
	int i, j;
	
	if(objectCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	try{
		SetShortInfo("Clone object textures to selected");
		
		for(i=0; i<textureCount; i++){
			pTextureList.AddTexture(textureList.GetTextureAt(i));
		}
		
		for(i=0; i<objectCount; i++){
			object = list.GetAt(i);
			
			if(! object->GetWorld()){
				DETHROW(deeInvalidParam);
			}
			
			undoData = new meUndoDataObjectTexture(object);
			textureCount = object->GetTextureCount();
			for(j=0; j<textureCount; j++){
				undoData->GetOldTextureList().AddTexture(object->GetTextureAt(j));
			}
			
			pList.Add(undoData);
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
}

meUObjectCloneTexturesToSelected::~meUObjectCloneTexturesToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUObjectCloneTexturesToSelected::Undo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	int i, j, textureCount;
	meObject *object;
	
	for(i=0; i<count; i++){
		undoData = (meUndoDataObjectTexture*)pList.GetAt(i);
		meObjectTextureList &oldTextureList = undoData->GetOldTextureList();
		object = undoData->GetObject();
		
		object->RemoveAllTextures();
		textureCount = oldTextureList.GetTextureCount();
		for(j=0; j<textureCount; j++){
			object->AddTexture(oldTextureList.GetTextureAt(j));
		}
	}
}

void meUObjectCloneTexturesToSelected::Redo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	meObjectTexture *texture = NULL;
	int i, j, textureCount;
	meObject *object;
	
	for(i=0; i<count; i++){
		undoData = (meUndoDataObjectTexture*)pList.GetAt(i);
		meObjectTextureList &newTextureList = undoData->GetNewTextureList();
		object = undoData->GetObject();
		
		if(newTextureList.GetTextureCount() == 0){
			textureCount = pTextureList.GetTextureCount();
			
			try{
				for(j=0; j<textureCount; j++){
					texture = new meObjectTexture(*pTextureList.GetTextureAt(j));
					newTextureList.AddTexture(texture);
					texture->FreeReference();
					texture = NULL;
				}
				
			}catch(const deException &){
				if(texture){
					texture->FreeReference();
				}
				throw;
			}
		}
		
		object->RemoveAllTextures();
		textureCount = newTextureList.GetTextureCount();
		for(j=0; j<textureCount; j++){
			object->AddTexture(newTextureList.GetTextureAt(j));
		}
	}
}



// Private Functions
//////////////////////

void meUObjectCloneTexturesToSelected::pCleanUp(){
	pList.RemoveAll();
}
