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

#include "seSkin.h"
#include "seSkinBuilder.h"
#include "seSkinListener.h"
#include "mapped/seMapped.h"
#include "texture/seTexture.h"
#include "property/seProperty.h"
#include "dynamicskin/seDynamicSkin.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Skin Editor"



// Class seSkin
/////////////////

// Constructor, destructor
////////////////////////////

seSkin::seSkin(igdeEnvironment *environment) :
igdeEditableEntity(environment),
pPreviewMode(epmModel)
{
	deEngine * const engine = GetEngine();
	
	deAnimatorController *amController = NULL;
	deAnimatorRuleAnimation *amRuleAnim = NULL;
	deAnimatorLink *engLink = NULL;
	
	pEngWorld = NULL;
	pCamera = NULL;
	
	pSky = NULL;
	
	pEngSkin = NULL;
	pEngComponent = NULL;
	pEngAnimator = NULL;
	pEngAnimatorInstance = NULL;
	pEngAnimatorAnim = NULL;
	pEngParticleEmitter = NULL;
	pMoveTime = 0.0f;
	pPlayback = true;
	
	pDynamicSkin = NULL;
	
	pDirtySkin = true;
	pDirtySkinAssignment = true;
	pRewindTextures = 0;
	pEnableSkinUpdate = true;
	
	pActiveMapped = nullptr;
	pActiveTexture = nullptr;
	
	try{
		SetFilePath("new.deskin");
		
		// create animator
		pEngAnimator.TakeOver(engine->GetAnimatorManager()->CreateAnimator();
		
		amController = new deAnimatorController;
		amController->SetClamp(false);
		pEngAnimator->AddController(amController);
		amController = NULL;
		
		engLink = new deAnimatorLink;
		engLink->SetController(0);
		pEngAnimator->AddLink(engLink);
		engLink = NULL;
		
		amRuleAnim = new deAnimatorRuleAnimation;
		amRuleAnim->SetEnabled(true);
		
		amRuleAnim->GetTargetMoveTime().AddLink(0);
		
		pEngAnimator->AddRule(amRuleAnim);
		pEngAnimatorAnim = amRuleAnim;
		amRuleAnim->FreeReference();
		amRuleAnim = NULL;
		
		pEngAnimatorInstance.TakeOver(engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator(pEngAnimator);
		
		// create world
		pEngWorld.TakeOver(engine->GetWorldManager()->CreateWorld();
		pEngWorld->SetGravity(decVector(0.0f, -9.81f, 0.0f));
		pEngWorld->SetDisableLights(false);
		pEngWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f));
		
		// create camera
		pCamera = new igdeCamera(engine);
		pCamera->SetEngineWorld(pEngWorld);
		pCamera->Reset();
		pCamera->SetPosition(decDVector(0.0, 0.0, 0.0));
		pCamera->SetOrientation(decVector(0.0f, 180.0f, 0.0f));
		pCamera->SetDistance(5.0f);
		pCamera->SetHighestIntensity(20.0f);
		pCamera->SetLowestIntensity(20.0f);
		pCamera->SetAdaptionTime(4.0f);
		
		// create sky
		pSky = new igdeWSky(*environment);
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pEngWorld);
		
		// create the environment wrapper object
		pEnvObject.TakeOverWith(*environment);
		pEnvObject->SetWorld(pEngWorld);
		pEnvObject->SetPosition(decDVector(0.0, -2.0, 0.0));
		
		decLayerMask layerMask;
		layerMask.SetBit(0);
		pEnvObject->SetCollisionFilter(decCollisionFilter(layerMask)); // just so something collides
		pEnvObject->SetCollisionFilterFallback(decCollisionFilter(layerMask)); // just so something collides
		
		pEnvObject->SetGDClassName("IGDETestTerrain");
		
		pCreateLight();
		pCreateParticleEmitter();
		
		// create empty skin
		deSkinBuilder skinBuilder;
		pEngSkin.TakeOver(engine->GetSkinManager()->CreateSkin("", skinBuilder);
		
		// create dynamic skin
		pDynamicSkin = new seDynamicSkin(this);
		
	}catch(const deException &){
		if(amRuleAnim){
			amRuleAnim->FreeReference();
		}
		if(engLink){
			delete engLink;
		}
		if(amController){
			delete amController;
		}
		pCleanUp();
		throw;
	}
}

seSkin::~seSkin(){
	pCleanUp();
}



// Management
///////////////

void seSkin::SetPreviewMode(ePreviewMode mode){
	if(pPreviewMode == mode){
		return;
	}
	
	pPreviewMode = mode;
	
	// make appropriate engine resource visible
	if(pEngComponent){
		pEngComponent->SetVisible(mode == epmModel);
	}
	pEngLight->SetActivated(mode == epmLight);
	
	// reset environment model, sky and camera
	pCamera->Reset();
	pCamera->SetOrientation(decVector(0.0f, 180.0f, 0.0f));
	pCamera->SetDistance(5.0f);
	pCamera->SetHighestIntensity(20.0f);
	pCamera->SetLowestIntensity(20.0f);
	pCamera->SetAdaptionTime(4.0f);
	
	switch(mode){
	case epmModel:
		pEnvObject->SetGDClassName("IGDETestTerrain");
		pEnvObject->SetPosition(decDVector(0.0, -2.0, 0.0));
		pSky->SetGDDefaultSky();
		pCamera->SetPosition(decDVector());
		break;
		
	case epmLight:
		pEnvObject->SetGDClassName("IGDELightSkinBox");
		pEnvObject->SetPosition(decDVector(0.0, -1.0, 0.0));
		pSky->SetPath("/igde/skies/black.desky");
		pCamera->SetPosition(decDVector(0.0, -1.0, 0.0));
		break;
	}
	
	// notify ui
	NotifyViewChanged();
	NotifyEnvObjectChanged();
	NotifySkyChanged();
	NotifyCameraChanged();
}

void seSkin::SetModelPath(const char *path){
	if(pModelPath.Equals(path)){
		return;
	}
	
	pModelPath = path;
	
	pUpdateComponent();
	pUpdateTextureDynamicSkins();
	NotifyViewChanged();
}

void seSkin::SetRigPath(const char *path){
	if(pRigPath.Equals(path)){
		return;
	}
	
	pRigPath = path;
	
	pUpdateComponent();
	pUpdateTextureDynamicSkins();
	NotifyViewChanged();
}

void seSkin::SetAnimationPath(const char *path){
	if(pAnimationPath.Equals(path)){
		return;
	}
	
	pAnimationPath = path;
	
	pUpdateAnimator();
	NotifyViewChanged();
}

void seSkin::SetMoveName(const char *moveName){
	if(pMoveName.Equals(moveName)){
		return;
	}
	
	pMoveName = moveName;
	
	pUpdateAnimatorMove();
	NotifyViewChanged();
}

void seSkin::SetMoveTime(float moveTime){
	if(fabs(moveTime - pMoveTime) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMoveTime = moveTime;
	
	pEngAnimatorInstance->GetControllerAt(0).SetCurrentValue(pMoveTime);
	pEngAnimatorInstance->NotifyControllerChangedAt(0);
	
	NotifyViewChanged();
}

void seSkin::SetPlayback(bool playback){
	if(playback == pPlayback){
		return;
	}
	
	pPlayback = playback;
	
	pEngAnimatorInstance->GetControllerAt(0).SetFrozen(!playback);
	
	NotifyViewChanged();
}

void seSkin::SetEnableSkinUpdate(bool enableSkinUpdate){
	pEnableSkinUpdate = enableSkinUpdate;
}



void seSkin::Dispose(){
	RemoveAllTextures();
	RemoveAllMapped();
	
	GetUndoSystem()->RemoveAll();
}

void seSkin::Reset(){
}

void seSkin::Update(float elapsed){
	//Rebuild();
	
	// update texture engine skins
	const int textureCount = pTextureList.GetCount();
	int i;
	
	for(i=0; i<textureCount; i++){
		pTextureList.GetAt(i)->UpdateEngineSkin();
	}
	
	if(pRewindTextures == 0){
		// assign texture engine skins to the engine component if dirty
		AssignTextureSkins();
		
	}else if(pRewindTextures == 1){
		if(pEngComponent && pEngComponent->GetModel()){
			const int textureCount2 = pEngComponent->GetTextureCount();
			for(i=0; i<textureCount2; i++){
				deComponentTexture &engComponentTexture = pEngComponent->GetTextureAt(i);
				engComponentTexture.SetSkin(NULL);
				engComponentTexture.SetTexture(0);
				pEngComponent->NotifyTextureChanged(i);
			}
			pEngLight->SetLightSkin(NULL);
		}
		pRewindTextures = 2;
		
	}else if(pRewindTextures == 2){
		if(pEngComponent && pEngComponent->GetModel()){
			for(i=0; i<textureCount; i++){
				pTextureList.GetAt(i)->AssignSkinToComponentTexture();
			}
		}
		pRewindTextures = 0;
	}
	
	pDynamicSkin->Update(elapsed);
	
	pEnvObject->Update(elapsed);
	
	if(!pEngAnimatorInstance->GetControllerAt(0).GetFrozen()){
		pEngAnimatorInstance->GetControllerAt(0).IncrementCurrentValue(elapsed);
		pEngAnimatorInstance->NotifyControllerChangedAt(0);
	}
	
	pEngAnimatorInstance->Apply();
	
	pEngWorld->ProcessPhysics(elapsed);
	pEngWorld->Update(elapsed);
}

void seSkin::Invalidate(){
	pDirtySkin = true;
}

void seSkin::AssignTextureSkins(){
	if(!pDirtySkinAssignment){
		return;
	}
	if(!pEngComponent){
		return;
	}
	if(!pEngComponent->GetModel()){
		return;
	}
	
	const deModel &engModel = *pEngComponent->GetModel();
	const int textureCount = engModel.GetTextureCount();
	int i;
	
	for(i=0; i<textureCount; i++){
		seTexture * const texture = GetTextureList().GetNamed(engModel.GetTextureAt(i)->GetName());
		deComponentTexture &engComponentTexture = pEngComponent->GetTextureAt(i);
		
		if(texture){
			engComponentTexture.SetSkin(texture->GetEngineSkin());
			engComponentTexture.SetTexture(0);
			
		}else{
			engComponentTexture.SetSkin(NULL);
			engComponentTexture.SetTexture(0);
		}
		
		pEngComponent->NotifyTextureChanged(i);
	}
	
	if(pTextureList.GetCount() > 0){
		pEngLight->SetLightSkin(pTextureList.GetAt(0)->GetEngineSkin());
	}
	
	pDirtySkinAssignment = false;
}

void seSkin::RewindTextures(){
	pRewindTextures = 1;
}

void seSkin::UpdateResources(){
	const int count = pTextureList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pTextureList.GetAt(i)->UpdateResources();
	}
}



// Mapped
///////////

void seSkin::AddMapped(seMapped *mapped){
	pMappedList.Add(mapped);
	mapped->SetSkin(this);
	NotifyMappedStructureChanged();
	
	if(!pActiveMapped){
		SetActiveMapped(mapped);
	}
}

void seSkin::RemoveMapped(seMapped *mapped){
	DEASSERT_NOTNULL(mapped)
	DEASSERT_TRUE(mapped->GetSkin() == this)
	
	if(mapped->GetActive()){
		if(pMappedList.GetCount() > 1){
			seMapped *activeMapped = pMappedList.GetAt(0);
			
			if(activeMapped == mapped){
				activeMapped = pMappedList.GetAt(1);
			}
			
			SetActiveMapped(activeMapped);
			
		}else{
			SetActiveMapped(nullptr);
		}
	}
	
	mapped->SetSkin(nullptr);
	pMappedList.Remove(mapped);
	NotifyMappedStructureChanged();
}

void seSkin::RemoveAllMapped(){
	const int count = pMappedList.GetCount();
	int i;
	
	SetActiveMapped(nullptr);
	
	for(i=0; i<count; i++){
		pMappedList.GetAt(i)->SetSkin(nullptr);
	}
	pMappedList.RemoveAll();
	NotifyMappedStructureChanged();
}

bool seSkin::HasActiveMapped() const{
	return pActiveMapped != nullptr;
}

void seSkin::SetActiveMapped(seMapped *mapped){
	if(pActiveMapped == mapped){
		return;
	}
	
	if(pActiveMapped){
		pActiveMapped->SetActive(false);
	}
	
	pActiveMapped = mapped;
	
	if(mapped){
		mapped->AddReference();
		mapped->SetActive(true);
	}
	
	NotifyActiveMappedChanged();
}



// Textures
/////////////

void seSkin::AddTexture(seTexture *texture){
	pTextureList.Add(texture);
	texture->SetSkin(this);
	NotifyTextureStructureChanged();
	
	if(!pActiveTexture){
		SetActiveTexture(texture);
	}
}

void seSkin::RemoveTexture(seTexture *texture){
	if(!texture || texture->GetSkin() != this) DETHROW(deeInvalidParam);
	
	if(texture->GetActive()){
		if(pTextureList.GetCount() > 1){
			seTexture *activeTexture = pTextureList.GetAt(0);
			
			if(activeTexture == texture){
				activeTexture = pTextureList.GetAt(1);
			}
			
			SetActiveTexture(activeTexture);
			
		}else{
			SetActiveTexture(NULL);
		}
	}
	
	texture->SetSkin(NULL);
	pTextureList.Remove(texture);
	NotifyTextureStructureChanged();
}

void seSkin::RemoveAllTextures(){
	const int count = pTextureList.GetCount();
	int t;
	
	SetActiveTexture(NULL);
	
	for(t=0; t<count; t++){
		pTextureList.GetAt(t)->SetSkin(NULL);
	}
	pTextureList.RemoveAll();
	NotifyTextureStructureChanged();
}

bool seSkin::HasActiveTexture() const{
	return pActiveTexture != nullptr;
}

void seSkin::SetActiveTexture(seTexture *texture){
	if(pActiveTexture != texture){
		if(pActiveTexture){
			pActiveTexture->SetActive(false);
		}
		
		pActiveTexture = texture;
		
		if(texture){
			texture->AddReference();
			texture->SetActive(true);
		}
		
		NotifyActiveTextureChanged();
	}
}



// Listeners
//////////////

void seSkin::AddListener(seSkinListener *listener){
	if(!listener) DETHROW(deeInvalidParam);
	
	pListeners.Add(listener);
}

void seSkin::RemoveListener(seSkinListener *listener){
	pListeners.RemoveIfPresent(listener);
}



void seSkin::NotifyStateChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->StateChanged(this);
	}
}

