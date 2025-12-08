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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassServer.h"
#include "deClassServerListener.h"
#include "../../peers/dedsServer.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/resources/deResource.h"
#include "dragengine/resources/deResourceManager.h"
#include "dragengine/resources/network/deServer.h"
#include "dragengine/resources/network/deServerManager.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>



// native structure
struct sSvrNatDat{
	deServer::Ref server;
};



// Native functions
/////////////////////

// Constructor, destructors
/////////////////////////////

// public func new()
deClassServer::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSvr,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassServer::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSvrNatDat *nd = (sSvrNatDat*)p_GetNativeData(myself);
	deClassServer *clsSvr = (deClassServer*)GetOwnerClass();
	deServerManager *svrMgr = clsSvr->GetGameEngine()->GetServerManager();
	
	// clear (important)
	nd->server = NULL;
	
	// create server
	nd->server = svrMgr->CreateServer();
	if(!nd->server) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassServer::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSvr,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassServer::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSvrNatDat *nd = (sSvrNatDat*)p_GetNativeData(myself);
	
	if(nd->server){
		nd->server->FreeReference();
		nd->server = NULL;
	}
}



// Management
///////////////

// public func String getAddress()
deClassServer::nfGetAddress::nfGetAddress(const sInitData &init) : dsFunction(init.clsSvr,
"getAddress", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassServer::nfGetAddress::RunFunction(dsRunTime *rt, dsValue *myself){
	deServer::Ref server = ((sSvrNatDat*)p_GetNativeData(myself))->server;
	
	rt->PushString(server->GetAddress());
}

// public func bool listenOn( String address )
deClassServer::nfListenOn::nfListenOn(const sInitData &init) : dsFunction(init.clsSvr,
"listenOn", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsStr); // address
}
void deClassServer::nfListenOn::RunFunction(dsRunTime *rt, dsValue *myself){
	deServer::Ref server = ((sSvrNatDat*)p_GetNativeData(myself))->server;
	
	rt->PushBool(server->ListenOn(rt->GetValue(0)->GetString()));
}

// public func void stopListening()
deClassServer::nfStopListening::nfStopListening(const sInitData &init) : dsFunction(init.clsSvr,
"stopListening", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassServer::nfStopListening::RunFunction(dsRunTime *rt, dsValue *myself){
	deServer::Ref server = ((sSvrNatDat*)p_GetNativeData(myself))->server;
	
	server->StopListening();
}



// Notifications
//////////////////

// public func ServerListener getServerListener()
deClassServer::nfGetServerListener::nfGetServerListener(const sInitData &init) : dsFunction(init.clsSvr,
"getServerListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSvrL){
}
void deClassServer::nfGetServerListener::RunFunction(dsRunTime *rt, dsValue *myself){
	deServer::Ref server = ((sSvrNatDat*)p_GetNativeData(myself))->server;
	dedsServer *scrServer = (dedsServer*)server->GetPeerScripting();
	deClassServer *clsServer = (deClassServer*)GetOwnerClass();
	
	if(scrServer){
		rt->PushObject(scrServer->GetCallback(), clsServer->GetClassServerListener());
		
	}else{
		rt->PushObject(NULL, clsServer->GetClassServerListener());
	}
}

// public func void setServerListener( ServerListener listener )
deClassServer::nfSetServerListener::nfSetServerListener(const sInitData &init) : dsFunction(init.clsSvr,
"setServerListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSvrL); // listener
}
void deClassServer::nfSetServerListener::RunFunction(dsRunTime *rt, dsValue *myself){
	deServer::Ref server = ((sSvrNatDat*)p_GetNativeData(myself))->server;
	dedsServer *scrServer = (dedsServer*)server->GetPeerScripting();
	if(scrServer){
		scrServer->SetCallback(rt->GetValue(0)->GetRealObject());
	}
}



// Misc
/////////

// public func int hashCode()
deClassServer::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsSvr, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassServer::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deServer::Ref server = ((sSvrNatDat*)p_GetNativeData(myself))->server;
	
	rt->PushInt((int)(intptr_t)server);
}

// public func bool equals( Object object )
deClassServer::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSvr, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassServer::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deServer::Ref server = ((sSvrNatDat*)p_GetNativeData(myself))->server;
	deClassServer *clsSvr = (deClassServer*)GetOwnerClass();
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSvr)){
		rt->PushBool(false);
	}else{
		deServer *otherServer = ((sSvrNatDat*)p_GetNativeData(obj))->server;
		rt->PushBool(server == otherServer);
	}
}



// Class deClassServer
/////////////////////////

// Constructor
////////////////

deClassServer::deClassServer(deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass("Server", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE){
	if(!gameEngine || !scrMgr) DSTHROW(dueInvalidParam);
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// store information into parser info
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	// do the rest
	p_SetNativeDataSize(sizeof(sSvrNatDat));
}

deClassServer::~deClassServer(){
}



// Management
///////////////

void deClassServer::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	pClsSvrL = pScrMgr->GetClassServerListener();
	
	init.clsSvr = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsSvrL = pClsSvrL;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetAddress(init));
	
	AddFunction(new nfListenOn(init));
	AddFunction(new nfStopListening(init));
	
	AddFunction(new nfGetServerListener(init));
	AddFunction(new nfSetServerListener(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deServer *deClassServer::GetServer(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sSvrNatDat*)p_GetNativeData(myself->GetBuffer()))->server;
}

void deClassServer::PushServer(dsRunTime *rt, deServer *server){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!server){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sSvrNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->server = server;
}
