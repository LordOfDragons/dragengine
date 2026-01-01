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
	
	deAnimatorController *amController = nullptr;
	deAnimatorRuleAnimation::Ref amRuleAnim;
	deAnimatorLink *engLink = nullptr;
	
	pEngWorld = nullptr;
	pCamera = nullptr;
	
	pSky = nullptr;
	
	pEngSkin = nullptr;
	pEngComponent = nullptr;
	pEngAnimator = nullptr;
	pEngAnimatorInstance = nullptr;
	pEngAnimatorAnim = nullptr;
	pEngParticleEmitter = nullptr;
	pMoveTime = 0.0f;
	pPlayback = true;
	
	pDynamicSkin = nullptr;
	
	pDirtySkin = true;
	pDirtySkinAssignment = true;
	pRewindTextures = 0;
	pEnableSkinUpdate = true;
	
	pActiveMapped = nullptr;
	pActiveTexture = nullptr;
	
	try{
		SetFilePath("new.deskin");
		
		// create animator
		pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
		
		amController = new deAnimatorController;
		amController->SetClamp(false);
		pEngAnimator->AddController(amController);
		amController = nullptr;
		
		engLink = new deAnimatorLink;
		engLink->SetController(0);
		pEngAnimator->AddLink(engLink);
		engLink = nullptr;
		
		amRuleAnim = deAnimatorRuleAnimation::Ref::New();
		amRuleAnim->SetEnabled(true);
		
		amRuleAnim->GetTargetMoveTime().AddLink(0);
		
		pEngAnimator->AddRule(amRuleAnim);
		pEngAnimatorAnim = amRuleAnim;
		amRuleAnim = nullptr;
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator(pEngAnimator);
		
		// create world
		pEngWorld = engine->GetWorldManager()->CreateWorld();
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
		pEnvObject = igdeWObject::Ref::New(*environment);
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
		pEngSkin = engine->GetSkinManager()->CreateSkin("", skinBuilder);
		
		// create dynamic skin
		pDynamicSkin = new seDynamicSkin(this);
		
	}catch(const deException &){
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
	if(mode == pPreviewMode){
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
	pTextures.Visit([](seTexture &t){
		t.UpdateEngineSkin();
	});
	
	if(pRewindTextures == 0){
		// assign texture engine skins to the engine component if dirty
		AssignTextureSkins();
		
	}else if(pRewindTextures == 1){
		if(pEngComponent && pEngComponent->GetModel()){
			const int textureCount2 = pEngComponent->GetTextureCount();
			int i;
			for(i=0; i<textureCount2; i++){
				deComponentTexture &engComponentTexture = pEngComponent->GetTextureAt(i);
				engComponentTexture.SetSkin(nullptr);
				engComponentTexture.SetTexture(0);
				pEngComponent->NotifyTextureChanged(i);
			}
			pEngLight->SetLightSkin(nullptr);
		}
		pRewindTextures = 2;
		
	}else if(pRewindTextures == 2){
		if(pEngComponent && pEngComponent->GetModel()){
			pTextures.Visit([](seTexture &t){
				t.AssignSkinToComponentTexture();
			});
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
		const decString &name = engModel.GetTextureAt(i)->GetName();
		seTexture * const texture = GetTextures().FindOrDefault([&](const seTexture &t){
			return t.GetName() == name;
		});
		deComponentTexture &engComponentTexture = pEngComponent->GetTextureAt(i);
		
		if(texture){
			engComponentTexture.SetSkin(texture->GetEngineSkin());
			engComponentTexture.SetTexture(0);
			
		}else{
			engComponentTexture.SetSkin(nullptr);
			engComponentTexture.SetTexture(0);
		}
		
		pEngComponent->NotifyTextureChanged(i);
	}
	
	if(pTextures.IsNotEmpty()){
		pEngLight->SetLightSkin(pTextures.First()->GetEngineSkin());
	}
	
	pDirtySkinAssignment = false;
}

void seSkin::RewindTextures(){
	pRewindTextures = 1;
}

void seSkin::UpdateResources(){
	pTextures.Visit([](seTexture &t){
		t.UpdateResources();
	});
}



// Mapped
///////////

void seSkin::AddMapped(seMapped *mapped){
	pMapped.Add(mapped);
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
		if(pMapped.GetCount() > 1){
			seMapped *activeMapped = pMapped.GetAt(0);
			
			if(activeMapped == mapped){
				activeMapped = pMapped.GetAt(1);
			}
			
			SetActiveMapped(activeMapped);
			
		}else{
			SetActiveMapped(nullptr);
		}
	}
	
	mapped->SetSkin(nullptr);
	pMapped.Remove(mapped);
	NotifyMappedStructureChanged();
}

void seSkin::RemoveAllMapped(){
	SetActiveMapped(nullptr);
	
	pMapped.Visit([](seMapped &m){
		m.SetSkin(nullptr);
	});
	pMapped.RemoveAll();
	NotifyMappedStructureChanged();
}

bool seSkin::HasActiveMapped() const{
	return pActiveMapped != nullptr;
}

void seSkin::SetActiveMapped(seMapped *mapped){
	if(mapped == pActiveMapped){
		return;
	}
	
	if(pActiveMapped){
		pActiveMapped->SetActive(false);
	}
	
	pActiveMapped = mapped;
	
	if(mapped){
		mapped->SetActive(true);
	}
	
	NotifyActiveMappedChanged();
}



// Textures
/////////////

void seSkin::AddTexture(seTexture *texture){
	pTextures.Add(texture);
	texture->SetSkin(this);
	NotifyTextureStructureChanged();
	
	if(!pActiveTexture){
		SetActiveTexture(texture);
	}
}

void seSkin::RemoveTexture(seTexture *texture){
	if(!texture || texture->GetSkin() != this) DETHROW(deeInvalidParam);
	
	if(texture->GetActive()){
		if(pTextures.GetCount() > 1){
			seTexture *activeTexture = pTextures.First();
			
			if(activeTexture == texture){
				activeTexture = pTextures.GetAt(1);
			}
			
			SetActiveTexture(activeTexture);
			
		}else{
			SetActiveTexture(nullptr);
		}
	}
	
	texture->SetSkin(nullptr);
	pTextures.Remove(texture);
	NotifyTextureStructureChanged();
}

void seSkin::RemoveAllTextures(){
	SetActiveTexture(nullptr);
	
	pTextures.Visit([](seTexture &t){
		t.SetSkin(nullptr);
	});
	pTextures.RemoveAll();
	NotifyTextureStructureChanged();
}

bool seSkin::HasActiveTexture() const{
	return pActiveTexture.IsNotNull();
}

void seSkin::SetActiveTexture(seTexture *texture){
	if(texture != pActiveTexture){
		if(pActiveTexture){
			pActiveTexture->SetActive(false);
		}
		
		pActiveTexture = texture;
		
		if(texture){
			texture->SetActive(true);
		}
		
		NotifyActiveTextureChanged();
	}
}



// Listeners
//////////////

void seSkin::AddListener(seSkinListener *listener){
	DEASSERT_NOTNULL(listener)
	
	pListeners.Add(listener);
}

void seSkin::RemoveListener(seSkinListener *listener){
	pListeners.Remove(listener);
}


void seSkin::NotifyStateChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.StateChanged(this);
	});
}

