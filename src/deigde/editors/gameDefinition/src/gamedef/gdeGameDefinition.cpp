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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gdeGameDefinition.h"
#include "gdeGameDefinitionListener.h"
#include "category/gdeCategory.h"
#include "objectClass/gdeObjectClass.h"
#include "objectClass/billboard/gdeOCBillboard.h"
#include "objectClass/camera/gdeOCCamera.h"
#include "objectClass/component/gdeOCComponent.h"
#include "objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "objectClass/light/gdeOCLight.h"
#include "objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "objectClass/navspace/gdeOCNavigationSpace.h"
#include "objectClass/particleemitter/gdeOCParticleEmitter.h"
#include "objectClass/forceField/gdeOCForceField.h"
#include "objectClass/snappoint/gdeOCSnapPoint.h"
#include "objectClass/speaker/gdeOCSpeaker.h"
#include "particleemitter/gdeParticleEmitter.h"
#include "skin/gdeSkin.h"
#include "sky/gdeSky.h"
#include "../gdEditor.h"
#include "../loadsave/gdeLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>



// Class gdeGameDefinition
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeGameDefinition::gdeGameDefinition(igdeEnvironment* environment) :
igdeEditableEntity(environment),

pSky(nullptr),

pCamera(nullptr),
pViewRatio(1.0f),

pIsProjectGameDef(false),

pVFSPath("/"),

pSelectedObjectType(eotNoSelection),
pPreviewVFS(nullptr)
{
	deEngine * const engine = GetEngine();
	
	try{
		SetFilePath("new.gamedef.xml");
		
		// create world
		pWorld = engine->GetWorldManager()->CreateWorld();
		pWorld->SetGravity(decVector(0.0f, -9.81f, 0.0f));
		pWorld->SetDisableLights(false);
		
		// create camera
		pCamera = new igdeCamera(engine);
		pCamera->SetEngineWorld(pWorld);
		pCamera->Reset();
		pCamera->SetPosition(decDVector(0.0, /*1.0*/0.0, 0.0));
		pCamera->SetOrientation(decVector(0.0f, 180.0f, 0.0f));
		pCamera->SetDistance(5.0f);
		pCamera->SetHighestIntensity(20.0f);
		pCamera->SetLowestIntensity(18.0f);
		pCamera->SetAdaptionTime(4.0f);
		
		// create sky
		pSky = new igdeWSky(*environment);
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pWorld);
		
		// create the environment wrapper object
		pEnvObject = igdeWObject::Ref::New(*environment);
		pEnvObject->SetWorld(pWorld);
		pEnvObject->SetPosition(decDVector(0.0, 0.0, 0.0));
		pEnvObject->SetVisible(false); // otherwise a stupid placeholder box shows
		
		decLayerMask layerMask;
		layerMask.SetBit(1);
		pEnvObject->SetCollisionFilter(decCollisionFilter(layerMask));
		pEnvObject->SetCollisionFilterFallback(decCollisionFilter(layerMask));
		
		//pEnvObject->SetGDClassName( "IGDETestTerrain" );
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

gdeGameDefinition::~gdeGameDefinition(){
	pCleanUp();
}



// Management
///////////////

void gdeGameDefinition::SetViewRatio(float viewRatio){
	pViewRatio = decMath::max(viewRatio, 0.1f);
}

void gdeGameDefinition::SetIsProjectGameDef(bool isProjectGameDef){
	pIsProjectGameDef = isProjectGameDef;
}

void gdeGameDefinition::SetID(const char *id){
	if(pID == id){
		return;
	}
	
	pID = id;
	
	NotifyGameDefinitionChanged();
}

void gdeGameDefinition::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	
	NotifyGameDefinitionChanged();
}

void gdeGameDefinition::SetBasePath(const char *path){
	if(pBasePath == path){
		return;
	}
	
	pBasePath = path;
	
	if(!pIsProjectGameDef){
		pPreviewVFS = nullptr;
		pVFS = nullptr;
	}
	
	NotifyBasePathChanged();
}

void gdeGameDefinition::SetVFSPath(const char *path){
	if(pVFSPath == path){
		return;
	}
	
	pVFSPath = path;
	
	if(!pIsProjectGameDef){
		pPreviewVFS = nullptr;
		pVFS = nullptr;
	}
	
	NotifyBasePathChanged();
}

void gdeGameDefinition::SetScriptModule(const char *identifier){
	if(pScriptModule == identifier){
		return;
	}
	
	pScriptModule = identifier;
	
	NotifyGameDefinitionChanged();
}



