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

#include "meCDOObject.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meCDOObject
///////////////////////////

// Constructor, destructor
////////////////////////////

meCDOObject::meCDOObject(const meObject &object) :
pClassname(object.GetClassName()),
pPosition(object.GetPosition()),
pRotation(object.GetRotation()),
pScale(object.GetScaling()),
pProperties(object.GetProperties()),
pAttachBehaviors(object.GetAttachBehaviors()),
pAttachToIndex(-1)
{
	const int count = object.GetTextureCount();
	meObjectTexture::Ref texture = NULL;
	int i;
	
	try{
		for(i=0; i<count; i++){
			texture.TakeOver(new meObjectTexture(*object.GetTextureAt(i)));
			pTextures.AddTexture(texture);
			texture = NULL;
		}
		
	}catch(const deException &){
		if(texture){
			throw;
		}
	}
}

meCDOObject::~meCDOObject(){
}



// Management
///////////////

void meCDOObject::SetAttachToIndex(int index){
	pAttachToIndex = index;
}

void meCDOObject::SetAttachToID(const char *id){
	pAttachToID = id;
}

void meCDOObject::UpdateObject(meObject &object) const{
	object.SetClassName(pClassname);
	object.SetPosition(pPosition);
	object.SetScaling(pScale);
	object.SetRotation(pRotation);
	object.SetProperties(pProperties);
	object.SetAttachBehaviors(pAttachBehaviors);
	
	const int count = pTextures.GetTextureCount();
	meObjectTexture::Ref texture = NULL;
	int i;
	
	try{
		for(i=0; i<count; i++){
			texture.TakeOver(new meObjectTexture(*pTextures.GetTextureAt(i)));
			object.AddTexture(texture);
			texture = NULL;
		}
		
	}catch(const deException &){
		if(texture){
			throw;
		}
	}
	
	object.SetAttachedToID(pAttachToID); // attach to non-pasted object
}
