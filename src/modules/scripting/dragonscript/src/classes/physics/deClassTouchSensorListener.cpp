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

#include "deClassTouchSensorListener.h"
#include "deClassTouchSensor.h"
#include "../collider/deClassCollider.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingTouchSensor.h>

#include <libdscript/exceptions.h>


// native functions
/////////////////////



// public func void colliderEntered( int shape, Collider collider )
deClassTouchSensorListener::nfColliderEntered::nfColliderEntered(const sInitData &init) :
dsFunction(init.clsTSL, "colliderEntered", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassTouchSensorListener::nfColliderEntered::RunFunction(dsRunTime *RT, dsValue *This){
}

// public func void colliderLeft( int shape, Collider collider )
deClassTouchSensorListener::nfColliderLeft::nfColliderLeft(const sInitData &init) :
dsFunction(init.clsTSL, "colliderLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCol); // collider
}
void deClassTouchSensorListener::nfColliderLeft::RunFunction(dsRunTime *RT, dsValue *This){
}



// class deClassTouchSensorListener
//////////////////////////////////

// constructor
deClassTouchSensorListener::deClassTouchSensorListener(deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass("TouchSensorListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT){
	if(!gameEngine || !scrMgr) DSTHROW(dueInvalidParam);
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	// set parser info
	GetParserInfo()->SetParent(DENS_SCENERY);
	// do the rest
	p_SetNativeDataSize(0);
}
deClassTouchSensorListener::~deClassTouchSensorListener(){
}

// management
void deClassTouchSensorListener::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsTSL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInt = engine->GetClassInt();
	init.clsCol = pScrMgr->GetClassCollider();
	
	// add functions
	AddFunction(new nfColliderEntered(init)); // function 0
	AddFunction(new nfColliderLeft(init)); // function 1
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexColliderEntered = funcList.GetIndexOf(GetFunction(0));
	pFuncIndexColliderLeft = funcList.GetIndexOf(GetFunction(1));
}
