/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deServer.h"
#include "deServerManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deNetworkSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/network/deBaseNetworkServer.h"
#include "../../systems/modules/scripting/deBaseScriptingServer.h"



// Class deServerManager
//////////////////////////

// Constructor, destructor
////////////////////////////

deServerManager::deServerManager( deEngine *engine ) :
deResourceManager( engine, ertServer ){
	SetLoggingName( "server" );
}

deServerManager::~deServerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deServerManager::GetServerCount() const{
	return pServers.GetCount();
}

deServer *deServerManager::GetRootServer() const{
	return ( deServer* )pServers.GetRoot();
}

deServer *deServerManager::CreateServer(){
	deServer *server = NULL;
	
	try{
		server = new deServer( this );
		
		GetNetworkSystem()->LoadServer( server );
		GetScriptingSystem()->LoadServer( server );
		
		pServers.Add( server );
		
	}catch( const deException & ){
		if( server ){
			server->FreeReference();
		}
		throw;
	}
	
	return server;
}



void deServerManager::ReleaseLeakingResources(){
	const int count = GetServerCount();
	if( count > 0 ){
		LogWarnFormat( "%i leaking servers", count );
		pServers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deServerManager::SystemNetworkLoad(){
	deServer *server = ( deServer* )pServers.GetRoot();
	deNetworkSystem &netSys = *GetNetworkSystem();
	
	while( server ){
		if( ! server->GetPeerNetwork() ){
			netSys.LoadServer( server );
		}
		
		server = ( deServer* )server->GetLLManagerNext();
	}
}

void deServerManager::SystemNetworkUnload(){
	deServer *server = ( deServer* )pServers.GetRoot();
	
	while( server ){
		server->SetPeerNetwork( NULL );
		server = ( deServer* )server->GetLLManagerNext();
	}
}

void deServerManager::SystemScriptingLoad(){
	deServer *server = ( deServer* )pServers.GetRoot();
	deScriptingSystem &scrSys = *GetScriptingSystem();
	
	while( server ){
		if( ! server->GetPeerScripting() ){
			scrSys.LoadServer( server );
		}
		
		server = ( deServer* )server->GetLLManagerNext();
	}
}

void deServerManager::SystemScriptingUnload(){
	deServer *server = ( deServer* )pServers.GetRoot();
	
	while( server ){
		server->SetPeerScripting( NULL );
		server = ( deServer* )server->GetLLManagerNext();
	}
}

void deServerManager::RemoveResource( deResource *resource ){
	pServers.RemoveIfPresent( resource );
}
