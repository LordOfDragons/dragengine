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

#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassParticleEmitter.h"
#include "deClassParticleEmitterController.h"
#include "deClassParticleEmitterInstance.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>



// Native Structure
/////////////////////

struct sParticleEmitterCtrlNatDat{
	deParticleEmitter::Ref emitter;
	deParticleEmitterInstance::Ref instance;
};



// Native functions
/////////////////////

// private func new()
deClassParticleEmitterController::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassParticleEmitterController::nfNew::RunFunction(dsRunTime*, dsValue*){
	DSTHROW(dueInvalidParam);
}

// public func destructor()
deClassParticleEmitterController::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassParticleEmitterController::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself))->~sParticleEmitterCtrlNatDat();
}



// Management
///////////////

// public func ParticleEmitter getParticleEmitter()
deClassParticleEmitterController::nfGetParticleEmitter::nfGetParticleEmitter(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getParticleEmitter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsParticleEmitter){
}
void deClassParticleEmitterController::nfGetParticleEmitter::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deScriptingDragonScript &ds = (static_cast<deClassParticleEmitterController*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassParticleEmitter()->PushParticleEmitter(rt, nd.emitter);
}

// public func ParticleEmitterInstance getParticleEmitterInstance()
deClassParticleEmitterController::nfGetParticleEmitterInstance::nfGetParticleEmitterInstance(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getParticleEmitterInstance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsParticleEmitterInst){
}
void deClassParticleEmitterController::nfGetParticleEmitterInstance::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deScriptingDragonScript &ds = (static_cast<deClassParticleEmitterController*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassParticleEmitterInstance()->PushInstance(rt, nd.instance);
}

// public func int getControllerIndex()
deClassParticleEmitterController::nfGetControllerIndex::nfGetControllerIndex(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getControllerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassParticleEmitterController::nfGetControllerIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.index);
}



// public func String getName()
deClassParticleEmitterController::nfGetName::nfGetName(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassParticleEmitterController::nfGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushString(controller.GetName());
}

// public func void setName( String name )
deClassParticleEmitterController::nfSetName::nfSetName(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassParticleEmitterController::nfSetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetName(rt->GetValue(0)->GetString());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getLower()
deClassParticleEmitterController::nfGetLower::nfGetLower(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassParticleEmitterController::nfGetLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetLower());
}

// public func float getUpper()
deClassParticleEmitterController::nfGetUpper::nfGetUpper(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassParticleEmitterController::nfGetUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetUpper());
}

// public func void setRange( float lower, float upper )
deClassParticleEmitterController::nfSetRange::nfSetRange(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // lower
	p_AddParameter(init.clsFloat); // upper
}
void deClassParticleEmitterController::nfSetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float lower = rt->GetValue(0)->GetFloat();
	const float upper = rt->GetValue(1)->GetFloat();
	
	controller.SetRange(lower, upper);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getValue()
deClassParticleEmitterController::nfGetValue::nfGetValue(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassParticleEmitterController::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetValue());
}

// public func void setValue( float value )
deClassParticleEmitterController::nfSetValue::nfSetValue(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // value
}
void deClassParticleEmitterController::nfSetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float value = rt->GetValue(0)->GetFloat();
	
	controller.SetValue(value);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getRelativeValue()
deClassParticleEmitterController::nfGetRelativeValue::nfGetRelativeValue(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getRelativeValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassParticleEmitterController::nfGetRelativeValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(decMath::linearStep(controller.GetValue(), controller.GetLower(), controller.GetUpper()));
}

// public func void setRelativeValue( float value )
deClassParticleEmitterController::nfSetRelativeValue::nfSetRelativeValue(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setRelativeValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // value
}
void deClassParticleEmitterController::nfSetRelativeValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float value = rt->GetValue(0)->GetFloat();
	
	controller.SetValue(decMath::linearStep(value, 0.0f, 1.0f, controller.GetLower(), controller.GetUpper()));
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool getFrozen()
deClassParticleEmitterController::nfGetFrozen::nfGetFrozen(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getFrozen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassParticleEmitterController::nfGetFrozen::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetFrozen());
}

