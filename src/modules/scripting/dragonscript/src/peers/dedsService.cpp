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

#include "dedsService.h"
#include "../deScriptingDragonScript.h"
#include "../classes/service/deClassService.h"
#include "../classes/service/deClassServiceListener.h"
#include "../classes/service/deClassServiceObject.h"
#include "../classes/utils/deClassUniqueID.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/resources/service/deService.h>



// Class dedsService
//////////////////////

// Constructor, destructor
////////////////////////////

dedsService::dedsService( deScriptingDragonScript &ds, deService *service ) :
pDS( ds ),
pService( service ),
pValOwner( NULL ),
pValCB( NULL ),
pHasCB( false )
{
	if( ! service ){
		DSTHROW( dueInvalidParam );
	}
	
	dsRunTime &rt = *ds.GetScriptEngine()->GetMainRunTime();
	
	pValOwner = rt.CreateValue();
	pValCB = rt.CreateValue( pDS.GetClassServiceListener() );
}

dedsService::~dedsService(){
	// check if the service resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the service resource
	// being deleted due to links breaking while freeing the value. if this is the
	// case delay the deletion until a safe time
	if( pService && pService->GetRefCount() > 0 ){
		if( pValCB ){
			pDS.AddValueDeleteLater( pValCB );
			pValCB = NULL;
			pHasCB = false;
		}
		
		if( pValOwner ){
			pDS.AddValueDeleteLater( pValOwner );
			pValOwner = NULL;
		}
		
	}else{
		if( pValCB ){
			pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
			pValCB = NULL;
			pHasCB = false;
		}
		
		if( pValOwner ){
			pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValOwner );
			pValOwner = NULL;
		}
	}
}



// Management
///////////////

dsRealObject *dedsService::GetOwner() const{
	return pValOwner->GetRealObject();
}

void dedsService::SetOwner( dsRealObject *object ){
	if( ! pValOwner ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValOwner, object );
		
	}else{
		rt.ClearValue( pValOwner );
	}
}



dsRealObject *dedsService::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsService::SetCallback( dsRealObject *object ){
	if( ! pValCB ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCB, object );
		rt.CastValueTo( pValCB, pValCB, pDS.GetClassServiceListener() );
		pHasCB = true;
		
	}else{
		rt.SetNull( pValCB, pDS.GetClassServiceListener() );
		pHasCB = false;
	}
}



void dedsService::RequestResponse( const decUniqueID &id,
const deServiceObject::Ref &response, bool finished ){
	if( ! pHasCB ){
		if( response ){
			response->FreeReference();
		}
		return;
	}
	
	if( ! response ){
		DSTHROW( dueInvalidParam );
	}
	
	const int funcIndex = pDS.GetClassServiceListener()->GetFuncIndexRequestResponse();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	
	try{
		rt->PushBool( finished ); // finished
		pDS.GetClassServiceObject()->PushServiceObject( rt, response ); // response
		pDS.GetClassUniqueID()->PushUniqueID( rt, id ); // id
		pDS.GetClassService()->PushService( rt, pService ); // service
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsService::RequestFailed( const decUniqueID &id, const deServiceObject::Ref &error ){
	if( ! pHasCB ){
		if( error ){
			error->FreeReference();
		}
		return;
	}
	
	if( ! error ){
		DSTHROW( dueInvalidParam );
	}
	
	const int funcIndex = pDS.GetClassServiceListener()->GetFuncIndexRequestFailed();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	
	try{
		pDS.GetClassServiceObject()->PushServiceObject( rt, error ); // error
		pDS.GetClassUniqueID()->PushUniqueID( rt, id ); // id
		pDS.GetClassService()->PushService( rt, pService ); // service
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsService::EventReceived( const deServiceObject::Ref &event ){
	if( ! pHasCB ){
		if( event ){
			event->FreeReference();
		}
		return;
	}
	
	if( ! event ){
		DSTHROW( dueInvalidParam );
	}
	
	const int funcIndex = pDS.GetClassServiceListener()->GetFuncIndexEventReceived();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	
	try{
		pDS.GetClassServiceObject()->PushServiceObject( rt, event ); // event
		pDS.GetClassService()->PushService( rt, pService ); // service
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}
