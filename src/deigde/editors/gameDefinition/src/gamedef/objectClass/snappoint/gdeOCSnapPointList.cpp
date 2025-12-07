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

#include "gdeOCSnapPoint.h"
#include "gdeOCSnapPointList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCSnapPoint
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCSnapPointList::gdeOCSnapPointList(){
}

gdeOCSnapPointList::gdeOCSnapPointList(const gdeOCSnapPointList &list) :
pSnapPoints(list.pSnapPoints){
}

gdeOCSnapPointList::~gdeOCSnapPointList(){
}



// Management
///////////////

int gdeOCSnapPointList::GetCount() const{
	return pSnapPoints.GetCount();
}

gdeOCSnapPoint *gdeOCSnapPointList::GetAt(int index) const{
	return (gdeOCSnapPoint*)pSnapPoints.GetAt(index);
}

int gdeOCSnapPointList::IndexOf(gdeOCSnapPoint *snapPoint) const{
	return pSnapPoints.IndexOf(snapPoint);
}

bool gdeOCSnapPointList::Has(gdeOCSnapPoint *snapPoint) const{
	return pSnapPoints.Has(snapPoint);
}

void gdeOCSnapPointList::Add(gdeOCSnapPoint *snapPoint){
	if(! snapPoint || Has(snapPoint)){
		DETHROW(deeInvalidParam);
	}
	pSnapPoints.Add(snapPoint);
}

void gdeOCSnapPointList::Remove(gdeOCSnapPoint *snapPoint){
	pSnapPoints.Remove(snapPoint);
}

void gdeOCSnapPointList::RemoveAll(){
	pSnapPoints.RemoveAll();
}



gdeOCSnapPointList &gdeOCSnapPointList::operator=(const gdeOCSnapPointList &list){
	pSnapPoints = list.pSnapPoints;
	return *this;
}
