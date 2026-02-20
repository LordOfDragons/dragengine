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

#include "deEosSdkFlow.h"
#include "../deEosSdkServiceEos.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deEosSdkFlow
///////////////////////

// Constructor, destructor
////////////////////////////

deEosSdkFlow::deEosSdkFlow(deEosSdkServiceEos &service, const decUniqueID &id) :
pService(service),
pId(id),
pHasFailed(false){
}

deEosSdkFlow::~deEosSdkFlow(){
}



// Management
///////////////

deEosSdk &deEosSdkFlow::GetModule() const{
	return pService.GetModule();
}


void deEosSdkFlow::Fail(const deException &e){
	const deEosSdkPendingRequest::Ref pr(pService.RemoveFirstPendingRequestWithId(pId));
	if(pr && !pHasFailed){
		Fail(pr, e);
		
	}else{
		GetModule().LogException(e);
		pHasFailed = true;
	}
}

void deEosSdkFlow::Fail(EOS_EResult res){
	const deEosSdkPendingRequest::Ref pr(pService.RemoveFirstPendingRequestWithId(pId));
	if(pr && !pHasFailed){
		Fail(pr, res);
		
	}else{
		GetModule().LogErrorFormat("deEosSdkFlow: %s", EOS_EResult_ToString(res));
		pHasFailed = true;
	}
}

void deEosSdkFlow::Fail(const deEosSdkPendingRequest::Ref &pr, const deException &e){
	if(pHasFailed){
		return;
	}
	
	GetModule().LogException(e);
	
	pr->data->SetStringChildAt("error", e.GetName().GetMiddle(3));
	pr->data->SetStringChildAt("message", e.GetDescription());
	
	GetModule().GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		pService.GetService(), pr->id, pr->data);
	pHasFailed = true;
}

void deEosSdkFlow::Fail(const deEosSdkPendingRequest::Ref &request, EOS_EResult res){
	GetModule().LogErrorFormat("deEosSdkFlow: %s", EOS_EResult_ToString(res));
	
	if(pHasFailed){
		return;
	}
	
	request->data->SetIntChildAt("code", (int)res);
	request->data->SetStringChildAt("error", EOS_EResult_ToString(res));
	request->data->SetStringChildAt("message", EOS_EResult_ToString(res));
	
	GetModule().GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		pService.GetService(), request->id, request->data);
	pHasFailed = true;
}

void deEosSdkFlow::Finish(){
	const deEosSdkPendingRequest::Ref pr(pService.RemoveFirstPendingRequestWithId(pId));
	if(pr && !pHasFailed){
		try{
			GetModule().GetGameEngine()->GetServiceManager()->QueueRequestResponse(
				pService.GetService(), pr->id, pr->data, true);
			
		}catch(const deException &e){
			Fail(pr, e);
		}
	}
	
	Abandon();
}

void deEosSdkFlow::Abandon(){
	delete this;
}
