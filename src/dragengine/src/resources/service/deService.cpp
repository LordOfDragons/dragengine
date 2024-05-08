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

#include "deService.h"
#include "deServiceObject.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/service/deBaseServiceService.h"
#include "../../systems/modules/scripting/deBaseScriptingService.h"



// Class deService
////////////////////

// Constructor, destructor
////////////////////////////

deService::deService( deBaseServiceModule &module, const char *name ) :
pModule( module ),
pName( name ),
pPeerService( nullptr ),
pPeerScripting( nullptr ){
}

deService::~deService(){
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	if( pPeerService ){
		delete pPeerService;
	}
}



// Management
///////////////

void deService::StartRequest( const decUniqueID &id, const deServiceObject &request ){
	DEASSERT_NOTNULL( pPeerService )
	
	pPeerService->StartRequest( id, request );
}

void deService::CancelRequest( const decUniqueID &id ){
	DEASSERT_NOTNULL( pPeerService )
	
	pPeerService->CancelRequest( id );
}

void deService::RequestResponse( const decUniqueID &id, deServiceObject *response, bool finished ){
	DEASSERT_NOTNULL( response )
	
	if( pPeerScripting ){
		pPeerScripting->RequestResponse( id, response, finished );
		
	}else{
		response->FreeReference();
	}
}

void deService::RequestFailed( const decUniqueID &id, deServiceObject *error ){
	DEASSERT_NOTNULL( error )
	
	if( pPeerScripting ){
		pPeerScripting->RequestFailed( id, error );
		
	}else{
		error->FreeReference();
	}
}

void deService::EventReceived( deServiceObject *event ){
	DEASSERT_NOTNULL( event )
	
	if( pPeerScripting ){
		pPeerScripting->EventReceived( event );
		
	}else{
		event->FreeReference();
	}
}



// System Peers
/////////////////

void deService::SetPeerService( deBaseServiceService *peer ){
	if( peer == pPeerService ){
		return;
	}
	
	if( pPeerService ){
		delete pPeerService;
	}
	
	pPeerService = peer;
}

void deService::SetPeerScripting( deBaseScriptingService *peer ){
	if( peer == pPeerScripting ){
		return;
	}
	
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	
	pPeerScripting = peer;
}
