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

#include "stClassPoint.h"
#include "../stClassScripting.h"
#include "../../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csPointClass : public csClassObject{
	OOP scripting;
};

struct csPoint : public csObject{
	stClassPoint *clsPoint;
	decPoint point;
};



// Class stClassPoint
/////////////////////////

// Constructor, destructor
////////////////////////////

stClassPoint::stClassPoint(ScriptingSmalltalk &st) :
pST(st){
	gst_define_cfunc("DEPoint.new", (void*)ccNew);
	gst_define_cfunc("DEPoint.newXY", (void*)ccNewXY);
	
	gst_define_cfunc("DEPoint.x", (void*)ccX);
	gst_define_cfunc("DEPoint.y", (void*)ccY);
	
	gst_define_cfunc("DEPoint.hash", (void*)ccHash);
	gst_define_cfunc("DEPoint.asString", (void*)ccAsString);
}

stClassPoint::~stClassPoint(){
}



// Management
///////////////

void stClassPoint::SetUpLinks(){
	pOOPClass = gst_class_name_to_oop("DEPoint");
	if(!pOOPClass){
		DETHROW(deeInvalidParam);
	}
	
	csPointClass &csclass = *((csPointClass*)OOP_TO_OBJ(pOOPClass));
	csclass.scripting = pST.GetClassScripting()->GetSingleton();
}

const decPoint &stClassPoint::OOPToPoint(OOP object) const{
	if(object == pST.GetNil()){
		DETHROW(deeNullPointer);
	}
	
	if(gst_get_object_class(object) != pOOPClass){
		DETHROW(deeInvalidAction);
	}
	return ((csPoint*)OOP_TO_OBJ(object))->point;
}

OOP stClassPoint::PointToOOP(const decPoint &vector){
	OOP oopVector = NULL;
	
	oopVector = pST.CreateNewObjectWithInit(pOOPClass, sizeof(csPoint) - sizeof(csObject));
	if(!oopVector || oopVector == pST.GetNil()){
		DETHROW(deeOutOfMemory);
	}
	
	csPoint &csobject = *((csPoint*)OOP_TO_OBJ(oopVector));
	csobject.clsPoint = this;
	csobject.point = vector;
	
	return oopVector;
}



// cCall Methods
//////////////////

OOP stClassPoint::ccNew(OOP self){
	const csPointClass &csclass = *((csPointClass*)OOP_TO_OBJ(self));
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	OOP result = NULL;
	
	try{
		result = st.GetClassPoint()->PointToOOP(decPoint());
		
	}catch(const deException &e){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}

OOP stClassPoint::ccNewXY(OOP self, int x, int y){
	const csPointClass &csclass = *((csPointClass*)OOP_TO_OBJ(self));
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	OOP result = NULL;
	
	try{
		result = st.GetClassPoint()->PointToOOP(decPoint(x, y));
		
	}catch(const deException &e){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}



int stClassPoint::ccX(OOP self){
	const csPoint &cscanvas = *((csPoint*)OOP_TO_OBJ(self));
	return cscanvas.point.x;
}

int stClassPoint::ccY(OOP self){
	const csPoint &cscanvas = *((csPoint*)OOP_TO_OBJ(self));
	return cscanvas.point.y;
}



int stClassPoint::ccHash(OOP self){
	const csPoint &cscanvas = *((csPoint*)OOP_TO_OBJ(self));
	
	return cscanvas.point.x * 10000 + cscanvas.point.y;
}

OOP stClassPoint::ccAsString(OOP self){
	const csPoint &cscanvas = *((csPoint*)OOP_TO_OBJ(self));
	
	char buffer[50];
	sprintf((char*)buffer, "(%i,%i)", cscanvas.point.x, cscanvas.point.y);
	
	return gst_string_to_oop(buffer);
}
