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

#include "deClassSky.h"
#include "deClassSkyController.h"
#include "deClassSkyInstance.h"
#include "../physics/deClassLayerMask.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyInstanceManager.h>
#include <dragengine/resources/sky/deSkyController.h>



// Native Structure
/////////////////////

struct sSkyInstNatDat{
	deSkyInstance::Ref instance;
};



// Native functions
/////////////////////

// public func new()
deClassSkyInstance::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSkyInst, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyInstance::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	deScriptingDragonScript &ds = ((deClassSkyInstance*)GetOwnerClass())->GetDS();
	deSkyInstanceManager &skyInstMgr = *ds.GetGameEngine()->GetSkyInstanceManager();
	sSkyInstNatDat * const nd = new (p_GetNativeData(myself)) sSkyInstNatDat;
	
	// create sky
	nd->instance = skyInstMgr.CreateSkyInstance();
}

// public func destructor()
deClassSkyInstance::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkyInst, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkyInstance::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->~sSkyInstNatDat();
}



// Management
///////////////

// public func Sky getSky()
deClassSkyInstance::nfGetSky::nfGetSky(const sInitData &init) :
dsFunction(init.clsSkyInst, "getSky", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSky){
}
void deClassSkyInstance::nfGetSky::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	deScriptingDragonScript &ds = ((deClassSkyInstance*)GetOwnerClass())->GetDS();
	
	ds.GetClassSky()->PushSky(rt, instance.GetSky());
}

// public func void setSky( Sky sky )
deClassSkyInstance::nfSetSky::nfSetSky(const sInitData &init) : dsFunction(init.clsSkyInst,
"setSky", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSky); // sky
}
void deClassSkyInstance::nfSetSky::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	deScriptingDragonScript &ds = ((deClassSkyInstance*)GetOwnerClass())->GetDS();
	
	instance.SetSky(ds.GetClassSky()->GetSky(rt->GetValue(0)->GetRealObject()));
}

// public func int getOrder()
deClassSkyInstance::nfGetOrder::nfGetOrder(const sInitData &init) :
dsFunction(init.clsSkyInst, "getOrder", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyInstance::nfGetOrder::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	rt->PushInt(instance.GetOrder());
}

// public func void setOrder( int order )
deClassSkyInstance::nfSetOrder::nfSetOrder(const sInitData &init) :
dsFunction(init.clsSkyInst, "setOrder", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // order
}
void deClassSkyInstance::nfSetOrder::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	instance.SetOrder(rt->GetValue(0)->GetInt());
}

// public func LayerMask getLayerMask()
deClassSkyInstance::nfGetLayerMask::nfGetLayerMask(const sInitData &init) :
dsFunction(init.clsSkyInst, "getLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask){
}
void deClassSkyInstance::nfGetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	deScriptingDragonScript &ds = ((deClassSkyInstance*)GetOwnerClass())->GetDS();
	
	ds.GetClassLayerMask()->PushLayerMask(rt, instance.GetLayerMask());
}

// public func void setLayerMask( LayerMask layerMask )
deClassSkyInstance::nfSetLayerMask::nfSetLayerMask(const sInitData &init) :
dsFunction(init.clsSkyInst, "setLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLayerMask); // layerMask
}
void deClassSkyInstance::nfSetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	deScriptingDragonScript &ds = ((deClassSkyInstance*)GetOwnerClass())->GetDS();
	
	instance.SetLayerMask(ds.GetClassLayerMask()->GetLayerMask(rt->GetValue(0)->GetRealObject()));
}

// func float getPassthroughTransparency()
deClassSkyInstance::nfGetPassthroughTransparency::nfGetPassthroughTransparency(const sInitData &init) :
dsFunction(init.clsSkyInst, "getPassthroughTransparency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSkyInstance::nfGetPassthroughTransparency::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	rt->PushFloat(instance.GetPassthroughTransparency());
}

// func void setPassthroughTransparency(float transparency)
deClassSkyInstance::nfSetPassthroughTransparency::nfSetPassthroughTransparency(const sInitData &init) :
dsFunction(init.clsSkyInst, "setPassthroughTransparency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // transparency
}
void deClassSkyInstance::nfSetPassthroughTransparency::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	instance.SetPassthroughTransparency(rt->GetValue(0)->GetFloat());
}


