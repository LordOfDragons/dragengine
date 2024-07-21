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

#include "deModio.h"
#include "deModioResource.h"
#include "deModioService.h"
#include "config/deModioModConfig.h"
#include "config/deModioUserConfig.h"
#include "convert/deMCCommon.h"
#include "convert/deMCFilterParams.h"
#include "convert/deMCModInfo.h"
#include "convert/deMCUser.h"
#include "convert/deMCTagOptions.h"
#include "parameters/deMPLogLevel.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/resources/loader/tasks/deResourceLoaderTask.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/threading/deThreadSafeObjectReference.h>


// Class deModioService
/////////////////////////

const char * const deModioService::serviceName = "Mod.io";
deModioService *deModioService::pGlobalService = nullptr;

deModioService::cInvalidator::cInvalidator() : invalidated( false ){}


// Constructor, destructor
////////////////////////////

deModioService::deModioService( deModio &module,
deService *service, const deServiceObject::Ref &data ) :
pModule( module ),
pService( service ),
pPortal( Modio::Portal::None ),
pIsInitialized( false ),
pRequiresEventHandlingCount( 0 ),
pInvalidator( cInvalidator::Ref::New( new cInvalidator ) ),
pVFS( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) ),
pBaseDirPath( nullptr ),
pBaseDirPathCount( 0 ),
pPauseModManagement( false ),
pModManagementEnabled( false ),
pElapsedUpdateProgress( 0.0f ),
pUpdateProgressInterval( 1.0f )
{
	if( pGlobalService ){
		DETHROW_INFO( deeInvalidAction, "Duplicate service" );
	}
	
	pApiKey = data->GetChildAt( "apiKey" )->GetString();
	pGameId = ( uint64_t )data->GetChildAt( "gameId" )->GetString().ToLongValid();
	pUserId = data->GetChildAt( "userId" )->GetString();
	
	deServiceObject::Ref child( data->GetChildAt( "portal" ) );
	if( child ){
		pPortal = deMCCommon::Portal( child );
	}
	
	Modio::InitializeOptions options;
	options.APIKey = Modio::ApiKey( pApiKey.GetString() );
	options.GameID = Modio::GameID( pGameId );
	options.User = pUserId;
	options.GameEnvironment = Modio::Environment::Live;
	options.PortalInUse = pPortal;
	
	module.LogInfo( "deModioService: Initialize service" );
	Modio::SetLogLevel( module.GetParamLogLevel().GetLogLevel() );
	
	Modio::SetLogCallback( [ this ]( Modio::LogLevel level, const std::string &message ){
		pOnLogCallback( level, message );
	});
	
	Modio::InitializeAsync( options, [ this ]( Modio::ErrorCode ec ){
		pOnInitialize( ec );
	});
	
	pGlobalService = this;
	AddRequiresEventHandlingCount();
}

deModioService::~deModioService(){
	pModule.LogInfo( "deModioService: Destroy" );
	pInvalidator->invalidated = true;
	
	pModule.StoreFailureStateIfFailed();
	
	while( pPendingRequests.GetCount() > 0 ){
		CancelRequest( ( ( deModioPendingRequest* )pPendingRequests.GetAt( 0 ) )->id );
	}
	
	pPauseModManagement = true;
	pUpdateModManagementEnabled();
	
	if( pIsInitialized ){
		bool done = false;
		
		Modio::ShutdownAsync( [ &done ]( Modio::ErrorCode ){
			done = true;
		});
		
		while( ! done ){
			Modio::RunPendingHandlers();
		}
	}
	
	Modio::SetLogCallback( []( Modio::LogLevel, const std::string& ){} );
	
	pGlobalService = nullptr;
	
	if( pRequiresEventHandlingCount > 0 ){
		pModule.RemoveRequiresEventHandlingCount();
	}
	
	if( pBaseDirPath ){
		delete [] pBaseDirPath;
	}
}


// Management
///////////////

void deModioService::StartRequest( const decUniqueID &id, const deServiceObject& request ){
	if( ! pIsInitialized ){
		DETHROW_INFO( deeInvalidAction, "Not initialized" );
	}
	
	const decString &function = request.GetChildAt( "function" )->GetString();
	
	if( function == "listAllMods" ){
		ListAllMods( id, request );
		
	}else if( function == "loadModResource" ){
		LoadModResource( id, request );
		
	}else if( function == "pauseModManagement" ){
		PauseModManagement( id, request );
		
	}else if( function == "authenticateUserExternal" ){
		AuthenticateUserExternal( id, request );
		
	}else if( function == "clearUserData" ){
		ClearUserData( id, request );
		
	}else if( function == "subscribeToMod" ){
		SubscribeToMod( id, request );
		
	}else if( function == "unsubscribeFromMod" ){
		UnsubscribeFromMod( id, request );
		
	}else if( function == "submitModRating" ){
		SubmitModRating( id, request );
		
	}else if( function == "revokeModRating" ){
		RevokeModRating( id, request );
		
	}else if( function == "getModTagOptions" ){
		GetModTagOptions( id, request );
		
	}else if( function == "getModInfo" ){
		GetModInfo( id, request );
		
	}else if( function == "loadUserResource" ){
		LoadUserResource( id, request );
		
	}else{
		DETHROW_INFO( deeInvalidParam, "Unknown function" );
	}
}

void deModioService::CancelRequest( const decUniqueID &id ){
	deModioPendingRequest * const pr = GetPendingRequestWithId( id );
	if( ! pr ){
		return;
	}
	
	pPendingRequests.RemoveFrom( pPendingRequests.IndexOf( pr ) );
	
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	so->SetStringChildAt( "error", "Cancelled" );
	so->SetStringChildAt( "message", "Request cancelled" );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed( pService, id, so );
}

