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

#include "meCamera.h"
#include "meLumimeter.h"
#include "meWorld.h"
#include "meWorldNotifier.h"
#include "meWorldGuiParameters.h"
#include "decal/meDecal.h"
#include "navspace/meNavigationSpace.h"
#include "object/meObject.h"
#include "object/texture/meObjectTexture.h"
#include "pathfinding/mePathFindTest.h"
#include "propfield/mePropField.h"
#include "propfield/mePropFieldType.h"
#include "terrain/meHeightTerrain.h"
#include "terrain/meHeightTerrainSector.h"
#include "weather/meWeather.h"
#include "objectshape/meObjectShape.h"
#include "../configuration/meConfiguration.h"
#include "../gui/meWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceCorner.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deMicrophoneManager.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/forcefield/deForceFieldManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicWorld.h>




// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meWorld
//////////////////

// Constructor, destructor
////////////////////////////

meWorld::meWorld(meWindowMain &windowMain, igdeEnvironment *environment) :
igdeEditableEntity(environment),
pWindowMain(windowMain),
pNextObjectID(1) // 0 is reserved for invalid or undefined IDs
{
	deEngine * const engine = GetEngine();
	
	pDEWorld = nullptr;
	
	pEngColCollider = nullptr;
	pEngForceField = nullptr;
	pSky = nullptr;
	pEngMicrophone = nullptr;
	
	pSize.Set(1000.0, 1000.0, 1000.0);
	pGravity.Set(0.0f, -9.81f, 0.0f);
	pHeightTerrain = nullptr;
	pWeather = nullptr;
	
	pDepChanged = false;
	
	pFreeRoamCamera = nullptr;
	pPlayerCamera = nullptr;
	pActiveCamera = nullptr;
	
	pLumimeter = nullptr;
	pPathFindTest = nullptr;
	
	pFullBright = false;
	
	pGuiParams = nullptr;
	
	try{
		SetFilePath("new.deworld");
		
		pGuiParams = new meWorldGuiParameters(*this);
		
		// create world
		pDEWorld = engine->GetWorldManager()->CreateWorld();
		pDEWorld->SetSize(pSize);
		pDEWorld->SetGravity(pGravity);
		pDEWorld->SetDisableLights(pFullBright);
		pUpdateAmbientLight();
		
		// create collision detection collider
		pEngColCollider = engine->GetColliderManager()->CreateColliderVolume();
		
		// create height terrain
		pHeightTerrain = meHeightTerrain::Ref::New(*this);
		pHeightTerrain->AddSector(meHeightTerrainSector::Ref::New(engine, decPoint()));
		pHeightTerrain->SetSaved(false);
		pHeightTerrain->SetDepChanged(false);
		pHeightTerrain->SetChanged(false);
		
		// create sky
		pSky = new igdeWSky(*environment);
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pDEWorld);
		
		// background object
		pBgObject = igdeWObject::Ref::New(*environment);
		pBgObject->SetWorld(pDEWorld);
		
		// create weather
		pWeather = new meWeather(this);
		
		// create cameras
		pFreeRoamCamera = new meCamera(engine);
		pFreeRoamCamera->SetName("Free Roaming Camera");
		pFreeRoamCamera->SetEnableGI(windowMain.GetConfiguration().GetEnableGI());
		pFreeRoamCamera->SetWorld(this);
		
		pPlayerCamera = new meCamera(engine);
		pPlayerCamera->SetName("Player Camera");
		pPlayerCamera->SetEnableGI(windowMain.GetConfiguration().GetEnableGI());
		pPlayerCamera->SetWorld(this);
		
		pActiveCamera = pFreeRoamCamera;
		
		// create sensors
		pLumimeter = meLumimeter::Ref::New(engine);
		pLumimeter->SetWorld(this);
		
		// create microphone
		pEngMicrophone = engine->GetMicrophoneManager()->CreateMicrophone();
		pEngMicrophone->SetMuted(false);
		pEngMicrophone->SetType(deMicrophone::emtPoint); // directed in fact but that's for later
		decLayerMask layerMaskMicrophone;
		layerMaskMicrophone.SetBit(elmAudio);
		pEngMicrophone->SetLayerMask(layerMaskMicrophone);
		pEngMicrophone->SetEnableAuralization(windowMain.GetConfiguration().GetEnableAuralization());
		pDEWorld->AddMicrophone(pEngMicrophone);
		
		// create path find test
		pPathFindTest = mePathFindTest::Ref::New(engine);
		pPathFindTest->SetWorld(this);
		
		pMusic = meMusic::Ref::New(*this);
		
		// debug drawer
		pDDLimitBox = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDDLimitBox->SetVisible(false);
		pDEWorld->AddDebugDrawer(pDDLimitBox);
		
		pDDSLimitBox = igdeWDebugDrawerShape::Ref::New();
		pDDSLimitBox->SetParentDebugDrawer(pDDLimitBox);
		pDDSLimitBox->SetEdgeColor(decColor(0.75f, 0.0f, 0.75f, 0.8f));
		pDDSLimitBox->SetFillColor(decColor(0.5f, 0.0f, 0.5f, 0.1f));
		
		// make sure all is set properly
		SetChanged(false);
		pDepChanged = false;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meWorld::~meWorld(){
	pCleanUp();
}



// Management
///////////////

void meWorld::SetLimitBoxMinExtend(const decVector &minExtend){
	if(minExtend.IsEqualTo(pLimitBoxMinExtend)){
		return;
	}
	
	pLimitBoxMinExtend = minExtend;
	pUpdateDDLimitBox();
	NotifyLimitBoxChanged();
}

void meWorld::SetLimitBoxMaxExtend(const decVector &maxExtend){
	if(maxExtend.IsEqualTo(pLimitBoxMaxExtend)){
		return;
	}
	
	pLimitBoxMaxExtend = maxExtend;
	pUpdateDDLimitBox();
	NotifyLimitBoxChanged();
}

void meWorld::SetLimitBoxExtends(const decVector &minExtend, const decVector &maxExtend){
	if(minExtend.IsEqualTo(pLimitBoxMinExtend) && maxExtend.IsEqualTo(pLimitBoxMaxExtend)){
		return;
	}
	
	pLimitBoxMinExtend = minExtend;
	pLimitBoxMaxExtend = maxExtend;
	pUpdateDDLimitBox();
	NotifyLimitBoxChanged();
}

void meWorld::SetNextObjectID(const decUniqueID& id){
	if(id == pNextObjectID){
		return;
	}
	
	pNextObjectID = id;
	SetChanged(true);
}

decUniqueID meWorld::NextObjectID(){
	const decUniqueID id(pNextObjectID);
	pNextObjectID.Increment();
	SetChanged(true);
	return id;
}



void meWorld::Dispose(){
	GetUndoSystem()->RemoveAll();
	pSelectionNavigationSpace.Reset();
	pSelectionDecal.Reset();
	pSelectionObjectShape.Reset();
	pSelectionObject.Reset();
	
	pFreeRoamCamera->Dispose();
	pPlayerCamera->Dispose();
	pTriggerTable.RemoveAll();
	
	RemoveAllNavSpaces();
	RemoveAllDecals();
	RemoveAllObjects();
}

void meWorld::Reset(){
	pSelectionNavigationSpace.Reset();
	pSelectionDecal.Reset();
	pSelectionObjectShape.Reset();
	pSelectionObject.Reset();
	
	NotifyObjectSelectionChanged();
}

void meWorld::InitDelegates(){
	if(pHeightTerrain) pHeightTerrain->InitDelegates(GetEnvironment());
}



// Collision Detection
////////////////////////

void meWorld::CollisionTestBox(const decDVector &position, const decQuaternion &orientation,
const decVector &halfExtends, deBaseScriptingCollider *listener, const decCollisionFilter &filter){
	decShapeBox *box = nullptr;
	decShapeList shapeList;
	
	pEngColCollider->SetCollisionFilter(filter);
	
	pEngColCollider->SetPosition(position);
	pEngColCollider->SetOrientation(orientation);
	
	try{
		box = new decShapeBox(halfExtends);
		shapeList.Add(box);
		box = nullptr;
		
	}catch(const deException &){
		if(box){
			delete box;
		}
		throw;
	}
	
	pEngColCollider->SetShapes(shapeList);
	
	pDEWorld->ColliderHits(pEngColCollider, listener);
}

void meWorld::CollisionTestBox(const decDVector &position,const decVector &minExtend, const decVector &maxExtend,
const decQuaternion &orientation, deBaseScriptingCollider *listener, const decCollisionFilter &filter){
	CollisionTestBox(decDMatrix::CreateWorld(position, orientation) * ((minExtend + maxExtend) * 0.5f),
		orientation, (maxExtend - minExtend) * 0.5f, listener, filter);
}



// Editing
////////////

void meWorld::SetDepChanged(bool changed){
	if(changed == pDepChanged){
		return;
	}
	
	pDepChanged = changed;
	NotifyStateChanged();
}



void meWorld::CheckChanged(){
	bool changed = GetChanged();
	changed |= pHeightTerrain && pHeightTerrain->GetChanged();
	SetChanged(changed);
}

void meWorld::CheckDepChanged(){
	bool depChanged = pDepChanged;
	depChanged |= pHeightTerrain && pHeightTerrain->GetDepChanged();
	SetDepChanged(depChanged);
}



void meWorld::ForceUpdateVegetation(bool fullUpdate){
	pHeightTerrain->ForceUpdateVegetation(fullUpdate);
}

void meWorld::ClearVegetation(){
	pHeightTerrain->ClearVegetation();
}



void meWorld::ElementModeChanged(){
	NotifyModeChanged();
	pShowStateChanged();
}

void meWorld::ElementVisibilityChanged(){
	NotifyEditingChanged();
	pShowStateChanged();
}

void meWorld::EnableGIChanged(){
	const bool enable = pWindowMain.GetConfiguration().GetEnableGI();
	
	pFreeRoamCamera->SetEnableGI(enable);
	pPlayerCamera->SetEnableGI(enable);
	
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		meObject &object = *pObjects.GetAt(i);
		if(object.GetCamera()){
			object.GetCamera()->SetEnableGI(enable);
		}
	}
}

