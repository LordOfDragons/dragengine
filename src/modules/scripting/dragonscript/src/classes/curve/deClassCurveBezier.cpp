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

#include "deClassCurveBezier.h"
#include "../math/deClassVector2.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sCBeNatDat{
	decCurveBezier *curve;
};



// Native functions
/////////////////////

// public func new()
deClassCurveBezier::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCBe,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCBeNatDat &nd = *((sCBeNatDat*)p_GetNativeData(myself));
	
	nd.curve = NULL;
	
	nd.curve = new decCurveBezier;
	if(!nd.curve) DSTHROW(dueOutOfMemory);
}

// public func new(CurveBezier curve)
deClassCurveBezier::nfNewCopy::nfNewCopy(const sInitData &init) :
dsFunction(init.clsCBe, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCBe);
}
void deClassCurveBezier::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sCBeNatDat &nd = *((sCBeNatDat*)p_GetNativeData(myself));
	const deClassCurveBezier &clsCBe = *((deClassCurveBezier*)GetOwnerClass());
	
	nd.curve = NULL;
	
	nd.curve = new decCurveBezier(clsCBe.GetCurve(rt->GetValue(0)->GetRealObject()));
}

// public static func newDefaultLinear()
deClassCurveBezier::nfNewDefaultLinear::nfNewDefaultLinear(const sInitData &init) :
dsFunction(init.clsCBe, "newDefaultLinear", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCBe){
}
void deClassCurveBezier::nfNewDefaultLinear::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(GetOwnerClass());
	sCBeNatDat &nd = *((sCBeNatDat*)p_GetNativeData(rt->GetValue(0)));
	nd.curve = NULL;
	
	try{
		nd.curve = new decCurveBezier();
		nd.curve->SetDefaultLinear();
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

// public static func newDefaultBezier()
deClassCurveBezier::nfNewDefaultBezier::nfNewDefaultBezier(const sInitData &init) :
dsFunction(init.clsCBe, "newDefaultBezier", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCBe){
}
void deClassCurveBezier::nfNewDefaultBezier::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(GetOwnerClass());
	sCBeNatDat &nd = *((sCBeNatDat*)p_GetNativeData(rt->GetValue(0)));
	nd.curve = NULL;
	
	try{
		nd.curve = new decCurveBezier();
		nd.curve->SetDefaultBezier();
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}

// public func destructor()
deClassCurveBezier::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCBe,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCBeNatDat &nd = *((sCBeNatDat*)p_GetNativeData(myself));
	
	if(nd.curve){
		delete nd.curve;
		nd.curve = NULL;
	}
}



// Management
///////////////

// public func int getPointCount()
deClassCurveBezier::nfGetPointCount::nfGetPointCount(const sInitData &init) : dsFunction(init.clsCBe,
"getPointCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCurveBezier::nfGetPointCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.GetPointCount());
}

// public func Vector2 getPointAt( int position )
deClassCurveBezier::nfGetPointAt::nfGetPointAt(const sInitData &init) : dsFunction(init.clsCBe,
"getPointAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // position
}
void deClassCurveBezier::nfGetPointAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript *ds = ((deClassCurveBezier*)GetOwnerClass())->GetDS();
	
	int position = rt->GetValue(0)->GetInt();
	
	ds->GetClassVector2()->PushVector2(rt, curve.GetPointAt(position).GetPoint());
}

// public func Vector2 getHandle1At( int position )
deClassCurveBezier::nfGetHandle1At::nfGetHandle1At(const sInitData &init) :
dsFunction(init.clsCBe, "getHandle1At", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // position
}
void deClassCurveBezier::nfGetHandle1At::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	const deScriptingDragonScript &ds = *(((deClassCurveBezier*)GetOwnerClass())->GetDS());
	
	int position = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector2()->PushVector2(rt, curve.GetPointAt(position).GetHandle1());
}

// public func Vector2 getHandle2At( int position )
deClassCurveBezier::nfGetHandle2At::nfGetHandle2At(const sInitData &init) :
dsFunction(init.clsCBe, "getHandle2At", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // position
}
void deClassCurveBezier::nfGetHandle2At::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	const deScriptingDragonScript &ds = *(((deClassCurveBezier*)GetOwnerClass())->GetDS());
	
	int position = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector2()->PushVector2(rt, curve.GetPointAt(position).GetHandle2());
}

