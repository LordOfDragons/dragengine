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

#include <inttypes.h>
#include <string.h>

#include "deEosSdk.h"
#include "deEosSdkServiceEos.h"
#include "flow/deEosSdkFlowInit.h"
#include "flow/deEosSdkFlowAuthLogin.h"
#include "flow/deEosSdkFlowAuthLogout.h"
#include "flow/deEosSdkFlowGetStatsAndAchievements.h"
#include "flow/deEosSdkFlowSetStatsAndAchievements.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/utils/decBase64.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>
#include <dragengine/systems/deInputSystem.h>


// Class deEosSdkServiceEos
/////////////////////////////

const char * const deEosSdkServiceEos::serviceName = "EOSSDK";

class cCallbackInfo{
public:
	deEosSdkServiceEos *service;
	decUniqueID id;
	
	cCallbackInfo(deEosSdkServiceEos *aservice) : service(aservice){}
	cCallbackInfo(deEosSdkServiceEos *aservice, const decUniqueID &aid) : service(aservice), id(aid){}
	cCallbackInfo(const cCallbackInfo &info) : service(info.service), id(info.id) {}
	
	static cCallbackInfo Get(const void *clientData){
		const cCallbackInfo info(*((cCallbackInfo*)clientData));
		delete (cCallbackInfo*)clientData;
		return info;
	}
};

static void fEosQueryUserInfoCallback(const EOS_UserInfo_QueryUserInfoCallbackInfo *data){
	const cCallbackInfo info(cCallbackInfo::Get(data->ClientData));
	info.service->OnQueryUserInfoCallback(info.id, *data);
}

static void fEosOnDisplaySettingsUpdatedCallback(
const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo *data){
	((deEosSdkServiceEos*)(data->ClientData))->OnDisplaySettingsUpdatedCallback(*data);
}

// Constructor, destructor
////////////////////////////

