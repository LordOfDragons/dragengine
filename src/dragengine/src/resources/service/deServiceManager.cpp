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

#include "deServiceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/utils/decUniqueID.h"
#include "../../threading/deMutexGuard.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/service/deBaseServiceModule.h"
#include "../../systems/modules/service/deBaseServiceService.h"


// Event queue item
/////////////////////

class cEvent : public deObject{
public:
	typedef deTObjectReference<cEvent> Ref;
	
	enum eEvents{
		eeRequestResponse,
		eeRequestFailed,
		eeEventReceived
	};
	
	const eEvents type;
	deService *service;
	const decUniqueID id;
	const deServiceObject::Ref data;
	const bool finished;
	
	cEvent(eEvents atype, deService *aservice, const decUniqueID &aid,
	const deServiceObject::Ref &adata, bool afinished) :
	type(atype), service(aservice), id(aid), data(adata), finished(afinished){
	}
};



// Class deServiceManager
///////////////////////////

// Constructor, Destructor
////////////////////////////

deServiceManager::deServiceManager(deEngine *engine) :
deResourceManager(engine, ertService),
pDirtyModules(false)
{
	SetLoggingName("service");
}

deServiceManager::~deServiceManager(){
}



// Management
///////////////

int deServiceManager::GetServiceCount() const{
	return pServices.GetCount();
}

deService *deServiceManager::GetRootService() const{
	return (deService*)pServices.GetRoot();
}

void deServiceManager::ReleaseLeakingResources(){
	const int count = GetServiceCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking services", count);
		pServices.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



decStringSet deServiceManager::GetAllSupportedSerices() const{
	const deModuleSystem &modsys = *GetEngine()->GetModuleSystem();
	const int count = modsys.GetModuleCount();
	decStringSet names;
	int i;
	
	for(i=0; i<count; i++){
		const deLoadableModule &loadmod = *modsys.GetModuleAt(i);
		if(loadmod.GetType() != deModuleSystem::emtService){
			continue;
		}
		if(!loadmod.IsLoaded()){
			continue;
		}
		if(!loadmod.GetModule()){
			continue;
		}
		
		deBaseServiceModule &srvmod = *((deBaseServiceModule*)loadmod.GetModule());
		names += srvmod.GetSupportedServices();
	}
	
	return names;
}

deService *deServiceManager::CreateService(const char *name, const deServiceObject::Ref &data){
	DEASSERT_NOTNULL(name)
	
	deEngine * const engine = GetEngine();
	const deModuleSystem &modsys = *engine->GetModuleSystem();
	const int count = modsys.GetModuleCount();
	int i;
	
	for(i=0; i<count; i++){
		const deLoadableModule &loadmod = *modsys.GetModuleAt(i);
		if(loadmod.GetType() != deModuleSystem::emtService){
			continue;
		}
		if(!loadmod.IsLoaded()){
			continue;
		}
		if(!loadmod.GetModule()){
			continue;
		}
		
		const deService::Ref service(deService::Ref::NewWith(this, name));
		
		deBaseServiceModule * const srvmod = (deBaseServiceModule*)loadmod.GetModule();
		deBaseServiceService * const peer = srvmod->CreateService(service, name, data);
		if(!peer){
			continue;
		}
		
		service->SetPeerService(srvmod, peer);
		engine->GetScriptingSystem()->CreateService(service);
		pServices.Add(service);
		pDirtyModules = true;
		service->AddReference(); // caller takes over reference
		return service;
	}
	
	DETHROW_INFO(deeInvalidParam, "Named service not supported");
}

void deServiceManager::QueueRequestResponse(deService *service, const decUniqueID &id,
const deServiceObject::Ref &response, bool finished){
	const deMutexGuard lock(pMutex);
	pEventQueue.Add(cEvent::Ref::NewWith(cEvent::eeRequestResponse, service, id, response, finished));
}

void deServiceManager::QueueRequestFailed(deService *service, const decUniqueID &id,
const deServiceObject::Ref &error){
	const deMutexGuard lock(pMutex);
	pEventQueue.Add(cEvent::Ref::NewWith(cEvent::eeRequestFailed, service, id, error, true));
}

void deServiceManager::QueueEventReceived(deService *service, const deServiceObject::Ref &event){
	const deMutexGuard lock(pMutex);
	pEventQueue.Add(cEvent::Ref::NewWith(cEvent::eeEventReceived, service, decUniqueID(), event, true));
}

void deServiceManager::RemoveAllMatchingEvents(deService *service){
	const deMutexGuard lock(pMutex);
	const decObjectList events(pEventQueue);
	const int count = events.GetCount();
	int i;
	
	pEventQueue.RemoveAll();
	
	for(i=0; i<count; i++){
		cEvent * const event = (cEvent*)events.GetAt(i);
		if(event->service != service){
			pEventQueue.Add(event);
		}
	}
}

void deServiceManager::FrameUpdate(){
	pUpdateModuleList();
	pUpdateModules();
	pProcessEvents();
}



void deServiceManager::SystemScriptingLoad(){
	deService *service = (deService*)pServices.GetRoot();
	
	while(service){
		if(!service->GetPeerScripting()){
			GetScriptingSystem()->CreateService(service);
		}
		
		service = (deService*) service->GetLLManagerNext();
	}
}

void deServiceManager::SystemScriptingUnload(){
	deService *service = (deService*)pServices.GetRoot();
	
	while(service){
		service->SetPeerScripting(nullptr);
		service = (deService*) service->GetLLManagerNext();
	}
}
void deServiceManager::RemoveResource(deResource *resource){
	pServices.RemoveIfPresent(resource);
	pDirtyModules = true;
}



// Private Functions
//////////////////////

void deServiceManager::pUpdateModuleList(){
	if(!pDirtyModules){
		return;
	}
	
	pDirtyModules = false;
	pModules.RemoveAll();
	
	const deService *service = (const deService*)pServices.GetRoot();
	while(service){
		pModules.AddIfAbsent(service->GetServiceModule());
		service = (const deService*)service->GetLLManagerNext();
	}
}

void deServiceManager::pUpdateModules(){
	const int count = pModules.GetCount();
	if(count == 0){
		return;
	}
	
	const float elapsed = GetEngine()->GetElapsedTime();
	int i;
	
	for(i=0; i<count; i++){
		((deBaseServiceModule*)pModules.GetAt(i))->FrameUpdate(elapsed);
	}
}

void deServiceManager::pProcessEvents(){
	decObjectList events;
	{
	const deMutexGuard lock(pMutex);
	events = pEventQueue;
	pEventQueue.RemoveAll();
	}
	
	const int count = events.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const cEvent &event = *((cEvent*)events.GetAt(i));
		switch(event.type){
		case cEvent::eEvents::eeRequestResponse:
			event.service->RequestResponse(event.id, event.data, event.finished);
			break;
			
		case cEvent::eEvents::eeRequestFailed:
			event.service->RequestFailed(event.id, event.data);
			break;
			
		case cEvent::eEvents::eeEventReceived:
			event.service->EventReceived(event.data);
			break;
		}
	}
}
