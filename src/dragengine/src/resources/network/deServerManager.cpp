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

deServerManager::deServerManager(deEngine *engine) :
deResourceManager(engine, ertServer){
	SetLoggingName("server");
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
	return (deServer*)pServers.GetRoot();
}

deServer *deServerManager::CreateServer(){
	deServer *server = NULL;
	
	try{
		server = new deServer(this);
		
		GetNetworkSystem()->LoadServer(server);
		GetScriptingSystem()->LoadServer(server);
		
		pServers.Add(server);
		
	}catch(const deException &){
		if(server){
			server->FreeReference();
		}
		throw;
	}
	
	return server;
}



void deServerManager::ReleaseLeakingResources(){
	const int count = GetServerCount();
	if(count > 0){
		LogWarnFormat("%i leaking servers", count);
		pServers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deServerManager::SystemNetworkLoad(){
	deServer *server = (deServer*)pServers.GetRoot();
	deNetworkSystem &netSys = *GetNetworkSystem();
	
	while(server){
		if(! server->GetPeerNetwork()){
			netSys.LoadServer(server);
		}
		
		server = (deServer*)server->GetLLManagerNext();
	}
}

void deServerManager::SystemNetworkUnload(){
	deServer *server = (deServer*)pServers.GetRoot();
	
	while(server){
		server->SetPeerNetwork(NULL);
		server = (deServer*)server->GetLLManagerNext();
	}
}

void deServerManager::SystemScriptingLoad(){
	deServer *server = (deServer*)pServers.GetRoot();
	deScriptingSystem &scrSys = *GetScriptingSystem();
	
	while(server){
		if(! server->GetPeerScripting()){
			scrSys.LoadServer(server);
		}
		
		server = (deServer*)server->GetLLManagerNext();
	}
}

void deServerManager::SystemScriptingUnload(){
	deServer *server = (deServer*)pServers.GetRoot();
	
	while(server){
		server->SetPeerScripting(NULL);
		server = (deServer*)server->GetLLManagerNext();
	}
}

void deServerManager::RemoveResource(deResource *resource){
	pServers.RemoveIfPresent(resource);
}
