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

#include "seLink.h"
#include "seLinkList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seLink
/////////////////

// Constructor, destructor
////////////////////////////

seLinkList::seLinkList(){
}

seLinkList::~seLinkList(){
}



// Management
///////////////

int seLinkList::GetCount() const{
	return pLinks.GetCount();
}

seLink *seLinkList::GetAt(int index) const{
	return (seLink*)pLinks.GetAt(index);
}

int seLinkList::IndexOf(seLink *link) const{
	return pLinks.IndexOf(link);
}

bool seLinkList::Has(seLink *link) const{
	return pLinks.Has(link);
}

void seLinkList::Add(seLink *link){
	if(! link){
		DETHROW(deeInvalidParam);
	}
	pLinks.Add(link);
}

void seLinkList::InsertAt(seLink *link, int index){
	if(! link){
		DETHROW(deeInvalidParam);
	}
	pLinks.Insert(link, index);
}

void seLinkList::MoveTo(seLink *link, int index){
	pLinks.Move(link, index);
}

void seLinkList::Remove(seLink *link){
	pLinks.Remove(link);
}

void seLinkList::RemoveAll(){
	pLinks.RemoveAll();
}



seLinkList &seLinkList::operator=(const seLinkList &list){
	pLinks = list.pLinks;
	return *this;
}

seLinkList &seLinkList::operator+=(const seLinkList &list){
	pLinks += list.pLinks;
	return *this;
}
