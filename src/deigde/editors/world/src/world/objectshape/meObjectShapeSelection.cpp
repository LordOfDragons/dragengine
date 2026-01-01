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
#include "meObjectShape.h"
#include "meObjectShapeSelection.h"
#include "../meWorld.h"

#include <dragengine/common/exceptions.h>



// Class meObjectShapeSelection
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectShapeSelection::meObjectShapeSelection(){
}

meObjectShapeSelection::~meObjectShapeSelection(){
	Reset();
}



// Management
///////////////

void meObjectShapeSelection::Add(meObjectShape *objectShape){
	DEASSERT_NOTNULL(objectShape)
	
	objectShape->SetSelected(true);
	pSelection.Add(objectShape);
}

void meObjectShapeSelection::Remove(meObjectShape *objectShape){
	DEASSERT_NOTNULL(objectShape)
	
	objectShape->SetSelected(false);
	pSelection.Remove(objectShape);
}

void meObjectShapeSelection::RemoveAll(){
	pSelection.Visit([](meObjectShape &s){
		s.SetSelected(false);
	});
	pSelection.RemoveAll();
}



bool meObjectShapeSelection::HasActive() const{
	return pActive.IsNotNull();
}

void meObjectShapeSelection::SetActive(meObjectShape *objectShape){
	if(pActive){
		pActive->SetActive(false);
	}
	
	pActive = objectShape;
	
	if(objectShape){
		objectShape->SetActive(true);
	}
}

void meObjectShapeSelection::ActivateNext(){
	meObjectShape *next = nullptr;
	if(pSelection.IsNotEmpty()){
		if(pSelection.First() != pActive){
			next = pSelection.First();
			
		}else if(pSelection.GetCount() > 1){
			next = pSelection.GetAt(1);
		}
	}
	SetActive(next);
}

void meObjectShapeSelection::Reset(){
	RemoveAll();
	SetActive(nullptr);
}



void meObjectShapeSelection::Backup(const meObjectShape::List &shapeList, decTList<int> &indicesSelected, int &indexActive){
	indicesSelected.RemoveAll();
	pSelection.Visit([&](meObjectShape *s){
		const int index = shapeList.IndexOf(s);
		if(index != -1){
			indicesSelected.Add(index);
		}
	});
	
	indexActive = pActive ? shapeList.IndexOf(pActive) : -1;
}

void meObjectShapeSelection::Restore(const meObjectShape::List &shapeList, const decTList<int> &indicesSelected, int indexActive){
	Reset();
	
	const int shapeCount = shapeList.GetCount();
	indicesSelected.Visit([&](int index){
		if(index >= 0 && index < shapeCount){
			Add(shapeList.GetAt(index));
		}
	});
	
	if(indexActive >= 0 && indexActive < shapeCount){
		SetActive(shapeList.GetAt(indexActive));
	}
	
	if(!pActive && pSelection.IsNotEmpty()){
		ActivateNext();
	}
}
