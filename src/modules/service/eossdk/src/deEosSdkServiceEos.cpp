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

#include "deEosSdk.h"
#include "deEosSdkServiceEos.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/utils/decBase64.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>


// Class deEosSdkServiceEos
/////////////////////////////

const char * const deEosSdkServiceEos::serviceName = "EOSSDK";

class cCallbackInfo{
public:
	deEosSdkServiceEos *service;
	decUniqueID id;
	
	cCallbackInfo( deEosSdkServiceEos *aservice, const decUniqueID &aid ) : service( aservice ), id( aid ){ }
	cCallbackInfo( const cCallbackInfo &info ) : service( info.service ), id( info.id ) { }
	
	static cCallbackInfo Get( const void *clientData ){
		const cCallbackInfo info( *( ( cCallbackInfo* )clientData ) );
		delete ( cCallbackInfo* )clientData;
		return info;
	}
};

static void fEosLoginAutoCallback( const EOS_Auth_LoginCallbackInfo *data ){
	const cCallbackInfo info( cCallbackInfo::Get( data->ClientData ) );
	info.service->OnLoginAutoCallback( info.id, *data );
}

static void fEosLoginAutoDeletePersistentAuthCallback( const EOS_Auth_DeletePersistentAuthCallbackInfo *data ){
	const cCallbackInfo info( cCallbackInfo::Get( data->ClientData ) );
	info.service->OnLoginAutoDeletePersistentAuthCallback( info.id, *data );
}

static void fEosLoginCallback( const EOS_Auth_LoginCallbackInfo *data ){
	const cCallbackInfo info( cCallbackInfo::Get( data->ClientData ) );
	info.service->OnLoginCallback( info.id, *data );
}

static void fEosLogoutCallback( const EOS_Auth_LogoutCallbackInfo *data ){
	const cCallbackInfo info( cCallbackInfo::Get( data->ClientData ) );
	info.service->OnLogoutCallback( info.id, *data );
}

static void fEosLogoutDeletePersistentAuthCallback( const EOS_Auth_DeletePersistentAuthCallbackInfo *data ){
	const cCallbackInfo info( cCallbackInfo::Get( data->ClientData ) );
	info.service->OnLogoutDeletePersistentAuthCallback( info.id, *data );
}

static void fEosQueryUserInfoCallback( const EOS_UserInfo_QueryUserInfoCallbackInfo *data ){
	const cCallbackInfo info( cCallbackInfo::Get( data->ClientData ) );
	info.service->OnQueryUserInfoCallback( info.id, *data );
}

// Constructor, destructor
////////////////////////////

deEosSdkServiceEos::deEosSdkServiceEos( deEosSdk &module,
deService *service, const deServiceObject::Ref &data ) :
pModule( module ),
pService( service ),
pHandlePlatform( nullptr ),
pHandleAuth( nullptr ),
pHandleUserInfo( nullptr ),
pLocalUserId( nullptr ),
pSelectedAccountId( nullptr )
{
	pModule.InitSdk( data );
	
	const decString &productId = data->GetChildAt( "productId" )->GetString();
	const decString &sandboxId = data->GetChildAt( "sandboxId" )->GetString();
	const decString &deploymentId = data->GetChildAt( "deploymentId" )->GetString();
	
	const char *clientId = nullptr;
	const deServiceObject::Ref &soClientId = data->GetChildAt( "clientId" );
	if( soClientId ){
		clientId = soClientId->GetString().GetString();
	}
	
	const char *clientSecret = nullptr;
	const deServiceObject::Ref &soClientSecret = data->GetChildAt( "clientSecret" );
	if( soClientSecret ){
		clientSecret = soClientSecret->GetString().GetString();
	}
	
	const deServiceObject::Ref &soIsServer = data->GetChildAt( "isServer" );
	const bool isServer = soIsServer ? soIsServer->GetBoolean() : false;
	
	EOS_Platform_Options options = {};
	options.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	options.ProductId = productId.GetString();
	options.SandboxId = sandboxId.GetString();
	options.ClientCredentials.ClientId = clientId;
	options.ClientCredentials.ClientSecret = clientSecret;
	options.bIsServer = isServer ? EOS_TRUE : EOS_FALSE;
	options.DeploymentId = deploymentId.GetString();
	
	pHandlePlatform = EOS_Platform_Create( &options );
	if( ! pHandlePlatform ){
		DETHROW_INFO( deeInvalidAction, "Failed create platform interface" );
	}
	
	module.AddFrameUpdater( this );
}

