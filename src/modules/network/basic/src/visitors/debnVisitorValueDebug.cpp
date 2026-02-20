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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "debnVisitorValueDebug.h"
#include "../deNetworkBasic.h"

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
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class debnVisitorValueDebug
////////////////////////////////

// Constructor, destructor
////////////////////////////

debnVisitorValueDebug::debnVisitorValueDebug(deNetworkBasic *module){
	pModule = module;
}

debnVisitorValueDebug::~debnVisitorValueDebug(){
}



// Management
///////////////



// Visiting
/////////////

void debnVisitorValueDebug::VisitValue(deNetworkValue*){
	DETHROW(deeInvalidParam);
}


void debnVisitorValueDebug::VisitInteger(deNetworkValueInteger *value){
#if defined __MINGW32__ || defined __MINGW64__
	#ifdef PRId64
		#undef PRId64
	#endif
	#define PRId64 "I64u"
#endif
	pModule->LogInfoFormat("- Integer: %" PRId64, value->GetInt());
}

void debnVisitorValueDebug::VisitFloat(deNetworkValueFloat *value){
	pModule->LogInfoFormat("- Float: %g", value->GetFloat());
}

void debnVisitorValueDebug::VisitString(deNetworkValueString *value){
	pModule->LogInfoFormat("- String: '%s'", value->GetString().GetString());
}

void debnVisitorValueDebug::VisitData(deNetworkValueData *value){
	int i, length = value->GetLength();
	uint8_t *data = value->GetData();
	decString text;
	
	text.AppendFormat("- Data: %i bytes:", length);
	for(i=0; i<20; i++){
		if(i == length) break;
		text.AppendFormat(" 0x%X(%c)", (unsigned char)data[i], data[i]);
	}
	
	pModule->LogInfo(text.GetString());
}

void debnVisitorValueDebug::VisitPoint2(deNetworkValuePoint2 *value){
	const decPoint &point = value->GetPoint();
	
	pModule->LogInfoFormat("- Point2: %i, %i", point.x, point.y);
}

void debnVisitorValueDebug::VisitPoint3(deNetworkValuePoint3 *value){
	const decPoint3 &point = value->GetPoint();
	
	pModule->LogInfoFormat("- Point3: %i, %i, %i", point.x, point.y, point.z);
}

void debnVisitorValueDebug::VisitVector2(deNetworkValueVector2 *value){
	const decVector2 &vector = value->GetVector();
	
	pModule->LogInfoFormat("- Vector2: %g, %g", vector.x, vector.y);
}

void debnVisitorValueDebug::VisitVector3(deNetworkValueVector3 *value){
	const decDVector &vector = value->GetVector();
	
	pModule->LogInfoFormat("- Vector3: %g, %g, %g", vector.x, vector.y, vector.z);
}

void debnVisitorValueDebug::VisitQuaternion(deNetworkValueQuaternion *value){
	const decQuaternion &quaternion = value->GetQuaternion();
	
	pModule->LogInfoFormat("- Quaternion: %g, %g, %g, %g", quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}
