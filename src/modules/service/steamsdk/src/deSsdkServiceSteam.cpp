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

#include "deSteamSdk.h"
#include "deSsdkServiceSteam.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>


#ifdef OS_W32
#define SSDKPFLLU "I64u"
#define SSDKPFLLX "I64x"
#else
#define SSDKPFLLU "llu"
#define SSDKPFLLX "llx"
#endif


// Class deSsdkServiceSteam
/////////////////////////////

const char * const deSsdkServiceSteam::serviceName = "SteamSDK";

// Constructor, destructor
////////////////////////////

deSsdkServiceSteam::deSsdkServiceSteam( deSteamSdk &module,
deService *service, const deServiceObject::Ref &data ) :
pModule( module ),
pService( service )
{
	pModule.InitSdk( data );
}

deSsdkServiceSteam::~deSsdkServiceSteam(){
	while( pPendingRequests.GetCount() > 0 ){
		CancelRequest( ( ( deSsdkPendingRequest* )pPendingRequests.GetAt( 0 ) )->id );
	}
}


// Management
///////////////

void deSsdkServiceSteam::StartRequest( const decUniqueID &id, const deServiceObject& request ){
	const decString &function = request.GetChildAt( "function" )->GetString();
	
	if( function == "requestCurrentStats" ){
		RequestCurrentStats( id );
		
	}else if( function == "getStats" ){
		GetStats( id, request );
		
	}else if( function == "resetAllStats" ){
		ResetAllStats( id, request );
		
	}else if( function == "setStats" ){
		SetStats( id, request );
		
	}else{
		DETHROW_INFO( deeInvalidParam, "Unknown function" );
	}
}

void deSsdkServiceSteam::CancelRequest( const decUniqueID &id ){
	deSsdkPendingRequest * const pr = GetPendingRequestWithId( id );
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

deSsdkPendingRequest *deSsdkServiceSteam::GetPendingRequestWithId( const decUniqueID &id ) const{
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deSsdkPendingRequest * const pr = ( deSsdkPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->id == id ){
			return pr;
		}
	}
	
	return nullptr;
}

deSsdkPendingRequest::Ref deSsdkServiceSteam::RemoveFirstPendingRequestWithId(
const decUniqueID &id ){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deSsdkPendingRequest * const pr = ( deSsdkPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->id == id ){
			const deSsdkPendingRequest::Ref prr( pr );
			pPendingRequests.RemoveFrom( i );
			return prr;
		}
	}
	
	return nullptr;
}

deSsdkPendingRequest::Ref deSsdkServiceSteam::RemoveFirstPendingRequestWithFunction(
const char *function ){
	const int count = pPendingRequests.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deSsdkPendingRequest * const pr = ( deSsdkPendingRequest* )pPendingRequests.GetAt( i );
		if( pr->function == function ){
			const deSsdkPendingRequest::Ref prr( pr );
			pPendingRequests.RemoveFrom( i );
			return prr;
		}
	}
	
	return nullptr;
}

deSsdkPendingRequest::Ref deSsdkServiceSteam::NewPendingRequest(
const decUniqueID &id, const decString &function ){
	const deSsdkPendingRequest::Ref pr( deSsdkPendingRequest::Ref::New( new deSsdkPendingRequest ) );
	pr->id = id;
	pr->function = function;
	pr->data->SetStringChildAt( "function", function );
	pPendingRequests.Add( pr );
	return pr;
}

void deSsdkServiceSteam::RequestCurrentStats( const decUniqueID &id ){
	DEASSERT_TRUE( SteamUserStats()->RequestCurrentStats() );
	NewPendingRequest( id, "requestCurrentStats" );
}

void deSsdkServiceSteam::GetStats( const decUniqueID &id, const deServiceObject& request ){
	const deServiceObject::Ref response( deServiceObject::Ref::New( new deServiceObject ) );
	deServiceObject::Ref soIn, soResp;
	int i, count;
	
	response->SetStringChildAt( "function", "getStats" );
	
	// stats
	soResp.TakeOver( new deServiceObject );
	response->SetChildAt( "stats", soResp );
	
	soIn = request.GetChildAt( "stats" );
	if( soIn ){
		const decStringList keys( soIn->GetChildrenKeys() );
		count = keys.GetCount();
		for( i=0; i<count; i++ ){
			const decString &apiName = keys.GetAt( i );
			int32 valueInt;
			float valueFloat;
			if( SteamUserStats()->GetStat( apiName, &valueInt ) ){
				soResp->SetIntChildAt( apiName, valueInt );
				
			}else if( SteamUserStats()->GetStat( apiName, &valueFloat ) ){
				soResp->SetFloatChildAt( apiName, valueFloat );
			}
		}
	}
	
	// achievements
	soResp.TakeOver( new deServiceObject );
	response->SetChildAt( "achievements", soResp );
	
	soIn = request.GetChildAt( "achievements" );
	if( soIn ){
		const decStringList keys( soIn->GetChildrenKeys() );
		count = keys.GetCount();
		for( i=0; i<count; i++ ){
			const decString &apiName = keys.GetAt( i );
			bool achieved;
			if( SteamUserStats()->GetAchievement( apiName, &achieved ) ){
				soResp->SetBoolChildAt( apiName, achieved );
			}
		}
	}
	
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
		deService::Ref( pService ), id, response, true );
}

