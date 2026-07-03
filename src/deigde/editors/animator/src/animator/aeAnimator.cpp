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
#include "locomotion/aeAnimatorLocomotionLeg.h"
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

aeAnimator::MetaContext::Ref aeAnimator::CreateMetaContext(aeWindowMain &windowMain, aeAnimator *animator){
	return MetaContext::Ref::New("animator.animator", "Animator", "Animator properties",
		windowMain.GetMCAnimatorProperties().metaProperties, animator);
}

aeAnimator::MetaContext::Ref aeAnimator::CreateMetaContextController(aeWindowMain &windowMain, aeAnimator *animator){
	return MetaContext::Ref::New("animator.animator_controller", "Controller", "Animator controller properties",
		windowMain.GetMCAnimatorProperties().metaPropertiesController, animator);
}

aeAnimator::MetaContext::Ref aeAnimator::CreateMetaContextLink(aeWindowMain &windowMain, aeAnimator *animator){
	return MetaContext::Ref::New("animator.animator_link", "Link", "Animator link properties",
		windowMain.GetMCAnimatorProperties().metaPropertiesLink, animator);
}

aeAnimator::MetaContext::Ref aeAnimator::CreateMetaContextRule(aeWindowMain &windowMain, aeAnimator *animator){
	return MetaContext::Ref::New("animator.animator_rule", "Rule", "Animator rule properties",
		windowMain.GetMCAnimatorProperties().metaPropertiesRule, animator);
}

aeAnimator::MetaContext::Ref aeAnimator::CreateMetaContextPlayground(aeWindowMain &windowMain, aeAnimator *animator){
	return MetaContext::Ref::New("animator.animator_playground", "Playground", "Animator playground properties",
		windowMain.GetMCAnimatorProperties().metaPropertiesPlayground, animator);
}

aeAnimator::MetaContext::Ref aeAnimator::CreateMetaContextAttachment(aeWindowMain &windowMain, aeAnimator *animator){
	return MetaContext::Ref::New("animator.animator_attachment", "Attachment", "Animator attachment properties",
		windowMain.GetMCAnimatorProperties().attachment.metaProperties, animator);
}

aeAnimator::MetaContext::Ref aeAnimator::CreateMetaContextView(aeWindowMain &windowMain, aeAnimator *animator){
	return MetaContext::Ref::New("animator.animator_view", "View", "Animator view properties",
		windowMain.GetMCAnimatorProperties().metaPropertiesView, animator);
}

// Constructor, destructor
////////////////////////////

