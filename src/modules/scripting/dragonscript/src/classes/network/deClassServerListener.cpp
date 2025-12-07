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
#include "deClassServerListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>



// Native functions
/////////////////////

// public func Connection incomingConnection()
deClassServerListener::nfClientConnected::nfClientConnected(const sInitData &init) :
dsFunction(init.clsSvrL, "clientConnected", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCon);
}
void deClassServerListener::nfClientConnected::RunFunction(dsRunTime *rt, dsValue *myself){
}



// class deClassServerListener
////////////////////////////////

// constructor
deClassServerListener::deClassServerListener(deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass("ServerListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT){
	if(! gameEngine || ! scrMgr) DSTHROW(dueInvalidParam);
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// set parser info
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	
	// do the rest
	p_SetNativeDataSize(0);
}
deClassServerListener::~deClassServerListener(){
}

// management
void deClassServerListener::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	pClsCon = pScrMgr->GetClassConnection();
	
	init.clsSvrL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	init.clsCon = pClsCon;
	
	// add functions
	AddFunction(new nfClientConnected(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

