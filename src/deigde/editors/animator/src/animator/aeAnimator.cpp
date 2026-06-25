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
pWindowMain(windowMain),
pMetaContext(aeMCAnimator::Ref::New(windowMain, this)),
pMetaContextController(aeMCAnimatorController::Ref::New(windowMain, this)),
pMetaContextLink(aeMCAnimatorLink::Ref::New(windowMain, this)),
pMetaContextRule(aeMCAnimatorRule::Ref::New(windowMain, this)),
pMetaContextAttachment(aeMCAnimatorAttachment::Ref::New(windowMain, this)),
pMetaContextView(aeMCAnimatorView::Ref::New(windowMain, this)),
hiddenBoneNames(pWindowMain.GetMCAnimatorProperties().hiddenBoneNames, pMetaContext),
hiddenVPSNames(pWindowMain.GetMCAnimatorProperties().hiddenVPSNames, pMetaContext),
hiddenMoveNames(pWindowMain.GetMCAnimatorProperties().hiddenMoveNames, pMetaContext),
rigPath(pWindowMain.GetMCAnimatorProperties().rig, pMetaContext),
animationPath(pWindowMain.GetMCAnimatorProperties().animation, pMetaContext),
affectedBones(pWindowMain.GetMCAnimatorProperties().affectedBones, pMetaContext),
affectedVertexPositionSets(pWindowMain.GetMCAnimatorProperties().affectedVertexPositionSets, pMetaContext),
controllers(pWindowMain.GetMCAnimatorProperties().controller.controllers, pMetaContextController),
controller(pWindowMain.GetMCAnimatorProperties().controller.controller, pMetaContextController),
links(pWindowMain.GetMCAnimatorProperties().link.links, pMetaContextLink),
link(pWindowMain.GetMCAnimatorProperties().link.link, pMetaContextLink),
ruleTree(pWindowMain.GetMCAnimatorProperties().rule.ruleTree, pMetaContextRule),
rules(pWindowMain.GetMCAnimatorProperties().rule.rules, pMetaContextRule),
rule(pWindowMain.GetMCAnimatorProperties().rule.rule, pMetaContextRule),
displayModelPath(pWindowMain.GetMCAnimatorProperties().displayModelPath, pMetaContextView),
displaySkinPath(pWindowMain.GetMCAnimatorProperties().displaySkinPath, pMetaContextView),
displayRigPath(pWindowMain.GetMCAnimatorProperties().displayRigPath, pMetaContextView),
playSpeed(pWindowMain.GetMCAnimatorProperties().playSpeed, pMetaContextView),
timeStep(pWindowMain.GetMCAnimatorProperties().timeStep, pMetaContextView),
resetState(pWindowMain.GetMCAnimatorProperties().resetState, pMetaContextView),
attachments(pWindowMain.GetMCAnimatorProperties().attachment.attachments, pMetaContextView),
attachment(pWindowMain.GetMCAnimatorProperties().attachment.attachment, pMetaContextView)
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
	
	pCamera = nullptr;
	
	pPaused = false;
	
	pDDBones = nullptr;
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
	
	uniqueNameController.SetIsUnique([this](const decString &name){
		return !controllers->HasNamed(name);
	});
	
	uniqueNameLink.SetIsUnique([this](const decString &name){
		return !links->HasMatching([&](const aeLink &each){
			return each.GetName() == name;
		});
	});
	
	uniqueNameRule.SetIsUnique([this](const decString &name){
		return !rules->HasMatching([&](const aeRule &each){
			return each.GetName() == name;
		});
	});
	
	uniqueNameAttachment.SetIsUnique([this](const decString &name){
		return !attachments->HasNamed(name);
	});
	
	rigPath.SetOnChanged([this](){
		pUpdateComponent();
		NotifyRigChanged();
	});
	
	animationPath.SetOnChanged([this](){
		pUpdateAnimator();
		pCamera->SetBone("");
		NotifyAnimationChanged();
	});
	
	affectedBones.SetOnChanged([this](){
		if(pEngAnimator){
			pEngAnimator->GetListBones() = affectedBones;
			pEngAnimator->NotifyBonesChanged();
		}
		NotifyAnimatorChanged();
	});
	
	affectedVertexPositionSets.SetOnChanged([this](){
		if(pEngAnimator){
			pEngAnimator->GetListVertexPositionSets() = affectedVertexPositionSets;
			pEngAnimator->NotifyVertexPositionSetsChanged();
		}
		NotifyAnimatorChanged();
	});
	
	controllers.SetOnChanged([this](){
		allowedListControllers = igdeMetaPropertyObjectType<aeController>::ObjectTypeList::New(controllers);
		pUpdateLinks();
		NotifyControllerStructureChanged();
	});
	controllers.SetOnObjectAdded([this](aeController &each){
		each.SetAnimator(this);
	});
	controllers.SetOnObjectRemoved([this](aeController &each){
		each.SetAnimator(nullptr);
	});
	controllers.SetOnActiveChanged([this](){
		NotifyActiveControllerChanged();
		const auto &active = controllers.GetActive();
		controller.SetValue(active ? active->GetMetaContext() : aeMCController::Ref());
	});
	
	links.SetOnChanged([this](){
		RebuildRules();
		NotifyLinkStructureChanged();
	});
	links.SetOnObjectAdded([this](aeLink &each){
		each.SetAnimator(this);
	});
	links.SetOnObjectRemoved([this](aeLink &each){
		each.SetAnimator(nullptr);
	});
	links.SetOnActiveChanged([this](){
		NotifyActiveLinkChanged();
		const auto &active = links.GetActive();
		link.SetValue(active ? active->GetMetaContext() : aeMCLink::Ref());
	});
	
	ruleTree.SetOnActiveChanged([this](){
		NotifyActiveRuleChanged();
		const auto &active = ruleTree.GetActive();
		rule.SetValue(active ? active->GetMetaContext() : aeMCRule::Ref());
	});
	
	rules.SetOnChanged([this](){
		pUpdateRuleIndices();
		RebuildRules();
		NotifyRuleStructureChanged();
	});
	rules.SetOnObjectAdded([this](aeRule &each){
		each.SetAnimator(this);
	});
	rules.SetOnObjectRemoved([this](aeRule &each){
		each.SetAnimator(nullptr);
	});
	
	attachments.SetOnChanged([this](){
		NotifyAttachmentStructureChanged();
	});
	attachments.SetOnObjectAdded([this](aeAttachment &each){
		each.SetAnimator(this);
	});
	attachments.SetOnObjectRemoved([this](aeAttachment &each){
		each.SetAnimator(nullptr);
	});
	attachments.SetOnActiveChanged([this](){
		NotifyActiveAttachmentChanged();
		const auto &active = attachments.GetActive();
		attachment.SetValue(active ? active->GetMetaContext() : aeMCAttachment::Ref());
	});
	
	displayModelPath.SetOnChanged([this](){
		pUpdateComponent();
	NotifyModelChanged();
	});
	displaySkinPath.SetOnChanged(displayModelPath.GetOnChanged());
	displayRigPath.SetOnChanged(displayModelPath.GetOnChanged());
	
	playSpeed.SetOnChanged([this](){
		NotifyPlaybackChanged();
	});
	timeStep.SetOnChanged(playSpeed.GetOnChanged());
	
	resetState.SetOnChanged([this](){
		RebuildRules();
		NotifyViewChanged();
	});
	
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
	if(pMetaContextRule){
		pMetaContextRule->Dispose();
		pMetaContextRule.Clear();
	}
	if(pMetaContextLink){
		pMetaContextLink->Dispose();
		pMetaContextLink.Clear();
	}
	if(pMetaContextController){
		pMetaContextController->Dispose();
		pMetaContextController.Clear();
	}
	if(pMetaContextView){
		pMetaContextView->Dispose();
		pMetaContextView.Clear();
	}
	if(pMetaContextAttachment){
		pMetaContextAttachment->Dispose();
		pMetaContextAttachment.Clear();
	}
	if(pMetaContext){
		pMetaContext->Dispose();
		pMetaContext.Clear();
	}
	RemoveAllRules();
	RemoveAllLinks();
	RemoveAllControllers();
}