void meWorld::EnableAuralizationChanged(){
	const bool enable = pWindowMain.GetConfiguration().GetEnableAuralization();
	
	if(pEngMicrophone){
		pEngMicrophone->SetEnableAuralization(enable);
	}
}

void meWorld::ClearScalingOfNonScaledElements(){
	const decVector unitScale(1.0f, 1.0f, 1.0f);
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		meObject &object = *pObjects.GetAt(i);
		if(object.GetScaleMode() == igdeGDClass::esmFixed){
			object.SetScaling(unitScale);
		}
	}
}



// Objects
////////////



// Sectors
////////////

void meWorld::AddObject(meObject *object){
	DEASSERT_NOTNULL(object)
	DEASSERT_FALSE(pObjects.Has(object))
	
	pObjects.Add(object);
	object->SetWorld(this);
	
	pObjectIDMap.SetAt(object->GetID().ToHexString(), object);
	
	SetChanged(true);
}

void meWorld::RemoveObject(meObject *object){
	DEASSERT_NOTNULL(object)
	DEASSERT_TRUE(pObjects.Has(object))
	
	pObjectIDMap.Remove(object->GetID().ToHexString());
	
	object->SetWorld(nullptr);
	pObjects.Remove(object);
	
	SetChanged(true);
}

void meWorld::RemoveAllObjects(){
	const int count = pObjects.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		pObjects.GetAt(i)->SetWorld(nullptr);
	}
	
	pObjects.RemoveAll();
	pObjectIDMap.RemoveAll();
	
	SetChanged(true);
}

