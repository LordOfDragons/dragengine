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

#include "deConnection.h"
#include "deConnectionManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deNetworkSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/network/deBaseNetworkConnection.h"
#include "../../systems/modules/scripting/deBaseScriptingConnection.h"
#include "../../common/exceptions.h"



// Class deConnectionManager
//////////////////////////////

// Constructor, destructor
////////////////////////////

deConnectionManager::deConnectionManager( deEngine *engine ) :
deResourceManager( engine, ertConnection ){
	SetLoggingName( "connection" );
}

deConnectionManager::~deConnectionManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deConnectionManager::GetConnectionCount() const{
	return pConnections.GetCount();
}

deConnection *deConnectionManager::GetRootConnection() const{
	return ( deConnection* )pConnections.GetRoot();
}

deConnection *deConnectionManager::CreateConnection(){
	deConnection *connection = NULL;
	
	try{
		connection = new deConnection( this );
		
		GetNetworkSystem()->LoadConnection( connection );
		GetScriptingSystem()->LoadConnection( connection );
		
		pConnections.Add( connection );
		
	}catch( const deException & ){
		if( connection ){
			connection->FreeReference();
		}
		throw;
	}
	
	return connection;
}



void deConnectionManager::ReleaseLeakingResources(){
	const int count = GetConnectionCount();
	if( count > 0 ){
		LogWarnFormat( "%i leaking connections", count );
		pConnections.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deConnectionManager::SystemNetworkLoad(){
	deConnection *connection = ( deConnection* )pConnections.GetRoot();
	deNetworkSystem &netSys = *GetNetworkSystem();
	
	while( connection ){
		if( ! connection->GetPeerNetwork() ){
			netSys.LoadConnection( connection );
		}
		
		connection = ( deConnection* )connection->GetLLManagerNext();
	}
}

void deConnectionManager::SystemNetworkUnload(){
	deConnection *connection = ( deConnection* )pConnections.GetRoot();
	
	while( connection ){
		connection->SetPeerNetwork( NULL );
		connection = ( deConnection* )connection->GetLLManagerNext();
	}
}

void deConnectionManager::SystemScriptingLoad(){
	deConnection *connection = ( deConnection* )pConnections.GetRoot();
	deScriptingSystem &scrSys = *GetScriptingSystem();
	
	while( connection ){
		if( ! connection->GetPeerScripting() ){
			scrSys.LoadConnection( connection );
		}
		
		connection = ( deConnection* )connection->GetLLManagerNext();
	}
}

void deConnectionManager::SystemScriptingUnload(){
	deConnection *connection = ( deConnection* )pConnections.GetRoot();
	
	while( connection ){
		connection->SetPeerScripting( NULL );
		connection = ( deConnection* )connection->GetLLManagerNext();
	}
}

void deConnectionManager::RemoveResource( deResource *resource ){
	pConnections.RemoveIfPresent( resource );
}
