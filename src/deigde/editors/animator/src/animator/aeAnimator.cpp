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
pMPHiddenBoneNames(pWindowMain.GetMCAnimatorProperties().hiddenBoneNames, pMetaContext),
pMPHiddenVpsNames(pWindowMain.GetMCAnimatorProperties().hiddenVPSNames, pMetaContext),
pMPHiddenMoveNames(pWindowMain.GetMCAnimatorProperties().hiddenMoveNames, pMetaContext),
pMPRigPath(pWindowMain.GetMCAnimatorProperties().rig, pMetaContext),
pMPAnimationPath(pWindowMain.GetMCAnimatorProperties().animation, pMetaContext),
pMPAffectedBones(pWindowMain.GetMCAnimatorProperties().affectedBones, pMetaContext),
pMPAffectedVps(pWindowMain.GetMCAnimatorProperties().affectedVertexPositionSets, pMetaContext),
pMPControllers(pWindowMain.GetMCAnimatorProperties().controller.controllers, pMetaContextController),
pMPController(pWindowMain.GetMCAnimatorProperties().controller.controller, pMetaContextController),
pMPLinks(pWindowMain.GetMCAnimatorProperties().link.links, pMetaContextLink),
pMPLink(pWindowMain.GetMCAnimatorProperties().link.link, pMetaContextLink),
pMPRuleTree(pWindowMain.GetMCAnimatorProperties().rule.ruleTree, pMetaContextRule),
pMPRules(pWindowMain.GetMCAnimatorProperties().rule.rules, pMetaContextRule),
pMPRule(pWindowMain.GetMCAnimatorProperties().rule.rule, pMetaContextRule),
pMPDisplayModelPath(pWindowMain.GetMCAnimatorProperties().displayModelPath, pMetaContextView),
pMPDisplaySkinPath(pWindowMain.GetMCAnimatorProperties().displaySkinPath, pMetaContextView),
pMPDisplayRigPath(pWindowMain.GetMCAnimatorProperties().displayRigPath, pMetaContextView),
pMPBaseAnimatorPath(pWindowMain.GetMCAnimatorProperties().baseAnimatorPath, pMetaContextView),
pMPResetState(pWindowMain.GetMCAnimatorProperties().resetState, pMetaContextView),
pMPPlaySpeed(pWindowMain.GetMCAnimatorProperties().playSpeed, pMetaContextView),
pMPTimeStep(pWindowMain.GetMCAnimatorProperties().timeStep, pMetaContextView),
pMPPaused(pWindowMain.GetMCAnimatorProperties().paused, pMetaContextView),
pMPSky(pWindowMain.GetMCAnimatorProperties().sky, pMetaContextView),
pMPEnvironmentObject(pWindowMain.GetMCAnimatorProperties().environmentObject, pMetaContextView),
pMPCamera(pWindowMain.GetMCAnimatorProperties().camera, pMetaContextView),
pMPAttachments(pWindowMain.GetMCAnimatorProperties().attachment.attachments, pMetaContextView),
pMPAttachment(pWindowMain.GetMCAnimatorProperties().attachment.attachment, pMetaContextView)
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
	
	pDDBones = nullptr;
	pDDSBoneSize = 1.0f;
	
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
		pSky = igdeWSky::Ref::New(windowMain.GetEnvironment());
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pEngWorld);
		pMPSky.SetValue(pSky->GetMetaContext(), false);
		
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
		pMPEnvironmentObject.SetValue(pEnvObject->GetMetaContext(), false);
		
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
		return !pMPControllers->HasNamed(name);
	});
	
	uniqueNameLink.SetIsUnique([this](const decString &name){
		return !pMPLinks->HasMatching([&](const aeLink &each){
			return each.GetName() == name;
		});
	});
	
	uniqueNameRule.SetIsUnique([this](const decString &name){
		return !pMPRules->HasMatching([&](const aeRule &each){
			return each.GetName() == name;
		});
	});
	
	uniqueNameAttachment.SetIsUnique([this](const decString &name){
		return !pMPAttachments->HasNamed(name);
	});
	
	pMPRigPath.onValueChanged = [this](){
		pUpdateComponent();
		NotifyRigChanged();
	};
	
	pMPAnimationPath.onValueChanged = [this](){
		pUpdateAnimator();
		pCamera->SetBone("");
		NotifyAnimationChanged();
	};
	
	pMPAffectedBones.onValueChanged = [this](){
		if(pEngAnimator){
			pEngAnimator->GetListBones() = pMPAffectedBones;
			pEngAnimator->NotifyBonesChanged();
		}
		NotifyAnimatorChanged();
	};
	
	pMPAffectedVps.onValueChanged = [this](){
		if(pEngAnimator){
			pEngAnimator->GetListVertexPositionSets() = pMPAffectedVps;
			pEngAnimator->NotifyVertexPositionSetsChanged();
		}
		NotifyAnimatorChanged();
	};
	
	pMPControllers.onValueChanged = [this](){
		pMPAllowedListControllers = igdeMetaPropertyObjectType<aeController>::ObjectTypeList::New(pMPControllers);
		pUpdateLinks();
		NotifyControllerStructureChanged();
	};
	pMPControllers.onObjectAdded = [this](aeController &each){
		each.SetAnimator(this);
	};
	pMPControllers.onObjectRemoved = [this](aeController &each){
		each.SetAnimator(nullptr);
	};
	pMPControllers.onActiveChanged = [this](){
		NotifyActiveControllerChanged();
		const auto &active = pMPControllers.GetActive();
		pMPController.SetValue(active ? active->GetMetaContext() : pMPController.Property().GetDefaultValue());
	};
	
	pMPLinks.onValueChanged = [this](){
		RebuildRules();
		NotifyLinkStructureChanged();
	};
	pMPLinks.onObjectAdded = [this](aeLink &each){
		each.SetAnimator(this);
	};
	pMPLinks.onObjectRemoved = [this](aeLink &each){
		each.SetAnimator(nullptr);
	};
	pMPLinks.onActiveChanged = [this](){
		NotifyActiveLinkChanged();
		const auto &active = pMPLinks.GetActive();
		pMPLink.SetValue(active ? active->GetMetaContext() : pMPLink.Property().GetDefaultValue());
	};
	
	pMPRuleTree.onActiveChanged = [this](){
		NotifyActiveRuleChanged();
		const auto &active = pMPRuleTree.GetActive();
		pMPRule.SetValue(active ? active->GetMetaContext() : pMPRule.Property().GetDefaultValue());
	};
	
	pMPRules.onValueChanged = [this](){
		pUpdateRuleIndices();
		RebuildRules();
		NotifyRuleStructureChanged();
	};
	pMPRules.onObjectAdded = [this](aeRule &each){
		each.SetAnimator(this);
	};
	pMPRules.onObjectRemoved = [this](aeRule &each){
		each.SetAnimator(nullptr);
	};
	
	pMPAttachments.onValueChanged = [this](){
		NotifyAttachmentStructureChanged();
	};
	pMPAttachments.onObjectAdded = [this](aeAttachment &each){
		each.SetAnimator(this);
	};
	pMPAttachments.onObjectRemoved = [this](aeAttachment &each){
		each.SetAnimator(nullptr);
	};
	pMPAttachments.onActiveChanged = [this](){
		NotifyActiveAttachmentChanged();
		const auto &active = pMPAttachments.GetActive();
		pMPAttachment.SetValue(active ? active->GetMetaContext() : pMPAttachment.Property().GetDefaultValue());
	};
	
	pMPDisplayModelPath.onValueChanged = [this](){
		pUpdateComponent();
		NotifyModelChanged();
	};
	pMPDisplaySkinPath.onValueChanged = pMPDisplayModelPath.onValueChanged;
	pMPDisplayRigPath.onValueChanged = pMPDisplayModelPath.onValueChanged;
	
	pMPBaseAnimatorPath.onValueChanged = [this](){
		pTestingSubAnimator->SetPathAnimator(pMPBaseAnimatorPath);
		pTestingSubAnimator->LoadAnimator(pWindowMain.GetLoadSaveSystem());
		NotifyViewChanged();
	};
	
	pMPResetState.onValueChanged = [this](){
		RebuildRules();
		NotifyViewChanged();
	};
	
	pMPSky.onValueChanged = [this](){
		NotifySkyChanged();
	};
	pMPEnvironmentObject.onValueChanged = [this](){
		NotifyEnvObjectChanged();
	};
	pMPCamera.onValueChanged = [this](){
		NotifyCameraChanged();
	};
	
	pMPPlaySpeed.onValueChanged = [this](){
		NotifyPlaybackChanged();
	};
	pMPTimeStep.onValueChanged = pMPPlaySpeed.onValueChanged;
	
	pMPPaused.onValueChanged = [this](){
		NotifyPlaybackChanged();
	};
	
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
	pMPDisplayModelPath = path;
}

