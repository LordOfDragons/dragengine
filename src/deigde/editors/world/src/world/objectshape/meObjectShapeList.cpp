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

#include "meObjectShape.h"
#include "meObjectShapeList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShape.h>
#include <deigde/codec/igdeCodecPropertyString.h>



// Class meObjectShapeList
////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectShapeList::meObjectShapeList(){
}

meObjectShapeList::~meObjectShapeList(){
}



// Management
///////////////

int meObjectShapeList::GetCount() const{
	return pObjectShapes.GetCount();
}

meObjectShape *meObjectShapeList::GetAt(int position) const{
	return (meObjectShape*)pObjectShapes.GetAt(position);
}

int meObjectShapeList::IndexOf(meObjectShape *objectShape) const{
	return pObjectShapes.IndexOf(objectShape);
}

bool meObjectShapeList::Has(meObjectShape *objectShape) const{
	return pObjectShapes.Has(objectShape);
}

void meObjectShapeList::Add(meObjectShape *objectShape){
	if(!objectShape){
		DETHROW(deeInvalidParam);
	}
	
	pObjectShapes.Add(objectShape);
}

void meObjectShapeList::AddIfAbsent(meObjectShape *objectShape){
	if(!objectShape){
		DETHROW(deeInvalidParam);
	}
	
	pObjectShapes.AddIfAbsent(objectShape);
}

void meObjectShapeList::Remove(meObjectShape *objectShape){
	if(!objectShape){
		DETHROW(deeInvalidParam);
	}
	
	pObjectShapes.Remove(objectShape);
}

void meObjectShapeList::RemoveIfPresent(meObjectShape *objectShape){
	if(!objectShape){
		DETHROW(deeInvalidParam);
	}
	
	pObjectShapes.RemoveIfPresent(objectShape);
}

void meObjectShapeList::RemoveAll(){
	pObjectShapes.RemoveAll();
}



void meObjectShapeList::CreateShapeList(decShapeList &list) const{
	const int count = pObjectShapes.GetCount();
	decShape *shape = NULL;
	int i;
	
	list.RemoveAll();
	
	try{
		for(i=0; i<count; i++){
			shape = ((meObjectShape*)pObjectShapes.GetAt(i))->GetShape()->Copy();
			list.Add(shape);
			shape = NULL;
		}
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
}

void meObjectShapeList::CreatePropertyString(decString &string) const{
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	
	CreateShapeList(shapeList);
	codec.EncodeShapeList(shapeList, string);
}



meObjectShapeList &meObjectShapeList::operator=(const meObjectShapeList &list){
	pObjectShapes = list.pObjectShapes;
	return *this;
}
