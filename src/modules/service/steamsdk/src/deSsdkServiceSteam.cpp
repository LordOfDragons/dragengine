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

#include "deSteamSdk.h"
#include "deSsdkServiceSteam.h"
#include "deSsdkResourceUrl.h"
#include "convert/deSCCommon.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/utils/decBase64.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>


// Class deSsdkServiceSteam
/////////////////////////////

const char * const deSsdkServiceSteam::serviceName = "SteamSDK";

// Constructor, destructor
////////////////////////////

deSsdkServiceSteam::deSsdkServiceSteam(deSteamSdk &module,
deService *service, const deServiceObject::Ref &data) :
pModule(module),
pService(service)
{
	pModule.InitSdk(data);
	
	// some information like avatars can be missing. update them in the hope
	// the information is ready soon enough.
	SteamFriends()->RequestUserInformation(SteamUser()->GetSteamID(), false);
}

deSsdkServiceSteam::~deSsdkServiceSteam(){
	while(pPendingRequests.GetCount() > 0){
		CancelRequest(((deSsdkPendingRequest*)pPendingRequests.GetAt(0))->id);
	}
}


// Management
///////////////

void deSsdkServiceSteam::StartRequest(const decUniqueID &id, const deServiceObject& request){
	const decString &function = request.GetChildAt("function")->GetString();
	
	if(function == "requestCurrentStats"){
		RequestCurrentStats(id);
		
	}else if(function == "getStats"){
		GetStats(id, request);
		
	}else if(function == "resetAllStats"){
		ResetAllStats(id, request);
		
	}else if(function == "setStats"){
		SetStats(id, request);
		
	}else if(function == "requestEncryptedAppTicket"){
		RequestEncryptedAppTicket(id, request);
		
	}else if(function == "loadUserResource"){
		LoadUserResource(id, request);
		
	}else{
		DETHROW_INFO(deeInvalidParam, "Unknown function");
	}
}

void deSsdkServiceSteam::CancelRequest(const decUniqueID &id){
	deSsdkPendingRequest * const pr = GetPendingRequestWithId(id);
	if(!pr){
		return;
	}
	
	pPendingRequests.RemoveFrom(pPendingRequests.IndexOf(pr));
	
	const deServiceObject::Ref so(deServiceObject::Ref::New());
	so->SetStringChildAt("error", "Cancelled");
	so->SetStringChildAt("message", "Request cancelled");
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, id, so);
}

deServiceObject::Ref deSsdkServiceSteam::RunAction(const deServiceObject &action){
	const decString &function = action.GetChildAt("function")->GetString();
	
	if(function == "getUserFeatures"){
		return GetUserFeatures();
		
	}else if(function == "getUserInfo"){
		return GetUserInfo();
		
	}else{
		DETHROW_INFO(deeInvalidParam, "Unknown function");
	}
}

// Request
////////////

deSsdkPendingRequest *deSsdkServiceSteam::GetPendingRequestWithId(const decUniqueID &id) const{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deSsdkPendingRequest * const pr = (deSsdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->id == id){
			return pr;
		}
	}
	
	return nullptr;
}

deSsdkPendingRequest::Ref deSsdkServiceSteam::RemoveFirstPendingRequestWithId(
const decUniqueID &id){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deSsdkPendingRequest * const pr = (deSsdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->id == id){
			const deSsdkPendingRequest::Ref prr(pr);
			pPendingRequests.RemoveFrom(i);
			return prr;
		}
	}
	
	return {};
}

deSsdkPendingRequest::Ref deSsdkServiceSteam::RemoveFirstPendingRequestWithFunction(
const char *function){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deSsdkPendingRequest * const pr = (deSsdkPendingRequest*)pPendingRequests.GetAt(i);
		if(pr->function == function){
			const deSsdkPendingRequest::Ref prr(pr);
			pPendingRequests.RemoveFrom(i);
			return prr;
		}
	}
	
	return {};
}

