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

// includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debnVisitorValueCreate.h"
#include "../deNetworkBasic.h"
#include "../values/debnValue.h"
#include "../values/debnValueInteger.h"
#include "../values/debnValueFloat.h"
#include "../values/debnValueString.h"
#include "../values/debnValueData.h"
#include "../values/debnValuePoint2.h"
#include "../values/debnValuePoint3.h"
#include "../values/debnValueVector2.h"
#include "../values/debnValueVector3.h"
#include "../values/debnValueQuaternion.h"
#include <dragengine/resources/network/value/deNetworkValue.h>
#include <dragengine/resources/network/value/deNetworkValueInteger.h>
#include <dragengine/resources/network/value/deNetworkValueFloat.h>
#include <dragengine/resources/network/value/deNetworkValueString.h>
#include <dragengine/resources/network/value/deNetworkValueData.h>
#include <dragengine/resources/network/value/deNetworkValuePoint2.h>
#include <dragengine/resources/network/value/deNetworkValuePoint3.h>
#include <dragengine/resources/network/value/deNetworkValueVector2.h>
#include <dragengine/resources/network/value/deNetworkValueVector3.h>
#include <dragengine/resources/network/value/deNetworkValueQuaternion.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class debnVisitorValueCreate
///////////////////////////////

// Constructor, destructor
////////////////////////////

debnVisitorValueCreate::debnVisitorValueCreate(){
	pValue = NULL;
}

debnVisitorValueCreate::~debnVisitorValueCreate(){
}



// Management
///////////////

void debnVisitorValueCreate::Reset(){
	pValue = NULL;
}



// Visiting
/////////////

void debnVisitorValueCreate::VisitValue(deNetworkValue*){
	DETHROW(deeInvalidParam);
}

void debnVisitorValueCreate::VisitInteger(deNetworkValueInteger *value){
	pValue = new debnValueInteger(*value);
}

void debnVisitorValueCreate::VisitFloat(deNetworkValueFloat *value){
	pValue = new debnValueFloat(*value);
}

void debnVisitorValueCreate::VisitString(deNetworkValueString *value){
	pValue = new debnValueString(*value);
}

void debnVisitorValueCreate::VisitData(deNetworkValueData *value){
	pValue = new debnValueData(*value);
}

void debnVisitorValueCreate::VisitPoint2(deNetworkValuePoint2 *value){
	pValue = new debnValuePoint2(*value);
}

void debnVisitorValueCreate::VisitPoint3(deNetworkValuePoint3 *value){
	pValue = new debnValuePoint3(*value);
}

void debnVisitorValueCreate::VisitVector2(deNetworkValueVector2 *value){
	pValue = new debnValueVector2(*value);
}

void debnVisitorValueCreate::VisitVector3(deNetworkValueVector3 *value){
	pValue = new debnValueVector3(*value);
}

void debnVisitorValueCreate::VisitQuaternion(deNetworkValueQuaternion *value){
	pValue = new debnValueQuaternion(*value);
}