deEosSdkServiceEos::~deEosSdkServiceEos(){
	while( pPendingRequests.GetCount() > 0 ){
		CancelRequest( ( ( deEosSdkPendingRequest* )pPendingRequests.GetAt( 0 ) )->id );
	}
	EOS_Platform_Tick( pHandlePlatform );
	
	pModule.RemoveFrameUpdater( this );
	
	EOS_Platform_Release( pHandlePlatform );
}


// Management
///////////////

void deEosSdkServiceEos::StartRequest( const decUniqueID &id, const deServiceObject& request ){
	const decString &function = request.GetChildAt( "function" )->GetString();
	
	if( function == "authLogin" ){
		AuthLoginAuto( id, request );
		
	}else if( function == "authLogout" ){
		AuthLogout( id, request );
		
	}else if( function == "queryUserInfo" ){
		QueryUserInfo( id, request );
		
	}else{
		DETHROW_INFO( deeInvalidParam, "Unknown function" );
	}
}

void deEosSdkServiceEos::CancelRequest( const decUniqueID &id ){
	deEosSdkPendingRequest * const pr = GetPendingRequestWithId( id );
	if( ! pr ){
		return;
	}
	
	pPendingRequests.RemoveFrom( pPendingRequests.IndexOf( pr ) );
	
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	so->SetStringChildAt( "error", "Cancelled" );
	so->SetStringChildAt( "message", "Request cancelled" );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed( pService, id, so );
}

deServiceObject::Ref deEosSdkServiceEos::RunAction( const deServiceObject &action ){
	const decString &function = action.GetChildAt( "function" )->GetString();
	
	if( function == "copyIdToken" ){
		return CopyIdToken( action );
		
	}else{
		DETHROW_INFO( deeInvalidParam, "Unknown function" );
	}
}

// Request
////////////

deEosSdkPendingRequest *deEosSdkServiceEos::GetPendingRequestWithId( const decUniqueID &id ) const{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deEosSdkPendingRequest * const pr = ( deEosSdkPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->id == id ){
			return pr;
		}
	}
	
	return nullptr;
}

deEosSdkPendingRequest::Ref deEosSdkServiceEos::RemoveFirstPendingRequestWithId(
const decUniqueID &id ){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deEosSdkPendingRequest * const pr = ( deEosSdkPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->id == id ){
			const deEosSdkPendingRequest::Ref prr( pr );
			pPendingRequests.RemoveFrom( i );
			return prr;
		}
	}
	
	return nullptr;
}

deEosSdkPendingRequest::Ref deEosSdkServiceEos::RemoveFirstPendingRequestWithFunction(
const char *function ){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deEosSdkPendingRequest * const pr = ( deEosSdkPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->function == function ){
			const deEosSdkPendingRequest::Ref prr( pr );
			pPendingRequests.RemoveFrom( i );
			return prr;
		}
	}
	
	return nullptr;
}

deEosSdkPendingRequest::Ref deEosSdkServiceEos::NewPendingRequest(
const decUniqueID &id, const decString &function, const deServiceObject::Ref &data ){
	const deEosSdkPendingRequest::Ref pr( deEosSdkPendingRequest::Ref::New( new deEosSdkPendingRequest( data ) ) );
	pr->id = id;
	pr->function = function;
	pr->data->SetStringChildAt( "function", function );
	pPendingRequests.Add( pr );
	return pr;
}


