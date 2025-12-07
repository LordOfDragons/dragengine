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

#include "stClassColor.h"
#include "../stClassScripting.h"
#include "../../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csClrClass : public csClassObject{
	OOP scripting;
};

struct csClr : public csObject{
	stClassColor *clsClr;
	decColor color;
};



// Class stClassColor
///////////////////////

// Constructor, destructor
////////////////////////////

stClassColor::stClassColor(ScriptingSmalltalk *st){
	if(!st) DETHROW(deeInvalidParam);
	
	pST = st;
	
	// register ccalls
	gst_define_cfunc("DEColor.New1", (void*)ccNew1);
	gst_define_cfunc("DEColor.New2", (void*)ccNew2);
	gst_define_cfunc("DEColor.NewInt1", (void*)ccNewInt1);
	gst_define_cfunc("DEColor.NewInt2", (void*)ccNewInt2);
	
	gst_define_cfunc("DEColor.Red", (void*)ccRed);
	gst_define_cfunc("DEColor.Green", (void*)ccGreen);
	gst_define_cfunc("DEColor.Blue", (void*)ccBlue);
	gst_define_cfunc("DEColor.Alpha", (void*)ccAlpha);
	gst_define_cfunc("DEColor.Clamp", (void*)ccClamp);
	gst_define_cfunc("DEColor.Opaque", (void*)ccOpaque);
	gst_define_cfunc("DEColor.Transparent", (void*)ccTransparent);
	gst_define_cfunc("DEColor.Invisible", (void*)ccInvisible);
	gst_define_cfunc("DEColor.Copy", (void*)ccCopy);
	
	gst_define_cfunc("DEColor.Equal", (void*)ccEqual);
	gst_define_cfunc("DEColor.Add", (void*)ccAdd);
	gst_define_cfunc("DEColor.Subtract", (void*)ccSubtract);
	gst_define_cfunc("DEColor.Multiply", (void*)ccMultiply);
	gst_define_cfunc("DEColor.Divide", (void*)ccDivide);
	gst_define_cfunc("DEColor.ScalarMultiply", (void*)ccScalarMultiply);
	gst_define_cfunc("DEColor.ScalarDivide", (void*)ccScalarDivide);
	
	gst_define_cfunc("DEColor.Hash", (void*)ccHash);
	gst_define_cfunc("DEColor.AsString", (void*)ccAsString);
}

stClassColor::~stClassColor(){
}



// Management
///////////////

void stClassColor::SetUpLinks(){
	pOOPClass = gst_class_name_to_oop("DEColor");
	if(!pOOPClass) DETHROW(deeInvalidParam);
	
	csClrClass &csclass = *((csClrClass*)OOP_TO_OBJ(pOOPClass));
	csclass.scripting = pST->GetClassScripting()->GetSingleton();
}

const decColor &stClassColor::OOPToColor(OOP object) const{
	if(gst_get_object_class(object) != pOOPClass) DETHROW(deeInvalidAction);
	if(object == pST->GetNil()) DETHROW(deeNullPointer);
	
	return ((csClr*)OOP_TO_OBJ(object))->color;
}

OOP stClassColor::ColorToOOP(const decColor &color){
	OOP oopColor = NULL;
	
	oopColor = pST->CreateNewObjectWithInit(pOOPClass, sizeof(csClr) - sizeof(csObject));
	if(!oopColor || oopColor == pST->GetNil()) DETHROW(deeOutOfMemory);
	
	csClr &csobject = *((csClr*)OOP_TO_OBJ(oopColor));
	csobject.clsClr = this;
	csobject.color = color;
	
	return oopColor;
}



// cCall Methods
//////////////////