// public func void setFrozen( bool frozen )
deClassParticleEmitterController::nfSetFrozen::nfSetFrozen(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setFrozen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // frozen
}
void deClassParticleEmitterController::nfSetFrozen::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const bool frozen = rt->GetValue(0)->GetBool();
	
	controller.SetFrozen(frozen);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool getClamp()
deClassParticleEmitterController::nfGetClamp::nfGetClamp(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "getClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassParticleEmitterController::nfGetClamp::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetClamp());
}

// public func void setClamp( bool clamp )
deClassParticleEmitterController::nfSetClamp::nfSetClamp(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // clamp
}
void deClassParticleEmitterController::nfSetClamp::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const bool clamp = rt->GetValue(0)->GetBool();
	
	controller.SetClamp(clamp);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}



// public func void increment( float amount )
deClassParticleEmitterController::nfIncrement::nfIncrement(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "increment", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // amount
}
void deClassParticleEmitterController::nfIncrement::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float amount = rt->GetValue(0)->GetFloat();
	
	controller.SetValue(controller.GetValue() + amount);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setToLower()
deClassParticleEmitterController::nfSetToLower::nfSetToLower(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setToLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassParticleEmitterController::nfSetToLower::RunFunction(dsRunTime*, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetValue(controller.GetLower());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setToUpper()
deClassParticleEmitterController::nfSetToUpper::nfSetToUpper(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setToUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassParticleEmitterController::nfSetToUpper::RunFunction(dsRunTime*, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetValue(controller.GetUpper());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void reverse()
deClassParticleEmitterController::nfReverse::nfReverse(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "reverse", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassParticleEmitterController::nfReverse::RunFunction(dsRunTime*, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetValue(controller.GetUpper() - controller.GetValue());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setValueFrom( ParticleEmitterController controller )
deClassParticleEmitterController::nfSetValueFrom::nfSetValueFrom(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setValueFrom", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsParticleEmitterCtrl); // controller
}
void deClassParticleEmitterController::nfSetValueFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	dsValue * const valFrom = rt->GetValue(0);
	if(!valFrom->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const sParticleEmitterCtrlNatDat &ndFrom = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(valFrom));
	deParticleEmitterController &controllerFrom = ndFrom.emitter
		? *nd.emitter->GetControllerAt(ndFrom.index)
		: nd.instance->GetControllerAt(ndFrom.index);
	
	controller.SetValue(controllerFrom.GetValue());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setAllFrom( ParticleEmitterController controller )
deClassParticleEmitterController::nfSetAllFrom::nfSetAllFrom(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "setAllFrom", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsParticleEmitterCtrl); // controller
}
void deClassParticleEmitterController::nfSetAllFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	dsValue * const valFrom = rt->GetValue(0);
	if(!valFrom->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const sParticleEmitterCtrlNatDat &ndFrom = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(valFrom));
	deParticleEmitterController &controllerFrom = ndFrom.emitter
		? *nd.emitter->GetControllerAt(ndFrom.index)
		: nd.instance->GetControllerAt(ndFrom.index);
	
	controller.SetFrozen(false);
	controller.SetRange(controllerFrom.GetLower(), controllerFrom.GetUpper());
	controller.SetValue(controllerFrom.GetValue());
	controller.SetClamp(controllerFrom.GetClamp());
	controller.SetFrozen(controllerFrom.GetFrozen());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool isAtLower()
deClassParticleEmitterController::nfIsAtLower::nfIsAtLower(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "isAtLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassParticleEmitterController::nfIsAtLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetValue() <= controller.GetLower() + FLOAT_SAFE_EPSILON);
}

// public func bool isAtUpper( int controller )
deClassParticleEmitterController::nfIsAtUpper::nfIsAtUpper(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "isAtUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassParticleEmitterController::nfIsAtUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	const deParticleEmitterController &controller = nd.emitter
		? *nd.emitter->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetValue() >= controller.GetUpper() - FLOAT_SAFE_EPSILON);
}



// public func bool equals( Object obj )
deClassParticleEmitterController::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsParticleEmitterCtrl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassParticleEmitterController::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassParticleEmitterController * const clsParticleEmitterCtrl = static_cast<deClassParticleEmitterController*>(GetOwnerClass());
	const sParticleEmitterCtrlNatDat &nd = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsParticleEmitterCtrl)){
		rt->PushBool(false);
		
	}else{
		const sParticleEmitterCtrlNatDat &other = *static_cast<const sParticleEmitterCtrlNatDat*>(p_GetNativeData(obj));
		rt->PushBool(nd.emitter == other.emitter && nd.instance == other.instance
			&& nd.index == other.index);
	}
}



// Class deClassParticleEmitterController
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassParticleEmitterController::deClassParticleEmitterController(deScriptingDragonScript &ds) :
dsClass("ParticleEmitterController", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sParticleEmitterCtrlNatDat));
}

