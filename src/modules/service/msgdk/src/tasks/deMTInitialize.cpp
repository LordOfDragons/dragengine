#include "deMTInitialize.h"
#include "../deMsgdkServiceMsgdk.h"
#include "../deMicrosoftGdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deMTInitialize
/////////////////////////

// Constructor, destructor
////////////////////////////

deMTInitialize::deMTInitialize(deMsgdkServiceMsgdk &service) :
deMsgdkAsyncTask(service.GetInvalidator()),
pService(service)
{
	pAutoLoginUser();
}

deMTInitialize::~deMTInitialize()
{
}


// Protect Functions
//////////////////////

void deMTInitialize::pAutoLoginUser()
{
	// according to documentation:
	// - use first AddDefaultUserSilently
	// - if E_GAMEUSER_NO_DEFAULT_USER use None to show login dialog
	//   - if cancelled this causes E_ABORT error
	pService.GetModule().LogInfo("deMTInitialize.pAutoLoginUser");
	pService.AssertResult(XUserAddAsync(XUserAddOptions::AddDefaultUserSilently,
		GetAsyncBlockPtr()), "deMTInitialize.pAutoLoginUser.XUserAddAsync");

	//AssertAsync(XUserAddAsync(XUserAddOptions::AllowGuests, ab.get()), ab);
	//AssertAsync(XUserAddAsync(XUserAddOptions::AddDefaultUserAllowingUI, ab.get()), ab);
	//AssertAsync(XUserAddAsync(XUserAddOptions::None, ab.get()), ab);
}

void deMTInitialize::OnFinished()
{
	XUserHandle user = nullptr;
	HRESULT result = XUserAddResult(GetAsyncBlockPtr(), &user);
	pService.GetModule().LogInfoFormat("deMTInitialize.OnFinished: res(%d)[%s]",
		result, pService.GetModule().GetErrorCodeString(result));

	const deServiceObject::Ref data(deServiceObject::Ref::New(new deServiceObject));
	data->SetStringChildAt("event", "initialized");
	pService.SetResultFields(result, data);
	
	if(SUCCEEDED(result))
	{
		try
		{
			pService.SetUser(user);
			pService.GetModule().LogInfo("deMTInitialize.OnFinished: Service initialized");
			pService.SetInitialized();
		}
		catch(const deException &e)
		{
			pService.SetResultFields(e, data);
		}
	}
	
	pService.GetModule().GetGameEngine()->GetServiceManager()->
		QueueEventReceived(pService.GetService(), data);
}
