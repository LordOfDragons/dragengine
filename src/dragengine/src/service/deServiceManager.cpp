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

#include "deServiceManager.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../common/utils/decUniqueID.h"
#include "../threading/deMutexGuard.h"
#include "../systems/deModuleSystem.h"
#include "../systems/deScriptingSystem.h"
#include "../systems/modules/deLoadableModule.h"
#include "../systems/modules/service/deBaseServiceModule.h"
#include "../systems/modules/service/deBaseServiceService.h"


// Event queue item
/////////////////////

class cEvent : public deObject{
public:
	typedef deTObjectReference<cEvent> Ref;
	
	enum eEvents{
		eeRequestResponse,
		eeRequestFailed,
		eeEventReceived
	};
	
	const eEvents type;
	const deService::Ref service;
	const decUniqueID id;
	const deServiceObject::Ref data;
	const bool finished;
	
	cEvent( eEvents atype, const deService::Ref &aservice, const decUniqueID &aid,
	const deServiceObject::Ref &adata, bool afinished ) :
	type( atype ), service( aservice ), id( aid ), data( adata ), finished( afinished ){
	}
};



// Class deServiceManager
///////////////////////////

// Constructor, Destructor
////////////////////////////

deServiceManager::deServiceManager( deEngine &engine ) :
pEngine( engine ){
}

deServiceManager::~deServiceManager(){
}



// Management
///////////////

decStringSet deServiceManager::GetAllSupportedSerices() const{
	const deModuleSystem &modsys = *pEngine.GetModuleSystem();
	const int count = modsys.GetModuleCount();
	decStringSet names;
	int i;
	
	for( i=0; i<count; i++ ){
		const deLoadableModule &loadmod = *modsys.GetModuleAt( i );
		if( loadmod.GetType() != deModuleSystem::emtService ){
			continue;
		}
		if( ! loadmod.IsLoaded() ){
			continue;
		}
		if( ! loadmod.GetModule() ){
			continue;
		}
		
		deBaseServiceModule &srvmod = *( ( deBaseServiceModule* )loadmod.GetModule() );
		names += srvmod.GetSupportedServices();
	}
	
	return names;
}

deService::Ref deServiceManager::CreateService( const char *name ){
	DEASSERT_NOTNULL( name )
	
	const deModuleSystem &modsys = *pEngine.GetModuleSystem();
	const int count = modsys.GetModuleCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deLoadableModule &loadmod = *modsys.GetModuleAt( i );
		if( loadmod.GetType() != deModuleSystem::emtService ){
			continue;
		}
		if( ! loadmod.IsLoaded() ){
			continue;
		}
		if( ! loadmod.GetModule() ){
			continue;
		}
		
		const deService::Ref service( deService::Ref::New( new deService( name ) ) );
		
		deBaseServiceModule * const srvmod = ( deBaseServiceModule* )loadmod.GetModule();
		deBaseServiceService * const peer = srvmod->CreateService( service, name );
		if( ! peer ){
			continue;
		}
		
		service->SetPeerService( srvmod, peer );
		pEngine.GetScriptingSystem()->CreateService( service );
		return service;
	}
	
	DETHROW_INFO( deeInvalidParam, "Named service not supported" );
}

void deServiceManager::QueueRequestResponse( const deService::Ref &service,
const decUniqueID &id, const deServiceObject::Ref &response, bool finished ){
	const deMutexGuard lock( pMutex );
	pEventQueue.Add( new cEvent( cEvent::eeRequestResponse, service, id, response, finished ) );
}

void deServiceManager::QueueRequestFailed( const deService::Ref &service,
const decUniqueID &id, const deServiceObject::Ref &error ){
	const deMutexGuard lock( pMutex );
	pEventQueue.Add( new cEvent( cEvent::eeRequestFailed, service, id, error, true ) );
}

void deServiceManager::QueueEventReceived( const deService::Ref &service,
const deServiceObject::Ref &event ){
	const deMutexGuard lock( pMutex );
	pEventQueue.Add( new cEvent( cEvent::eeEventReceived, service, decUniqueID(), event, true ) );
}

void deServiceManager::ProcessQueuedEvents(){
	decObjectList events;
	{
	const deMutexGuard lock( pMutex );
	events = pEventQueue;
	pEventQueue.RemoveAll();
	}
	
	const int count = events.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const cEvent &event = *( ( cEvent* )pEventQueue.GetAt( 0 ) );
		switch( event.type ){
		case cEvent::eEvents::eeRequestResponse:
			event.service->RequestResponse( event.id, event.data, event.finished );
			break;
			
		case cEvent::eEvents::eeRequestFailed:
			event.service->RequestFailed( event.id, event.data );
			break;
			
		case cEvent::eEvents::eeEventReceived:
			event.service->EventReceived( event.data );
			break;
		}
	}
}
