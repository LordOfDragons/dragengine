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

#include "deNetworkValue.h"
#include "deNetworkValueInteger.h"
#include "deNetworkValueFloat.h"
#include "deNetworkValueString.h"
#include "deNetworkValueData.h"
#include "deNetworkValuePoint2.h"
#include "deNetworkValuePoint3.h"
#include "deNetworkValueVector2.h"
#include "deNetworkValueVector3.h"
#include "deNetworkValueQuaternion.h"
#include "deNetworkValueVisitor.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deNetworkValueVisitor
////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueVisitor::deNetworkValueVisitor(){
}

deNetworkValueVisitor::~deNetworkValueVisitor(){
}



// Visiting
/////////////

void deNetworkValueVisitor::VisitValue(deNetworkValue *value){
}

void deNetworkValueVisitor::VisitInteger(deNetworkValueInteger *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitFloat(deNetworkValueFloat *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitString(deNetworkValueString *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitData(deNetworkValueData *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitPoint2(deNetworkValuePoint2 *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitPoint3(deNetworkValuePoint3 *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitVector2(deNetworkValueVector2 *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitVector3(deNetworkValueVector3 *value){
	VisitValue(value);
}

void deNetworkValueVisitor::VisitQuaternion(deNetworkValueQuaternion *value){
	VisitValue(value);
}
