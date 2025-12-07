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

#include "seTexture.h"
#include "seTextureList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seTexture
////////////////////

// Constructor, destructor
////////////////////////////

seTextureList::seTextureList(){
}

seTextureList::~seTextureList(){
}



// Management
///////////////

int seTextureList::GetCount() const{
	return pTextures.GetCount();
}

seTexture *seTextureList::GetAt(int index) const{
	return (seTexture*)pTextures.GetAt(index);
}

seTexture *seTextureList::GetNamed(const char *name) const{
	const int count = pTextures.GetCount();
	seTexture *texture;
	int p;
	
	for(p=0; p<count; p++){
		texture = (seTexture*)pTextures.GetAt(p);
		
		if(texture->GetName().Equals(name)){
			return texture;
		}
	}
	
	return NULL;
}

int seTextureList::IndexOf(seTexture *texture) const{
	return pTextures.IndexOf(texture);
}

int seTextureList::IndexOfNamed(const char *name) const{
	const int count = pTextures.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((seTexture*)pTextures.GetAt(p))->GetName().Equals(name)){
			return p;
		}
	}
	
	return -1;
}

bool seTextureList::Has(seTexture *texture) const{
	return pTextures.Has(texture);
}

bool seTextureList::HasNamed(const char *name) const{
	const int count = pTextures.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((seTexture*)pTextures.GetAt(p))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void seTextureList::Add(seTexture *texture){
	if(! texture || HasNamed(texture->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pTextures.Add(texture);
}

void seTextureList::InsertAt(seTexture *texture, int index){
	if(! texture || HasNamed(texture->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pTextures.Insert(texture, index);
}

void seTextureList::MoveTo(seTexture *texture, int index){
	pTextures.Move(texture, index);
}

void seTextureList::Remove(seTexture *texture){
	pTextures.Remove(texture);
}

void seTextureList::RemoveAll(){
	pTextures.RemoveAll();
}



seTextureList &seTextureList::operator=(const seTextureList &list){
	pTextures = list.pTextures;
	return *this;
}