deServiceObject::Ref deModioService::RunAction( const deServiceObject &action ){
	if( ! pIsInitialized ){
		DETHROW_INFO( deeInvalidAction, "Not initialized" );
	}
	
	const decString &function = action.GetChildAt( "function" )->GetString();
	
	if( function == "activateMods" ){
		ActivateMods();
		return nullptr;
		
	}else if( function == "isAuthenticated" ){
		return IsAuthenticated();
		
	}else if( function == "queryCurrentModUpdate" ){
		return QueryCurrentModUpdate();
		
	}else if( function == "queryUserSubscriptions" ){
		return QueryUserSubscriptions();
		
	}else if( function == "querySystemInstallations" ){
		return QuerySystemInstallations();
		
	}else if( function == "queryUserProfile" ){
		return QueryUserProfile();
		
	}else if( function == "setModDisabled" ){
		SetModDisabled( action );
		return nullptr;
		
	}else if( function == "modHasMatchingFiles" ){
		return ModHasMatchingFiles( action );
		
	}else if( function == "getActiveMods" ){
		return GetActiveMods();
		
	}else if( function == "getUserFeatures" ){
		return GetUserFeatures();
		
	}else{
		DETHROW_INFO( deeInvalidParam, "Unknown function" );
	}
}

void deModioService::FrameUpdate(float elapsed)
{
	pCheckProgressUpdate( elapsed );
}

// Request
////////////

deModioPendingRequest *deModioService::GetPendingRequestWithId( const decUniqueID &id ) const{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deModioPendingRequest * const pr = ( deModioPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->id == id ){
			return pr;
		}
	}
	
	return nullptr;
}

deModioPendingRequest::Ref deModioService::RemoveFirstPendingRequestWithId(
const decUniqueID &id ){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deModioPendingRequest * const pr = ( deModioPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->id == id ){
			const deModioPendingRequest::Ref prr( pr );
			pPendingRequests.RemoveFrom( i );
			return prr;
		}
	}
	
	return nullptr;
}

deModioPendingRequest::Ref deModioService::RemoveFirstPendingRequestWithFunction(
const char *function ){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deModioPendingRequest * const pr = ( deModioPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->function == function ){
			const deModioPendingRequest::Ref prr( pr );
			pPendingRequests.RemoveFrom( i );
			return prr;
		}
	}
	
	return nullptr;
}

deModioPendingRequest::Ref deModioService::NewPendingRequest(
const decUniqueID &id, const decString &function, const deServiceObject::Ref &data ){
	const deModioPendingRequest::Ref pr( deModioPendingRequest::Ref::New( new deModioPendingRequest( data ) ) );
	pr->id = id;
	pr->function = function;
	pr->data->SetStringChildAt( "function", function );
	pPendingRequests.Add( pr );
	return pr;
}



void deModioService::ListAllMods( const decUniqueID &id, const deServiceObject &request ){
	const Modio::FilterParams filter( deMCFilterParams::FilterParams( request ) );
	
	pModule.LogInfo( "deModioService.ListAllMods" );
	
	NewPendingRequest( id, "listAllMods" );
	AddRequiresEventHandlingCount();
	
	Modio::ListAllModsAsync( filter, [ this, id ]( Modio::ErrorCode ec, Modio::Optional<Modio::ModInfoList> results ){
		pOnListAllModsFinished( id, ec, results );
	});
}

void deModioService::LoadModResource( const decUniqueID &id, const deServiceObject &request ){
	const decString &url = request.GetChildAt( "url" )->GetString();
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	data->SetStringChildAt( "url", url );
	
	NewPendingRequest( id, "loadModResource", data );
	
	const deModioResource *resource = nullptr;
	if( ! pResources.GetAt( url, ( deObject** )&resource ) ){
		FailRequest( id, deeInvalidAction( __FILE__, __LINE__,
			"Resource information missing. List/Fetch mod info first" ) );
		return;
	}
	
	std::function<void(Modio::ErrorCode, Modio::Optional<std::string>)> callback =
		[ this, id ]( Modio::ErrorCode ec, Modio::Optional<std::string> filename ){
			pOnLoadResourceFinished( id, ec, filename );
		};
	
	switch( resource->type ){
	case deModioResource::etLogo:
		pModule.LogInfoFormat(
			"deModioService.LoadModResource: image mod=%" PRIi64 " type=logo(%d) url=%s",
			( int64_t )resource->modId, ( int )resource->logoSize, url.GetString() );
		
		data->SetStringChildAt( "resourceType", "image" );
		AddRequiresEventHandlingCount();
		
		Modio::GetModMediaAsync(resource->modId, resource->logoSize, callback);
		break;
		
	case deModioResource::etGalleryImage:
		pModule.LogInfoFormat(
			"deModioService.LoadModResource: image mod=%" PRIi64 " type=gallery(%d@%d) url=%s",
			( int64_t )resource->modId, ( int )resource->gallerySize,
			( int )resource->galleryIndex, url.GetString() );
		
		data->SetStringChildAt( "resourceType", "image" );
		AddRequiresEventHandlingCount();
		
		Modio::GetModMediaAsync(resource->modId, resource->gallerySize, resource->galleryIndex, callback);
		break;
		
	case deModioResource::etAvatar:
		pModule.LogInfoFormat(
			"deModioService.LoadModResource: image mod=%" PRIi64 " type=avatar(%d) url=%s",
			( int64_t )resource->modId, ( int )resource->avatarSize, url.GetString() );
		
		data->SetStringChildAt( "resourceType", "image" );
		AddRequiresEventHandlingCount();
		
		Modio::GetModMediaAsync(resource->modId, resource->avatarSize, callback);
		break;
		
	default:
		FailRequest( id, deeInvalidParam( __FILE__, __LINE__, "Invalid resource load type" ) );
	}
}

void deModioService::PauseModManagement( const decUniqueID &id, const deServiceObject &request ){
	pPauseModManagement = request.GetChildAt( "pause" )->GetBoolean();
	pUpdateModManagementEnabled();
	
	pModule.LogInfoFormat( "deModioService.PauseModManagement: pause=%d", pPauseModManagement );
	
	const deServiceObject::Ref response( deServiceObject::Ref::New( new deServiceObject ) );
	response->SetStringChildAt( "function", "pauseModManagement" );
	response->SetBoolChildAt( "paused", pPauseModManagement );
	
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
		pService, id, response, true );
}

