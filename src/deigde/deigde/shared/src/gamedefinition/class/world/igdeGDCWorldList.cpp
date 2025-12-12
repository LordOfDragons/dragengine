/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeGDCWorld.h"
#include "igdeGDCWorldList.h"

#include <dragengine/common/exceptions.h>


// Class igdeGDCWorld
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCWorldList::igdeGDCWorldList(){
}

igdeGDCWorldList::igdeGDCWorldList(const igdeGDCWorldList &list) :
pWorlds(list.pWorlds){
}

igdeGDCWorldList::~igdeGDCWorldList(){
}


// Management
///////////////

int igdeGDCWorldList::GetCount() const{
	return pWorlds.GetCount();
}

igdeGDCWorld *igdeGDCWorldList::GetAt(int index) const{
	return (igdeGDCWorld*)pWorlds.GetAt(index);
}

int igdeGDCWorldList::IndexOf(igdeGDCWorld *world) const{
	return pWorlds.IndexOf(world);
}

bool igdeGDCWorldList::Has(igdeGDCWorld *world) const{
	return pWorlds.Has(world);
}

void igdeGDCWorldList::Add(igdeGDCWorld *world){
	DEASSERT_NOTNULL(world)
	pWorlds.Add(world);
}

void igdeGDCWorldList::InsertAt(igdeGDCWorld *world, int index){
	DEASSERT_NOTNULL(world)
	pWorlds.Insert(world, index);
}

void igdeGDCWorldList::MoveTo(igdeGDCWorld *world, int index){
	pWorlds.Move(world, index);
}

void igdeGDCWorldList::Remove(igdeGDCWorld *world){
	pWorlds.Remove(world);
}

void igdeGDCWorldList::RemoveAll(){
	pWorlds.RemoveAll();
}


void igdeGDCWorldList::SetToDeepCopyFrom(const igdeGDCWorldList &list){
	const int count = list.GetCount();
	int i;
	for(i=0; i<count; i++){
		Add(igdeGDCWorld::Ref::New(*list.GetAt(i)));
	}
}


igdeGDCWorldList &igdeGDCWorldList::operator=(const igdeGDCWorldList &list){
	pWorlds = list.pWorlds;
	return *this;
}
