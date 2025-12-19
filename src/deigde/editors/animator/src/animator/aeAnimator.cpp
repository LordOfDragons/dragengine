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

#include "aeCamera.h"
#include "aeAnimator.h"
#include "aeAnimatorNotifier.h"
#include "aeSubAnimator.h"
#include "controller/aeController.h"
#include "link/aeLink.h"
#include "locomotion/aeAnimatorLocomotion.h"
#include "wakeboard/aeWakeboard.h"
#include "rule/aeRule.h"
#include "../gui/aeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeAnimator
/////////////////////

// Constructor, destructor
////////////////////////////

aeAnimator::aeAnimator(aeWindowMain &windowMain) :
igdeEditableEntity(&windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	deEngine * engine = GetEngine();
	
	pEngWorld = nullptr;
	
	pEngLight = nullptr;
	pEngComponent = nullptr;
	pEngAnimator = nullptr;
	pEngAnimatorInstance = nullptr;
	pEngCollider = nullptr;
	
	pLocomotion = nullptr;
	pWakeboard = nullptr;
	pSubAnimator = nullptr;
	pTestingSubAnimator = nullptr;
	pResetState = false;
	
	pCamera = nullptr;
	
	pActiveController = nullptr;
	pActiveLink = nullptr;
	pActiveRule = nullptr;
	
	pPaused = false;
	pPlaySpeed = 1.0f;
	pTimeStep = 0.05f;
	
	pDDBones = nullptr;
	pDDSBones = nullptr;
	pDDSBoneCount = 0;
	pDDSBoneSize = 1.0f;
	
	pSky = nullptr;
	
	try{
		SetFilePath("new.deanimator");
		
		pCreateWorld(); // creates animator
		pCreateCamera();
		pCreateCollider();
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator(pEngAnimator);
		
		pLocomotion = new aeAnimatorLocomotion(this);
		pWakeboard = new aeWakeboard(this);
		
		// create sky
		pSky = new igdeWSky(windowMain.GetEnvironment());
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pEngWorld);
		
		// create the environment wrapper object
		pEnvObject = igdeWObject::Ref::New(windowMain.GetEnvironment());
		pEnvObject->SetWorld(pEngWorld);
		pEnvObject->SetPosition(decDVector(0.0, 0.0, 0.0));
		
		decLayerMask layerMask;
		layerMask.SetBit(eclTerrain);
		layerMask.SetBit(eclElements);
		layerMask.SetBit(eclAI);
		layerMask.SetBit(eclGround);
		pEnvObject->SetCollisionFilter(decCollisionFilter(layerMask));
		pEnvObject->SetCollisionFilterFallback(decCollisionFilter(layerMask));
		
		pEnvObject->SetGDClassName("IGDETestTerrain");
		
		// create debug drawers
		pDDBones = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDDBones->SetXRay(true);
		pDDBones->SetVisible(false);
		pEngWorld->AddDebugDrawer(pDDBones);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	SetSaved(false);
	SetChanged(false);
}

aeAnimator::~aeAnimator(){
	pCleanUp();
}



// Management
///////////////

void aeAnimator::Dispose(){
	RemoveAllAttachments();
	
	GetUndoSystem()->RemoveAll();
	RemoveAllRules();
	RemoveAllLinks();
	RemoveAllControllers();
}

void aeAnimator::Reset(){
	GetUndoSystem()->RemoveAll();
}

void aeAnimator::SetDisplayModelPath(const char *path){
	if(pDisplayModelPath == path){
		return;
	}
	
	pDisplayModelPath = path;
	
	pUpdateComponent();
	NotifyModelChanged();
}

void aeAnimator::SetDisplaySkinPath(const char *path){
	if(pDisplaySkinPath == path){
		return;
	}
	
	pDisplaySkinPath = path;
	
	pUpdateComponent();
	NotifyModelChanged();
}

void aeAnimator::SetDisplayRigPath(const char *path){
	if(pDisplayRigPath == path){
		return;
	}
	
	pDisplayRigPath = path;
	
	pUpdateComponent();
	NotifyModelChanged();
}

void aeAnimator::SetRigPath(const char *path){
	if(pRigPath == path){
		return;
	}
	
	pRigPath = path;
	
	pUpdateComponent();
	NotifyRigChanged();
}