void deEosSdkServiceEos::AuthLoginAuto( const decUniqueID &id, const deServiceObject &request ){
	const deServiceObject::Ref &soExchangeCode = request.GetChildAt( "exchangeCode" );
	const deServiceObject::Ref &soUserId = request.GetChildAt( "userId" );
	const deServiceObject::Ref &soUserPassword = request.GetChildAt( "userPassword" );
	
	if( soExchangeCode || soUserId || soUserPassword ){
		AuthLogin( id, request, true );
		return;
	}
	
	EOS_Auth_Credentials credentials = {};
	credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	credentials.Type = EOS_ELoginCredentialType::EOS_LCT_PersistentAuth;
	
	EOS_Auth_LoginOptions options = pCreateLoginOptions( request );
	options.Credentials = &credentials;
	
	pModule.LogInfo( "deEosSdkServiceEos: Logging in user using persistent authentification");
	NewPendingRequest( id, "authLogin" )->request.TakeOver( new deServiceObject( request, true ) );
	EOS_Auth_Login( pGetHandleAuth(), &options, new cCallbackInfo( this, id ), fEosLoginAutoCallback );
}

void deEosSdkServiceEos::AuthLogin( const decUniqueID &id, const deServiceObject &request,
bool startRequest ){
	const deServiceObject::Ref &soExchangeCode = request.GetChildAt( "exchangeCode" );
	const deServiceObject::Ref &soUserId = request.GetChildAt( "userId" );
	const deServiceObject::Ref &soUserPassword = request.GetChildAt( "userPassword" );
	
	EOS_Auth_Credentials credentials = {};
	credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	
	if( soExchangeCode ){
		credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExchangeCode;
		credentials.Token = soExchangeCode->GetString().GetString();
		
	} else if( soUserId && soUserPassword ){
		credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Password;
		credentials.Id = soUserId->GetString().GetString();
		credentials.Token = soUserPassword->GetString().GetString();
		
	} else {
		credentials.Type = EOS_ELoginCredentialType::EOS_LCT_AccountPortal;
	}
	
	EOS_Auth_LoginOptions options = pCreateLoginOptions( request );
	options.Credentials = &credentials;
	
	pModule.LogInfo( "deEosSdkServiceEos: Logging in user");
	if( startRequest ){
		NewPendingRequest( id, "authLogin" );
	}
	EOS_Auth_Login( pGetHandleAuth(), &options, new cCallbackInfo( this, id ), fEosLoginCallback );
}

void deEosSdkServiceEos::AuthLogout( const decUniqueID &id, const deServiceObject &request ){
	if( ! pLocalUserId ){
		DETHROW_INFO( deeInvalidAction, "No user logged in" );
	}
	
	EOS_Auth_LogoutOptions options = {};
	options.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
	options.LocalUserId = pLocalUserId;
	
	pModule.LogInfo( "deEosSdkServiceEos: Logging out user");
	NewPendingRequest( id, "authLogout" );
	EOS_Auth_Logout( pGetHandleAuth(), &options, new cCallbackInfo( this, id ), fEosLogoutCallback );
}

void deEosSdkServiceEos::QueryUserInfo( const decUniqueID &id, const deServiceObject &request ){
	if( ! pLocalUserId ){
		DETHROW_INFO( deeInvalidAction, "No user logged in" );
	}
	
	EOS_UserInfo_QueryUserInfoOptions options = {};
	options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
	options.LocalUserId = pLocalUserId;
	options.TargetUserId = pLocalUserId;
	
	pModule.LogInfo( "deEosSdkServiceEos: Get user information");
	NewPendingRequest( id, "queryUserInfo" );
	EOS_UserInfo_QueryUserInfo( pGetHandleUserInfo(), &options,
		new cCallbackInfo( this, id ), fEosQueryUserInfoCallback );
}

deServiceObject::Ref deEosSdkServiceEos::CopyIdToken( const deServiceObject &action ){
	if( ! pSelectedAccountId ){
		DETHROW_INFO( deeInvalidAction, "No user logged in" );
	}
	
	EOS_Auth_CopyIdTokenOptions options = {};
	options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
	options.AccountId = pSelectedAccountId;
	
	EOS_Auth_IdToken *token = nullptr;
	deServiceObject::Ref result;
	
	try{
		const EOS_EResult res = EOS_Auth_CopyIdToken( pGetHandleAuth(), &options, &token );
		if( res != EOS_EResult::EOS_Success ){
			DETHROW_INFO( deeInvalidAction, EOS_EResult_ToString( res ) );
		}
		
		result = deServiceObject::NewString( token->JsonWebToken );
		
		EOS_Auth_IdToken_Release( token );
		
	}catch( const deException &e ){
		if( token ){
			EOS_Auth_IdToken_Release( token );
		}
		throw;
	}
	
	return result;
}

