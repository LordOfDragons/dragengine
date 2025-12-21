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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peeEmitter.h"
#include "peeEmitterListener.h"
#include "peeController.h"
#include "peeType.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>



// Definitions
////////////////

#define LOGSOURCE "Particle Emitter Editor"



// Class peeEmitter
/////////////////////

// Constructor, destructor
////////////////////////////

peeEmitter::peeEmitter(igdeEnvironment *environment, peeLoadSaveSystem &loadSaveSystem) :
igdeEditableEntity(environment),
pLoadSaveSystem(loadSaveSystem)
{
	deEngine * const engine = GetEngine();
	
	pEngWorld = nullptr;
	pCamera = nullptr;
	
	pDDEmitter = nullptr;
	
	pSky = nullptr;
	
	pEngEmitter = nullptr;
	pEngEmitterInstance = nullptr;
	
	pActiveController = nullptr;
	pActiveType = nullptr;
	
	pWarmUpTime = 0.0f;
	pBurstLifetime = 1.0f;
	pEmitBurst = false;
	pEnableCasting = true;
	
	pBurstInterval = 1.0f;
	
	try{
		SetFilePath("new.depemit");
		
		// create world
		pEngWorld = engine->GetWorldManager()->CreateWorld();
		pEngWorld->SetGravity(decVector(0.0f, -9.81f, 0.0f));
		pEngWorld->SetDisableLights(false);
		pEngWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f));
		
		// create camera
		pCamera = new igdeCamera(engine);
		pCamera->SetEngineWorld(pEngWorld);
		pCamera->Reset();
		pCamera->SetPosition(decDVector(0.0, 1.0, 0.0));
		pCamera->SetOrientation(decVector(0.0f, 180.0f, 0.0f));
		pCamera->SetDistance(5.0f);
		pCamera->SetHighestIntensity(20.0f);
		pCamera->SetLowestIntensity(18.0f);
		pCamera->SetAdaptionTime(4.0f);
		
		// create sky
		pSky = new igdeWSky(*environment);
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pEngWorld);
		
		// create the environment wrapper object
		pEnvObject = igdeWObject::Ref::New(*environment);
		pEnvObject->SetWorld(pEngWorld);
		pEnvObject->SetPosition(decDVector(0.0, 0.0, 0.0));
		
		decLayerMask layerMask;
		layerMask.SetBit(peeEmitter::eclmObjects);
		pEnvObject->SetCollisionFilter(decCollisionFilter(layerMask));
		pEnvObject->SetCollisionFilterFallback(decCollisionFilter(layerMask));
		
		pEnvObject->SetGDClassName("IGDETestTerrain");
		
		// create particle emitter instance
		pEngEmitterInstance = engine->GetParticleEmitterInstanceManager()->CreateInstance();
		pEngWorld->AddParticleEmitter(pEngEmitterInstance);
		
		pEngEmitterInstance->SetPosition(decDVector(pPosition));
		pEngEmitterInstance->SetWarmUpTime(pWarmUpTime);
		pEngEmitterInstance->SetEnableCasting(pEnableCasting);
		
		layerMask.ClearMask();
		layerMask.SetBit(peeEmitter::eclmHeightTerrains);
		layerMask.SetBit(peeEmitter::eclmObjects);
		layerMask.SetBit(peeEmitter::eclmPropFields);
		layerMask.SetBit(peeEmitter::eclmWind);
		pEngEmitterInstance->SetCollisionFilter(decCollisionFilter(layerMask, layerMask));
		
		// create debug drawers
		pDDEmitter = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDDEmitter->SetXRay(true);
		pEngWorld->AddDebugDrawer(pDDEmitter);
		
		// create debug drawer shape
		pDDSEmitter.SetArrowLength(0.25f);
		pDDSEmitter.SetArrowSize(0.02f);
		pDDSEmitter.SetParentDebugDrawer(pDDEmitter);
		pDDSEmitter.SetVisible(true);
		
		// set default parameters
		SetPosition(decVector(0.0f, 1.0f, 0.0f));
		SetOrientation(decVector(0.0f, 0.0f, 0.0f)); // 90.0f, 0.0f, 0.0f
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	SetChanged(false);
	SetSaved(false);
}

peeEmitter::~peeEmitter(){
	pCleanUp();
}



// Management
///////////////

void peeEmitter::SetPosition(const decVector &position){
	if(!pPosition.IsEqualTo(position)){
		const decDVector dposition(position);
		
		pPosition = position;
		
		pEngEmitterInstance->SetPosition(dposition);
		pDDSEmitter.SetPosition(dposition);
		
		NotifyEmitterChanged();
	}
}

