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

deConnectionManager::deConnectionManager(deEngine *engine) :
deResourceManager(engine, ertConnection){
	SetLoggingName("connection");
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
	return (deConnection*)pConnections.GetRoot();
}

deConnection::Ref deConnectionManager::CreateConnection(){
	const deConnection::Ref connection(deConnection::Ref::New(this));
	GetNetworkSystem()->LoadConnection(connection);
	GetScriptingSystem()->LoadConnection(connection);
	pConnections.Add(connection);
	return connection;
}



void deConnectionManager::ReleaseLeakingResources(){
	const int count = GetConnectionCount();
	if(count > 0){
		LogWarnFormat("%i leaking connections", count);
		pConnections.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deConnectionManager::SystemNetworkLoad(){
	deConnection *connection = (deConnection*)pConnections.GetRoot();
	deNetworkSystem &netSys = *GetNetworkSystem();
	
	while(connection){
		if(!connection->GetPeerNetwork()){
			netSys.LoadConnection(connection);
		}
		
		connection = (deConnection*)connection->GetLLManagerNext();
	}
}

void deConnectionManager::SystemNetworkUnload(){
	deConnection *connection = (deConnection*)pConnections.GetRoot();
	
	while(connection){
		connection->SetPeerNetwork(NULL);
		connection = (deConnection*)connection->GetLLManagerNext();
	}
}

void deConnectionManager::SystemScriptingLoad(){
	deConnection *connection = (deConnection*)pConnections.GetRoot();
	deScriptingSystem &scrSys = *GetScriptingSystem();
	
	while(connection){
		if(!connection->GetPeerScripting()){
			scrSys.LoadConnection(connection);
		}
		
		connection = (deConnection*)connection->GetLLManagerNext();
	}
}

void deConnectionManager::SystemScriptingUnload(){
	deConnection *connection = (deConnection*)pConnections.GetRoot();
	
	while(connection){
		connection->SetPeerScripting(NULL);
		connection = (deConnection*)connection->GetLLManagerNext();
	}
}

void deConnectionManager::RemoveResource(deResource *resource){
	pConnections.RemoveIfPresent(resource);
}
