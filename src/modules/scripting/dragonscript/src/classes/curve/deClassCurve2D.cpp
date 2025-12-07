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

#include "deClassCurve2D.h"
#include "../math/deClassVector2.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <libdscript/exceptions.h>



// Native Structure
/////////////////////

struct sCurve2DNatDat{
	decCurve2D *curve;
};



// Native functions
/////////////////////

// public func new()
deClassCurve2D::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCurve2D,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurve2D::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCurve2DNatDat *nd = (sCurve2DNatDat*)p_GetNativeData(myself);
	
	// clear ( important )
	nd->curve = NULL;
	
	// create curve
	nd->curve = new decCurve2D;
	if(! nd->curve) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassCurve2D::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCurve2D,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurve2D::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCurve2DNatDat *nd = (sCurve2DNatDat*)p_GetNativeData(myself);
	
	if(nd->curve){
		delete nd->curve;
		nd->curve = NULL;
	}
}



// Management
///////////////

// public func int getPointCount()
deClassCurve2D::nfGetPointCount::nfGetPointCount(const sInitData &init) : dsFunction(init.clsCurve2D,
"getPointCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCurve2D::nfGetPointCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.GetPointCount());
}

// public func Vector2 getPointAt( int position )
deClassCurve2D::nfGetPointAt::nfGetPointAt(const sInitData &init) : dsFunction(init.clsCurve2D,
"getPointAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // position
}
void deClassCurve2D::nfGetPointAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript *ds = ((deClassCurve2D*)GetOwnerClass())->GetDS();
	
	int position = rt->GetValue(0)->GetInt();
	
	ds->GetClassVector2()->PushVector2(rt, curve.GetPointAt(position));
}

// public func int indexOfPointClosestTo( float coordinate )
deClassCurve2D::nfIndexOfPointClosestTo::nfIndexOfPointClosestTo(const sInitData &init) : dsFunction(init.clsCurve2D,
"indexOfPointClosestTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsFlt); // coordinate
}
void deClassCurve2D::nfIndexOfPointClosestTo::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.IndexOfPointClosestTo(rt->GetValue(0)->GetFloat()));
}

// public func int indexOfPointBefore( float coordinate )
deClassCurve2D::nfIndexOfPointBefore::nfIndexOfPointBefore(const sInitData &init) : dsFunction(init.clsCurve2D,
"indexOfPointBefore", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsFlt); // coordinate
}
void deClassCurve2D::nfIndexOfPointBefore::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.IndexOfPointBefore(rt->GetValue(0)->GetFloat()));
}

// public func int addPoint( Vector2 point )
deClassCurve2D::nfAddPoint::nfAddPoint(const sInitData &init) : dsFunction(init.clsCurve2D,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsVec2); // point
}
void deClassCurve2D::nfAddPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	deScriptingDragonScript *ds = ((deClassCurve2D*)GetOwnerClass())->GetDS();
	
	dsRealObject *objPoint = rt->GetValue(0)->GetRealObject();
	if(! objPoint) DSTHROW(dueNullPointer);
	
	rt->PushInt(curve.AddPoint(ds->GetClassVector2()->GetVector2(objPoint)));
}

// public func int addPoint( float x, float y )
deClassCurve2D::nfAddPoint2::nfAddPoint2(const sInitData &init) : dsFunction(init.clsCurve2D,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsFlt); // x
	p_AddParameter(init.clsFlt); // y
}
void deClassCurve2D::nfAddPoint2::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	float x = rt->GetValue(0)->GetFloat();
	float y = rt->GetValue(1)->GetFloat();
	
	rt->PushInt(curve.AddPoint(decVector2(x, y)));
}

// public func void removePointFrom( int position )
deClassCurve2D::nfRemovePointFrom::nfRemovePointFrom(const sInitData &init) : dsFunction(init.clsCurve2D,
"removePointFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // position
}
void deClassCurve2D::nfRemovePointFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.RemovePointFrom(rt->GetValue(0)->GetInt());
}

// public func void removeAllPoints()
deClassCurve2D::nfRemoveAllPoints::nfRemoveAllPoints(const sInitData &init) : dsFunction(init.clsCurve2D,
"removeAllPoints", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurve2D::nfRemoveAllPoints::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.RemoveAllPoints();
}



// public func void setDefaultCurve()
deClassCurve2D::nfSetDefaultCurve::nfSetDefaultCurve(const sInitData &init) : dsFunction(init.clsCurve2D,
"setDefaultCurve", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCurve2D::nfSetDefaultCurve::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.SetDefaultCurve();
}

