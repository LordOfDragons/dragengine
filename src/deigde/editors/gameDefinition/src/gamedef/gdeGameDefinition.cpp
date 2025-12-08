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

pWorld(NULL),

pSky(NULL),

pCamera(NULL),
pViewRatio(1.0f),

pIsProjectGameDef(false),

pVFSPath("/"),

pActiveCategory(NULL),
pActiveObjectClass(NULL),
pActiveOCBillboard(NULL),
pActiveOCCamera(NULL),
pActiveOCComponent(NULL),
pActiveOCEnvMapProbe(NULL),
pActiveOCLight(NULL),
pActiveOCNavigationBlocker(NULL),
pActiveOCNavigationSpace(NULL),
pActiveOCParticleEmitter(NULL),
pActiveOCForceField(NULL),
pActiveOCSnapPoint(NULL),
pActiveOCSpeaker(NULL),
pActiveParticleEmitter(NULL),
pActiveSkin(NULL),
pActiveSky(NULL),

pSelectedObjectType(eotNoSelection),

pVFS(NULL),
pPreviewVFS(NULL)
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
		pEnvObject.TakeOver(new igdeWObject(*environment));
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
		pPreviewVFS = NULL;
			pVFS = NULL;
		}
	}
	
	NotifyBasePathChanged();
}

void gdeGameDefinition::SetVFSPath(const char *path){
	if(pVFSPath == path){
		return;
	}
	
	pVFSPath = path;
	
	if(!pIsProjectGameDef){
		pPreviewVFS = NULL;
			pVFS = NULL;
		}
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
	decObjectList list;
	int i, j;
	
	const int count = pBaseGameDefinitionIDList.GetCount();
	for(i=0; i<count; i++){
		const decString &id = pBaseGameDefinitionIDList.GetAt(i);
		
		// check if game definition has been already loaded in the previous call to
		// UpdateBaseGameDefinitions(). avoids reloading all base game definitions
		for(j=0; j<oldCount; j++){
			gdeGameDefinition * const checkGameDef = (gdeGameDefinition*)pBaseGameDefinitions.GetAt(j);
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
			gameDefinition.TakeOver(loadSaveSystem.LoadGameDefinition(path));
			
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
	const gdeCategory *category = pCategoriesObjectClass.GetWithPath(path);
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
	
	return NULL;
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
		pObjectClassCategoryNameList += ((gdeGameDefinition*)pBaseGameDefinitions.GetAt(i))->GetObjectClassCategoryNameList();
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
	
	pUsedTagsObjectClass.SortAscending();
	
	NotifyObjectClassUsedTagsChanged();
}



void gdeGameDefinition::NotifyWorldPropertiesChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->WorldPropertiesChanged(this);
	}
}

void gdeGameDefinition::NotifyWorldPropertyChanged(gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->WorldPropertyChanged(this, property);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyWorldPropertyNameChanged(gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->WorldPropertyNameChanged(this, property);
	}
	
	SetChanged(true);
}



void gdeGameDefinition::NotifyDecalPropertiesChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->DecalPropertiesChanged(this);
	}
}

