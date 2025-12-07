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

#include "deClassCurveBezier3D.h"
#include "../math/deClassVector.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/common/curve/decCurveBezier3D.h>
#include <dragengine/common/curve/decCurveBezier3DPoint.h>
#include <dragengine/common/curve/decCurveBezier3DEvaluator.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native Structure
/////////////////////

struct sCBezier3DNatDat{
	decCurveBezier3D *curve;
	decCurveBezier3DEvaluator *evaluator;
};



// Native functions
/////////////////////

// public func new()
deClassCurveBezier3D::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCBezier3D,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier3D::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCBezier3DNatDat &nd = *((sCBezier3DNatDat*)p_GetNativeData(myself));
	
	nd.curve = NULL;
	nd.evaluator = NULL;
	
	nd.curve = new decCurveBezier3D;
	nd.evaluator = new decCurveBezier3DEvaluator(*nd.curve);
}

// public func new( CurveBezier3D copy )
deClassCurveBezier3D::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsCBezier3D,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCBezier3D); // copy
}
void deClassCurveBezier3D::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sCBezier3DNatDat &nd = *((sCBezier3DNatDat*)p_GetNativeData(myself));
	deClassCurveBezier3D &clsCBezier3D = *((deClassCurveBezier3D*)GetOwnerClass());
	
	nd.curve = NULL;
	nd.evaluator = NULL;
	
	const decCurveBezier3D &copy = clsCBezier3D.GetCurve(rt->GetValue(0)->GetRealObject());
	
	nd.curve = new decCurveBezier3D(copy);
	nd.evaluator = new decCurveBezier3DEvaluator(*nd.curve);
}

// public func destructor()
deClassCurveBezier3D::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCBezier3D,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier3D::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCBezier3DNatDat &nd = *((sCBezier3DNatDat*)p_GetNativeData(myself));
	
	if(nd.evaluator){
		delete nd.evaluator;
		nd.evaluator = NULL;
	}
	if(nd.curve){
		delete nd.curve;
		nd.curve = NULL;
	}
}



// Management
///////////////

// public func int getPointCount()
deClassCurveBezier3D::nfGetPointCount::nfGetPointCount(const sInitData &init) : dsFunction(init.clsCBezier3D,
"getPointCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassCurveBezier3D::nfGetPointCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.GetPointCount());
}

// public func Vector getPointAt( int index )
deClassCurveBezier3D::nfGetPointAt::nfGetPointAt(const sInitData &init) : dsFunction(init.clsCBezier3D,
"getPointAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
	p_AddParameter(init.clsInteger); // index
}
void deClassCurveBezier3D::nfGetPointAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	const deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, curve.GetPointAt(index).GetPoint());
}

// public func Vector getHandle1At( int index )
deClassCurveBezier3D::nfGetHandle1At::nfGetHandle1At(const sInitData &init) : dsFunction(init.clsCBezier3D,
"getHandle1At", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
	p_AddParameter(init.clsInteger); // index
}
void deClassCurveBezier3D::nfGetHandle1At::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	const deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, curve.GetPointAt(index).GetHandle1());
}

// public func Vector getHandle2At( int index )
deClassCurveBezier3D::nfGetHandle2At::nfGetHandle2At(const sInitData &init) : dsFunction(init.clsCBezier3D,
"getHandle2At", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
	p_AddParameter(init.clsInteger); // index
}
void deClassCurveBezier3D::nfGetHandle2At::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	const deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	
	ds.GetClassVector()->PushVector(rt, curve.GetPointAt(index).GetHandle2());
}

// public func void setPointAt( int index, Vector point )
deClassCurveBezier3D::nfSetPointAt::nfSetPointAt(const sInitData &init) : dsFunction(init.clsCBezier3D,
"setPointAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // index
	p_AddParameter(init.clsVector); // point
}
void deClassCurveBezier3D::nfSetPointAt::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const decVector &point = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const int index = rt->GetValue(0)->GetInt();
	
	curve.GetPointAt(index).SetPoint(point);
}

// public func void setPointAt( int index, Vector point, Vecctor handle1, Vector handle2 )
deClassCurveBezier3D::nfSetPointAt2::nfSetPointAt2(const sInitData &init) : dsFunction(init.clsCBezier3D,
"setPointAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // index
	p_AddParameter(init.clsVector); // point
	p_AddParameter(init.clsVector); // handle1
	p_AddParameter(init.clsVector); // handle2
}
void deClassCurveBezier3D::nfSetPointAt2::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const decVector &point = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector &handle1 = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decVector &handle2 = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	const int index = rt->GetValue(0)->GetInt();
	
	curve.GetPointAt(index).SetPoint(point, handle1, handle2);
}

// public func void addPoint( Vector point )
deClassCurveBezier3D::nfAddPoint::nfAddPoint(const sInitData &init) : dsFunction(init.clsCBezier3D,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // point
}
void deClassCurveBezier3D::nfAddPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const decVector &point = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	
	curve.AddPoint(decCurveBezier3DPoint(point));
}

