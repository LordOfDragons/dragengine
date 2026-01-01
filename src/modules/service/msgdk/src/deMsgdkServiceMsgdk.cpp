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
#include "deMsgdkResourceUrl.h"
#include "convert/deMCCommon.h"
#include "tasks/deMTLoadUserResource.h"
#include "tasks/deMTAddUser.h"
#include "tasks/deMTGetTokenAndSignature.h"
#include "tasks/deMTGetStatsAndAchievements.h"
#include "tasks/deMTSetStatsAndAchievements.h"
#include "tasks/deMTResetAllStats.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/utils/decBase64.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>

#include <shlwapi.h>



// Class deMsgdkServiceMsgdk
//////////////////////////////

const char * const deMsgdkServiceMsgdk::serviceName = "MicrosoftGdk";
deMsgdkServiceMsgdk *deMsgdkServiceMsgdk::pGlobalService = nullptr;


// Constructor, destructor
////////////////////////////

deMsgdkServiceMsgdk::deMsgdkServiceMsgdk(deMicrosoftGdk &module,
	deService *service, const deServiceObject::Ref &data) :
pModule(module),
pService(service),
pIsInitialized(false),
pInvalidator(deMsgdkAsyncTask::Invalidator::Ref::New()),
pUser(nullptr),
pUserId(0),
pUserLocalId({}),
pXblContext(nullptr),
pAchievementsSynced(false)
{
	if(pGlobalService)
	{
		DETHROW_INFO(deeInvalidAction, "Duplicate service");
	}

	pModule.InitSdk(data);

	pIsInitialized = true;
	pGlobalService = this;
}

deMsgdkServiceMsgdk::~deMsgdkServiceMsgdk()
{
	while(pPendingRequests.GetCount() > 0)
	{
		CancelRequest(((deMsgdkPendingRequest*)pPendingRequests.GetAt(0))->id);
	}

	SetUser(nullptr);
	pFrameUpdateTasks.RemoveAll();

	pInvalidator->Invalidate();
	pGlobalService = nullptr;
}


// Management
///////////////

void deMsgdkServiceMsgdk::StartRequest(const decUniqueID& id, const deServiceObject& request)
{
	const decString &function = request.GetChildAt("function")->GetString();
	
	if(function == "loadUserResource")
	{
		new deMTLoadUserResource(*this, id, request);
	}
	else if(function == "userAdd")
	{
		new deMTAddUser(*this, id, request);
	}
	else if(function == "getTokenAndSignature")
	{
		new deMTGetTokenAndSignature(*this, id, request);
	}
	else if(function == "getStatsAndAchievements")
	{
		new deMTGetStatsAndAchievements(*this, id, request);
	}
	else if(function == "setStatsAndAchievements")
	{
		new deMTSetStatsAndAchievements(*this, id, request);
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
	
	const deServiceObject::Ref so(deServiceObject::Ref::New());
	so->SetStringChildAt("error", "Cancelled");
	so->SetStringChildAt("message", "Request cancelled");
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, id, so);
}

deServiceObject::Ref deMsgdkServiceMsgdk::RunAction(const deServiceObject &action)
{
	const decString &function = action.GetChildAt("function")->GetString();
	
	if(function == "getUserFeatures")
	{
		return GetUserFeatures();
	}
	else if(function == "getUserInfo")
	{
		return GetUserInfo();
	}
	else if(function == "isUserLoggedIn")
	{
		return IsUserLoggedIn();
	}
	else if(function == "userRemove")
	{
		SetUser(nullptr);
		return {};
	}
	else if(function == "resetAllStats")
	{
		//new deMTResetAllStats(*this, action);
		//return nullptr;
		DETHROW_INFO(deeInvalidAction, "Not supported");
	}
	else
	{
		DETHROW_INFO(deeInvalidParam, "Unknown function");
	}
}

void deMsgdkServiceMsgdk::OnFrameUpdate(float elapsed)
{
	UpdateAchievementManager();
	FrameUpdateTasks(elapsed);
}



// Request
////////////

