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
#include <new>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassLumimeter.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../world/deClassWorld.h"
#include "../graphics/deClassColor.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/resources/sensor/deLumimeter.h"
#include "dragengine/resources/sensor/deLumimeterManager.h"
#include "dragengine/resources/skin/deSkin.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>



// native structure
struct sLumimeterNatDat{
	deLumimeter::Ref lumimeter;
};



// Constructors, destructor
/////////////////////////////

// public func new()
deClassLumimeter::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsLM,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLumimeter::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sLumimeterNatDat *nd = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself);
	deClassLumimeter *clsLumimeter = static_cast<deClassLumimeter*>(GetOwnerClass());
	deLumimeterManager *lumimeterMgr = clsLumimeter->GetGameEngine()->GetLumimeterManager();
	
	// create lumimeter
	nd->lumimeter = lumimeterMgr->CreateLumimeter();
	if(!nd->lumimeter) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassLumimeter::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsLM,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassLumimeter::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->~sLumimeterNatDat();
}



// Management
///////////////

// public func DVector getPosition()
deClassLumimeter::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsLM,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassLumimeter::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	deClassLumimeter *clsLumimeter = static_cast<deClassLumimeter*>(GetOwnerClass());
	
	clsLumimeter->GetClassDVector()->PushDVector(rt, lumimeter->GetPosition());
}

// public func void setPosition( DVector position )
deClassLumimeter::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsLM,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
}
void deClassLumimeter::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	deClassLumimeter *clsLumimeter = static_cast<deClassLumimeter*>(GetOwnerClass());
	
	dsRealObject *obj = rt->GetValue(0)->GetRealObject();
	if(!obj) DSTHROW(dueNullPointer);
	
	lumimeter->SetPosition(clsLumimeter->GetClassDVector()->GetDVector(obj));
}

// public func Vector getDirection()
deClassLumimeter::nfGetDirection::nfGetDirection(const sInitData &init) : dsFunction(init.clsLM,
"getDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassLumimeter::nfGetDirection::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	deClassLumimeter *clsLumimeter = static_cast<deClassLumimeter*>(GetOwnerClass());
	
	clsLumimeter->GetClassVector()->PushVector(rt, lumimeter->GetDirection());
}

// public func void setDirection( Vector direction )
deClassLumimeter::nfSetDirection::nfSetDirection(const sInitData &init) : dsFunction(init.clsLM,
"setDirection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // direction
}
void deClassLumimeter::nfSetDirection::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	deClassLumimeter *clsLumimeter = static_cast<deClassLumimeter*>(GetOwnerClass());
	
	dsRealObject *obj = rt->GetValue(0)->GetRealObject();
	if(!obj) DSTHROW(dueNullPointer);
	
	lumimeter->SetDirection(clsLumimeter->GetClassVector()->GetVector(obj));
}

// public func float getConeInnerAngle()
deClassLumimeter::nfGetConeInnerAngle::nfGetConeInnerAngle(const sInitData &init) : dsFunction(init.clsLM,
"getConeInnerAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassLumimeter::nfGetConeInnerAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	rt->PushFloat(lumimeter->GetConeInnerAngle() * RAD2DEG);
}

// public func void setConeInnerAngle( float angle )
deClassLumimeter::nfSetConeInnerAngle::nfSetConeInnerAngle(const sInitData &init) : dsFunction(init.clsLM,
"setConeInnerAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // angle
}
void deClassLumimeter::nfSetConeInnerAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	lumimeter->SetConeInnerAngle(rt->GetValue(0)->GetFloat() * DEG2RAD);
}

// public func float getConeOuterAngle()
deClassLumimeter::nfGetConeOuterAngle::nfGetConeOuterAngle(const sInitData &init) : dsFunction(init.clsLM,
"getConeOuterAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassLumimeter::nfGetConeOuterAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	rt->PushFloat(lumimeter->GetConeOuterAngle() * RAD2DEG);
}

// public func void setConeOuterAngle( float angle )
deClassLumimeter::nfSetConeOuterAngle::nfSetConeOuterAngle(const sInitData &init) : dsFunction(init.clsLM,
"setConeOuterAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // angle
}
void deClassLumimeter::nfSetConeOuterAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	lumimeter->SetConeOuterAngle(rt->GetValue(0)->GetFloat() * DEG2RAD);
}