void gdeGameDefinition::SetBaseGameDefinitionIDList(const decStringList &ids){
	pBaseGameDefinitionIDList = ids;
}

void gdeGameDefinition::UpdateBaseGameDefinitions(gdeLoadSaveSystem &loadSaveSystem){
	const int oldCount = pBaseGameDefinitions.GetCount();
	decTObjectOrderedSet<gdeGameDefinition> list;
	int i, j;
	
	const int count = pBaseGameDefinitionIDList.GetCount();
	for(i=0; i<count; i++){
		const decString &id = pBaseGameDefinitionIDList.GetAt(i);
		
		// check if game definition has been already loaded in the previous call to
		// UpdateBaseGameDefinitions(). avoids reloading all base game definitions
		for(j=0; j<oldCount; j++){
			gdeGameDefinition * const checkGameDef = pBaseGameDefinitions.GetAt(j);
			if(checkGameDef->GetID() == id){
				list.Add(checkGameDef);
				break;
			}
		}
		if(j < oldCount){
			continue;  // already loaded
		}
		
		// resolve game definition id using the environment
		GetEnvironment()->GetLogger()->LogInfoFormat(LOGSOURCE,
			"UpdateBaseGameDefinitions: Resolving Base Game Definition ID '%s'", id.GetString());
		
		const igdeGameDefinition * const sharedGameDefinition = GetEnvironment()->GetSharedGameDefinition(id);
		if(!sharedGameDefinition){
			GetEnvironment()->GetLogger()->LogInfoFormat(LOGSOURCE,
				"UpdateBaseGameDefinitions: Failed resolving Game Definition ID '%s'", id.GetString());
			continue;
		}
		
		// load game definitions. this potentially loads base game definitions too
		const decString &path = sharedGameDefinition->GetFilename();
		GetEnvironment()->GetLogger()->LogInfoFormat(LOGSOURCE,
			"UpdateBaseGameDefinitions: Loading Game Definition '%s'", path.GetString());
		
		gdeGameDefinition::Ref gameDefinition;
		try{
			gameDefinition = loadSaveSystem.LoadGameDefinition(path);
			
		}catch(const deException &e){
			GetEnvironment()->GetLogger()->LogInfoFormat(LOGSOURCE,
				"UpdateBaseGameDefinitions: Failed Loading Game Definition '%s'", path.GetString());
			GetEnvironment()->GetLogger()->LogException(LOGSOURCE, e);
			continue;
		}
		
		gameDefinition->UpdateBaseGameDefinitions(loadSaveSystem);
		
		gameDefinition->SetFilePath(path);
		gameDefinition->SetChanged(false);
		gameDefinition->SetSaved(true);
		
		list.Add(gameDefinition);
	}
	
	pBaseGameDefinitions = list;
	pClassNameList.RemoveAll();
	
	NotifyBaseGameDefinitionsChanged();
}

int gdeGameDefinition::GetBaseGameDefinitionCount() const{
	return pBaseGameDefinitions.GetCount();
}

const gdeGameDefinition &gdeGameDefinition::GetBaseGameDefinitionAt(int index) const{
	return *((const gdeGameDefinition *)pBaseGameDefinitions.GetAt(index));
}



void gdeGameDefinition::SetDefaultObjectClass(const char *objectClass){
	if(pDefaultObjectClass == objectClass){
		return;
	}
	
	pDefaultObjectClass = objectClass;
	
	NotifyGameDefinitionChanged();
}

void gdeGameDefinition::SetDefaultSkin(const char *skin){
	if(pDefaultSkin == skin){
		return;
	}
	
	pDefaultSkin = skin;
	
	NotifyGameDefinitionChanged();
}

void gdeGameDefinition::SetDefaultSky(const char *sky){
	if(pDefaultSky == sky){
		return;
	}
	
	pDefaultSky = sky;
	
	NotifyGameDefinitionChanged();
}



const gdeCategory *gdeGameDefinition::FindCategoryObjectClass(const char *path) const{
	const gdeCategory *category = pCategoriesObjectClass.FindWithPath(path);
	if(category){
		return category;
	}
	
	const int count = pBaseGameDefinitions.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		category = ((const gdeGameDefinition *)pBaseGameDefinitions.GetAt(i))->FindCategoryObjectClass(path);
		if(category){
			return category;
		}
	}
	
	return {};
}