void seSkin::NotifyUndoChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->UndoChanged(this);
	}
}

void seSkin::NotifySkinChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->SkinChanged(this);
	}
	
	SetChanged(true);
	//RequestRebuild();
}

void seSkin::NotifySkyChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->SkyChanged(this);
	}
}

void seSkin::NotifyEnvObjectChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->EnvObjectChanged(this);
	}
}

void seSkin::NotifyViewChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->ViewChanged(this);
	}
}

void seSkin::NotifyCameraChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->CameraChanged(this);
	}
}



void seSkin::NotifyMappedStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->MappedStructureChanged(this);
	}
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyMappedChanged(seMapped *mapped){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->MappedChanged(this, mapped);
	}
	
	SetChanged(true);
	Invalidate();
	
	const int textureCount = pTextureList.GetCount();
	for(i=0; i<textureCount; i++){
		pTextureList.GetAt(i)->InvalidateEngineSkin();
	}
}

void seSkin::NotifyMappedNameChanged(seMapped *mapped){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->MappedNameChanged(this, mapped);
	}
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyActiveMappedChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->ActiveMappedChanged(this);
	}
}



void seSkin::NotifyTextureStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->TextureStructureChanged(this);
	}
	
	SetChanged(true);
	Invalidate();
	pDirtySkinAssignment = true;
}