void gdeGameDefinition::NotifyDecalPropertyChanged(gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->DecalPropertyChanged(this, property);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyDecalPropertyNameChanged(gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->DecalPropertyNameChanged(this, property);
	}
	
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
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->AutoFindPathObjectClassesChanged(this);
	}
	
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
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->AutoFindPathSkinsChanged(this);
	}
	
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
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->AutoFindPathSkiesChanged(this);
	}
	
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
		
			pVFS = NULL;
		}
		
	}else{
		if(!pVFS){
			deVFSDiskDirectory *container = NULL;
			
			try{
				pVFS = new deVirtualFileSystem;
				
				container = new deVFSDiskDirectory(decPath::CreatePathUnix(pVFSPath),
					decPath::CreatePathNative(pBasePath));
				container->SetReadOnly(true);
				pVFS->AddContainer(container);
				container->FreeReference();
				
			}catch(const deException &){
				if(container){
					container->FreeReference();
				}
					pVFS = NULL;
				}
				
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
	pObjectClasses.Add(objectClass);
	objectClass->SetGameDefinition(this);
	
	pClassNameList.RemoveAll();
	
	NotifyObjectClassStructureChanged();
	
	if(!pActiveObjectClass){
		SetActiveObjectClass(objectClass);
	}
}

void gdeGameDefinition::RemoveObjectClass(gdeObjectClass *objectClass){
	if(!objectClass || objectClass->GetGameDefinition() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(objectClass == pActiveObjectClass){
		SetActiveOCBillboard(NULL);
		SetActiveOCCamera(NULL);
		SetActiveOCComponent(NULL);
		SetActiveOCEnvMapProbe(NULL);
		SetActiveOCLight(NULL);
		SetActiveOCNavigationBlocker(NULL);
		SetActiveOCNavigationSpace(NULL);
		SetActiveOCParticleEmitter(NULL);
		SetActiveOCForceField(NULL);
		SetActiveOCSnapPoint(NULL);
		SetActiveOCSpeaker(NULL);
		SetActiveOCWorld(nullptr);
		
		if(pObjectClasses.GetCount() == 1){
			SetActiveObjectClass(NULL);
			
		}else{
			if(pObjectClasses.GetAt(0) == objectClass){
				SetActiveObjectClass(pObjectClasses.GetAt(1));
				
			}else{
				SetActiveObjectClass(pObjectClasses.GetAt(0));
			}
		}
	}
	
	objectClass->SetGameDefinition(NULL);
	pObjectClasses.Remove(objectClass);
	
	pClassNameList.RemoveAll();
	
	NotifyObjectClassStructureChanged();
}

void gdeGameDefinition::RemoveAllObjectClasses(){
	const int count = pObjectClasses.GetCount();
	int i;
	
	SetActiveOCSpeaker(NULL);
	SetActiveOCSnapPoint(NULL);
	SetActiveOCParticleEmitter(NULL);
	SetActiveOCForceField(NULL);
	SetActiveOCNavigationSpace(NULL);
	SetActiveOCNavigationBlocker(NULL);
	SetActiveOCLight(NULL);
	SetActiveOCEnvMapProbe(NULL);
	SetActiveOCComponent(NULL);
	SetActiveOCCamera(NULL);
	SetActiveOCBillboard(NULL);
	SetActiveOCWorld(nullptr);
	SetActiveObjectClass(NULL);
	
	for(i=0; i<count; i++){
		pObjectClasses.GetAt(i)->SetGameDefinition(NULL);
	}
	pObjectClasses.RemoveAll();
	
	pClassNameList.RemoveAll();
	
	NotifyObjectClassStructureChanged();
}

const gdeObjectClass * const gdeGameDefinition::FindObjectClass(const char *name) const{
	const gdeObjectClass *objectClass = pObjectClasses.GetNamed(name);
	if(objectClass){
		return objectClass;
	}
	
	const int count = pBaseGameDefinitions.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		objectClass = ((const gdeGameDefinition *)pBaseGameDefinitions.GetAt(i))->FindObjectClass(name);
		if(objectClass){
			return objectClass;
		}
	}
	
	return NULL;
}

const decStringSet &gdeGameDefinition::GetClassNameList(){
	if(pClassNameList.GetCount() > 0){
		return pClassNameList;
	}
	
	int i, count = pObjectClasses.GetCount();
	for(i=0; i<count; i++){
		pClassNameList.Add(pObjectClasses.GetAt(i)->GetName());
	}
	
	count = pBaseGameDefinitions.GetCount();
	for(i=0; i<count; i++){
		pClassNameList += ((gdeGameDefinition*)pBaseGameDefinitions.GetAt(i))->GetClassNameList();
	}
	
	return pClassNameList;
}



bool gdeGameDefinition::HasActiveObjectClass() const{
	return pActiveObjectClass != NULL;
}

void gdeGameDefinition::SetActiveObjectClass(gdeObjectClass *objectClass){
	if(objectClass == pActiveObjectClass){
		return;
	}
	
	
	pActiveObjectClass = objectClass;
	
	NotifyActiveObjectClassChanged();
}



bool gdeGameDefinition::HasActiveOCBillboard() const{
	return pActiveOCBillboard != NULL;
}

void gdeGameDefinition::SetActiveOCBillboard(gdeOCBillboard *billboard){
	if(billboard == pActiveOCBillboard){
		return;
	}
	
	
	pActiveOCBillboard = billboard;
	
	NotifyActiveOCBillboardChanged();
}



bool gdeGameDefinition::HasActiveOCCamera() const{
	return pActiveOCCamera != NULL;
}

void gdeGameDefinition::SetActiveOCCamera(gdeOCCamera *camera){
	if(camera == pActiveOCCamera){
		return;
	}
	
	
	pActiveOCCamera = camera;
	
	NotifyActiveOCCameraChanged();
}



bool gdeGameDefinition::HasActiveOCComponent() const{
	return pActiveOCComponent != NULL;
}

void gdeGameDefinition::SetActiveOCComponent(gdeOCComponent *component){
	if(component == pActiveOCComponent){
		return;
	}
	
	
	pActiveOCComponent = component;
	
	NotifyActiveOCComponentChanged();
}



bool gdeGameDefinition::HasActiveOCEnvMapProbe() const{
	return pActiveOCEnvMapProbe != NULL;
}

void gdeGameDefinition::SetActiveOCEnvMapProbe(gdeOCEnvMapProbe *envMapProbe){
	if(envMapProbe == pActiveOCEnvMapProbe){
		return;
	}
	
	
	pActiveOCEnvMapProbe = envMapProbe;
	
	NotifyActiveOCEnvMapProbeChanged();
}



bool gdeGameDefinition::HasActiveOCLight() const{
	return pActiveOCLight != NULL;
}

void gdeGameDefinition::SetActiveOCLight(gdeOCLight *light){
	if(light == pActiveOCLight){
		return;
	}
	
	
	pActiveOCLight = light;
	
	NotifyActiveOCLightChanged();
}



bool gdeGameDefinition::HasActiveOCNavigationBlocker() const{
	return pActiveOCNavigationBlocker != NULL;
}

void gdeGameDefinition::SetActiveOCNavigationBlocker(gdeOCNavigationBlocker *navblocker){
	if(navblocker == pActiveOCNavigationBlocker){
		return;
	}
	
	
	pActiveOCNavigationBlocker = navblocker;
	
	NotifyActiveOCNavigationBlockerChanged();
}



bool gdeGameDefinition::HasActiveOCNavigationSpace() const{
	return pActiveOCNavigationSpace != NULL;
}

void gdeGameDefinition::SetActiveOCNavigationSpace(gdeOCNavigationSpace *navSpace){
	if(navSpace == pActiveOCNavigationSpace){
		return;
	}
	
	
	pActiveOCNavigationSpace = navSpace;
	
	NotifyActiveOCNavigationSpaceChanged();
}



bool gdeGameDefinition::HasActiveOCParticleEmitter() const{
	return pActiveOCParticleEmitter != NULL;
}

void gdeGameDefinition::SetActiveOCParticleEmitter(gdeOCParticleEmitter *emitter){
	if(emitter == pActiveOCParticleEmitter){
		return;
	}
	
	
	pActiveOCParticleEmitter = emitter;
	
	NotifyActiveOCParticleEmitterChanged();
}



bool gdeGameDefinition::HasActiveOCForceField() const{
	return pActiveOCForceField != NULL;
}

void gdeGameDefinition::SetActiveOCForceField(gdeOCForceField *field){
	if(field == pActiveOCForceField){
		return;
	}
	
	
	pActiveOCForceField = field;
	
	NotifyActiveOCForceFieldChanged();
}



bool gdeGameDefinition::HasActiveOCSnapPoint() const{
	return pActiveOCSnapPoint != NULL;
}

void gdeGameDefinition::SetActiveOCSnapPoint(gdeOCSnapPoint *snappoint){
	if(snappoint == pActiveOCSnapPoint){
		return;
	}
	
	
	pActiveOCSnapPoint = snappoint;
	
	NotifyActiveOCSnapPointChanged();
}



bool gdeGameDefinition::HasActiveOCSpeaker() const{
	return pActiveOCSpeaker != NULL;
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
	pParticleEmitters.Add(particleEmitter);
	particleEmitter->SetGameDefinition(this);
	NotifyParticleEmitterStructureChanged();
	
	if(!pActiveParticleEmitter){
		SetActiveParticleEmitter(particleEmitter);
	}
}

void gdeGameDefinition::RemoveParticleEmitter(gdeParticleEmitter *particleEmitter){
	if(!particleEmitter || particleEmitter->GetGameDefinition() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(particleEmitter == pActiveParticleEmitter){
		if(pParticleEmitters.GetCount() == 1){
			SetActiveParticleEmitter(NULL);
			
		}else{
			if(pParticleEmitters.GetAt(0) == particleEmitter){
				SetActiveParticleEmitter(pParticleEmitters.GetAt(1));
				
			}else{
				SetActiveParticleEmitter(pParticleEmitters.GetAt(0));
			}
		}
	}
	
	particleEmitter->SetGameDefinition(NULL);
	pParticleEmitters.Remove(particleEmitter);
	NotifyParticleEmitterStructureChanged();
}

void gdeGameDefinition::RemoveAllParticleEmitters(){
	const int count = pParticleEmitters.GetCount();
	int i;
	
	SetActiveParticleEmitter(NULL);
	
	for(i=0; i<count; i++){
		pParticleEmitters.GetAt(i)->SetGameDefinition(NULL);
	}
	pParticleEmitters.RemoveAll();
	NotifyParticleEmitterStructureChanged();
}

bool gdeGameDefinition::HasActiveParticleEmitter() const{
	return pActiveParticleEmitter != NULL;
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
	pSkins.Add(skin);
	skin->SetGameDefinition(this);
	NotifySkinStructureChanged();
	
	if(!pActiveSkin){
		SetActiveSkin(skin);
	}
}

void gdeGameDefinition::RemoveSkin(gdeSkin *skin){
	if(!skin || skin->GetGameDefinition() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(skin == pActiveSkin){
		if(pSkins.GetCount() == 1){
			SetActiveSkin(NULL);
			
		}else{
			if(pSkins.GetAt(0) == skin){
				SetActiveSkin(pSkins.GetAt(1));
				
			}else{
				SetActiveSkin(pSkins.GetAt(0));
			}
		}
	}
	
	skin->SetGameDefinition(NULL);
	pSkins.Remove(skin);
	NotifySkinStructureChanged();
}

void gdeGameDefinition::RemoveAllSkins(){
	const int count = pSkins.GetCount();
	int i;
	
	SetActiveSkin(NULL);
	
	for(i=0; i<count; i++){
		pSkins.GetAt(i)->SetGameDefinition(NULL);
	}
	pSkins.RemoveAll();
	NotifySkinStructureChanged();
}

bool gdeGameDefinition::HasActiveSkin() const{
	return pActiveSkin != NULL;
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
	pSkies.Add(sky);
	sky->SetGameDefinition(this);
	NotifySkyStructureChanged();
	
	if(!pActiveSky){
		SetActiveSky(sky);
	}
}

void gdeGameDefinition::RemoveSky(gdeSky *sky){
	if(!sky || sky->GetGameDefinition() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(sky == pActiveSky){
		if(pSkies.GetCount() == 1){
			SetActiveSky(NULL);
			
		}else{
			if(pSkies.GetAt(0) == sky){
				SetActiveSky(pSkies.GetAt(1));
				
			}else{
				SetActiveSky(pSkies.GetAt(0));
			}
		}
	}
	
	sky->SetGameDefinition(NULL);
	pSkies.Remove(sky);
	NotifySkyStructureChanged();
}

void gdeGameDefinition::RemoveAllSkies(){
	const int count = pSkies.GetCount();
	int i;
	
	SetActiveSky(NULL);
	
	for(i=0; i<count; i++){
		pSkies.GetAt(i)->SetGameDefinition(NULL);
	}
	pSkies.RemoveAll();
	NotifySkyStructureChanged();
}

bool gdeGameDefinition::HasActiveSky() const{
	return pActiveSky != NULL;
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
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void gdeGameDefinition::RemoveListener(gdeGameDefinitionListener *listener){
	pListeners.RemoveIfPresent(listener);
}



void gdeGameDefinition::NotifyStateChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->StateChanged(this);
	}
}

void gdeGameDefinition::NotifyUndoChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->UndoChanged(this);
	}
}

void gdeGameDefinition::NotifyViewChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ViewChanged(this);
	}
}