const decStringSet &gdeGameDefinition::GetObjectClassCategoryNameList(){
	if(pObjectClassCategoryNameList.GetCount() > 0){
		return pObjectClassCategoryNameList;
	}
	
	int i, count = pObjectClasses.GetCount();
	for(i=0; i<count; i++){
		pObjectClassCategoryNameList.Add(pCategoriesObjectClass.GetAt(i)->GetName());
	}
	
	count = pBaseGameDefinitions.GetCount();
	for(i=0; i<count; i++){
		pObjectClassCategoryNameList += pBaseGameDefinitions.GetAt(i)->GetObjectClassCategoryNameList();
	}
	
	return pObjectClassCategoryNameList;
}



void gdeGameDefinition::SetActiveCategory(gdeCategory *category){
	if(category == pActiveCategory){
		return;
	}
	pActiveCategory = category;
	NotifyActiveCategoryChanged();
}



void gdeGameDefinition::UpdateUsedTagsObjectClass(){
	const int count = pObjectClasses.GetCount();
	int i;
	
	pUsedTagsObjectClass.RemoveAll();
	
	for(i=0; i<count; i++){
		pUsedTagsObjectClass += pObjectClasses.GetAt(i)->GetHideTags();
		pUsedTagsObjectClass += pObjectClasses.GetAt(i)->GetPartialHideTags();
	}
	
	NotifyObjectClassUsedTagsChanged();
}


void gdeGameDefinition::NotifyWorldPropertiesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.WorldPropertiesChanged(this);
	});
}

void gdeGameDefinition::NotifyWorldPropertyChanged(gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.WorldPropertyChanged(this, property);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyWorldPropertyNameChanged(gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.WorldPropertyNameChanged(this, property);
	});
	
	SetChanged(true);
}



void gdeGameDefinition::NotifyDecalPropertiesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.DecalPropertiesChanged(this);
	});
}

void gdeGameDefinition::NotifyDecalPropertyChanged(gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.DecalPropertyChanged(this, property);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyDecalPropertyNameChanged(gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.DecalPropertyNameChanged(this, property);
	});
	
	SetChanged(true);
}



void gdeGameDefinition::SetAutoFindPathObjectClasses(const decStringList &list){
	if(list == pAutoFindPathObjectClasses){
		return;
	}
	pAutoFindPathObjectClasses = list;
	NotifyAutoFindPathObjectClassesChanged();
}

void gdeGameDefinition::NotifyAutoFindPathObjectClassesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.AutoFindPathObjectClassesChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::SetAutoFindPathSkins(const decStringList &list){
	if(list == pAutoFindPathSkins){
		return;
	}
	pAutoFindPathSkins = list;
	NotifyAutoFindPathSkinsChanged();
}

void gdeGameDefinition::NotifyAutoFindPathSkinsChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.AutoFindPathSkinsChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::SetAutoFindPathSkies(const decStringList &list){
	if(list == pAutoFindPathSkies){
		return;
	}
	pAutoFindPathSkies = list;
	NotifyAutoFindPathSkiesChanged();
}

void gdeGameDefinition::NotifyAutoFindPathSkiesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.AutoFindPathSkiesChanged(this);
	});
	
	SetChanged(true);
}


void gdeGameDefinition::SetSelectedObjectType(eObjectTypes objectType){
	if(objectType == pSelectedObjectType){
		return;
	}
	
	pSelectedObjectType = objectType;
	NotifySelectedObjectChanged();
}

void gdeGameDefinition::UpdateDefinedUsedIDs(){
	const int occount = pObjectClasses.GetCount();
	decStringSet definedIDs;
	decStringSet usedIDs;
	int i;
	
	for(i=0; i<occount; i++){
		pObjectClasses.GetAt(i)->GetDefinedUsedIDs(definedIDs, usedIDs);
	}
	
	if(definedIDs == pDefinedIDs && usedIDs == pUsedIDs){
		return;
	}
	
	pDefinedIDs = definedIDs;
	pUsedIDs = usedIDs;
	NotifyDefinedUsedIDsChanged();
}



void gdeGameDefinition::Update(float elapsed){
	pEnvObject->Update(elapsed);
	
	pWorld->ProcessPhysics(elapsed);
	pWorld->Update(elapsed);
}