void aeAnimator::SetDisplaySkinPath(const char *path){
	pMPDisplaySkinPath = path;
}

void aeAnimator::SetDisplayRigPath(const char *path){
	pMPDisplayRigPath = path;
}

void aeAnimator::SetRigPath(const char *path){
	pMPRigPath = path;
}

void aeAnimator::SetAnimationPath(const char *path){
	pMPAnimationPath = path;
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
	const float controllerElapsed = elapsed * pMPPlaySpeed;
	const float realTimeElapsed = elapsed * pMPPlaySpeed;
	
	// update the locomotion testing
	pLocomotion->Update(realTimeElapsed);
	
	// update the controllers which are linked to the elapsed time
	if(!pMPPaused){
		pMPControllers->Visit([&](aeController &each){
			each.UpdateValue(controllerElapsed);
		});
	}
	
	// reset the animation states if required
	if(pEngComponent && pMPResetState){
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
	if(!pMPPaused){
		pMPAttachments->Visit([&](aeAttachment &each){
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

void aeAnimator::SetPaused(bool value){
	pMPPaused = value;
}

void aeAnimator::SetPlaySpeed(float value){
	pMPPlaySpeed = value;
}

void aeAnimator::SetTimeStep(float value){
	pMPTimeStep = value;
}



void aeAnimator::SetResetState(bool value){
	pMPResetState = value;
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
	auto list = pMPControllers.GetValue();
	list.AddOrThrow(acontroller);
	pMPControllers = list;
}

void aeAnimator::InsertControllerAt(aeController *acontroller, int index){
	auto list = pMPControllers.GetValue();
	list.InsertOrThrow(acontroller, index);
	pMPControllers = list;
}

void aeAnimator::MoveControllerTo(aeController *acontroller, int index){
	auto list = pMPControllers.GetValue();
	list.Move(acontroller, index);
	pMPControllers = list;
}

void aeAnimator::RemoveController(aeController *acontroller){
	auto list = pMPControllers.GetValue();
	list.RemoveOrThrow(acontroller);
	pMPControllers = list;
}

void aeAnimator::RemoveAllControllers(){
	pMPControllers = {};
}

void aeAnimator::SetActiveController(aeController *acontroller){
	pMPControllers.SetActive(acontroller);
}

void aeAnimator::ResetControllers(){
	pMPControllers->Visit([](aeController &each){
		each.ResetValue();
	});
}
void aeAnimator::ResetControllersWith(int locomotionAttribute){
	pMPControllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.ResetValue();
		}
	});
}
	
void aeAnimator::InverseControllersWith(int locomotionAttribute){
	pMPControllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.InverseValue();
		}
	});
}