deSsdkPendingRequest::Ref deSsdkServiceSteam::NewPendingRequest(
const decUniqueID &id, const decString &function, const deServiceObject::Ref &data){
	const deSsdkPendingRequest::Ref pr(deSsdkPendingRequest::Ref::New(data));
	pr->id = id;
	pr->function = function;
	pr->data->SetStringChildAt("function", function);
	pPendingRequests.Add(pr);
	return pr;
}

void deSsdkServiceSteam::RequestCurrentStats(const decUniqueID &id){
	NewPendingRequest(id, "requestCurrentStats");
	
	try{
		if(!SteamUserStats()->RequestCurrentStats()){
			DETHROW_INFO(deeInvalidAction, "SteamUserStats.RequestCurrentStats Failed");
		}
		
	}catch(const deException &e){
		FailRequest(id, e);
	}
}

void deSsdkServiceSteam::GetStats(const decUniqueID &id, const deServiceObject& request){
	const deServiceObject::Ref response(deServiceObject::Ref::New());
	deServiceObject::Ref soIn, soResp;
	int i, count;
	
	response->SetStringChildAt("function", "getStats");
	
	// stats
	soResp = deServiceObject::Ref::New();
	response->SetChildAt("stats", soResp);
	
	soIn = request.GetChildAt("stats");
	if(soIn){
		count = soIn->GetChildCount();
		for(i=0; i<count; i++){
			const decString &apiName = soIn->GetChildAt(i)->GetString();
			int32 valueInt;
			float valueFloat;
			if(SteamUserStats()->GetStat(apiName, &valueInt)){
				soResp->SetIntChildAt(apiName, valueInt);
				
			}else if(SteamUserStats()->GetStat(apiName, &valueFloat)){
				soResp->SetFloatChildAt(apiName, valueFloat);
			}
		}
	}
	
	// achievements
	soResp = deServiceObject::Ref::New();
	response->SetChildAt("achievements", soResp);
	
	soIn = request.GetChildAt("achievements");
	if(soIn){
		count = soIn->GetChildCount();
		for(i=0; i<count; i++){
			const decString &apiName = soIn->GetChildAt(i)->GetString();
			bool achieved;
			if(SteamUserStats()->GetAchievement(apiName, &achieved)){
				soResp->SetBoolChildAt(apiName, achieved);
			}
		}
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
		pService, id, response, true);
}

void deSsdkServiceSteam::ResetAllStats(const decUniqueID &id, const deServiceObject &request){
	const deServiceObject::Ref soResetAchievements(request.GetChildAt("resetAchievements"));
	const bool resetAchievements = soResetAchievements && soResetAchievements->GetBoolean();
	
	DEASSERT_TRUE(SteamUserStats()->ResetAllStats(resetAchievements))
	
	const deServiceObject::Ref response(deServiceObject::Ref::New());
	response->SetStringChildAt("function", "resetAllStats");
	
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
		pService, id, response, true);
}

void deSsdkServiceSteam::RequestEncryptedAppTicket(const decUniqueID &id, const deServiceObject &request){
	if(pEncAppTicket.IsEmpty()){
		NewPendingRequest(id, "requestEncryptedAppTicket");
		pCROnEncryptedAppTicketResponse.Run(this, SteamUser()->RequestEncryptedAppTicket(nullptr, 0));
		
	}else{
		const deServiceObject::Ref response(deServiceObject::Ref::New());
		response->SetStringChildAt("function", "requestEncryptedAppTicket");
		response->SetBoolChildAt("success", true);
		response->SetStringChildAt("ticket", pEncAppTicket);
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(pService, id, response, true);
	}
}

