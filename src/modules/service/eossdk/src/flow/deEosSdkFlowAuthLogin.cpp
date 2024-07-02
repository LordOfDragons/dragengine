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

#include "deEosSdkFlowAuthLogin.h"
#include "../deEosSdkServiceEos.h"
#include "../deEosSdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deEosSdkFlowAuthLogin
////////////////////////////////

static void fAutoLoginCallback( const EOS_Auth_LoginCallbackInfo *data ){
	( ( deEosSdkFlowAuthLogin* )data->ClientData )->OnAutoLoginCallback( *data );
}

static void fAutoLoginDeletePersistentAuthCallback(
const EOS_Auth_DeletePersistentAuthCallbackInfo *data ){
	( ( deEosSdkFlowAuthLogin* )data->ClientData )->
		OnAutoLoginDeletePersistentAuthCallback( *data );
}

static void fLoginCallback( const EOS_Auth_LoginCallbackInfo *data ){
	( ( deEosSdkFlowAuthLogin* )data->ClientData )->OnLoginCallback( *data );
}

static void fConnectLoginCallback( const EOS_Connect_LoginCallbackInfo *data ){
	( ( deEosSdkFlowAuthLogin* )data->ClientData )->OnConnectLoginCallback( *data );
}

// Constructor, destructor
////////////////////////////

deEosSdkFlowAuthLogin::deEosSdkFlowAuthLogin( deEosSdkServiceEos &service,
	const decUniqueID &id, const deServiceObject &request ) :
deEosSdkFlow( service, id ),
pScope( EOS_EAuthScopeFlags::EOS_AS_BasicProfile )
{
	// get parameters
	deServiceObject::Ref so;
	
	so = request.GetChildAt( "exchangeCode" );
	if( so ){
		pExchangeCode = so->GetString();
	}
	
	so = request.GetChildAt( "userId" );
	if( so ){
		pUserId = so->GetString();
	}
	
	so = request.GetChildAt( "userPassword" );
	if( so ){
		pUserPassword = so->GetString();
	}
	
	so = request.GetChildAt("scopeFriends");
	if( so && so->GetBoolean() ){
		pScope |= EOS_EAuthScopeFlags::EOS_AS_FriendsList;
	}
	
	so = request.GetChildAt("scopePresence");
	if( so && so->GetBoolean() ){
		pScope |= EOS_EAuthScopeFlags::EOS_AS_Presence;
	}
	
	// begin login
	service.NewPendingRequest( id, "authLogin" );
	
	try{
		if( pExchangeCode.IsEmpty() && pUserId.IsEmpty() && pUserPassword.IsEmpty() ){
			AutoLogin();
			
		}else{
			Login();
		}
		
	}catch( const deException &e ){
		Fail( e );
	}
}



// Management
///////////////

void deEosSdkFlowAuthLogin::AutoLogin(){
	EOS_Auth_Credentials credentials = {};
	credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	credentials.Type = EOS_ELoginCredentialType::EOS_LCT_PersistentAuth;
	
	EOS_Auth_LoginOptions options = {};
	options.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	options.ScopeFlags = pScope;
	options.Credentials = &credentials;
	
	GetModule().LogInfo( "deEosSdkFlowAuthLogin: Logging in user using persistent authentification" );
	EOS_Auth_Login( pService.GetHandleAuth(), &options, this, fAutoLoginCallback );
}

void deEosSdkFlowAuthLogin::Login(){
	EOS_Auth_Credentials credentials = {};
	credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	
	if( ! pExchangeCode.IsEmpty() ){
		credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExchangeCode;
		credentials.Token = pExchangeCode;
		
	} else if( ! pUserId.IsEmpty() && ! pUserPassword.IsEmpty() ){
		credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Password;
		credentials.Id = pUserId;
		credentials.Token = pUserPassword;
		
	} else {
		credentials.Type = EOS_ELoginCredentialType::EOS_LCT_AccountPortal;
	}
	
	EOS_Auth_LoginOptions options = {};
	options.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	options.ScopeFlags = pScope;
	options.Credentials = &credentials;
	
	GetModule().LogInfo( "deEosSdkFlowAuthLogin: Logging in user" );
	EOS_Auth_Login( pService.GetHandleAuth(), &options, this, fLoginCallback );
}

