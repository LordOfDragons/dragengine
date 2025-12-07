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

#include "aeControllerTarget.h"
#include "aeControllerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeControllerTargetList::aeControllerTargetList(){
}

aeControllerTargetList::aeControllerTargetList(const aeControllerTargetList &copy) :
pTargets(copy.pTargets){
}

aeControllerTargetList::~aeControllerTargetList(){
}



// Management
///////////////

int aeControllerTargetList::GetCount() const{
	return pTargets.GetCount();
}

aeControllerTarget *aeControllerTargetList::GetAt(int index) const{
	return (aeControllerTarget*)pTargets.GetAt(index);
}

int aeControllerTargetList::IndexOf(aeControllerTarget *target) const{
	return pTargets.IndexOf(target);
}

bool aeControllerTargetList::Has(aeControllerTarget *target) const{
	return pTargets.Has(target);
}

void aeControllerTargetList::Add(aeControllerTarget *target){
	if(! target || Has(target)){
		DETHROW(deeInvalidParam);
	}
	pTargets.Add(target);
}

void aeControllerTargetList::Insert(aeControllerTarget *target, int index){
	if(! target || Has(target)){
		DETHROW(deeInvalidParam);
	}
	pTargets.Insert(target, index);
}

void aeControllerTargetList::Move(aeControllerTarget *target, int index){
	pTargets.Move(target, index);
}

void aeControllerTargetList::Remove(aeControllerTarget *target){
	pTargets.RemoveFrom(pTargets.IndexOf(target));
}

void aeControllerTargetList::RemoveAll(){
	pTargets.RemoveAll();
}



// Operators
//////////////

aeControllerTargetList &aeControllerTargetList::operator=(const aeControllerTargetList &list){
	pTargets = list.pTargets;
	return *this;
}