void seSkin::NotifyTextureChanged(seTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->TextureChanged(this, texture);
	}
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyTextureNameChanged(seTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkinListener*)pListeners.GetAt(i))->TextureNameChanged(this, texture);
	}
	
	SetChanged(true);
	Invalidate();
	pDirtySkinAssignment = true;
}

void seSkin::NotifyActiveTextureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->ActiveTextureChanged(this);
	}
}



void seSkin::NotifyPropertyStructureChanged(seTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->PropertyStructureChanged(this, texture);
	}
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyPropertyChanged(seTexture *texture, seProperty *property){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->PropertyChanged(this, texture, property);
	}
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyActivePropertyChanged(seTexture *texture){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for(l=0; l<listenerCount; l++){
		((seSkinListener*)pListeners.GetAt(l))->ActivePropertyChanged(this, texture);
	}
}

void seSkin::NotifyPropertyNodeStructureChanged(seTexture *texture, seProperty *property){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->PropertyNodeStructureChanged(this, texture, property);
	}
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyPropertyNodeChanged(seTexture *texture, seProperty *property, sePropertyNode *node){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->PropertyNodeChanged(this, texture, property, node);
	}
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyPropertyActiveNodeChanged(seTexture *texture, seProperty *property){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->PropertyActiveNodeChanged(this, texture, property);
	}
}