deVirtualFileSystem *gdeGameDefinition::GetPreviewVFS(){
	if(pPreviewVFS){
		return pPreviewVFS;
	}
	
	if(pIsProjectGameDef){ // pBasePath == GetEnvironment()->GetGameProject()->GetPathProject()){
		pPreviewVFS = GetEngine()->GetVirtualFileSystem();
		pVFS = nullptr;
		
	}else{
		if(!pVFS){
			try{
				pVFS = deVirtualFileSystem::Ref::New();
				pVFS->AddContainer(deVFSDiskDirectory::Ref::New(decPath::CreatePathUnix(pVFSPath),
					decPath::CreatePathNative(pBasePath), true));
				
			}catch(const deException &){
				pVFS = nullptr;
				throw;
			}
		}
		
		pPreviewVFS = pVFS;
	}
	
	return pPreviewVFS;
}



// Object classes
///////////////////

void gdeGameDefinition::AddObjectClass(gdeObjectClass *objectClass){
	DEASSERT_NOTNULL(objectClass)
	pObjectClasses.AddOrThrow(objectClass);
	
	objectClass->SetGameDefinition(this);
	
	pClassNameList.RemoveAll();
	
	NotifyObjectClassStructureChanged();
	
	if(!pActiveObjectClass){
		SetActiveObjectClass(objectClass);
	}
}

void gdeGameDefinition::RemoveObjectClass(gdeObjectClass *objectClass){
	const gdeObjectClass::Ref guard(objectClass);
	pObjectClasses.RemoveOrThrow(objectClass);
	
	if(objectClass == pActiveObjectClass){
		SetActiveOCBillboard(nullptr);
		SetActiveOCCamera(nullptr);
		SetActiveOCComponent(nullptr);
		SetActiveOCEnvMapProbe(nullptr);
		SetActiveOCLight(nullptr);
		SetActiveOCNavigationBlocker(nullptr);
		SetActiveOCNavigationSpace(nullptr);
		SetActiveOCParticleEmitter(nullptr);
		SetActiveOCForceField(nullptr);
		SetActiveOCSnapPoint(nullptr);
		SetActiveOCSpeaker(nullptr);
		SetActiveOCWorld(nullptr);
		
		pActiveObjectClass = nullptr;
	}
	
	objectClass->SetGameDefinition(nullptr);
	
	pClassNameList.RemoveAll();
	
	NotifyObjectClassStructureChanged();
}

void gdeGameDefinition::RemoveAllObjectClasses(){
	if(pObjectClasses.IsEmpty()){
		return;
	}
	
	SetActiveOCSpeaker(nullptr);
	SetActiveOCSnapPoint(nullptr);
	SetActiveOCParticleEmitter(nullptr);
	SetActiveOCForceField(nullptr);
	SetActiveOCNavigationSpace(nullptr);
	SetActiveOCNavigationBlocker(nullptr);
	SetActiveOCLight(nullptr);
	SetActiveOCEnvMapProbe(nullptr);
	SetActiveOCComponent(nullptr);
	SetActiveOCCamera(nullptr);
	SetActiveOCBillboard(nullptr);
	SetActiveOCWorld(nullptr);
	SetActiveObjectClass(nullptr);
	
	pObjectClasses.Visit([](gdeObjectClass &c){
		c.SetGameDefinition(nullptr);
	});
	pObjectClasses.RemoveAll();
	
	pClassNameList.RemoveAll();
	
	NotifyObjectClassStructureChanged();
}

const gdeObjectClass *gdeGameDefinition::FindObjectClass(const char *name) const{
	const gdeObjectClass *objectClass = pObjectClasses.FindNamed(name);
	if(objectClass){
		return objectClass;
	}
	
	for(const auto &gd : pBaseGameDefinitions){
		objectClass = gd->FindObjectClass(name);
		if(objectClass){
			return objectClass;
		}
	}
	
	return nullptr;
}

const decStringSet &gdeGameDefinition::GetClassNameList(){
	if(pClassNameList.IsNotEmpty()){
		return pClassNameList;
	}
	
	pObjectClasses.Visit([&](const gdeObjectClass &oc){
		pClassNameList.Add(oc.GetName());
	});
	
	pBaseGameDefinitions.Visit([&](gdeGameDefinition &gd){
		pClassNameList += gd.GetClassNameList();
	});
	
	return pClassNameList;
}



bool gdeGameDefinition::HasActiveObjectClass() const{
	return pActiveObjectClass != nullptr;
}

void gdeGameDefinition::SetActiveObjectClass(gdeObjectClass *objectClass){
	if(objectClass == pActiveObjectClass){
		return;
	}
	pActiveObjectClass = objectClass;
	NotifyActiveObjectClassChanged();
}



