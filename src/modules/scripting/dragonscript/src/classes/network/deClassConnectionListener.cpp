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
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassConnection.h"
#include "deClassNetworkState.h"
#include "deClassNetworkMessage.h"
#include "deClassConnectionListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/deEngine.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingConnection.h"
#include <libdscript/exceptions.h>



// Native functions
/////////////////////

// public func void connectionClosed()
deClassConnectionListener::nfConnectionClosed::nfConnectionClosed(const sInitData &init) :
dsFunction(init.clsConL, "connectionClosed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
}
void deClassConnectionListener::nfConnectionClosed::RunFunction(dsRunTime*, dsValue*){
}

// public func void messageProgress( int bytesReceived )
deClassConnectionListener::nfMessageProgress::nfMessageProgress(const sInitData &init) :
dsFunction(init.clsConL, "messageProgress", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // bytesReceived
}
void deClassConnectionListener::nfMessageProgress::RunFunction(dsRunTime*, dsValue*){
}

// public func void messageReceived( NetworkMessage message )
deClassConnectionListener::nfMessageReceived::nfMessageReceived(const sInitData &init) :
dsFunction(init.clsConL, "messageReceived", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsNM); // message
}
void deClassConnectionListener::nfMessageReceived::RunFunction(dsRunTime*, dsValue*){
}

// public func bool linkState( NetworkState state, NetworkMessage message )
deClassConnectionListener::nfLinkState::nfLinkState(const sInitData &init) :
dsFunction(init.clsConL, "linkState", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsNS); // state
	p_AddParameter(init.clsNM); // message
}
void deClassConnectionListener::nfLinkState::RunFunction(dsRunTime*, dsValue*){
}



// Class deClassConnectionListener
////////////////////////////////////

// Constructor, Destructors
/////////////////////////////

deClassConnectionListener::deClassConnectionListener(deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass("ConnectionListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT){
	if(! gameEngine || ! scrMgr) DSTHROW(dueInvalidParam);
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// set parser info
	GetParserInfo()->SetParent(DENS_NETWORKING);
	
	// do the rest
	p_SetNativeDataSize(0);
}

deClassConnectionListener::~deClassConnectionListener(){
}



// Management
///////////////

void deClassConnectionListener::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	pClsNM = pScrMgr->GetClassNetworkMessage();
	pClsNS = pScrMgr->GetClassNetworkState();
	
	init.clsConL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsNM = pClsNM;
	init.clsNS = pClsNS;
	
	// add functions
	AddFunction(new nfConnectionClosed(init)); // function 0
	AddFunction(new nfMessageProgress(init)); // function 1
	AddFunction(new nfMessageReceived(init)); // function 2
	AddFunction(new nfLinkState(init)); // function 3
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexMessageReceived = funcList.GetIndexOf(GetFunction(2));
	pFuncIndexLinkState = funcList.GetIndexOf(GetFunction(3));
}