void aeAnimator::IncrementControllersWith(int locomotionAttribute, float incrementBy){
	pMPControllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.IncrementCurrentValue(incrementBy);
		}
	});
}



// Links
//////////

void aeAnimator::AddLink(aeLink *alink){
	auto list = pMPLinks.GetValue();
	list.AddOrThrow(alink);
	pMPLinks = list;
}

void aeAnimator::RemoveLink(aeLink *alink){
	auto list = pMPLinks.GetValue();
	list.RemoveOrThrow(alink);
	pMPLinks = list;
}

void aeAnimator::RemoveAllLinks(){
	pMPLinks = {};
}

void aeAnimator::SetActiveLink(aeLink *alink){
	pMPLinks.SetActive(alink);
}

int aeAnimator::CountLinkUsage(aeLink *alink) const{
	int count = 0;
	pMPRules.GetValue().Visit([&](const aeRule &each){
		count += each.CountLinkUsage(alink);
	});
	return count;
}



// Rules
//////////

void aeAnimator::AddRule(aeRule *arule){
	auto list = pMPRules.GetValue();
	list.AddOrThrow(arule);
	pMPRules = list;
}

void aeAnimator::InsertRuleAt(aeRule *arule, int index){
	auto list = pMPRules.GetValue();
	list.InsertOrThrow(arule, index);
	pMPRules = list;
}

