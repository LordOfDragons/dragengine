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
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassPoint.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// native structure
struct sPtNatDat{
	decPoint point;
};



// native functions
/////////////////////

// constructors, destructor
/////////////////////////////

// public func new()
deClassPoint::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsPt,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassPoint::nfNew::RunFunction(dsRunTime *RT, dsValue *This){
	decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	point.Set(0, 0);
}

// public func new( int x, int y )
deClassPoint::nfNew2::nfNew2(const sInitData &init) : dsFunction(init.clsPt,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // x
	p_AddParameter(init.clsInt); // y
}
void deClassPoint::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	point.x = rt->GetValue(0)->GetInt();
	point.y = rt->GetValue(1)->GetInt();
}

// public func new( Point p )
deClassPoint::nfNew3::nfNew3(const sInitData &init) : dsFunction(init.clsPt,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPt); // p
}
void deClassPoint::nfNew3::RunFunction(dsRunTime *RT, dsValue *This){
	decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	point = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
}

// public func destructor()
deClassPoint::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsPt,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassPoint::nfDestructor::RunFunction(dsRunTime *RT, dsValue *This){
}



// accessors
//////////////

// public func int getX()
deClassPoint::nfGetX::nfGetX(const sInitData &init) : dsFunction(init.clsPt,
"getX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassPoint::nfGetX::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	RT->PushInt(point.x);
}

// public func int getY()
deClassPoint::nfGetY::nfGetY(const sInitData &init) : dsFunction(init.clsPt,
"getY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassPoint::nfGetY::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	RT->PushInt(point.y);
}

// public func Point smallest( Point p )
deClassPoint::nfSmallest::nfSmallest(const sInitData &init) : dsFunction(init.clsPt,
"smallest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsPt); // p
}
void deClassPoint::nfSmallest::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	
	const decPoint &p = clsPoint.GetPoint(rt->GetValue(0)->GetRealObject());
	
	clsPoint.PushPoint(rt, decPoint(decMath::min(point.x, p.x), decMath::min(point.y, p.y)));
}

// public func Point smallest( int x, int y )
deClassPoint::nfSmallest2::nfSmallest2(const sInitData &init) : dsFunction(init.clsPt,
"smallest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsInt); // x
	p_AddParameter(init.clsInt); // y
}
void deClassPoint::nfSmallest2::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	
	clsPoint.PushPoint(rt, decPoint(decMath::min(point.x, x), decMath::min(point.y, y)));
}

// public func Point largest( Point p )
deClassPoint::nfLargest::nfLargest(const sInitData &init) : dsFunction(init.clsPt,
"largest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsPt); // p
}
void deClassPoint::nfLargest::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	const decPoint &p = clsPoint.GetPoint(rt->GetValue(0)->GetRealObject());
	clsPoint.PushPoint(rt, decPoint(decMath::max(point.x, p.x), decMath::max(point.y, p.y)));
}

// public func Point largest( int x, int y )
deClassPoint::nfLargest2::nfLargest2(const sInitData &init) : dsFunction(init.clsPt,
"largest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsInt); // x
	p_AddParameter(init.clsInt); // y
}
void deClassPoint::nfLargest2::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	const int x = rt->GetValue(0)->GetInt();
	const int y = rt->GetValue(1)->GetInt();
	
	clsPoint.PushPoint(rt, decPoint(decMath::max(point.x, x), decMath::max(point.y, y)));
}

// public func Point absolute()
deClassPoint::nfAbsolute::nfAbsolute(const sInitData &init) : dsFunction(init.clsPt,
"absolute", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
}
void deClassPoint::nfAbsolute::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	clsPoint.PushPoint(rt, point.Absolute());
}



// testing
////////////

// public func bool isEqualTo( Point p )
deClassPoint::nfIsEqualTo::nfIsEqualTo(const sInitData &init) : dsFunction(init.clsPt,
"isEqualTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsPt); // v
}
void deClassPoint::nfIsEqualTo::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	const decPoint &p = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
	RT->PushBool(point == p);
}

// public func bool isAtLeast( int value )
deClassPoint::nfIsAtLeast::nfIsAtLeast(const sInitData &init) : dsFunction(init.clsPt,
"isAtLeast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsInt); // value
}
void deClassPoint::nfIsAtLeast::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	int value = RT->GetValue(0)->GetInt();
	RT->PushBool(point.x >= value && point.y >= value);
}

