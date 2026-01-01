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
	
	pObject = nullptr;
	
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
}

meUObjectAddUsedTextures::~meUObjectAddUsedTextures(){
}



// Management
///////////////

void meUObjectAddUsedTextures::Undo(){
	pTextureList.Visit([&](meObjectTexture *texture){
		pObject->RemoveTexture(texture);
	});
}

void meUObjectAddUsedTextures::Redo(){
	if(pTextureList.IsEmpty()){
		const igdeGDCComponent * const gdcomponent = meHelpers::FindFirstComponent(pObject->GetGDClass());
		
		const igdeGDCCTexture::List *gdctexlist = nullptr;
		const igdeGDCCTexture::List *gdccomptex = nullptr;
		
		if(gdcomponent){
			gdctexlist = &gdcomponent->GetTextureList();
		}
		if(pObject->GetGDClass()){
			gdccomptex = &pObject->GetGDClass()->GetComponentTextures();
		}
		
		pTextureNameList.Visit([&](const decString &name){
			igdeGDCCTexture *gdctex = nullptr;
			if(gdctexlist){
				gdctex = gdctexlist->FindOrDefault([&](const igdeGDCCTexture &t){
					return t.GetName() == name;
				});
			}
			if(!gdctex && gdccomptex){
				gdctex = gdccomptex->FindOrDefault([&](const igdeGDCCTexture &t){
					return t.GetName() == name;
				});
			}
			
			meObjectTexture::Ref texture;
			meHelpers::CreateTexture(texture, pObject, name, gdctex);
			pTextureList.Add(texture);
		});
	}
	
	pTextureList.Visit([&](meObjectTexture *t){
		pObject->AddTexture(t);
	});
}
