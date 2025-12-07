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

#include "meUObjectTextureSetSkin.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"
#include "../../../../world/object/texture/meObjectTextureList.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectTextureSetSkin
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectTextureSetSkin::meUObjectTextureSetSkin(meObjectTexture *texture, const char *newskin){
	if(!texture || !newskin) DETHROW(deeInvalidParam);
	
	meObject *object = texture->GetObject();
	if(!object) DETHROW(deeInvalidParam);
	
	meWorld *world = object->GetWorld();
	if(!world) DETHROW(deeInvalidParam);
	
	SetShortInfo("Set object texture skin");
	
	pTextures = NULL;
	pTextureCount = 0;
	
	try{
		pTextures = new sTexture[1];
		if(!pTextures) DETHROW(deeOutOfMemory);
		
		pTextures[0].oldskin = texture->GetSkinPath();
		pTextures[0].newskin = newskin;
		pTextures[0].texture = texture;
		
		pTextureCount = 1;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUObjectTextureSetSkin::meUObjectTextureSetSkin(meObjectTextureList &textures, const char *newskin){
	int count = textures.GetTextureCount();
	meObjectTexture *texture;
	
	if(count == 0 || !newskin) DETHROW(deeInvalidParam);
	
	SetShortInfo("Set object texture skins");
	
	pTextures = NULL;
	pTextureCount = 0;
	
	try{
		pTextures = new sTexture[count];
		if(!pTextures) DETHROW(deeOutOfMemory);
		
		for(pTextureCount=0; pTextureCount<count; pTextureCount++){
			texture = textures.GetTextureAt(pTextureCount);
			pTextures[pTextureCount].oldskin = texture->GetSkinPath();
			pTextures[pTextureCount].newskin = newskin;
			pTextures[pTextureCount].texture = texture;
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meUObjectTextureSetSkin::~meUObjectTextureSetSkin(){
	pCleanUp();
}



// Management
///////////////

void meUObjectTextureSetSkin::Undo(){
	int i;
	for(i=0; i<pTextureCount; i++){
		pTextures[i].texture->SetSkinPath(pTextures[i].oldskin);
	}
}

void meUObjectTextureSetSkin::Redo(){
	int i;
	for(i=0; i<pTextureCount; i++){
		pTextures[i].texture->SetSkinPath(pTextures[i].newskin);
	}
}



// Private Functions
//////////////////////

void meUObjectTextureSetSkin::pCleanUp(){
	if(pTextures){
		while(pTextureCount > 0){
			pTextureCount--;
			pTextures[pTextureCount].texture->FreeReference();
		}
		delete [] pTextures;
	}
}
