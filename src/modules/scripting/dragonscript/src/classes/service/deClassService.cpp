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

#include <stdint.h>

#include "deClassService.h"
#include "deClassServiceListener.h"
#include "deClassServiceObject.h"
#include "../utils/deClassUniqueID.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../peers/dedsService.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/resources/service/deService.h>
#include <dragengine/resources/service/deServiceObject.h>
#include <dragengine/resources/service/deServiceManager.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sServiceNatDat{
	deService::Ref service;
};



// Native functions
/////////////////////

// func new(String name)
deClassService::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsService, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
}

void deClassService::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	deScriptingDragonScript &ds = (static_cast<deClassService*>(GetOwnerClass()))->GetDS();
	sServiceNatDat * const nd = new (p_GetNativeData(myself)) sServiceNatDat;
	nd->service = nullptr;
	
	// create object
	const char * const name = rt->GetValue(0)->GetString();
	DS_WITH_ENGEX(ds,
		nd->service = ds.GetGameEngine()->GetServiceManager()->CreateService(name, {});
)
}

// func new(String name, ServiceObject data)
deClassService::nfNew2::nfNew2(const sInitData &init) :
dsFunction(init.clsService, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsServiceObject); // data
}

void deClassService::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	deScriptingDragonScript &ds = (static_cast<deClassService*>(GetOwnerClass()))->GetDS();
	sServiceNatDat * const nd = new (p_GetNativeData(myself)) sServiceNatDat;
	nd->service = nullptr;
	
	// create object
	const char * const name = rt->GetValue(0)->GetString();
	const deServiceObject::Ref data(ds.GetClassServiceObject()->GetServiceObject(
		rt->GetValue(1)->GetRealObject()));
	
	DS_WITH_ENGEX(ds,
		nd->service = ds.GetGameEngine()->GetServiceManager()->CreateService(name, data);
)
}

// public func destructor()
deClassService::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsService, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}

void deClassService::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sServiceNatDat*>(p_GetNativeData(myself))->~sServiceNatDat();
}



// Management
///////////////

// func String getName()
deClassService::nfGetName::nfGetName(const sInitData &init) :
dsFunction(init.clsService, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}

void deClassService::nfGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushString(nd.service->GetName());
}


// func ServiceListener getListener()
deClassService::nfGetListener::nfGetListener(const sInitData &init) :
dsFunction(init.clsService, "getListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceListener){
}

void deClassService::nfGetListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = (static_cast<deClassService*>(GetOwnerClass()))->GetDS();
	dedsService * const peer = (dedsService*)nd.service->GetPeerScripting();
	
	if(peer){
		rt->PushObject(peer->GetCallback(), ds.GetClassServiceListener());
		
	}else{
		rt->PushObject(nullptr, ds.GetClassServiceListener());
	}
}


// func void setListener(ServiceListener listener)
deClassService::nfSetListener::nfSetListener(const sInitData &init) :
dsFunction(init.clsService, "setListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsServiceListener); // listener
}

void deClassService::nfSetListener::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	dedsService * const peer = (dedsService*)nd.service->GetPeerScripting();
	if(peer){
		peer->SetCallback(rt->GetValue(0)->GetRealObject());
	}
}


// static func UniqueID nextId()
deClassService::nfNextId::nfNextId(const sInitData &init) :
dsFunction(init.clsService, "nextId", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsUniqueID){
}

void deClassService::nfNextId::RunFunction(dsRunTime *rt, dsValue*){
	deClassService &clsService = *(static_cast<deClassService*>(GetOwnerClass()));
	clsService.GetDS().GetClassUniqueID()->PushUniqueID(rt, clsService.NextId());
}


// func void startRequest(ServiceObject request)
deClassService::nfStartRequest::nfStartRequest(const sInitData &init) :
dsFunction(init.clsService, "startRequest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsServiceObject); // request
}

void deClassService::nfStartRequest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	deClassService &clsService = *(static_cast<deClassService*>(GetOwnerClass()));
	const deServiceObject * const request = clsService.GetDS().GetClassServiceObject()->
		GetServiceObject(rt->GetValue(0)->GetRealObject());
	
	if(!request){
		DSTHROW(dueNullPointer);
	}
	
	DS_WITH_ENGEX(clsService.GetDS(),
		nd.service->StartRequest(clsService.NextId(), *request);
)
}