// public func void offsetPointsBy( float offset )
deClassCurve2D::nfOffsetPointsBy::nfOffsetPointsBy(const sInitData &init) : dsFunction(init.clsCurve2D,
"offsetPointsBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // offset
}
void deClassCurve2D::nfOffsetPointsBy::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.OffsetPointsBy(rt->GetValue(0)->GetFloat());
}

// public func void scalePointsBy( float scale )
deClassCurve2D::nfScalePointsBy::nfScalePointsBy(const sInitData &init) : dsFunction(init.clsCurve2D,
"scalePointsBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // scale
}
void deClassCurve2D::nfScalePointsBy::RunFunction(dsRunTime *rt, dsValue *myself){
	decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	curve.ScalePointsBy(rt->GetValue(0)->GetFloat());
}



// public func float evaluateConstant( float coordinate )
deClassCurve2D::nfEvaluateConstant::nfEvaluateConstant(const sInitData &init) : dsFunction(init.clsCurve2D,
"evaluateConstant", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsFlt); // coordinate
}
void deClassCurve2D::nfEvaluateConstant::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushFloat(curve.EvaluateConstant(rt->GetValue(0)->GetFloat()));
}

// public func float evaluateLinear( float coordinate )
deClassCurve2D::nfEvaluateLinear::nfEvaluateLinear(const sInitData &init) : dsFunction(init.clsCurve2D,
"evaluateLinear", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsFlt); // coordinate
}
void deClassCurve2D::nfEvaluateLinear::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushFloat(curve.EvaluateLinear(rt->GetValue(0)->GetFloat()));
}



// public func int hashCode()
deClassCurve2D::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCurve2D, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCurve2D::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	
	rt->PushInt(curve.GetPointCount());
}

// public func bool equals( Object obj )
deClassCurve2D::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCurve2D, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCurve2D::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decCurve2D &curve = *((sCurve2DNatDat*)p_GetNativeData(myself))->curve;
	deClassCurve2D *clsCurve2D = (deClassCurve2D*)GetOwnerClass();
	dsValue *obj = rt->GetValue(0);
	
	if(! p_IsObjOfType(obj, clsCurve2D)){
		rt->PushBool(false);
		
	}else{
		const decCurve2D &other = *((sCurve2DNatDat*)p_GetNativeData(obj))->curve;
		
		if(curve.GetPointCount() == other.GetPointCount()){
			int p, count = curve.GetPointCount();
			bool equal = true;
			
			for(p=0; p<count; p++){
				if(! curve.GetPointAt(p).IsEqualTo(other.GetPointAt(p))){
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




// Class deClassCurve2D
///////////////////////////

// Constructor, destructor
////////////////////////////

deClassCurve2D::deClassCurve2D(deScriptingDragonScript *ds) :
dsClass("Curve2D", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(! ds) DSTHROW(dueInvalidParam);
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCurve2DNatDat));
}

deClassCurve2D::~deClassCurve2D(){
}



// Management
///////////////

void deClassCurve2D::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	memset(&init, '\0', sizeof(init));
	
	init.clsCurve2D = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec2 = pDS->GetClassVector2();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPointCount(init));
	AddFunction(new nfGetPointAt(init));
	AddFunction(new nfIndexOfPointClosestTo(init));
	AddFunction(new nfIndexOfPointBefore(init));
	AddFunction(new nfAddPoint(init));
	AddFunction(new nfAddPoint2(init));
	AddFunction(new nfRemovePointFrom(init));
	AddFunction(new nfRemoveAllPoints(init));
	
	AddFunction(new nfSetDefaultCurve(init));
	AddFunction(new nfOffsetPointsBy(init));
	AddFunction(new nfScalePointsBy(init));
	
	AddFunction(new nfEvaluateConstant(init));
	AddFunction(new nfEvaluateLinear(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

const decCurve2D &deClassCurve2D::GetCurve(dsRealObject *myself) const{
	if(! myself){
		DSTHROW(dueNullPointer);
	}
	
	return *((sCurve2DNatDat*)p_GetNativeData(myself->GetBuffer()))->curve;
}

void deClassCurve2D::PushCurve(dsRunTime *rt, const decCurve2D &curve){
	if(! rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sCurve2DNatDat &nd = *((sCurve2DNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.curve = NULL;
	
	try{
		nd.curve = new decCurve2D(curve);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