bool gdeGameDefinition::HasActiveOCBillboard() const{
	return pActiveOCBillboard != nullptr;
}

void gdeGameDefinition::SetActiveOCBillboard(gdeOCBillboard *billboard){
	if(billboard == pActiveOCBillboard){
		return;
	}
	pActiveOCBillboard = billboard;
	NotifyActiveOCBillboardChanged();
}



bool gdeGameDefinition::HasActiveOCCamera() const{
	return pActiveOCCamera != nullptr;
}

void gdeGameDefinition::SetActiveOCCamera(gdeOCCamera *camera){
	if(camera == pActiveOCCamera){
		return;
	}
	pActiveOCCamera = camera;
	NotifyActiveOCCameraChanged();
}



bool gdeGameDefinition::HasActiveOCComponent() const{
	return pActiveOCComponent != nullptr;
}

void gdeGameDefinition::SetActiveOCComponent(gdeOCComponent *component){
	if(component == pActiveOCComponent){
		return;
	}
	pActiveOCComponent = component;
	NotifyActiveOCComponentChanged();
}



bool gdeGameDefinition::HasActiveOCEnvMapProbe() const{
	return pActiveOCEnvMapProbe != nullptr;
}

void gdeGameDefinition::SetActiveOCEnvMapProbe(gdeOCEnvMapProbe *envMapProbe){
	if(envMapProbe == pActiveOCEnvMapProbe){
		return;
	}
	pActiveOCEnvMapProbe = envMapProbe;
	NotifyActiveOCEnvMapProbeChanged();
}



bool gdeGameDefinition::HasActiveOCLight() const{
	return pActiveOCLight != nullptr;
}

void gdeGameDefinition::SetActiveOCLight(gdeOCLight *light){
	if(light == pActiveOCLight){
		return;
	}
	pActiveOCLight = light;
	NotifyActiveOCLightChanged();
}



bool gdeGameDefinition::HasActiveOCNavigationBlocker() const{
	return pActiveOCNavigationBlocker != nullptr;
}

void gdeGameDefinition::SetActiveOCNavigationBlocker(gdeOCNavigationBlocker *navblocker){
	if(navblocker == pActiveOCNavigationBlocker){
		return;
	}
	pActiveOCNavigationBlocker = navblocker;
	NotifyActiveOCNavigationBlockerChanged();
}



bool gdeGameDefinition::HasActiveOCNavigationSpace() const{
	return pActiveOCNavigationSpace != nullptr;
}

void gdeGameDefinition::SetActiveOCNavigationSpace(gdeOCNavigationSpace *navSpace){
	if(navSpace == pActiveOCNavigationSpace){
		return;
	}
	pActiveOCNavigationSpace = navSpace;
	NotifyActiveOCNavigationSpaceChanged();
}



bool gdeGameDefinition::HasActiveOCParticleEmitter() const{
	return pActiveOCParticleEmitter != nullptr;
}

void gdeGameDefinition::SetActiveOCParticleEmitter(gdeOCParticleEmitter *emitter){
	if(emitter == pActiveOCParticleEmitter){
		return;
	}
	pActiveOCParticleEmitter = emitter;
	NotifyActiveOCParticleEmitterChanged();
}



bool gdeGameDefinition::HasActiveOCForceField() const{
	return pActiveOCForceField != nullptr;
}

void gdeGameDefinition::SetActiveOCForceField(gdeOCForceField *field){
	if(field == pActiveOCForceField){
		return;
	}
	pActiveOCForceField = field;
	NotifyActiveOCForceFieldChanged();
}



bool gdeGameDefinition::HasActiveOCSnapPoint() const{
	return pActiveOCSnapPoint != nullptr;
}

void gdeGameDefinition::SetActiveOCSnapPoint(gdeOCSnapPoint *snappoint){
	if(snappoint == pActiveOCSnapPoint){
		return;
	}
	pActiveOCSnapPoint = snappoint;
	NotifyActiveOCSnapPointChanged();
}



bool gdeGameDefinition::HasActiveOCSpeaker() const{
	return pActiveOCSpeaker != nullptr;
}

void gdeGameDefinition::SetActiveOCSpeaker(gdeOCSpeaker *speaker){
	if(speaker == pActiveOCSpeaker){
		return;
	}
	pActiveOCSpeaker = speaker;
	NotifyActiveOCSpeakerChanged();
}



bool gdeGameDefinition::HasActiveOCWorld() const{
	return pActiveOCWorld != nullptr;
}