void aeAnimator::SetAnimationPath(const char *path){
	if(pAnimationPath == path){
		return;
	}
	
	pAnimationPath = path;
	
	pUpdateAnimator();
	pCamera->SetBone("");
	NotifyAnimationChanged();
}

bool aeAnimator::GetShowBones() const{
	return pDDBones->GetVisible();
}

void aeAnimator::SetShowBones(bool showBones){
	if(showBones != pDDBones->GetVisible()){
		pDDBones->SetVisible(showBones);
		NotifyViewChanged();
	}
}

void aeAnimator::SetDDBoneSize(float size){
	size = decMath::max(size, 0.01f);
	if(fabsf(size - pDDSBoneSize) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pDDSBoneSize = size;
	pUpdateDDSBones();
	NotifyViewChanged();
}



// Engine Specific
////////////////////

void aeAnimator::UpdateWorld(float elapsed){
	const float controllerElapsed = elapsed * pPlaySpeed;
	const float realTimeElapsed = elapsed * pPlaySpeed;
	int i;
	
	// update the locomotion testing
	pLocomotion->Update(realTimeElapsed);
	
	// update the controllers which are linked to the elapsed time
	if(!pPaused){
		const int count = pControllers.GetCount();
		for(i=0; i<count; i++){
			pControllers.GetAt(i)->UpdateValue(controllerElapsed);
		}
	}
	
	// reset the animation states if required
	if(pEngComponent && pResetState){
		const int boneCount = pEngComponent->GetBoneCount();
		for(i=0; i<boneCount; i++){
			deComponentBone &bone = pEngComponent->GetBoneAt(i);
			bone.SetPosition(decVector());
			bone.SetRotation(decQuaternion());
			bone.SetScale(decVector(1.0f, 1.0f, 1.0f));
		}
		
		const int vertexPositionSetCount = pEngComponent->GetVertexPositionSetCount();
		for(i=0; i<vertexPositionSetCount; i++){
			pEngComponent->SetVertexPositionSetWeightAt(i, 0.0f);
		}
		
		pEngComponent->InvalidateBones();
	}
	
	// apply the testing sub animator
	if(pTestingSubAnimator->GetEngineAnimator()){
		pTestingSubAnimator->CopyControllers(*pEngAnimatorInstance);
		pTestingSubAnimator->Apply();
	}
	
	// update the animation
	pEngAnimatorInstance->Apply();
	
	// do physics
	pEngWorld->ProcessPhysics(realTimeElapsed);
	
	// post physics updates the locomotion testing
	pLocomotion->PostPhysics();
	
	// update environment object
	pEnvObject->Update(realTimeElapsed);
	
	// adjust the position
	if(pLocomotion->GetEnabled()){
		pEngCollider->SetPosition(pLocomotion->GetPosition());
		pEngCollider->SetOrientation(pLocomotion->GetOrientationQuaternion());
	}
	
	// feet on ground
	pLocomotion->FeetOnGround();
	
	// post updates
	pLocomotion->PostUpdate();
	
	// update attachments
	if(!pPaused){
		pAttachments.Visit([&](aeAttachment *attachment){
			attachment->Update(realTimeElapsed);
		});
	}
	
	// update debug drawers
	pUpdateDDSBones();
	
	// update the scene
	pEngWorld->Update(realTimeElapsed);
	
	// update the camera
	pCamera->Update();
}



// Editing
////////////

void aeAnimator::SetPaused(bool paused){
	if(paused != pPaused){
		pPaused = paused;
		
		NotifyPlaybackChanged();
	}
}

void aeAnimator::SetPlaySpeed(float playSpeed){
	if(fabsf(playSpeed - pPlaySpeed) > 1e-5f){
		pPlaySpeed = playSpeed;
		
		NotifyPlaybackChanged();
	}
}

void aeAnimator::SetTimeStep(float timeStep){
	if(fabsf(timeStep - pTimeStep) > 1e-5f){
		pTimeStep = timeStep;
		
		NotifyPlaybackChanged();
	}
}



void aeAnimator::SetResetState(bool resetState){
	if(resetState != pResetState){
		pResetState = resetState;
		RebuildRules();
		NotifyViewChanged();
	}
}



void aeAnimator::AttachmentsForceUpdate(){
	pEngCollider->AttachmentsForceUpdate();
}

void aeAnimator::ResetSimulation(){
	// reset the locomotion simulation. this resets also the position and orientation
	// of the collider but not the velocities. these are though anyways not used
	pLocomotion->Reset();
	NotifyLocomotionChanged();
	
	// reset the controllers and apply the animator to get a clean state
	ResetControllers();
	pEngAnimatorInstance->Apply();
	
	// reset collider position which also updates attachments
	pEngCollider->SetPosition(decDVector());
	pEngCollider->SetOrientation(decQuaternion());
	
	// force update of all attachments to get a clean state there too
	pEngCollider->AttachmentsForceUpdate();
	
	// reset the physics states of all attachments. this has to be done after the
	// attachments have been updated by the physics module as this can changes states
	AttachmentsResetPhysics();
}



void aeAnimator::SetPathAttachmentConfig(const char *path){
	pPathAttConfig = path;
}



// Controllers
////////////////

void aeAnimator::AddController(aeController *controller){
	pControllers.Add(controller);
	controller->SetAnimator(this);
	NotifyControllerStructureChanged();
}

void aeAnimator::InsertControllerAt(aeController *controller, int index){
	pControllers.Insert(controller, index);
	controller->SetAnimator(this);
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::MoveControllerTo(aeController *controller, int index){
	pControllers.Move(controller, index);
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::RemoveController(aeController *controller){
	if(!pControllers.Has(controller)){
		DETHROW(deeInvalidParam);
	}
	
	if(pActiveController == controller){
		if(pControllers.GetCount() > 1){
			if(pControllers.GetAt(0) == controller){
				SetActiveController(pControllers.GetAt(1));
				
			}else{
				SetActiveController(pControllers.GetAt(0));
			}
			
		}else{
			SetActiveController(nullptr);
		}
	}
	
	controller->SetAnimator(nullptr);
	pControllers.Remove(controller);
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::RemoveAllControllers(){
	SetActiveController(nullptr);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetAnimator(nullptr);
	}
	pControllers.RemoveAll();
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::SetActiveController(aeController *controller){
	if(controller == pActiveController){
		return;
	}
	
	pActiveController = controller;
	NotifyActiveControllerChanged();
}

void aeAnimator::ResetControllers(){
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->ResetValue();
	}
}

void aeAnimator::ResetControllersWith(int locomotionAttribute){
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(pControllers.GetAt(i)->GetLocomotionAttribute() == locomotionAttribute){
			pControllers.GetAt(i)->ResetValue();
		}
	}
}

void aeAnimator::InverseControllersWith(int locomotionAttribute){
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(pControllers.GetAt(i)->GetLocomotionAttribute() == locomotionAttribute){
			pControllers.GetAt(i)->InverseValue();
		}
	}
}

void aeAnimator::IncrementControllersWith(int locomotionAttribute, float incrementBy){
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		if(pControllers.GetAt(i)->GetLocomotionAttribute() == locomotionAttribute){
			pControllers.GetAt(i)->IncrementCurrentValue(incrementBy);
		}
	}
}