void deMsgdkServiceMsgdk::SetUser(XUserHandle user)
{
	if(pUser)
	{
		if(pXblContext)
		{
			XblContextCloseHandle(pXblContext);
			pXblContext = nullptr;
		}
		
		XblAchievementsManagerRemoveLocalUser(pUser);
		pAchievementsSynced = false;

		XUserCloseHandle(pUser);
		pUser = nullptr;
		pUserId = 0;
		pUserLocalId = {};
	}

	if(user){
		uint64_t userId;
		AssertResult(XUserGetId(user, &userId), "deMsgdkServiceMsgdk.SetUser.XUserGetId");

		XUserLocalId userLocalId;
		AssertResult(XUserGetLocalId(user, &userLocalId), "deMsgdkServiceMsgdk.SetUser.XUserGetLocalId");

		pModule.LogInfo("deMsgdkServiceMsgdk.SetUser: Add user to achievements manager");
		AssertResult(XblAchievementsManagerAddLocalUser(user, nullptr),
			"deMsgdkServiceMsgdk.SetUser.XblAchievementsManagerAddLocalUser");
		
		pModule.LogInfo("deMsgdkServiceMsgdk.SetUser: Create XBL context");
		AssertResult(XblContextCreateHandle(user, &pXblContext),
			"deMsgdkServiceMsgdk.SetUser.XblContextCreateHandle");

		pUserId = userId;
		pUserLocalId = userLocalId;
	}
	pUser = user;
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
	
	return {};
}

deMsgdkPendingRequest::Ref deMsgdkServiceMsgdk::RemoveFirstPendingRequestWithFunction(
	const char* function)
{
	const int count = pPendingRequests.GetCount();
	int i;

	for (i = 0; i < count; i++)
	{
		deMsgdkPendingRequest* const pr = (deMsgdkPendingRequest*)pPendingRequests.GetAt(i);
		if (pr->function == function)
		{
			const deMsgdkPendingRequest::Ref prr(pr);
			pPendingRequests.RemoveFrom(i);
			return prr;
		}
	}

	return {};
}

deMsgdkPendingRequest::Ref deMsgdkServiceMsgdk::NewPendingRequest(
const decUniqueID &id, const decString &function, const deServiceObject::Ref &data)
{
	const deMsgdkPendingRequest::Ref pr(deMsgdkPendingRequest::Ref::New(data));
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
		pAuthProviderIcon = pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderIcon.webp", "/");
	}
	if(!pAuthProviderImage)
	{
		pAuthProviderImage = pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderImage.webp", "/");
	}
	
	const deServiceObject::Ref so(deServiceObject::Ref::New());
	so->SetBoolChildAt("canManualLogin", false);
	so->SetBoolChildAt("canAutomaticLogin", true);
	so->SetBoolChildAt("canLogout", false);
	
	const deServiceObject::Ref soAtp(deServiceObject::Ref::New());
	soAtp->SetStringChildAt("id", "xboxLive");
	soAtp->SetResourceChildAt("icon", pAuthProviderIcon);
	soAtp->SetResourceChildAt("image", pAuthProviderImage);
	soAtp->SetStringChildAt("name", "XBox Live");
	so->SetChildAt("authTokenProvider", soAtp);
	
	return so;
}

deServiceObject::Ref deMsgdkServiceMsgdk::GetUserInfo()
{
	if(!pUser)
	{
		DETHROW_INFO(deeInvalidAction, "No user logged in");
	}

	const deServiceObject::Ref so(deServiceObject::Ref::New());
	char gamertag[101] = {};
	size_t gamertagLen;

	uint64_t id;
	AssertResult(XUserGetId(pUser, &id), "deMsgdkServiceMsgdk.GetUserInfo.XUserGetId");
	so->SetStringChildAt("id", deMCCommon::UInt64ToString(id));

	AssertResult(XUserGetGamertag(pUser, XUserGamertagComponent::Modern, sizeof(gamertag),
		gamertag, &gamertagLen), "deMsgdkServiceMsgdk.GetUserInfo.XUserGetGamertag1");
	so->SetStringChildAt("gamertagModern", gamertag);

	AssertResult(XUserGetGamertag(pUser, XUserGamertagComponent::UniqueModern, sizeof(gamertag),
		gamertag, &gamertagLen), "deMsgdkServiceMsgdk.GetUserInfo.XUserGetGamertag2");
	so->SetStringChildAt("gamertagUniqueModern", gamertag);

	decString profileUrl;
	profileUrl.Format("https://www.xbox.com/play/user/%s", UriEncode(gamertag).GetString());
	so->SetStringChildAt("profileUrl", profileUrl);

	so->SetStringChildAt("gamerPicture", deMsgdkResourceUrl::FormatUrl(
		"user", id, "gamerPicture", "extraLarge"));
	so->SetStringChildAt("gamerPictureSmall", deMsgdkResourceUrl::FormatUrl(
		"user", id, "gamerPicture", "small"));
	so->SetStringChildAt("gamerPictureMedium", deMsgdkResourceUrl::FormatUrl(
		"user", id, "gamerPicture", "medium"));
	so->SetStringChildAt("gamerPictureLarge", deMsgdkResourceUrl::FormatUrl(
		"user", id, "gamerPicture", "large"));
	return so;
}