aeAnimator::aeAnimator(aeWindowMain &windowMain) :
igdeEditableEntity(windowMain.GetEnvironment()),
pWindowMain(windowMain),
pMetaContext(CreateMetaContext(windowMain, this)),
pMetaContextController(CreateMetaContextController(windowMain, this)),
pMetaContextLink(CreateMetaContextLink(windowMain, this)),
pMetaContextRule(CreateMetaContextRule(windowMain, this)),
pMetaContextPlayground(CreateMetaContextPlayground(windowMain, this)),
pMetaContextAttachment(CreateMetaContextAttachment(windowMain, this)),
pMetaContextView(CreateMetaContextView(windowMain, this)),
mpHiddenBoneNames(pWindowMain.GetMCAnimatorProperties().hiddenBoneNames, pMetaContext),
mpHiddenVpsNames(pWindowMain.GetMCAnimatorProperties().hiddenVPSNames, pMetaContext),
mpHiddenMoveNames(pWindowMain.GetMCAnimatorProperties().hiddenMoveNames, pMetaContext),
mpRigPath(pWindowMain.GetMCAnimatorProperties().rig, pMetaContext),
mpAnimationPath(pWindowMain.GetMCAnimatorProperties().animation, pMetaContext),
mpAffectedBones(pWindowMain.GetMCAnimatorProperties().affectedBones, pMetaContext),
mpAffectedVps(pWindowMain.GetMCAnimatorProperties().affectedVertexPositionSets, pMetaContext),
mpControllers(pWindowMain.GetMCAnimatorProperties().controller.controllers, pMetaContextController),
mpController(pWindowMain.GetMCAnimatorProperties().controller.controller, pMetaContextController),
mpLinks(pWindowMain.GetMCAnimatorProperties().link.links, pMetaContextLink),
mpLink(pWindowMain.GetMCAnimatorProperties().link.link, pMetaContextLink),
mpRuleTree(pWindowMain.GetMCAnimatorProperties().rule.ruleTree, pMetaContextRule),
mpRules(pWindowMain.GetMCAnimatorProperties().rule.rules, pMetaContextRule),
mpRule(pWindowMain.GetMCAnimatorProperties().rule.rule, pMetaContextRule),
mpPlaygroundControllers(pWindowMain.GetMCAnimatorProperties().playgroundControllers, pMetaContextPlayground),
mpDisplayModelPath(pWindowMain.GetMCAnimatorProperties().displayModelPath, pMetaContextView),
mpDisplaySkinPath(pWindowMain.GetMCAnimatorProperties().displaySkinPath, pMetaContextView),
mpDisplayRigPath(pWindowMain.GetMCAnimatorProperties().displayRigPath, pMetaContextView),
mpBaseAnimatorPath(pWindowMain.GetMCAnimatorProperties().baseAnimatorPath, pMetaContextView),
mpResetState(pWindowMain.GetMCAnimatorProperties().resetState, pMetaContextView),
mpPlaySpeed(pWindowMain.GetMCAnimatorProperties().playSpeed, pMetaContextView),
mpTimeStep(pWindowMain.GetMCAnimatorProperties().timeStep, pMetaContextView),
mpPaused(pWindowMain.GetMCAnimatorProperties().paused, pMetaContextView),
mpSky(pWindowMain.GetMCAnimatorProperties().sky, pMetaContextView),
mpEnvironmentObject(pWindowMain.GetMCAnimatorProperties().environmentObject, pMetaContextView),
mpCamera(pWindowMain.GetMCAnimatorProperties().camera, pMetaContextView),
mpAttachments(pWindowMain.GetMCAnimatorProperties().attachment.attachments, pMetaContextView),
mpAttachment(pWindowMain.GetMCAnimatorProperties().attachment.attachment, pMetaContextView)
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
		mpSky.SetValue(pSky->GetMetaContext(), false);
		
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
		mpEnvironmentObject.SetValue(pEnvObject->GetMetaContext(), false);
		
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
		return !mpControllers->HasNamed(name);
	});
	
	uniqueNameLink.SetIsUnique([this](const decString &name){
		return !mpLinks->HasMatching([&](const aeLink &each){
			return each.GetName() == name;
		});
	});
	
	uniqueNameRule.SetIsUnique([this](const decString &name){
		return !mpRules->HasMatching([&](const aeRule &each){
			return each.GetName() == name;
		});
	});
	
	uniqueNameAttachment.SetIsUnique([this](const decString &name){
		return !mpAttachments->HasNamed(name);
	});
	
	mpRigPath.onValueChanged = [this](){
		pUpdateComponent();
		NotifyRigChanged();
	};
	
	mpAnimationPath.onValueChanged = [this](){
		pUpdateAnimator();
		pCamera->SetBone("");
		NotifyAnimationChanged();
	};
	
	mpAffectedBones.onValueChanged = [this](){
		if(pEngAnimator){
			pEngAnimator->GetListBones() = mpAffectedBones;
			pEngAnimator->NotifyBonesChanged();
		}
		NotifyAnimatorChanged();
	};
	
	mpAffectedVps.onValueChanged = [this](){
		if(pEngAnimator){
			pEngAnimator->GetListVertexPositionSets() = mpAffectedVps;
			pEngAnimator->NotifyVertexPositionSetsChanged();
		}
		NotifyAnimatorChanged();
	};
	
	mpControllers.onValueChanged = [this](){
		mpAllowedListControllers = igdeMetaPropertyObjectType<aeController>::ObjectTypeList::New(mpControllers);
		pUpdateLinks();
		NotifyControllerStructureChanged();
		pUpdatePlaygroundControllers();
	};
	mpControllers.onObjectAdded = [this](aeController &each){
		each.SetAnimator(this);
	};
	mpControllers.onObjectRemoved = [this](aeController &each){
		each.SetAnimator(nullptr);
	};
	mpControllers.onActiveChanged = [this](){
		NotifyActiveControllerChanged();
		const auto &active = mpControllers.GetActive();
		mpController.SetValue(active ? active->GetMetaContext() : mpController.Property().GetDefaultValue());
	};
	
	mpLinks.onValueChanged = [this](){
		RebuildRules();
		NotifyLinkStructureChanged();
	};
	mpLinks.onObjectAdded = [this](aeLink &each){
		each.SetAnimator(this);
	};
	mpLinks.onObjectRemoved = [this](aeLink &each){
		each.SetAnimator(nullptr);
	};
	mpLinks.onActiveChanged = [this](){
		NotifyActiveLinkChanged();
		const auto &active = mpLinks.GetActive();
		mpLink.SetValue(active ? active->GetMetaContext() : mpLink.Property().GetDefaultValue());
	};
	
	mpRuleTree.onActiveChanged = [this](){
		NotifyActiveRuleChanged();
		const auto &active = mpRuleTree.GetActive();
		mpRule.SetValue(active ? active->GetMetaContext() : mpRule.Property().GetDefaultValue());
	};
	
	mpRules.onValueChanged = [this](){
		pUpdateRuleIndices();
		RebuildRules();
		NotifyRuleStructureChanged();
	};
	mpRules.onObjectAdded = [this](aeRule &each){
		each.SetAnimator(this);
	};
	mpRules.onObjectRemoved = [this](aeRule &each){
		each.SetAnimator(nullptr);
	};
	
	mpAttachments.onValueChanged = [this](){
		NotifyAttachmentStructureChanged();
	};
	mpAttachments.onObjectAdded = [this](aeAttachment &each){
		each.SetAnimator(this);
	};
	mpAttachments.onObjectRemoved = [this](aeAttachment &each){
		each.SetAnimator(nullptr);
	};
	mpAttachments.onActiveChanged = [this](){
		NotifyActiveAttachmentChanged();
		const auto &active = mpAttachments.GetActive();
		mpAttachment.SetValue(active ? active->GetMetaContext() : mpAttachment.Property().GetDefaultValue());
	};
	
	mpDisplayModelPath.onValueChanged = [this](){
		pUpdateComponent();
		NotifyModelChanged();
	};
	mpDisplaySkinPath.onValueChanged = mpDisplayModelPath.onValueChanged;
	mpDisplayRigPath.onValueChanged = mpDisplayModelPath.onValueChanged;
	
	mpBaseAnimatorPath.onValueChanged = [this](){
		pTestingSubAnimator->SetPathAnimator(mpBaseAnimatorPath);
		pTestingSubAnimator->LoadAnimator(pWindowMain.GetLoadSaveSystem());
		NotifyViewChanged();
	};
	
	mpResetState.onValueChanged = [this](){
		RebuildRules();
		NotifyViewChanged();
	};
	
	mpSky.onValueChanged = [this](){
		NotifySkyChanged();
	};
	mpEnvironmentObject.onValueChanged = [this](){
		NotifyEnvObjectChanged();
	};
	mpCamera.onValueChanged = [this](){
		NotifyCameraChanged();
	};
	
	mpPlaySpeed.onValueChanged = [this](){
		NotifyPlaybackChanged();
	};
	mpTimeStep.onValueChanged = mpPlaySpeed.onValueChanged;
	
	mpPaused.onValueChanged = [this](){
		NotifyPlaybackChanged();
	};
	
	GetUndoSystem()->SetMetaProperty(pMetaContext, pWindowMain.GetMCAnimatorProperties().undoHistory);
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
	if(pMetaContextView){
		pMetaContextView->Dispose();
		pMetaContextView.Clear();
	}
	if(pMetaContextAttachment){
		pMetaContextAttachment->Dispose();
		pMetaContextAttachment.Clear();
	}
	if(pMetaContextPlayground){
		pMetaContextPlayground->Dispose();
		pMetaContextPlayground.Clear();
	}
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
	mpDisplayModelPath = path;
}

