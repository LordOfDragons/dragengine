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

#include "deClassNetworkMessage.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/deNetworkMessageReader.h>
#include <dragengine/resources/network/deNetworkMessageWriter.h>

#include <libdscript/exceptions.h>


// native structure
struct sNMNatDat{
	deNetworkMessage::Ref message;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassNetworkMessage::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsNetworkMessage, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNetworkMessage::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	sNMNatDat &nd = *((sNMNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.message = NULL;
	
	// create message
	nd.message.TakeOver(new deNetworkMessage);
}

// public func destructor()
deClassNetworkMessage::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsNetworkMessage, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNetworkMessage::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sNMNatDat &nd = *((sNMNatDat*)p_GetNativeData(myself));
	
	if(nd.message){
		nd.message->FreeReference();
		nd.message = NULL;
	}
}



// Management
///////////////

// public func int getDataLength()
deClassNetworkMessage::nfGetDataLength::nfGetDataLength(const sInitData &init) :
dsFunction(init.clsNetworkMessage, "getDataLength", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassNetworkMessage::nfGetDataLength::RunFunction(dsRunTime *rt, dsValue *myself){
	const deNetworkMessage &message = *(((sNMNatDat*)p_GetNativeData(myself))->message);
	rt->PushInt(message.GetDataLength());
}

// public func void clear()
deClassNetworkMessage::nfClear::nfClear(const sInitData &init) :
dsFunction(init.clsNetworkMessage, "clear", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassNetworkMessage::nfClear::RunFunction(dsRunTime*, dsValue *myself){
	deNetworkMessage &message = *(((sNMNatDat*)p_GetNativeData(myself))->message);
	message.Clear();
}



// public func FileReader getReader()
deClassNetworkMessage::nfGetReader::nfGetReader(const sInitData &init) :
dsFunction(init.clsNetworkMessage, "getReader", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFileReader){
}
void deClassNetworkMessage::nfGetReader::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkMessage * const message = ((sNMNatDat*)p_GetNativeData(myself))->message;
	deScriptingDragonScript &ds = ((deClassNetworkMessage*)GetOwnerClass())->GetDS();
	
	ds.GetClassFileReader()->PushFileReader(rt, deNetworkMessageReader::Ref::NewWith(message));
}

// public func FileWriter getWriter( bool append )
deClassNetworkMessage::nfGetWriter::nfGetWriter(const sInitData &init) :
dsFunction(init.clsNetworkMessage, "getWriter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFileWrite){
	p_AddParameter(init.clsBool); // append
}
void deClassNetworkMessage::nfGetWriter::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkMessage * const message = ((sNMNatDat*)p_GetNativeData(myself))->message;
	deScriptingDragonScript &ds = ((deClassNetworkMessage*)GetOwnerClass())->GetDS();
	
	const bool append = rt->GetValue(0)->GetBool();
	ds.GetClassFileWriter()->PushFileWriter(rt, deNetworkMessageWriter::Ref::NewWith(message, append));
}



// Misc
/////////

// public func int hashCode()
deClassNetworkMessage::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsNetworkMessage, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassNetworkMessage::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkMessage * const message = ((sNMNatDat*)p_GetNativeData(myself))->message;
	rt->PushInt((int)(intptr_t)message);
}

// public func bool equals( Object object )
deClassNetworkMessage::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsNetworkMessage, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // object
}
void deClassNetworkMessage::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deNetworkMessage * const message = ((sNMNatDat*)p_GetNativeData(myself))->message;
	deClassNetworkMessage *clsNetworkMessage = (deClassNetworkMessage*)GetOwnerClass();
	dsValue * const object = rt->GetValue(0);
	
	if(!p_IsObjOfType(object, clsNetworkMessage)){
		rt->PushBool(false);
		
	}else{
		deNetworkMessage * const otherMessage = ((sNMNatDat*)p_GetNativeData(object))->message;
		rt->PushBool(message == otherMessage);
	}
}



// Class deClassNetworkMessage
////////////////////////////////

// Constructor
////////////////

deClassNetworkMessage::deClassNetworkMessage(deScriptingDragonScript &ds) :
dsClass("NetworkMessage", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_NETWORKING);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sNMNatDat));
}

deClassNetworkMessage::~deClassNetworkMessage(){
}



// Management
///////////////

void deClassNetworkMessage::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsNetworkMessage = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWrite = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetDataLength(init));
	AddFunction(new nfClear(init));
	AddFunction(new nfGetReader(init));
	AddFunction(new nfGetWriter(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deNetworkMessage *deClassNetworkMessage::GetNetworkMessage(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sNMNatDat*)p_GetNativeData(myself->GetBuffer()))->message;
}

void deClassNetworkMessage::PushNetworkMessage(dsRunTime *rt, deNetworkMessage::Ref message){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!message){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sNMNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->message = message;
}