void meWorld::ReassignObjectIDs(){
	const int count = pObjects.GetCount();
	int i;
	
	pObjectIDMap.RemoveAll();
	pNextObjectID = decUniqueID(1);
	
	for(i=0; i<count; i++){
		meObject * const object = pObjects.GetAt(i);
		
		object->SetID(pNextObjectID);
		pNextObjectID.Increment();
		
		pObjectIDMap.SetAt(object->GetID().ToHexString(), object);
	}
	
	SetChanged(true);
}

meObject *meWorld::GetObjectWithID(const decUniqueID &id) const{
	return GetObjectWithID(id.ToHexString());
}

meObject *meWorld::GetObjectWithID(const char *hexID) const{
	return pObjectIDMap.GetAtOrDefault(hexID);
}



// Decals
///////////

void meWorld::AddDecal(meDecal *decal){
	DEASSERT_NOTNULL(decal)
	DEASSERT_FALSE(pDecals.Has(decal))
	
	pDecals.Add(decal);
	decal->SetWorld(this);
	SetChanged(true);
}

void meWorld::RemoveDecal(meDecal *decal){
	DEASSERT_NOTNULL(decal)
	DEASSERT_TRUE(pDecals.Has(decal))
	
	decal->SetWorld(nullptr);
	pDecals.Remove(decal);
	SetChanged(true);
}

void meWorld::RemoveAllDecals(){
	const int count = pDecals.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		pDecals.GetAt(i)->SetWorld(nullptr);
	}
	
	pDecals.RemoveAll();
	
	SetChanged(true);
}



// Navigation Spaces
//////////////////////

void meWorld::AddNavSpace(meNavigationSpace *navspace){
	DEASSERT_NOTNULL(navspace)
	DEASSERT_FALSE(pNavSpaces.Has(navspace))
	
	pNavSpaces.Add(navspace);
	navspace->SetWorld(this);
	SetChanged(true);
}

void meWorld::RemoveNavSpace(meNavigationSpace *navspace){
	DEASSERT_NOTNULL(navspace)
	DEASSERT_TRUE(pNavSpaces.Has(navspace))
	
	navspace->SetWorld(nullptr);
	pNavSpaces.Remove(navspace);
	SetChanged(true);
}

