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

#include "gdeOCInherit.h"
#include "gdeOCInheritList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCInherit
///////////////////////

// Constructor, destructor
////////////////////////////

gdeOCInheritList::gdeOCInheritList(){
}

gdeOCInheritList::gdeOCInheritList(const gdeOCInheritList &list) :
pInheritObjectClasses(list.pInheritObjectClasses){
}

gdeOCInheritList::~gdeOCInheritList(){
}



// Management
///////////////

int gdeOCInheritList::GetCount() const{
	return pInheritObjectClasses.GetCount();
}

gdeOCInherit *gdeOCInheritList::GetAt(int index) const{
	return (gdeOCInherit*)pInheritObjectClasses.GetAt(index);
}

int gdeOCInheritList::IndexOf(gdeOCInherit *inherit) const{
	return pInheritObjectClasses.IndexOf(inherit);
}

bool gdeOCInheritList::Has(gdeOCInherit *inherit) const{
	return pInheritObjectClasses.Has(inherit);
}

void gdeOCInheritList::Add(gdeOCInherit *inherit){
	if(! inherit){
		DETHROW(deeInvalidParam);
	}
	
	pInheritObjectClasses.Add(inherit);
}

void gdeOCInheritList::Remove(gdeOCInherit *inherit){
	pInheritObjectClasses.Remove(inherit);
}

void gdeOCInheritList::RemoveAll(){
	pInheritObjectClasses.RemoveAll();
}



gdeOCInheritList &gdeOCInheritList::operator=(const gdeOCInheritList &list){
	pInheritObjectClasses = list.pInheritObjectClasses;
	return *this;
}