void seSkin::NotifyPropertyNodeSelectionChanged(seTexture *texture, seProperty *property){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->PropertyNodeSelectionChanged(this, texture, property);
	}
}

void seSkin::NotifyPropertyActiveNodeGroupChanged(seTexture *texture, seProperty *property){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->PropertyActiveNodeGroupChanged(this, texture, property);
	}
}

void seSkin::NotifyPropertyActiveNodeLayerChanged(seTexture *texture, seProperty *property){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSkinListener*)pListeners.GetAt(i))->PropertyActiveNodeLayerChanged(this, texture, property);
	}
}



void seSkin::NotifyDynamicSkinRenderableStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	pUpdateTextureDynamicSkins();
	
	for(i=0; i<listenerCount; i++){
		((seSkinListener*)pListeners.GetAt(i))->DynamicSkinRenderableStructureChanged(this);
	}
}

void seSkin::NotifyDynamicSkinRenderableChanged(seDynamicSkinRenderable *renderable){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkinListener*)pListeners.GetAt(i))->DynamicSkinRenderableChanged(this, renderable);
	}
}

void seSkin::NotifyDynamicSkinRenderableNameChanged(seDynamicSkinRenderable *renderable){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkinListener*)pListeners.GetAt(i))->DynamicSkinRenderableNameChanged(this, renderable);
	}
}