void deSsdkServiceSteam::LoadUserResource(const decUniqueID &id, const deServiceObject &request){
	const deSsdkResourceUrl url(request.GetChildAt("url")->GetString());
	
	const int handle = pLoadResource(url);
	pModule.LogInfoFormat("deSsdkServiceSteam.LoadUserResource: url=%s handle=%d",
		url.url.GetString(), handle);
	
	const deServiceObject::Ref data(deServiceObject::Ref::New());
	data->SetStringChildAt("url", url.url);
	NewPendingRequest(id, "loadUserResource", data);
	
	if(handle == -1){
		// image will be loaded asynchronously. wait for OnAvatarImageLoaded
		// then call GetLargeFriendAvatar again
		
	}else{
		// image ready to process or user has no avatar. the callback handles this
		OnAvatarImageLoaded(nullptr);
	}
}

deServiceObject::Ref deSsdkServiceSteam::GetUserFeatures(){
	if(!pAuthProviderIcon){
		pAuthProviderIcon = pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderIcon.webp", "/");
	}
	if(!pAuthProviderImage){
		pAuthProviderImage = pModule.GetGameEngine()->GetImageManager()->LoadImage(
			&pModule.GetVFS(), "/share/image/authProviderImage.webp", "/");
	}
	
	const deServiceObject::Ref so(deServiceObject::Ref::New());
	so->SetBoolChildAt("canManualLogin", false);
	so->SetBoolChildAt("canAutomaticLogin", true);
	so->SetBoolChildAt("canLogout", false);
	
	const deServiceObject::Ref soAtp(deServiceObject::Ref::New());
	soAtp->SetStringChildAt("id", "steam");
	soAtp->SetResourceChildAt("icon", pAuthProviderIcon);
	soAtp->SetResourceChildAt("image", pAuthProviderImage);
	soAtp->SetStringChildAt("name", "Steam");
	so->SetChildAt("authTokenProvider", soAtp);
	
	return so;
}

deServiceObject::Ref deSsdkServiceSteam::GetUserInfo(){
	const deServiceObject::Ref so(deServiceObject::Ref::New());
	const decString id(deSCCommon::SteamIDToString(SteamUser()->GetSteamID()));
	
	so->SetStringChildAt("id", id);
	so->SetStringChildAt("displayName", SteamFriends()->GetPersonaName());
	so->SetStringChildAt("profileUrl", decString("https://steamcommunity.com/profiles/") + id);
	so->SetStringChildAt("avatarSmall", deSsdkResourceUrl::FormatUrl("user", id, "avatar", "small"));
	so->SetStringChildAt("avatarMedium", deSsdkResourceUrl::FormatUrl("user", id, "avatar", "medium"));
	so->SetStringChildAt("avatarLarge", deSsdkResourceUrl::FormatUrl("user", id, "avatar", "large"));
	
	return so;
}

void deSsdkServiceSteam::SetStats(const decUniqueID &id, const deServiceObject &request){
	const deServiceObject::Ref response(deServiceObject::Ref::New());
	deServiceObject::Ref soIn, soResp;
	int i, count;
	
	// stats
	soResp = deServiceObject::Ref::New();
	response->SetChildAt("stats", soResp);
	
	soIn = request.GetChildAt("stats");
	if(soIn){
		const decStringList keys(soIn->GetChildrenKeys());
		count = keys.GetCount();
		for(i=0; i<count; i++){
			const decString &apiName = keys.GetAt(i);
			const deServiceObject::Ref soValue(soIn->GetChildAt(apiName));
			
			if(soValue->IsInteger()){
				const int value = soValue->GetInteger();
				DEASSERT_TRUE(SteamUserStats()->SetStat(apiName, value))
				soResp->SetIntChildAt(apiName, value);
				
			}else if(soValue->IsFloat()){
				const float value = (float)soValue->GetFloat();
				DEASSERT_TRUE(SteamUserStats()->SetStat(apiName, value))
				soResp->SetFloatChildAt(apiName, value);
				
			}else{
				DETHROW_INFO(deeInvalidParam, apiName);
			}
		}
	}
	
	// achievements
	soResp = deServiceObject::Ref::New();
	response->SetChildAt("achievements", soResp);
	
	soIn = request.GetChildAt("achievements");
	if(soIn){
		const decStringList keys(soIn->GetChildrenKeys());
		count = keys.GetCount();
		for(i=0; i<count; i++){
			const decString &apiName = keys.GetAt(i);
			const bool achieved = soIn->GetChildAt(apiName)->GetBoolean();
			
			if(achieved){
				DEASSERT_TRUE(SteamUserStats()->SetAchievement(apiName))
				
			}else{
				DEASSERT_TRUE(SteamUserStats()->ClearAchievement(apiName))
			}
			
			soResp->SetBoolChildAt(apiName, achieved);
		}
	}
	
	// store
	NewPendingRequest(id, "setStats", response);
	try{
		if(!SteamUserStats()->StoreStats()){
			DETHROW_INFO(deeInvalidAction, "SteamUserStats.StoreStats Failed");
		}
		
	}catch(const deException &e){
		FailRequest(id, e);
	}
}