void peeEmitter::SetOrientation(const decVector &orientation){
	if(!pOrientation.IsEqualTo(orientation)){
		const decQuaternion &qorientation = decMatrix::CreateRotation(orientation * DEG2RAD).ToQuaternion();
		
		pOrientation = orientation;
		
		pEngEmitterInstance->SetOrientation(qorientation);
		pDDSEmitter.SetOrientation(qorientation);
		
		NotifyEmitterChanged();
	}
}



void peeEmitter::SetWarmUpTime(float warmUpTime){
	if(fabsf(warmUpTime - pWarmUpTime) > FLOAT_SAFE_EPSILON){
		pWarmUpTime = warmUpTime;
		pEngEmitterInstance->SetWarmUpTime(warmUpTime);
		NotifyEmitterChanged();
	}
}

void peeEmitter::SetBurstLifetime(float lifetime){
	if(fabsf(lifetime - pBurstLifetime) > FLOAT_SAFE_EPSILON){
		pBurstLifetime = lifetime;
		FreeEmitter();
		NotifyEmitterChanged();
	}
}

void peeEmitter::SetEmitBurst(bool emitBurst){
	if(emitBurst != pEmitBurst){
		pEmitBurst = emitBurst;
		
		FreeEmitter();
		NotifyEmitterChanged();
		
		pEngEmitterInstance->SetEnableCasting(pEnableCasting);
	}
}

void peeEmitter::SetEnableCasting(bool enableCasting){
	if(enableCasting != pEnableCasting){
		pEnableCasting = enableCasting;
		
		NotifyEmitterChanged();
		
		if(enableCasting){
			pBurstTimer = 0.0f;
			pEngEmitterInstance->ResetBurst();
		}
		
		pEngEmitterInstance->SetEnableCasting(enableCasting);
	}
}



void peeEmitter::SetBurstInterval(float interval){
	if(fabs(interval - pBurstInterval) > FLOAT_SAFE_EPSILON){
		pBurstInterval = interval;
		NotifyViewChanged();
	}
}



void peeEmitter::Dispose(){
	RemoveAllTypes();
	RemoveAllControllers();
	
	GetUndoSystem()->RemoveAll();
}

void peeEmitter::Reset(){
}

void peeEmitter::Update(float elapsed){
	if(!pEngEmitter){
		RebuildEmitter();
	}
	
	if(pEmitBurst){
		float burstTime = pBurstLifetime + pBurstInterval;
		
		if(burstTime < 0.1f){
			burstTime = 0.1f;
		}
		
		pBurstTimer += elapsed;
		
		if(pBurstTimer >= burstTime){
			pBurstTimer = fmodf(pBurstTimer, burstTime);
			pEngEmitterInstance->ResetBurst();
			pEngEmitterInstance->SetEnableCasting(pEnableCasting);
		}
	}
	
	pEnvObject->Update(elapsed);
	
	pEngWorld->Update(elapsed);
	pEngWorld->ProcessPhysics(elapsed);
}

void peeEmitter::FreeEmitter(){
	pEngEmitterInstance->SetEmitter(nullptr);
	pEngEmitter = nullptr;
}

void peeEmitter::RebuildEmitter(){
	deParticleEmitterController *engController = nullptr;
	const int controllerCount = pControllers.GetCount();
	const int typeCount = pTypes.GetCount();
	int i;
	
	FreeEmitter();
	
	for(i=0; i<controllerCount; i++){
		pControllers.GetAt(i)->SetEngineControllerIndex(i);
	}
	for(i=0; i<typeCount; i++){
		pTypes.GetAt(i)->SetIndex(i);
	}
	
	pBurstTimer = 0.0f;
	
	try{
		pEngEmitter = GetEngine()->GetParticleEmitterManager()->CreateParticleEmitter();
		
		for(i=0; i<controllerCount; i++){
			engController = new deParticleEmitterController;
			pControllers.GetAt(i)->UpdateEngineController(*engController);
			pEngEmitter->AddController(engController);
			engController = nullptr;
		}
		
		pEngEmitter->SetBurstLifetime(pBurstLifetime);
		pEngEmitter->SetEmitBurst(pEmitBurst);
		
		pEngEmitter->SetTypeCount(typeCount);
		for(i=0; i<typeCount; i++){
			pTypes.GetAt(i)->UpdateEngineType(pEngEmitter->GetTypeAt(i));
			pEngEmitter->NotifyTypeChangedAt(i);
		}
		
		pEngEmitterInstance->SetEmitter(pEngEmitter);
		
	}catch(const deException &e){
		if(engController){
			delete engController;
		}
		e.PrintError();
		FreeEmitter();
		return;
	}
}