void deModioService::AuthenticateUserExternal( const decUniqueID &id, const deServiceObject &request ){
	const Modio::AuthenticationProvider provider =
		deMCCommon::AuthenticationProvider( request.GetChildAt( "provider" ) );
	
	Modio::AuthenticationParams authParams = {};
	authParams.AuthToken = request.GetChildAt( "token" )->GetString().GetString();
	authParams.bURLEncodeAuthToken = true;
	authParams.bUserHasAcceptedTerms = request.GetChildAt( "termsAccepted" )->GetBoolean();
	
	deServiceObject::Ref so( request.GetChildAt( "parameters" ) );
	if( so ){
		const decStringList keys( so->GetChildrenKeys() );
		const int count = keys.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			const decString &key = keys.GetAt( i );
			authParams.ExtendedParameters[ key.GetString() ] =
				so->GetChildAt( key.GetString() )->GetString().GetString();
		}
	}
	
	pModule.LogInfoFormat( "deModioService.AuthenticateUserExternal: provider=%d", ( int )provider );
	
	NewPendingRequest( id, "authenticateUserExternal" );
	AddRequiresEventHandlingCount();
	
	Modio::AuthenticateUserExternalAsync( authParams, provider, [ this, id ]( Modio::ErrorCode ec ){
		pOnAuthenticateUserExternal( id, ec );
	});
}

void deModioService::ClearUserData( const decUniqueID &id, const deServiceObject &request ){
	pModule.LogInfo( "deModioService.ClearUserData" );
	
	NewPendingRequest( id, "clearUserData" );
	AddRequiresEventHandlingCount();
	
	Modio::ClearUserDataAsync( [ this, id ]( Modio::ErrorCode ec ){
		pOnClearUserData( id, ec );
	});
}

void deModioService::SubscribeToMod( const decUniqueID &id, const deServiceObject &request ){
	const Modio::ModID modId( deMCCommon::ID( *request.GetChildAt( "modId" ) ) );
	const bool dependencies( request.GetChildAt( "dependencies" )->GetBoolean() );
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	data->SetChildAt( "modId", deMCCommon::ID( modId ) );
	
	pModule.LogInfoFormat( "deModioService.SubscribeToMod: id=%" PRIi64, ( int64_t )modId );
	
	NewPendingRequest( id, "subscribeToMod", data );
	AddRequiresEventHandlingCount();
	
	Modio::SubscribeToModAsync( modId, dependencies, [ this, id ]( Modio::ErrorCode ec ){
		pOnSubscribeToMod( id, ec );
	});
}

void deModioService::UnsubscribeFromMod( const decUniqueID &id, const deServiceObject &request ){
	const Modio::ModID modId( deMCCommon::ID( *request.GetChildAt( "modId" ) ) );
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	data->SetChildAt( "modId", deMCCommon::ID( modId ) );
	
	pModule.LogInfoFormat( "deModioService.UnsubscribeFromMod: id=%" PRIi64, ( int64_t )modId );
	
	NewPendingRequest( id, "unsubscribeFromMod", data );
	AddRequiresEventHandlingCount();
	
	Modio::UnsubscribeFromModAsync( modId, [ this, id ]( Modio::ErrorCode ec ){
		pOnUnsubscribeFromMod( id, ec );
	});
}

void deModioService::GetModInfo( const decUniqueID &id, const deServiceObject &request ){
	const Modio::ModID modId( deMCCommon::ID( *request.GetChildAt( "modId" ) ) );
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	data->SetChildAt( "modId", deMCCommon::ID( modId ) );
	
	pModule.LogInfoFormat( "deModioService.GetModInfo: id=%" PRIi64, ( int64_t )modId );
	
	NewPendingRequest( id, "getModInfo", data );
	AddRequiresEventHandlingCount();
	
	Modio::GetModInfoAsync( modId, [ this, id ]( Modio::ErrorCode ec, Modio::Optional<Modio::ModInfo> info ){
		pOnGetModInfo( id, ec, info );
	});
}

void deModioService::SubmitModRating( const decUniqueID &id, const deServiceObject &request ){
	const Modio::ModID modId( deMCCommon::ID( *request.GetChildAt( "modId" ) ) );
	const deServiceObject::Ref sorating( request.GetChildAt( "rating" ) );
	Modio::Rating rating = Modio::Rating::Neutral;
	
	if( sorating ){
		switch( sorating->GetInteger() ){
		case 0:
			rating = Modio::Rating::Negative;
			break;
			
		case 1:
			rating = Modio::Rating::Positive;
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "rating" );
		}
	}
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	const decString strModId( deMCCommon::IDToString( modId ) );
	data->SetChildAt( "modId", deServiceObject::NewString( strModId ) );
	
	pModule.LogInfoFormat( "deModioService.SubmitModRating: id=%" PRIi64 " rating=%d",
		( int64_t )modId, ( int )rating );
	
	pModule.GetUserConfig( pUserId ).SetUserRating( strModId, rating );
	
	NewPendingRequest( id, "submitModRating", data );
	AddRequiresEventHandlingCount();
	
	Modio::SubmitModRatingAsync( modId, rating, [ this, id ]( Modio::ErrorCode ec ){
		pOnRequestFinished( id, ec );
	});
}

void deModioService::RevokeModRating( const decUniqueID &id, const deServiceObject &request ){
	const Modio::ModID modId( deMCCommon::ID( *request.GetChildAt( "modId" ) ) );
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	const decString strModId( deMCCommon::IDToString( modId ) );
	data->SetChildAt( "modId", deServiceObject::NewString( strModId ) );
	
	pModule.LogInfoFormat( "deModioService.RevokeModRating: id=%" PRIi64, ( int64_t )modId );
	
	pModule.GetUserConfig( pUserId ).SetUserRating( strModId, Modio::Rating::Neutral );
	
	NewPendingRequest( id, "revokeModRating", data );
	AddRequiresEventHandlingCount();
	
	Modio::SubmitModRatingAsync( modId, Modio::Rating::Neutral, [ this, id ]( Modio::ErrorCode ec ){
		pOnRequestFinished( id, ec );
	});
}

