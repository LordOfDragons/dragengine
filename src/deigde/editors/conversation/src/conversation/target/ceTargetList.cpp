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

#include "ceTarget.h"
#include "ceTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceTarget
///////////////////

// Constructor, destructor
////////////////////////////

ceTargetList::ceTargetList(){
}

ceTargetList::~ceTargetList(){
}



// Management
///////////////

int ceTargetList::GetCount() const{
	return pTargets.GetCount();
}

ceTarget *ceTargetList::GetAt(int index) const{
	return (ceTarget*)pTargets.GetAt(index);
}

ceTarget *ceTargetList::GetNamed(const char *name) const{
	const int count = pTargets.GetCount();
	ceTarget *target;
	int i;
	
	for(i=0; i<count; i++){
		target = (ceTarget*)pTargets.GetAt(i);
		
		if(target->GetName().Equals(name)){
			return target;
		}
	}
	
	return NULL;
}

int ceTargetList::IndexOf(ceTarget *target) const{
	return pTargets.IndexOf(target);
}

int ceTargetList::IndexOfNamed(const char *name) const{
	const int count = pTargets.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceTarget*)pTargets.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

bool ceTargetList::Has(ceTarget *target) const{
	return pTargets.Has(target);
}

bool ceTargetList::HasNamed(const char *name) const{
	const int count = pTargets.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceTarget*)pTargets.GetAt(i))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void ceTargetList::Add(ceTarget *target){
	if(!target || HasNamed(target->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pTargets.Add(target);
}

void ceTargetList::Remove(ceTarget *target){
	pTargets.Remove(target);
}

void ceTargetList::RemoveAll(){
	pTargets.RemoveAll();
}



ceTargetList &ceTargetList::operator=(const ceTargetList &list){
	pTargets = list.pTargets;
	return *this;
}

ceTargetList &ceTargetList::operator+=(const ceTargetList &list){
	pTargets += list.pTargets;
	return *this;
}
