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

#include "seControllerTarget.h"
#include "seControllerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

seControllerTargetList::seControllerTargetList(){
}

seControllerTargetList::seControllerTargetList(const seControllerTargetList &copy) :
pTargets(copy.pTargets){
}

seControllerTargetList::~seControllerTargetList(){
}



// Management
///////////////

int seControllerTargetList::GetCount() const{
	return pTargets.GetCount();
}

seControllerTarget *seControllerTargetList::GetAt(int index) const{
	return (seControllerTarget*)pTargets.GetAt(index);
}

int seControllerTargetList::IndexOf(seControllerTarget *target) const{
	return pTargets.IndexOf(target);
}

bool seControllerTargetList::Has(seControllerTarget *target) const{
	return pTargets.Has(target);
}

void seControllerTargetList::Add(seControllerTarget *target){
	if(!target || Has(target)){
		DETHROW(deeInvalidParam);
	}
	pTargets.Add(target);
}

void seControllerTargetList::Insert(seControllerTarget *target, int index){
	if(!target || Has(target)){
		DETHROW(deeInvalidParam);
	}
	pTargets.Insert(target, index);
}

void seControllerTargetList::Move(seControllerTarget *target, int index){
	pTargets.Move(target, index);
}

void seControllerTargetList::Remove(seControllerTarget *target){
	pTargets.RemoveFrom(pTargets.IndexOf(target));
}

void seControllerTargetList::RemoveAll(){
	pTargets.RemoveAll();
}



// Operators
//////////////

seControllerTargetList &seControllerTargetList::operator=(const seControllerTargetList &list){
	pTargets = list.pTargets;
	return *this;
}