// Controllers
////////////////

void peeEmitter::AddController(peeController *controller){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	if(pControllers.HasMatching([&](const peeController *each){
		return each->GetName() == controller->GetName();
	})){
		DETHROW(deeInvalidParam);
	}
	
	pControllers.Add(controller);
	controller->SetEmitter(this);
	NotifyControllerStructureChanged();
	
	if(!pActiveController){
		SetActiveController(controller);
	}
}

void peeEmitter::InsertControllerAt(peeController *controller, int index){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	if(pControllers.HasMatching([&](const peeController *each){
		return each->GetName() == controller->GetName();
	})){
		DETHROW(deeInvalidParam);
	}
	
	pControllers.Insert(controller, index);
	controller->SetEmitter(this);
	NotifyControllerStructureChanged();
	
	if(!pActiveController){
		SetActiveController(controller);
	}
}

void peeEmitter::MoveControllerTo(peeController *controller, int index){
	pControllers.Move(controller, index);
	NotifyControllerStructureChanged();
}

void peeEmitter::RemoveController(peeController *controller){
	if(!controller || controller->GetEmitter() != this) DETHROW(deeInvalidParam);
	
	if(controller->GetActive()){
		if(pControllers.GetCount() == 1){
			SetActiveController(nullptr);
			
		}else{
			if(pControllers.GetAt(0) == controller){
				SetActiveController(pControllers.GetAt(1));
				
			}else{
				SetActiveController(pControllers.GetAt(0));
			}
		}
	}
	
	controller->SetEmitter(nullptr);
	pControllers.Remove(controller);
	NotifyControllerStructureChanged();
}

void peeEmitter::RemoveAllControllers(){
	const int count = pControllers.GetCount();
	int i;
	
	SetActiveController(nullptr);
	
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetEmitter(nullptr);
	}
	pControllers.RemoveAll();
	NotifyControllerStructureChanged();
}

bool peeEmitter::HasActiveController() const{
	return pActiveController != nullptr;
}

void peeEmitter::SetActiveController(peeController *controller){
	if(controller != pActiveController){
		if(pActiveController){
			pActiveController->SetActive(false);
		}
		
		pActiveController = controller;
		
		if(controller){
			controller->SetActive(true);
		}
		
		NotifyActiveControllerChanged();
	}
}



// Types
//////////

void peeEmitter::AddType(peeType *type){
	if(!type){
		DETHROW(deeInvalidParam);
	}
	if(pTypes.HasMatching([&](const peeType *each){
		return each->GetName() == type->GetName();
	})){
		DETHROW(deeInvalidParam);
	}
	
	FreeEmitter();
	
	pTypes.Add(type);
	type->SetEmitter(this);
	NotifyTypeStructureChanged();
	
	if(!pActiveType){
		SetActiveType(type);
	}
}

void peeEmitter::InsertTypeAt(peeType *type, int index){
	if(!type){
		DETHROW(deeInvalidParam);
	}
	if(pTypes.HasMatching([&](const peeType *each){
		return each->GetName() == type->GetName();
	})){
		DETHROW(deeInvalidParam);
	}
	
	FreeEmitter();
	
	pTypes.Insert(type, index);
	type->SetEmitter(this);
	NotifyTypeStructureChanged();
	
	if(!pActiveType){
		SetActiveType(type);
	}
}

void peeEmitter::MoveTypeTo(peeType *type, int index){
	FreeEmitter();
	pTypes.Move(type, index);
	NotifyTypeStructureChanged();
}

void peeEmitter::RemoveType(peeType *type){
	if(!type || type->GetEmitter() != this){
		DETHROW(deeInvalidParam);
	}
	
	FreeEmitter();
	
	if(type->GetActive()){
		if(pTypes.GetCount() == 1){
			SetActiveType(nullptr);
			
		}else{
			if(pTypes.GetAt(0) == type){
				SetActiveType(pTypes.GetAt(1));
				
			}else{
				SetActiveType(pTypes.GetAt(0));
			}
		}
	}
	
	type->SetEmitter(nullptr);
	pTypes.Remove(type);
	NotifyTypeStructureChanged();
}

void peeEmitter::RemoveAllTypes(){
	const int count = pTypes.GetCount();
	int i;
	
	FreeEmitter();
	SetActiveType(nullptr);
	
	for(i=0; i<count; i++){
		pTypes.GetAt(i)->SetEmitter(nullptr);
	}
	pTypes.RemoveAll();
	NotifyTypeStructureChanged();
}

