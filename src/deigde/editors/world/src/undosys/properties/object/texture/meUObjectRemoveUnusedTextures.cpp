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

#include "meUObjectRemoveUnusedTextures.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Class meUObjectRemoveUnusedTextures
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectRemoveUnusedTextures::meUObjectRemoveUnusedTextures(meObject *object){
	if(!object){
		DETHROW(deeInvalidParam);
	}
	
	const int count = object->GetTextureCount();
	decStringList modelTextureNames;
	int i;
	
	meWorld * const world = object->GetWorld();
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	pObject = NULL;
	
	SetShortInfo("Remove Unused Object Textures");
	
	object->GetModelTextureNameList(modelTextureNames);
	for(i=0; i<count; i++){
		if(!modelTextureNames.Has(object->GetTextureAt(i)->GetName())){
			pTextureList.AddTexture(object->GetTextureAt(i));
		}
	}
	
	pObject = object;
	object->AddReference();
}

meUObjectRemoveUnusedTextures::~meUObjectRemoveUnusedTextures(){
	pTextureList.RemoveAllTextures();
	if(pObject){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectRemoveUnusedTextures::Undo(){
	const int count = pTextureList.GetTextureCount();
	int i;
	
	for(i=0; i<count; i++){
		pObject->AddTexture(pTextureList.GetTextureAt(i));
	}
}

void meUObjectRemoveUnusedTextures::Redo(){
	const int count = pTextureList.GetTextureCount();
	int i;
	
	for(i=0; i<count; i++){
		pObject->RemoveTexture(pTextureList.GetTextureAt(i));
	}
}