// Links
//////////

void aeAnimator::AddLink(aeLink *link){
	pLinks.Add(link);
	link->SetAnimator(this);
	NotifyLinkStructureChanged();
}

void aeAnimator::RemoveLink(aeLink *link){
	if(!pLinks.Has(link)){
		DETHROW(deeInvalidParam);
	}
	
	if(pActiveLink == link){
		if(pLinks.GetCount() > 1){
			if(pLinks.GetAt(0) == link){
				SetActiveLink(pLinks.GetAt(1));
				
			}else{
				SetActiveLink(pLinks.GetAt(0));
			}
			
		}else{
			SetActiveLink(nullptr);
		}
	}
	
	link->SetAnimator(nullptr);
	pLinks.Remove(link);
	
	RebuildRules();
	NotifyLinkStructureChanged();
}

void aeAnimator::RemoveAllLinks(){
	SetActiveLink(nullptr);
	
	const int ruleCount = pRules.GetCount();
	int i;
	for(i=0; i<ruleCount; i++){
		pRules.GetAt(i)->RemoveLinksFromAllTargets();
	}
	
	const int count = pLinks.GetCount();
	for(i=0; i<count; i++){
		pLinks.GetAt(i)->SetAnimator(nullptr);
	}
	pLinks.RemoveAll();
	
	RebuildRules();
	NotifyLinkStructureChanged();
}