void meWorld::RemoveAllNavSpaces(){
	if(pNavSpaces.IsEmpty()){
		return;
	}
	
	pNavSpaces.Visit([](meNavigationSpace &n){
		n.SetWorld(nullptr);
	});
	pNavSpaces.RemoveAll();
	SetChanged(true);
}



// World Parameters
/////////////////////

void meWorld::SetSize(const decDVector &size){
	if(!(size > decDVector(1.0, 1.0, 1.0))){
		DETHROW(deeInvalidParam);
	}
	if(size.IsEqualTo(pSize)){
		return;
	}
	
	pSize = size;
	pDEWorld->SetSize(size);
	
	NotifyWorldParametersChanged();
}

void meWorld::SetGravity(const decVector &gravity){
	if(gravity.IsEqualTo(pGravity)){
		return;
	}
	
	pGravity = gravity;
	pDEWorld->SetGravity(gravity);
	
	NotifyWorldParametersChanged();
}

void meWorld::SetFullBright(bool fullBright){
	pFullBright = fullBright;
	
	pDEWorld->SetDisableLights(fullBright);
	
	pUpdateAmbientLight();
}



// Properties
///////////////

void meWorld::SetProperty(const char *key, const char *value){
	bool activeChanged = false;
	
	pProperties.SetAt(key, value);
	
	if(pActiveProperty.IsEmpty()){
		pActiveProperty = key;
		activeChanged = true;
	}
	
	NotifyPropertiesChanged();
	if(activeChanged){
		NotifyActivePropertyChanged();
	}
}

void meWorld::SetProperties(const decStringDictionary &properties){
	pProperties = properties;
	
	if(pProperties.IsEmpty()){
		pActiveProperty.Empty();
		
	}else{
		pActiveProperty = pProperties.GetKeys().GetAt(0);
	}
	
	NotifyPropertiesChanged();
	NotifyActivePropertyChanged();
}

void meWorld::RemoveProperty(const char *key){
	if(!pProperties.Has(key)){
		return;
	}
	
	bool activeChanged = false;
	
	pProperties.Remove(key);
	
	if(pActiveProperty == key){
		if(pProperties.IsEmpty()){
			pActiveProperty.Empty();
			
		}else{
			pActiveProperty = pProperties.GetKeys().GetAt(0);
		}
		activeChanged = true;
	}
	
	NotifyPropertiesChanged();
	if(activeChanged){
		NotifyActivePropertyChanged();
	}
}

void meWorld::RemoveAllProperties(){
	if(pProperties.IsEmpty()){
		return;
	}
	
	pProperties.RemoveAll();
	pActiveProperty.Empty();
	
	NotifyPropertiesChanged();
	NotifyActivePropertyChanged();
}

void meWorld::SetActiveProperty(const char *property){
	if(pActiveProperty == property){
		return;
	}
	
	pActiveProperty = property;
	
	NotifyActivePropertyChanged();
}



// Camera
///////////

void meWorld::SetActiveCamera(meCamera *camera){
	if(camera == pActiveCamera){
		return;
	}
	
	pActiveCamera = camera;
	
	const int objectCount = pObjects.GetCount();
	int i;
	for(i=0; i<objectCount; i++){
		pObjects.GetAt(i)->OnActiveCameraChanged();
	}
	
	NotifyActiveCameraChanged();
}



// #define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
#define DEBUG_PRINT_TIMER(what)			GetEnvironment()->GetLogger()->LogInfoFormat("World Editor", "World Timer: %s = %iys", what, (int)(timer.GetElapsedTime() * 1000000.0))
#define DEBUG_PRINT_TIMER_TOTAL()		GetEnvironment()->GetLogger()->LogInfoFormat("World Editor", "World Timer-Total = %iys", (int)(timerTotal.GetElapsedTime() * 1000000.0))
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL()
#endif

void meWorld::ActivateMicrophone(){
	GetEngine()->GetAudioSystem()->SetActiveMicrophone(pEngMicrophone);
}

