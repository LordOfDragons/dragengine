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
	
	pDEWorld = NULL;
	
	pEngColCollider = NULL;
	pEngForceField = NULL;
	pSky = NULL;
	pEngMicrophone = NULL;
	
	pSize.Set(1000.0, 1000.0, 1000.0);
	pGravity.Set(0.0f, -9.81f, 0.0f);
	pHeightTerrain = NULL;
	pWeather = NULL;
	
	pDepChanged = false;
	
	pFreeRoamCamera = NULL;
	pPlayerCamera = NULL;
	pActiveCamera = NULL;
	
	pLumimeter = NULL;
	pPathFindTest = NULL;
	
	pFullBright = false;
	
	pGuiParams = NULL;
	
	pNotifiers = NULL;
	pNotifierCount = 0;
	pNotifierSize = 0;
	
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
		pHeightTerrain.TakeOver(new meHeightTerrain(*this));
		
		meHeightTerrainSector * const htsector = new meHeightTerrainSector(engine, decPoint());
		pHeightTerrain->AddSector(htsector);
		htsector->FreeReference();
		
		pHeightTerrain->SetSaved(false);
		pHeightTerrain->SetDepChanged(false);
		pHeightTerrain->SetChanged(false);
		
		// create sky
		pSky = new igdeWSky(*environment);
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pDEWorld);
		
		// background object
		pBgObject.TakeOver(new igdeWObject(*environment));
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
		pLumimeter.TakeOver(new meLumimeter(engine));
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
		pPathFindTest.TakeOver(new mePathFindTest(engine));
		pPathFindTest->SetWorld(this);
		
		pMusic.TakeOver(new meMusic(*this));
		
		// debug drawer
		pDDLimitBox.TakeOver(engine->GetDebugDrawerManager()->CreateDebugDrawer());
		pDDLimitBox->SetVisible(false);
		pDEWorld->AddDebugDrawer(pDDLimitBox);
		
		pDDSLimitBox.SetParentDebugDrawer(pDDLimitBox);
		pDDSLimitBox.SetEdgeColor(decColor(0.75f, 0.0f, 0.75f, 0.8f));
		pDDSLimitBox.SetFillColor(decColor(0.5f, 0.0f, 0.5f, 0.1f));
		
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
	decShapeBox *box = NULL;
	decShapeList shapeList;
	
	pEngColCollider->SetCollisionFilter(filter);
	
	pEngColCollider->SetPosition(position);
	pEngColCollider->SetOrientation(orientation);
	
	try{
		box = new decShapeBox(halfExtends);
		shapeList.Add(box);
		box = NULL;
		
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
	
	object->SetWorld(NULL);
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
		pObjects.GetAt(i)->SetWorld(NULL);
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
	deObject *object;
	if(pObjectIDMap.GetAt(hexID, &object)){
		return (meObject*)object;
		
	}else{
		return NULL;
	}
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
	
	decal->SetWorld(NULL);
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
		pDecals.GetAt(i)->SetWorld(NULL);
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
	const int count = pNavSpaces.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		pNavSpaces.GetAt(i)->SetWorld(NULL);
	}
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
	
	if(pProperties.GetCount() == 0){
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
		if(pProperties.GetCount() == 0){
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
	if(pProperties.GetCount() == 0){
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
#include "dragengine/common/utils/decTimer.h"
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

meWorldNotifier *meWorld::GetNotifierAt(int index) const{
	if(index < 0 || index >= pNotifierCount) DETHROW(deeInvalidParam);
	
	return pNotifiers[index];
}

int meWorld::IndexOfNotifier(meWorldNotifier *notifier) const{
	if(!notifier) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		if(notifier == pNotifiers[i]) return i;
	}
	
	return -1;
}

bool meWorld::HasNotifier(meWorldNotifier *notifier) const{
	if(!notifier) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		if(notifier == pNotifiers[i]) return true;
	}
	
	return false;
}

void meWorld::AddNotifier(meWorldNotifier *notifier){
	if(HasNotifier(notifier)) DETHROW(deeInvalidParam);
	
	if(pNotifierCount == pNotifierSize){
		int newSize = pNotifierSize * 3 / 2 + 1;
		meWorldNotifier **newArray = new meWorldNotifier*[newSize];
		if(pNotifiers){
			memcpy(newArray, pNotifiers, sizeof(meWorldNotifier*) * pNotifierSize);
			delete [] pNotifiers;
		}
		pNotifiers = newArray;
		pNotifierSize = newSize;
	}
	
	pNotifiers[pNotifierCount] = notifier;
	pNotifierCount++;
}

void meWorld::RemoveNotifier(meWorldNotifier *notifier){
	int i, index = IndexOfNotifier(notifier);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pNotifierCount; i++){
		pNotifiers[i - 1] = pNotifiers[i];
	}
	pNotifierCount--;
}