// public func bool isAtMost( int value )
deClassPoint::nfIsAtMost::nfIsAtMost(const sInitData &init) : dsFunction(init.clsPt,
"isAtMost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsInt); // value
}
void deClassPoint::nfIsAtMost::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	int value = RT->GetValue(0)->GetInt();
	RT->PushBool(point.x <= value && point.y <= value);
}

// public func bool isZero()
deClassPoint::nfIsZero::nfIsZero(const sInitData &init) :
dsFunction(init.clsPt, "isZero", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassPoint::nfIsZero::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	RT->PushBool(point == decPoint());
}



// public func Point compMultiply( Point point )
deClassPoint::nfCompMultiply::nfCompMultiply(const sInitData &init) : dsFunction(init.clsPt,
"compMultiply", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsPt); // point
}
void deClassPoint::nfCompMultiply::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	
	const decPoint &argPoint = clsPoint.GetPoint(rt->GetValue(0)->GetRealObject());
	
	clsPoint.PushPoint(rt, decPoint(point.x * argPoint.x, point.y * argPoint.y));
}

// public func Point compDivide( Point point )
deClassPoint::nfCompDivide::nfCompDivide(const sInitData &init) : dsFunction(init.clsPt,
"compDivide", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsPt); // point
}
void deClassPoint::nfCompDivide::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	
	const decPoint &argPoint = clsPoint.GetPoint(rt->GetValue(0)->GetRealObject());
	if(argPoint.x == 0 || argPoint.y == 0){
		DSTHROW(dueDivisionByZero);
	}
	
	clsPoint.PushPoint(rt, decPoint(point.x / argPoint.x, point.y / argPoint.y));
}



// File Handling
//////////////////

// static public func Point readFromFile( FileReader reader )
deClassPoint::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsPt,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsPt){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassPoint::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *clsPoint.GetScriptModule()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	decPoint point;
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	clsPoint.PushPoint(rt, reader->ReadPoint());
}

// public func void writeToFile( FileWriter writer )
deClassPoint::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsPt,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassPoint::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	const deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *clsPoint.GetScriptModule()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WritePoint(point);
}



// operators
//////////////

// public func Point -()
deClassPoint::nfOpMinus::nfOpMinus(const sInitData &init) : dsFunction(init.clsPt,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
}
void deClassPoint::nfOpMinus::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	clsPoint->PushPoint(RT, -point);
}

// public func Point +( Point v )
deClassPoint::nfOpAdd::nfOpAdd(const sInitData &init) : dsFunction(init.clsPt,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsPt); // v
}
void deClassPoint::nfOpAdd::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	const decPoint &p = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
	clsPoint->PushPoint(RT, point + p);
}

// public func Point -( Point v )
deClassPoint::nfOpSubtract::nfOpSubtract(const sInitData &init) : dsFunction(init.clsPt,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsPt); // v
}
void deClassPoint::nfOpSubtract::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	const decPoint &p = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
	clsPoint->PushPoint(RT, point - p);
}

// public func Point *( int k )
deClassPoint::nfOpScale::nfOpScale(const sInitData &init) : dsFunction(init.clsPt,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsInt); // k
}
void deClassPoint::nfOpScale::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	clsPoint->PushPoint(RT, point * RT->GetValue(0)->GetInt());
}

// public func Point /( int k )
deClassPoint::nfOpDivide::nfOpDivide(const sInitData &init) : dsFunction(init.clsPt,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
	p_AddParameter(init.clsInt); // k
}
void deClassPoint::nfOpDivide::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	clsPoint->PushPoint(RT, point / RT->GetValue(0)->GetInt());
}

// public func int *( Point point )
deClassPoint::nfOpMultiply::nfOpMultiply(const sInitData &init) : dsFunction(init.clsPt,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsPt); // point
}
void deClassPoint::nfOpMultiply::RunFunction(dsRunTime *rt, dsValue *myself){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(myself))->point;
	deClassPoint &clsPoint = *((deClassPoint*)GetOwnerClass());
	
	const decPoint &argPoint = clsPoint.GetPoint(rt->GetValue(0)->GetRealObject());
	
	rt->PushInt(point * argPoint);
}