deServiceObject::Ref deMsgdkServiceMsgdk::IsUserLoggedIn()
{
	return deServiceObject::NewBool(pUser != nullptr);
}



void deMsgdkServiceMsgdk::FailRequest(const decUniqueID& id, HRESULT result)
{
	const deMsgdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithId(id));
	if(pr)
	{
		FailRequest(pr, result);
	}
	else
	{
		pModule.LogErrorFormat("Failed: %s", pModule.GetErrorCodeString(result).GetString());
	}
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

void deMsgdkServiceMsgdk::FailRequest(const deMsgdkPendingRequest::Ref &pr, HRESULT result)
{
	const decString message(pModule.GetErrorCodeString(result));
	pModule.LogErrorFormat("Failed: %s", message.GetString());
	
	pr->data->SetStringChildAt("error", "MsgdkError");
	pr->data->SetIntChildAt("errorResult", result);
	pr->data->SetStringChildAt("message", message);
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
}

void deMsgdkServiceMsgdk::FailRequest(const deMsgdkPendingRequest::Ref &pr, const deException &e)
{
	pModule.LogException(e);
	
	pr->data->SetStringChildAt("error", e.GetName().GetMiddle(3));
	pr->data->SetStringChildAt("message", e.GetDescription());
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
}

void deMsgdkServiceMsgdk::AssertResult(HRESULT result, const char *source) const
{
	if (FAILED(result))
    {
		const decString message(pModule.GetErrorCodeString(result));
		pModule.LogErrorFormat("%s: %s", source, message.GetString());
		DETHROW_INFO(deeInvalidAction, message);
    }
}

void deMsgdkServiceMsgdk::UpdateAchievementManager()
{
	const XblAchievementsManagerEvent *events = nullptr;
	size_t eventCount = 0;
	HRESULT result = XblAchievementsManagerDoWork(&events, &eventCount);
	if(FAILED(result))
	{
		const decString message(pModule.GetErrorCodeString(result));
		pModule.LogErrorFormat(
			"deMsgdkServiceMsgdk.UpdateAchievementManager.XblAchievementsManagerDoWork: %s",
			message.GetString());
		return;
	}

	if(!events || eventCount == 0 || !pXblContext)
	{
		return;
	}

	uint32_t i;
	for(i=0; i<eventCount; i++)
	{
		switch(events[i].eventType)
		{
		case XblAchievementsManagerEventType::LocalUserInitialStateSynced:
			pAchievementsSynced = true;
			break;

		case XblAchievementsManagerEventType::AchievementProgressUpdated:
			//events[i].progressInfo.achievementId;
			break;

		case XblAchievementsManagerEventType::AchievementUnlocked:
			//events[i].progressInfo.achievementId;
			break;
		}
	}
}

void deMsgdkServiceMsgdk::AddFrameUpdateTask(deMsgdkAsyncTask* task)
{
	DEASSERT_NOTNULL(task)
	DEASSERT_FALSE(pFrameUpdateTasks.Has(task))
	pFrameUpdateTasks.Add(task);
}

void deMsgdkServiceMsgdk::RemoveFrameUpdateTask(deMsgdkAsyncTask* task)
{
	DEASSERT_NOTNULL(task)

	const int index = pFrameUpdateTasks.IndexOf(task);
	if(index != -1)
	{
		pFrameUpdateTasks.RemoveFrom(index);
	}
}

void deMsgdkServiceMsgdk::FrameUpdateTasks(float elapsed)
{
	const int count = pFrameUpdateTasks.GetCount();
	if(count == 0)
	{
		return;
	}

	const decObjectList tasks(pFrameUpdateTasks);
	int i;
	for(i=0; i<count; i++)
	{
		((deMsgdkAsyncTask*)tasks.GetAt(i))->OnFrameUpdate(elapsed);
	}
}

decString deMsgdkServiceMsgdk::UriEncode(const char *url) const
{
	decUnicodeString wurl(decUnicodeString::NewFromUTF8(url));
	wchar_t * const wwurl = new wchar_t[wurl.GetLength()];
	deOSWindows::UnicodeToWide(wurl, wwurl, wurl.GetLength() + 1);

	const DWORD flags = 0;
	DWORD lenEscaped = 1;
	wchar_t dummy = 0;
	UrlEscape(wwurl, &dummy, &lenEscaped, flags);

	wchar_t * const escaped = new wchar_t[lenEscaped + 1];
	memset(escaped, 0, sizeof(wchar_t) * (lenEscaped + 1));
	AssertResult(UrlEscape(wwurl, escaped, &lenEscaped, flags), "deMsgdkServiceMsgdk.UriEncode.UrlEscape");
	
	return deOSWindows::WideToUtf8(escaped);
}



// Callbacks
//////////////

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
