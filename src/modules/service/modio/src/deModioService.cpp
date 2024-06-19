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
#include "deModioService.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>


// Class deModioService
/////////////////////////////

const char * const deModioService::serviceName = "Mod.io";
deModioService *deModioService::pGlobalService = nullptr;


// Constructor, destructor
////////////////////////////

deModioService::deModioService( deModio &module,
deService *service, const deServiceObject::Ref &data ) :
pModule( module ),
pService( service ),
pPortal( Modio::Portal::None ),
pIsInitialized( false ),
pRequiresEventHandlingCount( 0 )
{
	if( pGlobalService ){
		DETHROW_INFO( deeInvalidAction, "Duplicate service" );
	}
	
	pApiKey = data->GetChildAt( "apiKey" )->GetString();
	pGameId = ( uint64_t )data->GetChildAt( "gameId" )->GetString().ToLongValid();
	pUserId = data->GetChildAt( "userId" )->GetString();
	
	const decString &environment = data->GetChildAt( "environment" )->GetString();
	if( environment == "live" ){
		pEnvironment = Modio::Environment::Live;
		
	}else if( environment == "test" ){
		pEnvironment = Modio::Environment::Test;
		
	}else{
		decString message;
		message.Format( "Invalid environment: %s", environment.GetString() );
		DETHROW_INFO( deeInvalidParam, message );
	}
	
	deServiceObject::Ref child( data->GetChildAt( "portal" ) );
	if( child ){
		const decString &portal = child->GetString();
		if( portal == "apple" ){
			pPortal = Modio::Portal::Apple;
			
		}else if( portal == "epicGamesStore" ){
			pPortal = Modio::Portal::EpicGamesStore;
			
		}else if( portal == "gog" ){
			pPortal = Modio::Portal::GOG;
			
		}else if( portal == "google" ){
			pPortal = Modio::Portal::Google;
			
		}else if( portal == "itchio" ){
			pPortal = Modio::Portal::Itchio;
			
		}else if( portal == "nintendo" ){
			pPortal = Modio::Portal::Nintendo;
			
		}else if( portal == "psn" ){
			pPortal = Modio::Portal::PSN;
			
		}else if( portal == "steam" ){
			pPortal = Modio::Portal::Steam;
			
		}else if( portal == "xboxLive" ){
			pPortal = Modio::Portal::XboxLive;
			
		}else{
			decString message;
			message.Format( "Invalid portal: %s", portal.GetString() );
			DETHROW_INFO( deeInvalidParam, message );
		}
	}
	
	Modio::InitializeOptions options;
	options.APIKey = Modio::ApiKey( pApiKey.GetString() );
	options.GameID = Modio::GameID( pGameId );
	options.User = pUserId;
	options.GameEnvironment = pEnvironment;
	options.PortalInUse = pPortal;
	
	module.LogInfo( "deModioService: Initialize service" );
	Modio::InitializeAsync( options, [ this ]( Modio::ErrorCode ec ){
		pOnInitializeFinished( ec );
	});
	
	pGlobalService = this;
	AddRequiresEventHandlingCount();
}

deModioService::~deModioService(){
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
	
	pGlobalService = nullptr;
	
	if( pRequiresEventHandlingCount > 0 ){
		pModule.RemoveRequiresEventHandlingCount();
	}
}


// Management
///////////////

void deModioService::StartRequest( const decUniqueID &id, const deServiceObject& request ){
	const decString &function = request.GetChildAt( "function" )->GetString();
	
	if( function == "xxx" ){
		///RequestCurrentStats( id );
		
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



void deModioService::FailRequest( const decUniqueID &id, const deException &e ){
	pModule.LogException( e );
	
	const deModioPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( ! pr ){
		return;
	}
	
	pr->data->SetStringChildAt( "error", e.GetName().GetMiddle( 3 ) );
	pr->data->SetStringChildAt( "message", e.GetDescription() );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		deService::Ref( pService ), id, pr->data );
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
		data->SetStringChildAt( "message", ec.message().c_str() );
		
	} else {
		data->SetBoolChildAt( "success", true );
		pIsInitialized = true;
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueEventReceived( deService::Ref( pService ), data );
}