deEosSdkServiceEos::deEosSdkServiceEos(deEosSdk &module,
deService *service, const deServiceObject::Ref &data) :
pModule(module),
pService(service),
pHandlePlatform(nullptr),
pHandleAuth(nullptr),
pHandleUserInfo(nullptr),
pHandleAchievements(nullptr),
pHandleStats(nullptr),
pHandleConnect(nullptr),
pHandleUI(nullptr),
pNotifyIdUi(0),
pUiExclusiveModeEnabled(false),
localUserId(nullptr),
selectedAccountId(nullptr),
productUserId(nullptr)
{
	try{
		pModule.InitSdk(data);
	
		const decString &productId = data->GetChildAt("productId")->GetString();
		const decString &sandboxId = data->GetChildAt("sandboxId")->GetString();
		const decString &deploymentId = data->GetChildAt("deploymentId")->GetString();
	
		const char *clientId = nullptr;
		const deServiceObject::Ref &soClientId = data->GetChildAt("clientId");
		if(soClientId){
			clientId = soClientId->GetString().GetString();
		}
	
		const char *clientSecret = nullptr;
		const deServiceObject::Ref &soClientSecret = data->GetChildAt("clientSecret");
		if(soClientSecret){
			clientSecret = soClientSecret->GetString().GetString();
		}
	
		const deServiceObject::Ref &soIsServer = data->GetChildAt("isServer");
		const bool isServer = soIsServer ? soIsServer->GetBoolean() : false;
	
		EOS_Platform_Options options = {};
		options.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
		options.ProductId = productId.GetString();
		options.SandboxId = sandboxId.GetString();
		options.ClientCredentials.ClientId = clientId;
		options.ClientCredentials.ClientSecret = clientSecret;
		options.bIsServer = isServer ? EOS_TRUE : EOS_FALSE;
		options.DeploymentId = deploymentId.GetString();

		#ifdef OS_W32
		options.Flags |= EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL;
		// options.Flags |= EOS_PF_DISABLE_OVERLAY;
		#endif

		pHandlePlatform = EOS_Platform_Create(&options);
		if(!pHandlePlatform){
			DETHROW_INFO(deeInvalidAction, "Failed create platform interface");
		}
		
		pAddListenUIUpdate();

		module.AddFrameUpdater(this);
		new deEosSdkFlowInit(*this);

	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deEosSdkServiceEos::~deEosSdkServiceEos(){
	pCleanUp();
}


// Management
///////////////

EOS_HAuth deEosSdkServiceEos::GetHandleAuth(){
	if(!pHandleAuth){
		pHandleAuth = EOS_Platform_GetAuthInterface(pHandlePlatform);
	}
	return pHandleAuth;
}

EOS_HUserInfo deEosSdkServiceEos::GetHandleUserInfo(){
	if(!pHandleUserInfo){
		pHandleUserInfo = EOS_Platform_GetUserInfoInterface(pHandlePlatform);
	}
	return pHandleUserInfo;
}

EOS_HAchievements deEosSdkServiceEos::GetHandleAchievements(){
	if(!pHandleAchievements){
		pHandleAchievements = EOS_Platform_GetAchievementsInterface(pHandlePlatform);
	}
	return pHandleAchievements;
}

EOS_HStats deEosSdkServiceEos::GetHandleStats(){
	if(!pHandleStats){
		pHandleStats = EOS_Platform_GetStatsInterface(pHandlePlatform);
	}
	return pHandleStats;
}

EOS_HConnect deEosSdkServiceEos::GetHandleConnect(){
	if(!pHandleConnect){
		pHandleConnect = EOS_Platform_GetConnectInterface(pHandlePlatform);
	}
	return pHandleConnect;
}

EOS_HUI deEosSdkServiceEos::GetHandleUI()
{
	if(!pHandleUI){
		pHandleUI = EOS_Platform_GetUIInterface(pHandlePlatform);
	}
	return pHandleUI;
}

void deEosSdkServiceEos::StartRequest(const decUniqueID &id, const deServiceObject& request){
	const decString &function = request.GetChildAt("function")->GetString();
	
	if(function == "authLogin"){
		new deEosSdkFlowAuthLogin(*this, id, request);
		
	}else if(function == "authLogout"){
		new deEosSdkFlowAuthLogout(*this, id);
		
	}else if(function == "queryUserInfo"){
		QueryUserInfo(id, request);
		
	}else if(function == "getStatsAndAchievements"){
		new deEosSdkFlowGetStatsAndAchievements(*this, id, request);
		
	}else if(function == "setStatsAndAchievements"){
		new deEosSdkFlowSetStatsAndAchievements(*this, id, request);
		
	}else{
		DETHROW_INFO(deeInvalidParam, "Unknown function");
	}
}

void deEosSdkServiceEos::CancelRequest(const decUniqueID &id){
	deEosSdkPendingRequest * const pr = GetPendingRequestWithId(id);
	if(!pr){
		return;
	}
	
	pPendingRequests.RemoveFrom(pPendingRequests.IndexOf(pr));
	
	const deServiceObject::Ref so(deServiceObject::Ref::NewWith());
	so->SetStringChildAt("error", "Cancelled");
	so->SetStringChildAt("message", "Request cancelled");
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, id, so);
}

deServiceObject::Ref deEosSdkServiceEos::RunAction(const deServiceObject &action){
	const decString &function = action.GetChildAt("function")->GetString();
	
	if(function == "copyIdToken"){
		return CopyIdToken(action);
		
	}else if(function == "isUserLoggedIn"){
		return IsUserLoggedIn(action);
		
	}else if(function == "getUserFeatures"){
		return GetUserFeatures();
		
	}else{
		DETHROW_INFO(deeInvalidParam, "Unknown function");
	}
}

// Request
////////////

deEosSdkPendingRequest *deEosSdkServiceEos::GetPendingRequestWithId(const decUniqueID &id) const{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deEosSdkPendingRequest * const pr = (deEosSdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->id == id){
			return pr;
		}
	}
	
	return nullptr;
}

deEosSdkPendingRequest::Ref deEosSdkServiceEos::RemoveFirstPendingRequestWithId(
const decUniqueID &id){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deEosSdkPendingRequest * const pr = (deEosSdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->id == id){
			const deEosSdkPendingRequest::Ref prr(pr);
			pPendingRequests.RemoveFrom(i);
			return prr;
		}
	}
	
	return nullptr;
}

deEosSdkPendingRequest::Ref deEosSdkServiceEos::RemoveFirstPendingRequestWithFunction(
const char *function){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deEosSdkPendingRequest * const pr = (deEosSdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->function == function){
			const deEosSdkPendingRequest::Ref prr(pr);
			pPendingRequests.RemoveFrom(i);
			return prr;
		}
	}
	
	return nullptr;
}

deEosSdkPendingRequest::Ref deEosSdkServiceEos::NewPendingRequest(
const decUniqueID &id, const decString &function, const deServiceObject::Ref &data){
	const deEosSdkPendingRequest::Ref pr(deEosSdkPendingRequest::Ref::NewWith(data));
	pr->id = id;
	pr->function = function;
	pr->data->SetStringChildAt("function", function);
	pPendingRequests.Add(pr);
	return pr;
}


