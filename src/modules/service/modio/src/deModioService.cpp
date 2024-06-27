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
#include "convert/deMCCommon.h"
#include "convert/deMCFilterParams.h"
#include "convert/deMCModInfo.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
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
	Modio::SetLogLevel( Modio::LogLevel::Info );
	Modio::SetLogLevel( Modio::LogLevel::Trace );
	
	Modio::SetLogCallback( [ this ]( Modio::LogLevel level, const std::string &message ){
		pOnLogCallback( level, message );
	});
	
	Modio::InitializeAsync( options, [ this ]( Modio::ErrorCode ec ){
		pOnInitializeFinished( ec );
	});
	
	pGlobalService = this;
	AddRequiresEventHandlingCount();
}

deModioService::~deModioService(){
	pModule.LogInfo( "deModioService: Destroy" );
	pInvalidator->invalidated = true;
	
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
		
	}else if( function == "loadResource" ){
		LoadResource( id, request );
		
	}else if( function == "pauseModManagement" ){
		PauseModManagement( id, request );
		
	}else if( function == "authenticateUserExternal" ){
		AuthenticateUserExternal( id, request );
		
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
	
	if( function == "isAuthenticated" ){
		return deServiceObject::NewBool( Modio::QueryUserProfile().has_value() );
		
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
	
	NewPendingRequest( id, "listAllMods" );
	
	AddRequiresEventHandlingCount();
	Modio::ListAllModsAsync( filter, [ this, id ]( Modio::ErrorCode ec, Modio::Optional<Modio::ModInfoList> results ){
		pOnListAllModsFinished( id, ec, results );
	});
}

void deModioService::LoadResource( const decUniqueID &id, const deServiceObject &request ){
	const decString &url = request.GetChildAt( "url" )->GetString();
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	data->SetStringChildAt( "url", url );
	
	NewPendingRequest( id, "loadResource", data );
	
	const deModioResource *resource = nullptr;
	if( ! pResources.GetAt( url, ( deObject** )&resource ) ){
		FailRequest( id, deeInvalidAction( __FILE__, __LINE__,
			"Resource information missing. List/Fetch mod info first" ) );
		return;
	}
	
	auto callback = [ this, id ]( Modio::ErrorCode ec, Modio::Optional<std::string> filename ){
		pOnLoadResourceFinished( id, ec, filename );
	};
	
	switch( resource->type ){
	case deModioResource::etLogo:
		data->SetStringChildAt( "resourceType", "image" );
		AddRequiresEventHandlingCount();
		Modio::GetModMediaAsync(resource->modId, resource->logoSize, callback);
		break;
		
	case deModioResource::etGalleryImage:
		data->SetStringChildAt( "resourceType", "image" );
		AddRequiresEventHandlingCount();
		Modio::GetModMediaAsync(resource->modId, resource->gallerySize, resource->galleryIndex, callback);
		break;
		
	case deModioResource::etAvatar:
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
	
	NewPendingRequest( id, "authenticateUserExternal" );
	AddRequiresEventHandlingCount();
	Modio::AuthenticateUserExternalAsync( authParams, provider, [ this, id ]( Modio::ErrorCode ec ){
		pOnAuthenticateUserExternal( id, ec );
	});
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

void deModioService::pOnInitializeFinished( Modio::ErrorCode ec ){
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
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueEventReceived( pService, data );
}

void deModioService::pOnListAllModsFinished( const decUniqueID &id,
Modio::ErrorCode ec, Modio::Optional<Modio::ModInfoList> results ){
	RemoveRequiresEventHandlingCount();
	if( ec ){
		FailRequest( id, ec );
		return;
	}
	
	const deModioPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( ! pr ){
		return;
	}
	
	try{
		const deServiceObject::Ref outMods( deServiceObject::NewList() );
		
		if( results.has_value() ){
			for( const Modio::ModInfo &mod : *results ){
				outMods->AddChild( deMCModInfo::ModInfo( mod ) );
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
	RemoveRequiresEventHandlingCount();
	if( ec ){
		FailRequest( id, ec );
		return;
	}
	
	try{
		if( ! filename.has_value() ){
			DETHROW_INFO( deeInvalidParam, "Missing filename in server response" );
		}
		
		pModule.LogInfoFormat( "deModioService.pOnLoadResourceFinished: %s", filename->c_str() );
		
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
		pModule.GetGameEngine()->GetParallelProcessing().AddTask( ( deParallelTask* )( deThreadSafeObject* )task );
		
	}catch( const deException &e ){
		FailRequest( id, e );
		return;
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

void deModioService::pOnAuthenticateUserExternal( const decUniqueID &id, Modio::ErrorCode ec ){
	RemoveRequiresEventHandlingCount();
	if( ec ){
		FailRequest( id, ec );
		return;
	}
	
	const deModioPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
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
