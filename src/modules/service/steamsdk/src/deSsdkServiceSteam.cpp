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

#include "deSsdkServiceSteam.h"
#include "deSteamSdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/resources/service/deServiceObject.h>


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
}


// Management
///////////////

void deSsdkServiceSteam::StartRequest(const decUniqueID &id, const deServiceObject& request ){
	deServiceManager &srvmgr = *pModule.GetGameEngine()->GetServiceManager();
	const deServiceObject::Ref so( deServiceObject::Ref::New( new deServiceObject ) );
	so->AddChild( "error", deServiceObject::Ref::New( deServiceObject::NewString( "invalidRequest" ) ) );
	srvmgr.QueueRequestFailed( deService::Ref( pService ), id, so );
}

void deSsdkServiceSteam::CancelRequest( const decUniqueID &id ){
}