void gdeGameDefinition::SetActiveOCWorld(gdeOCWorld *world){
	if(pActiveOCWorld == world){
		return;
	}
	
	pActiveOCWorld = world;
	NotifyActiveOCWorldChanged();
}



// ParticleEmitters
/////////////////////

void gdeGameDefinition::AddParticleEmitter(gdeParticleEmitter *particleEmitter){
	DEASSERT_NOTNULL(particleEmitter)
	pParticleEmitters.AddOrThrow(particleEmitter);
	
	particleEmitter->SetGameDefinition(this);
	NotifyParticleEmitterStructureChanged();
	
	if(!pActiveParticleEmitter){
		SetActiveParticleEmitter(particleEmitter);
	}
}

void gdeGameDefinition::RemoveParticleEmitter(gdeParticleEmitter *particleEmitter){
	const gdeParticleEmitter::Ref guard(particleEmitter);
	pParticleEmitters.RemoveOrThrow(particleEmitter);
	
	if(particleEmitter == pActiveParticleEmitter){
		pActiveParticleEmitter = nullptr;
	}
	
	particleEmitter->SetGameDefinition(nullptr);
	NotifyParticleEmitterStructureChanged();
}

void gdeGameDefinition::RemoveAllParticleEmitters(){
	if(pParticleEmitters.IsEmpty()){
		return;
	}
	
	SetActiveParticleEmitter(nullptr);
	
	pParticleEmitters.Visit([](gdeParticleEmitter &pe){
		pe.SetGameDefinition(nullptr);
	});
	pParticleEmitters.RemoveAll();
	NotifyParticleEmitterStructureChanged();
}

bool gdeGameDefinition::HasActiveParticleEmitter() const{
	return pActiveParticleEmitter != nullptr;
}

void gdeGameDefinition::SetActiveParticleEmitter(gdeParticleEmitter *particleEmitter){
	if(particleEmitter == pActiveParticleEmitter){
		return;
	}
	pActiveParticleEmitter = particleEmitter;
	NotifyActiveParticleEmitterChanged();
}



// Skins
//////////

void gdeGameDefinition::AddSkin(gdeSkin *skin){
	DEASSERT_NOTNULL(skin)
	pSkins.AddOrThrow(skin);
	
	skin->SetGameDefinition(this);
	NotifySkinStructureChanged();
	
	if(!pActiveSkin){
		SetActiveSkin(skin);
	}
}

void gdeGameDefinition::RemoveSkin(gdeSkin *skin){
	const gdeSkin::Ref guard(skin);
	pSkins.RemoveOrThrow(skin);
	
	if(skin == pActiveSkin){
		pActiveSkin = nullptr;
	}
	
	skin->SetGameDefinition(nullptr);
	NotifySkinStructureChanged();
}

void gdeGameDefinition::RemoveAllSkins(){
	if(pSkins.IsEmpty()){
		return;
	}
	
	SetActiveSkin(nullptr);
	
	pSkins.Visit([](gdeSkin &s){
		s.SetGameDefinition(nullptr);
	});
	pSkins.RemoveAll();
	NotifySkinStructureChanged();
}

bool gdeGameDefinition::HasActiveSkin() const{
	return pActiveSkin != nullptr;
}

void gdeGameDefinition::SetActiveSkin(gdeSkin *skin){
	if(skin == pActiveSkin){
		return;
	}
	pActiveSkin = skin;
	NotifyActiveSkinChanged();
}



// Skins
//////////

void gdeGameDefinition::AddSky(gdeSky *sky){
	DEASSERT_NOTNULL(sky)
	pSkies.AddOrThrow(sky);
	
	sky->SetGameDefinition(this);
	NotifySkyStructureChanged();
	
	if(!pActiveSky){
		SetActiveSky(sky);
	}
}

void gdeGameDefinition::RemoveSky(gdeSky *sky){
	const gdeSky::Ref guard(sky);
	pSkies.RemoveOrThrow(sky);
	
	if(sky == pActiveSky){
		pActiveSky = nullptr;
	}
	
	sky->SetGameDefinition(nullptr);
	NotifySkyStructureChanged();
}

void gdeGameDefinition::RemoveAllSkies(){
	if(pSkies.IsEmpty()){
		return;
	}
	
	SetActiveSky(nullptr);
	
	pSkies.Visit([](gdeSky &s){
		s.SetGameDefinition(nullptr);
	});
	pSkies.RemoveAll();
	NotifySkyStructureChanged();
}