void deSsdkServiceSteam::ResetAllStats( const decUniqueID &id, const deServiceObject &request ){
	const deServiceObject::Ref soResetAchievements( request.GetChildAt( "resetAchievements" ) );
	const bool resetAchievements = soResetAchievements && soResetAchievements->GetBoolean();
	
	DEASSERT_TRUE( SteamUserStats()->ResetAllStats( resetAchievements ) )
	
	const deServiceObject::Ref response( deServiceObject::Ref::New( new deServiceObject ) );
	response->SetStringChildAt( "function", "resetAllStats" );
	
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
		deService::Ref( pService ), id, response, true );
}

void deSsdkServiceSteam::SetStats( const decUniqueID &id, const deServiceObject &request ){
	deServiceObject::Ref soIn;
	int i, count;
	
	// stats
	soIn = request.GetChildAt( "stats" );
	if( soIn ){
		const decStringList keys( soIn->GetChildrenKeys() );
		count = keys.GetCount();
		for( i=0; i<count; i++ ){
			const decString &apiName = keys.GetAt( i );
			const deServiceObject::Ref soValue( soIn->GetChildAt( apiName ) );
			
			if( soValue->IsInteger() ){
				DEASSERT_TRUE( SteamUserStats()->SetStat( apiName, soValue->GetInteger() ) )
				
			}else if( soValue->IsFloat() ){
				DEASSERT_TRUE( SteamUserStats()->SetStat( apiName, ( float )soValue->GetFloat() ) )
				
			}else{
				DETHROW_INFO( deeInvalidParam, apiName );
			}
		}
	}
	
	// achievements
	soIn = request.GetChildAt( "achievements" );
	if( soIn ){
		const decStringList keys( soIn->GetChildrenKeys() );
		count = keys.GetCount();
		for( i=0; i<count; i++ ){
			const decString &apiName = keys.GetAt( i );
			if( soIn->GetChildAt( apiName )->GetBoolean() ){
				DEASSERT_TRUE( SteamUserStats()->SetAchievement( apiName ) )
				
			}else{
				DEASSERT_TRUE( SteamUserStats()->ClearAchievement( apiName ) )
			}
		}
	}
	
	// store
	DEASSERT_TRUE( SteamUserStats()->StoreStats() );
	NewPendingRequest( id, "setStats" );
}



void deSsdkServiceSteam::FailRequest( const decUniqueID &id, const deException &e ){
	pModule.LogException( e );
	
	const deSsdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithId( id ) );
	if( ! pr ){
		return;
	}
	
	pr->data->SetStringChildAt( "error", e.GetName().GetMiddle( 3 ) );
	pr->data->SetStringChildAt( "message", e.GetDescription() );
	pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
		deService::Ref( pService ), id, pr->data );
}



// Steam Callbacks
////////////////////

void deSsdkServiceSteam::OnUserStatsReceived( UserStatsReceived_t *response ){
	const deSsdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithFunction( "requestCurrentStats" ) );
	if( ! pr ){
		return;
	}
	
	try{
		switch( response->m_eResult ){
		case k_EResultOK:
		case k_EResultFail: // player has no stats yet
			break;
			
		default:
			pr->data->SetStringChildAt( "error", "InvalidAction" );
			pr->data->SetIntChildAt( "errorResult", response->m_eResult );
			pr->data->SetStringChildAt( "message", "Request failed" );
			pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
				deService::Ref( pService ), pr->id, pr->data );
			return;
		}
		
		decString string;
		string.Format( "%" SSDKPFLLU, response->m_nGameID );
		pr->data->SetStringChildAt( "gameId", string );
		
		string.Format( "%u", response->m_steamIDUser.GetAccountID() );
		pr->data->SetStringChildAt( "accountId", string );
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			deService::Ref( pService ), pr->id, pr->data, true );
		
	}catch( const deException &e ){
		FailRequest( pr->id, e );
	}
}

void deSsdkServiceSteam::OnUserStatsStored( UserStatsStored_t *response ){
	const deSsdkPendingRequest::Ref pr( RemoveFirstPendingRequestWithFunction( "setStats" ) );
	if( ! pr ){
		return;
	}
	
	try{
		switch( response->m_eResult ){
		case k_EResultOK:
			break;
			
		default:
			pr->data->SetStringChildAt( "error", "InvalidAction" );
			pr->data->SetIntChildAt( "errorResult", response->m_eResult );
			pr->data->SetStringChildAt( "message", "Request failed" );
			pModule.GetGameEngine()->GetServiceManager()->QueueRequestFailed(
				deService::Ref( pService ), pr->id, pr->data );
			return;
		}
		
		decString string;
		string.Format( "%" SSDKPFLLU, response->m_nGameID );
		pr->data->SetStringChildAt( "gameId", string );
		
		pModule.GetGameEngine()->GetServiceManager()->QueueRequestResponse(
			deService::Ref( pService ), pr->id, pr->data, true );
		
	}catch( const deException &e ){
		FailRequest( pr->id, e );
	}
}