// public func int getControllerCount()
deClassSkyInstance::nfGetControllerCount::nfGetControllerCount(const sInitData &init) :
dsFunction(init.clsSkyInst, "getControllerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSkyInstance::nfGetControllerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkyInstance &instance = *(static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance);
	
	rt->PushInt(instance.GetControllerCount());
}

// public func SkyController getControllerAt( int index )
deClassSkyInstance::nfGetControllerAt::nfGetControllerAt(const sInitData &init) :
dsFunction(init.clsSkyInst, "getControllerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyCtrl){
	p_AddParameter(init.clsInt); // index
}
void deClassSkyInstance::nfGetControllerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance * const instance = static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance;
	deScriptingDragonScript &ds = ((deClassSkyInstance*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	
	if(index < 0){
		ds.GetClassSkyController()->PushController(rt, instance,
			instance->GetControllerCount() + index);
		
	}else{
		ds.GetClassSkyController()->PushController(rt, instance, index);
	}
}

// public func SkyController getControllerNamed( String name )
deClassSkyInstance::nfGetControllerNamed::nfGetControllerNamed(const sInitData &init) :
dsFunction(init.clsSkyInst, "getControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkyCtrl){
	p_AddParameter(init.clsStr); // name
}
void deClassSkyInstance::nfGetControllerNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance * const instance = static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance;
	const deScriptingDragonScript &ds = ((deClassSkyInstance*)GetOwnerClass())->GetDS();
	const int index = instance->IndexOfControllerNamed(rt->GetValue(0)->GetString());
	
	if(index == -1){
		rt->PushObject(NULL, ds.GetClassSkyController());
		
	}else{
		ds.GetClassSkyController()->PushController(rt, instance, index);
	}
}

// public func int indexOfControllerNamed( String name )
deClassSkyInstance::nfIndexOfControllerNamed::nfIndexOfControllerNamed(const sInitData &init) :
dsFunction(init.clsSkyInst, "indexOfControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsStr); // name
}
void deClassSkyInstance::nfIndexOfControllerNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance * const instance = static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance;
	rt->PushInt(instance->IndexOfControllerNamed(rt->GetValue(0)->GetString()));
}



// public func int hashCode()
deClassSkyInstance::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsSkyInst, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassSkyInstance::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkyInstance * const instance = static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance;
	
	rt->PushInt((int)(intptr_t)instance);
}

// public func bool equals( Object obj )
deClassSkyInstance::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSkyInst, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassSkyInstance::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deSkyInstance * const instance = static_cast<sSkyInstNatDat*>(p_GetNativeData(myself))->instance;
	deClassSkyInstance * const clsSkyInst = (deClassSkyInstance*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSkyInst)){
		rt->PushBool(false);
		
	}else{
		deSkyInstance * const other = static_cast<sSkyInstNatDat*>(p_GetNativeData(obj))->instance;
		rt->PushBool(instance == other);
	}
}



// Class deClassSkyInstance
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkyInstance::deClassSkyInstance(deScriptingDragonScript &ds) :
dsClass("SkyInstance", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSkyInstNatDat));
}

deClassSkyInstance::~deClassSkyInstance(){
}



// Management
///////////////

void deClassSkyInstance::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsSkyInst = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsSky = pDS.GetClassSky();
	init.clsSkyCtrl = pDS.GetClassSkyController();
	init.clsLayerMask = pDS.GetClassLayerMask();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetSky(init));
	AddFunction(new nfSetSky(init));
	AddFunction(new nfGetOrder(init));
	AddFunction(new nfSetOrder(init));
	AddFunction(new nfGetLayerMask(init));
	AddFunction(new nfSetLayerMask(init));
	AddFunction(new nfGetPassthroughTransparency(init));
	AddFunction(new nfSetPassthroughTransparency(init));
	
	AddFunction(new nfGetControllerCount(init));
	AddFunction(new nfGetControllerAt(init));
	AddFunction(new nfGetControllerNamed(init));
	AddFunction(new nfIndexOfControllerNamed(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

deSkyInstance *deClassSkyInstance::GetInstance(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sSkyInstNatDat*>(p_GetNativeData(myself->GetBuffer()))->instance;
}

void deClassSkyInstance::PushInstance(dsRunTime *rt, deSkyInstance *instance){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!instance){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	static_cast<sSkyInstNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->instance = instance;
	instance->AddReference();
}
