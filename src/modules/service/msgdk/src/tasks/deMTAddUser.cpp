#include "deMTAddUser.h"
#include "../deMsgdkServiceMsgdk.h"
#include "../deMicrosoftGdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deMTAddUser
//////////////////////

// Constructor, destructor
////////////////////////////

deMTAddUser::deMTAddUser(deMsgdkServiceMsgdk &service, const decUniqueID &id, const deServiceObject &request) :
deMsgdkAsyncTask(service.GetInvalidator()),
pService(service),
pRequestId(id)
{
	// according to documentation:
	// - use first AddDefaultUserSilently
	// - if E_GAMEUSER_NO_DEFAULT_USER use None to show login dialog
	//   - if cancelled this causes E_ABORT error
	
	XUserAddOptions options = XUserAddOptions::None;
	bool defaultUser = false;
	bool allowGuests = false;
	deServiceObject::Ref so;

	so = request.GetChildAt("defaultUser");
	if(so)
	{
		defaultUser = so->GetBoolean();
	}

	so = request.GetChildAt("allowGuests");
	if(so)
	{
		allowGuests = so->GetBoolean();
	}

	if(defaultUser)
	{
		service.GetModule().LogInfo("deMTAddUser: Add default user to game session");
		options |= XUserAddOptions::AddDefaultUserSilently;
	}
	else
	{
		service.GetModule().LogInfo("deMTAddUser: Add user using system UI to game session");
		if(allowGuests)
		{
			options |= XUserAddOptions::AllowGuests;
		}
	}

	pService.NewPendingRequest(pRequestId, "userAdd");
	try
	{
		pService.AssertResult(XUserAddAsync(options, GetAsyncBlockPtr()), "deMTAddUser.XUserAddAsync");
	}
	catch(const deException &e)
	{
		pService.FailRequest(pRequestId, e);
		FreeReference();
	}
}

deMTAddUser::~deMTAddUser()
{
}


// Protect Functions
//////////////////////

void deMTAddUser::OnFinished()
{
	const deMsgdkPendingRequest::Ref pr(pService.GetPendingRequestWithId(pRequestId));
	if(!pr)
	{
		return;
	}
	
	XUserHandle user = nullptr;
	HRESULT result = XUserAddResult(GetAsyncBlockPtr(), &user);
	pService.GetModule().LogInfoFormat("deMTAddUser.OnFinished: res(%d)[%s]",
		result, pService.GetModule().GetErrorCodeString(result));

	if(FAILED(result))
	{
		pService.FailRequest(pr, result);
		return;
	}
	
	try
	{
		pService.SetUser(user);
		pService.GetModule().LogInfo("deMTAddUser.OnFinished: Service initialized");

		pService.GetModule().GetGameEngine()->GetServiceManager()->
			QueueRequestResponse(pService.GetService(), pr->id, pr->data, true);
	}
	catch(const deException &e)
	{
		pService.FailRequest(pr, e);
	}
}
