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

#include "meObject.h"
#include "meObjectList.h"
#include "meObjectSelection.h"
#include "../meWorld.h"

#include <dragengine/common/exceptions.h>



// Class meObjectSelection
//////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectSelection::meObjectSelection(){
	pActive = NULL;
}

meObjectSelection::~meObjectSelection(){
	Reset();
}



// Management
///////////////

void meObjectSelection::Add(meObject *object){
	if(! object){
		DETHROW(deeInvalidParam);
	}
	
	object->SetSelected(true);
	pSelection.AddIfAbsent(object);
}

void meObjectSelection::Remove(meObject *object){
	if(! object){
		DETHROW(deeInvalidParam);
	}
	
	object->SetSelected(false);
	pSelection.RemoveIfPresent(object);
}

void meObjectSelection::RemoveAll(){
	const int count = pSelection.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSelection.GetAt(i)->SetSelected(false);
	}
	
	pSelection.RemoveAll();
}



bool meObjectSelection::HasActive() const{
	return pActive != NULL;
}

void meObjectSelection::SetActive(meObject *object){
	if(pActive){
		pActive->SetActive(false);
		pActive->FreeReference();
	}
	
	pActive = object;
	
	if(object){
		object->AddReference();
		object->SetActive(true);
	}
}

void meObjectSelection::ActivateNext(){
	const int count = pSelection.GetCount();
	meObject *next = NULL;
	int i;
	
	for(i=0; i<count; i++){
		if(pActive != pSelection.GetAt(i)){
			next = pSelection.GetAt(i);
			break;
		}
	}
	
	SetActive(next);
}

void meObjectSelection::Reset(){
	RemoveAll();
	SetActive(NULL);
}