void deEosSdkServiceEos::FailRequest( const decUniqueID &id, const deException &e ){
	const deEosSdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( pr ){
		FailRequest( pr, e );
		
	}else{
		pModule.LogException( e );
	}
}

void deEosSdkServiceEos::FailRequest( const decUniqueID &id, EOS_EResult res ){
	const deEosSdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( pr ){
		FailRequest( pr, res );
		
	}else{
		pModule.LogErrorFormat( "deEosSdkServiceEos: %s", EOS_EResult_ToString( res ) );
	}
}

void deEosSdkServiceEos::FailRequest( const deEosSdkPendingRequest::Ref &pr, const deException &e ){
	pModule.LogException( e );
	
	pr->data->SetStringChildAt( "error", e.GetName().GetMiddle( 3 ) );
	pr->data->SetStringChildAt( "message", e.GetDescription() );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed( pService, pr->id, pr->data );
}

void deEosSdkServiceEos::FailRequest( const deEosSdkPendingRequest::Ref &request, EOS_EResult res ){
	pModule.LogErrorFormat( "deEosSdkServiceEos: %s", EOS_EResult_ToString( res ) );
	
	request->data->SetIntChildAt( "code", ( int )res );
	request->data->SetStringChildAt( "error", EOS_EResult_ToString( res ) );
	request->data->SetStringChildAt( "message", EOS_EResult_ToString( res ) );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		pService, request->id, request->data );
}

// EOS Callbacks
//////////////////

void deEosSdkServiceEos::OnLoginAutoCallback( const decUniqueID &id, const EOS_Auth_LoginCallbackInfo &data ){
	pModule.LogInfoFormat( "deEosSdkServiceEos.OnLoginAutoCallback: res=%d", ( int )data.ResultCode );
	if( data.ResultCode == EOS_EResult::EOS_Success ){
		OnLoginCallback( id, data );
		return;
	}
	
	EOS_Auth_DeletePersistentAuthOptions options = {};
	options.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
	
	pModule.LogInfo( "deEosSdkServiceEos.OnLoginAutoCallback: Login failed. Delete persistent auth token" );
	EOS_Auth_DeletePersistentAuth( pGetHandleAuth(), &options,
		new cCallbackInfo( this, id ), fEosLoginAutoDeletePersistentAuthCallback );
}

void deEosSdkServiceEos::OnLoginAutoDeletePersistentAuthCallback( const decUniqueID &id,
const EOS_Auth_DeletePersistentAuthCallbackInfo &data ){
	pModule.LogInfoFormat( "deEosSdkServiceEos.OnLoginAutoDeletePersistentAuthCallback: res=%d", ( int )data.ResultCode );
	deEosSdkPendingRequest * const pr = GetPendingRequestWithId( id );
	if( pr ){
		AuthLogin( id, pr->request, false );
	}
}

void deEosSdkServiceEos::OnLoginCallback(const decUniqueID &id, const EOS_Auth_LoginCallbackInfo &data){
	pModule.LogInfoFormat( "deEosSdkServiceEos.OnLoginCallback: res=%d", ( int )data.ResultCode );
	if( data.ResultCode != EOS_EResult::EOS_Success ){
		FailRequest( id, data.ResultCode );
		return;
	}
	
	pLocalUserId = data.LocalUserId;
	pSelectedAccountId = data.SelectedAccountId;
	
	const deEosSdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( ! pr ){
		return;
	}
	
	try{
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true );
		
	}catch( const deException &e ){
		FailRequest( pr, e );
		return;
	}
}

void deEosSdkServiceEos::OnLogoutCallback(const decUniqueID &id, const EOS_Auth_LogoutCallbackInfo &data){
	pModule.LogInfoFormat( "deEosSdkServiceEos.OnLogoutCallback: res=%d", ( int )data.ResultCode );
	if( data.ResultCode != EOS_EResult::EOS_Success ){
		FailRequest( id, data.ResultCode );
		return;
	}
	
	pLocalUserId = nullptr;
	pSelectedAccountId = nullptr;
	
	EOS_Auth_DeletePersistentAuthOptions options = {};
	options.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
	
	pModule.LogInfo( "deEosSdkServiceEos.OnLogoutCallback: Delete persistent auth token" );
	EOS_Auth_DeletePersistentAuth( pGetHandleAuth(), &options,
		new cCallbackInfo( this, id ), fEosLogoutDeletePersistentAuthCallback );
}