// public func int findPointPriorTo( float coordinate )
deClassCurveBezier::nfFindPointPriorTo::nfFindPointPriorTo(const sInitData &init) : dsFunction(init.clsCBe,
"findPointPriorTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsFlt); // coordinate
}
void deClassCurveBezier::nfFindPointPriorTo::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.FindPointPriorTo(rt->GetValue(0)->GetFloat()));
}

// public func int addPoint( Vector2 point, Vector2 handle1, Vector2 handle2 )
deClassCurveBezier::nfAddPoint::nfAddPoint(const sInitData &init) : dsFunction(init.clsCBe,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsVec2); // point
	p_AddParameter(init.clsVec2); // handle1
	p_AddParameter(init.clsVec2); // handle2
}
void deClassCurveBezier::nfAddPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript *ds = ((deClassCurveBezier*)GetOwnerClass())->GetDS();
	const deClassVector2 &clsVec2 = *ds->GetClassVector2();
	
	dsRealObject *objPoint = rt->GetValue(0)->GetRealObject();
	dsRealObject *objHandle1 = rt->GetValue(1)->GetRealObject();
	dsRealObject *objHandle2 = rt->GetValue(2)->GetRealObject();
	if(!objPoint || !objHandle1 || !objHandle2) DSTHROW(dueNullPointer);
	
	const decVector2 &point = clsVec2.GetVector2(objPoint);
	const decVector2 &handle1 = clsVec2.GetVector2(objHandle1);
	const decVector2 &handle2 = clsVec2.GetVector2(objHandle2);
	
	rt->PushInt(curve.AddPoint(decCurveBezierPoint(point, handle1, handle2)));
}

// public func int addPoint( float pointX, float pointY, float handle1X, float handle1Y, float handle2X, float handle2Y )
deClassCurveBezier::nfAddPoint2::nfAddPoint2(const sInitData &init) : dsFunction(init.clsCBe,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsFlt); // pointX
	p_AddParameter(init.clsFlt); // pointY
	p_AddParameter(init.clsFlt); // handle1X
	p_AddParameter(init.clsFlt); // handle1Y
	p_AddParameter(init.clsFlt); // handle2X
	p_AddParameter(init.clsFlt); // handle2Y
}
void deClassCurveBezier::nfAddPoint2::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	float pointX = rt->GetValue(0)->GetFloat();
	float pointY = rt->GetValue(1)->GetFloat();
	float handle1X = rt->GetValue(2)->GetFloat();
	float handle1Y = rt->GetValue(3)->GetFloat();
	float handle2X = rt->GetValue(4)->GetFloat();
	float handle2Y = rt->GetValue(5)->GetFloat();
	
	rt->PushInt(curve.AddPoint(decCurveBezierPoint(decVector2(pointX, pointY),
		decVector2(handle1X, handle1Y), decVector2(handle2X, handle2Y))));
}

// public func int addPoint( Vector2 point )
deClassCurveBezier::nfAddPoint3::nfAddPoint3(const sInitData &init) : dsFunction(init.clsCBe,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsVec2); // point
}
void deClassCurveBezier::nfAddPoint3::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	const deScriptingDragonScript &ds = *(((deClassCurveBezier*)GetOwnerClass())->GetDS());
	
	const decVector2 &point = ds.GetClassVector2()->GetVector2(rt->GetValue(0)->GetRealObject());
	
	rt->PushInt(curve.AddPoint(decCurveBezierPoint(point, point, point)));
}

// public func int addPoint( float pointX, float pointY )
deClassCurveBezier::nfAddPoint4::nfAddPoint4(const sInitData &init) : dsFunction(init.clsCBe,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsFlt); // pointX
	p_AddParameter(init.clsFlt); // pointY
}
void deClassCurveBezier::nfAddPoint4::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	const float pointX = rt->GetValue(0)->GetFloat();
	const float pointY = rt->GetValue(1)->GetFloat();
	const decVector2 point(pointX, pointY);
	
	rt->PushInt(curve.AddPoint(decCurveBezierPoint(point, point, point)));
}

