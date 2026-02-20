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

#include "deClassService.h"
#include "deClassServiceListener.h"
#include "deClassServiceObject.h"
#include "../utils/deClassUniqueID.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/resources/service/deServiceObject.h>

#include <libdscript/exceptions.h>


// Native Functions
/////////////////////

// func void requestResponse(Service service, UniqueID id, ServiceObject response, bool finished)
deClassServiceListener::nfRequestResponse::nfRequestResponse(const sInitData &init) :
dsFunction(init.clsServiceListener, "requestResponse", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsService); // service
	p_AddParameter(init.clsUniqueID); // id
	p_AddParameter(init.clsServiceObject); // response
	p_AddParameter(init.clsBoolean); // finished
}

void deClassServiceListener::nfRequestResponse::RunFunction(dsRunTime*, dsValue*){
}


// func void requestFailed(Service service, UniqueID id, ServiceObject error)
deClassServiceListener::nfRequestFailed::nfRequestFailed(const sInitData &init) :
dsFunction(init.clsServiceListener, "requestFailed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsService); // service
	p_AddParameter(init.clsUniqueID); // id
	p_AddParameter(init.clsServiceObject); // error
}

void deClassServiceListener::nfRequestFailed::RunFunction(dsRunTime*, dsValue*){
}


// func void eventReceived(Service service, ServiceObject event)
deClassServiceListener::nfEventReceived::nfEventReceived(const sInitData &init) :
dsFunction(init.clsServiceListener, "eventReceived", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsService); // service
	p_AddParameter(init.clsServiceObject); // event
}
void deClassServiceListener::nfEventReceived::RunFunction(dsRunTime*, dsValue*){
}



// Class deClassServiceListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassServiceListener::deClassServiceListener(deScriptingDragonScript &ds) :
dsClass("ServiceListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	
	p_SetNativeDataSize(0);
}

deClassServiceListener::~deClassServiceListener(){
}



// Management
///////////////

void deClassServiceListener::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsServiceListener = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsUniqueID = pDS.GetClassUniqueID();
	init.clsService = pDS.GetClassService();
	init.clsServiceObject = pDS.GetClassServiceObject();
	
	AddFunction(new nfRequestResponse(init)); // function 0
	AddFunction(new nfRequestFailed(init)); // function 1
	AddFunction(new nfEventReceived(init)); // function 2
	
	CalcMemberOffsets();
	
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexRequestResponse = funcList.GetIndexOf(GetFunction(0));
	pFuncIndexRequestFailed = funcList.GetIndexOf(GetFunction(1));
	pFuncIndexEventReceived = funcList.GetIndexOf(GetFunction(2));
}