OOP stClassColor::ccNew1(OOP self, double red, double green, double blue){
	csClrClass &csclass = *((csClrClass*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	OOP result = NULL;
	
	try{
		result = st.GetClassColor()->ColorToOOP(decColor((float)red, (float)green, (float)blue));
		
	}catch(const deException &e){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}

OOP stClassColor::ccNew2(OOP self, double red, double green, double blue, double alpha){
	csClrClass &csclass = *((csClrClass*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	OOP result = NULL;
	
	try{
		result = st.GetClassColor()->ColorToOOP(decColor((float)red, (float)green, (float)blue, (float)alpha));
		
	}catch(const deException &e){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}

OOP stClassColor::ccNewInt1(OOP self, int red, int green, int blue){
	csClrClass &csclass = *((csClrClass*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	OOP result = NULL;
	
	try{
		result = st.GetClassColor()->ColorToOOP(decColor((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f));
		
	}catch(const deException &e){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}

OOP stClassColor::ccNewInt2(OOP self, int red, int green, int blue, int alpha){
	csClrClass &csclass = *((csClrClass*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	OOP result = NULL;
	
	try{
		result = st.GetClassColor()->ColorToOOP(decColor((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f, (float)alpha / 255.0f));
		
	}catch(const deException &e){
		result = st.GetNil();
		e.PrintError();
	}
	
	return result;
}



double stClassColor::ccRed(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return (double)csobject.color.r;
}

double stClassColor::ccGreen(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return (double)csobject.color.g;
}

double stClassColor::ccBlue(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return (double)csobject.color.b;
}

double stClassColor::ccAlpha(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return (double)csobject.color.a;
}

OOP stClassColor::ccClamp(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	OOP result = NULL;
	
	decColor newColor(csobject.color);
	newColor.Normalize();
	
	try{
		result = st.GetClassColor()->ColorToOOP(newColor);
		
	}catch(const deException &e){
		e.PrintError();
		result = st.GetNil();
	}
	
	return result;
}

bool stClassColor::ccOpaque(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return csobject.color.a > 0.99999f;
}

bool stClassColor::ccTransparent(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return csobject.color.a >= 0.00001f && csobject.color.a <= 0.99999f;
}

bool stClassColor::ccInvisible(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return csobject.color.a < 0.00001f;
}

OOP stClassColor::ccCopy(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	OOP result = NULL;
	
	try{
		result = st.GetClassColor()->ColorToOOP(csobject.color);
		
	}catch(const deException &e){
		e.PrintError();
		result = st.GetNil();
	}
	
	return result;
}



bool stClassColor::ccEqual(OOP self, OOP other){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	const decColor &otherColor = st.GetClassColor()->OOPToColor(other);
	
	return csobject.color.IsEqualTo(otherColor);
}

OOP stClassColor::ccAdd(OOP self, OOP other){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	const decColor &otherColor = st.GetClassColor()->OOPToColor(other);
	
	return st.GetClassColor()->ColorToOOP(csobject.color + otherColor);
}

OOP stClassColor::ccSubtract(OOP self, OOP other){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	const decColor &otherColor = st.GetClassColor()->OOPToColor(other);
	
	return st.GetClassColor()->ColorToOOP(csobject.color - otherColor);
}

OOP stClassColor::ccMultiply(OOP self, OOP other){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	const decColor &otherColor = st.GetClassColor()->OOPToColor(other);
	
	return st.GetClassColor()->ColorToOOP(decColor(csobject.color.r * otherColor.r,
		csobject.color.g * otherColor.g,
		csobject.color.b * otherColor.b,
		csobject.color.a * otherColor.a));
}

OOP stClassColor::ccDivide(OOP self, OOP other){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	const decColor &otherColor = st.GetClassColor()->OOPToColor(other);
	
	return st.GetClassColor()->ColorToOOP(decColor(csobject.color.r / otherColor.r,
		csobject.color.g / otherColor.g,
		csobject.color.b / otherColor.b,
		csobject.color.a / otherColor.a));
}

OOP stClassColor::ccScalarMultiply(OOP self, double scalar){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	
	return st.GetClassColor()->ColorToOOP(csobject.color * (float)scalar);
}

OOP stClassColor::ccScalarDivide(OOP self, double scalar){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	ScriptingSmalltalk &st = *csobject.clsClr->GetST();
	
	return st.GetClassColor()->ColorToOOP(csobject.color / (float)scalar);
}




int stClassColor::ccHash(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	return (int)(csobject.color.r * 1000000.0f)
		+ (int)(csobject.color.g * 1000000.0f)
		+ (int)(csobject.color.b * 1000000.0f)
		+ (int)(csobject.color.a * 1000000.0f);
}

OOP stClassColor::ccAsString(OOP self){
	csClr &csobject = *((csClr*)OOP_TO_OBJ(self));
	
	char buffer[50];
	sprintf((char*)&buffer, "(%f,%f,%f,%f)", csobject.color.r, csobject.color.g, csobject.color.b, csobject.color.a);
	
	//printf( "buffer '%s'\n", buffer );
	return gst_string_to_oop(buffer);
}