bool gdeGameDefinition::HasActiveSky() const{
	return pActiveSky != nullptr;
}

void gdeGameDefinition::SetActiveSky(gdeSky *sky){
	if(sky == pActiveSky){
		return;
	}
	pActiveSky = sky;
	NotifyActiveSkyChanged();
}



// Listeners
//////////////

void gdeGameDefinition::AddListener(gdeGameDefinitionListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void gdeGameDefinition::RemoveListener(gdeGameDefinitionListener *listener){
	pListeners.Remove(listener);
}


void gdeGameDefinition::NotifyStateChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.StateChanged(this);
	});
}

void gdeGameDefinition::NotifyUndoChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.UndoChanged(this);
	});
}

void gdeGameDefinition::NotifyViewChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ViewChanged(this);
	});
}

void gdeGameDefinition::NotifyGameDefinitionChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.GameDefinitionChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyBasePathChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.BasePathChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyBaseGameDefinitionsChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.BaseGameDefinitionsChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkyChanged(this);
	});
}

void gdeGameDefinition::NotifyEnvObjectChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.EnvObjectChanged(this);
	});
}

void gdeGameDefinition::NotifyCameraChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.CameraChanged(this);
	});
}

void gdeGameDefinition::NotifySelectedObjectChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SelectedObjectChanged(this);
	});
}



void gdeGameDefinition::NotifyObjectClassCategoriesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ObjectClassCategoriesChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkinCategoriesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkinCategoriesChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyCategoriesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkyCategoriesChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyParticleEmitterCategoriesChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ParticleEmitterCategoriesChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveCategoryChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveCategoryChanged(this);
	});
}



void gdeGameDefinition::NotifyObjectClassUsedTagsChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ObjectClassUsedTagsChanged(this);
	});
}

void gdeGameDefinition::NotifyDefinedUsedIDsChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.DefinedUsedIDsChanged(this);
	});
}



