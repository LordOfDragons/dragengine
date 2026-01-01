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

#include "deScriptingSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/scripting/deBaseScriptingModule.h"
#include "../deEngine.h"
#include "../app/deOS.h"
#include "../common/exceptions.h"
#include "../input/deInputEvent.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../filesystem/deVFSDiskDirectory.h"
#include "../filesystem/deVFSRedirect.h"
#include "../logger/deLogger.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/deResourceManager.h"
#include "../resources/collider/deCollider.h"
#include "../resources/network/deServer.h"
#include "../resources/network/deConnection.h"
#include "../resources/network/deNetworkState.h"
#include "../resources/sensor/deTouchSensor.h"
#include "../resources/sensor/deSoundLevelMeter.h"
#include "../resources/particle/deParticleEmitterInstance.h"
#include "../resources/propfield/dePropField.h"
#include "../resources/sound/deSpeaker.h"
#include "../resources/service/deService.h"



extern const int *vResourcePeerCreationOrder;

#define LOGSOURCE "ScriptSystem"



// class deScriptingSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

deScriptingSystem::deScriptingSystem(deEngine *engine) :
deBaseSystem(engine, "Script", deModuleSystem::emtScript),
pActiveModule(NULL),
pEditMode(false),
pGameRunning(false)
{
}

deScriptingSystem::~deScriptingSystem(){
}



// Management
///////////////

void deScriptingSystem::SetScriptDirectory(const char *scriptDirectory){
	pScriptDirectory = scriptDirectory;
}

void deScriptingSystem::SetScriptVersion(const char *scriptVersion){
	pScriptVersion = scriptVersion;
}

void deScriptingSystem::SetGameObject(const char *gameObject){
	pGameObject = gameObject;
}

void deScriptingSystem::AddVFSSharedDataDir(deVirtualFileSystem &vfs) const{
	if(!pActiveModule){
		return;
	}
	
	const char * const dataDir = pActiveModule->GetSharedDataDir();
	if(!dataDir){
		return;
	}
	
	const char * const vfsPath = pActiveModule->GetVFSSharedDataDir();
	if(!vfsPath){
		return;
	}
	
	const deEngine &engine = *GetEngine();
	deVirtualFileSystem * const osFileSystem = engine.GetOSFileSystem();
	const deLoadableModule &loadableModule = pActiveModule->GetLoadableModule();
	
	const char * const typeName = engine.GetModuleSystem()->GetTypeDirectory(loadableModule.GetType());
	const decString &directoryName = loadableModule.GetDirectoryName();
	const decString &version = loadableModule.GetVersion();
	
	const decPath pathRoot(decPath::CreatePathUnix(vfsPath));
	decPath pathRedirect;
	
	if(osFileSystem){
		pathRedirect.SetFromUnix("/share/modules");
		pathRedirect.AddUnixPath(typeName);
		pathRedirect.AddUnixPath(directoryName);
		pathRedirect.AddComponent(version);
		pathRedirect.AddUnixPath(dataDir);
		
		vfs.AddContainer(deVFSRedirect::Ref::New(pathRoot, pathRedirect, osFileSystem, true));
		
	}else{
		// build disk path
		decPath pathDisk;
		pathDisk.SetFromNative(engine.GetOS()->GetPathShare());
		pathDisk.AddUnixPath("modules");
		pathDisk.AddUnixPath(typeName);
		pathDisk.AddUnixPath(directoryName);
		pathDisk.AddComponent(version);
		pathDisk.AddUnixPath(dataDir);
		
		// add container
		engine.GetLogger()->LogInfoFormat(LOGSOURCE, "Adding disk container '%s' => '%s'(ro)",
			pathDisk.GetPathNative().GetString(), vfsPath);
		
		vfs.AddContainer(deVFSDiskDirectory::Ref::New(pathRoot, pathDisk, true));
	}
	
	// asset libraries
	const deVirtualFileSystem::Ref &vfsAssetLibraries = engine.GetModuleSystem()->GetVFSAssetLibraries();
	if(vfsAssetLibraries->GetContainerCount() > 0){
		pathRedirect.SetFromUnix("/modules");
		pathRedirect.AddUnixPath(typeName);
		pathRedirect.AddUnixPath(directoryName);
		pathRedirect.AddComponent(version);
		pathRedirect.AddUnixPath(dataDir);
		
		vfs.AddContainer(deVFSRedirect::Ref::New(pathRoot, pathRedirect, vfsAssetLibraries, true));
	}
}

void deScriptingSystem::AddVFSContainers(deVirtualFileSystem &vfs, const char *stage){
	if(pActiveModule){
		pActiveModule->AddVFSContainers(vfs, stage);
	}
}

void deScriptingSystem::SetEditMode(bool editMode)
{
	if(GetIsRunning()) DETHROW(deeInvalidParam);
	pEditMode = editMode;
}

void deScriptingSystem::SetActiveModule(deLoadableModule *module){
	deBaseSystem::SetActiveModule(module);
	pActiveModule = (deBaseScriptingModule*)module->GetModule();
}

void deScriptingSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deScriptingSystem::PostStart(){
	if(pScriptDirectory.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	
	deEngine &engine = *GetEngine();
	int i;
	
	if(!pActiveModule->Init(pScriptDirectory, pGameObject)){
		DETHROW(deeInvalidAction);
	}
	
	for(i=0; i<engine.GetResourceManagerCount(); i++){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemScriptingLoad();
	}
}

void deScriptingSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	ExitGame();
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy(pActiveModule);
	
	for(i=engine.GetResourceManagerCount()-1; i>=0; i--){
		engine.GetResourceManagerAt(vResourcePeerCreationOrder[i])->SystemScriptingUnload();
	}
	
	pActiveModule->ShutDown();
}



void deScriptingSystem::InitGame(){
	if(!GetIsRunning() || pGameRunning){
		return;
	}
	
	if(!pActiveModule->InitGame()){
		GetEngine()->SignalScriptFailed();
	}
	pGameRunning = true;
}

void deScriptingSystem::ExitGame(){
	if(!GetIsRunning() || !pGameRunning){
		return;
	}
	
	if(!pActiveModule->ExitGame()){
		GetEngine()->SignalScriptFailed();
	}
	pGameRunning = false;
}

void deScriptingSystem::UserRequestQuit(){
	if(!GetIsRunning() && !pGameRunning){
		return;
	}
	pActiveModule->UserRequestQuit();
}



void deScriptingSystem::LoadCollider(deCollider *collider){
	if(!GetIsRunning()) return;
	if(collider->GetPeerScripting()) DETHROW(deeInvalidParam);
	
	collider->SetPeerScripting(pActiveModule->CreateCollider(collider));
}

void deScriptingSystem::LoadServer(deServer *server){
	if(!GetIsRunning()) return;
	if(server->GetPeerScripting()) DETHROW(deeInvalidParam);
	
	server->SetPeerScripting(pActiveModule->CreateServer(server));
}

void deScriptingSystem::LoadConnection(deConnection *connection){
	if(!GetIsRunning()) return;
	if(connection->GetPeerScripting()) DETHROW(deeInvalidParam);
	
	connection->SetPeerScripting(pActiveModule->CreateConnection(connection));
}

void deScriptingSystem::LoadNetworkState(deNetworkState *state){
	if(!GetIsRunning()) return;
	if(state->GetPeerScripting()) DETHROW(deeInvalidParam);
	
	state->SetPeerScripting(pActiveModule->CreateNetworkState(state));
}

void deScriptingSystem::LoadTouchSensor(deTouchSensor *touchSensor){
	if(!GetIsRunning()) return;
	
	if(touchSensor->GetPeerScripting()) DETHROW(deeInvalidParam);
	
	touchSensor->SetPeerScripting(pActiveModule->CreateTouchSensor(touchSensor));
}

void deScriptingSystem::LoadPropField(dePropField *propField){
	if(!GetIsRunning()) return;
	
	if(propField->GetPeerScripting()) DETHROW(deeInvalidParam);
	
	propField->SetPeerScripting(pActiveModule->CreatePropField(propField));
}

void deScriptingSystem::LoadParticleEmitterInstance(deParticleEmitterInstance *instance){
	if(GetIsRunning() && !instance->GetPeerScripting()){
		instance->SetPeerScripting(pActiveModule->CreateParticleEmitterInstance(instance));
	}
}

void deScriptingSystem::LoadSoundLevelMeter(deSoundLevelMeter *meter){
	if(GetIsRunning() && !meter->GetPeerScripting()){
		meter->SetPeerScripting(pActiveModule->CreateSoundLevelMeter(meter));
	}
}

void deScriptingSystem::LoadSpeaker(deSpeaker *speaker){
	if(GetIsRunning() && !speaker->GetPeerScripting()){
		speaker->SetPeerScripting(pActiveModule->CreateSpeaker(speaker));
	}
}

void deScriptingSystem::CreateService(deService *service){
	if(!GetIsRunning() || service->GetPeerScripting()){
		return;
	}
	
	service->SetPeerScripting(pActiveModule->CreateService(service));
}



void deScriptingSystem::OnFrameUpdate(){
	if(!pActiveModule->OnFrameUpdate()){
		GetEngine()->SignalScriptFailed();
	}
}

void deScriptingSystem::OnResizeRenderWindow(){
	if(GetIsRunning()){
		if(!pActiveModule->OnResizeRenderWindow()){
			GetEngine()->SignalScriptFailed();
		}
	}
}

void deScriptingSystem::OnAppActivate(){
	if(GetIsRunning()){
		if(!pActiveModule->OnAppActivate()){
			GetEngine()->SignalScriptFailed();
		}
	}
}

void deScriptingSystem::SendEvent(deInputEvent *event){
	if(!pActiveModule->SendEvent(event)){
		GetEngine()->SignalScriptFailed();
	}
}