void deModioService::GetModTagOptions( const decUniqueID &id, const deServiceObject &request ){
	pModule.LogInfo( "deModioService.GetModTagOptions" );
	
	NewPendingRequest( id, "getModTagOptions" );
	AddRequiresEventHandlingCount();
	
	Modio::GetModTagOptionsAsync( [ this, id ]( Modio::ErrorCode ec,
	Modio::Optional<Modio::ModTagOptions> tagOptions ){
		pOnGetModTagOptions( id, ec, tagOptions );
	});
}

void deModioService::LoadUserResource( const decUniqueID &id, const deServiceObject &request ){
	const decString &url = request.GetChildAt( "url" )->GetString();
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	data->SetStringChildAt( "url", url );
	
	NewPendingRequest( id, "loadUserResource", data );
	
	const Modio::Optional<Modio::User> user( Modio::QueryUserProfile() );
	if( ! user.has_value() ){
		FailRequest( id, deeInvalidAction( __FILE__, __LINE__,
			"Resource information missing. User not logged in" ) );
		return;
	}
	
	Modio::AvatarSize avatarSize;
	
	if( url == user.value().Avatar.Original.c_str() ){
		avatarSize = Modio::AvatarSize::Original;
		
	}else if( url == user.value().Avatar.Thumb100x100.c_str() ){
		avatarSize = Modio::AvatarSize::Thumb100;
		
	}else if( url == user.value().Avatar.Thumb50x50.c_str() ){
		avatarSize = Modio::AvatarSize::Thumb50;
		
	}else{
		FailRequest( id, deeInvalidAction( __FILE__, __LINE__, "Resource not found" ) );
		return;
	}
	
	pModule.LogInfoFormat( "deModioService.LoadUserResource: image type=avatar(%d) url=%s",
		( int )avatarSize, url.GetString() );
	
	data->SetStringChildAt( "resourceType", "image" );
	AddRequiresEventHandlingCount();
	
	Modio::GetUserMediaAsync( avatarSize, [ this, id ]( Modio::ErrorCode ec, Modio::Optional<std::string> filename ){
		pOnLoadResourceFinished( id, ec, filename );
	});
}

void deModioService::ActivateMods(){
	pActivateMods();
}

deServiceObject::Ref deModioService::IsAuthenticated(){
	return deServiceObject::NewBool( Modio::QueryUserProfile().has_value() );
}

deServiceObject::Ref deModioService::QueryCurrentModUpdate(){
	const Modio::Optional<Modio::ModProgressInfo> result( Modio::QueryCurrentModUpdate() );
	return result.has_value() ? deMCCommon::ModProgressInfo( *result ) : nullptr;
}

deServiceObject::Ref deModioService::QueryUserSubscriptions(){
	const std::map<Modio::ModID, Modio::ModCollectionEntry> result( Modio::QueryUserSubscriptions() );
	std::map<Modio::ModID, Modio::ModCollectionEntry>::const_iterator iter;
	const deModioUserConfig &config = pModule.GetUserConfig( pUserId );
	
	const deServiceObject::Ref so( deServiceObject::NewList() );
	for( iter = result.cbegin(); iter != result.cend(); iter++ ){
		const Modio::ModCollectionEntry &mod = iter->second;
		const deServiceObject::Ref soMod( deMCModInfo::ModCollectionEntry( mod, config ) );
		soMod->SetBoolChildAt( "disabled", config.GetModDisabled(
			deMCCommon::IDToString( mod.GetID() ) ) );
		so->AddChild( soMod );
	}
	return so;
}

deServiceObject::Ref deModioService::QuerySystemInstallations(){
	const std::map<Modio::ModID, Modio::ModCollectionEntry> result( Modio::QuerySystemInstallations() );
	std::map<Modio::ModID, Modio::ModCollectionEntry>::const_iterator iter;
	const deModioUserConfig &config = pModule.GetUserConfig( pUserId );
	
	const deServiceObject::Ref so( deServiceObject::NewList() );
	for( iter = result.cbegin(); iter != result.cend(); iter++ ){
		const Modio::ModCollectionEntry &mod = iter->second;
		const deServiceObject::Ref soMod( deMCModInfo::ModCollectionEntry( mod, config ) );
		soMod->SetBoolChildAt( "disabled", config.GetModDisabled(
			deMCCommon::IDToString( mod.GetID() ) ) );
		so->AddChild( soMod );
	}
	return so;
}

deServiceObject::Ref deModioService::QueryUserProfile(){
	const Modio::Optional<Modio::User> result( Modio::QueryUserProfile() );
	return result.has_value() ? deMCUser::User( *result ) : nullptr;
}

void deModioService::SetModDisabled( const deServiceObject &action ){
	const decString &modId = action.GetChildAt( "modId" )->GetString();
	const bool disabled = action.GetChildAt( "disabled" )->GetBoolean();
	pModule.GetUserConfig( pUserId ).SetModDisabled( modId, disabled );
}

class cHasModMatchingFilesSearch : public deFileSearchVisitor{
private:
	const bool pRecursive;
	decPath *pPatterns;
	const int pPatternCount;
	bool pMatched;
	
public:
	cHasModMatchingFilesSearch( bool recursive, const deServiceObject &soPatterns ) :
	pRecursive( recursive ),
	pPatterns( nullptr ),
	pPatternCount( soPatterns.GetChildCount() ),
	pMatched( false )
	{
		int i;
		pPatterns = new decPath[ pPatternCount ];
		for( i=0; i<pPatternCount; i++ ){
			pPatterns[ i ].SetFromUnix( soPatterns.GetChildAt( i )->GetString() );
		}
	}
	
	~cHasModMatchingFilesSearch() override{
		if( pPatterns ){
			delete [] pPatterns;
		}
	}
	
	inline bool GetMatched() const{ return pMatched; }
	
	bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ) override{
		int i;
		for( i=0; i<pPatternCount; i++ ){
			if( path.MatchesPattern( pPatterns[ i ] ) ){
				pMatched = true;
				return false;
			}
		}
		return true;
	}
	
	bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ) override{
		if( pRecursive ){
			vfs.SearchFiles( path, *this );
		}
		return true;
	}
};

deServiceObject::Ref deModioService::ModHasMatchingFiles( const deServiceObject &action ){
	const decPath directory( decPath::CreatePathUnix( action.GetChildAt( "directory" )->GetString() ) );
	const bool recursive = action.GetChildAt( "recursive" )->GetBoolean();
	const deServiceObject &soPatterns = action.GetChildAt( "patterns" );
	const deServiceObject &soModId = action.GetChildAt( "modId" );
	
	const std::map<Modio::ModID, Modio::ModCollectionEntry> mods( Modio::QueryUserSubscriptions() );
	const std::map<Modio::ModID, Modio::ModCollectionEntry>::const_iterator iter(
		mods.find( ( Modio::ModID )deMCCommon::ID( soModId ) ) );
	if( iter == mods.cend() ){
		DETHROW_INFO( deeInvalidAction, "Modification not installed" );
	}
	
	try{
		const deVFSDiskDirectory::Ref container( deVFSDiskDirectory::Ref::New(
			new deVFSDiskDirectory( decPath::CreatePathUnix( "/" ),
				decPath::CreatePathNative( iter->second.GetPath().c_str() ), true ) ) );
		
		const int count = soPatterns.GetChildCount();
		if( count == 0 ){
			return deServiceObject::NewBool( false );
		}
		
		cHasModMatchingFilesSearch visitor( recursive, soPatterns );
		
		const deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		vfs->AddContainer( container );
		vfs->SearchFiles( directory, visitor );
		
		return deServiceObject::NewBool( visitor.GetMatched() );
		
	}catch( const deException & ){
		DETHROW_INFO( deeInvalidAction, "Modification files unavailable" );
	}
}

deServiceObject::Ref deModioService::GetActiveMods(){
	const std::map<Modio::ModID, Modio::ModCollectionEntry> subscribed( Modio::QueryUserSubscriptions() );
	std::map<Modio::ModID, Modio::ModCollectionEntry>::const_iterator iter;
	const deModioUserConfig &userConfig = pModule.GetUserConfig( pUserId );
	const deServiceObject::Ref so( deServiceObject::NewList() );
	const decObjectList &modConfigs = pModule.GetActiveMods();
	const int modCount = modConfigs.GetCount();
	int i;
	
	for( i=0; i<modCount; i++ ){
		const deModioModConfig &modConfig = *( ( deModioModConfig* )modConfigs.GetAt( i ) );
		iter = subscribed.find( ( Modio::ModID )deMCCommon::ID( modConfig.id ) );
		DEASSERT_FALSE( iter == subscribed.cend() )
		so->AddChild( deMCModInfo::ModCollectionEntry( iter->second, userConfig ) );
	}
	
	return so;
}

deServiceObject::Ref deModioService::GetUserFeatures(){
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	so->SetBoolChildAt( "canManualLogin", true );
	so->SetBoolChildAt( "canAutomaticLogin", true );
	so->SetBoolChildAt( "canLogout", true );
	
	const deServiceObject::Ref soAuthProviders( deServiceObject::NewList() );
	soAuthProviders->AddStringChild( "steam" );
	soAuthProviders->AddStringChild( "epic" );
	so->SetChildAt( "canAuthProviderLogin", soAuthProviders );
	
	return so;
}

void deModioService::FailRequest( const decUniqueID &id, const deException &e ){
	const deModioPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( pr ){
		FailRequest( pr, e );
		
	}else{
		pModule.LogException( e );
	}
}

void deModioService::FailRequest( const decUniqueID &id, const Modio::ErrorCode &ec ){
	const deModioPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( pr ){
		FailRequest( pr, ec );
		
	}else{
		pModule.LogErrorFormat( "deModioService: %s", ec.message().c_str() );
	}
}

void deModioService::FailRequest( const deModioPendingRequest::Ref &request, const deException &e ){
	pModule.LogException( e );
	
	request->data->SetStringChildAt( "error", e.GetName().GetMiddle( 3 ) );
	request->data->SetStringChildAt( "message", e.GetDescription() );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		pService, request->id, request->data );
}

void deModioService::FailRequest( const deModioPendingRequest::Ref &request, const Modio::ErrorCode &ec ){
	pModule.LogErrorFormat( "deModioService: %s", ec.message().c_str() );
	
	request->data->SetIntChildAt( "code", ec.value() );
	request->data->SetStringChildAt( "error", ec.category().name() );
	request->data->SetStringChildAt( "message", ec.message().c_str() );
	
	if( ec == Modio::ApiError::UserNoAcceptTermsOfUse ){
		if( ! pPendingRequests.Has( request ) ){
			pPendingRequests.Add( request );
		}
		
		const decUniqueID id( request->id );
		AddRequiresEventHandlingCount();
		Modio::GetTermsOfUseAsync( [ this, id ]( Modio::ErrorCode ec2, Modio::Optional<Modio::Terms> terms ){
			pModule.LogInfoFormat( "deModioService.GetTermsOfUseAsync: ec(%d)[%s]",
				ec2.value(), ec2.message().c_str() );
			
			const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec2 ) );
			if( pr ){
				if( terms.has_value() ){
					pr->data->SetChildAt( "needAcceptTerms", deMCCommon::NeedAcceptTerms( terms.value() ) );
				}
				
				RemoveRequiresEventHandlingCount();
				pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
					pService, pr->id, pr->data );
			}
		});
		return;
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		pService, request->id, request->data );
}