void meWorld::RemoveAllNotifiers(){
	while(pNotifierCount > 0){
		pNotifierCount--;
		pNotifiers[pNotifierCount]->FreeReference();
	}
}



void meWorld::NotifyWorldParametersChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->WorldParametersChanged(this);
	}
	
	SetChanged(true);
}

void meWorld::NotifySkyChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->SkyChanged(this);
	}
	
	SetChanged(true); // sky information is saved as world-editor specific data
}

void meWorld::NotifyBgObjectChanged(){
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->BgObjectChanged(this);
	}
	
	SetChanged(true); // bg object is saved as world-editor specific data
}

void meWorld::NotifyLimitBoxChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->LimitBoxChanged(this);
	}
	
	SetChanged(true); // limit box is saved as world-editor specific data
}

void meWorld::NotifyModeChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ModeChanged(this);
	}
}

void meWorld::NotifyStateChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->StateChanged(this);
	}
}

void meWorld::NotifyUndoChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->UndoChanged(this);
	}
}

void meWorld::NotifyLumimeterChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->LumimeterChanged(this);
	}
}

void meWorld::NotifyPathFindTestChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PathFindTestChanged(this);
	}
}

void meWorld::NotifyMusicChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->MusicChanged(this);
	}
}

void meWorld::NotifyLightingChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->LightingChanged(this);
	}
}

void meWorld::NotifyEditingChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->EditingChanged(this);
	}
}

void meWorld::NotifyHeightPaintChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HeightPaintChanged(this);
	}
}

void meWorld::NotifyMaskPaintChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->MaskPaintChanged(this);
	}
}

void meWorld::NotifyVisibilityPaintChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->VisibilityPaintChanged(this);
	}
}

void meWorld::NotifyTriggerTableChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->TriggerTableChanged(this);
	}
}

void meWorld::NotifyClassHideTagsChanged(){
	int i;
	
	pShowStateChanged();
	
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->ClassHideTagsChanged(this);
	}
}



void meWorld::NotifyPropertiesChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->PropertiesChanged(this);
	}
	
	SetChanged(true);
}

void meWorld::NotifyActivePropertyChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->ActivePropertyChanged(this);
	}
}



void meWorld::NotifyHTChanged(){
	int n;
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTChanged(this);
	}
}

void meWorld::NotifyHTStateChanged(){
	int n;
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTStateChanged(this);
	}
}

void meWorld::NotifyHTSChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSChanged(this, sector);
	}
}

void meWorld::NotifyHTSHeightChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSHeightChanged(this, sector);
	}
}

void meWorld::NotifyHTSVisibilityChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSVisibilityChanged(this, sector);
	}
}

void meWorld::NotifyHTSTextureCountChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSTextureCountChanged(this, sector);
	}
}

void meWorld::NotifyHTSActiveTextureChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSActiveTextureChanged(this, sector);
	}
}

void meWorld::NotifyHTSTextureChanged(meHeightTerrainSector *sector, meHeightTerrainTexture *texture){
	if(!sector || !texture) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSTextureChanged(this, sector, texture);
	}
}

void meWorld::NotifyHTSTextureMaskChanged(meHeightTerrainSector *sector, meHeightTerrainTexture *texture){
	if(!sector || !texture) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSTextureMaskChanged(this, sector, texture);
	}
}

void meWorld::NotifyHTNavSpaceCountChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTNavSpaceCountChanged(this);
	}
}

void meWorld::NotifyHTActiveNavSpaceChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTActiveNavSpaceChanged(this);
	}
}

void meWorld::NotifyHTNavSpaceChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTNavSpaceChanged(this, navspace);
	}
}

void meWorld::NotifyHTNavSpaceTypeCountChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTNavSpaceTypeCountChanged(this, navspace);
	}
}

void meWorld::NotifyHTNavSpaceActiveTypeChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTNavSpaceActiveTypeChanged(this, navspace);
	}
}

void meWorld::NotifyHTNavSpaceTypeChanged(meHeightTerrainNavSpace *navspace,
meHeightTerrainNavSpaceType *type){
	if(!navspace || !type){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTNavSpaceTypeChanged(this, navspace, type);
	}
}

void meWorld::NotifyHTNavSpaceFacesChanged(meHeightTerrainNavSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTNavSpaceFacesChanged(this, navspace);
	}
}

void meWorld::NotifyHTNavSpaceSelectedPointsChanged(){
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->HTNavSpaceSelectedPointsChanged(this);
	}
}

void meWorld::NotifyHTSPropFieldCountChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSPropFieldCountChanged(this, sector);
	}
}

void meWorld::NotifyHTSActivePropFieldChanged(meHeightTerrainSector *sector){
	if(!sector) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSActivePropFieldChanged(this, sector);
	}
}

void meWorld::NotifyHTSPropFieldChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField){
	if(!sector || !propField) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSPropFieldChanged(this, sector, propField);
	}
}

void meWorld::NotifyHTSPropFieldMaskChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField){
	if(!sector || !propField) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSPropFieldMaskChanged(this, sector, propField);
	}
}

void meWorld::NotifyHTSPFTypeCountChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField){
	if(!sector || !propField) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSPFTypeCountChanged(this, sector, propField);
	}
}

void meWorld::NotifyHTSPFTypeChanged(meHeightTerrainSector *sector, meHeightTerrainPropField *propField, meHeightTerrainPFType *type){
	if(!sector || !propField || !type) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTSPFTypeChanged(this, sector, propField, type);
	}
}

void meWorld::NotifyHTVLayerCountChanged(){
	int n;
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLayerCountChanged(this);
	}
}

void meWorld::NotifyHTActiveVLayerChanged(){
	int n;
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTActiveVLayerChanged(this);
	}
}

void meWorld::NotifyHTVLayerChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLayerChanged(this, vlayer);
	}
}

void meWorld::NotifyHTVLVariationCountChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLVariationCountChanged(this, vlayer);
	}
}

void meWorld::NotifyHTVLActiveVariationChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLActiveVariationChanged(this, vlayer);
	}
}

void meWorld::NotifyHTVLVariationChanged(meHTVegetationLayer *vlayer, meHTVVariation *variation){
	if(!vlayer || !variation) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLVariationChanged(this, vlayer, variation);
	}
}

void meWorld::NotifyHTVLRuleCountChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLRuleCountChanged(this, vlayer);
	}
}

void meWorld::NotifyHTVLActiveRuleChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLActiveRuleChanged(this, vlayer);
	}
}

void meWorld::NotifyHTVLRuleChanged(meHTVegetationLayer *vlayer, meHTVRule *rule){
	if(!vlayer || !rule) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLRuleChanged(this, vlayer, rule);
	}
}

void meWorld::NotifyHTVLRuleMoved(meHTVegetationLayer *vlayer, meHTVRule *rule){
	if(!vlayer || !rule) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLRuleMoved(this, vlayer, rule);
	}
}

void meWorld::NotifyHTVLLinkCountChanged(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->HTVLLinkCountChanged(this, vlayer);
	}
}



void meWorld::NotifyObjectSelectionChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectSelectionChanged(this);
	}
}

void meWorld::NotifyObjectChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectChanged(this, object);
	}
}

void meWorld::NotifyObjectClassChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectClassChanged(this, object);
	}
}

void meWorld::NotifyObjectGeometryChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectGeometryChanged(this, object);
	}
	
	pHeightTerrain->InvalidatePropFields(object);
}

void meWorld::NotifyObjectPropertiesChanged(meObject *object){
	if(!object){
		DETHROW(deeInvalidParam);
	}
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectPropertiesChanged(this, object);
	}
}

void meWorld::NotifyObjectActivePropertyChanged(meObject *object){
	if(!object){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->ObjectActivePropertyChanged(this, object);
	}
}

void meWorld::NotifyObjectAttachBehaviorsChanged(meObject *object){
	DEASSERT_NOTNULL(object)
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->ObjectAttachBehaviorsChanged(this, object);
	}
}

void meWorld::NotifyObjectActiveAttachBehaviorChanged(meObject *object){
	DEASSERT_NOTNULL(object)
	
	int i;
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->ObjectActiveAttachBehaviorChanged(this, object);
	}
}

void meWorld::NotifyObjectAdded(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectAdded(this, object);
	}
}

void meWorld::NotifyObjectRemoved(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectRemoved(this, object);
	}
}

void meWorld::NotifyObjectTextureCountChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectTextureCountChanged(this, object);
	}
}

void meWorld::NotifyObjectActiveTextureChanged(meObject *object){
	if(!object) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectActiveTextureChanged(this, object);
	}
}

void meWorld::NotifyObjectTextureChanged(meObject *object, meObjectTexture *texture){
	if(!object || !texture) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectTextureChanged(this, object, texture);
	}
}

void meWorld::NotifyObjectTexturePropertiesChanged(meObject *object, meObjectTexture *texture){
	if(!object || !texture){
		DETHROW(deeInvalidParam);
	}
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectTexturePropertiesChanged(this, object, texture);
	}
}

