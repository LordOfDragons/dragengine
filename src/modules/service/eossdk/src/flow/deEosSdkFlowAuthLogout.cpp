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

#include "deEosSdkFlowAuthLogout.h"
#include "../deEosSdkServiceEos.h"
#include "../deEosSdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deEosSdkFlowAuthLogout
/////////////////////////////////

static void fConnectLogoutCallback(const EOS_Connect_LogoutCallbackInfo *data){
	((deEosSdkFlowAuthLogout*)data->ClientData)->OnConnectLogoutCallback(*data);
}

static void fAuthLogoutCallback(const EOS_Auth_LogoutCallbackInfo *data){
	((deEosSdkFlowAuthLogout*)data->ClientData)->OnAuthLogoutCallback(*data);
}

static void fDeletePersistentAuthCallback(const EOS_Auth_DeletePersistentAuthCallbackInfo *data){
	((deEosSdkFlowAuthLogout*)data->ClientData)->OnDeletePersistentAuthCallback(*data);
}

// Constructor, destructor
////////////////////////////

deEosSdkFlowAuthLogout::deEosSdkFlowAuthLogout(
	deEosSdkServiceEos &service, const decUniqueID &id) :
deEosSdkFlow(service, id)
{
	service.NewPendingRequest(id, "authLogout");
	
	try{
		if(!service.localUserId){
			DETHROW_INFO(deeInvalidAction, "No user logged in");
		}
		
		ConnectLogout();
		
	}catch(const deException &e){
		Fail(e);
		Finish();
	}
}



// Management
///////////////

void deEosSdkFlowAuthLogout::ConnectLogout(){
	if(!pService.productUserId){
		AuthLogout();
		return;
	}
	
	EOS_Connect_LogoutOptions options{};
	options.ApiVersion = EOS_CONNECT_LOGOUT_API_LATEST;
	options.LocalUserId = pService.productUserId;
	
	GetModule().LogInfo("deEosSdkFlowAuthLogout.ConnectLogout: Connect logout user");
	EOS_Connect_Logout(pService.GetHandleConnect(), &options, this, fConnectLogoutCallback);
}

void deEosSdkFlowAuthLogout::AuthLogout(){
	DEASSERT_NOTNULL(pService.localUserId)
	
	EOS_Auth_LogoutOptions options{};
	options.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
	options.LocalUserId = pService.localUserId;
	
	GetModule().LogInfo("deEosSdkFlowAuthLogout.AuthLogout: Logging out user");
	EOS_Auth_Logout(pService.GetHandleAuth(), &options, this, fAuthLogoutCallback);
}

void deEosSdkFlowAuthLogout::DeletePersistentAuth(){
	EOS_Auth_DeletePersistentAuthOptions options{};
	options.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
	
	GetModule().LogInfo("deEosSdkFlowAuthLogout.DeletePersistentAuth: Delete persistent auth token");
	EOS_Auth_DeletePersistentAuth(pService.GetHandleAuth(), &options, this, fDeletePersistentAuthCallback);
}

void deEosSdkFlowAuthLogout::OnConnectLogoutCallback(const EOS_Connect_LogoutCallbackInfo &data){
	GetModule().LogInfoFormat("deEosSdkFlowAuthLogout.OnConnectLogoutCallback: res=%d",
		(int)data.ResultCode);
	
	if(data.ResultCode == EOS_EResult::EOS_Success){
		GetModule().LogInfo("deEosSdkFlowAuthLogout.OnConnectLogoutCallback: Game service logged out.");
		pService.productUserId = nullptr;
		
		try{
			AuthLogout();
			
		}catch(const deException &e){
			Fail(e);
			Finish();
		}
		
	}else{
		Fail(data.ResultCode);
		Finish();
	}
}

void deEosSdkFlowAuthLogout::OnAuthLogoutCallback(const EOS_Auth_LogoutCallbackInfo &data){
	GetModule().LogInfoFormat("deEosSdkFlowAuthLogout.OnAuthLogoutCallback: res=%d",
		(int)data.ResultCode);
	
	if(data.ResultCode == EOS_EResult::EOS_Success){
		GetModule().LogInfo("deEosSdkFlowAuthLogout.OnAuthLogoutCallback: User logged out.");
		pService.localUserId = nullptr;
		
		try{
			DeletePersistentAuth();
			
		}catch(const deException &e){
			GetModule().LogException(e);
			Finish();
		}
		
	}else{
		Fail(data.ResultCode);
		Finish();
	}
}

void deEosSdkFlowAuthLogout::OnDeletePersistentAuthCallback(
const EOS_Auth_DeletePersistentAuthCallbackInfo &data){
	GetModule().LogInfoFormat("deEosSdkFlowAuthLogout.OnDeletePersistentAuthCallback: res=%d",
		(int)data.ResultCode);
	
	if(data.ResultCode == EOS_EResult::EOS_Success){
		GetModule().LogInfo("deEosSdkFlowAuthLogout.OnDeletePersistentAuthCallback: Persistent auth token deleted.");
		
	}else{
		GetModule().LogErrorFormat("deEosSdkFlow: %s", EOS_EResult_ToString(data.ResultCode));
	}
	
	Finish();
}
