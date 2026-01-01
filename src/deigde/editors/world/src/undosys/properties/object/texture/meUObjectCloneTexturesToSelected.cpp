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
const meObject::List &list, const meObjectTexture::List &textureList){
	DEASSERT_TRUE(list.IsNotEmpty())
	DEASSERT_TRUE(textureList.IsNotEmpty())
	
	SetShortInfo("Clone object textures to selected");
	pTextureList = textureList;
	
	list.Visit([&](meObject *object){
		DEASSERT_NOTNULL(object->GetWorld())
		
		const meUndoDataObjectTexture::Ref udata(meUndoDataObjectTexture::Ref::New(object));
		udata->GetOldTextureList() = object->GetTextures();
		pList.Add(udata);
	});
}

meUObjectCloneTexturesToSelected::~meUObjectCloneTexturesToSelected(){
}



// Management
///////////////

void meUObjectCloneTexturesToSelected::Undo(){
	pList.Visit([&](const meUndoDataObjectTexture &data){
		meObject * const object = data.GetObject();
		
		object->RemoveAllTextures();
		data.GetOldTextureList().Visit([&](meObjectTexture *t){
			object->AddTexture(t);
		});
	});
}

void meUObjectCloneTexturesToSelected::Redo(){
	pList.Visit([&](meUndoDataObjectTexture &data){
		meObject * const object = data.GetObject();
		
		if(data.GetNewTextureList().IsEmpty()){
			pTextureList.Visit([&](const meObjectTexture &t){
				data.GetNewTextureList().Add(meObjectTexture::Ref::New(t));
			});
		}
		
		object->RemoveAllTextures();
		data.GetNewTextureList().Visit([&](meObjectTexture *t){
			object->AddTexture(t);
		});
	});
}