void deEosSdkFlowAuthLogin::ConnectLogin(){
	// get auth token from logged in user
	/*
	EOS_Auth_CopyIdTokenOptions ctoptions = {};
	ctoptions.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
	ctoptions.AccountId = pService.selectedAccountId;
	
	EOS_Auth_IdToken *token = nullptr;
	EOS_EResult res = EOS_Auth_CopyIdToken( pService.GetHandleAuth(), &ctoptions, &token );
	if( res != EOS_EResult::EOS_Success ){
		Fail( res );
		return;
	}
	*/
	
	EOS_Auth_CopyUserAuthTokenOptions cuatoptions = {};
	cuatoptions.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;
	
	EOS_Auth_Token *token = nullptr;
	EOS_EResult res = EOS_Auth_CopyUserAuthToken( pService.GetHandleAuth(),
		&cuatoptions, pService.localUserId, &token );
	if( res != EOS_EResult::EOS_Success ){
		Fail( res );
		return;
	}
	
	// use auth token to connect game service
	EOS_Connect_Credentials credentials = {};
	credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
	credentials.Type = EOS_EExternalCredentialType::EOS_ECT_EPIC;
	//credentials.Token = token->JsonWebToken;
	credentials.Token = token->AccessToken;
	
	EOS_Connect_UserLoginInfo userLoginInfo = {};
	userLoginInfo.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
	
	EOS_Connect_LoginOptions options = {};
	options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
	options.Credentials = &credentials;
	options.UserLoginInfo = &userLoginInfo;
	
	GetModule().LogInfo( "deEosSdkFlowInit: Connect login using logged in user" );
	EOS_Connect_Login( pService.GetHandleConnect(), &options, this, fConnectLoginCallback );
	
	//EOS_Auth_IdToken_Release( token );
	EOS_Auth_Token_Release( token );
}



void deEosSdkFlowAuthLogin::OnAutoLoginCallback( const EOS_Auth_LoginCallbackInfo &data ){
	GetModule().LogInfoFormat(
		"deEosSdkFlowAuthLogin.OnAutoLoginCallback: res=%d",
		( int )data.ResultCode );
	
	if( data.ResultCode == EOS_EResult::EOS_Success ){
		OnLoginCallback( data );
		return;
	}
	
	EOS_Auth_DeletePersistentAuthOptions options = {};
	options.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
	
	GetModule().LogInfo( "deEosSdkFlowAuthLogin.OnAutoLoginCallback: Login failed. Delete persistent auth token" );
	EOS_Auth_DeletePersistentAuth( pService.GetHandleAuth(), &options,
		this, fAutoLoginDeletePersistentAuthCallback );
}

void deEosSdkFlowAuthLogin::OnAutoLoginDeletePersistentAuthCallback(
const EOS_Auth_DeletePersistentAuthCallbackInfo &data ){
	GetModule().LogInfoFormat(
		"deEosSdkFlowAuthLogin.OnAutoLoginDeletePersistentAuthCallback: res=%d",
		( int )data.ResultCode );
	
	if( pService.GetPendingRequestWithId( pId ) ){
		Login();
		
	} else {
		Abandon();
	}
}

void deEosSdkFlowAuthLogin::OnLoginCallback( const EOS_Auth_LoginCallbackInfo &data ){
	GetModule().LogInfoFormat(
		"deEosSdkFlowAuthLogin.OnLoginCallback: res=%d",
		( int )data.ResultCode );
	
	if( data.ResultCode == EOS_EResult::EOS_Success ){
		GetModule().LogInfo( "deEosSdkFlowAuthLogin: User logged in." );
		pService.localUserId = data.LocalUserId;
		pService.selectedAccountId = data.SelectedAccountId;
		ConnectLogin();
		
	}else{
		Fail( data.ResultCode );
	}
}

void deEosSdkFlowAuthLogin::OnConnectLoginCallback( const EOS_Connect_LoginCallbackInfo &data ){
	GetModule().LogInfoFormat(
		"deEosSdkFlowAuthLogin.OnConnectLoginCallback: res=%d",
		( int )data.ResultCode );
	
	if( data.ResultCode == EOS_EResult::EOS_Success ){
		GetModule().LogInfo( "deEosSdkFlowAuthLogin: Game service connected using logged in user." );
		pService.productUserId = data.LocalUserId;
		Success();
		
	}else{
		Fail( data.ResultCode );
	}
}
