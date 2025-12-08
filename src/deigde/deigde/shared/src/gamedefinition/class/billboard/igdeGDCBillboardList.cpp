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

#include "igdeGDCBillboard.h"
#include "igdeGDCBillboardList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCBillboard
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCBillboardList::igdeGDCBillboardList(){
}

igdeGDCBillboardList::igdeGDCBillboardList(const igdeGDCBillboardList &list){
	pBillboards = list.pBillboards;
}

igdeGDCBillboardList::~igdeGDCBillboardList(){
}



// Management
///////////////

int igdeGDCBillboardList::GetCount() const{
	return pBillboards.GetCount();
}

igdeGDCBillboard *igdeGDCBillboardList::GetAt(int index) const{
	return (igdeGDCBillboard*)pBillboards.GetAt(index);
}

int igdeGDCBillboardList::IndexOf(igdeGDCBillboard *billboard) const{
	return pBillboards.IndexOf(billboard);
}

bool igdeGDCBillboardList::Has(igdeGDCBillboard *billboard) const{
	return pBillboards.Has(billboard);
}

void igdeGDCBillboardList::Add(igdeGDCBillboard *billboard){
	if(!billboard){
		DETHROW(deeInvalidParam);
	}
	
	pBillboards.Add(billboard);
}

void igdeGDCBillboardList::InsertAt(igdeGDCBillboard *billboard, int index){
	if(!billboard){
		DETHROW(deeInvalidParam);
	}
	
	pBillboards.Insert(billboard, index);
}

void igdeGDCBillboardList::MoveTo(igdeGDCBillboard *billboard, int index){
	pBillboards.Move(billboard, index);
}

void igdeGDCBillboardList::Remove(igdeGDCBillboard *billboard){
	pBillboards.Remove(billboard);
}

void igdeGDCBillboardList::RemoveAll(){
	pBillboards.RemoveAll();
}



void igdeGDCBillboardList::SetToDeepCopyFrom(const igdeGDCBillboardList &list){
	const int count = list.GetCount();
	igdeGDCBillboard *billboard = NULL;
	
	int i;
	
	try{
		for(i=0; i<count; i++){
			billboard = new igdeGDCBillboard(*list.GetAt(i));
			Add(billboard);
			billboard->FreeReference();
			billboard = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
}



// Operators
//////////////

igdeGDCBillboardList &igdeGDCBillboardList::operator=(const igdeGDCBillboardList &list){
	pBillboards = list.pBillboards;
	return *this;
}
