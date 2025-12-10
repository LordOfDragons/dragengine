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

#include "deClassInputDevice.h"
#include "deClassInputDeviceAxis.h"
#include "deClassInputDeviceComponent.h"
#include "../graphics/deClassImage.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsInputDevice.h"

#include <dragengine/deEngine.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sIDAxisNatDat{
	dedsInputDevice *device;
	int axisIndex;
};



// Native functions
/////////////////////

// public func destructor()
deClassInputDeviceAxis::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsIDAxis, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassInputDeviceAxis::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sIDAxisNatDat*>(p_GetNativeData(myself))->~sIDAxisNatDat();
}



// Management
///////////////

// public func InputDevice getInputDevice()
deClassInputDeviceAxis::nfGetInputDevice::nfGetInputDevice(const sInitData &init) :
dsFunction(init.clsIDAxis, "getInputDevice", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDevice){
}
void deClassInputDeviceAxis::nfGetInputDevice::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (static_cast<deClassInputDeviceAxis*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassInputDevice()->PushInputDevice(rt, nd.device);
}

// public func int getAxisIndex()
deClassInputDeviceAxis::nfGetAxisIndex::nfGetAxisIndex(const sInitData &init) :
dsFunction(init.clsIDAxis, "getAxisIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassInputDeviceAxis::nfGetAxisIndex::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	
	rt->PushInt(nd.axisIndex);
}



// public func String getID()
deClassInputDeviceAxis::nfGetID::nfGetID(const sInitData &init) :
dsFunction(init.clsIDAxis, "getID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassInputDeviceAxis::nfGetID::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	
	rt->PushString(axis.GetID());
}

// public func String getName()
deClassInputDeviceAxis::nfGetName::nfGetName(const sInitData &init) :
dsFunction(init.clsIDAxis, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassInputDeviceAxis::nfGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	
	rt->PushString(axis.GetName());
}

// public func InputDeviceAxisType getType()
deClassInputDeviceAxis::nfGetType::nfGetType(const sInitData &init) :
dsFunction(init.clsIDAxis, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDeviceAxisType){
}
void deClassInputDeviceAxis::nfGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	
	rt->PushValue((static_cast<deClassInputDeviceAxis*>(GetOwnerClass()))->GetClassInputDeviceAxisType()
		->GetVariable(axis.GetType())->GetStaticValue());
}

// public func String getComponent()
deClassInputDeviceAxis::nfGetComponent::nfGetComponent(const sInitData &init) :
dsFunction(init.clsIDAxis, "getComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassInputDeviceAxis::nfGetComponent::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	
	rt->PushString(axis.GetComponent());
}

// public func Image getDisplayImage()
deClassInputDeviceAxis::nfGetDisplayImage::nfGetDisplayImage(const sInitData &init) :
dsFunction(init.clsIDAxis, "getDisplayImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage){
}
void deClassInputDeviceAxis::nfGetDisplayImage::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	const deScriptingDragonScript &ds = (static_cast<deClassInputDeviceAxis*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassImage()->PushImage(rt, axis.GetDisplayImage());
}

// public func int getDisplayIconCount()
deClassInputDeviceAxis::nfGetDisplayIconCount::nfGetDisplayIconCount(const sInitData &init) :
dsFunction(init.clsIDAxis, "getDisplayIconCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassInputDeviceAxis::nfGetDisplayIconCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	rt->PushInt(axis.GetDisplayIconCount());
}

// public func Image getDisplayIconAt( int index )
deClassInputDeviceAxis::nfGetDisplayIconAt::nfGetDisplayIconAt(const sInitData &init) :
dsFunction(init.clsIDAxis, "getDisplayIconAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage){
	p_AddParameter(init.clsInteger); // index
}
void deClassInputDeviceAxis::nfGetDisplayIconAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	const deScriptingDragonScript &ds = (static_cast<deClassInputDeviceAxis*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassImage()->PushImage(rt, axis.GetDisplayIconAt(rt->GetValue(0)->GetInt()));
}

// public func Image getLargestDisplayIconX( int maxWidth )
deClassInputDeviceAxis::nfGetLargestDisplayIconX::nfGetLargestDisplayIconX(const sInitData &init) :
dsFunction(init.clsIDAxis, "getLargestDisplayIconX", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage){
	p_AddParameter(init.clsInteger); // maxWidth
}
void deClassInputDeviceAxis::nfGetLargestDisplayIconX::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	const deScriptingDragonScript &ds = (static_cast<deClassInputDeviceAxis*>(GetOwnerClass()))->GetDS();
	const int count = axis.GetDisplayIconCount();
	const int maxWidth = rt->GetValue(0)->GetInt();
	deImage *bestIcon = nullptr;
	int bestWidth = 0;
	int i;
	
	for(i=0; i<count; i++){
		deImage * const icon = axis.GetDisplayIconAt(i);
		if(icon->GetWidth() <= maxWidth && (!bestIcon || icon->GetWidth() >= bestWidth)){
			bestIcon = icon;
			bestWidth = icon->GetWidth();
		}
	}
	
	ds.GetClassImage()->PushImage(rt, bestIcon);
}

