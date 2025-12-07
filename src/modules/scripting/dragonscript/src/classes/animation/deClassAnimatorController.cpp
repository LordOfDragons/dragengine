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

#include "deClassAnimator.h"
#include "deClassAnimatorController.h"
#include "deClassAnimatorInstance.h"
#include "../math/deClassVector.h"
#include "../math/deClassQuaternion.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>



// Native Structure
/////////////////////

struct sAnimatorCtrlNatDat{
	deAnimator *animator;
	deAnimatorInstance *instance;
	int index;
};



// Native functions
/////////////////////

// private func new()
deClassAnimatorController::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorController::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	DSTHROW(dueInvalidParam);
}

// public func destructor()
deClassAnimatorController::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorController::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sAnimatorCtrlNatDat &nd = *((sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	if(nd.animator){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	if(nd.instance){
		nd.instance->FreeReference();
		nd.instance = NULL;
	}
	nd.index = -1;
}



// Management
///////////////

// public func Animator getAnimator()
deClassAnimatorController::nfGetAnimator::nfGetAnimator(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getAnimator", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimator){
}
void deClassAnimatorController::nfGetAnimator::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deScriptingDragonScript &ds = ((deClassAnimatorController*)GetOwnerClass())->GetDS();
	
	ds.GetClassAnimator()->PushAnimator(rt, nd.animator);
}

// public func AnimatorInstance getAnimatorInstance()
deClassAnimatorController::nfGetAnimatorInstance::nfGetAnimatorInstance(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getAnimatorInstance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimatorInst){
}
void deClassAnimatorController::nfGetAnimatorInstance::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deScriptingDragonScript &ds = ((deClassAnimatorController*)GetOwnerClass())->GetDS();
	
	ds.GetClassAnimatorInstance()->PushAnimatorInstance(rt, nd.instance);
}

// public func int getControllerIndex()
deClassAnimatorController::nfGetControllerIndex::nfGetControllerIndex(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getControllerIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassAnimatorController::nfGetControllerIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	
	rt->PushInt(nd.index);
}



// public func String getName()
deClassAnimatorController::nfGetName::nfGetName(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassAnimatorController::nfGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushString(controller.GetName());
}

// public func void setName( String name )
deClassAnimatorController::nfSetName::nfSetName(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassAnimatorController::nfSetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetName(rt->GetValue(0)->GetString());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getLower()
deClassAnimatorController::nfGetLower::nfGetLower(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAnimatorController::nfGetLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetMinimumValue());
}

// public func float getUpper()
deClassAnimatorController::nfGetUpper::nfGetUpper(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAnimatorController::nfGetUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetMaximumValue());
}

// public func void setRange( float lower, float upper )
deClassAnimatorController::nfSetRange::nfSetRange(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // lower
	p_AddParameter(init.clsFloat); // upper
}
void deClassAnimatorController::nfSetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float lower = rt->GetValue(0)->GetFloat();
	const float upper = rt->GetValue(1)->GetFloat();
	
	controller.SetValueRange(lower, upper);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getValue()
deClassAnimatorController::nfGetValue::nfGetValue(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAnimatorController::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(controller.GetCurrentValue());
}

// public func void setValue( float value )
deClassAnimatorController::nfSetValue::nfSetValue(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // value
}
void deClassAnimatorController::nfSetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float value = rt->GetValue(0)->GetFloat();
	
	controller.SetCurrentValue(value);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func float getRelativeValue()
deClassAnimatorController::nfGetRelativeValue::nfGetRelativeValue(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getRelativeValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAnimatorController::nfGetRelativeValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushFloat(decMath::linearStep(controller.GetCurrentValue(),
		controller.GetMinimumValue(), controller.GetMaximumValue()));
}

// public func void setRelativeValue( float value )
deClassAnimatorController::nfSetRelativeValue::nfSetRelativeValue(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setRelativeValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // value
}
void deClassAnimatorController::nfSetRelativeValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float value = rt->GetValue(0)->GetFloat();
	
	controller.SetCurrentValue(decMath::linearStep(value, 0.0f, 1.0f,
		controller.GetMinimumValue(), controller.GetMaximumValue()));
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func Vector getVector()
deClassAnimatorController::nfGetVector::nfGetVector(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getVector", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
}
void deClassAnimatorController::nfGetVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deClassAnimatorController &clsAC = *((deClassAnimatorController*)GetOwnerClass());
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	clsAC.GetDS().GetClassVector()->PushVector(rt, controller.GetVector());
}

// public func void setVector( Vector vector )
deClassAnimatorController::nfSetVector::nfSetVector(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setVector", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // vector
}
void deClassAnimatorController::nfSetVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deClassAnimatorController &clsAC = *((deClassAnimatorController*)GetOwnerClass());
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const decVector &vector = clsAC.GetDS().GetClassVector()->
		GetVector(rt->GetValue(0)->GetRealObject());
	
	controller.SetVector(vector);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func Vector getRotation()