void aeAnimator::SetDisplaySkinPath(const char *path){
	mpDisplaySkinPath = path;
}

void aeAnimator::SetDisplayRigPath(const char *path){
	mpDisplayRigPath = path;
}

void aeAnimator::SetRigPath(const char *path){
	mpRigPath = path;
}

void aeAnimator::SetAnimationPath(const char *path){
	mpAnimationPath = path;
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
	const float controllerElapsed = elapsed * mpPlaySpeed;
	const float realTimeElapsed = elapsed * mpPlaySpeed;
	
	// update the locomotion testing
	pLocomotion->Update(realTimeElapsed);
	
	// update the controllers which are linked to the elapsed time
	if(!mpPaused){
		mpControllers->Visit([&](aeController &each){
			each.UpdateValue(controllerElapsed);
		});
	}
	
	// reset the animation states if required
	if(pEngComponent && mpResetState){
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
	if(!mpPaused){
		mpAttachments->Visit([&](aeAttachment &each){
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
	mpPaused = value;
}

void aeAnimator::SetPlaySpeed(float value){
	mpPlaySpeed = value;
}

void aeAnimator::SetTimeStep(float value){
	mpTimeStep = value;
}



void aeAnimator::SetResetState(bool value){
	mpResetState = value;
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
	auto list = mpControllers.GetValue();
	list.AddOrThrow(acontroller);
	mpControllers = list;
}

void aeAnimator::InsertControllerAt(aeController *acontroller, int index){
	auto list = mpControllers.GetValue();
	list.InsertOrThrow(acontroller, index);
	mpControllers = list;
}

void aeAnimator::MoveControllerTo(aeController *acontroller, int index){
	auto list = mpControllers.GetValue();
	list.Move(acontroller, index);
	mpControllers = list;
}

void aeAnimator::RemoveController(aeController *acontroller){
	auto list = mpControllers.GetValue();
	list.RemoveOrThrow(acontroller);
	mpControllers = list;
}

void aeAnimator::RemoveAllControllers(){
	mpControllers = {};
}

void aeAnimator::SetActiveController(aeController *acontroller){
	mpControllers.SetActive(acontroller);
}

void aeAnimator::ResetControllers(){
	mpControllers->Visit([](aeController &each){
		each.ResetValue();
	});
}
void aeAnimator::ResetControllersWith(int locomotionAttribute){
	mpControllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.ResetValue();
		}
	});
}
	
void aeAnimator::InverseControllersWith(int locomotionAttribute){
	mpControllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.InverseValue();
		}
	});
}

