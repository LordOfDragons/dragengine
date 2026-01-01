#include "deMTGetTokenAndSignature.h"
#include "../deMsgdkServiceMsgdk.h"
#include "../deMicrosoftGdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deMTGetTokenAndSignature
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deMTGetTokenAndSignature::deMTGetTokenAndSignature(deMsgdkServiceMsgdk &service,
	const decUniqueID &id, const deServiceObject &request) :
deMsgdkAsyncTask(service.GetInvalidator()),
pService(service),
pRequestId(id)
{
	pService.GetModule().LogInfo("deMTGetTokenAndSignature: Request token");
	pService.NewPendingRequest(pRequestId, "getTokenAndSignature");
	try
	{
		if(!service.GetUser())
		{
			DETHROW_INFO(deeInvalidAction, "No user logged in");
		}

		pService.AssertResult(XUserGetTokenAndSignatureAsync(service.GetUser(),
			XUserGetTokenAndSignatureOptions::None, "POST", "https://*.modapi.io",
			0, nullptr, 0, nullptr, GetAsyncBlockPtr()),
			"deMTGetTokenAndSignature.XUserGetTokenAndSignatureAsync");
	}
	catch(const deException &e)
	{
		pService.FailRequest(pRequestId, e);
		FreeReference();
	}
}

deMTGetTokenAndSignature::~deMTGetTokenAndSignature()
{
}


// Protect Functions
//////////////////////

void deMTGetTokenAndSignature::OnFinished()
{
	const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
	if(!pr)
	{
		return;
	}
	
	size_t bufferLen;
	HRESULT result = XUserGetTokenAndSignatureResultSize(GetAsyncBlockPtr(), &bufferLen);
	if(FAILED(result))
	{
		pService.FailRequest(pr, result);
		return;
	}

	uint8_t *buffer = new uint8_t[bufferLen];
	try
	{
		XUserGetTokenAndSignatureData *tsd;
		size_t bufferUsed;
		HRESULT result = XUserGetTokenAndSignatureResult(GetAsyncBlockPtr(),
			bufferLen, buffer, &tsd, &bufferUsed);
		if(FAILED(result))
		{
			delete [] buffer;
			pService.FailRequest(pr, result);
			return;
		}
		
		pr->data->SetStringChildAt("token", tsd->token);
		pr->data->SetStringChildAt("signature", tsd->signature);
		pService.GetModule().LogInfo("deMTGetTokenAndSignature.OnFinished: Token received");

		pService.GetModule().GetGameEngine()->GetServiceManager()->
			QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
	}
	catch(const deException &e)
	{
		delete [] buffer;
		pService.FailRequest(pr, e);
		return;
	}
}