deClassAnimatorController::nfGetRotation::nfGetRotation(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getRotation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector){
}
void deClassAnimatorController::nfGetRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deClassAnimatorController &clsAC = *((deClassAnimatorController*)GetOwnerClass());
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	clsAC.GetDS().GetClassVector()->PushVector(rt, controller.GetVector() / DEG2RAD);
}

// public func void setRotation( Vector rotation )
deClassAnimatorController::nfSetRotation::nfSetRotation(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setRotation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVector); // rotation
}
void deClassAnimatorController::nfSetRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deClassAnimatorController &clsAC = *((deClassAnimatorController*)GetOwnerClass());
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const decVector &rotation = clsAC.GetDS().GetClassVector()->
		GetVector(rt->GetValue(0)->GetRealObject());
	
	controller.SetVector(rotation * DEG2RAD);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func Quaternion getOrientation()
deClassAnimatorController::nfGetOrientation::nfGetOrientation(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion){
}
void deClassAnimatorController::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deClassAnimatorController &clsAC = *((deClassAnimatorController*)GetOwnerClass());
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	clsAC.GetDS().GetClassQuaternion()->PushQuaternion(rt,
		decQuaternion::CreateFromEuler(controller.GetVector()));
}

// public func void setOrientation(Quaternion orientation)
deClassAnimatorController::nfSetOrientation::nfSetOrientation(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuaternion); // orientation
}
void deClassAnimatorController::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deClassAnimatorController &clsAC = *((deClassAnimatorController*)GetOwnerClass());
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const decQuaternion orientation(clsAC.GetDS().GetClassQuaternion()->
		GetQuaternion(rt->GetValue(0)->GetRealObject()));
	
	controller.SetVector(orientation.GetEulerAngles());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool getFrozen()
deClassAnimatorController::nfGetFrozen::nfGetFrozen(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getFrozen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAnimatorController::nfGetFrozen::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetFrozen());
}

// public func void setFrozen( bool frozen )
deClassAnimatorController::nfSetFrozen::nfSetFrozen(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setFrozen", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // frozen
}
void deClassAnimatorController::nfSetFrozen::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const bool frozen = rt->GetValue(0)->GetBool();
	
	controller.SetFrozen(frozen);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool getClamp()
deClassAnimatorController::nfGetClamp::nfGetClamp(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "getClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAnimatorController::nfGetClamp::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetClamp());
}

// public func void setClamp( bool clamp )
deClassAnimatorController::nfSetClamp::nfSetClamp(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setClamp", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // clamp
}
void deClassAnimatorController::nfSetClamp::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const bool clamp = rt->GetValue(0)->GetBool();
	
	controller.SetClamp(clamp);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}



// public func void increment( float amount )
deClassAnimatorController::nfIncrement::nfIncrement(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "increment", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // amount
}
void deClassAnimatorController::nfIncrement::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	const float amount = rt->GetValue(0)->GetFloat();
	
	controller.SetCurrentValue(controller.GetCurrentValue() + amount);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setToLower()