void seSkin::NotifyUndoChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.UndoChanged(this);
	});
}

void seSkin::NotifySkinChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.SkinChanged(this);
	});
	
	SetChanged(true);
	//RequestRebuild();
}

void seSkin::NotifySkyChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.SkyChanged(this);
	});
}

void seSkin::NotifyEnvObjectChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.EnvObjectChanged(this);
	});
}

void seSkin::NotifyViewChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.ViewChanged(this);
	});
}

void seSkin::NotifyCameraChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.CameraChanged(this);
	});
}



void seSkin::NotifyMappedStructureChanged(){
	pUpdateTextureDynamicSkins();
	
	pListeners.Visit([&](seSkinListener &l){
		l.MappedStructureChanged(this);
	});
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyMappedChanged(seMapped *mapped){
	pListeners.Visit([&](seSkinListener &l){
		l.MappedChanged(this, mapped);
	});
	
	SetChanged(true);
	Invalidate();
	
	pTextures.Visit([](seTexture &l){
		l.InvalidateEngineSkin();
	});
}

void seSkin::NotifyMappedNameChanged(seMapped *mapped){
	pListeners.Visit([&](seSkinListener &l){
		l.MappedNameChanged(this, mapped);
	});
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyActiveMappedChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.ActiveMappedChanged(this);
	});
}



void seSkin::NotifyTextureStructureChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.TextureStructureChanged(this);
	});
	
	SetChanged(true);
	Invalidate();
	pDirtySkinAssignment = true;
}

void seSkin::NotifyTextureChanged(seTexture *texture){
	pListeners.Visit([&](seSkinListener &l){
		l.TextureChanged(this, texture);
	});
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyTextureNameChanged(seTexture *texture){
	pListeners.Visit([&](seSkinListener &l){
		l.TextureNameChanged(this, texture);
	});
	
	SetChanged(true);
	Invalidate();
	pDirtySkinAssignment = true;
}

void seSkin::NotifyActiveTextureChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.ActiveTextureChanged(this);
	});
}