void deModioService::OnFinishedLoadResource( const decUniqueID &id, const decString &path,
deResource *resource ){
	if( ! resource ){
		decString message;
		message.Format( "Failed loading cached resource: %s", path.GetString() );
		FailRequest( id, deeInvalidAction( __FILE__, __LINE__, message ) );
		return;
	}
	
	pModule.LogInfoFormat( "deModioService.OnFinishedLoadResource: path=%s", path.GetString() );
	
	const deModioPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( ! pr ){
		return;
	}
	
	try{
		pr->data->SetResourceChildAt( "resource", resource );
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true );
		
	}catch( const deException &e ){
		FailRequest( pr, e );
		return;
	}
}



void deModioService::AddRequiresEventHandlingCount(){
	pRequiresEventHandlingCount++;
	if( pRequiresEventHandlingCount == 1 ){
		pModule.AddRequiresEventHandlingCount();
	}
}

void deModioService::RemoveRequiresEventHandlingCount(){
	if( pRequiresEventHandlingCount == 0 ){
		pModule.LogWarn("deModioService.RemoveRequiresEventHandlingCount"
			" called with pRequiresEventHandlingCount == 0");
		return;
	}
	
	pRequiresEventHandlingCount--;
	if( pRequiresEventHandlingCount == 0 ) {
		pModule.RemoveRequiresEventHandlingCount();
	}
}


// Callbacks
//////////////