void aeAnimator::IncrementControllersWith(int locomotionAttribute, float incrementBy){
	mpControllers->Visit([&](aeController &each){
		if(each.GetLocomotionAttribute() == locomotionAttribute){
			each.IncrementCurrentValue(incrementBy);
		}
	});
}



// Links
//////////

void aeAnimator::AddLink(aeLink *alink){
	auto list = mpLinks.GetValue();
	list.AddOrThrow(alink);
	mpLinks = list;
}

void aeAnimator::RemoveLink(aeLink *alink){
	auto list = mpLinks.GetValue();
	list.RemoveOrThrow(alink);
	mpLinks = list;
}

void aeAnimator::RemoveAllLinks(){
	mpLinks = {};
}

void aeAnimator::SetActiveLink(aeLink *alink){
	mpLinks.SetActive(alink);
}

int aeAnimator::CountLinkUsage(aeLink *alink) const{
	int count = 0;
	mpRules->Visit([&](const aeRule &each){
		count += each.CountLinkUsage(alink);
	});
	return count;
}



// Rules
//////////

void aeAnimator::AddRule(aeRule *arule){
	auto list = mpRules.GetValue();
	list.AddOrThrow(arule);
	mpRules = list;
}

void aeAnimator::InsertRuleAt(aeRule *arule, int index){
	auto list = mpRules.GetValue();
	list.InsertOrThrow(arule, index);
	mpRules = list;
}

