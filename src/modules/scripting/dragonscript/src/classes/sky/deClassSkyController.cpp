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

#include "deClassSky.h"
#include "deClassSkyController.h"
#include "deClassSkyInstance.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyController.h>



// Native Structure
/////////////////////

struct sSkyCtrlNatDat{
	deSky *sky;
	deSkyInstance *instance;
	int index;
};



// Native functions
/////////////////////

// private func new()
deClassSkyController::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSkyCtrl, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyController::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	DSTHROW(dueInvalidParam);
}

// public func destructor()
deClassSkyController::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkyCtrl, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyController::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSkyCtrlNatDat &nd = *((sSkyCtrlNatDat*)p_GetNativeData(myself));
	if(nd.sky){
		nd.sky->FreeReference();
		nd.sky = NULL;
	}
	if(nd.instance){
		nd.instance->FreeReference();
		nd.instance = NULL;
	}
	nd.index = -1;
}



// Management
///////////////

// public func Sky getSky()
deClassSkyController::nfGetSky::nfGetSky(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky){
}
void deClassSkyController::nfGetSky::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deScriptingDragonScript &ds = ((deClassSkyController*)GetOwnerClass())->GetDS();
	
	ds.GetClassSky()->PushSky(rt, nd.sky);
}

// public func SkyInstance getSkyInstance()
deClassSkyController::nfGetSkyInstance::nfGetSkyInstance(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getSkyInstance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyInst){
}
void deClassSkyController::nfGetSkyInstance::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deScriptingDragonScript &ds = ((deClassSkyController*)GetOwnerClass())->GetDS();
	
	ds.GetClassSkyInstance()->PushInstance(rt, nd.instance);
}

// public func int getControllerIndex()
deClassSkyController::nfGetControllerIndex::nfGetControllerIndex(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getControllerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyController::nfGetControllerIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.index);
}



// public func String getName()
deClassSkyController::nfGetName::nfGetName(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassSkyController::nfGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushString(controller.GetName());
}

// public func void setName( String name )
deClassSkyController::nfSetName::nfSetName(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassSkyController::nfSetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetName(rt->GetValue(0)->GetString());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getLower()
deClassSkyController::nfGetLower::nfGetLower(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyController::nfGetLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetMinimumValue());
}

// public func float getUpper()
deClassSkyController::nfGetUpper::nfGetUpper(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyController::nfGetUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetMaximumValue());
}

// public func void setRange( float lower, float upper )
deClassSkyController::nfSetRange::nfSetRange(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // lower
	p_AddParameter(init.clsFloat); // upper
}
void deClassSkyController::nfSetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float lower = rt->GetValue(0)->GetFloat();
	const float upper = rt->GetValue(1)->GetFloat();
	
	controller.SetValueRange(lower, upper);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getValue()
deClassSkyController::nfGetValue::nfGetValue(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyController::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetCurrentValue());
}

// public func void setValue( float value )
deClassSkyController::nfSetValue::nfSetValue(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // value
}
void deClassSkyController::nfSetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float value = rt->GetValue(0)->GetFloat();
	
	controller.SetCurrentValue(value);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool getFrozen()
deClassSkyController::nfGetFrozen::nfGetFrozen(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getFrozen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSkyController::nfGetFrozen::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetFrozen());
}

// public func void setFrozen( bool frozen )
deClassSkyController::nfSetFrozen::nfSetFrozen(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setFrozen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // frozen
}
void deClassSkyController::nfSetFrozen::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const bool frozen = rt->GetValue(0)->GetBool();
	
	controller.SetFrozen(frozen);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool getClamp()
deClassSkyController::nfGetClamp::nfGetClamp(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "getClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSkyController::nfGetClamp::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetClamp());
}

// public func void setClamp( bool clamp )
deClassSkyController::nfSetClamp::nfSetClamp(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // clamp
}
void deClassSkyController::nfSetClamp::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const bool clamp = rt->GetValue(0)->GetBool();
	
	controller.SetClamp(clamp);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}