void seSkin::NotifyPropertyStructureChanged(seTexture *texture){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyStructureChanged(this, texture);
	});
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyPropertyChanged(seTexture *texture, seProperty *property){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyChanged(this, texture, property);
	});
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyActivePropertyChanged(seTexture *texture){
	pListeners.Visit([&](seSkinListener &l){
		l.ActivePropertyChanged(this, texture);
	});
}

void seSkin::NotifyPropertyNodeStructureChanged(seTexture *texture, seProperty *property){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyNodeStructureChanged(this, texture, property);
	});
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyPropertyNodeChanged(seTexture *texture, seProperty *property, sePropertyNode *node){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyNodeChanged(this, texture, property, node);
	});
	
	SetChanged(true);
	Invalidate();
}

void seSkin::NotifyPropertyActiveNodeChanged(seTexture *texture, seProperty *property){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyActiveNodeChanged(this, texture, property);
	});
}

void seSkin::NotifyPropertyNodeSelectionChanged(seTexture *texture, seProperty *property){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyNodeSelectionChanged(this, texture, property);
	});
}

void seSkin::NotifyPropertyActiveNodeGroupChanged(seTexture *texture, seProperty *property){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyActiveNodeGroupChanged(this, texture, property);
	});
}

void seSkin::NotifyPropertyActiveNodeLayerChanged(seTexture *texture, seProperty *property){
	pListeners.Visit([&](seSkinListener &l){
		l.PropertyActiveNodeLayerChanged(this, texture, property);
	});
}



void seSkin::NotifyDynamicSkinRenderableStructureChanged(){
	pUpdateTextureDynamicSkins();
	
	pListeners.Visit([&](seSkinListener &l){
		l.DynamicSkinRenderableStructureChanged(this);
	});
}

void seSkin::NotifyDynamicSkinRenderableChanged(seDynamicSkinRenderable *renderable){
	pListeners.Visit([&](seSkinListener &l){
		l.DynamicSkinRenderableChanged(this, renderable);
	});
}

void seSkin::NotifyDynamicSkinRenderableNameChanged(seDynamicSkinRenderable *renderable){
	pListeners.Visit([&](seSkinListener &l){
		l.DynamicSkinRenderableNameChanged(this, renderable);
	});
}

void seSkin::NotifyDynamicSkinActiveRenderableChanged(){
	pListeners.Visit([&](seSkinListener &l){
		l.DynamicSkinActiveRenderableChanged(this);
	});
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
		pEngAnimatorInstance->SetComponent(nullptr);
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
	pEngLight = GetEngine()->GetLightManager()->CreateLight();
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
	
	pEngParticleEmitter = GetEngine()->GetParticleEmitterManager()->CreateParticleEmitter();
	
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
	deModel::Ref model;
	deRig::Ref rig;
	
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
		pEngComponent->SetDynamicSkin(nullptr);
	}
	
	pEngAnimatorInstance->SetComponent(nullptr); // otherwise the animator is not reset
	
	if(model && pEngSkin){
		if(pEngComponent){
			pEngComponent->SetModelAndSkin(model, pEngSkin);
			
		}else{
			pEngComponent = GetEngine()->GetComponentManager()->CreateComponent(model, pEngSkin);
			pEngWorld->AddComponent(pEngComponent);
		}
		
	}else if(pEngComponent){
		pEngWorld->RemoveComponent(pEngComponent);
		pEngComponent = nullptr;
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
	
	// update dynamic skin
	if(pEngComponent){
		pEngComponent->SetDynamicSkin(pDynamicSkin->GetEngineDynamicSkin());
	}
	
	// update animator
	if(pEngComponent){
		pEngAnimator->SetRig(pEngComponent->GetRig());
		
	}else{
		pEngAnimator->SetRig(nullptr);
	}
	
	pEngAnimatorInstance->SetComponent(pEngComponent);
	
	pUpdateAnimator();
	
	// request update of skin assignments
	pDirtySkinAssignment = true;
}

void seSkin::pUpdateAnimator(){
	deAnimation::Ref animation;
	
	try{
		if(strlen(pAnimationPath) > 0){
			animation = GetEngine()->GetAnimationManager()->LoadAnimation(pAnimationPath, "/");
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	pEngAnimator->SetAnimation(animation);
	pUpdateAnimatorMove();
}

void seSkin::pUpdateAnimatorMove(){
	deAnimationMove *move = nullptr;
	deAnimation::Ref animation;
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
			pEngComponent->GetTextureAt(i).SetDynamicSkin(nullptr);
			pEngComponent->NotifyTextureChanged(i);
		}
	}
	
	// then assign them again
	pTextures.Visit([](seTexture &t){
		t.AssignSkinToComponentTexture();
	});
}