void meWorld::NotifyObjectTextureActivePropertyChanged(meObject *object, meObjectTexture *texture){
	if(!object || !texture){
		DETHROW(deeInvalidParam);
	}
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectTextureActivePropertyChanged(this, object, texture);
	}
}



void meWorld::NotifyObjectShapeListChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectShapeListChanged(this);
	}
}

void meWorld::NotifyObjectShapeSelectionChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ObjectShapeSelectionChanged(this);
	}
}



void meWorld::NotifyDecalSelectionChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->DecalSelectionChanged(this);
	}
}

void meWorld::NotifyDecalCountChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->DecalCountChanged(this);
	}
}

void meWorld::NotifyDecalChanged(meDecal *decal){
	if(!decal) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->DecalChanged(this, decal);
	}
}

void meWorld::NotifyDecalGeometryChanged(meDecal *decal){
	if(!decal) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->DecalGeometryChanged(this, decal);
	}
}

void meWorld::NotifyDecalPropertiesChanged(meDecal *decal){
	if(!decal){
		DETHROW(deeInvalidParam);
	}
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->DecalPropertiesChanged(this, decal);
	}
}

void meWorld::NotifyDecalActivePropertyChanged(meDecal *decal){
	if(!decal){
		DETHROW(deeInvalidParam);
	}
	
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->DecalActivePropertyChanged(this, decal);
	}
}



void meWorld::NotifyNavSpaceSelectionChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->NavSpaceSelectionChanged(this);
	}
}

void meWorld::NotifyNavSpaceCountChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->NavSpaceCountChanged(this);
	}
}

void meWorld::NotifyNavSpaceChanged(meNavigationSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->NavSpaceChanged(this, navspace);
	}
	
	pPathFindTest->Invalidate();
}

void meWorld::NotifyNavSpaceGeometryChanged(meNavigationSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->NavSpaceGeometryChanged(this, navspace);
	}
	
	pPathFindTest->Invalidate();
}

void meWorld::NotifyNavSpaceUsedCostTypesChanged(meNavigationSpace *navspace){
	if(!navspace){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->NavSpaceUsedCostTypesChanged(this, navspace);
	}
}



void meWorld::NotifyPFSelectionChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PFSelectionChanged(this);
	}
}

void meWorld::NotifyPFCountChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PFCountChanged(this);
	}
}

void meWorld::NotifyPFChanged(mePropField *field){
	if(!field) DETHROW(deeInvalidParam);
	
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PFChanged(this, field);
	}
}

void meWorld::NotifyPFTypeCountChanged(mePropField *field){
	if(!field) DETHROW(deeInvalidParam);
	
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PFTypeCountChanged(this, field);
	}
}

void meWorld::NotifyPFActiveTypeChanged(mePropField *field){
	if(!field) DETHROW(deeInvalidParam);
	
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PFActiveTypeChanged(this, field);
	}
}

void meWorld::NotifyPFTypeChanged(mePropField *field, mePropFieldType *type){
	if(!field || !type) DETHROW(deeInvalidParam);
	
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PFTypeChanged(this, field, type);
	}
}



void meWorld::NotifyActiveCameraChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ActiveCameraChanged(this);
	}
}

void meWorld::NotifyCameraChanged(meCamera *camera){
	if(!camera) DETHROW(deeInvalidParam);
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->CameraChanged(this, camera);
	}
}



// Private Functions
//////////////////////

void meWorld::pCleanUp(){
	RemoveAllNotifiers();
	if(pNotifiers){
		delete [] pNotifiers;
	}
	
	pSelectionNavigationSpace.RemoveAll();
	pSelectionDecal.RemoveAll();
	pSelectionObjectShape.RemoveAll();
	pSelectionObject.RemoveAll();
	
	pObjectShapes.RemoveAll();
	
	RemoveAllNavSpaces();
	RemoveAllDecals();
	RemoveAllObjects();
	
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
				GetEngine()->GetAudioSystem()->SetActiveMicrophone(NULL);
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
	pDDSLimitBox.RemoveAllShapes();
	
	if(!(pLimitBoxMaxExtend - pLimitBoxMinExtend).IsZero()){
		pDDSLimitBox.AddBoxShape(
			decVector(0.001f, 0.001f, 0.001f).Largest(pLimitBoxMaxExtend - pLimitBoxMinExtend) * 0.5f,
			(pLimitBoxMaxExtend + pLimitBoxMinExtend) * 0.5f, decQuaternion());
		pDDLimitBox->SetVisible(true);
		
	}else{
		pDDLimitBox->SetVisible(false);
	}
}