// public func void increment( float amount )
deClassSkyController::nfIncrement::nfIncrement(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "increment", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // amount
}
void deClassSkyController::nfIncrement::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float amount = rt->GetValue(0)->GetFloat();
	
	controller.SetCurrentValue(controller.GetCurrentValue() + amount);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setToLower()
deClassSkyController::nfSetToLower::nfSetToLower(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setToLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyController::nfSetToLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetCurrentValue(controller.GetMaximumValue());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setToUpper()
deClassSkyController::nfSetToUpper::nfSetToUpper(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "setToUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyController::nfSetToUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetCurrentValue(controller.GetMinimumValue());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool isAtLower()
deClassSkyController::nfIsAtLower::nfIsAtLower(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "isAtLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSkyController::nfIsAtLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetCurrentValue()
		<= controller.GetMinimumValue() + FLOAT_SAFE_EPSILON);
}

// public func bool isAtUpper( int controller )
deClassSkyController::nfIsAtUpper::nfIsAtUpper(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "isAtUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSkyController::nfIsAtUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	const deSkyController &controller = nd.sky
		? nd.sky->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetCurrentValue()
		>= controller.GetMaximumValue() - FLOAT_SAFE_EPSILON);
}



// public func bool equals( Object obj )
deClassSkyController::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSkyCtrl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassSkyController::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkyController * const clsSkyCtrl = (deClassSkyController*)GetOwnerClass();
	const sSkyCtrlNatDat &nd = *((const sSkyCtrlNatDat*)p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSkyCtrl)){
		rt->PushBool(false);
		
	}else{
		const sSkyCtrlNatDat &other = *((const sSkyCtrlNatDat*)p_GetNativeData(obj));
		rt->PushBool(nd.sky == other.sky && nd.instance == other.instance
			&& nd.index == other.index);
	}
}



// Class deClassSkyController
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyController::deClassSkyController(deScriptingDragonScript &ds) :
dsClass("SkyController", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSkyCtrlNatDat));
}

deClassSkyController::~deClassSkyController(){
}



// Management
///////////////

void deClassSkyController::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsSkyCtrl = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsSky = pDS.GetClassSky();
	init.clsSkyInst = pDS.GetClassSkyInstance();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetSky(init));
	AddFunction(new nfGetSkyInstance(init));
	AddFunction(new nfGetControllerIndex(init));
	
	AddFunction(new nfGetName(init));
	AddFunction(new nfSetName(init));
	AddFunction(new nfGetLower(init));
	AddFunction(new nfGetUpper(init));
	AddFunction(new nfSetRange(init));
	AddFunction(new nfGetValue(init));
	AddFunction(new nfSetValue(init));
	AddFunction(new nfGetFrozen(init));
	AddFunction(new nfSetFrozen(init));
	AddFunction(new nfGetClamp(init));
	AddFunction(new nfSetClamp(init));
	
	AddFunction(new nfIncrement(init));
	AddFunction(new nfSetToLower(init));
	AddFunction(new nfSetToUpper(init));
	AddFunction(new nfIsAtLower(init));
	AddFunction(new nfIsAtUpper(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassSkyController::PushController(dsRunTime *rt, deSky *sky, int index){
	if(!rt || !sky || index < 0 || index >= sky->GetControllerCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSkyCtrlNatDat &nd = *((sSkyCtrlNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.sky = sky;
	sky->AddReference();
	nd.instance = NULL;
	nd.index = index;
}

void deClassSkyController::PushController(dsRunTime *rt, deSkyInstance *instance, int index){
	if(!rt || !instance || index < 0 || index >= instance->GetControllerCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSkyCtrlNatDat &nd = *((sSkyCtrlNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.sky = NULL;
	nd.instance = instance;
	instance->AddReference();
	nd.index = index;
}