void deModioService::pOnInitialize( Modio::ErrorCode ec ){
	pModule.LogInfoFormat( "deModioService.pOnInitializeFinished: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	RemoveRequiresEventHandlingCount();
	pUpdateModManagementEnabled();
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	
	data->SetStringChildAt( "event", "initialized" );
	deMCCommon::ErrorOutcome( data, ec );
	
	if( ! ec ) {
		pIsInitialized = true;
		
		pPrintBaseInfos();
		pInitVFS();
		
		if( Modio::QueryUserProfile().has_value() ){
			pModule.LogInfo( "deModioService.pOnInitializeFinished: User logged in. Fetch updates" );
			Modio::FetchExternalUpdatesAsync( [ this ]( Modio::ErrorCode ec2 ){
				pOnInitializeFetchUpdates( ec2 );
			});
			return;
		}
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueEventReceived( pService, data );
}

void deModioService::pOnInitializeFetchUpdates( Modio::ErrorCode ec ){
	pModule.LogInfoFormat( "deModioService.pOnInitializeFetchUpdates: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	data->SetStringChildAt( "event", "initialized" );
	data->SetBoolChildAt( "success", true );
	pModule.GetGameEngine()->GetServiceManager()->QueueEventReceived( pService, data );
}

void deModioService::pOnRequestFinished( const decUniqueID &id, Modio::ErrorCode ec ){
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( pr ){
		pOnBaseResponseExit( pr );
	}
}

void deModioService::pOnListAllModsFinished( const decUniqueID &id,
Modio::ErrorCode ec, Modio::Optional<Modio::ModInfoList> results ){
	pModule.LogInfoFormat( "deModioService.pOnListAllModsFinished: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( ! pr ){
		return;
	}
	
	try{
		const deServiceObject::Ref outMods( deServiceObject::NewList() );
		const deModioUserConfig &config = pModule.GetUserConfig( pUserId );
		
		if( results.has_value() ){
			for( const Modio::ModInfo &mod : *results ){
				outMods->AddChild( deMCModInfo::ModInfo( mod, config ) );
			}
		}
		
		pr->data->SetChildAt( "mods", outMods );
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true );
		
	}catch( const deException &e ){
		FailRequest( pr, e );
		return;
	}
	
	// store information required to load images later
	if( results.has_value() ){
		for( const Modio::ModInfo &mod : *results ){
			deModioResource::Register( pResources, mod );
		}
	}
}

class cLoadResourceTask : public deParallelTask{
private:
	deModioService &pService;
	const deModioService::cInvalidator::Ref pInvalidator;
	deThreadSafeObjectReference pTaskLoadResource;
	const decUniqueID pRequestId;
	const decString pPath;
	
public:
	cLoadResourceTask ( deModioService &service, const decUniqueID &requestId,
		const decString &path, deResourceLoader::eResourceType resourceType ) :
	deParallelTask( &service.GetModule() ),
	pService( service ),
	pInvalidator( service.GetInvalidator() ),
	pRequestId( requestId ),
	pPath( path )
	{
		SetEmptyRun( true );
		
		switch( resourceType ){
		case deResourceLoader::ertImage:
			pLoadImage();
			break;
			
		default:
			Cancel();
		}
	}
	
	void Run() override{}
	
	void Finished() override{
		if( pInvalidator->invalidated ){
			Cancel();
			return;
		}
		
		const deResourceLoaderTask * const rlt = ( deResourceLoaderTask* )( deThreadSafeObject* )pTaskLoadResource;
		
		if( ! IsCancelled() && rlt->GetState() == deResourceLoaderTask::esSucceeded ){
			pService.OnFinishedLoadResource( pRequestId, pPath, rlt->GetResource() );
			
		}else{
			pService.OnFinishedLoadResource( pRequestId, pPath, nullptr );
		}
	}
	
	decString GetDebugName() const override{
		return "ModioService-LoadResource";
	}
	
private:
	void pLoadImage(){
		pTaskLoadResource = pService.GetModule().GetGameEngine()->GetResourceLoader()->
			AddLoadRequest( pService.GetVFS(), pPath, deResourceLoader::ertImage );
		deResourceLoaderTask * const rlt = ( deResourceLoaderTask* )( deThreadSafeObject* )pTaskLoadResource;
		
		switch( rlt->GetState() ){
		case deResourceLoaderTask::esPending:
			if( ! DoesDependOn( rlt ) ){
				AddDependsOn( rlt );
			}
			break;
			
		case deResourceLoaderTask::esSucceeded:
			break;
			
		case deResourceLoaderTask::esFailed:
			Cancel();
			break;
		}
	}
};


void deModioService::pOnLoadResourceFinished( const decUniqueID &id, Modio::ErrorCode ec,
Modio::Optional<std::string> filename ){
	pModule.LogInfoFormat( "deModioService.pOnLoadResourceFinished: ec(%d)[%s] filename=%s",
		ec.value(), ec.message().c_str(), filename.has_value() ? filename->c_str() : "-" );
	
	RemoveRequiresEventHandlingCount();
	if( ec ){
		FailRequest( id, ec );
		return;
	}
	
	try{
		if( ! filename.has_value() ){
			DETHROW_INFO( deeInvalidParam, "Missing filename in server response" );
		}
		
		decPath path( decPath::CreatePathNative( filename->c_str() ) );
		int i;
		
		for( i=0; i<pBaseDirPathCount; i++ ){
			const decPath &bdpath = pBaseDirPath[ i ];
			if( bdpath.IsParentOf( path ) ){
				path = path.RelativePath( bdpath, false );
				/*pModule.LogInfoFormat( "-> Relative to '%s': %s",
					pBaseDirPathStr[ i ].GetString(), path.GetPathUnix().GetString() );*/
				break;
			}
		}
		
		deThreadSafeObjectReference task;
		task.TakeOver( new cLoadResourceTask( *this, id, path.GetPathUnix(), deResourceLoader::ertImage ) );
		pModule.GetGameEngine()->GetParallelProcessing().AddTaskAsync( ( deParallelTask* )( deThreadSafeObject* )task );
		
	}catch( const deException &e ){
		FailRequest( id, e );
		return;
	}
}

void deModioService::pOnGetModInfo( const decUniqueID &id, Modio::ErrorCode ec,
Modio::Optional<Modio::ModInfo> info ){
	pModule.LogInfoFormat( "deModioService.pOnGetModInfo: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( ! pr ){
		return;
	}
	
	try{
		if( ! info.has_value() ){
			DETHROW_INFO( deeInvalidParam, "Mod info not received from server" );
		}
		
		const deModioUserConfig &config = pModule.GetUserConfig( pUserId );
		pr->data->SetChildAt( "info", deMCModInfo::ModInfo( *info, config ) );
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true );
		
	}catch( const deException &e ){
		FailRequest( pr, e );
	}
	
	// store information required to load images later
	if( info.has_value() ){
		deModioResource::Register( pResources, *info );
	}
}

void deModioService::pOnAuthenticateUserExternal( const decUniqueID &id, Modio::ErrorCode ec ){
	pModule.LogInfoFormat( "deModioService.pOnAuthenticateUserExternal: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec, true ) );
	if( ! pr ){
		return;
	}
	
	pModManagementEnabled = false;
	pUpdateModManagementEnabled();
	
	pModule.LogInfo( "deModioService.pOnAuthenticateUserExternal: Fetch updates" );
	Modio::FetchExternalUpdatesAsync( [ this, id ]( Modio::ErrorCode ec2 ){
		pOnAuthenticateUserExternalFetchUpdates( id, ec2 );
	});
}

void deModioService::pOnAuthenticateUserExternalFetchUpdates(
const decUniqueID &id, Modio::ErrorCode ec ){
	pModule.LogInfoFormat( "deModioService.pOnAuthenticateUserExternalFetchUpdates: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( pr ){
		pOnBaseResponseExit( pr );
	}
}

void deModioService::pOnClearUserData( const decUniqueID &id, Modio::ErrorCode ec ){
	pModule.LogInfoFormat( "deModioService.pOnClearUserData: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( ! pr ){
		return;
	}
	
	pModManagementEnabled = false;
	pActivateMods();
	pOnBaseResponseExit( pr );
}

void deModioService::pOnSubscribeToMod( const decUniqueID &id, Modio::ErrorCode ec ){
	pModule.LogInfoFormat( "deModioService.pOnSubscribeToMod: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( pr ){
		pOnBaseResponseExit( pr );
	}
}

void deModioService::pOnUnsubscribeFromMod( const decUniqueID &id, Modio::ErrorCode ec ){
	pModule.LogInfoFormat( "deModioService.pOnUnsubscribeFromMod: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	pActivateMods();
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( pr ){
		pOnBaseResponseExit( pr );
	}
}

void deModioService::pOnGetModTagOptions( const decUniqueID &id, Modio::ErrorCode ec,
Modio::Optional<Modio::ModTagOptions> tagOptions ){
	pModule.LogInfoFormat( "deModioService.pOnGetModTagOptions: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	
	const deModioPendingRequest::Ref pr( pOnBaseResponseInit( id, ec ) );
	if( pr ){
		if( tagOptions.has_value() ){
			pr->data->SetChildAt( "categories", deMCTagOptions::ModTagOptions( *tagOptions ) );
		}
		pOnBaseResponseExit( pr );
	}
}

void deModioService::pOnLogCallback( Modio::LogLevel level, const std::string &message ){
	switch( level ){
	case Modio::LogLevel::Trace:
	case Modio::LogLevel::Info:
		pModule.LogInfoFormat( "deModioService.pOnLogCallback: %s", message.c_str() );
		break;
		
	case Modio::LogLevel::Warning:
		pModule.LogWarnFormat( "deModioService.pOnLogCallback: %s", message.c_str() );
		break;
		
	case Modio::LogLevel::Error:
		pModule.LogErrorFormat( "deModioService.pOnLogCallback: %s", message.c_str() );
		break;
	}
}

void deModioService::pOnModManagement( Modio::ModManagementEvent event ){
	const bool busy = Modio::IsModManagementBusy();
	
	pModule.LogInfoFormat( "deModioService.pOnModManagement: ev=%d id=%" PRIi64 " busy=%d ec(%d)[%s]",
		( int )event.Event, ( int64_t )event.ID, busy, event.Status.value(), event.Status.message().c_str() );
	
	switch( event.Event ){
	case Modio::ModManagementEvent::EventType::BeginUpdate:
		pActivateMods();
		break;
		
	default:
		break;
	}
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	
	data->SetStringChildAt( "event", "modManagement" );
	data->SetChildAt( "modId", deMCCommon::ID( event.ID ) );
	data->SetChildAt( "subEvent", deMCCommon::ModManagementEventType( event.Event ) );
	data->SetBoolChildAt( "busy", busy );
	deMCCommon::ErrorOutcome( data, event.Status );
	
	const Modio::Optional<Modio::ModProgressInfo> progress = Modio::QueryCurrentModUpdate();
	if( progress.has_value() ){
		data->SetChildAt( "progress", deMCCommon::ModProgressInfo( *progress ) );
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueEventReceived( pService, data );
}

void deModioService::pPrintBaseInfos(){
	pModule.LogInfoFormat( "deModioService: DefaultModInstallationDirectory: %s",
		Modio::GetDefaultModInstallationDirectory( Modio::GameID( pGameId ) ).c_str() );
	
	pModule.LogInfo( "deModioService: BaseModInstallationDirectories:" );
	const std::vector<std::string> directories( Modio::GetBaseModInstallationDirectories() );
	if( directories.empty() ){
		return;
	}
	
	pBaseDirPath = new decPath[ directories.size() ];
	
	std::vector<std::string>::const_iterator iter;
	for( iter = directories.cbegin(); iter != directories.cend(); iter++ ){
		pModule.LogInfoFormat( "- %s", iter->c_str() );
		
		decPath &path = pBaseDirPath[ pBaseDirPathCount++ ];
		path.SetFromNative( iter->c_str() );
		
		// path returned is of the form: <homedir>/mod.io/common/<game-id>/mods/
		// cache files are of the form: <homedir>/mod.io/common/<game-id>/cache/mods/<mod-id>/<path>
		// mounting the base path is not going to work this way. using the parent directory
		// should work unless the directory structure changes
		path.RemoveLastComponent();
		
		pBaseDirPathStr.Add( path.GetPathNative() );
	}
}

void deModioService::pInitVFS(){
	pModule.LogInfo( "deModioService: InitVFS" );
	const decPath rootPath( decPath::CreatePathUnix( "/" ) );
	int i;
	
	for( i=0; i<pBaseDirPathCount; i++ ){
		const decPath &diskPath = pBaseDirPath[ i ];
		pModule.LogInfoFormat( "- Add '%s' => '%s' (ro)",
			pBaseDirPathStr[ i ].GetString(), rootPath.GetPathUnix().GetString() );
		pVFS->AddContainer( deVFSDiskDirectory::Ref::New(
			new deVFSDiskDirectory( rootPath, diskPath, true ) ) );
	}
}

void deModioService::pUpdateModManagementEnabled(){
	if( pPauseModManagement ){
		if( ! pModManagementEnabled ){
			return;
		}
		
		Modio::DisableModManagement();
		RemoveRequiresEventHandlingCount();
		pModManagementEnabled = false;
		pModule.LogInfo( "deModioService.pUpdateModManagementEnabled: Management disabled" );
		
	} else {
		if( pModManagementEnabled ){
			return;
		}
		
		const Modio::ErrorCode ec = Modio::EnableModManagement( [ this ]( Modio::ModManagementEvent event ){
			pOnModManagement( event );
		});
		
		if( ec ){
			pModule.LogErrorFormat( "deModioService.pUpdateModManagementEnabled: %s", ec.message().c_str() );
			
		}else{
			AddRequiresEventHandlingCount();
			pModManagementEnabled = true;
			pModule.LogInfo( "deModioService.pUpdateModManagementEnabled: Management enabled" );
		}
	}
}

void deModioService::pCheckProgressUpdate( float elapsed ){
	pElapsedUpdateProgress += elapsed;
	if( pElapsedUpdateProgress < pUpdateProgressInterval ){
		return;
	}
	
	pElapsedUpdateProgress = 0.0f;
	
	const Modio::Optional<Modio::ModProgressInfo> progress = Modio::QueryCurrentModUpdate();
	if( ! progress.has_value() ){
		return;
	}
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	
	data->SetStringChildAt( "event", "modManagement" );
	data->SetChildAt( "modId", deMCCommon::ID( progress->ID ) );
	data->SetStringChildAt( "subEvent", "progress" );
	data->SetBoolChildAt( "busy", Modio::IsModManagementBusy() );
	data->SetChildAt( "progress", deMCCommon::ModProgressInfo( *progress ) );
	data->SetBoolChildAt( "success", true );
	
	pModule.GetGameEngine()->GetServiceManager()->QueueEventReceived( pService, data );
}

void deModioService::pActivateMods(){
	pModule.LogInfo( "deModioService.pActivateMods" );
	
	const std::map<Modio::ModID, Modio::ModCollectionEntry> result( Modio::QueryUserSubscriptions() );
	std::map<Modio::ModID, Modio::ModCollectionEntry>::const_iterator iter;
	deModioModConfig::Ref config;
	decObjectList configs;
	
	for( iter = result.cbegin(); iter != result.cend(); iter++ ){
		const Modio::ModCollectionEntry &mod = iter->second;
		
		switch( mod.GetModState() ){
		case Modio::ModState::Installed:
		case Modio::ModState::UpdatePending:
			config.TakeOver( new deModioModConfig );
			config->id = deMCCommon::IDToString( mod.GetID() );
			config->path = mod.GetPath().c_str();
			if( mod.GetModProfile().FileInfo.has_value() ){
				config->releaseVersion = mod.GetModProfile().FileInfo.value().Version.c_str();
			}
			configs.Add( config );
			break;
			
		default:
			break;
		}
	}
	
	pModule.SetModConfigs( configs );
	pModule.ActivateMods( pUserId );
}

deModioPendingRequest::Ref deModioService::pOnBaseResponseInit( const decUniqueID &id,
Modio::ErrorCode ec, bool peekPendingRequest ){
	if( ec ){
		FailRequest( id, ec );
		return nullptr;
	}
	
	if( peekPendingRequest ){
		return GetPendingRequestWithId( id );
		
	}else{
		return RemoveFirstPendingRequestWithId( id );
	}
}

bool deModioService::pOnBaseResponseExit( const deModioPendingRequest::Ref &pr ){
	RemoveRequiresEventHandlingCount();
	try{
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			pService, pr->id, pr->data, true );
		return true;
		
	}catch( const deException &e ){
		FailRequest( pr, e );
		return false;
	}
}