deClassAnimatorController::nfSetToLower::nfSetToLower(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setToLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorController::nfSetToLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetCurrentValue(controller.GetMinimumValue());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setToCenter()
deClassAnimatorController::nfSetToCenter::nfSetToCenter(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setToCenter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorController::nfSetToCenter::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetCurrentValue((controller.GetMinimumValue() + controller.GetMaximumValue()) * 0.5f);
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setToUpper()
deClassAnimatorController::nfSetToUpper::nfSetToUpper(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setToUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorController::nfSetToUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetCurrentValue(controller.GetMaximumValue());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void reverse()
deClassAnimatorController::nfReverse::nfReverse(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "reverse", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimatorController::nfReverse::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	controller.SetCurrentValue(controller.GetMaximumValue() - controller.GetCurrentValue());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setValueFrom( AnimatorController controller )
deClassAnimatorController::nfSetValueFrom::nfSetValueFrom(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setValueFrom", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimatorCtrl); // controller
}
void deClassAnimatorController::nfSetValueFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	dsValue * const valFrom = rt->GetValue(0);
	if(!valFrom->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const sAnimatorCtrlNatDat &ndFrom = *((const sAnimatorCtrlNatDat*)p_GetNativeData(valFrom));
	deAnimatorController &controllerFrom = ndFrom.animator
		? *nd.animator->GetControllerAt(ndFrom.index)
		: nd.instance->GetControllerAt(ndFrom.index);
	
	controller.SetCurrentValue(controllerFrom.GetCurrentValue());
	controller.SetVector(controllerFrom.GetVector());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func void setAllFrom( AnimatorController controller )
deClassAnimatorController::nfSetAllFrom::nfSetAllFrom(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "setAllFrom", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsAnimatorCtrl); // controller
}
void deClassAnimatorController::nfSetAllFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	dsValue * const valFrom = rt->GetValue(0);
	if(!valFrom->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const sAnimatorCtrlNatDat &ndFrom = *((const sAnimatorCtrlNatDat*)p_GetNativeData(valFrom));
	deAnimatorController &controllerFrom = ndFrom.animator
		? *nd.animator->GetControllerAt(ndFrom.index)
		: nd.instance->GetControllerAt(ndFrom.index);
	
	controller.SetFrozen(false);
	controller.SetValueRange(controllerFrom.GetMinimumValue(), controllerFrom.GetMaximumValue());
	controller.SetCurrentValue(controllerFrom.GetCurrentValue());
	controller.SetVector(controllerFrom.GetVector());
	controller.SetClamp(controllerFrom.GetClamp());
	controller.SetFrozen(controllerFrom.GetFrozen());
	
	if(nd.instance){
		nd.instance->NotifyControllerChangedAt(nd.index);
	}
}

// public func bool isAtLower()
deClassAnimatorController::nfIsAtLower::nfIsAtLower(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "isAtLower", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAnimatorController::nfIsAtLower::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetCurrentValue() <= controller.GetMinimumValue() + FLOAT_SAFE_EPSILON);
}

// public func bool isAtUpper( int controller )
deClassAnimatorController::nfIsAtUpper::nfIsAtUpper(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "isAtUpper", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAnimatorController::nfIsAtUpper::RunFunction(dsRunTime *rt, dsValue *myself){
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	const deAnimatorController &controller = nd.animator
		? *nd.animator->GetControllerAt(nd.index)
		: nd.instance->GetControllerAt(nd.index);
	
	rt->PushBool(controller.GetCurrentValue() >= controller.GetMaximumValue() - FLOAT_SAFE_EPSILON);
}



// public func bool equals( Object obj )
deClassAnimatorController::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsAnimatorCtrl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassAnimatorController::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimatorController * const clsAnimatorCtrl = (deClassAnimatorController*)GetOwnerClass();
	const sAnimatorCtrlNatDat &nd = *((const sAnimatorCtrlNatDat*)p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsAnimatorCtrl)){
		rt->PushBool(false);
		
	}else{
		const sAnimatorCtrlNatDat &other = *((const sAnimatorCtrlNatDat*)p_GetNativeData(obj));
		rt->PushBool(nd.animator == other.animator && nd.instance == other.instance
			&& nd.index == other.index);
	}
}



// Class deClassAnimatorController
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassAnimatorController::deClassAnimatorController(deScriptingDragonScript &ds) :
dsClass("AnimatorController", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sAnimatorCtrlNatDat));
}

deClassAnimatorController::~deClassAnimatorController(){
}



// Management
///////////////

void deClassAnimatorController::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsAnimatorCtrl = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsVector = pDS.GetClassVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsAnimator = pDS.GetClassAnimator();
	init.clsAnimatorInst = pDS.GetClassAnimatorInstance();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetAnimator(init));
	AddFunction(new nfGetAnimatorInstance(init));
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
	AddFunction(new nfGetVector(init));
	AddFunction(new nfSetVector(init));
	AddFunction(new nfGetRotation(init));
	AddFunction(new nfSetRotation(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetFrozen(init));
	AddFunction(new nfSetFrozen(init));
	AddFunction(new nfGetClamp(init));
	AddFunction(new nfSetClamp(init));
	
	AddFunction(new nfIncrement(init));
	AddFunction(new nfSetToLower(init));
	AddFunction(new nfSetToCenter(init));
	AddFunction(new nfSetToUpper(init));
	AddFunction(new nfReverse(init));
	AddFunction(new nfSetValueFrom(init));
	AddFunction(new nfSetAllFrom(init));
	AddFunction(new nfIsAtLower(init));
	AddFunction(new nfIsAtUpper(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassAnimatorController::GetController(dsRealObject *myself, deAnimator *&animator,
deAnimatorInstance *&instance, int &index) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	const sAnimatorCtrlNatDat &nd = *((sAnimatorCtrlNatDat*)p_GetNativeData(myself->GetBuffer()));
	animator = nd.animator;
	instance = nd.instance;
	index = nd.index;
}

void deClassAnimatorController::PushController(dsRunTime *rt, deAnimator *animator, int index){
	if(!rt || !animator || index < 0 || index >= animator->GetControllerCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sAnimatorCtrlNatDat &nd = *((sAnimatorCtrlNatDat*)p_GetNativeData(
		rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.animator = animator;
	animator->AddReference();
	nd.instance = NULL;
	nd.index = index;
}

void deClassAnimatorController::PushController(dsRunTime *rt, deAnimatorInstance *instance, int index){
	if(!rt || !instance || index < 0 || index >= instance->GetControllerCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sAnimatorCtrlNatDat &nd = *((sAnimatorCtrlNatDat*)p_GetNativeData(
		rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.animator = NULL;
	nd.instance = instance;
	instance->AddReference();
	nd.index = index;
}
