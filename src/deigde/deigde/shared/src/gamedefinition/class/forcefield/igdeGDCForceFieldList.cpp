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

#include "igdeGDCForceField.h"
#include "igdeGDCForceFieldList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCForceField
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCForceFieldList::igdeGDCForceFieldList(){
}

igdeGDCForceFieldList::igdeGDCForceFieldList(const igdeGDCForceFieldList &list){
	pFields = list.pFields;
}

igdeGDCForceFieldList::~igdeGDCForceFieldList(){
}



// Management
///////////////

int igdeGDCForceFieldList::GetCount() const{
	return pFields.GetCount();
}

igdeGDCForceField *igdeGDCForceFieldList::GetAt(int index) const{
	return (igdeGDCForceField*)pFields.GetAt(index);
}

int igdeGDCForceFieldList::IndexOf(igdeGDCForceField::Ref field) const{
	return pFields.IndexOf(field);
}

bool igdeGDCForceFieldList::Has(igdeGDCForceField::Ref field) const{
	return pFields.Has(field);
}

void igdeGDCForceFieldList::Add(igdeGDCForceField::Ref field){
	if(!field){
		DETHROW(deeInvalidParam);
	}
	pFields.Add(field);
}

void igdeGDCForceFieldList::InsertAt(igdeGDCForceField::Ref field, int index){
	if(!field){
		DETHROW(deeInvalidParam);
	}
	pFields.Insert(field, index);
}

void igdeGDCForceFieldList::MoveTo(igdeGDCForceField::Ref field, int index){
	pFields.Move(field, index);
}

void igdeGDCForceFieldList::Remove(igdeGDCForceField::Ref field){
	pFields.Remove(field);
}

void igdeGDCForceFieldList::RemoveAll(){
	pFields.RemoveAll();
}



void igdeGDCForceFieldList::SetToDeepCopyFrom(const igdeGDCForceFieldList &list){
	const int count = list.GetCount();
	igdeGDCForceField::Ref field = NULL;
	
	int i;
	
	try{
		for(i=0; i<count; i++){
			field.TakeOver(new igdeGDCForceField(*list.GetAt(i)));
			Add(field);
			field = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
}



// Operators
//////////////

igdeGDCForceFieldList &igdeGDCForceFieldList::operator=(const igdeGDCForceFieldList &list){
	pFields = list.pFields;
	return *this;
}
