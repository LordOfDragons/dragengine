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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dedsConnection.h"
#include "../deScriptingDragonScript.h"
#include "../classes/network/deClassConnection.h"
#include "../classes/network/deClassNetworkState.h"
#include "../classes/network/deClassNetworkMessage.h"
#include "../classes/network/deClassConnectionListener.h"

#include <dragengine/resources/network/deConnection.h>
#include <dragengine/resources/network/deNetworkState.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/deNetworkMessageReference.h>

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>



// Class dedsConnection
/////////////////////////

// Constructor, destructor
////////////////////////////

dedsConnection::dedsConnection( deScriptingDragonScript &ds, deConnection *connection ) :
pDS( ds ),
pConnection( connection ),
pValCB( NULL ),
pHasCB( false )
{
	if( ! connection ){
		DSTHROW( dueInvalidParam );
	}
	
	pValCB = ds.GetScriptEngine()->GetMainRunTime()->CreateValue( ds.GetClassConnectionListener() );
}

dedsConnection::~dedsConnection(){
	if( ! pValCB ){
		return;
	}
	
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if( pConnection && pConnection->GetRefCount() > 0 ){
		pDS.AddValueDeleteLater( pValCB );
		
	}else{
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
	}
	
	pValCB = NULL;
	pHasCB = false;
}



// Management
///////////////

void dedsConnection::ConnectionClosed(){
	if( pHasCB ){
		dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
		
		try{
			rt.RunFunction( pValCB, "connectionClosed", 0 );
			
		}catch( const duException &e ){
			rt.PrintExceptionTrace();
			e.PrintError();
		}
	}
}

void dedsConnection::MessageProgress( int bytesReceived ){
	if( pHasCB ){
		dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
		
		try{
			rt.PushInt( bytesReceived ); // bytesReceived
			rt.RunFunction( pValCB, "messageProgress", 1 );
			
		}catch( const duException &e ){
			rt.PrintExceptionTrace();
			e.PrintError();
		}
	}
}

void dedsConnection::MessageReceived( deNetworkMessage *message ){
	if( ! pHasCB || ! message ){
		return;
	}
	
	const int funcIndex = pDS.GetClassConnectionListener()->GetFuncIndexMessageReceived();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassNetworkMessage &clsNM = *pDS.GetClassNetworkMessage();
	
	try{
		clsNM.PushNetworkMessage( rt, message );
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

bool dedsConnection::LinkState( deNetworkState *networkState, deNetworkMessage *message ){
	if( ! networkState || ! message || message->GetDataLength() < 1 ){
		return false;
	}
	
	const int funcIndex = pDS.GetClassConnectionListener()->GetFuncIndexLinkState();
	
	if( pHasCB ){
		dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
		deClassNetworkMessage &clsNM = *pDS.GetClassNetworkMessage();
		deClassNetworkState &clsNS = *pDS.GetClassNetworkState();
		
		try{
			clsNM.PushNetworkMessage( rt, message );
			clsNS.PushNetworkState( rt, networkState );
			rt->RunFunctionFast( pValCB, funcIndex );
			return rt->GetReturnBool();
			
		}catch( const duException &e ){
			rt->PrintExceptionTrace();
			e.PrintError();
		}
	}
	
	return false;
}



// DS Internal Stuff
//////////////////////

dsRealObject *dedsConnection::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsConnection::SetCallback( dsRealObject *cbObj ){
	if( pValCB ){
		dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
		
		if( cbObj ){
			rt.SetObject( pValCB, cbObj );
			rt.CastValueTo( pValCB, pValCB, pDS.GetClassConnectionListener() );
			pHasCB = true;
			
		}else{
			rt.SetNull( pValCB, pDS.GetClassConnectionListener() );
			pHasCB = false;
		}
	}
}