// public func void addPoint( Vector point, Vector handle1, Vector handle2 )
deClassCurveBezier3D::nfAddPoint2::nfAddPoint2(const sInitData &init) : dsFunction(init.clsCBezier3D,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // point
	p_AddParameter(init.clsVector); // handle1
	p_AddParameter(init.clsVector); // handle2
}
void deClassCurveBezier3D::nfAddPoint2::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const decVector &point = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	const decVector &handle1 = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector &handle2 = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	
	curve.AddPoint(decCurveBezier3DPoint(point, handle1, handle2));
}

// public func void insertPoint( int index, Vector point )
deClassCurveBezier3D::nfInsertPoint::nfInsertPoint(const sInitData &init) : dsFunction(init.clsCBezier3D,
"insertPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // index
	p_AddParameter(init.clsVector); // point
}
void deClassCurveBezier3D::nfInsertPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const decVector &point = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const int index = rt->GetValue(0)->GetInt();
	
	curve.InsertPoint(index, decCurveBezier3DPoint(point));
}

// public func void insertPoint( int index, Vector point, Vector handle1, Vector handle2 )
deClassCurveBezier3D::nfInsertPoint2::nfInsertPoint2(const sInitData &init) : dsFunction(init.clsCBezier3D,
"insertPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // index
	p_AddParameter(init.clsVector); // point
	p_AddParameter(init.clsVector); // handle1
	p_AddParameter(init.clsVector); // handle2
}
void deClassCurveBezier3D::nfInsertPoint2::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const decVector &point = ds.GetClassVector()->GetVector(rt->GetValue(1)->GetRealObject());
	const decVector &handle1 = ds.GetClassVector()->GetVector(rt->GetValue(2)->GetRealObject());
	const decVector &handle2 = ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject());
	const int index = rt->GetValue(0)->GetInt();
	
	curve.InsertPoint(index, decCurveBezier3DPoint(point, handle1, handle2));
}

// public func void removePointFrom( int position )
deClassCurveBezier3D::nfRemovePointFrom::nfRemovePointFrom(const sInitData &init) : dsFunction(init.clsCBezier3D,
"removePointFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // position
}
void deClassCurveBezier3D::nfRemovePointFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.RemovePointFrom(rt->GetValue(0)->GetInt());
}

// public func void removeAllPoints()
deClassCurveBezier3D::nfRemoveAllPoints::nfRemoveAllPoints(const sInitData &init) : dsFunction(init.clsCBezier3D,
"removeAllPoints", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurveBezier3D::nfRemoveAllPoints::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.RemoveAllPoints();
}



// public func CurveBezierInterpolation getInterpolationMode()
deClassCurveBezier3D::nfGetInterpolationMode::nfGetInterpolationMode(const sInitData &init) :
dsFunction(init.clsCBezier3D, "getInterpolationMode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCurveBezierInterpolation){
}
void deClassCurveBezier3D::nfGetInterpolationMode::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushValue(((deClassCurveBezier3D*)GetOwnerClass())->GetClassCurveBezierInterpolation()
		->GetVariable(curve.GetInterpolationMode())->GetStaticValue());
}

// public func void setInterpolationMode(CurveBezierInterpolation mode)
deClassCurveBezier3D::nfSetInterpolationMode::nfSetInterpolationMode(const sInitData &init) :
dsFunction(init.clsCBezier3D, "setInterpolationMode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCurveBezierInterpolation); // mode
}
void deClassCurveBezier3D::nfSetInterpolationMode::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.SetInterpolationMode((decCurveBezier3D::eInterpolationModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue(0)->GetRealObject()));
}



// public func Vector evaluateAt( int segment, float blend )
deClassCurveBezier3D::nfEvaluateAt::nfEvaluateAt(const sInitData &init) : dsFunction(init.clsCBezier3D,
"evaluateAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
	p_AddParameter(init.clsInteger); // segment
	p_AddParameter(init.clsFloat); // blend
}
void deClassCurveBezier3D::nfEvaluateAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3DEvaluator &evaluator = *((sCBezier3DNatDat*)p_GetNativeData(myself))->evaluator;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const int segment = rt->GetValue(0)->GetInt();
	const float blend = rt->GetValue(1)->GetFloat();
	
	ds.GetClassVector()->PushVector(rt, evaluator.EvaluateAt(segment, blend));
}

// public func Vector evaluateAt( float curveValue )
deClassCurveBezier3D::nfEvaluateAt2::nfEvaluateAt2(const sInitData &init) : dsFunction(init.clsCBezier3D,
"evaluateAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
	p_AddParameter(init.clsFloat); // curveValue
}
void deClassCurveBezier3D::nfEvaluateAt2::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3DEvaluator &evaluator = *((sCBezier3DNatDat*)p_GetNativeData(myself))->evaluator;
	deScriptingDragonScript &ds = ((deClassCurveBezier3D*)GetOwnerClass())->GetDS();
	
	const float curveValue = rt->GetValue(0)->GetFloat();
	float segment;
	const float blend = modff(curveValue, &segment);
	
	ds.GetClassVector()->PushVector(rt, evaluator.EvaluateAt((int)segment, blend));
}



