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

#include "gdk_include.h"
#include "deMicrosoftGdk.h"
#include "deMsgdkServiceMsgdk.h"
#include "tasks/deMTInitialize.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/utils/decBase64.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>


// Class deMsgdkServiceMsgdk
//////////////////////////////

const char * const deMsgdkServiceMsgdk::serviceName = "MicrosoftGdk";

// Constructor, destructor
////////////////////////////

deMsgdkServiceMsgdk::deMsgdkServiceMsgdk(deMicrosoftGdk &module,
	deService *service, const deServiceObject::Ref &data) :
pModule(module),
pService(service),
pIsInitialized(false),
pInvalidator(deMsgdkAsyncTask::Invalidator::Ref::New(new deMsgdkAsyncTask::Invalidator)),
pUser(nullptr),
pUserLocalId({})
{
	pModule.InitSdk(data);
	new deMTInitialize(*this);
}

deMsgdkServiceMsgdk::~deMsgdkServiceMsgdk()
{
	while( pPendingRequests.GetCount() > 0 )
	{
		CancelRequest(((deMsgdkPendingRequest*)pPendingRequests.GetAt(0))->id);
	}

	if(pUser)
	{
		XUserCloseHandle(pUser);
		pUser = nullptr;
	}

	pInvalidator->Invalidate();
}


// Management
///////////////

void deMsgdkServiceMsgdk::StartRequest(const decUniqueID& id, const deServiceObject& request)
{
	const decString &function = request.GetChildAt( "function" )->GetString();
	
	if(function == "dummy")
	{
		
	}
	else
	{
		DETHROW_INFO(deeInvalidParam, "Unknown function");
	}
}

void deMsgdkServiceMsgdk::CancelRequest(const decUniqueID& id)
{
	deMsgdkPendingRequest * const pr = GetPendingRequestWithId(id);
	if(!pr)
	{
		return;
	}
	
	pPendingRequests.RemoveFrom(pPendingRequests.IndexOf(pr));
	
	const deServiceObject::Ref so(deServiceObject::Ref::New(new deServiceObject));
	so->SetStringChildAt("error", "Cancelled");
	so->SetStringChildAt("message", "Request cancelled");
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, id, so);
}

deServiceObject::Ref deMsgdkServiceMsgdk::RunAction( const deServiceObject &action )
{
	const decString &function = action.GetChildAt("function")->GetString();
	
	if(function == "getUserFeatures")
	{
		return GetUserFeatures();
	}
	else
	{
		DETHROW_INFO(deeInvalidParam, "Unknown function");
	}
}



// Request
////////////

void deMsgdkServiceMsgdk::SetUser(XUserHandle user)
{
	if(pUser)
	{
		XUserCloseHandle(pUser);
		pUser = nullptr;
		pUserLocalId = {};
	}

	pUser = user;

	if(pUser){
		AssertResult(XUserGetLocalId(pUser, &pUserLocalId));
	}
}

deMsgdkPendingRequest *deMsgdkServiceMsgdk::GetPendingRequestWithId(const decUniqueID &id) const
{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++)
	{
		deMsgdkPendingRequest * const pr = (deMsgdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->id == id)
		{
			return pr;
		}
	}
	
	return nullptr;
}

deMsgdkPendingRequest::Ref deMsgdkServiceMsgdk::RemoveFirstPendingRequestWithId(
	const decUniqueID &id)
{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++)
	{
		deMsgdkPendingRequest * const pr = (deMsgdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->id == id)
		{
			const deMsgdkPendingRequest::Ref prr(pr);
			pPendingRequests.RemoveFrom(i);
			return prr;
		}
	}
	
	return nullptr;
}

deMsgdkPendingRequest::Ref deMsgdkServiceMsgdk::RemoveFirstPendingRequestWithFunction(
	const char *function)
{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++)
	{
		deMsgdkPendingRequest * const pr = (deMsgdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->function == function)
		{
			const deMsgdkPendingRequest::Ref prr(pr);
			pPendingRequests.RemoveFrom(i);
			return prr;
		}
	}
	
	return nullptr;
}

deMsgdkPendingRequest::Ref deMsgdkServiceMsgdk::NewPendingRequest(
const decUniqueID &id, const decString &function, const deServiceObject::Ref &data)
{
	const deMsgdkPendingRequest::Ref pr(deMsgdkPendingRequest::Ref::New(
		new deMsgdkPendingRequest(data)));
	pr->id = id;
	pr->function = function;
	pr->data->SetStringChildAt("function", function);
	pPendingRequests.Add(pr);
	return pr;
}

deServiceObject::Ref deMsgdkServiceMsgdk::GetUserFeatures()
{
	if(!pAuthProviderIcon)
	{
		pAuthProviderIcon.TakeOver(pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderIcon.webp", "/"));
	}
	if(!pAuthProviderImage)
	{
		pAuthProviderImage.TakeOver(pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderImage.webp", "/"));
	}
	
	const deServiceObject::Ref so(deServiceObject::Ref::New(new deServiceObject));
	so->SetBoolChildAt("canManualLogin", false);
	so->SetBoolChildAt("canAutomaticLogin", true);
	so->SetBoolChildAt("canLogout", false);
	
	const deServiceObject::Ref soAtp(deServiceObject::Ref::New(new deServiceObject));
	soAtp->SetStringChildAt("id", "xboxLive");
	soAtp->SetResourceChildAt("icon", pAuthProviderIcon);
	soAtp->SetResourceChildAt("image", pAuthProviderImage);
	soAtp->SetStringChildAt("name", "XBox Live");
	so->SetChildAt("authTokenProvider", soAtp);
	
	return so;
}



void deMsgdkServiceMsgdk::FailRequest(const decUniqueID &id, const deException &e)
{
	const deMsgdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithId(id));
	if(pr)
	{
		FailRequest(pr, e);
	}
	else
	{
		pModule.LogException(e);
	}
}

void deMsgdkServiceMsgdk::FailRequest(const deMsgdkPendingRequest::Ref &pr, const deException &e)
{
	pModule.LogException(e);
	
	pr->data->SetStringChildAt("error", e.GetName().GetMiddle(3));
	pr->data->SetStringChildAt("message", e.GetDescription());
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
}

void deMsgdkServiceMsgdk::AssertResult(HRESULT result)
{
	if (FAILED(result))
    {
		DETHROW_INFO(deeInvalidAction, pModule.GetErrorCodeString(result));
    }
}



// Callbacks
//////////////

void deMsgdkServiceMsgdk::SetInitialized()
{
	pIsInitialized = true;
}

void deMsgdkServiceMsgdk::SetResultFields(HRESULT result, deServiceObject& so) const
{
	if(SUCCEEDED(result)){
		so.SetBoolChildAt("success", true);
		
	}else{
		so.SetBoolChildAt("success", false);
		so.SetStringChildAt("error", "MsgdkError");
		so.SetIntChildAt("errorResult", result);
		so.SetStringChildAt("message", pModule.GetErrorCodeString(result));
	}
}

void deMsgdkServiceMsgdk::SetResultFields(const deException& exception, deServiceObject& so) const
{
	so.SetBoolChildAt("success", false);
	so.SetStringChildAt("error", exception.GetName().GetMiddle(3));
	so.SetStringChildAt("message", exception.GetDescription());
}
