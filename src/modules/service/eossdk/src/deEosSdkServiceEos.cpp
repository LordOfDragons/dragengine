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

// Constructor, destructor
////////////////////////////

deEosSdkServiceEos::deEosSdkServiceEos( deEosSdk &module,
deService *service, const deServiceObject::Ref &data ) :
pModule( module ),
pService( service )
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
	
	pPlatform = EOS_Platform_Create( &options );
	if( ! pPlatform ){
		DETHROW_INFO( deeInvalidAction, "Failed create platform interface" );
	}
	
	module.AddFrameUpdater( this );
}

deEosSdkServiceEos::~deEosSdkServiceEos(){
	while( pPendingRequests.GetCount() > 0 ){
		CancelRequest( ( ( deEosSdkPendingRequest* )pPendingRequests.GetAt( 0 ) )->id );
	}
	EOS_Platform_Tick( pPlatform );
	
	pModule.RemoveFrameUpdater( this );
	
	EOS_Platform_Release( pPlatform );
}


// Management
///////////////

void deEosSdkServiceEos::StartRequest( const decUniqueID &id, const deServiceObject& request ){
	const decString &function = request.GetChildAt( "function" )->GetString();
	
	if( function == "requestCurrentStats" ){
		
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
	
	if( function == "dummy" ){
		return nullptr;
		
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



void deEosSdkServiceEos::FailRequest( const decUniqueID &id, const deException &e ){
	const deEosSdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( pr ){
		FailRequest( pr, e );
		
	}else{
		pModule.LogException( e );
	}
}

void deEosSdkServiceEos::FailRequest( const deEosSdkPendingRequest::Ref &pr, const deException &e ){
	pModule.LogException( e );
	
	pr->data->SetStringChildAt( "error", e.GetName().GetMiddle( 3 ) );
	pr->data->SetStringChildAt( "message", e.GetDescription() );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed( pService, pr->id, pr->data );
}



// deEosSdk::cFrameUpdater
////////////////////////////

void deEosSdkServiceEos::FrameUpdate( float elapsed ){
	EOS_Platform_Tick( pPlatform );
}



// Private Functions
//////////////////////