// public func float getConeExponent()
deClassLumimeter::nfGetConeExponent::nfGetConeExponent(const sInitData &init) : dsFunction(init.clsLM,
"getConeExponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassLumimeter::nfGetConeExponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	rt->PushFloat(lumimeter->GetConeExponent());
}

// public func void setConeExponent( float exponent )
deClassLumimeter::nfSetConeExponent::nfSetConeExponent(const sInitData &init) : dsFunction(init.clsLM,
"setConeExponent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // exponent
}
void deClassLumimeter::nfSetConeExponent::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	lumimeter->SetConeExponent(rt->GetValue(0)->GetFloat());
}

// func World getParentWorld()
deClassLumimeter::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsLM, "getParentWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}

void deClassLumimeter::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter &lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	deClassWorld &clsWorld = *((static_cast<deClassLumimeter*>(GetOwnerClass()))->GetScriptModule()->GetClassWorld());
	clsWorld.PushWorld(rt, lumimeter.GetParentWorld());
}

// Measuring
//////////////

// public func float measureLuminance()
deClassLumimeter::nfMeasureLuminance::nfMeasureLuminance(const sInitData &init) : dsFunction(init.clsLM,
"measureLuminance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassLumimeter::nfMeasureLuminance::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	rt->PushFloat(lumimeter->MeasureLuminance());
}

// public func Color measureColor()
deClassLumimeter::nfMeasureColor::nfMeasureColor(const sInitData &init) : dsFunction(init.clsLM,
"measureColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
}
void deClassLumimeter::nfMeasureColor::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	deClassLumimeter *clsLumimeter = static_cast<deClassLumimeter*>(GetOwnerClass());
	
	clsLumimeter->GetClassColor()->PushColor(rt, lumimeter->MeasureColor());
}



// Misc
/////////

// public func int hashCode()
deClassLumimeter::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsLM, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassLumimeter::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	
	rt->PushInt((int)(intptr_t)lumimeter);
}

// public func bool equals( Object obj )
deClassLumimeter::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsLM, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassLumimeter::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deLumimeter *lumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(myself))->lumimeter;
	deClassLumimeter *clsLumimeter = static_cast<deClassLumimeter*>(GetOwnerClass());
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsLumimeter)){
		rt->PushBool(false);
		
	}else{
		deLumimeter *otherLumimeter = static_cast<sLumimeterNatDat*>(p_GetNativeData(obj))->lumimeter;
		rt->PushBool(lumimeter == otherLumimeter);
	}
}



// Class deClassLumimeter
///////////////////////////

// Constructor, destructor
////////////////////////////

deClassLumimeter::deClassLumimeter(deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass("Lumimeter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!gameEngine || !scrMgr) DSTHROW(dueInvalidParam);
	
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sLumimeterNatDat));
}

deClassLumimeter::~deClassLumimeter(){
}



// Management
///////////////

void deClassLumimeter::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	pClsVec = pScrMgr->GetClassVector();
	pClsDVec = pScrMgr->GetClassDVector();
	pClsClr = pScrMgr->GetClassColor();
	
	init.clsLM = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pClsVec;
	init.clsDVec = pClsDVec;
	init.clsClr = pClsClr;
	init.clsWorld = pScrMgr->GetClassWorld();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetDirection(init));
	AddFunction(new nfSetDirection(init));
	AddFunction(new nfGetConeInnerAngle(init));
	AddFunction(new nfSetConeInnerAngle(init));
	AddFunction(new nfGetConeOuterAngle(init));
	AddFunction(new nfSetConeOuterAngle(init));
	AddFunction(new nfGetConeExponent(init));
	AddFunction(new nfSetConeExponent(init));
	AddFunction(new nfGetParentWorld(init));
	
	AddFunction(new nfMeasureLuminance(init));
	AddFunction(new nfMeasureColor(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deLumimeter *deClassLumimeter::GetLumimeter(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sLumimeterNatDat*>(p_GetNativeData(myself->GetBuffer()))->lumimeter;
}

void deClassLumimeter::PushLumimeter(dsRunTime *rt, deLumimeter *lumimeter){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!lumimeter){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sLumimeterNatDat)->lumimeter = lumimeter;
}
