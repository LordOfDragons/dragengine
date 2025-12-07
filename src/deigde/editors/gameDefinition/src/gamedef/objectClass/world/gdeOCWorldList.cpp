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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeOCWorld.h"
#include "gdeOCWorldList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class gdeOCWorld
/////////////////////

// Constructor, destructor
////////////////////////////

gdeOCWorldList::gdeOCWorldList(const gdeOCWorldList &list) :
pWorlds(list.pWorlds){
}


// Management
///////////////

int gdeOCWorldList::GetCount() const{
	return pWorlds.GetCount();
}

gdeOCWorld *gdeOCWorldList::GetAt(int index) const{
	return (gdeOCWorld*)pWorlds.GetAt(index);
}

int gdeOCWorldList::IndexOf(gdeOCWorld *world) const{
	return pWorlds.IndexOf(world);
}

bool gdeOCWorldList::Has(gdeOCWorld *world) const{
	return pWorlds.Has(world);
}

void gdeOCWorldList::Add(gdeOCWorld *world){
	DEASSERT_NOTNULL(world)
	DEASSERT_FALSE(Has(world))
	
	pWorlds.Add(world);
}

void gdeOCWorldList::Remove(gdeOCWorld *world){
	pWorlds.Remove(world);
}

void gdeOCWorldList::RemoveAll(){
	pWorlds.RemoveAll();
}


gdeOCWorldList &gdeOCWorldList::operator=(const gdeOCWorldList &list){
	pWorlds = list.pWorlds;
	return *this;
}
