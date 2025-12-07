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

#include "gdeOCEnvMapProbe.h"
#include "gdeOCEnvMapProbeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCEnvMapProbe
///////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCEnvMapProbeList::gdeOCEnvMapProbeList(){
}

gdeOCEnvMapProbeList::gdeOCEnvMapProbeList(const gdeOCEnvMapProbeList &list) :
pProbes(list.pProbes){
}

gdeOCEnvMapProbeList::~gdeOCEnvMapProbeList(){
}



// Management
///////////////

int gdeOCEnvMapProbeList::GetCount() const{
	return pProbes.GetCount();
}

gdeOCEnvMapProbe *gdeOCEnvMapProbeList::GetAt(int index) const{
	return (gdeOCEnvMapProbe*)pProbes.GetAt(index);
}

int gdeOCEnvMapProbeList::IndexOf(gdeOCEnvMapProbe *probe) const{
	return pProbes.IndexOf(probe);
}

bool gdeOCEnvMapProbeList::Has(gdeOCEnvMapProbe *probe) const{
	return pProbes.Has(probe);
}

void gdeOCEnvMapProbeList::Add(gdeOCEnvMapProbe *probe){
	if(!probe || Has(probe)){
		DETHROW(deeInvalidParam);
	}
	pProbes.Add(probe);
}

void gdeOCEnvMapProbeList::Remove(gdeOCEnvMapProbe *probe){
	pProbes.Remove(probe);
}

void gdeOCEnvMapProbeList::RemoveAll(){
	pProbes.RemoveAll();
}



gdeOCEnvMapProbeList &gdeOCEnvMapProbeList::operator=(const gdeOCEnvMapProbeList &list){
	pProbes = list.pProbes;
	return *this;
}