void aeAnimator::SetActiveLink(aeLink *link){
	if(link == pActiveLink){
		return;
	}
	
	pActiveLink = link;
	NotifyActiveLinkChanged();
}

int aeAnimator::CountLinkUsage(aeLink *link) const{
	const int ruleCount = pRules.GetCount();
	int i, usageCount = 0;
	
	for(i=0; i<ruleCount; i++){
		usageCount += pRules.GetAt(i)->CountLinkUsage(link);
	}
	
	return usageCount;
}



// Rules
//////////

void aeAnimator::AddRule(aeRule *rule){
	pRules.Add(rule);
	rule->SetAnimator(this);
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::InsertRuleAt(aeRule *rule, int index){
	pRules.Insert(rule, index);
	rule->SetAnimator(this);
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::MoveRuleTo(aeRule *rule, int index){
	pRules.Move(rule, index);
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::RemoveRule(aeRule *rule){
	if(!pRules.Has(rule)){
		DETHROW(deeInvalidParam);
	}
	
	if(pActiveRule == rule){
		if(pRules.GetCount() > 1){
			if(pRules.GetAt(0) == rule){
				SetActiveRule(pRules.GetAt(1));
				
			}else{
				SetActiveRule(pRules.GetAt(0));
			}
			
		}else{
			SetActiveRule(nullptr);
		}
	}
	
	rule->SetAnimator(nullptr);
	pRules.Remove(rule);
	
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::RemoveAllRules(){
	SetActiveRule(nullptr);
	
	const int count = pRules.GetCount();
	int i;
	for(i=0; i<count; i++){
		pRules.GetAt(i)->SetAnimator(nullptr);
	}
	pRules.RemoveAll();
	
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::SetActiveRule(aeRule *rule){
	if(rule == pActiveRule){
		return;
	}
	
	pActiveRule = rule;
	NotifyActiveRuleChanged();
}

void aeAnimator::RebuildRules(){
	const int count = pRules.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRules.GetAt(i)->SetEngineRule(nullptr);
	}
	
	if(!pEngAnimator){
		return;
	}
	
	pEngAnimator->RemoveAllRules();
	
	if(pResetState){
		const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
		engRule->SetUseLastState(true);
		pEngAnimator->AddRule(engRule);
	}
	
	for(i=0; i<count; i++){
		const deAnimatorRule::Ref engRule(pRules.GetAt(i)->CreateEngineRule());
		pEngAnimator->AddRule(engRule);
		pRules.GetAt(i)->SetEngineRule(engRule);
	}
}



// Bone Management
////////////////////

void aeAnimator::SetListBones(const decStringSet &bones){
	if(bones == pListBones){
		return;
	}
	
	pListBones = bones;
	
	if(pEngAnimator){
		pEngAnimator->GetListBones() = bones;
		pEngAnimator->NotifyBonesChanged();
	}
	
	NotifyAnimatorChanged();
}

void aeAnimator::AddBone(const char *bone){
	if(!bone) DETHROW(deeInvalidParam);
	
	if(!pListBones.Has(bone)){
		pListBones.Add(bone);
		
		if(pEngAnimator){
			pEngAnimator->GetListBones().Add(bone);
			pEngAnimator->NotifyBonesChanged();
		}
		
		NotifyAnimatorChanged();
	}
}

void aeAnimator::RemoveBone(const char *bone){
	if(pListBones.Has(bone)){
		pListBones.Remove(bone);
		
		if(pEngAnimator){
			pEngAnimator->GetListBones().Remove(bone);
			pEngAnimator->NotifyBonesChanged();
		}
		
		NotifyAnimatorChanged();
	}
}

void aeAnimator::RemoveAllBones(){
	if(pListBones.GetCount() > 0){
		pListBones.RemoveAll();
		
		if(pEngAnimator){
			pEngAnimator->GetListBones().RemoveAll();
			pEngAnimator->NotifyBonesChanged();
		}
		
		NotifyAnimatorChanged();
	}
}



// Vertex position set management
///////////////////////////////////

void aeAnimator::SetListVertexPositionSets(const decStringSet &sets){
	if(sets == pListVertexPositionSets){
		return;
	}
	
	pListVertexPositionSets = sets;
	
	if(pEngAnimator){
		pEngAnimator->GetListVertexPositionSets() = sets;
		pEngAnimator->NotifyVertexPositionSetsChanged();
	}
	
	NotifyAnimatorChanged();
}

void aeAnimator::AddVertexPositionSet(const char *vertexPositionSet){
	DEASSERT_NOTNULL(vertexPositionSet)
	
	if(!pListVertexPositionSets.Has(vertexPositionSet)){
		pListVertexPositionSets.Add(vertexPositionSet);
		
		if(pEngAnimator){
			pEngAnimator->GetListVertexPositionSets().Add(vertexPositionSet);
			pEngAnimator->NotifyVertexPositionSetsChanged();
		}
		
		NotifyAnimatorChanged();
	}
}

void aeAnimator::RemoveVertexPositionSet(const char *vertexPositionSet){
	if(pListVertexPositionSets.Has(vertexPositionSet)){
		pListVertexPositionSets.Remove(vertexPositionSet);
		
		if(pEngAnimator){
			pEngAnimator->GetListVertexPositionSets().Remove(vertexPositionSet);
			pEngAnimator->NotifyVertexPositionSetsChanged();
		}
		
		NotifyAnimatorChanged();
	}
}

void aeAnimator::RemoveAllVertexPositionSets(){
	if(pListVertexPositionSets.GetCount() > 0){
		pListVertexPositionSets.RemoveAll();
		
		if(pEngAnimator){
			pEngAnimator->GetListVertexPositionSets().RemoveAll();
			pEngAnimator->NotifyVertexPositionSetsChanged();
		}
		
		NotifyAnimatorChanged();
	}
}



// Attachments
////////////////

aeAttachment *aeAnimator::GetAttachmentNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pAttachments.FindOrDefault([&](const aeAttachment &a){ return a.GetName() == name; });
}

void aeAnimator::AddAttachment(aeAttachment *attachment){
	pAttachments.Add(attachment);
	attachment->SetAnimator(this);
	
	NotifyAttachmentStructureChanged();
}

void aeAnimator::RemoveAttachment(aeAttachment *attachment){
	const aeAttachment::Ref guard(attachment);
	pAttachments.Remove(attachment);
	attachment->SetAnimator(nullptr);
	NotifyAttachmentStructureChanged();
}

void aeAnimator::RemoveAllAttachments(){
	SetActiveAttachment(nullptr);
	
	pAttachments.Visit([](aeAttachment *attachment){
		attachment->SetAnimator(nullptr);
	});
	pAttachments.RemoveAll();
	
	NotifyAttachmentStructureChanged();
}

void aeAnimator::SetActiveAttachment(aeAttachment *attachment){
	if(attachment == pActiveAttachment){
		return;
	}
	
	pActiveAttachment = attachment;
	NotifyActiveAttachmentChanged();
}

void aeAnimator::AttachAttachments(){
	pAttachments.Visit([](aeAttachment *attachment){
		attachment->AttachCollider();
	});
}

void aeAnimator::DetachAttachments(){
	pAttachments.Visit([](aeAttachment *attachment){
		attachment->DetachCollider();
	});
}

void aeAnimator::AttachmentsResetPhysics(){
	pAttachments.Visit([](aeAttachment *attachment){
		attachment->ResetPhysics();
	});
}



// Notifiers
//////////////

void aeAnimator::AddNotifier(aeAnimatorNotifier *notifier){
	pNotifiers.Add(notifier);
}

void aeAnimator::RemoveNotifier(aeAnimatorNotifier *notifier){
	pNotifiers.Remove(notifier);
}

void aeAnimator::RemoveAllNotifiers(){
	pNotifiers.RemoveAll();
}


void aeAnimator::NotifyStateChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->StateChanged(this);
	});
}