void seSkin::NotifyDynamicSkinActiveRenderableChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkinListener*)pListeners.GetAt(i))->DynamicSkinActiveRenderableChanged(this);
	}
}



// Private Functions
//////////////////////

void seSkin::pCleanUp(){
	if(pSky){
		delete pSky;
	}
	pEnvObject = nullptr;
	
	pListeners.RemoveAll();
	
	if(pCamera) delete pCamera;
	
	SetActiveTexture(nullptr);
	RemoveAllTextures();
	
	SetActiveMapped(nullptr);
	RemoveAllMapped();
	
	if(pEngAnimatorInstance){
		pEngAnimatorInstance->SetComponent(NULL);
	}
	pEngAnimatorAnim = nullptr;
	
	if(pDynamicSkin){
		delete pDynamicSkin;
	}
	
	if(pEngWorld){
		if(pEngLight && pEngLight->GetParentWorld()){
			pEngWorld->RemoveLight(pEngLight);
		}
		if(pEngComponent && pEngComponent->GetParentWorld()){
			pEngWorld->RemoveComponent(pEngComponent);
		}
	}
}



void seSkin::pCreateLight(){
	pEngLight.TakeOver(GetEngine()->GetLightManager()->CreateLight());
	pEngLight->SetType(deLight::eltPoint);
	pEngLight->SetActivated(false);
	pEngLight->SetAmbientRatio(0.0f);
	pEngLight->SetHalfIntensityDistance(0.25f);
	pEngLight->SetIntensity(20.0f);
	
	decLayerMask layerMask;
	layerMask.SetBit(0);
	pEngLight->SetLayerMask(layerMask);
	pEngLight->SetLayerMaskShadow(layerMask);
	
	pEngLight->SetRange(25.0f);
	pEngWorld->AddLight(pEngLight);
}

