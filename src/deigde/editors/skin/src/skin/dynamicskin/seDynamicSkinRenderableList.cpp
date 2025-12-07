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

#include "seDynamicSkinRenderable.h"
#include "seDynamicSkinRenderableList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seDynamicSkinRenderable
//////////////////////////////////

// Constructor, destructor
////////////////////////////

seDynamicSkinRenderableList::seDynamicSkinRenderableList(){
}

seDynamicSkinRenderableList::~seDynamicSkinRenderableList(){
}



// Management
///////////////

int seDynamicSkinRenderableList::GetCount() const{
	return pRenderables.GetCount();
}

seDynamicSkinRenderable *seDynamicSkinRenderableList::GetAt(int index) const{
	return (seDynamicSkinRenderable*)pRenderables.GetAt(index);
}

seDynamicSkinRenderable *seDynamicSkinRenderableList::GetNamed(const char *name) const{
	const int count = pRenderables.GetCount();
	seDynamicSkinRenderable *renderable;
	int p;
	
	for(p=0; p<count; p++){
		renderable = (seDynamicSkinRenderable*)pRenderables.GetAt(p);
		
		if(renderable->GetName().Equals(name)){
			return renderable;
		}
	}
	
	return NULL;
}

int seDynamicSkinRenderableList::IndexOf(seDynamicSkinRenderable *renderable) const{
	return pRenderables.IndexOf(renderable);
}

int seDynamicSkinRenderableList::IndexOfNamed(const char *name) const{
	const int count = pRenderables.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((seDynamicSkinRenderable*)pRenderables.GetAt(p))->GetName().Equals(name)){
			return p;
		}
	}
	
	return -1;
}

bool seDynamicSkinRenderableList::Has(seDynamicSkinRenderable *renderable) const{
	return pRenderables.Has(renderable);
}

bool seDynamicSkinRenderableList::HasNamed(const char *name) const{
	const int count = pRenderables.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		if(((seDynamicSkinRenderable*)pRenderables.GetAt(p))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void seDynamicSkinRenderableList::Add(seDynamicSkinRenderable *renderable){
	if(!renderable || HasNamed(renderable->GetName().GetString())){
		DETHROW(deeInvalidParam);
	}
	
	pRenderables.Add(renderable);
}

void seDynamicSkinRenderableList::InsertAt(seDynamicSkinRenderable *renderable, int index){
	if(!renderable || HasNamed(renderable->GetName().GetString())){
		DETHROW(deeInvalidParam);
	}
	
	pRenderables.Insert(renderable, index);
}

void seDynamicSkinRenderableList::MoveTo(seDynamicSkinRenderable *renderable, int index){
	pRenderables.Move(renderable, index);
}

void seDynamicSkinRenderableList::Remove(seDynamicSkinRenderable *renderable){
	pRenderables.Remove(renderable);
}

void seDynamicSkinRenderableList::RemoveAll(){
	pRenderables.RemoveAll();
}



seDynamicSkinRenderableList &seDynamicSkinRenderableList::operator=(const seDynamicSkinRenderableList &list){
	pRenderables = list.pRenderables;
	return *this;
}