void aeAnimator::Reset(){
	GetUndoSystem()->RemoveAll();
}

void aeAnimator::SetDisplayModelPath(const char *path){
	displayModelPath = path;
}

void aeAnimator::SetDisplaySkinPath(const char *path){
	displaySkinPath = path;
}

void aeAnimator::SetDisplayRigPath(const char *path){
	displayRigPath = path;
}

void aeAnimator::SetRigPath(const char *path){
	rigPath = path;
}

void aeAnimator::SetAnimationPath(const char *path){
	animationPath = path;
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
	const float controllerElapsed = elapsed * playSpeed;
	const float realTimeElapsed = elapsed * playSpeed;
	
	// update the locomotion testing
	pLocomotion->Update(realTimeElapsed);
	
	// update the controllers which are linked to the elapsed time
	if(!pPaused){
		controllers->Visit([&](aeController &each){
			each.UpdateValue(controllerElapsed);
		});
	}
	
	// reset the animation states if required
	if(pEngComponent && resetState){
		pEngComponent->GetBones().Visit([](deComponentBone &bone){
			bone.SetPosition(decVector());
			bone.SetRotation(decQuaternion());
			bone.SetScale(decVector(1.0f, 1.0f, 1.0f));
		});
		
		const int vertexPositionSetCount = pEngComponent->GetVertexPositionSetWeights().GetCount();
		int i;
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
		attachments->Visit([&](aeAttachment &each){
			each.Update(realTimeElapsed);
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

void aeAnimator::SetPlaySpeed(float value){
	playSpeed = value;
}

void aeAnimator::SetTimeStep(float value){
	timeStep = value;
}



void aeAnimator::SetResetState(bool value){
	resetState = value;
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
	pathAttachmentConfig = path;
}



// Controllers
////////////////

void aeAnimator::AddController(aeController *acontroller){
	auto list = controllers.GetValue();
	list.AddOrThrow(acontroller);
	controllers = list;
}

void aeAnimator::InsertControllerAt(aeController *acontroller, int index){
	auto list = controllers.GetValue();
	list.InsertOrThrow(acontroller, index);
	controllers = list;
}

void aeAnimator::MoveControllerTo(aeController *acontroller, int index){
	auto list = controllers.GetValue();
	list.Move(acontroller, index);
	controllers = list;
}

void aeAnimator::RemoveController(aeController *acontroller){
	auto list = controllers.GetValue();
	list.RemoveOrThrow(acontroller);
	controllers = list;
}

void aeAnimator::RemoveAllControllers(){
	controllers = {};
}

void aeAnimator::SetActiveController(aeController *acontroller){
	controllers.SetActive(acontroller);
}

void aeAnimator::ResetControllers(){
	controllers->Visit([](aeController &each){
		each.ResetValue();
	});
}
void aeAnimator::ResetControllersWith(int locomotionAttribute){
	controllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.ResetValue();
		}
	});
}
	
void aeAnimator::InverseControllersWith(int locomotionAttribute){
	controllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.InverseValue();
		}
	});
}

