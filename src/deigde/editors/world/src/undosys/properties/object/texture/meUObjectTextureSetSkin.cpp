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

#include "meUObjectTextureSetSkin.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectTextureSetSkin
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectTextureSetSkin::meUObjectTextureSetSkin(meObjectTexture *texture, const char *newskin){
	DEASSERT_NOTNULL(texture)
	DEASSERT_NOTNULL(newskin)
	
	meObject * const object = texture->GetObject();
	DEASSERT_NOTNULL(object)
	
	DEASSERT_NOTNULL(object->GetWorld())
	
	SetShortInfo("Set object texture skin");
	
	const cTexture::Ref udata(cTexture::Ref::New());
	udata->oldskin = texture->GetSkinPath();
	udata->newskin = newskin;
	udata->texture = texture;
	pTextures.Add(udata);
}

meUObjectTextureSetSkin::meUObjectTextureSetSkin(meObjectTexture::List &textures, const char *newskin){
	DEASSERT_NOTNULL(newskin)
	DEASSERT_TRUE(textures.IsNotEmpty())
	
	SetShortInfo("Set object texture skins");
	
	textures.Visit([&](meObjectTexture *texture){
		const cTexture::Ref udata(cTexture::Ref::New());
		udata->oldskin = texture->GetSkinPath();
		udata->newskin = newskin;
		udata->texture = texture;
		pTextures.Add(udata);
	});
}

meUObjectTextureSetSkin::~meUObjectTextureSetSkin(){
}



// Management
///////////////

void meUObjectTextureSetSkin::Undo(){
	pTextures.Visit([&](const cTexture &data){
		data.texture->SetSkinPath(data.oldskin);
	});
}

void meUObjectTextureSetSkin::Redo(){
	pTextures.Visit([&](const cTexture &data){
		data.texture->SetSkinPath(data.newskin);
	});
}
