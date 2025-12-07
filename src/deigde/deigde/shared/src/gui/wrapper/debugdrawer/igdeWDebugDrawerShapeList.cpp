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

#include "igdeWDebugDrawerShape.h"
#include "igdeWDebugDrawerShapeList.h"

#include <dragengine/common/exceptions.h>



// Class igdeWDebugDrawerShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWDebugDrawerShapeList::igdeWDebugDrawerShapeList(){
}

igdeWDebugDrawerShapeList::~igdeWDebugDrawerShapeList(){
}



// Management
///////////////

int igdeWDebugDrawerShapeList::GetCount() const{
	return pList.GetCount();
}

igdeWDebugDrawerShape *igdeWDebugDrawerShapeList::GetAt(int index) const{
	return (igdeWDebugDrawerShape*)pList.GetAt(index);
}

bool igdeWDebugDrawerShapeList::Has(igdeWDebugDrawerShape *wrapper) const{
	return pList.Has(wrapper);
}

void igdeWDebugDrawerShapeList::Add(igdeWDebugDrawerShape *wrapper){
	if(! wrapper){
		DETHROW(deeInvalidParam);
	}
	
	pList.Add(wrapper);
}

void igdeWDebugDrawerShapeList::Remove(igdeWDebugDrawerShape *wrapper){
	pList.Remove(wrapper);
	delete wrapper;
}

void igdeWDebugDrawerShapeList::RemoveAll(){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (igdeWDebugDrawerShape*)pList.GetAt(i);
	}
	pList.RemoveAll();
}



void igdeWDebugDrawerShapeList::SetParentDebugDrawerAll(deDebugDrawer *debugDrawer){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeWDebugDrawerShape*)pList.GetAt(i))->SetParentDebugDrawer(debugDrawer);
	}
}

void igdeWDebugDrawerShapeList::SetVisibleAll(bool visible){
	const int count = pList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeWDebugDrawerShape*)pList.GetAt(i))->SetVisible(visible);
	}
}