void meWorld::UpdateDEWorld(float elapsed){
DEBUG_RESET_TIMERS;
	UpdateSensors();
DEBUG_PRINT_TIMER("Update Sensors");
	/*
	pEngForceField->SetForce(pEngForceField->GetForce() + pFFDir * elapsed);
	if(pEngForceField->GetForce() > 350.0f){
		pFFDir = -700.0f;
		pEngForceField->SetForce(350.0f);
	}else if(pEngForceField->GetForce() < 15.0f){
		pFFDir = 700.0f;
		pEngForceField->SetForce(15.0f);
	}
	*/
	/*
	pFFDir = fmodf(pFFDir + elapsed, 130.0f);
	if(pFFDir < 40.0f){
		if(fabsf(pEngForceField->GetForce() - 15.0f) > 1.0f){
			pEngForceField->SetForce(15.0f);
			GetLogger()->LogInfoFormat(LOGSOURCE, "*** Wind force set to 15\n");
		}
	}else if(pFFDir < 70.0f){
		if(fabsf(pEngForceField->GetForce() - 65.0f) > 1.0f){
			pEngForceField->SetForce(65.0f);
			GetLogger()->LogInfoFormat(LOGSOURCE, "*** Wind force set to 65\n");
		}
	}else if(pFFDir < 100.0f){
		if(fabsf(pEngForceField->GetForce() - 350.0f) > 1.0f){
			pEngForceField->SetForce(350.0f);
			GetLogger()->LogInfoFormat(LOGSOURCE, "*** Wind force set to 350\n");
		}
	}else{ // pFFDir < 130.0f
		if(fabsf(pEngForceField->GetForce() - 1400.0f) > 1.0f){
			pEngForceField->SetForce(1400.0f);
			GetLogger()->LogInfoFormat(LOGSOURCE, "*** Wind force set to 1400\n");
		}
	}
	*/
	
	pWeather->Update(elapsed);
DEBUG_PRINT_TIMER("Update Weather");
	
	const int objectCount = pObjects.GetCount();
	int i;
	
	for(i=0; i<objectCount; i++){
		pObjects.GetAt(i)->Update(elapsed);
	}
	DEBUG_PRINT_TIMER("Update Objects");
	
	/*
	const int decalCount = pDecalList.GetCount();
	for(i=0; i<decalCount; i++){
		pDecalList.GetAt(i)->Update(elapsed);
	}
	DEBUG_PRINT_TIMER("Update Decals");
	*/
	
	pHeightTerrain->Update();
DEBUG_PRINT_TIMER("Update Height Terrain");
	
	// update microphone
	if(pActiveCamera){
		pEngMicrophone->SetPosition(pActiveCamera->GetPosition());
		pEngMicrophone->SetOrientation(decMatrix::CreateRotation(pActiveCamera->GetOrientation() * DEG2RAD).ToQuaternion());
	}
DEBUG_PRINT_TIMER("Update Microphone");
	
	// update world
	pDEWorld->Update(elapsed);
DEBUG_PRINT_TIMER("Update World");
	
	// update navigation test path
	pPathFindTest->Update();
	
	// detect collisions
	pDEWorld->ProcessPhysics(elapsed);
DEBUG_PRINT_TIMER("Detect Collisions");
	
	pBgObject->Update(elapsed);
DEBUG_PRINT_TIMER_TOTAL();
}

void meWorld::UpdateSensors(){
	// lumimeter
	if(pLumimeter->GetTrackCamera()){
		pLumimeter->SetPosition(pActiveCamera->GetPosition());
		pLumimeter->SetDirection(pActiveCamera->GetViewMatrix().TransformView());
	}
}

void meWorld::GameDefChanged(){
	int i;
	
	const int objectCount = pObjects.GetCount();
	for(i=0; i<objectCount; i++){
		pObjects.GetAt(i)->OnGameDefinitionChanged();
	}
	
	const int decalCount = pDecals.GetCount();
	for(i=0; i<decalCount; i++){
		pDecals.GetAt(i)->OnGameDefinitionChanged();
	}
}



// Testing
////////////

void meWorld::SetPathNavTest(const char *path){
	pPathNavTest = path;
}



// Notifiers
//////////////

void meWorld::AddNotifier(meWorldNotifier *notifier){
	DEASSERT_NOTNULL(notifier)
	pNotifiers.Add(notifier);
}

void meWorld::RemoveNotifier(meWorldNotifier *notifier){
	pNotifiers.Remove(notifier);
}


void meWorld::NotifyWorldParametersChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.WorldParametersChanged(this);
	});
	
	SetChanged(true);
}

void meWorld::NotifySkyChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.SkyChanged(this);
	});
	
	SetChanged(true); // sky information is saved as world-editor specific data
}

void meWorld::NotifyBgObjectChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.BgObjectChanged(this);
	});
	
	SetChanged(true); // bg object is saved as world-editor specific data
}

void meWorld::NotifyLimitBoxChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.LimitBoxChanged(this);
	});
	
	SetChanged(true); // limit box is saved as world-editor specific data
}

void meWorld::NotifyModeChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ModeChanged(this);
	});
}

void meWorld::NotifyStateChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.StateChanged(this);
	});
}