void gdeGameDefinition::NotifyGameDefinitionChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->GameDefinitionChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyBasePathChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->BasePathChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyBaseGameDefinitionsChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->BaseGameDefinitionsChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkyChanged(this);
	}
}

void gdeGameDefinition::NotifyEnvObjectChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->EnvObjectChanged(this);
	}
}

void gdeGameDefinition::NotifyCameraChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->CameraChanged(this);
	}
}

void gdeGameDefinition::NotifySelectedObjectChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SelectedObjectChanged(this);
	}
}



void gdeGameDefinition::NotifyObjectClassCategoriesChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ObjectClassCategoriesChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkinCategoriesChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkinCategoriesChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyCategoriesChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkyCategoriesChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyParticleEmitterCategoriesChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ParticleEmitterCategoriesChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveCategoryChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveCategoryChanged(this);
	}
}



void gdeGameDefinition::NotifyObjectClassUsedTagsChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ObjectClassUsedTagsChanged(this);
	}
}

void gdeGameDefinition::NotifyDefinedUsedIDsChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->DefinedUsedIDsChanged(this);
	}
}



void gdeGameDefinition::NotifyObjectClassStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ObjectClassStructureChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyObjectClassChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ObjectClassChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyObjectClassNameChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ObjectClassNameChanged(this, objectClass);
	}
	
	pClassNameList.RemoveAll();
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertyChanged(gdeObjectClass *objectClass, gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCPropertyChanged(this, objectClass, property);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertyNameChanged(gdeObjectClass *objectClass, gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCPropertyNameChanged(this, objectClass, property);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertiesChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCPropertiesChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCPropertyValuesChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCPropertyValuesChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturePropertyChanged(gdeObjectClass *objectClass, gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCTexturePropertyChanged(this, objectClass, property);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturePropertyNameChanged(gdeObjectClass *objectClass, gdeProperty *property){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCTexturePropertyNameChanged(this, objectClass, property);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturePropertiesChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCTexturePropertiesChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCInheritChanged(gdeObjectClass *objectClass, gdeOCInherit *inherit){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCInheritChanged(this, objectClass, inherit);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCInheritsChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCInheritsChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCBillboardsChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCBillboardsChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCBillboardChanged(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCBillboardChanged(this, objectClass, billboard);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCCamerasChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCCamerasChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCCameraChanged(gdeObjectClass *objectClass, gdeOCCamera *camera){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCCameraChanged(this, objectClass, camera);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentsChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCComponentsChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentChanged(gdeObjectClass *objectClass, gdeOCComponent *component){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCComponentChanged(this, objectClass, component);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentActiveTextureChanged(
gdeObjectClass *objectClass, gdeOCComponent *component){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCComponentActiveTextureChanged(this, objectClass, component);
	}
}

void gdeGameDefinition::NotifyOCComponentTextureChanged(gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCComponentTextureChanged(this, objectClass, component, texture);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentTextureNameChanged(gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCComponentTextureNameChanged(this, objectClass, component, texture);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCComponentTexturePropertiesChanged(gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCComponentTexturePropertiesChanged(this, objectClass, component, texture);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCEnvMapProbesChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCEnvMapProbesChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCEnvMapProbeChanged(gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))
			->OCEnvMapProbeChanged(this, objectClass, envMapProbe);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCLightsChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCLightsChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCLightChanged(gdeObjectClass *objectClass, gdeOCLight *light){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCLightChanged(this, objectClass, light);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationBlockersChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCNavigationBlockersChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationBlockerChanged(
gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCNavigationBlockerChanged(this, objectClass, navblocker);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationSpacesChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCNavigationSpacesChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCNavigationSpaceChanged(
gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCNavigationSpaceChanged(this, objectClass, navspace);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCParticleEmittersChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCParticleEmittersChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCParticleEmitterChanged(
gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCParticleEmitterChanged(this, objectClass, emitter);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCForceFieldsChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCForceFieldsChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCForceFieldChanged(
gdeObjectClass *objectClass, gdeOCForceField *field){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCForceFieldChanged(this, objectClass, field);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSnapPointsChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCSnapPointsChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSnapPointChanged(gdeObjectClass *objectClass,
gdeOCSnapPoint *snappoint){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCSnapPointChanged(this, objectClass, snappoint);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSnapPointNameChanged(gdeObjectClass *objectClass,
gdeOCSnapPoint *snappoint){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCSnapPointNameChanged(this, objectClass, snappoint);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSpeakersChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCSpeakersChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCSpeakerChanged(gdeObjectClass *objectClass, gdeOCSpeaker *speaker){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			OCSpeakerChanged(this, objectClass, speaker);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCWorldsChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCWorldsChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCWorldChanged(gdeObjectClass *objectClass, gdeOCWorld *world){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCWorldChanged(this, objectClass, world);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTexturesChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCTexturesChanged(this, objectClass);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCTextureChanged(gdeObjectClass *objectClass, gdeOCComponentTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCTextureChanged(this, objectClass, texture);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyOCActiveTextureChanged(gdeObjectClass *objectClass){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->OCActiveTextureChanged(this, objectClass);
	}
}

void gdeGameDefinition::NotifyActiveObjectClassChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveObjectClassChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCBillboardChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCBillboardChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCCameraChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCCameraChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCComponentChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCComponentChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCEnvMapProbeChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCEnvMapProbeChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCLightChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCLightChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCNavigationBlockerChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCNavigationBlockerChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCNavigationSpaceChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCNavigationSpaceChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCParticleEmitterChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCParticleEmitterChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCForceFieldChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCForceFieldChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCSnapPointChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCSnapPointChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCSpeakerChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCSpeakerChanged(this);
	}
}

void gdeGameDefinition::NotifyActiveOCWorldChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveOCWorldChanged(this);
	}
}



void gdeGameDefinition::NotifyParticleEmitterStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ParticleEmitterStructureChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyParticleEmitterChanged(gdeParticleEmitter *particleEmitter){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ParticleEmitterChanged(this, particleEmitter);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyParticleEmitterNameChanged(gdeParticleEmitter *particleEmitter){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ParticleEmitterNameChanged(this, particleEmitter);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveParticleEmitterChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveParticleEmitterChanged(this);
	}
}



void gdeGameDefinition::NotifySkinStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkinStructureChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkinChanged(gdeSkin *skin){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkinChanged(this, skin);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkinNameChanged(gdeSkin *skin){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkinNameChanged(this, skin);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveSkinChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveSkinChanged(this);
	}
}



void gdeGameDefinition::NotifySkyStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkyStructureChanged(this);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyChanged(gdeSky *sky){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkyChanged(this, sky);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyNameChanged(gdeSky *sky){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->SkyNameChanged(this, sky);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifyActiveSkyChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->ActiveSkyChanged(this);
	}
}

void gdeGameDefinition::NotifySkyControllerStructureChanged(gdeSky *sky){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			SkyControllerStructureChanged(this, sky);
	}
	
	SetChanged(true);
}

void gdeGameDefinition::NotifySkyControllerChanged(gdeSky *sky, gdeSkyController *controller){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((gdeGameDefinitionListener*)pListeners.GetAt(i))->
			SkyControllerChanged(this, sky, controller);
	}
	
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

	SetActiveCategory(NULL);
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
