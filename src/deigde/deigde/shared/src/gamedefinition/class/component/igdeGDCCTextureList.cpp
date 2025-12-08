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

#include "igdeGDCCTexture.h"
#include "igdeGDCCTextureList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCCTexture
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCCTextureList::igdeGDCCTextureList(){
}

igdeGDCCTextureList::igdeGDCCTextureList(const igdeGDCCTextureList &list){
	pTextures = list.pTextures;
}

igdeGDCCTextureList::~igdeGDCCTextureList(){
}



// Management
///////////////

int igdeGDCCTextureList::GetCount() const{
	return pTextures.GetCount();
}

igdeGDCCTexture *igdeGDCCTextureList::GetAt(int index) const{
	return (igdeGDCCTexture*)pTextures.GetAt(index);
}

igdeGDCCTexture *igdeGDCCTextureList::GetNamed(const char *name) const{
	const int count = pTextures.GetCount();
	igdeGDCCTexture *texture;
	int i;
	
	for(i=0; i<count; i++){
		texture = (igdeGDCCTexture*)pTextures.GetAt(i);
		
		if(texture->GetName().Equals(name)){
			return texture;
		}
	}
	
	return NULL;
}

int igdeGDCCTextureList::IndexOf(igdeGDCCTexture *texture) const{
	return pTextures.IndexOf(texture);
}

int igdeGDCCTextureList::IndexOfNamed(const char *name) const{
	const int count = pTextures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((igdeGDCCTexture*)pTextures.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

bool igdeGDCCTextureList::Has(igdeGDCCTexture *texture) const{
	return pTextures.Has(texture);
}

bool igdeGDCCTextureList::HasNamed(const char *name) const{
	return GetNamed(name) != NULL;
}

void igdeGDCCTextureList::Add(igdeGDCCTexture *texture){
	if(!texture){
		DETHROW(deeInvalidParam);
	}
	
	pTextures.Add(texture);
}

void igdeGDCCTextureList::InsertAt(igdeGDCCTexture *texture, int index){
	if(!texture){
		DETHROW(deeInvalidParam);
	}
	
	pTextures.Insert(texture, index);
}

void igdeGDCCTextureList::MoveTo(igdeGDCCTexture *texture, int index){
	pTextures.Move(texture, index);
}

void igdeGDCCTextureList::Remove(igdeGDCCTexture *texture){
	pTextures.Remove(texture);
}

void igdeGDCCTextureList::RemoveAll(){
	pTextures.RemoveAll();
}



void igdeGDCCTextureList::SetToDeepCopyFrom(const igdeGDCCTextureList &list){
	const int count = list.GetCount();
	igdeGDCCTexture *texture = NULL;
	
	int i;
	
	try{
		for(i=0; i<count; i++){
			texture = new igdeGDCCTexture(*list.GetAt(i));
			Add(texture);
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



// Operators
//////////////

igdeGDCCTextureList &igdeGDCCTextureList::operator=(const igdeGDCCTextureList &list){
	pTextures = list.pTextures;
	return *this;
}
