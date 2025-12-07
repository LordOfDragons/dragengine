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

#include "meUObjectAddUsedTextures.h"
#include "../../../../utils/meHelpers.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>



// Class meUObjectAddUsedTextures
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectAddUsedTextures::meUObjectAddUsedTextures(meObject *object){
	if(!object){
		DETHROW(deeInvalidParam);
	}
	
	meWorld * const world = object->GetWorld();
	if(!world){
		DETHROW(deeInvalidParam);
	}
	
	pObject = NULL;
	
	SetShortInfo("Add Used Object Textures");
	
	decStringList modelTextureNames;
	object->GetModelTextureNameList(modelTextureNames);
	const int count = modelTextureNames.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(!object->HasTextureNamed(modelTextureNames.GetAt(i).GetString())){
			pTextureNameList.Add(modelTextureNames.GetAt(i));
		}
	}
	
	pObject = object;
	object->AddReference();
}

meUObjectAddUsedTextures::~meUObjectAddUsedTextures(){
	pTextureList.RemoveAllTextures();
	if(pObject){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectAddUsedTextures::Undo(){
	const int count = pTextureList.GetTextureCount();
	int i;
	
	for(i=0; i<count; i++){
		pObject->RemoveTexture(pTextureList.GetTextureAt(i));
	}
}

void meUObjectAddUsedTextures::Redo(){
	int i, count;
	
	if(pTextureList.GetTextureCount() == 0){
		const igdeGDCComponent * const gdcomponent = meHelpers::FindFirstComponent(pObject->GetGDClass());
		
		const igdeGDCCTextureList *gdctexlist = NULL;
		const igdeGDCCTextureList *gdccomptex = NULL;
		
		if(gdcomponent){
			gdctexlist = &gdcomponent->GetTextureList();
		}
		if(pObject->GetGDClass()){
			gdccomptex = &pObject->GetGDClass()->GetComponentTextures();
		}
		
		count = pTextureNameList.GetCount();
		for(i=0; i<count; i++){
			const decString &textureName = pTextureNameList.GetAt(i);
			
			igdeGDCCTexture *gdctex = NULL;
			if(gdctexlist){
				gdctex = gdctexlist->GetNamed(textureName);
			}
			if(!gdctex && gdccomptex){
				gdctex = gdccomptex->GetNamed(textureName);
			}
			
			meObjectTexture::Ref texture;
			meHelpers::CreateTexture(texture, pObject, textureName, gdctex);
			pTextureList.AddTexture(texture);
		}
	}
	
	count = pTextureList.GetTextureCount();
	for(i=0; i<count; i++){
		pObject->AddTexture(pTextureList.GetTextureAt(i));
	}
}
