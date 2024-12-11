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

#include "projRemoteServer.h"
#include "projRemoteServerLogger.h"
#include "projRemoteServerThread.h"
#include "projRemoteClient.h"
#include "../projProject.h"
#include "../profile/projProfile.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/common/file/decPath.h>



// Class projRemoteServer
///////////////////////////

// Constructor, destructor
////////////////////////////

projRemoteServer::projRemoteServer(projProject &project, igdeEnvironment &environment) :
pProject(project),
pThreadUpdate(nullptr)
{
	SetLogger(std::make_shared<projRemoteServerLogger>("RemoteServer", *environment.GetLogger()));
	SetPathDataDir(environment.GetGameProject()->GetDirectoryPath().GetString());
}

projRemoteServer::~projRemoteServer(){
}

// Management
///////////////

derlRemoteClient::Ref projRemoteServer::CreateClient(const derlRemoteClientConnection::Ref &connection){
	return std::make_shared<projRemoteClient>(pProject, *this, connection);
}

void projRemoteServer::ListenForClientConnections(const decString &address){
	StopListenClientConnections();
	
	try{
		ListenOn(address.GetString());
		
	}catch(const std::exception &e){
		DETHROW_INFO(deeInvalidParam, e.what());
	}
	
	pThreadUpdate = new projRemoteServerThread(*this);
	pThreadUpdate->Start();
}

void projRemoteServer::StopListenClientConnections(){
	pExitThread();
}

void projRemoteServer::RemoteSynchronizeAllClients(){
	for(const derlRemoteClient::Ref &each : GetClients()){
		std::static_pointer_cast<projRemoteClient>(each)->Synchronize();
	}
}

projRemoteServer::TaskProfileData::Ref projRemoteServer::GetTaskProfileData(){
	const std::lock_guard<std::mutex> guard(GetMutex());
	if(!pTaskProfileData){
		pUpdateTaskProfileData();
	}
	return pTaskProfileData;
}


void projRemoteServer::OnProfileStructureChanged(){
	const std::lock_guard<std::mutex> guard(GetMutex());
	pDropProfileData();
}

void projRemoteServer::OnProfileChanged(projProfile *profile){
	if(pProject.GetActiveProfile() != profile){
		return;
	}
	
	const std::lock_guard<std::mutex> guard(GetMutex());
	pDropProfileData();
}

void projRemoteServer::OnActiveProfileChanged(){
	const std::lock_guard<std::mutex> guard(GetMutex());
	pDropProfileData();
}


// Private Functions
//////////////////////

void projRemoteServer::pExitThread(){
	if(!pThreadUpdate){
		return;
	}
	
	pThreadUpdate->ExitThread();
	pThreadUpdate->WaitForExit();
	delete pThreadUpdate;
	pThreadUpdate = nullptr;
}

void projRemoteServer::pUpdateTaskProfileData(){
	igdeEnvironment &env = *pProject.GetEnvironment();
	
	pTaskProfileData = std::make_shared<TaskProfileData>();
	
	pTaskProfileData->vfs = env.GetFileSystemGame();
	
	const igdeGameDefinitionList &list = env.GetGameProject()->GetBaseGameDefinitionList();
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGameDefinition &gameDef = *list.GetAt(i);
		if(gameDef.GetVFSPath().IsEmpty()){
			continue;
		}
		
		pTaskProfileData->excludeBaseGameDefPath.AddIfAbsent(
			decPath::CreatePathUnix(gameDef.GetVFSPath()));
	}
	
	if(pProject.GetActiveProfile()){
		pTaskProfileData->excludePatterns = pProject.GetActiveProfile()->GetExcludePatterns();
	}
}

void projRemoteServer::pDropProfileData(){
	pTaskProfileData.reset();
	
	for(const derlRemoteClient::Ref &each : GetClients()){
		std::static_pointer_cast<projRemoteClient>(each)->DropProfileData();
	}
}