void aeAnimator::NotifyUndoChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->UndoChanged(this);
	});
}

void aeAnimator::NotifyAnimatorChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->AnimatorChanged(this);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyViewChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ViewChanged(this);
	});
}

void aeAnimator::NotifyModelChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ModelChanged(this);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifySkyChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->SkyChanged(this);
	});
}

void aeAnimator::NotifyEnvObjectChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->EnvObjectChanged(this);
	});
}

void aeAnimator::NotifyRigChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->RigChanged(this);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyAnimationChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->AnimationChanged(this);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyPlaybackChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->PlaybackChanged(this);
	});
}

void aeAnimator::NotifyLocomotionChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->LocomotionChanged(this);
	});
}



void aeAnimator::NotifyActiveControllerChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ActiveControllerChanged(this, pActiveController);
	});
}

void aeAnimator::NotifyControllerChanged(aeController *controller){
	DEASSERT_NOTNULL(controller)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ControllerChanged(this, controller);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyControllerNameChanged(aeController *controller){
	DEASSERT_NOTNULL(controller)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ControllerNameChanged(this, controller);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyControllerValueChanged(aeController *controller){
	DEASSERT_NOTNULL(controller)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ControllerValueChanged(this, controller);
	});
}

void aeAnimator::NotifyControllerStructureChanged(){
	pUpdateEngineControllers();
	
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ControllerStructureChanged(this);
	});
	
	SetChanged(true);
}