deClassParticleEmitterController::~deClassParticleEmitterController(){
}



// Management
///////////////

void deClassParticleEmitterController::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsParticleEmitterCtrl = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsParticleEmitter = pDS.GetClassParticleEmitter();
	init.clsParticleEmitterInst = pDS.GetClassParticleEmitterInstance();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetParticleEmitter(init));
	AddFunction(new nfGetParticleEmitterInstance(init));
	AddFunction(new nfGetControllerIndex(init));
	
	AddFunction(new nfGetName(init));
	AddFunction(new nfSetName(init));
	AddFunction(new nfGetLower(init));
	AddFunction(new nfGetUpper(init));
	AddFunction(new nfSetRange(init));
	AddFunction(new nfGetValue(init));
	AddFunction(new nfSetValue(init));
	AddFunction(new nfGetRelativeValue(init));
	AddFunction(new nfSetRelativeValue(init));
	AddFunction(new nfGetFrozen(init));
	AddFunction(new nfSetFrozen(init));
	AddFunction(new nfGetClamp(init));
	AddFunction(new nfSetClamp(init));
	
	AddFunction(new nfIncrement(init));
	AddFunction(new nfSetToLower(init));
	AddFunction(new nfSetToUpper(init));
	AddFunction(new nfReverse(init));
	AddFunction(new nfSetValueFrom(init));
	AddFunction(new nfSetAllFrom(init));
	AddFunction(new nfIsAtLower(init));
	AddFunction(new nfIsAtUpper(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassParticleEmitterController::GetController(dsRealObject *myself, deParticleEmitter *&emitter,
deParticleEmitterInstance *&instance, int &index) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	const sParticleEmitterCtrlNatDat &nd = *static_cast<sParticleEmitterCtrlNatDat*>(p_GetNativeData(myself->GetBuffer()));
	emitter = nd.emitter;
	instance = nd.instance;
	index = nd.index;
}

void deClassParticleEmitterController::PushController(dsRunTime *rt, deParticleEmitter *emitter, int index){
	if(!rt || !emitter || index < 0 || index >= emitter->GetControllerCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sParticleEmitterCtrlNatDat &nd = *static_cast<sParticleEmitterCtrlNatDat*>(p_GetNativeData(
		rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.emitter = emitter;
	emitter->AddReference();
	nd.instance = nullptr;
	nd.index = index;
}

void deClassParticleEmitterController::PushController(dsRunTime *rt, deParticleEmitterInstance *instance, int index){
	if(!rt || !instance || index < 0 || index >= instance->GetControllerCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sParticleEmitterCtrlNatDat &nd = *static_cast<sParticleEmitterCtrlNatDat*>(p_GetNativeData(
		rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.emitter = nullptr;
	nd.instance = instance;
	instance->AddReference();
	nd.index = index;
}
