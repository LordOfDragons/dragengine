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

#include "igdeGDCEnvMapProbe.h"
#include "igdeGDCEnvMapProbeList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCEnvMapProbe
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCEnvMapProbeList::igdeGDCEnvMapProbeList(){
}

igdeGDCEnvMapProbeList::igdeGDCEnvMapProbeList(const igdeGDCEnvMapProbeList &list){
	pProbes = list.pProbes;
}

igdeGDCEnvMapProbeList::~igdeGDCEnvMapProbeList(){
}



// Management
///////////////

int igdeGDCEnvMapProbeList::GetCount() const{
	return pProbes.GetCount();
}

igdeGDCEnvMapProbe *igdeGDCEnvMapProbeList::GetAt(int index) const{
	return (igdeGDCEnvMapProbe*)pProbes.GetAt(index);
}

int igdeGDCEnvMapProbeList::IndexOf(igdeGDCEnvMapProbe *probe) const{
	return pProbes.IndexOf(probe);
}

bool igdeGDCEnvMapProbeList::Has(igdeGDCEnvMapProbe *probe) const{
	return pProbes.Has(probe);
}

void igdeGDCEnvMapProbeList::Add(igdeGDCEnvMapProbe *probe){
	if(!probe){
		DETHROW(deeInvalidParam);
	}
	
	pProbes.Add(probe);
}

void igdeGDCEnvMapProbeList::InsertAt(igdeGDCEnvMapProbe *probe, int index){
	if(!probe){
		DETHROW(deeInvalidParam);
	}
	
	pProbes.Insert(probe, index);
}

void igdeGDCEnvMapProbeList::MoveTo(igdeGDCEnvMapProbe *probe, int index){
	pProbes.Move(probe, index);
}

void igdeGDCEnvMapProbeList::Remove(igdeGDCEnvMapProbe *probe){
	pProbes.Remove(probe);
}

void igdeGDCEnvMapProbeList::RemoveAll(){
	pProbes.RemoveAll();
}



void igdeGDCEnvMapProbeList::SetToDeepCopyFrom(const igdeGDCEnvMapProbeList &list){
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		Add(igdeGDCEnvMapProbe::Ref::NewWith(*list.GetAt(i)));
	}
}



// Operators
//////////////

igdeGDCEnvMapProbeList &igdeGDCEnvMapProbeList::operator=(const igdeGDCEnvMapProbeList &list){
	pProbes = list.pProbes;
	return *this;
}
