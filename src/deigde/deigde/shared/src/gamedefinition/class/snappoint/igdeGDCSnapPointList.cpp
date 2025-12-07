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

#include "igdeGDCSnapPoint.h"
#include "igdeGDCSnapPointList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeGDCSnapPoint
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCSnapPointList::igdeGDCSnapPointList(){
}

igdeGDCSnapPointList::igdeGDCSnapPointList(const igdeGDCSnapPointList &list) :
pSnapPoints(list.pSnapPoints){
}

igdeGDCSnapPointList::~igdeGDCSnapPointList(){
}



// Management
///////////////

int igdeGDCSnapPointList::GetCount() const{
	return pSnapPoints.GetCount();
}

igdeGDCSnapPoint *igdeGDCSnapPointList::GetAt(int index) const{
	return (igdeGDCSnapPoint*)pSnapPoints.GetAt(index);
}

int igdeGDCSnapPointList::IndexOf(igdeGDCSnapPoint *snapPoint) const{
	return pSnapPoints.IndexOf(snapPoint);
}

bool igdeGDCSnapPointList::Has(igdeGDCSnapPoint *snapPoint) const{
	return pSnapPoints.Has(snapPoint);
}

void igdeGDCSnapPointList::Add(igdeGDCSnapPoint *snapPoint){
	if(!snapPoint || Has(snapPoint)){
		DETHROW(deeInvalidParam);
	}
	pSnapPoints.Add(snapPoint);
}

void igdeGDCSnapPointList::Remove(igdeGDCSnapPoint *snapPoint){
	pSnapPoints.Remove(snapPoint);
}

void igdeGDCSnapPointList::RemoveAll(){
	pSnapPoints.RemoveAll();
}



igdeGDCSnapPointList &igdeGDCSnapPointList::operator=(const igdeGDCSnapPointList &list){
	pSnapPoints = list.pSnapPoints;
	return *this;
}