void meWorld::NotifyUndoChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.UndoChanged(this);
	});
}

void meWorld::NotifyLumimeterChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.LumimeterChanged(this);
	});
}

void meWorld::NotifyPathFindTestChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PathFindTestChanged(this);
	});
}

void meWorld::NotifyMusicChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.MusicChanged(this);
	});
}

void meWorld::NotifyLightingChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.LightingChanged(this);
	});
}

void meWorld::NotifyEditingChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.EditingChanged(this);
	});
}

void meWorld::NotifyHeightPaintChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HeightPaintChanged(this);
	});
}

void meWorld::NotifyMaskPaintChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.MaskPaintChanged(this);
	});
}

void meWorld::NotifyVisibilityPaintChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.VisibilityPaintChanged(this);
	});
}

void meWorld::NotifyTriggerTableChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.TriggerTableChanged(this);
	});
}

void meWorld::NotifyClassHideTagsChanged(){
	pShowStateChanged();
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ClassHideTagsChanged(this);
	});
}

void meWorld::NotifyPropertiesChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PropertiesChanged(this);
	});
	
	SetChanged(true);
}

void meWorld::NotifyActivePropertyChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ActivePropertyChanged(this);
	});
}

void meWorld::NotifyHTChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTChanged(this);
	});
}

void meWorld::NotifyHTStateChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTStateChanged(this);
	});
}

void meWorld::NotifyHTSChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSChanged(this, sector);
	});
}

void meWorld::NotifyHTSHeightChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSHeightChanged(this, sector);
	});
}

void meWorld::NotifyHTSVisibilityChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSVisibilityChanged(this, sector);
	});
}

void meWorld::NotifyHTSTextureCountChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSTextureCountChanged(this, sector);
	});
}

void meWorld::NotifyHTSActiveTextureChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSActiveTextureChanged(this, sector);
	});
}

void meWorld::NotifyHTSTextureChanged(meHeightTerrainSector *sector, meHeightTerrainTexture *texture){
	if(!sector || !texture) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSTextureChanged(this, sector, texture);
	});
}

void meWorld::NotifyHTSTextureMaskChanged(meHeightTerrainSector *sector, meHeightTerrainTexture *texture){
	if(!sector || !texture) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSTextureMaskChanged(this, sector, texture);
	});
}

void meWorld::NotifyHTNavSpaceCountChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTNavSpaceCountChanged(this);
	});
}

void meWorld::NotifyHTActiveNavSpaceChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTActiveNavSpaceChanged(this);
	});
}

void meWorld::NotifyHTNavSpaceChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTNavSpaceChanged(this, navspace);
	});
}

void meWorld::NotifyHTNavSpaceTypeCountChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTNavSpaceTypeCountChanged(this, navspace);
	});
}

void meWorld::NotifyHTNavSpaceActiveTypeChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTNavSpaceActiveTypeChanged(this, navspace);
	});
}

void meWorld::NotifyHTNavSpaceTypeChanged(meHeightTerrainNavSpace *navspace,
meHeightTerrainNavSpaceType *type){
	if(!navspace || !type){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTNavSpaceTypeChanged(this, navspace, type);
	});
}

void meWorld::NotifyHTNavSpaceFacesChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTNavSpaceFacesChanged(this, navspace);
	});
}

void meWorld::NotifyHTNavSpaceSelectedPointsChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTNavSpaceSelectedPointsChanged(this);
	});
}

void meWorld::NotifyHTSPropFieldCountChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSPropFieldCountChanged(this, sector);
	});
}

void meWorld::NotifyHTSActivePropFieldChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSActivePropFieldChanged(this, sector);
	});
}

void meWorld::NotifyHTSPropFieldChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField){
	if(!sector || !propField) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSPropFieldChanged(this, sector, propField);
	});
}

void meWorld::NotifyHTSPropFieldMaskChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField){
	if(!sector || !propField) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSPropFieldMaskChanged(this, sector, propField);
	});
}

void meWorld::NotifyHTSPFTypeCountChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField){
	if(!sector || !propField) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSPFTypeCountChanged(this, sector, propField);
	});
}

void meWorld::NotifyHTSPFTypeChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField, meHeightTerrainPFType *type){
	if(!sector || !propField || !type) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTSPFTypeChanged(this, sector, propField, type);
	});
}

void meWorld::NotifyHTVLayerCountChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLayerCountChanged(this);
	});
}

void meWorld::NotifyHTActiveVLayerChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTActiveVLayerChanged(this);
	});
}

void meWorld::NotifyHTVLayerChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLayerChanged(this, vlayer);
	});
}

void meWorld::NotifyHTVLVariationCountChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLVariationCountChanged(this, vlayer);
	});
}

void meWorld::NotifyHTVLActiveVariationChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLActiveVariationChanged(this, vlayer);
	});
}

void meWorld::NotifyHTVLVariationChanged(meHTVegetationLayer *vlayer, meHTVVariation *variation){
	if(!vlayer || !variation) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLVariationChanged(this, vlayer, variation);
	});
}

void meWorld::NotifyHTVLRuleCountChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLRuleCountChanged(this, vlayer);
	});
}

void meWorld::NotifyHTVLActiveRuleChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLActiveRuleChanged(this, vlayer);
	});
}

void meWorld::NotifyHTVLRuleChanged(meHTVegetationLayer *vlayer, meHTVRule *rule){
	if(!vlayer || !rule) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLRuleChanged(this, vlayer, rule);
	});
}

void meWorld::NotifyHTVLRuleMoved(meHTVegetationLayer *vlayer, meHTVRule *rule){
	if(!vlayer || !rule) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLRuleMoved(this, vlayer, rule);
	});
}

void meWorld::NotifyHTVLLinkCountChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.HTVLLinkCountChanged(this, vlayer);
	});
}

void meWorld::NotifyObjectSelectionChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectSelectionChanged(this);
	});
}

void meWorld::NotifyObjectChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectChanged(this, object);
	});
}

void meWorld::NotifyObjectClassChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectClassChanged(this, object);
	});
}

void meWorld::NotifyObjectGeometryChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectGeometryChanged(this, object);
	});
	
	pHeightTerrain->InvalidatePropFields(object);
}

void meWorld::NotifyObjectPropertiesChanged(meObject *object){
	if(!object){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectPropertiesChanged(this, object);
	});
}

void meWorld::NotifyObjectActivePropertyChanged(meObject *object){
	if(!object){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectActivePropertyChanged(this, object);
	});
}

void meWorld::NotifyObjectAttachBehaviorsChanged(meObject *object){
	DEASSERT_NOTNULL(object)
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectAttachBehaviorsChanged(this, object);
	});
}

void meWorld::NotifyObjectActiveAttachBehaviorChanged(meObject *object){
	DEASSERT_NOTNULL(object)
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectActiveAttachBehaviorChanged(this, object);
	});
}

void meWorld::NotifyObjectAdded(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectAdded(this, object);
	});
}

void meWorld::NotifyObjectRemoved(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectRemoved(this, object);
	});
}

void meWorld::NotifyObjectTextureCountChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectTextureCountChanged(this, object);
	});
}

void meWorld::NotifyObjectActiveTextureChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectActiveTextureChanged(this, object);
	});
}

void meWorld::NotifyObjectTextureChanged(meObject *object, meObjectTexture *texture){
	if(!object || !texture) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectTextureChanged(this, object, texture);
	});
}

void meWorld::NotifyObjectTexturePropertiesChanged(meObject *object, meObjectTexture *texture){
	if(!object || !texture){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectTexturePropertiesChanged(this, object, texture);
	});
}

void meWorld::NotifyObjectTextureActivePropertyChanged(meObject *object, meObjectTexture *texture){
	if(!object || !texture){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectTextureActivePropertyChanged(this, object, texture);
	});
}

void meWorld::NotifyObjectShapeListChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectShapeListChanged(this);
	});
}

void meWorld::NotifyObjectShapeSelectionChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ObjectShapeSelectionChanged(this);
	});
}

void meWorld::NotifyDecalSelectionChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.DecalSelectionChanged(this);
	});
}

void meWorld::NotifyDecalCountChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.DecalCountChanged(this);
	});
}

void meWorld::NotifyDecalChanged(meDecal *decal){
	if(!decal) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.DecalChanged(this, decal);
	});
}

void meWorld::NotifyDecalGeometryChanged(meDecal *decal){
	if(!decal) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.DecalGeometryChanged(this, decal);
	});
}

void meWorld::NotifyDecalPropertiesChanged(meDecal *decal){
	if(!decal){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.DecalPropertiesChanged(this, decal);
	});
}

void meWorld::NotifyDecalActivePropertyChanged(meDecal *decal){
	if(!decal){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.DecalActivePropertyChanged(this, decal);
	});
}

void meWorld::NotifyNavSpaceSelectionChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.NavSpaceSelectionChanged(this);
	});
}

void meWorld::NotifyNavSpaceCountChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.NavSpaceCountChanged(this);
	});
}

void meWorld::NotifyNavSpaceChanged(meNavigationSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.NavSpaceChanged(this, navspace);
	});
	
	pPathFindTest->Invalidate();
}