void deEosSdkServiceEos::QueryUserInfo(const decUniqueID &id, const deServiceObject &request){
	EOS_UserInfo_QueryUserInfoOptions options = {};
	options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
	options.LocalUserId = localUserId;
	options.TargetUserId = localUserId;
	
	pModule.LogInfo("deEosSdkServiceEos: Get user information");
	NewPendingRequest(id, "queryUserInfo");
	try{
		if(!localUserId){
			DETHROW_INFO(deeInvalidAction, "No user logged in");
		}
		
		EOS_UserInfo_QueryUserInfo(GetHandleUserInfo(), &options,
			new cCallbackInfo(this, id), fEosQueryUserInfoCallback);
		
	}catch(const deException &e){
		FailRequest(id, e);
	}
}

deServiceObject::Ref deEosSdkServiceEos::CopyIdToken(const deServiceObject &action){
	if(!selectedAccountId){
		DETHROW_INFO(deeInvalidAction, "No user logged in");
	}
	
	EOS_Auth_CopyIdTokenOptions options = {};
	options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
	options.AccountId = selectedAccountId;
	
	EOS_Auth_IdToken *token = nullptr;
	deServiceObject::Ref result;
	
	try{
		const EOS_EResult res = EOS_Auth_CopyIdToken(GetHandleAuth(), &options, &token);
		if(res != EOS_EResult::EOS_Success){
			DETHROW_INFO(deeInvalidAction, EOS_EResult_ToString(res));
		}
		
		result = deServiceObject::NewString(token->JsonWebToken);
		
		EOS_Auth_IdToken_Release(token);
		
	}catch(const deException &){
		if(token){
			EOS_Auth_IdToken_Release(token);
		}
		throw;
	}
	
	return result;
}

deServiceObject::Ref deEosSdkServiceEos::IsUserLoggedIn(const deServiceObject &action){
	return deServiceObject::NewBool(localUserId != nullptr);
}

deServiceObject::Ref deEosSdkServiceEos::GetUserFeatures(){
	if(!pAuthProviderIcon){
		pAuthProviderIcon.TakeOver(pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderIcon.webp", "/"));
	}
	if(!pAuthProviderImage){
		pAuthProviderImage.TakeOver(pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderImage.webp", "/"));
	}
	
	const deServiceObject::Ref so(deServiceObject::Ref::NewWith());
	so->SetBoolChildAt("canManualLogin", true);
	so->SetBoolChildAt("canAutomaticLogin", true);
	so->SetBoolChildAt("canLogout", true);
	
	const deServiceObject::Ref soAtp(deServiceObject::Ref::NewWith());
	soAtp->SetStringChildAt("id", "epic");
	soAtp->SetResourceChildAt("icon", pAuthProviderIcon);
	soAtp->SetResourceChildAt("image", pAuthProviderImage);
	soAtp->SetStringChildAt("name", "EPIC Games");
	so->SetChildAt("authTokenProvider", soAtp);
	
	return so;
}

void deEosSdkServiceEos::FailRequest(const decUniqueID &id, const deException &e){
	const deEosSdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithId(id));
	if(pr){
		FailRequest(pr, e);
		
	}else{
		pModule.LogException(e);
	}
}

void deEosSdkServiceEos::FailRequest(const decUniqueID &id, EOS_EResult res){
	const deEosSdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithId(id));
	if(pr){
		FailRequest(pr, res);
		
	}else{
		pModule.LogErrorFormat("deEosSdkServiceEos: %s", EOS_EResult_ToString(res));
	}
}

void deEosSdkServiceEos::FailRequest(const deEosSdkPendingRequest::Ref &pr, const deException &e){
	pModule.LogException(e);
	
	pr->data->SetStringChildAt("error", e.GetName().GetMiddle(3));
	pr->data->SetStringChildAt("message", e.GetDescription());
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
}

void deEosSdkServiceEos::FailRequest(const deEosSdkPendingRequest::Ref &request, EOS_EResult res){
	pModule.LogErrorFormat("deEosSdkServiceEos: %s", EOS_EResult_ToString(res));
	
	request->data->SetIntChildAt("code", (int)res);
	request->data->SetStringChildAt("error", EOS_EResult_ToString(res));
	request->data->SetStringChildAt("message", EOS_EResult_ToString(res));
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		pService, request->id, request->data);
}

// EOS Callbacks
//////////////////