// public func void removePointFrom( int position )
deClassCurveBezier::nfRemovePointFrom::nfRemovePointFrom(const sInitData &init) : dsFunction(init.clsCBe,
"removePointFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // position
}
void deClassCurveBezier::nfRemovePointFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	curve.RemovePointFrom(rt->GetValue(0)->GetInt());
}

// public func void removeAllPoints()
deClassCurveBezier::nfRemoveAllPoints::nfRemoveAllPoints(const sInitData &init) : dsFunction(init.clsCBe,
"removeAllPoints", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier::nfRemoveAllPoints::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	curve.RemoveAllPoints();
}

// public func CurveBezierInterpolation getInterpolationMode()
deClassCurveBezier::nfGetInterpolationMode::nfGetInterpolationMode(const sInitData &init) : dsFunction(init.clsCBe,
"getInterpolationMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCurveBezierInterpolation){
}
void deClassCurveBezier::nfGetInterpolationMode::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushValue(((deClassCurveBezier*)GetOwnerClass())->GetClassCurveBezierInterpolation()
		->GetVariable(curve.GetInterpolationMode())->GetStaticValue());
}

// public func void setInterpolationMode( CurveBezierInterpolation mode )
deClassCurveBezier::nfSetInterpolationMode::nfSetInterpolationMode(const sInitData &init) : dsFunction(init.clsCBe,
"setInterpolationMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCurveBezierInterpolation); // mode
}
void deClassCurveBezier::nfSetInterpolationMode::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	curve.SetInterpolationMode((decCurveBezier::eInterpolationModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void setDefaultLinear()
deClassCurveBezier::nfSetDefaultLinear::nfSetDefaultLinear(const sInitData &init) :
dsFunction(init.clsCBe, "setDefaultLinear", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier::nfSetDefaultLinear::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	curve.SetDefaultLinear();
}

// public func void setDefaultBezier()
deClassCurveBezier::nfSetDefaultBezier::nfSetDefaultBezier(const sInitData &init) :
dsFunction(init.clsCBe, "setDefaultBezier", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier::nfSetDefaultBezier::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	curve.SetDefaultBezier();
}



// public func float evaluateAt( float coordinate )
deClassCurveBezier::nfEvaluateAt::nfEvaluateAt(const sInitData &init) : dsFunction(init.clsCBe,
"evaluateAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsFlt); // coordinate
}
void deClassCurveBezier::nfEvaluateAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier *curve = ((sCBeNatDat*)p_GetNativeData(myself))->curve;
	decCurveBezierEvaluator evaluator(*curve);
	
	rt->PushFloat(evaluator.EvaluateAt(rt->GetValue(0)->GetFloat()));
}



// static public func CurveBezier readFromFile(FileReader reader)
deClassCurveBezier::nfReadFromFile::nfReadFromFile(const sInitData &init) :
dsFunction(init.clsCBe, "readFromFile", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCBe){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassCurveBezier::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassCurveBezier &clsCurveBezier = *((deClassCurveBezier*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *clsCurveBezier.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	const int version = reader->ReadByte();
	decCurveBezier curve;
	uint32_t i, count;
	
	switch(version){
	case 0:
		curve.SetInterpolationMode((decCurveBezier::eInterpolationModes)reader->ReadByte());
		count = reader->ReadVarUInt();
		for(i=0; i<count; i++){
			const decVector2 point(reader->ReadVector2());
			const decVector2 handle1(reader->ReadVector2());
			const decVector2 handle2(reader->ReadVector2());
			curve.AddPoint({point, handle1, handle2});
		}
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	clsCurveBezier.PushCurve(rt, curve);
}

// public func void writeToFile(FileWriter writer)
deClassCurveBezier::nfWriteToFile::nfWriteToFile(const sInitData &init) :
dsFunction(init.clsCBe, "writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassCurveBezier::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	const deClassCurveBezier &clsCurveBezier = *((deClassCurveBezier*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *clsCurveBezier.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WriteByte(0); // version 0
	writer->WriteByte((uint8_t)curve.GetInterpolationMode());
	
	const int count = curve.GetPointCount();
	writer->WriteVarUInt((uint32_t)count);
	
	int i;
	for(i=0; i<count; i++){
		const decCurveBezierPoint &point = curve.GetPointAt(i);
		writer->WriteVector2(point.GetPoint());
		writer->WriteVector2(point.GetHandle1());
		writer->WriteVector2(point.GetHandle2());
	}
}



// public func int hashCode()
deClassCurveBezier::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCBe, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCurveBezier::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.GetPointCount());
}

// public func bool equals( Object obj )
deClassCurveBezier::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCBe, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCurveBezier::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier &curve = *((sCBeNatDat*)p_GetNativeData(myself))->curve;
	deClassCurveBezier *clsCBe = (deClassCurveBezier*)GetOwnerClass();
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsCBe)){
		rt->PushBool(false);
		
	}else{
		const decCurveBezier &other = *((sCBeNatDat*)p_GetNativeData(obj))->curve;
		
		if(curve.GetPointCount() == other.GetPointCount()){
			
			int p, count = curve.GetPointCount();
			bool equal = true;
			
			for(p=0; p<count; p++){
				const decCurveBezierPoint &pt1 = curve.GetPointAt(p);
				const decCurveBezierPoint &pt2 = other.GetPointAt(p);
				
				if(!pt1.GetPoint().IsEqualTo(pt2.GetPoint())){
					equal = false;
					break;
				}
				
				if(!pt1.GetHandle1().IsEqualTo(pt2.GetHandle1())){
					equal = false;
					break;
				}
				
				if(!pt1.GetHandle2().IsEqualTo(pt2.GetHandle2())){
					equal = false;
					break;
				}
			}
			
			rt->PushBool(equal);
			
		}else{
			rt->PushBool(false);
		}
	}
}