void aeAnimator::IncrementControllersWith(int locomotionAttribute, float incrementBy){
	controllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.IncrementCurrentValue(incrementBy);
		}
	});
}



// Links
//////////

void aeAnimator::AddLink(aeLink *alink){
	auto list = links.GetValue();
	list.AddOrThrow(alink);
	links = list;
}

void aeAnimator::RemoveLink(aeLink *alink){
	auto list = links.GetValue();
	list.RemoveOrThrow(alink);
	links = list;
}

void aeAnimator::RemoveAllLinks(){
	links = {};
}

void aeAnimator::SetActiveLink(aeLink *alink){
	links.SetActive(alink);
}

int aeAnimator::CountLinkUsage(aeLink *alink) const{
	int count = 0;
	rules.GetValue().Visit([&](const aeRule &each){
		count += each.CountLinkUsage(alink);
	});
	return count;
}



// Rules
//////////

void aeAnimator::AddRule(aeRule *arule){
	auto list = rules.GetValue();
	list.AddOrThrow(arule);
	rules = list;
}

void aeAnimator::InsertRuleAt(aeRule *arule, int index){
	auto list = rules.GetValue();
	list.InsertOrThrow(arule, index);
	rules = list;
}

void aeAnimator::MoveRuleTo(aeRule *arule, int index){
	auto list = rules.GetValue();
	list.Move(arule, index);
	rules = list;
}