void deEosSdkServiceEos::OnLogoutDeletePersistentAuthCallback( const decUniqueID &id,
const EOS_Auth_DeletePersistentAuthCallbackInfo &data ){
	pModule.LogInfoFormat( "deEosSdkServiceEos.OnLogoutDeletePersistentAuthCallback: res=%d", ( int )data.ResultCode );
	const deEosSdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( ! pr ){
		return;
	}
	
	try{
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true );
		
	}catch( const deException &e ){
		FailRequest( pr, e );
		return;
	}
}

void deEosSdkServiceEos::OnQueryUserInfoCallback( const decUniqueID &id,
const EOS_UserInfo_QueryUserInfoCallbackInfo &data ){
	pModule.LogInfoFormat( "deEosSdkServiceEos.OnQueryUserInfoCallback: res=%d", ( int )data.ResultCode );
	const deEosSdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( ! pr ){
		return;
	}
	
	EOS_UserInfo *info = nullptr;
	
	try{
		EOS_UserInfo_CopyUserInfoOptions options = {};
		options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
		options.LocalUserId = data.LocalUserId;
		options.TargetUserId = data.TargetUserId;
		
		EOS_UserInfo_CopyUserInfo( pGetHandleUserInfo(), &options, &info );
		const EOS_UserInfo &ri = *info;
		
		char userId[ EOS_EPICACCOUNTID_MAX_LENGTH + 1 ];
		int32_t userIdLen = sizeof( userId );
		if( EOS_EpicAccountId_ToString( ri.UserId, userId, &userIdLen ) == EOS_EResult::EOS_Success ){
			pr->data->SetStringChildAt( "userId", userId );
		}
		
		if( ri.DisplayName ){
			pr->data->SetStringChildAt( "displayName", ri.DisplayName );
		}
		if( ri.Nickname ){
			pr->data->SetStringChildAt( "nickname", ri.Nickname );
		}
		if( ri.Country ){
			pr->data->SetStringChildAt( "country", ri.Country );
		}
		if( ri.PreferredLanguage ){
			pr->data->SetStringChildAt( "preferredLanguage", ri.PreferredLanguage );
		}
		
		EOS_UserInfo_Release( info );
		info = nullptr;
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true );
		
	}catch( const deException &e ){
		if( info ){
			EOS_UserInfo_Release( info );
		}
		
		FailRequest( pr, e );
		return;
	}
}



// deEosSdk::cFrameUpdater
////////////////////////////

void deEosSdkServiceEos::FrameUpdate( float elapsed ){
	EOS_Platform_Tick( pHandlePlatform );
}



// Private Functions
//////////////////////

EOS_HAuth deEosSdkServiceEos::pGetHandleAuth()
{
	if( ! pHandleAuth ){
		pHandleAuth = EOS_Platform_GetAuthInterface( pHandlePlatform );
	}
	return pHandleAuth;
}

EOS_HUserInfo deEosSdkServiceEos::pGetHandleUserInfo(){
	if( ! pHandleUserInfo ){
		pHandleUserInfo = EOS_Platform_GetUserInfoInterface( pHandlePlatform );
	}
	return pHandleUserInfo;
}

EOS_Auth_LoginOptions deEosSdkServiceEos::pCreateLoginOptions( const deServiceObject &request ) const{
	EOS_Auth_LoginOptions options = {};
	options.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	options.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile;
	
	const deServiceObject::Ref &soScopeFriends = request.GetChildAt("scopeFriends");
	if( soScopeFriends && soScopeFriends->GetBoolean() ){
		options.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_FriendsList;
	}
	
	const deServiceObject::Ref &soScopePresence = request.GetChildAt("scopePresence");
	if( soScopePresence && soScopePresence->GetBoolean() ){
		options.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Presence;
	}
	
	return options;
}