// Class deClassCurveBezier
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassCurveBezier::deClassCurveBezier(deScriptingDragonScript *ds) :
dsClass("CurveBezier", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds) DSTHROW(dueInvalidParam);
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCBeNatDat));
}

deClassCurveBezier::~deClassCurveBezier(){
}



// Management
///////////////

void deClassCurveBezier::CreateClassMembers(dsEngine *engine){
	// store classes
	pClsCurveBezierInterpolation = engine->GetClass("Dragengine.Scenery.CurveBezierInterpolation");
	
	sInitData init;
	init.clsCBe = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec2 = pDS->GetClassVector2();
	init.clsCurveBezierInterpolation = pClsCurveBezierInterpolation;
	init.clsFileReader = pDS->GetClassFileReader();
	init.clsFileWriter = pDS->GetClassFileWriter();
	
	// add constants
	AddConstant(new dsConstant("CONSTANT", init.clsInt, decCurveBezier::eimConstant));
	AddConstant(new dsConstant("LINEAR", init.clsInt, decCurveBezier::eimLinear));
	AddConstant(new dsConstant("BEZIER", init.clsInt, decCurveBezier::eimBezier));
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfNewDefaultLinear(init));
	AddFunction(new nfNewDefaultBezier(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPointCount(init));
	AddFunction(new nfGetPointAt(init));
	AddFunction(new nfGetHandle1At(init));
	AddFunction(new nfGetHandle2At(init));
	AddFunction(new nfFindPointPriorTo(init));
	AddFunction(new nfAddPoint(init));
	AddFunction(new nfAddPoint2(init));
	AddFunction(new nfAddPoint3(init));
	AddFunction(new nfAddPoint4(init));
	AddFunction(new nfRemovePointFrom(init));
	AddFunction(new nfRemoveAllPoints(init));
	AddFunction(new nfGetInterpolationMode(init));
	AddFunction(new nfSetInterpolationMode(init));
	AddFunction(new nfSetDefaultLinear(init));
	AddFunction(new nfSetDefaultBezier(init));
	
	AddFunction(new nfEvaluateAt(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

const decCurveBezier &deClassCurveBezier::GetCurve(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return *((sCBeNatDat*)p_GetNativeData(myself->GetBuffer()))->curve;
}

void deClassCurveBezier::PushCurve(dsRunTime *rt, const decCurveBezier &curve){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sCBeNatDat &nd = *((sCBeNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.curve = nullptr;
	
	try{
		nd.curve = new decCurveBezier(curve);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