void aeAnimator::RemoveRule(aeRule *arule){
	auto list = rules.GetValue();
	list.RemoveOrThrow(arule);
	rules = list;
}

void aeAnimator::RemoveAllRules(){
	rules = {};
}

void aeAnimator::SetActiveRule(aeRule *arule){
	ruleTree.SetActive(arule);
}

void aeAnimator::RebuildRules(){
	rules.GetValue().Visit([](aeRule &each){
		each.SetEngineRule(nullptr);
	});
	
	if(!pEngAnimator){
		return;
	}
	
	pEngAnimator->RemoveAllRules();
	
	if(resetState){
		const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
		engRule->SetUseLastState(true);
		pEngAnimator->AddRule(engRule);
	}
	
	rules.GetValue().Visit([&](aeRule &each){
		const deAnimatorRule::Ref engRule(each.CreateEngineRule());
		pEngAnimator->AddRule(engRule);
		each.SetEngineRule(engRule);
	});
}



// Bone Management
////////////////////

void aeAnimator::SetListBones(const decStringSet &bones){
	affectedBones = bones;
}

void aeAnimator::AddBone(const char *bone){
	affectedBones = affectedBones.GetValue() + decStringSet(devctag, bone);
}

void aeAnimator::RemoveBone(const char *bone){
	affectedBones = affectedBones.GetValue() - decStringSet(devctag, bone);
}

void aeAnimator::RemoveAllBones(){
	affectedBones = {};
}


// Vertex position set management
///////////////////////////////////

void aeAnimator::SetListVertexPositionSets(const decStringSet &sets){
	affectedVertexPositionSets = sets;
}

void aeAnimator::AddVertexPositionSet(const char *vps){
	affectedVertexPositionSets = affectedVertexPositionSets.GetValue() + decStringSet(devctag, vps);
}

void aeAnimator::RemoveVertexPositionSet(const char *vps){
	affectedVertexPositionSets = affectedVertexPositionSets.GetValue() - decStringSet(devctag, vps);
}

void aeAnimator::RemoveAllVertexPositionSets(){
	affectedVertexPositionSets = {};
}


// Attachments
////////////////

aeAttachment *aeAnimator::GetAttachmentNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return attachments->FindOrDefault([&](const aeAttachment &a){ return a.GetName() == name; });
}

void aeAnimator::AddAttachment(aeAttachment *each){
	auto set = attachments.GetValue();
	set.AddOrThrow(each);
	attachments = set;
}

void aeAnimator::RemoveAttachment(aeAttachment *each){
	auto set = attachments.GetValue();
	set.RemoveOrThrow(each);
	attachments = set;
}

void aeAnimator::RemoveAllAttachments(){
	attachments = {};
}

void aeAnimator::SetActiveAttachment(aeAttachment *each){
	attachments.SetActive(each);
}

void aeAnimator::AttachAttachments(){
	attachments->Visit([](aeAttachment &each){
		each.AttachCollider();
	});
}

void aeAnimator::DetachAttachments(){
	attachments->Visit([](aeAttachment &each){
		each.DetachCollider();
	});
}