void deSsdkServiceSteam::FailRequest(const decUniqueID &id, const deException &e){
	const deSsdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithId(id));
	if(pr){
		FailRequest(pr, e);
		
	}else{
		pModule.LogException(e);
	}
}

void deSsdkServiceSteam::FailRequest(const deSsdkPendingRequest::Ref &pr, const deException &e){
	pModule.LogException(e);
	
	pr->data->SetStringChildAt("error", e.GetName().GetMiddle(3));
	pr->data->SetStringChildAt("message", e.GetDescription());
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
}



// Steam Callbacks
////////////////////

void deSsdkServiceSteam::OnUserStatsReceived(UserStatsReceived_t *response){
	const deSsdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithFunction("requestCurrentStats"));
	if(!pr){
		return;
	}
	
	try{
		pSetResultFields(response->m_eResult, pr->data);
		
		switch(response->m_eResult){
		case k_EResultOK:
		case k_EResultFail: // player has no stats yet
			pr->data->SetBoolChildAt("success", true);
			break;
			
		default:
			pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
			return;
		}
		
		decString string;
		string.Format("%" PRIu64, (uint64_t)response->m_nGameID);
		pr->data->SetStringChildAt("gameId", string);
		
		string.Format("%" PRIu32, (uint32_t)response->m_steamIDUser.GetAccountID());
		pr->data->SetStringChildAt("accountId", string);
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(pService, pr->id, pr->data, true);
		
	}catch(const deException &e){
		FailRequest(pr, e);
	}
}

void deSsdkServiceSteam::OnUserStatsStored(UserStatsStored_t *response){
	const deSsdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithFunction("setStats"));
	if(!pr){
		return;
	}
	
	try{
		pSetResultFields(response->m_eResult, pr->data);
		
		if(response->m_eResult != k_EResultOK){
			pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
			return;
		}
		
		decString string;
		string.Format("%" PRIu64, (uint64_t)response->m_nGameID);
		pr->data->SetStringChildAt("gameId", string);
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(pService, pr->id, pr->data, true);
		
	}catch(const deException &e){
		FailRequest(pr, e);
	}
}

void deSsdkServiceSteam::OnEncryptedAppTicketResponse(EncryptedAppTicketResponse_t *response, bool iofailure){
	const deSsdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithFunction("requestEncryptedAppTicket"));
	if(!pr){
		return;
	}
	
	uint8 rgubTicket[1024];
	uint32 cubTicket = 0;
	
	try{
		if(iofailure){
			response->m_eResult = k_EResultIOFailure;
		}
		
		pSetResultFields(response->m_eResult, pr->data);
		
		if(response->m_eResult != k_EResultOK){
			pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(pService, pr->id, pr->data);
			return;
		}
		
		if(!SteamUser()->GetEncryptedAppTicket(rgubTicket, sizeof(rgubTicket), &cubTicket)){
			DETHROW_INFO(deeInvalidAction, "Failed retrieving encrypted app ticket");
		}
		
		pModule.LogInfo("deSsdkServiceSteam.OnEncryptedAppTicketResponse: Ticket retrieved");
		pEncAppTicket = decBase64::Encode(rgubTicket, cubTicket);
		pr->data->SetStringChildAt("ticket", pEncAppTicket);
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(pService, pr->id, pr->data, true);
		
	}catch(const deException &e){
		FailRequest(pr, e);
	}
}