void gdeGameDefinition::NotifyObjectClassStructureChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ObjectClassStructureChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyObjectClassChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ObjectClassChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyObjectClassNameChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ObjectClassNameChanged(this, objectClass);
	});
	
	pClassNameList.RemoveAll();
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertyChanged(gdeObjectClass *objectClass, gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCPropertyChanged(this, objectClass, property);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertyNameChanged(gdeObjectClass *objectClass, gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCPropertyNameChanged(this, objectClass, property);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertiesChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCPropertiesChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertyValuesChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCPropertyValuesChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturePropertyChanged(gdeObjectClass *objectClass, gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCTexturePropertyChanged(this, objectClass, property);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturePropertyNameChanged(gdeObjectClass *objectClass, gdeProperty *property){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCTexturePropertyNameChanged(this, objectClass, property);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturePropertiesChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCTexturePropertiesChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCInheritChanged(gdeObjectClass *objectClass, gdeOCInherit *inherit){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCInheritChanged(this, objectClass, inherit);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCInheritsChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCInheritsChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCBillboardsChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCBillboardsChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCBillboardChanged(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCBillboardChanged(this, objectClass, billboard);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCCamerasChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCCamerasChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCCameraChanged(gdeObjectClass *objectClass, gdeOCCamera *camera){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCCameraChanged(this, objectClass, camera);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentsChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCComponentsChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentChanged(gdeObjectClass *objectClass, gdeOCComponent *component){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCComponentChanged(this, objectClass, component);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentActiveTextureChanged(
gdeObjectClass *objectClass, gdeOCComponent *component){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCComponentActiveTextureChanged(this, objectClass, component);
	});
}

void gdeGameDefinition::NotifyOCComponentTextureChanged(gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCComponentTextureChanged(this, objectClass, component, texture);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentTextureNameChanged(gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCComponentTextureNameChanged(this, objectClass, component, texture);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentTexturePropertiesChanged(gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCComponentTexturePropertiesChanged(this, objectClass, component, texture);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCEnvMapProbesChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCEnvMapProbesChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCEnvMapProbeChanged(gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCEnvMapProbeChanged(this, objectClass, envMapProbe);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCLightsChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCLightsChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCLightChanged(gdeObjectClass *objectClass, gdeOCLight *light){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCLightChanged(this, objectClass, light);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationBlockersChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCNavigationBlockersChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationBlockerChanged(
gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCNavigationBlockerChanged(this, objectClass, navblocker);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationSpacesChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCNavigationSpacesChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationSpaceChanged(
gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCNavigationSpaceChanged(this, objectClass, navspace);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCParticleEmittersChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCParticleEmittersChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCParticleEmitterChanged(
gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCParticleEmitterChanged(this, objectClass, emitter);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCForceFieldsChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCForceFieldsChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCForceFieldChanged(
gdeObjectClass *objectClass, gdeOCForceField *field){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCForceFieldChanged(this, objectClass, field);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSnapPointsChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCSnapPointsChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSnapPointChanged(gdeObjectClass *objectClass,
gdeOCSnapPoint *snappoint){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCSnapPointChanged(this, objectClass, snappoint);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSnapPointNameChanged(gdeObjectClass *objectClass,
gdeOCSnapPoint *snappoint){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCSnapPointNameChanged(this, objectClass, snappoint);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSpeakersChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCSpeakersChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSpeakerChanged(gdeObjectClass *objectClass, gdeOCSpeaker *speaker){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCSpeakerChanged(this, objectClass, speaker);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCWorldsChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCWorldsChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCWorldChanged(gdeObjectClass *objectClass, gdeOCWorld *world){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCWorldChanged(this, objectClass, world);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturesChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCTexturesChanged(this, objectClass);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTextureChanged(gdeObjectClass *objectClass, gdeOCComponentTexture *texture){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCTextureChanged(this, objectClass, texture);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCActiveTextureChanged(gdeObjectClass *objectClass){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.OCActiveTextureChanged(this, objectClass);
	});
}

void gdeGameDefinition::NotifyActiveObjectClassChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveObjectClassChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCBillboardChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCBillboardChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCCameraChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCCameraChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCComponentChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCComponentChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCEnvMapProbeChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCEnvMapProbeChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCLightChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCLightChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCNavigationBlockerChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCNavigationBlockerChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCNavigationSpaceChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCNavigationSpaceChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCParticleEmitterChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCParticleEmitterChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCForceFieldChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCForceFieldChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCSnapPointChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCSnapPointChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCSpeakerChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCSpeakerChanged(this);
	});
}

void gdeGameDefinition::NotifyActiveOCWorldChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveOCWorldChanged(this);
	});
}



void gdeGameDefinition::NotifyParticleEmitterStructureChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ParticleEmitterStructureChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyParticleEmitterChanged(gdeParticleEmitter *particleEmitter){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ParticleEmitterChanged(this, particleEmitter);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyParticleEmitterNameChanged(gdeParticleEmitter *particleEmitter){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ParticleEmitterNameChanged(this, particleEmitter);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveParticleEmitterChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveParticleEmitterChanged(this);
	});
}



void gdeGameDefinition::NotifySkinStructureChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkinStructureChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkinChanged(gdeSkin *skin){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkinChanged(this, skin);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkinNameChanged(gdeSkin *skin){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkinNameChanged(this, skin);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveSkinChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveSkinChanged(this);
	});
}



void gdeGameDefinition::NotifySkyStructureChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkyStructureChanged(this);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyChanged(gdeSky *sky){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkyChanged(this, sky);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyNameChanged(gdeSky *sky){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkyNameChanged(this, sky);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveSkyChanged(){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.ActiveSkyChanged(this);
	});
}

void gdeGameDefinition::NotifySkyControllerStructureChanged(gdeSky *sky){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkyControllerStructureChanged(this, sky);
	});
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyControllerChanged(gdeSky *sky, gdeSkyController *controller){
	pListeners.Visit([&](gdeGameDefinitionListener &listener){
		listener.SkyControllerChanged(this, sky, controller);
	});
	
	SetChanged(true);
}



// Private functions
//////////////////////

void gdeGameDefinition::pCleanUp(){
	pListeners.RemoveAll();
	
	GetUndoSystem()->RemoveAll();
	
	RemoveAllSkies();
	RemoveAllSkins();
	RemoveAllParticleEmitters();
	RemoveAllObjectClasses();

	SetActiveCategory(nullptr);
	pCategoriesObjectClass.RemoveAll();
	pCategoriesSkin.RemoveAll();
	pCategoriesSky.RemoveAll();
	pCategoriesParticleEmitter.RemoveAll();
	
	pBaseGameDefinitions.RemoveAll();
	
	if(pSky){
		delete pSky;
	}
	pEnvObject = nullptr;
	if(pCamera){
		delete pCamera;
	}
}