void aeAnimator::AttachmentsResetPhysics(){
	attachments->Visit([](aeAttachment &each){
		each.ResetPhysics();
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

void aeAnimator::NotifyBasePathChanged(){
	auto &properties = pWindowMain.GetMCAnimatorProperties();
	properties.rig->NotifyBasePathChanged(pMetaContext);
	properties.animation->NotifyBasePathChanged(pMetaContext);
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
		listener->ActiveControllerChanged(this, controllers.GetActive());
	});
}

void aeAnimator::NotifyControllerChanged(aeController *acontroller){
	DEASSERT_NOTNULL(acontroller)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ControllerChanged(this, acontroller);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyControllerNameChanged(aeController *acontroller){
	DEASSERT_NOTNULL(acontroller)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ControllerNameChanged(this, acontroller);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyControllerValueChanged(aeController *acontroller){
	DEASSERT_NOTNULL(acontroller)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->ControllerValueChanged(this, acontroller);
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
		listener->ActiveLinkChanged(this, links.GetActive());
	});
}

void aeAnimator::NotifyLinkChanged(aeLink *alink){
	DEASSERT_NOTNULL(alink)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->LinkChanged(this, alink);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyLinkNameChanged(aeLink *alink){
	DEASSERT_NOTNULL(alink)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->LinkNameChanged(this, alink);
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
		listener->ActiveRuleChanged(this, rules.GetActive());
	});
}

void aeAnimator::NotifyRuleChanged(aeRule *arule){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->RuleChanged(this, arule);
	});
	
	SetChanged(true);
}

void aeAnimator::NotifyRuleNameChanged(aeRule *arule){
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->RuleNameChanged(this, arule);
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
		listener->ActiveAttachmentChanged(this, attachments.GetActive());
	});
}