void deSsdkServiceSteam::OnAvatarImageLoaded(AvatarImageLoaded_t *response){
	const deSsdkPendingRequest::Ref pr(RemoveFirstPendingRequestWithFunction("loadUserResource"));
	if(!pr){
		return;
	}
	
	try{
		pr->data->SetBoolChildAt("success", true);
		
		const deSsdkResourceUrl url(pr->data->GetChildAt("url")->GetString());
		const int handle = pLoadResource(url);
		pModule.LogInfoFormat("deSsdkServiceSteam.OnAvatarImageLoaded: url=%s handle=%d  response=%d",
			url.url.GetString(), handle, response ? 1 : 0);
		
		if(handle == 0){
			// user has no avatar image
			
		}else if(handle == -1){
			// image will be loaded asynchronously. wait for OnAvatarImageLoaded then call
			// GetLargeFriendAvatar again. can not happen here
			DETHROW_INFO(deeInvalidAction, "Loading resource resulted in -1");
			
		}else{
			// good. we can load the image
			pCreateImage(handle, pr->data, "resource");
		}
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(pService, pr->id, pr->data, true);
		
	}catch(const deException &e){
		FailRequest(pr, e);
	}
}



// Private Functions
//////////////////////

void deSsdkServiceSteam::pSetResultFields(EResult result, deServiceObject &so) const{
	if(result == k_EResultOK){
		so.SetBoolChildAt("success", true);
		
	}else{
		so.SetBoolChildAt("success", false);
		so.SetStringChildAt("error", "SteamError");
		so.SetIntChildAt("errorResult", result);
		so.SetStringChildAt("message", pModule.GetResultMessage(result));
	}
}

void deSsdkServiceSteam::pCreateImage(int handle, deServiceObject &so, const char *key){
	uint32 width, height;
	DEASSERT_TRUE(SteamUtils()->GetImageSize(handle, &width, &height))
	
	const deImage::Ref image(pModule.GetGameEngine()->GetImageManager()->
		CreateImage(width, height, 1, 4, 8));
	DEASSERT_TRUE(SteamUtils()->GetImageRGBA(handle,
		(uint8*)image->GetData(), width * height * 4))
	
	image->NotifyImageDataChanged();
	
	so.SetResourceChildAt(key, image);
}

int deSsdkServiceSteam::pLoadResource(const deSsdkResourceUrl &url){
	if(url.type != "res"){
		DETHROW_INFO(deeInvalidParam, "url");
	}
	
	if(url.getComponentAt(0) == "user"){
		const CSteamID sid(deSCCommon::SteamID(url.getComponentAt(1)));
		if(url.getComponentAt(2) == "avatar"){
			if(url.getComponentAt(3) == "large"){
				return SteamFriends()->GetLargeFriendAvatar(sid);
				
			}else if(url.getComponentAt(3) == "medium"){
				return SteamFriends()->GetMediumFriendAvatar(sid);
				
			}else if(url.getComponentAt(3) == "small"){
				return SteamFriends()->GetSmallFriendAvatar(sid);
				
			}else{
				DETHROW_INFO(deeInvalidParam, "url");
			}
			
		}else{
			DETHROW_INFO(deeInvalidParam, "url");
		}
		
	}else{
		DETHROW_INFO(deeInvalidParam, "url");
	}
}