bool peeEmitter::HasActiveType() const{
	return pActiveType != nullptr;
}

void peeEmitter::SetActiveType(peeType *type){
	if(type != pActiveType){
		if(pActiveType){
			pActiveType->SetActive(false);
		}
		
		pActiveType = type;
		
		if(type){
			type->SetActive(true);
		}
		
		NotifyActiveTypeChanged();
	}
}



// Listeners
//////////////

void peeEmitter::AddListener(peeEmitterListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void peeEmitter::RemoveListener(peeEmitterListener *listener){
	pListeners.Remove(listener);
}
void peeEmitter::NotifyStateChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.StateChanged(this);
	});
}

void peeEmitter::NotifyUndoChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.UndoChanged(this);
	});
}

void peeEmitter::NotifyEmitterChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.EmitterChanged(this);
	});
	
	SetChanged(true);
}

void peeEmitter::NotifySkyChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.SkyChanged(this);
	});
}

void peeEmitter::NotifyEnvObjectChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.EnvObjectChanged(this);
	});
}

void peeEmitter::NotifyViewChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.ViewChanged(this);
	});
}

void peeEmitter::NotifyCameraChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.CameraChanged(this);
	});
}



void peeEmitter::NotifyControllerStructureChanged(){
	FreeEmitter();
	
	pListeners.Visit([&](peeEmitterListener &l){
		l.ControllerStructureChanged(this);
	});
	
	SetChanged(true);
}

void peeEmitter::NotifyControllerNameChanged(peeController *controller){
	FreeEmitter();
	
	pListeners.Visit([&](peeEmitterListener &l){
		l.ControllerNameChanged(this, controller);
	});
	
	SetChanged(true);
}

void peeEmitter::NotifyControllerChanged(peeController *controller){
	pListeners.Visit([&](peeEmitterListener &l){
		l.ControllerChanged(this, controller);
	});
	
	SetChanged(true);
}

void peeEmitter::NotifyControllerValueChanged(peeController *controller){
	pListeners.Visit([&](peeEmitterListener &l){
		l.ControllerValueChanged(this, controller);
	});
}

void peeEmitter::NotifyActiveControllerChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.ActiveControllerChanged(this);
	});
}



void peeEmitter::NotifyTypeStructureChanged(){
	FreeEmitter();
	
	pListeners.Visit([&](peeEmitterListener &l){
		l.TypeStructureChanged(this);
	});
	
	SetChanged(true);
}

void peeEmitter::NotifyTypeChanged(peeType *type){
	FreeEmitter();
	
	pListeners.Visit([&](peeEmitterListener &l){
		l.TypeChanged(this, type);
	});
	
	SetChanged(true);
}

void peeEmitter::NotifyTypeActiveTrailControllerChanged(peeType *type){
	pListeners.Visit([&](peeEmitterListener &l){
		l.TypeActiveTrailControllerChanged(this, type);
	});
}

void peeEmitter::NotifyTypeActiveEmitControllerChanged(peeType *type){
	pListeners.Visit([&](peeEmitterListener &l){
		l.TypeActiveEmitControllerChanged(this, type);
	});
}

void peeEmitter::NotifyTypeParameterChanged(peeType *type, peeParameter *parameter){
	pListeners.Visit([&](peeEmitterListener &l){
		l.TypeParameterChanged(this, type, parameter);
	});
	
	SetChanged(true);
}

void peeEmitter::NotifyActiveTypeParameterChanged(peeType *type){
	pListeners.Visit([&](peeEmitterListener &l){
		l.ActiveTypeParameterChanged(this, type);
	});
}

void peeEmitter::NotifyActiveTypeChanged(){
	pListeners.Visit([&](peeEmitterListener &l){
		l.ActiveTypeChanged(this);
	});
}


// Private Functions
//////////////////////

void peeEmitter::pCleanUp(){
	pDDSEmitter.SetParentDebugDrawer(nullptr);
	
	if(pSky){
		delete pSky;
	}
	pEnvObject = nullptr;
	
	pListeners.RemoveAll();
	
	if(pCamera){
		delete pCamera;
	}
	
	RemoveAllTypes();
	RemoveAllControllers();
	
	if(pEngEmitterInstance){
		pEngEmitterInstance->SetEmitter(nullptr);
	}
	if(pEngWorld){
		if(pDDEmitter){
			pEngWorld->RemoveDebugDrawer(pDDEmitter);
		}
		
		if(pEngEmitterInstance){
			pEngWorld->RemoveParticleEmitter(pEngEmitterInstance);
		}
	}
}