// public func Image getLargestDisplayIconY( int maxHeight )
deClassInputDeviceAxis::nfGetLargestDisplayIconY::nfGetLargestDisplayIconY(const sInitData &init) :
dsFunction(init.clsIDAxis, "getLargestDisplayIconY", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage){
	p_AddParameter(init.clsInteger); // maxHeight
}
void deClassInputDeviceAxis::nfGetLargestDisplayIconY::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	const deScriptingDragonScript &ds = (static_cast<deClassInputDeviceAxis*>(GetOwnerClass()))->GetDS();
	const int count = axis.GetDisplayIconCount();
	const int maxHeight = rt->GetValue(0)->GetInt();
	deImage *bestIcon = nullptr;
	int bestHeight = 0;
	int i;
	
	for(i=0; i<count; i++){
		deImage * const icon = axis.GetDisplayIconAt(i);
		if(icon->GetHeight() <= maxHeight && (!bestIcon || icon->GetHeight() >= bestHeight)){
			bestIcon = icon;
			bestHeight = icon->GetHeight();
		}
	}
	
	ds.GetClassImage()->PushImage(rt, bestIcon);
}

// public func String getDisplayText()
deClassInputDeviceAxis::nfGetDisplayText::nfGetDisplayText(const sInitData &init) :
dsFunction(init.clsIDAxis, "getDisplayText", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassInputDeviceAxis::nfGetDisplayText::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	const deInputDeviceAxis &axis = nd.device->GetDevice()->GetAxisAt(nd.axisIndex);
	
	rt->PushString(axis.GetDisplayText());
}



// public func float getValue()
deClassInputDeviceAxis::nfGetValue::nfGetValue(const sInitData &init) :
dsFunction(init.clsIDAxis, "getValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassInputDeviceAxis::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	deScriptingDragonScript &ds = (static_cast<deClassInputDeviceAxis*>(GetOwnerClass()))->GetDS();
	
	switch(nd.device->GetDeviceSource()){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
		const int deviceIndex = module.IndexOfDeviceWithID(nd.device->GetDevice()->GetID());
		if(deviceIndex != -1){
			rt->PushFloat(module.GetAxisValue(deviceIndex, nd.axisIndex));
			
		}else{
			rt->PushFloat(0.0f);
		}
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
		const int deviceIndex = module.IndexOfDeviceWithID(nd.device->GetDevice()->GetID());
		if(deviceIndex != -1){
			rt->PushFloat(module.GetAxisValue(deviceIndex, nd.axisIndex));
			
		}else{
			rt->PushFloat(0.0f);
		}
		}break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "input source");
	}
}



// public func bool equals( Object obj )
deClassInputDeviceAxis::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsIDAxis, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassInputDeviceAxis::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassInputDeviceAxis * const clsIDAxis = static_cast<deClassInputDeviceAxis*>(GetOwnerClass());
	const sIDAxisNatDat &nd = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(myself));
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsIDAxis)){
		rt->PushBool(false);
		
	}else{
		const sIDAxisNatDat &other = *static_cast<const sIDAxisNatDat*>(p_GetNativeData(obj));
		rt->PushBool(nd.device == other.device && nd.axisIndex == other.axisIndex);
	}
}



// Class deClassInputDeviceAxis
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassInputDeviceAxis::deClassInputDeviceAxis(deScriptingDragonScript &ds) :
dsClass("InputDeviceAxis", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sIDAxisNatDat));
}

deClassInputDeviceAxis::~deClassInputDeviceAxis(){
}



// Management
///////////////

void deClassInputDeviceAxis::CreateClassMembers(dsEngine *engine){
	pClsInputDeviceAxisType = engine->GetClass("Dragengine.InputDeviceAxisType");
	
	sInitData init;
	init.clsIDAxis = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsInputDevice = pDS.GetClassInputDevice();
	init.clsInputDeviceAxisType = pClsInputDeviceAxisType;
	init.clsImage = pDS.GetClassImage();
	
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetInputDevice(init));
	AddFunction(new nfGetAxisIndex(init));
	
	AddFunction(new nfGetID(init));
	AddFunction(new nfGetName(init));
	AddFunction(new nfGetType(init));
	AddFunction(new nfGetComponent(init));
	AddFunction(new nfGetDisplayImage(init));
	AddFunction(new nfGetDisplayIconCount(init));
	AddFunction(new nfGetDisplayIconAt(init));
	AddFunction(new nfGetLargestDisplayIconX(init));
	AddFunction(new nfGetLargestDisplayIconY(init));
	AddFunction(new nfGetDisplayText(init));
	
	AddFunction(new nfGetValue(init));
	
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

void deClassInputDeviceAxis::PushAxis(dsRunTime *rt, dedsInputDevice *device, int index){
	if(!rt || !device || index < 0 || index >= device->GetDevice()->GetAxisCount()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sIDAxisNatDat * const nd = new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sIDAxisNatDat;
	nd->device = device;
	device->AddReference();
	nd->axisIndex = index;
}
