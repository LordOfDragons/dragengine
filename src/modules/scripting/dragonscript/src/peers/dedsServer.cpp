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

#include <libdscript/libdscript.h>
#include <stdio.h>
#include <stdlib.h>

#include "dedsServer.h"
#include "../deScriptingDragonScript.h"
#include "../classes/network/deClassServer.h"
#include "../classes/network/deClassConnection.h"
#include "../classes/network/deClassServerListener.h"

#include <dragengine/resources/network/deServer.h>
#include <dragengine/resources/network/deConnection.h>



// Class dedsServer
/////////////////////

// Constructor, destructor
////////////////////////////

dedsServer::dedsServer( deScriptingDragonScript *ds, deServer *server ){
	if( ! ds || ! server ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	pServer = server;
	pHasCB = false;
	pValCB = ds->GetScriptEngine()->GetMainRunTime()->CreateValue( pDS->GetClassServerListener() );
}

dedsServer::~dedsServer(){
	if( ! pValCB ){
		return;
	}
	
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if( pServer && pServer->GetRefCount() > 0 ){
		pDS->AddValueDeleteLater( pValCB );
		
	}else{
		pDS->GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
	}
	
	pValCB = nullptr;
	pHasCB = false;
}



// Management
///////////////

dsRealObject *dedsServer::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsServer::SetCallback( dsRealObject *object ){
	if( ! pValCB ){
		return;
	}
	
	dsRunTime &rt = *pDS->GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCB, object );
		pHasCB = true;
		
	}else{
		rt.SetNull( pValCB, pDS->GetClassServerListener() );
		pHasCB = false;
	}
}



// Notifications
//////////////////

void dedsServer::ClientConnected( deConnection *connection ){
	if( ! pHasCB ){
		connection->Disconnect();
		return;
	}
	
	dsRunTime * const rt = pDS->GetScriptEngine()->GetMainRunTime();
	deClassConnection &clsCon = *pDS->GetClassConnection();
	
	try{
		clsCon.PushConnection( rt, connection );
		rt->RunFunction( pValCB, "clientConnected", 1 );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
		connection->Disconnect();
	}
}