void aeAnimator::MoveRuleTo(aeRule *arule, int index){
	auto list = mpRules.GetValue();
	list.Move(arule, index);
	mpRules = list;
}

void aeAnimator::RemoveRule(aeRule *arule){
	auto list = mpRules.GetValue();
	list.RemoveOrThrow(arule);
	mpRules = list;
}

void aeAnimator::RemoveAllRules(){
	mpRules = {};
}

void aeAnimator::SetActiveRule(aeRule *arule){
	mpRuleTree.SetActive(arule);
}

void aeAnimator::RebuildRules(){
	mpRules.GetValue().Visit([](aeRule &each){
		each.SetEngineRule(nullptr);
	});
	
	if(!pEngAnimator){
		return;
	}
	
	pEngAnimator->RemoveAllRules();
	
	if(mpResetState){
		const deAnimatorRuleStateSnapshot::Ref engRule(deAnimatorRuleStateSnapshot::Ref::New());
		engRule->SetUseLastState(true);
		pEngAnimator->AddRule(engRule);
	}
	
	mpRules.GetValue().Visit([&](aeRule &each){
		const deAnimatorRule::Ref engRule(each.CreateEngineRule());
		pEngAnimator->AddRule(engRule);
		each.SetEngineRule(engRule);
	});
}



// Bone Management
////////////////////

void aeAnimator::SetListBones(const decStringSet &bones){
	mpAffectedBones = bones;
}

void aeAnimator::AddBone(const char *bone){
	mpAffectedBones = mpAffectedBones.GetValue() + decStringSet(devctag, bone);
}

void aeAnimator::RemoveBone(const char *bone){
	mpAffectedBones = mpAffectedBones.GetValue() - decStringSet(devctag, bone);
}

void aeAnimator::RemoveAllBones(){
	mpAffectedBones = {};
}


// Vertex position set management
///////////////////////////////////

void aeAnimator::SetListVertexPositionSets(const decStringSet &sets){
	mpAffectedVps = sets;
}

void aeAnimator::AddVertexPositionSet(const char *vps){
	mpAffectedVps = mpAffectedVps.GetValue() + decStringSet(devctag, vps);
}

void aeAnimator::RemoveVertexPositionSet(const char *vps){
	mpAffectedVps = mpAffectedVps.GetValue() - decStringSet(devctag, vps);
}

void aeAnimator::RemoveAllVertexPositionSets(){
	mpAffectedVps = {};
}


// Attachments
////////////////

aeAttachment *aeAnimator::GetAttachmentNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return mpAttachments->FindOrDefault([&](const aeAttachment &a){ return a.GetName() == name; });
}

void aeAnimator::AddAttachment(aeAttachment *each){
	auto set = mpAttachments.GetValue();
	set.AddOrThrow(each);
	mpAttachments = set;
}

void aeAnimator::RemoveAttachment(aeAttachment *each){
	auto set = mpAttachments.GetValue();
	set.RemoveOrThrow(each);
	mpAttachments = set;
}

void aeAnimator::RemoveAllAttachments(){
	mpAttachments = {};
}

void aeAnimator::SetActiveAttachment(aeAttachment *each){
	mpAttachments.SetActive(each);
}

void aeAnimator::AttachAttachments(){
	mpAttachments->Visit([](aeAttachment &each){
		each.AttachCollider();
	});
}