void aeAnimator::NotifyAttachmentChanged(aeAttachment *each){
	DEASSERT_NOTNULL(each)
	pNotifiers.Visit([&](aeAnimatorNotifier *listener){
		listener->AttachmentChanged(this, each);
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
	pDDSBones.RemoveAll();
	
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
	deRig::Ref displayRig, engineRig;
	
	try{
		if(!displayModelPath->IsEmpty()){
			displayModel = engine->GetModelManager()->LoadModel(displayModelPath, GetDirectoryPath());
		}
		if(!displaySkinPath->IsEmpty()){
			displaySkin = engine->GetSkinManager()->LoadSkin(displaySkinPath, GetDirectoryPath());
		}
		if(!displayRigPath->IsEmpty()){
			displayRig = engine->GetRigManager()->LoadRig(displayRigPath, GetDirectoryPath());
		}
		if(!rigPath->IsEmpty()){
			engineRig = engine->GetRigManager()->LoadRig(rigPath, GetDirectoryPath());
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	const auto oldModel = pEngComponent ? pEngComponent->GetModel().Pointer() : nullptr;
	const auto oldRig = pEngComponent ? pEngComponent->GetRig().Pointer() : nullptr;
	const auto oldSkin = pEngComponent ? pEngComponent->GetSkin().Pointer() : nullptr;
	
	const bool sameModel = displayModel == oldModel;
	const bool sameSkin = displaySkin == oldSkin;
	const bool sameRig = displayRig == oldRig;
	const bool sameEngineRig = engineRig == pEngRig;
	
	if(sameModel && sameSkin && sameRig && sameEngineRig){
		return;
	}
	
	// detach all colliders
	DetachAttachments();
	
	// disable collider
	pEngCollider->SetComponent(nullptr);
	pEngCollider->SetEnabled(false);
	
	pEngRig = engineRig;
	
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
			pEngComponent->SetEnableGI(false);
			pEngWorld->AddComponent(pEngComponent);
			
			pEngCollider->AddAttachment(deColliderAttachment::Ref::New(pEngComponent));
		}
		
	}else if(pEngComponent){
		deColliderAttachment * const colAtt = pEngCollider->GetAttachmentWith(pEngComponent);
		if(colAtt){
			pEngCollider->RemoveAttachment(colAtt);
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
	
	if(!sameModel){
		pUpdateHiddenVertexPositionSetNames();
	}
	if(!sameEngineRig){
		pUpdateHiddenBoneNames();
	}
}

void aeAnimator::pUpdateAnimator(){
	deAnimation::Ref animation;
	
	try{
		if(!animationPath->IsEmpty()){
			animation = GetEngine()->GetAnimationManager()->
				LoadAnimation(animationPath, GetDirectoryPath());
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	if(animation == pEngAnimator->GetAnimation()){
		return;
	}
	
	pEngAnimator->SetAnimation(animation);
	
	pSubAnimator->SetComponentAndAnimation(pEngComponent, animation);
// 	pTestingSubAnimator->SetComponentAndAnimation( pEngComponent, animation );
	pTestingSubAnimator->SetComponent(pEngComponent);
	
	pAnimCompChanged();
	pUpdateHiddenMoveNames();
}

void aeAnimator::pUpdateLinks(){
	links.GetValue().Visit([](aeLink &each){
		each.UpdateController();
	});
	RebuildRules();
}

void aeAnimator::pUpdateRuleIndices(){
	rules.GetValue().VisitIndexed([](int i, aeRule &each){
		each.SetIndex(i);
	});
}

void aeAnimator::pAnimCompChanged(){
	rules.GetValue().Visit([](aeRule &each){
		each.UpdateCompAnim();
	});
}

void aeAnimator::pUpdateEngineControllers(){
	// set all engine controller indices in our controllers to -1
	controllers->Visit([&](aeController &each){
		each.SetIndex(-1);
	});
	
	// remove the animator from the animator instance
	pEngAnimatorInstance->SetAnimator(nullptr);
	
	// remove all controllers
	pEngAnimator->RemoveAllControllers();
	
	// add an engine controller for each controller we have
	controllers->Visit([&](aeController &each){
		pEngAnimator->AddController(deAnimatorController::Ref::New());
	});
	
	// assign the animator to the animator instance. this creates the controllers
	// inside the animator instance
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
	
	// now assign the matching engine controller indices to our controllers
	controllers->VisitIndexed([&](int i, aeController &each){
		each.SetIndex(i);
	});
	
	// links have to be updated now
	pUpdateLinks();
}

void aeAnimator::pUpdateDDSBones(){
	decDMatrix matrix;
	int boneCount = 0;
	
	if(pEngComponent){
		boneCount = pEngComponent->GetBones().GetCount();
		pEngComponent->PrepareBones();
		matrix = decDMatrix(pEngComponent->GetMatrix()).Normalized();
	}
	
	if(boneCount != pDDSBones.GetCount()){
		pDDSBones.RemoveAll();
		int i;
		for(i=0; i<boneCount; i++){
			auto shape = deTUniqueReference<igdeWCoordSysArrows>::New();
			shape->SetParentDebugDrawer(pDDBones);
			pDDSBones.Add(std::move(shape));
		}
	}
	
	pDDBones->SetPosition(matrix.GetPosition());
	pDDBones->SetOrientation(matrix.ToQuaternion());
	
	pDDSBones.VisitIndexed([&](int i, igdeWCoordSysArrows &d){
		if(i < boneCount && pEngComponent){
			const decMatrix boneMatrix = pEngComponent->GetBoneAt(i).GetMatrix().Normalized();
			d.SetPosition(boneMatrix.GetPosition());
			d.SetOrientation(boneMatrix.ToQuaternion());
			d.SetScale(decVector(pDDSBoneSize, pDDSBoneSize, pDDSBoneSize));
			d.SetVisible(true);
			
		}else{
			d.SetVisible(false);
		}
	});
}


void aeAnimator::pUpdateHiddenBoneNames(){
	decStringSet names;
	if(pEngComponent && pEngComponent->GetRig()){
		pEngComponent->GetRig()->GetBones().Visit([&](const deRigBone &bone){
			names.Add(bone.GetName());
		});
	}
	hiddenBoneNames = names;
}

void aeAnimator::pUpdateHiddenVertexPositionSetNames(){
	decStringSet names;
	if(pEngComponent && pEngComponent->GetModel()){
		pEngComponent->GetModel()->GetVertexPositionSets().Visit([&](const deModelVertexPositionSet &vps){
			names.Add(vps.GetName());
		});
	}
	hiddenVPSNames = names;
}

void aeAnimator::pUpdateHiddenMoveNames(){
	decStringSet names;
	if(pEngAnimator && pEngAnimator->GetAnimation()){
		pEngAnimator->GetAnimation()->GetMoves().Visit([&](const deAnimationMove &move){
			names.Add(move.GetName());
		});
	}
	hiddenMoveNames = names;
}