void aeAnimator::NotifyActiveLinkChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ActiveLinkChanged(this, pActiveLink);
	});
}

void aeAnimator::NotifyLinkChanged(aeLink *link){
	DEASSERT_NOTNULL(link)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->LinkChanged(this, link);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyLinkNameChanged(aeLink *link){
	DEASSERT_NOTNULL(link)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->LinkNameChanged(this, link);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyLinkStructureChanged(){
	pUpdateLinks();
	
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->LinkStructureChanged(this);
	});
	
	SetChanged(true);
}



void aeAnimator::NotifyActiveRuleChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ActiveRuleChanged(this, pActiveRule);
	});
}

void aeAnimator::NotifyRuleChanged(aeRule *rule){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->RuleChanged(this, rule);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyRuleNameChanged(aeRule *rule){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->RuleNameChanged(this, rule);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyRuleStructureChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->RuleStructureChanged(this);
	});
	
	SetChanged(true);
}



void aeAnimator::NotifyActiveAttachmentChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ActiveAttachmentChanged(this, pActiveAttachment);
	});
}

void aeAnimator::NotifyAttachmentChanged(aeAttachment *attachment){
	DEASSERT_NOTNULL(attachment)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->AttachmentChanged(this, attachment);
	});
}

void aeAnimator::NotifyAttachmentStructureChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->AttachmentStructureChanged(this);
	});
}



void aeAnimator::NotifyCameraChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->CameraChanged(this);
	});
}

void aeAnimator::NotifyCameraViewChanged(){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->CameraViewChanged(this);
	});
}



// Private Functions
//////////////////////

void aeAnimator::pCleanUp(){
	if(pDDSBones){
		delete [] pDDSBones;
	}
	
	RemoveAllNotifiers();
	
	pEnvObject = nullptr;
	if(pSky){
		delete pSky;
	}
	
	if(pCamera){
		delete pCamera;
	}
	
	RemoveAllAttachments();
	RemoveAllRules();
	RemoveAllLinks();
	RemoveAllControllers();
	
	if(pTestingSubAnimator){
		delete pTestingSubAnimator;
	}
	if(pSubAnimator){
		delete pSubAnimator;
	}
	if(pWakeboard){
		delete pWakeboard;
	}
	if(pLocomotion){
		delete pLocomotion;
	}
	
	if(pEngAnimatorInstance){
		pEngAnimatorInstance->SetAnimator(nullptr);
		pEngAnimatorInstance->SetComponent(nullptr);
	}
	if(pEngAnimator){
		pEngAnimator->SetRig(nullptr);
	}
	
	if(pEngWorld){
		if(pDDBones){
			pEngWorld->RemoveDebugDrawer(pDDBones);
		}
		
		if(pEngComponent){
			pEngWorld->RemoveComponent(pEngComponent);
		}
		
		if(pEngCollider){
			pEngCollider->SetComponent(nullptr);
			pEngWorld->RemoveCollider(pEngCollider);
		}
		
		if(pEngLight){
			pEngWorld->RemoveLight(pEngLight);
		}
	}
}



