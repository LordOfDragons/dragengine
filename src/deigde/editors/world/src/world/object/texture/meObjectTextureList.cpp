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

#include "meObjectTexture.h"
#include "meObjectTextureList.h"

#include <dragengine/common/exceptions.h>



// Class meObjectTextureList
//////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectTextureList::meObjectTextureList(){
}

meObjectTextureList::~meObjectTextureList(){
}



// Management
///////////////

int meObjectTextureList::GetTextureCount() const{
	return pTextures.GetCount();
}

meObjectTexture *meObjectTextureList::GetTextureAt(int position) const{
	return (meObjectTexture*)pTextures.GetAt(position);
}

int meObjectTextureList::IndexOfTexture(meObjectTexture *texture) const{
	return pTextures.IndexOf(texture);
}

bool meObjectTextureList::HasTexture(meObjectTexture *texture){
	return pTextures.Has(texture);
}

void meObjectTextureList::AddTexture(meObjectTexture *texture){
	if(! texture) DETHROW(deeInvalidParam);
	
	pTextures.Add(texture);
}

void meObjectTextureList::AddTextureIfAbsent(meObjectTexture *texture){
	if(! texture) DETHROW(deeInvalidParam);
	
	pTextures.AddIfAbsent(texture);
}

void meObjectTextureList::RemoveTexture(meObjectTexture *texture){
	if(! texture) DETHROW(deeInvalidParam);
	
	pTextures.Remove(texture);
}

void meObjectTextureList::RemoveTextureIfPresent(meObjectTexture *texture){
	if(! texture) DETHROW(deeInvalidParam);
	
	pTextures.RemoveIfPresent(texture);
}

void meObjectTextureList::RemoveAllTextures(){
	pTextures.RemoveAll();
}