// static public func CurveBezier readFromFile(FileReader reader)
deClassCurveBezier3D::nfReadFromFile::nfReadFromFile(const sInitData &init) :
dsFunction(init.clsCBezier3D, "readFromFile", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCBezier3D){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassCurveBezier3D::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassCurveBezier3D &clsCurveBezier3D = *((deClassCurveBezier3D*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *clsCurveBezier3D.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	const int version = reader->ReadByte();
	decCurveBezier3D curve;
	uint32_t i, count;
	
	switch(version){
	case 0:
		curve.SetInterpolationMode((decCurveBezier3D::eInterpolationModes)reader->ReadByte());
		count = reader->ReadVarUInt();
		for(i=0; i<count; i++){
			const decVector point(reader->ReadVector());
			const decVector handle1(reader->ReadVector());
			const decVector handle2(reader->ReadVector());
			curve.AddPoint({point, handle1, handle2});
		}
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	clsCurveBezier3D.PushCurve(rt, curve);
}

// public func void writeToFile(FileWriter writer)
deClassCurveBezier3D::nfWriteToFile::nfWriteToFile(const sInitData &init) :
dsFunction(init.clsCBezier3D, "writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassCurveBezier3D::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	const deClassCurveBezier3D &clsCurveBezier3D = *((deClassCurveBezier3D*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *clsCurveBezier3D.GetDS().GetClassFileWriter();
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
		const decCurveBezier3DPoint &point = curve.GetPointAt(i);
		writer->WriteVector(point.GetPoint());
		writer->WriteVector(point.GetHandle1());
		writer->WriteVector(point.GetHandle2());
	}
}



// public func int hashCode()
deClassCurveBezier3D::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCBezier3D, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassCurveBezier3D::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.GetPointCount());
}

// public func bool equals( Object obj )
deClassCurveBezier3D::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCBezier3D, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassCurveBezier3D::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurveBezier3D &curve = *((sCBezier3DNatDat*)p_GetNativeData(myself))->curve;
	deClassCurveBezier3D *clsCBezier3D = (deClassCurveBezier3D*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(! p_IsObjOfType(obj, clsCBezier3D)){
		rt->PushBool(false);
		
	}else{
		const decCurveBezier3D &other = *((sCBezier3DNatDat*)p_GetNativeData(obj))->curve;
		rt->PushBool(curve == other);
	}
}




// Class deClassCurveBezier3D
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassCurveBezier3D::deClassCurveBezier3D(deScriptingDragonScript &ds) :
dsClass("CurveBezier3D", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCBezier3DNatDat));
}

deClassCurveBezier3D::~deClassCurveBezier3D(){
}



// Management
///////////////

void deClassCurveBezier3D::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	pClsCurveBezierInterpolation = engine->GetClass("Dragengine.Scenery.CurveBezierInterpolation");
	
	// store classes
	memset(&init, '\0', sizeof(init));
	
	init.clsCBezier3D = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsVector = pDS.GetClassVector();
	init.clsCurveBezierInterpolation = pClsCurveBezierInterpolation;
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPointCount(init));
	AddFunction(new nfGetPointAt(init));
	AddFunction(new nfGetHandle1At(init));
	AddFunction(new nfGetHandle2At(init));
	AddFunction(new nfSetPointAt(init));
	AddFunction(new nfSetPointAt2(init));
	AddFunction(new nfAddPoint(init));
	AddFunction(new nfAddPoint2(init));
	AddFunction(new nfRemovePointFrom(init));
	AddFunction(new nfRemoveAllPoints(init));
	
	AddFunction(new nfGetInterpolationMode(init));
	AddFunction(new nfSetInterpolationMode(init));
	
	AddFunction(new nfEvaluateAt(init));
	AddFunction(new nfEvaluateAt2(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

decCurveBezier3D &deClassCurveBezier3D::GetCurve(dsRealObject *myself) const{
	if(! myself){
		DSTHROW(dueNullPointer);
	}
	
	return *((sCBezier3DNatDat*)p_GetNativeData(myself->GetBuffer()))->curve;
}

decCurveBezier3DEvaluator &deClassCurveBezier3D::GetEvaluator(dsRealObject *myself) const{
	if(! myself){
		DSTHROW(dueNullPointer);
	}
	
	return *((sCBezier3DNatDat*)p_GetNativeData(myself->GetBuffer()))->evaluator;
}

void deClassCurveBezier3D::PushCurve(dsRunTime *rt, const decCurveBezier3D &curve){
	if(! rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sCBezier3DNatDat &nd = *((sCBezier3DNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.curve = nullptr;
	nd.evaluator = nullptr;
	
	try{
		nd.curve = new decCurveBezier3D(curve);
		nd.evaluator = new decCurveBezier3DEvaluator(*nd.curve);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