void aeAnimator::pCreateWorld(){
	deEngine * const engine = GetEngine();
	
	// create world
	pEngWorld = engine->GetWorldManager()->CreateWorld();
	pEngWorld->SetGravity(decVector(0.0f, -9.81f, 0.0f));
	pEngWorld->SetDisableLights(false);
	
	pEngWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f));
	
	// create animator
	pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
	
	// create sub animator
	pSubAnimator = new aeSubAnimator(engine);
	
	// create testing sub animator
	pTestingSubAnimator = new aeSubAnimator(engine);
}

void aeAnimator::pCreateCamera(){
	pCamera = new aeCamera(this, GetEngine());
	
	pCamera->SetEngineWorld(pEngWorld);
	
	pCamera->Reset();
}

void aeAnimator::pCreateCollider(){
	pEngCollider = GetEngine()->GetColliderManager()->CreateColliderComponent();
	
	pEngCollider->SetResponseType(deCollider::ertKinematic);
	pEngCollider->SetUseLocalGravity(true);
	pEngCollider->SetMass(50.0f);
	pEngCollider->SetEnabled(false);
	
	decCollisionFilter filter(pEngCollider->GetCollisionFilter());
	filter.GetCategory().SetBit(aeAnimator::eclElements);
	filter.GetFilter().SetBit(aeAnimator::eclElements);
	pEngCollider->SetCollisionFilter(filter);
	
	pEngWorld->AddCollider(pEngCollider);
}