void aeAnimator::DetachAttachments(){
	mpAttachments->Visit([](aeAttachment &each){
		each.DetachCollider();
	});
}

void aeAnimator::AttachmentsResetPhysics(){
	mpAttachments->Visit([](aeAttachment &each){
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
		listener->ActiveControllerChanged(this, mpControllers.GetActive());
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
		listener->ActiveLinkChanged(this, mpLinks.GetActive());
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
		listener->ActiveRuleChanged(this, mpRules.GetActive());
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
		listener->ActiveAttachmentChanged(this, mpAttachments.GetActive());
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
	mpCamera.SetValue(pCamera->GetMetaContext(), false);
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
		if(!mpDisplayModelPath->IsEmpty()){
			displayModel = engine->GetModelManager()->LoadModel(mpDisplayModelPath, GetDirectoryPath());
		}
		if(!mpDisplaySkinPath->IsEmpty()){
			displaySkin = engine->GetSkinManager()->LoadSkin(mpDisplaySkinPath, GetDirectoryPath());
		}
		if(!mpDisplayRigPath->IsEmpty()){
			displayRig = engine->GetRigManager()->LoadRig(mpDisplayRigPath, GetDirectoryPath());
		}
		if(!mpRigPath->IsEmpty()){
			engineRig = engine->GetRigManager()->LoadRig(mpRigPath, GetDirectoryPath());
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
		if(!mpAnimationPath->IsEmpty()){
			animation = GetEngine()->GetAnimationManager()->
				LoadAnimation(mpAnimationPath, GetDirectoryPath());
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
	mpLinks.GetValue().Visit([](aeLink &each){
		each.UpdateController();
	});
	RebuildRules();
}

void aeAnimator::pUpdateRuleIndices(){
	mpRules.GetValue().VisitIndexed([](int i, aeRule &each){
		each.SetIndex(i);
	});
}

void aeAnimator::pUpdatePlaygroundControllers(){
	aeMCPAnimatorPlaygroundControllers::ListType sliders;
	mpControllers->Visit([&](const aeController &controller){
		sliders.Add(controller.GetMetaContext());
	});
	mpPlaygroundControllers = sliders;
}


void aeAnimator::pAnimCompChanged(){
	mpRules.GetValue().Visit([](aeRule &each){
		each.UpdateCompAnim();
	});
}

void aeAnimator::pUpdateEngineControllers(){
	// set all engine controller indices in our controllers to -1
	mpControllers->Visit([&](aeController &each){
		each.SetIndex(-1);
	});
	
	// remove the animator from the animator instance
	pEngAnimatorInstance->SetAnimator(nullptr);
	
	// remove all controllers
	pEngAnimator->RemoveAllControllers();
	
	// add an engine controller for each controller we have
	mpControllers->Visit([&](aeController &each){
		pEngAnimator->AddController(deAnimatorController::Ref::New());
	});
	
	// assign the animator to the animator instance. this creates the controllers
	// inside the animator instance
	pEngAnimatorInstance->SetAnimator(pEngAnimator);
	
	// now assign the matching engine controller indices to our controllers
	mpControllers->VisitIndexed([&](int i, aeController &each){
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
	mpHiddenBoneNames = names;
}

void aeAnimator::pUpdateHiddenVertexPositionSetNames(){
	decStringSet names;
	if(pEngComponent && pEngComponent->GetModel()){
		pEngComponent->GetModel()->GetVertexPositionSets().Visit([&](const deModelVertexPositionSet &vps){
			names.Add(vps.GetName());
		});
	}
	mpHiddenVpsNames = names;
}

void aeAnimator::pUpdateHiddenMoveNames(){
	decStringSet names;
	if(pEngAnimator && pEngAnimator->GetAnimation()){
		pEngAnimator->GetAnimation()->GetMoves().Visit([&](const deAnimationMove &move){
			names.Add(move.GetName());
		});
	}
	mpHiddenMoveNames = names;
}