void aeAnimator::MoveRuleTo(aeRule *arule, int index){
	auto list = pMPRules.GetValue();
	list.Move(arule, index);
	pMPRules = list;
}

void aeAnimator::RemoveRule(aeRule *arule){
	auto list = pMPRules.GetValue();
	list.RemoveOrThrow(arule);
	pMPRules = list;
}

void aeAnimator::RemoveAllRules(){
	pMPRules = {};
}

void aeAnimator::SetActiveRule(aeRule *arule){
	pMPRuleTree.SetActive(arule);
}

void aeAnimator::RebuildRules(){
	pMPRules.GetValue().Visit([](aeRule &each){
		each.SetEngineRule(nullptr);
	});
	
	if(!pEngAnimator){
		return;
	}
	
	pEngAnimator->RemoveAllRules();
	
	if(pMPResetState){
		const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
		engRule->SetUseLastState(true);
		pEngAnimator->AddRule(engRule);
	}
	
	pMPRules.GetValue().Visit([&](aeRule &each){
		const deAnimatorRule::Ref engRule(each.CreateEngineRule());
		pEngAnimator->AddRule(engRule);
		each.SetEngineRule(engRule);
	});
}



// Bone Management
////////////////////

void aeAnimator::SetListBones(const decStringSet &bones){
	pMPAffectedBones = bones;
}

void aeAnimator::AddBone(const char *bone){
	pMPAffectedBones = pMPAffectedBones.GetValue() + decStringSet(devctag, bone);
}

void aeAnimator::RemoveBone(const char *bone){
	pMPAffectedBones = pMPAffectedBones.GetValue() - decStringSet(devctag, bone);
}

void aeAnimator::RemoveAllBones(){
	pMPAffectedBones = {};
}


// Vertex position set management
///////////////////////////////////

void aeAnimator::SetListVertexPositionSets(const decStringSet &sets){
	pMPAffectedVps = sets;
}

void aeAnimator::AddVertexPositionSet(const char *vps){
	pMPAffectedVps = pMPAffectedVps.GetValue() + decStringSet(devctag, vps);
}

void aeAnimator::RemoveVertexPositionSet(const char *vps){
	pMPAffectedVps = pMPAffectedVps.GetValue() - decStringSet(devctag, vps);
}

void aeAnimator::RemoveAllVertexPositionSets(){
	pMPAffectedVps = {};
}


// Attachments
////////////////

aeAttachment *aeAnimator::GetAttachmentNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pMPAttachments->FindOrDefault([&](const aeAttachment &a){ return a.GetName() == name; });
}

void aeAnimator::AddAttachment(aeAttachment *each){
	auto set = pMPAttachments.GetValue();
	set.AddOrThrow(each);
	pMPAttachments = set;
}

void aeAnimator::RemoveAttachment(aeAttachment *each){
	auto set = pMPAttachments.GetValue();
	set.RemoveOrThrow(each);
	pMPAttachments = set;
}

void aeAnimator::RemoveAllAttachments(){
	pMPAttachments = {};
}

void aeAnimator::SetActiveAttachment(aeAttachment *each){
	pMPAttachments.SetActive(each);
}

void aeAnimator::AttachAttachments(){
	pMPAttachments->Visit([](aeAttachment &each){
		each.AttachCollider();
	});
}

void aeAnimator::DetachAttachments(){
	pMPAttachments->Visit([](aeAttachment &each){
		each.DetachCollider();
	});
}