void meWorld::NotifyNavSpaceGeometryChanged(meNavigationSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.NavSpaceGeometryChanged(this, navspace);
	});
	
	pPathFindTest->Invalidate();
}

void meWorld::NotifyNavSpaceUsedCostTypesChanged(meNavigationSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.NavSpaceUsedCostTypesChanged(this, navspace);
	});
}

void meWorld::NotifyPFSelectionChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PFSelectionChanged(this);
	});
}

void meWorld::NotifyPFCountChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PFCountChanged(this);
	});
}

void meWorld::NotifyPFChanged(mePropField *field){
	if(!field) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PFChanged(this, field);
	});
}

void meWorld::NotifyPFTypeCountChanged(mePropField *field){
	if(!field) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PFTypeCountChanged(this, field);
	});
}

void meWorld::NotifyPFActiveTypeChanged(mePropField *field){
	if(!field) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PFActiveTypeChanged(this, field);
	});
}

void meWorld::NotifyPFTypeChanged(mePropField *field, mePropFieldType *type){
	if(!field || !type) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.PFTypeChanged(this, field, type);
	});
}

void meWorld::NotifyActiveCameraChanged(){
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.ActiveCameraChanged(this);
	});
}

void meWorld::NotifyCameraChanged(meCamera *camera){
	if(!camera) DETHROW(deeInvalidParam);
	pNotifiers.Visit([&](meWorldNotifier &l){
		l.CameraChanged(this, camera);
	});
}



// Private Functions
//////////////////////

void meWorld::pCleanUp(){
	pNotifiers.RemoveAll();
	
	pSelectionNavigationSpace.RemoveAll();
	pSelectionDecal.RemoveAll();
	pSelectionObjectShape.RemoveAll();
	pSelectionObject.RemoveAll();
	
	pObjectShapes.RemoveAll();
	
	RemoveAllNavSpaces();
	RemoveAllDecals();
	RemoveAllObjects();
	
	pHeightTerrain = nullptr;
	
	if(pPlayerCamera){
		delete pPlayerCamera;
	}
	if(pFreeRoamCamera){
		delete pFreeRoamCamera;
	}
	if(pWeather){
		delete pWeather;
	}
	
	pMusic = nullptr;
	if(pPathFindTest){
		pPathFindTest->SetWorld(nullptr);
	}
	if(pLumimeter){
		pLumimeter->SetWorld(nullptr);
	}
	
	if(pGuiParams){
		delete pGuiParams;
	}
	pBgObject = nullptr;
	if(pSky){
		delete pSky;
	}
	if(pDEWorld){
		if(pEngMicrophone){
			if(GetEngine()->GetAudioSystem()->GetActiveMicrophone() == pEngMicrophone){
				GetEngine()->GetAudioSystem()->SetActiveMicrophone(nullptr);
			}
			
			pDEWorld->RemoveMicrophone(pEngMicrophone);
		}
	}
}

void meWorld::pUpdateAmbientLight(){
	if(pFullBright){
		pDEWorld->SetAmbientLight(decColor(1.0f, 1.0f, 1.0f));
		
	}else{
		pDEWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f));
	}
}

void meWorld::pShowStateChanged(){
	int i;
	
	pHeightTerrain->ShowStateChanged();
	
	const int objectCount = pObjects.GetCount();
	for(i=0; i<objectCount; i++){
		pObjects.GetAt(i)->ShowStateChanged();
	}
	
	const int navSpaceCount = pNavSpaces.GetCount();
	for(i=0; i<navSpaceCount; i++){
		pNavSpaces.GetAt(i)->ShowStateChanged();
	}
	
	const int decalCount = pDecals.GetCount();
	for(i=0; i<decalCount; i++){
		pDecals.GetAt(i)->ShowStateChanged();
	}
	
	const int objectShapeCount = pObjectShapes.GetCount();
	for(i=0; i<objectShapeCount; i++){
		pObjectShapes.GetAt(i)->ShowStateChanged();
	}
}

void meWorld::pUpdateDDLimitBox(){
	pDDSLimitBox->RemoveAllShapes();
	
	if(!(pLimitBoxMaxExtend - pLimitBoxMinExtend).IsZero()){
		pDDSLimitBox->AddBoxShape(
			decVector(0.001f, 0.001f, 0.001f).Largest(pLimitBoxMaxExtend - pLimitBoxMinExtend) * 0.5f,
			(pLimitBoxMaxExtend + pLimitBoxMinExtend) * 0.5f, decQuaternion());
		pDDLimitBox->SetVisible(true);
		
	}else{
		pDDLimitBox->SetVisible(false);
	}
}