void seSkin::pCreateParticleEmitter(){
#if 0
	const float deviation = DEG2RAD * 20.0f;
	
	pEngParticleEmitter.TakeOver(GetEngine()->GetParticleEmitterManager()->CreateParticleEmitter();
	
	pEngParticleEmitter->SetCastTimeToLive(1.5f, 2.5f);
	pEngParticleEmitter->SetCastInterval(0.04f, 0.06f);
	pEngParticleEmitter->SetCastDirection(decVector(-deviation, -deviation, 0.0f), decVector(deviation, deviation, 0.0f));
	/*
	pParameters[deParticleEmitter::epSize].SetCastValues(1.0f, 1.0f);
	pParameters[deParticleEmitter::epRed].SetCastValues(1.0f, 1.0f);
	pParameters[deParticleEmitter::epGreen].SetCastValues(1.0f, 1.0f);
	pParameters[deParticleEmitter::epBlue].SetCastValues(1.0f, 1.0f);
	pParameters[deParticleEmitter::epTransparency].SetCastValues(1.0f, 1.0f);
	pParameters[deParticleEmitter::epEmissivity].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epMass].SetCastValues(0.1f, 0.1f);
	pParameters[deParticleEmitter::epLinearVelocity].SetCastValues(1.0f, 1.0f);
	pParameters[deParticleEmitter::epAngularVelocity].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epRotation].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epBrownMotion].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epDamping].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epDrag].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epGravityX].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epGravityY].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epGravityZ].SetCastValues(0.0f, 0.0f);
	pParameters[deParticleEmitter::epLocalGravity].SetCastValues(0.0f, 0.0f);
	*/
	
	pEngWorld->AddParticleEmitter(pEngParticleEmitter);
#endif
}



void seSkin::pUpdateComponent(){
	deModel *model = NULL;
	deRig *rig = NULL;
	
	// load model and rig
	try{
		if(!pModelPath.IsEmpty()){
			model = GetEngine()->GetModelManager()->LoadModel(pModelPath.GetString(), "/");
		}
		if(!pRigPath.IsEmpty()){
			rig = GetEngine()->GetRigManager()->LoadRig(pRigPath.GetString(), "/");
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	// update component
	if(pEngComponent){
		pEngComponent->SetDynamicSkin(NULL);
	}
	
	try{
		pEngAnimatorInstance->SetComponent(NULL); // otherwise the animator is not reset
		
		if(model && pEngSkin){
			if(pEngComponent){
				pEngComponent->SetModelAndSkin(model, pEngSkin);
				
			}else{
				pEngComponent.TakeOver(GetEngine()->GetComponentManager()->CreateComponent(model, pEngSkin);
				pEngWorld->AddComponent(pEngComponent);
			}
			
		}else if(pEngComponent){
			pEngWorld->RemoveComponent(pEngComponent);
			pEngComponent = NULL;
		}
		
		if(pEngComponent && rig){
			pEngComponent->SetRig(rig);
		}
		
		if(pEngComponent){
			pEngComponent->SetVisible(true);
			pEngComponent->SetPosition(decDVector(0.0f, 0.0f, 0.0f));
			pEngComponent->SetOrientation(decQuaternion());
			
			decLayerMask layerMask;
			layerMask.SetBit(0);
			pEngComponent->SetLayerMask(layerMask);
		}
		
		if(model) model->FreeReference();
		if(rig) rig->FreeReference();
		
	}catch(const deException &){
		if(model) model->FreeReference();
		if(rig) rig->FreeReference();
		throw;
	}
	
	// update dynamic skin
	if(pEngComponent){
		pEngComponent->SetDynamicSkin(pDynamicSkin->GetEngineDynamicSkin());
	}
	
	// update animator
	if(pEngComponent){
		pEngAnimator->SetRig(pEngComponent->GetRig());
		
	}else{
		pEngAnimator->SetRig(NULL);
	}
	
	pEngAnimatorInstance->SetComponent(pEngComponent);
	
	pUpdateAnimator();
	
	// request update of skin assignments
	pDirtySkinAssignment = true;
}

void seSkin::pUpdateAnimator(){
	deAnimation *animation = NULL;
	
	try{
		if(strlen(pAnimationPath) > 0){
			animation = GetEngine()->GetAnimationManager()->LoadAnimation(pAnimationPath, "/");
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	try{
		pEngAnimator->SetAnimation(animation);
		
	}catch(const deException &){
		if(animation){
			animation->FreeReference();
		}
		throw;
	}
	
	if(animation){
		animation->FreeReference();
	}
	
	pUpdateAnimatorMove();
}

void seSkin::pUpdateAnimatorMove(){
	deAnimationMove *move = NULL;
	deAnimation *animation;
	int index;
	
	pEngAnimatorAnim->SetMoveName(pMoveName);
	pEngAnimator->NotifyRulesChanged();
	
	animation = pEngAnimator->GetAnimation();
	if(animation){
		index = animation->FindMove(pMoveName);
		if(index != -1){
			move = animation->GetMove(index);
		}
	}
	
	if(move){
		pEngAnimatorInstance->GetControllerAt(0).SetValueRange(0.0f, move->GetPlaytime());
		pEngAnimatorInstance->NotifyControllerChangedAt(0);
	}
}

void seSkin::pUpdateTextureDynamicSkins(){
	int i;
	
	// clear first all dynamic skins
	if(pEngComponent){
		const int engineTextureCount = pEngComponent->GetTextureCount();
		
		for(i=0; i<engineTextureCount; i++){
			pEngComponent->GetTextureAt(i).SetDynamicSkin(NULL);
			pEngComponent->NotifyTextureChanged(i);
		}
	}
	
	// then assign them again
	const int textureCount = pTextureList.GetCount();
	
	for(i=0; i<textureCount; i++){
		pTextureList.GetAt(i)->AssignSkinToComponentTexture();
	}
}
