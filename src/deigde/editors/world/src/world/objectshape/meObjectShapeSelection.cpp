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
#include "meObjectShapeSelection.h"
#include "../meWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>



// Class meObjectShapeSelection
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectShapeSelection::meObjectShapeSelection(){
	pActive = NULL;
}

meObjectShapeSelection::~meObjectShapeSelection(){
	Reset();
}



// Management
///////////////

void meObjectShapeSelection::Add(meObjectShape *objectShape){
	if(!objectShape){
		DETHROW(deeInvalidParam);
	}
	
	objectShape->SetSelected(true);
	pSelection.AddIfAbsent(objectShape);
}

void meObjectShapeSelection::Remove(meObjectShape *objectShape){
	if(!objectShape){
		DETHROW(deeInvalidParam);
	}
	
	objectShape->SetSelected(false);
	pSelection.RemoveIfPresent(objectShape);
}

void meObjectShapeSelection::RemoveAll(){
	const int count = pSelection.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSelection.GetAt(i)->SetSelected(false);
	}
	
	pSelection.RemoveAll();
}



bool meObjectShapeSelection::HasActive() const{
	return pActive != NULL;
}

void meObjectShapeSelection::SetActive(meObjectShape *objectShape){
	if(pActive){
		pActive->SetActive(false);
		pActive->FreeReference();
	}
	
	pActive = objectShape;
	
	if(objectShape){
		objectShape->AddReference();
		objectShape->SetActive(true);
	}
}

void meObjectShapeSelection::ActivateNext(){
	const int count = pSelection.GetCount();
	meObjectShape *next = NULL;
	int i;
	
	for(i=0; i<count; i++){
		if(pActive != pSelection.GetAt(i)){
			next = pSelection.GetAt(i);
			break;
		}
	}
	
	SetActive(next);
}

void meObjectShapeSelection::Reset(){
	RemoveAll();
	SetActive(NULL);
}



void meObjectShapeSelection::Backup(const meObjectShapeList &shapeList, decIntList &indicesSelected, int &indexActive){
	const int count = pSelection.GetCount();
	int i, index;
	
	indicesSelected.RemoveAll();
	
	for(i=0; i<count; i++){
		index = shapeList.IndexOf(pSelection.GetAt(i));
		
		if(index != -1){
			indicesSelected.Add(index);
		}
	}
	
	if(pActive){
		indexActive = shapeList.IndexOf(pActive);
		
	}else{
		indexActive = -1;
	}
}

void meObjectShapeSelection::Restore(const meObjectShapeList &shapeList, const decIntList &indicesSelected, int indexActive){
	const int count = indicesSelected.GetCount();
	const int shapeCount = shapeList.GetCount();
	int i, index;
	
	Reset();
	
	for(i=0; i<count; i++){
		index = indicesSelected.GetAt(i);
		
		if(index >= 0 && index < shapeCount){
			Add(shapeList.GetAt(index));
		}
	}
	
	if(indexActive >= 0 && indexActive < shapeCount){
		SetActive(shapeList.GetAt(indexActive));
	}
	
	if(!pActive && pSelection.GetCount() > 0){
		ActivateNext();
	}
}
