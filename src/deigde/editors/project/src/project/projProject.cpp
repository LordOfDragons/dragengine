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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "projProject.h"
#include "projProjectListener.h"
#include "profile/projProfile.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Project"



// Class projProject
//////////////////////////

// Constructor, destructor
////////////////////////////

projProject::projProject(igdeEnvironment *environment) :
igdeEditableEntity(environment),

pScriptDirectory("/scripts"),
pGameObject("MyGameApp"),
pPathConfig("/config"),
pPathCapture("/capture"),

pActiveProfile(nullptr),
pRemoteServer(std::make_shared<projRemoteServer>(*this, *environment))
{
	try{
		decPath path;
		path.SetFromNative(environment->GetGameProject()->GetDirectoryPath());
		path.AddComponent("project.xml");
		SetFilePath(path.GetPathNative());
		SetSaved(true);
		SetChanged(false);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

projProject::~projProject(){
	pCleanUp();
}



// Management
///////////////

void projProject::SetScriptDirectory(const char *directory){
	if(pScriptDirectory == directory){
		return;
	}
	
	pScriptDirectory = directory;
	NotifyProjectChanged();
}

void projProject::SetGameObject(const char *gameObject){
	if(pGameObject == gameObject){
		return;
	}
	
	pGameObject = gameObject;
	NotifyProjectChanged();
}

void projProject::SetPathConfig(const char *path){
	if(pPathConfig == path){
		return;
	}
	
	pPathConfig = path;
	NotifyProjectChanged();
}

void projProject::SetPathCapture(const char *path){
	if(pPathCapture == path){
		return;
	}
	
	pPathCapture = path;
	NotifyProjectChanged();
}



// Profiles
/////////////

void projProject::AddProfile(projProfile *profile){
	pProfiles.Add(profile);
	profile->SetProject(this);
	NotifyProfileStructureChanged();
	
	if(! pActiveProfile){
		SetActiveProfile(profile);
	}
}

void projProject::RemoveProfile(projProfile *profile){
	if(! profile || profile->GetProject() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(profile == pActiveProfile){
		if(pProfiles.GetCount() == 1){
			SetActiveProfile(NULL);
			
		}else{
			if(pProfiles.GetAt(0) == profile){
				SetActiveProfile(pProfiles.GetAt(1));
				
			}else{
				SetActiveProfile(pProfiles.GetAt(0));
			}
		}
	}
	
	profile->SetProject(NULL);
	pProfiles.Remove(profile);
	NotifyProfileStructureChanged();
}

void projProject::RemoveAllProfiles(){
	const int count = pProfiles.GetCount();
	int i;
	
	SetActiveProfile(NULL);
	
	for(i=0; i<count; i++){
		pProfiles.GetAt(i)->SetProject(NULL);
	}
	pProfiles.RemoveAll();
	NotifyProfileStructureChanged();
}



void projProject::SetActiveProfile(projProfile *profile){
	if(profile == pActiveProfile){
		return;
	}
	
	if(pActiveProfile){
		pActiveProfile->FreeReference();
	}
	
	pActiveProfile = profile;
	
	if(profile){
		profile->AddReference();
	}
	
	NotifyActiveProfileChanged();
}



void projProject::NotifyProfileStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->ProfileStructureChanged(this);
	}
	
	SetChanged(true);
	
	pRemoteServer->OnProfileStructureChanged();
}

void projProject::NotifyProfileChanged(projProfile *profile){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->ProfileChanged(this, profile);
	}
	
	SetChanged(true);
	
	pRemoteServer->OnProfileChanged(profile);
}

void projProject::NotifyProfileNameChanged(projProfile *profile){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->ProfileNameChanged(this, profile);
	}
	
	SetChanged(true);
	
	pRemoteServer->OnProfileChanged(profile);
}

void projProject::NotifyActiveProfileChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->ActiveProfileChanged(this);
	}
	
	pRemoteServer->OnActiveProfileChanged();
}

void projProject::NotifyRemoteClientConnected(const projRemoteClient::Ref &client){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->RemoteClientConnected(this, client);
	}
}


// Test running
/////////////////

void projProject::SetActiveLaunchProfile(const char *profile){
	if(profile == pActiveLaunchProfile){
		return;
	}
	
	pActiveLaunchProfile = profile;
	
	NotifyActiveLaunchProfileChanged();
}



void projProject::NotifyActiveLaunchProfileChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->ActiveLaunchProfileChanged(this);
	}
	
	SetChanged(true);
}



// Listeners
//////////////

void projProject::AddListener(projProjectListener *listener){
	pListeners.Add(listener);
}

void projProject::RemoveListener(projProjectListener *listener){
	pListeners.Remove(listener);
}



void projProject::NotifyStateChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->StateChanged(this);
	}
}

void projProject::NotifyUndoChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->UndoChanged(this);
	}
}

void projProject::NotifyProjectChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projProjectListener*)pListeners.GetAt(i))->ProjectChanged(this);
	}
	
	SetChanged(true);
}



// Private Functions
//////////////////////

void projProject::pCleanUp(){
	pListeners.RemoveAll();
	
	GetUndoSystem()->RemoveAll();
	
	RemoveAllProfiles();
	
	if(pRemoteServer){
		try{
			pRemoteServer->StopListenClientConnections();
		}catch(...){
		}
		pRemoteServer.reset();
	}
}