void aeAnimator::pUpdateComponent(){
	deEngine * const engine = GetEngine();
	deModel::Ref displayModel;
	deSkin::Ref displaySkin;
	deRig::Ref displayRig;
	
	// detach all colliders
	DetachAttachments();
	
	// disable collider
	pEngCollider->SetComponent(nullptr);
	pEngCollider->SetEnabled(false);
	
	// try to load the model, skin and rig if possible
	try{
		if(!pDisplayModelPath.IsEmpty()){
			displayModel = engine->GetModelManager()->LoadModel(pDisplayModelPath, GetDirectoryPath());
		}
		
		if(!pDisplaySkinPath.IsEmpty()){
			displaySkin = engine->GetSkinManager()->LoadSkin(pDisplaySkinPath, GetDirectoryPath());
		}
		
		if(!pDisplayRigPath.IsEmpty()){
			displayRig = engine->GetRigManager()->LoadRig(pDisplayRigPath, GetDirectoryPath());
		}
		
		if(pRigPath.IsEmpty()){
			pEngRig = nullptr;
			
		}else{
			pEngRig = engine->GetRigManager()->LoadRig(pRigPath, GetDirectoryPath());
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	// protect the loaded parts
	try{
		// if the skin is missing use the default one
		if(!displaySkin){
			displaySkin = GetGameDefinition()->GetDefaultSkin();
		}
		
		// reset the animator
		pEngAnimatorInstance->SetComponent(nullptr); // otherwise the animator is not reset
		
		// update the component with the model and skin
		if(displayModel && displaySkin){
			if(pEngComponent){
				pEngComponent->SetModelAndSkin(displayModel, displaySkin);
				
			}else{
				pEngComponent = engine->GetComponentManager()->CreateComponent(displayModel, displaySkin);
				pEngWorld->AddComponent(pEngComponent);
				
				pEngCollider->AddAttachment(new deColliderAttachment(pEngComponent));
			}
			
		}else if(pEngComponent){
			deColliderAttachment * const attachment = pEngCollider->GetAttachmentWith(pEngComponent);
			if(attachment){
				pEngCollider->RemoveAttachment(attachment);
			}
			
			pEngWorld->RemoveComponent(pEngComponent);
			pEngComponent = nullptr;
		}
		
		// set the rig if the component exists
		if(pEngComponent){
			pEngComponent->SetRig(displayRig);
			pEngComponent->SetVisible(true);
			pEngComponent->SetPosition(decDVector());
			pEngComponent->SetOrientation(decQuaternion());
		}
		
		// set animator rig
		pEngAnimator->SetRig(pEngRig);
		
		// free the reference we hold
		if(displayRig){
			displayRig = nullptr;
		}
		if(displayModel){
			displayModel = nullptr;
		}
		if(displaySkin){
			displaySkin = nullptr;
		}
		
	}catch(const deException &){
		throw;
	}
	
	// update the collider
	pEngCollider->SetComponent(pEngComponent);
	pEngCollider->SetEnabled(pEngComponent != nullptr);
	
	// update the animator
	pEngAnimatorInstance->SetComponent(pEngComponent);
	pUpdateAnimator();
	
	// notify rules
	pAnimCompChanged();
	
	// attach colliders
	AttachAttachments();
}

void aeAnimator::pUpdateAnimator(){
	deAnimation::Ref animation;
	
	try{
		if(!pAnimationPath.IsEmpty()){
			animation = GetEngine()->GetAnimationManager()->
				LoadAnimation(pAnimationPath, GetDirectoryPath());
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	pEngAnimator->SetAnimation(animation);
	
	pSubAnimator->SetComponentAndAnimation(pEngComponent, animation);
// 	pTestingSubAnimator->SetComponentAndAnimation( pEngComponent, animation );
	pTestingSubAnimator->SetComponent(pEngComponent);
	
	pAnimCompChanged();
}

void aeAnimator::pUpdateLinks(){
	const int count = pLinks.GetCount();
	int i;
	for(i=0; i<count; i++){
		pLinks.GetAt(i)->UpdateController();
	}
	RebuildRules();
}

void aeAnimator::pAnimCompChanged(){
	const int count = pRules.GetCount();
	int i;
	for(i=0; i<count; i++){
		pRules.GetAt(i)->UpdateCompAnim();
	}
}

void aeAnimator::pUpdateEngineControllers(){
	deAnimatorController *engController = nullptr;
	int i;
	
	// set all engine controller indices in our controllers to -1
	const int count = pControllers.GetCount();
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetEngineControllerIndex(-1);
	}
	
	// remove the animator from the animator instance
	pEngAnimatorInstance->SetAnimator(nullptr);
	
	// remove all controllers
	pEngAnimator->RemoveAllControllers();
	
	// add an engine controller for each controller we have
	try{
		for(i=0; i<count; i++){
			engController = new deAnimatorController;
			pEngAnimator->AddController(engController);
			engController = nullptr;
		}
		
	}catch(const deException &){
		if(engController){
			delete engController;
		}
		
		throw;
	}
	
	// assign the animator to the animator instance. this creates the controllers
	// inside the animator instance
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
	
	// now assign the matching engine controller indices to our controllers
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetEngineControllerIndex(i);
	}
	
	// links have to be updated now
	pUpdateLinks();
}

void aeAnimator::pUpdateDDSBones(){
	decDMatrix matrix;
	int boneCount = 0;
	int i;
	
	if(pEngComponent){
		boneCount = pEngComponent->GetBoneCount();
		pEngComponent->PrepareBones();
		matrix = decDMatrix(pEngComponent->GetMatrix()).Normalized();
	}
	
	if(boneCount != pDDSBoneCount){
		if(pDDSBones){
			delete [] pDDSBones;
			pDDSBones = nullptr;
			pDDSBoneCount = 0;
		}
		
		if(boneCount > 0){
			pDDSBones = new igdeWCoordSysArrows[boneCount];
			
			for(pDDSBoneCount=0; pDDSBoneCount<boneCount; pDDSBoneCount++){
				pDDSBones[pDDSBoneCount].SetParentDebugDrawer(pDDBones);
			}
		}
	}
	
	pDDBones->SetPosition(matrix.GetPosition());
	pDDBones->SetOrientation(matrix.ToQuaternion());
	
	for(i=0; i<pDDSBoneCount; i++){
		if(i < boneCount && pEngComponent){
			const decMatrix boneMatrix = pEngComponent->GetBoneAt(i).GetMatrix().Normalized();
			pDDSBones[i].SetPosition(boneMatrix.GetPosition());
			pDDSBones[i].SetOrientation(boneMatrix.ToQuaternion());
			pDDSBones[i].SetScale(decVector(pDDSBoneSize, pDDSBoneSize, pDDSBoneSize));
			pDDSBones[i].SetVisible(true);
			
		}else{
			pDDSBones[i].SetVisible(false);
		}
	}
}