void deEosSdkServiceEos::OnQueryUserInfoCallback(const decUniqueID &id,
const EOS_UserInfo_QueryUserInfoCallbackInfo &data){
	pModule.LogInfoFormat("deEosSdkServiceEos.OnQueryUserInfoCallback: res=%d", (int)data.ResultCode);
	if(data.ResultCode != EOS_EResult::EOS_Success){
		FailRequest(id, data.ResultCode);
		return;
	}
	
	const deEosSdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithId(id));
	if(!pr){
		return;
	}
	
	EOS_UserInfo *info = nullptr;
	
	try{
		EOS_UserInfo_CopyUserInfoOptions options = {};
		options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
		options.LocalUserId = data.LocalUserId;
		options.TargetUserId = data.TargetUserId;
		
		EOS_UserInfo_CopyUserInfo(GetHandleUserInfo(), &options, &info);
		const EOS_UserInfo &ri = *info;
		
		char userId[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
		int32_t userIdLen = sizeof(userId);
		if(EOS_EpicAccountId_ToString(ri.UserId, userId, &userIdLen) == EOS_EResult::EOS_Success){
			pr->data->SetStringChildAt("userId", userId);
			
			decString profileUrl;
			profileUrl.Format("https://store.epicgames.com/en-US/u/%s", userId);
			pr->data->SetStringChildAt("profileUrl", profileUrl);
		}
		
		if(ri.DisplayName){
			pr->data->SetStringChildAt("displayName", ri.DisplayName);
		}
		if(ri.Nickname){
			pr->data->SetStringChildAt("nickname", ri.Nickname);
		}
		if(ri.Country){
			pr->data->SetStringChildAt("country", ri.Country);
		}
		if(ri.PreferredLanguage){
			pr->data->SetStringChildAt("preferredLanguage", ri.PreferredLanguage);
		}
		
		EOS_UserInfo_Release(info);
		info = nullptr;
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true);
		
	}catch(const deException &e){
		if(info){
			EOS_UserInfo_Release(info);
		}
		
		FailRequest(pr, e);
		return;
	}
}

void deEosSdkServiceEos::OnDisplaySettingsUpdatedCallback(
const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo &data){
	if(data.bIsExclusiveInput){
		if(!pUiExclusiveModeEnabled){
			pModule.LogInfo("deEosSdkServiceEos.OnDisplaySettingsUpdatedCallback: Start drop input");
			pModule.GetGameEngine()->GetInputSystem()->StartDropInput();
			pUiExclusiveModeEnabled = true;
		}

	}else{
		if(pUiExclusiveModeEnabled){
			pModule.LogInfo("deEosSdkServiceEos.OnDisplaySettingsUpdatedCallback: Stop drop input");
			pModule.GetGameEngine()->GetInputSystem()->StopDropInput();
			pUiExclusiveModeEnabled = false;
		}
	}
}



// deEosSdk::cFrameUpdater
////////////////////////////

void deEosSdkServiceEos::FrameUpdate(float elapsed){
	EOS_Platform_Tick(pHandlePlatform);
}



// Private Functions
//////////////////////

void deEosSdkServiceEos::pCleanUp(){
	while(pPendingRequests.GetCount() > 0){
		CancelRequest(((deEosSdkPendingRequest*)pPendingRequests.GetAt(0))->id);
	}
	if(pHandlePlatform){
		EOS_Platform_Tick(pHandlePlatform);
	}

	pModule.RemoveFrameUpdater(this);
	
	pRemoveListenUIUpdate();

	if(pHandlePlatform){
		EOS_Platform_Release(pHandlePlatform);
	}

	pHandlePlatform = nullptr;
	pHandleAuth = nullptr;
	pHandleUserInfo = nullptr;
	pHandleAchievements = nullptr;
	pHandleStats = nullptr;
	pHandleConnect = nullptr;
	pHandleUI = nullptr;
}

void deEosSdkServiceEos::pAddListenUIUpdate(){
	EOS_UI_AddNotifyDisplaySettingsUpdatedOptions options{};
	options.ApiVersion = EOS_UI_ADDNOTIFYDISPLAYSETTINGSUPDATED_API_LATEST;
	
	pNotifyIdUi = EOS_UI_AddNotifyDisplaySettingsUpdated(GetHandleUI(),
		&options, this, fEosOnDisplaySettingsUpdatedCallback);
	if(pNotifyIdUi){
		pModule.LogInfo("deEosSdkServiceEos.pAddListenUIUpdate: Registered");
		
	}else{
		pModule.LogInfo("deEosSdkServiceEos.pAddListenUIUpdate: Register failed");
	}
}

void deEosSdkServiceEos::pRemoveListenUIUpdate()
{
	if(!pNotifyIdUi){
		return;
	}

	EOS_UI_RemoveNotifyDisplaySettingsUpdated(GetHandleUI(), pNotifyIdUi);
	pNotifyIdUi = 0;
	pModule.LogInfo("deEosSdkServiceEos.pRemoveListenUIUpdate: Unregistered");
}
