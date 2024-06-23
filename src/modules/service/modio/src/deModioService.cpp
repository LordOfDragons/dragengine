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
pBaseDirPathCount( 0 )
{
	if( pGlobalService ){
		DETHROW_INFO( deeInvalidAction, "Duplicate service" );
	}
	
	pApiKey = data->GetChildAt( "apiKey" )->GetString();
	pGameId = ( uint64_t )data->GetChildAt( "gameId" )->GetString().ToLongValid();
	pUserId = data->GetChildAt( "userId" )->GetString();
	pEnvironment = deMCCommon::Environment( data->GetChildAt( "environment" ) );
	
	deServiceObject::Ref child( data->GetChildAt( "portal" ) );
	if( child ){
		pPortal = deMCCommon::Portal( child );
	}
	
	Modio::InitializeOptions options;
	options.APIKey = Modio::ApiKey( pApiKey.GetString() );
	options.GameID = Modio::GameID( pGameId );
	options.User = pUserId;
	options.GameEnvironment = pEnvironment;
	options.PortalInUse = pPortal;
	
	module.LogInfo( "deModioService: Initialize service" );
	Modio::SetLogLevel( Modio::LogLevel::Trace );
	// Modio::SetLogLevel( Modio::LogLevel::Info );
	
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
	pInvalidator->invalidated = true;
	
	while( pPendingRequests.GetCount() > 0 ){
		CancelRequest( ( ( deModioPendingRequest* )pPendingRequests.GetAt( 0 ) )->id );
	}
	
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
	const decString &function = request.GetChildAt( "function" )->GetString();
	
	if( function == "listAllMods" ){
		ListAllMods( id, request );
		
	}else if( function == "loadResource" ){
		LoadResource( id, request );
		
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
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		deService::Ref( pService ), id, so );
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
		Modio::GetModMediaAsync(resource->modId, resource->logoSize, callback);
		break;
		
	case deModioResource::etGalleryImage:
		data->SetStringChildAt( "resourceType", "image" );
		Modio::GetModMediaAsync(resource->modId, resource->gallerySize, resource->galleryIndex, callback);
		break;
		
	case deModioResource::etAvatar:
		data->SetStringChildAt( "resourceType", "image" );
		Modio::GetModMediaAsync(resource->modId, resource->avatarSize, callback);
		break;
		
	default:
		FailRequest( id, deeInvalidParam( __FILE__, __LINE__, "Invalid resource load type" ) );
	}
	
	/*
	this one here is a problem. the mod information returns URLs for avatar, gallery images
	and logo but there are multiple functions to fetch the resources:
	
	Modio::GetModMediaAsync(Modio::ModID ModId, Modio::LogoSize LogoSize,
		std::function<void(Modio::ErrorCode, Modio::Optional<std::string>)> Callback);
	
	Modio::GetModMediaAsync(Modio::ModID ModId, Modio::GallerySize GallerySize, Modio::GalleryIndex Index,
		std::function<void(Modio::ErrorCode, Modio::Optional<std::string>)> Callback);
	
	Modio::GetModMediaAsync(Modio::ModID ModId, Modio::AvatarSize AvatarSize,
		std::function<void(Modio::ErrorCode, Modio::Optional<std::string>)> Callback)
	
	hence we are required to deduce from the URL what kind of resource the user wants to
	retrieve and thus which of the functions to call. at least the same same callback can
	be used for all function variants since it returns the filename on disk. once we have
	that filename we can use the matching image module to load the image.
	
	example urls:
	- logo: https://thumb.modcdn.io/mods/f04b/4099326/crop_320x180/img_test_red_roo.jpg
	- gallery: https://thumb.modcdn.io/mods/f04b/4099326/crop_1280x720/img_test_blue_roo.jpg
	- gallery: https://thumb.modcdn.io/mods/f04b/4099326/crop_320x180/img_test_blue_roo.jpg
	- avatar: https://thumb.modcdn.io/members/0102/32775296/crop_100x100/6d7efe1dc5d3a94a1782e1a199d0a2b.jpg
	*/
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
		pModule.LogError( ec.message().c_str() );
	}
}

void deModioService::FailRequest( const deModioPendingRequest::Ref &request, const deException &e ){
	pModule.LogException( e );
	
	request->data->SetStringChildAt( "error", e.GetName().GetMiddle( 3 ) );
	request->data->SetStringChildAt( "message", e.GetDescription() );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		deService::Ref( pService ), request->id, request->data );
}

void deModioService::FailRequest( const deModioPendingRequest::Ref &request, const Modio::ErrorCode &ec ){
	pModule.LogError( ec.message().c_str() );
	
	request->data->SetIntChildAt( "code", ec.value() );
	request->data->SetStringChildAt( "error", ec.category().name() );
	request->data->SetStringChildAt( "message", ec.message().c_str() );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		deService::Ref( pService ), request->id, request->data );
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
			deService::Ref( pService ), pr->id, pr->data, true );
		
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
		pModule.LogWarn("RemoveRequiresEventHandlingCount called with pRequiresEventHandlingCount == 0");
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
	pModule.LogInfoFormat( "deModioService: Initialize service finished: ec(%d)[%s]",
		ec.value(), ec.message().c_str() );
	RemoveRequiresEventHandlingCount();
	
	const deServiceObject::Ref data( deServiceObject::Ref::New( new deServiceObject ) );
	
	data->SetStringChildAt( "event", "initialized" );
	
	if( ec ) {
		data->SetBoolChildAt( "success", false );
		data->SetIntChildAt( "code", ec.value() );
		data->SetStringChildAt( "error", ec.category().name() );
		data->SetStringChildAt( "message", ec.message().c_str() );
		
	} else {
		data->SetBoolChildAt( "success", true );
		pIsInitialized = true;
		
		pPrintBaseInfos();
		pInitVFS();
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueEventReceived( deService::Ref( pService ), data );
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
			deService::Ref( pService ), pr->id, pr->data, true );
		
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
		
		pModule.LogInfoFormat( "pOnLoadResourceFinished: %s", filename->c_str() );
		
		decPath path( decPath::CreatePathNative( filename->c_str() ) );
		int i;
		
		for( i=0; i<pBaseDirPathCount; i++ ){
			const decPath &bdpath = pBaseDirPath[ i ];
			if( bdpath.IsParentOf( path ) ){
				path = path.RelativePath( bdpath, false );
				pModule.LogInfoFormat( "-> Relative to '%s': %s",
					pBaseDirPathStr[ i ].GetString(), path.GetPathUnix().GetString() );
				break;
			}
		}
		
		pModule.GetGameEngine()->GetParallelProcessing().AddTask( new cLoadResourceTask(
			*this, id, path.GetPathUnix(), deResourceLoader::ertImage ) );
		
	}catch( const deException &e ){
		FailRequest( id, e );
		return;
	}
}

void deModioService::pOnLogCallback( Modio::LogLevel level, const std::string &message ){
	switch( level ){
	case Modio::LogLevel::Trace:
	case Modio::LogLevel::Info:
		pModule.LogInfoFormat( "Log: %s", message.c_str() );
		break;
		
	case Modio::LogLevel::Warning:
		pModule.LogWarnFormat( "Log: %s", message.c_str() );
		break;
		
	case Modio::LogLevel::Error:
		pModule.LogErrorFormat( "Log: %s", message.c_str() );
		break;
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
		const char * const path = iter->c_str();
		pModule.LogInfoFormat( "- %s", path );
		pBaseDirPathStr.Add( path );
		pBaseDirPath[ pBaseDirPathCount++ ].SetFromNative( path );
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