// func void startRequest(UniqueID id, ServiceObject request)
deClassService::nfStartRequest2::nfStartRequest2(const sInitData &init) :
dsFunction(init.clsService, "startRequest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsUniqueID); // id
	p_AddParameter(init.clsServiceObject); // request
}

void deClassService::nfStartRequest2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	deScriptingDragonScript &ds = (static_cast<deClassService*>(GetOwnerClass()))->GetDS();
	const decUniqueID &id = ds.GetClassUniqueID()->GetUniqueID(
		rt->GetValue(0)->GetRealObject());
	const deServiceObject * const request = ds.GetClassServiceObject()->GetServiceObject(
		rt->GetValue(1)->GetRealObject());
	
	if(!request){
		DSTHROW(dueNullPointer);
	}
	
	DS_WITH_ENGEX(ds,
		nd.service->StartRequest(id, *request);
)
}


// func void cancelRequest(UniqueID id)
deClassService::nfCancelRequest::nfCancelRequest(const sInitData &init) :
dsFunction(init.clsService, "cancelRequest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsUniqueID); // id
}

void deClassService::nfCancelRequest::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	deScriptingDragonScript &ds = (static_cast<deClassService*>(GetOwnerClass()))->GetDS();
	const decUniqueID &id = ds.GetClassUniqueID()->GetUniqueID(
		rt->GetValue(0)->GetRealObject());
	
	DS_WITH_ENGEX(ds,
		nd.service->CancelRequest(id);
)
}


// func ServiceObject runActionn(ServiceObject action)
deClassService::nfRunAction::nfRunAction(const sInitData &init) :
dsFunction(init.clsService, "runAction", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsServiceObject); // action
}

void deClassService::nfRunAction::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	deClassService &clsService = *(static_cast<deClassService*>(GetOwnerClass()));
	deClassServiceObject &clsServiceObject = *clsService.GetDS().GetClassServiceObject();
	const deServiceObject * const action = clsServiceObject.GetServiceObject(
		rt->GetValue(0)->GetRealObject());
	
	if(!action){
		DSTHROW(dueNullPointer);
	}
	
	DS_WITH_ENGEX(clsService.GetDS(),
		clsServiceObject.PushServiceObject(rt, nd.service->RunAction(*action));
)
}


// func int hashCode()
deClassService::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsService, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassService::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushInt((int)(intptr_t)nd.service);
}


// func bool equals( Object obj )
deClassService::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsService, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}

void deClassService::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceNatDat &nd = *static_cast<sServiceNatDat*>(p_GetNativeData(myself));
	if(!nd.service){
		DSTHROW(dueNullPointer);
	}
	
	deClassService * const clsService = static_cast<deClassService*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsService)){
		rt->PushBool(false);
		
	}else{
		deService * const other = static_cast<sServiceNatDat*>(p_GetNativeData(obj))->service;
		rt->PushBool(nd.service == other);
	}
}


// Class deClassService
/////////////////////////

// Constructor, destructor
////////////////////////////

deClassService::deClassService(deScriptingDragonScript &ds) :
dsClass("Service", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds),
pNextId(1)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sServiceNatDat));
}

deClassService::~deClassService(){
}



// Management
///////////////

void deClassService::CreateClassMembers(dsEngine *engine){
	sInitData init;
	init.clsService = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInteger = engine->GetClassInt();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsServiceListener = pDS.GetClassServiceListener();
	init.clsServiceObject = pDS.GetClassServiceObject();
	init.clsUniqueID = pDS.GetClassUniqueID();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetName(init));
	
	AddFunction(new nfGetListener(init));
	AddFunction(new nfSetListener(init));
	
	AddFunction(new nfNextId(init));
	AddFunction(new nfStartRequest(init));
	AddFunction(new nfStartRequest2(init));
	AddFunction(new nfCancelRequest(init));
	AddFunction(new nfRunAction(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

deService *deClassService::GetService(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sServiceNatDat*>(p_GetNativeData(myself->GetBuffer()))->service;
}

void deClassService::PushService(dsRunTime *rt, deService *service){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!service){
		rt->PushObject(nullptr, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	static_cast<sServiceNatDat*>(p_GetNativeData(
		rt->GetValue(0)->GetRealObject()->GetBuffer()))->service = service;
	service->AddReference();
}

decUniqueID deClassService::NextId(){
	const decUniqueID id(pNextId);
	pNextId.Increment();
	return id;
}