// public func bool <( Point v )
deClassPoint::nfOpLess::nfOpLess(const sInitData &init) : dsFunction(init.clsPt,
"<", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsPt); // v
}
void deClassPoint::nfOpLess::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	const decPoint &p = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
	RT->PushBool(point < p);
}

// public func bool <=( Point v )
deClassPoint::nfOpLessEqual::nfOpLessEqual(const sInitData &init) : dsFunction(init.clsPt,
"<=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsPt); // v
}
void deClassPoint::nfOpLessEqual::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	const decPoint &p = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
	RT->PushBool(point <= p);
}

// public func bool >( Point v )
deClassPoint::nfOpGreater::nfOpGreater(const sInitData &init) : dsFunction(init.clsPt,
">", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsPt); // v
}
void deClassPoint::nfOpGreater::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	const decPoint &p = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
	RT->PushBool(point > p);
}

// public func bool >=( Point v )
deClassPoint::nfOpGreaterEqual::nfOpGreaterEqual(const sInitData &init) : dsFunction(init.clsPt,
">=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsPt); // v
}
void deClassPoint::nfOpGreaterEqual::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	const decPoint &p = clsPoint->GetPoint(RT->GetValue(0)->GetRealObject());
	RT->PushBool(point >= p);
}



// special functions
//////////////////////

// public func bool equals( Object other )
deClassPoint::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsPt,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassPoint::nfEquals::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	deClassPoint *clsPoint = (deClassPoint*)GetOwnerClass();
	dsValue *obj = RT->GetValue(0);
	if(!p_IsObjOfType(obj, clsPoint)){
		RT->PushBool(false);
	}else{
		const decPoint &otherPoint = ((sPtNatDat*)p_GetNativeData(obj))->point;
		RT->PushBool(point == otherPoint);
	}
}

// public func int hashCode()
deClassPoint::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsPt,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassPoint::nfHashCode::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	RT->PushInt((int)(point.x * 1000000)
		+ (int)(point.y * 1000));
}

// public func String toString()
deClassPoint::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsPt,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassPoint::nfToString::RunFunction(dsRunTime *RT, dsValue *This){
	const decPoint &point = ((sPtNatDat*)p_GetNativeData(This))->point;
	decString text;
	text.Format("(%i,%i)", point.x, point.y);
	RT->PushString(text);
}



// Class deClassPoint
////////////////////////

// Constructor
////////////////

deClassPoint::deClassPoint(deEngine *gameEngine, deScriptingDragonScript *scriptManager) :
dsClass("Point", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!gameEngine || !scriptManager){
		DSTHROW(dueInvalidParam);
	}
	
	pGameEngine = gameEngine;
	pScrMgr = scriptManager;
	
	GetParserInfo()->SetParent(DENS_GUI);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sPtNatDat));
}

deClassPoint::~deClassPoint(){
}



// Management
///////////////

void deClassPoint::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsPt = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFileReader = pScrMgr->GetClassFileReader();
	init.clsFileWriter = pScrMgr->GetClassFileWriter();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfNew3(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetX(init));
	AddFunction(new nfGetY(init));
	AddFunction(new nfSmallest(init));
	AddFunction(new nfSmallest2(init));
	AddFunction(new nfLargest(init));
	AddFunction(new nfLargest2(init));
	AddFunction(new nfAbsolute(init));
	
	AddFunction(new nfIsEqualTo(init));
	AddFunction(new nfIsAtLeast(init));
	AddFunction(new nfIsAtMost(init));
	AddFunction(new nfIsZero(init));
	
	AddFunction(new nfCompMultiply(init));
	AddFunction(new nfCompDivide(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfOpMinus(init));
	AddFunction(new nfOpAdd(init));
	AddFunction(new nfOpSubtract(init));
	AddFunction(new nfOpScale(init));
	AddFunction(new nfOpDivide(init));
	AddFunction(new nfOpMultiply(init));
	AddFunction(new nfOpLess(init));
	AddFunction(new nfOpLessEqual(init));
	AddFunction(new nfOpGreater(init));
	AddFunction(new nfOpGreaterEqual(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	AddFunction(new nfToString(init));
}

const decPoint &deClassPoint::GetPoint(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	return ((sPtNatDat*)p_GetNativeData(myself->GetBuffer()))->point;
}

void deClassPoint::PushPoint(dsRunTime *rt, const decPoint &point){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sPtNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->point = point;
}