void aeAnimator::AttachmentsResetPhysics(){
	pMPAttachments->Visit([](aeAttachment &each){
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
		listener->ActiveControllerChanged(this, pMPControllers.GetActive());
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
		listener->ActiveLinkChanged(this, pMPLinks.GetActive());
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
		listener->ActiveRuleChanged(this, pMPRules.GetActive());
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
		listener->ActiveAttachmentChanged(this, pMPAttachments.GetActive());
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
	pSky.Clear();
	pCamera.Clear();
	
	RemoveAllAttachments();
	RemoveAllRules();
	RemoveAllLinks();
	RemoveAllControllers();
	
	pTestingSubAnimator.Clear();
	pSubAnimator.Clear();
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
	
	// create sub animators
	pSubAnimator = deTUniqueReference<aeSubAnimator>::New(engine);
	pTestingSubAnimator = deTUniqueReference<aeSubAnimator>::New(engine);
}

void aeAnimator::pCreateCamera(){
	pCamera = aeCamera::Ref::New(*this, GetEngine());
	pCamera->SetEngineWorld(pEngWorld);
	pCamera->Reset();
	pMPCamera.SetValue(pCamera->GetMetaContext(), false);
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
		if(!pMPDisplayModelPath->IsEmpty()){
			displayModel = engine->GetModelManager()->LoadModel(pMPDisplayModelPath, GetDirectoryPath());
		}
		if(!pMPDisplaySkinPath->IsEmpty()){
			displaySkin = engine->GetSkinManager()->LoadSkin(pMPDisplaySkinPath, GetDirectoryPath());
		}
		if(!pMPDisplayRigPath->IsEmpty()){
			displayRig = engine->GetRigManager()->LoadRig(pMPDisplayRigPath, GetDirectoryPath());
		}
		if(!pMPRigPath->IsEmpty()){
			engineRig = engine->GetRigManager()->LoadRig(pMPRigPath, GetDirectoryPath());
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
		if(!pMPAnimationPath->IsEmpty()){
			animation = GetEngine()->GetAnimationManager()->
				LoadAnimation(pMPAnimationPath, GetDirectoryPath());
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
	pMPLinks.GetValue().Visit([](aeLink &each){
		each.UpdateController();
	});
	RebuildRules();
}

void aeAnimator::pUpdateRuleIndices(){
	pMPRules.GetValue().VisitIndexed([](int i, aeRule &each){
		each.SetIndex(i);
	});
}

void aeAnimator::pAnimCompChanged(){
	pMPRules.GetValue().Visit([](aeRule &each){
		each.UpdateCompAnim();
	});
}

void aeAnimator::pUpdateEngineControllers(){
	// set all engine controller indices in our controllers to -1
	pMPControllers->Visit([&](aeController &each){
		each.SetIndex(-1);
	});
	
	// remove the animator from the animator instance
	pEngAnimatorInstance->SetAnimator(nullptr);
	
	// remove all controllers
	pEngAnimator->RemoveAllControllers();
	
	// add an engine controller for each controller we have
	pMPControllers->Visit([&](aeController &each){
		pEngAnimator->AddController(deAnimatorController::Ref::New());
	});
	
	// assign the animator to the animator instance. this creates the controllers
	// inside the animator instance
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
	
	// now assign the matching engine controller indices to our controllers
	pMPControllers->VisitIndexed([&](int i, aeController &each){
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
	pMPHiddenBoneNames = names;
}

void aeAnimator::pUpdateHiddenVertexPositionSetNames(){
	decStringSet names;
	if(pEngComponent && pEngComponent->GetModel()){
		pEngComponent->GetModel()->GetVertexPositionSets().Visit([&](const deModelVertexPositionSet &vps){
			names.Add(vps.GetName());
		});
	}
	pMPHiddenVpsNames = names;
}

void aeAnimator::pUpdateHiddenMoveNames(){
	decStringSet names;
	if(pEngAnimator && pEngAnimator->GetAnimation()){
		pEngAnimator->GetAnimation()->GetMoves().Visit([&](const deAnimationMove &move){
			names.Add(move.GetName());
		});
	}
	pMPHiddenMoveNames = names;
}
