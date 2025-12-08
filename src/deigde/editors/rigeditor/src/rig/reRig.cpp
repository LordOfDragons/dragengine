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

#include "reRig.h"
#include "reCamera.h"
#include "reRigBuilder.h"
#include "reRigNotifier.h"
#include "reRigTexture.h"
#include "bone/reRigBone.h"
#include "bone/reSelectionBones.h"
#include "shape/reRigShape.h"
#include "shape/reSelectionShapes.h"
#include "push/reRigPush.h"
#include "push/reSelectionPushes.h"
#include "constraint/reRigConstraint.h"
#include "constraint/reSelectionConstraints.h"
#include "../rigeditor.h"
#include "../collisions/reCLApplyPush.h"
#include "../collisions/reCLRigSimulation.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
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
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Rig Editor"



// Class reRig
////////////////

// Constructor, destructor
////////////////////////////

reRig::reRig(igdeEnvironment *environment) :
igdeEditableEntity(environment),
pEngAnimatorAnim(NULL),
pEngAnimatorRestPose(NULL),

pComponentTextures(NULL),
pComponentTextureCount(0),

pSky(NULL),
pPoseChanged(true),

pModelCollision(false),

pMoveName("idle"),
pMoveTime(0.0f),
pPlayTime(0.0f),
pUseRestPose(true),
pPlaybackMove(false),

pCamera(NULL),

pBones(NULL),
pBoneCount(0),
pBoneSize(0),

pShapes(NULL),
pShapeCount(0),
pShapeSize(0),

pConstraints(NULL),
pConstraintCount(0),
pConstraintSize(0),

pPushes(NULL),
pPushCount(0),
pPushSize(0),

pNotifiers(NULL),
pNotifierCount(0),
pNotifierSize(0),

pLockAxisX(false),
pLockAxisY(false),
pLockAxisZ(false),
pUseLocal(false),

pShowBones(true),
pShowRigShapes(true),
pShowAllBoneShapes(false),
pShowRigConstraints(true),
pShowAllBoneConstraints(false),
pShowRigPushes(true),
pShowShapeXRay(true),

pElementMode(eemBone),
pWorkMode(ewmSelect),
pSimulationRunning(false),
pCLSimulation(*this),
pGravity(0.0f, -9.81f, 0.0f),
pMass(10.0f),
pDynamic(true),
pSlowmotion(1.0f),

pSelectionBones(NULL),
pSelectionShapes(NULL),
pSelectionPushes(NULL),
pSelectionConstraints(NULL),

pDirtyRig(true)
{
	deEngine * const engine = GetEngine();
	
	deAnimatorController *amController = NULL;
	deAnimatorRuleAnimation::Ref amRuleAnim = NULL;
	deAnimatorRuleStateManipulator::Ref amRuleRestPose = NULL;
	deAnimatorRuleStateSnapshot::Ref amRuleStateSnapshot = NULL;
	deAnimatorLink *engLink = NULL;
	
	try{
		SetFilePath("new.derig");
		
		// create animator
		pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
		
		amController = new deAnimatorController;
		amController->SetClamp(true);
		pEngAnimator->AddController(amController);
		amController = NULL;
		
		engLink = new deAnimatorLink;
		engLink->SetController(0);
		pEngAnimator->AddLink(engLink);
		engLink = NULL;
		
		amRuleStateSnapshot.TakeOver(new deAnimatorRuleStateSnapshot);
		amRuleStateSnapshot->SetUseLastState(true);
		pEngAnimator->AddRule(amRuleStateSnapshot);
		amRuleStateSnapshot = NULL;
		
		amRuleAnim.TakeOver(new deAnimatorRuleAnimation);
		amRuleAnim->GetTargetMoveTime().AddLink(0);
		amRuleAnim->SetEnabled(!pUseRestPose);
		pEngAnimator->AddRule(amRuleAnim);
		pEngAnimatorAnim = amRuleAnim;
		amRuleAnim = NULL;
		
		amRuleRestPose.TakeOver(new deAnimatorRuleStateManipulator);
		amRuleRestPose->SetEnabled(true);
		amRuleRestPose->SetEnablePosition(true);
		amRuleRestPose->SetEnableRotation(true);
		amRuleRestPose->SetEnabled(pUseRestPose);
		pEngAnimator->AddRule(amRuleRestPose);
		pEngAnimatorRestPose = amRuleRestPose;
		amRuleRestPose = NULL;
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator(pEngAnimator);
		
		// create the world and scene
		pCreateWorld();
		
		// create camera
		pCreateCamera();
		
		// create sky
		pSky = new igdeWSky(*environment);
		pSky->SetGDDefaultSky();
		pSky->SetWorld(pEngWorld);
		
		// create the environment wrapper object
		pEnvObject.TakeOver(new igdeWObject(*environment));
		pEnvObject->SetWorld(pEngWorld);
		pEnvObject->SetPosition(decDVector(0.0, 0.0, 0.0));
		
		decLayerMask layerMask;
		layerMask.SetBit(eclmSimulation);
		pEnvObject->SetCollisionFilter(decCollisionFilter(layerMask));
		pEnvObject->SetCollisionFilterFallback(decCollisionFilter(layerMask));
		
		pEnvObject->SetGDClassName("IGDETestTerrain");
		
		// create simulation collider
		pEngSimCollider = engine->GetColliderManager()->CreateColliderComponent();
		pEngSimCollider->SetEnabled(false);
		pEngSimCollider->SetResponseType(deCollider::ertDynamic);
		pEngSimCollider->SetUseLocalGravity(false);//! pDynamic);
		pEngSimCollider->SetMass(pMass);
		
		layerMask.ClearMask();
		layerMask.SetBit(reRig::eclmSimulation);
		
		pEngSimCollider->SetCollisionFilter(decCollisionFilter(layerMask));
		
		pEngWorld->AddCollider(pEngSimCollider);
		
		// create selections
		pSelectionBones = new reSelectionBones(this);
		pSelectionShapes = new reSelectionShapes(this);
		pSelectionPushes = new reSelectionPushes(this);
		pSelectionConstraints = new reSelectionConstraints(this);
		
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

reRig::~reRig(){
	pCleanUp();
}



// Management
///////////////

void reRig::Dispose(){
	pSelectionBones->Reset();
	pSelectionShapes->Reset();
	pSelectionPushes->Reset();
	pSelectionConstraints->Reset();
	
	pCamera->SetPosition(decDVector(0.0f, 1.0f, 0.0f));
	
	RemoveAllPushes();
	RemoveAllConstraints();
	RemoveAllShapes();
	RemoveAllBones();
	
	GetUndoSystem()->RemoveAll();
}

void reRig::Reset(){
	pSelectionBones->Reset();
	pSelectionShapes->Reset();
	pSelectionPushes->Reset();
	pSelectionConstraints->Reset();
	
	GetUndoSystem()->RemoveAll();
}

void reRig::SetModelPath(const char *path){
	if(pModelPath == path){
		return;
	}
	
	pModelPath = path;
	
	pUpdateComponent();
	NotifyResourceChanged();
}

void reRig::SetSkinPath(const char *path){
	if(pSkinPath == path){
		return;
	}
	
	pSkinPath = path;
	
	pUpdateComponent();
	NotifyResourceChanged();
}

void reRig::SetAnimationPath(const char *path){
	if(pAnimationPath == path){
		return;
	}
	
	pAnimationPath = path;
	
	pUpdateAnimator();
	NotifyResourceChanged();
}

void reRig::SetCentralMassPoint(const decVector &cmp){
	if(cmp.IsEqualTo(pCMP)){
		return;
	}
	
	pCMP = cmp;
	
	Invalidate();
	NotifyRigChanged();
}

void reRig::SetModelCollision(bool modelCollision){
	if(modelCollision == pModelCollision){
		return;
	}
	
	pModelCollision = modelCollision;
	
	Invalidate();
	NotifyRigChanged();
}

void reRig::SetMoveName(const char *moveName){
	if(pMoveName == moveName){
		return;
	}
	
	pMoveName = moveName;
	
	pUpdateAnimatorMove();
	NotifyViewChanged();
}

void reRig::SetMoveTime(float moveTime){
	if(fabs(moveTime - pMoveTime) > 1e-3f){
		pMoveTime = moveTime;
		
		pUpdateAnimatorTime();
		NotifyViewChanged();
	}
}

void reRig::SetShowBones(bool showBones){
	if(showBones == pShowBones){
		return;
	}
	
	pShowBones = showBones;
	
	pBoneShowStateChanged();
	NotifyViewChanged();
}

void reRig::SetShowRigShapes(bool showShapes){
	if(showShapes != pShowRigShapes){
		pShowRigShapes = showShapes;
		
		pShapeShowStateChanged();
		NotifyViewChanged();
	}
}

void reRig::SetShowAllBoneShapes(bool showShapes){
	if(showShapes != pShowAllBoneShapes){
		pShowAllBoneShapes = showShapes;
		
		pBoneShowStateChanged();
		NotifyViewChanged();
	}
}

void reRig::SetShapeXRay(bool xray){
	if(xray == pShowShapeXRay){
		return;
	}
	
	pShowShapeXRay = xray;
	
	pShapeShowStateChanged();
	pBoneShowStateChanged();
	
	NotifyViewChanged();
}

void reRig::SetShowRigConstraints(bool showConstraints){
	if(showConstraints != pShowRigConstraints){
		pShowRigConstraints = showConstraints;
		
		pConstraintShowStateChanged();
		NotifyViewChanged();
	}
}

void reRig::SetShowAllBoneConstraints(bool showConstraints){
	if(showConstraints != pShowAllBoneConstraints){
		pShowAllBoneConstraints = showConstraints;
		
		pBoneShowStateChanged();
		NotifyViewChanged();
	}
}

void reRig::SetShowRigPushes(bool showPushes){
	if(showPushes != pShowRigPushes){
		pShowRigPushes = showPushes;
		
		pPushShowStateChanged();
		NotifyViewChanged();
	}
}

void reRig::SetUseRestPose(bool useRestPose){
	if(useRestPose == pUseRestPose){
		return;
	}
	
	pUseRestPose = useRestPose;
	
	pEngAnimatorAnim->SetEnabled(!useRestPose);
	pEngAnimatorRestPose->SetEnabled(useRestPose);
	pEngAnimator->NotifyRulesChanged();
	
	NotifyViewChanged();
}

void reRig::SetPlaybackMove(bool playbackMove){
	if(playbackMove != pPlaybackMove){
		pPlaybackMove = playbackMove;
		
		pEngAnimatorInstance->GetControllerAt(0).SetClamp(!playbackMove);
		pEngAnimatorInstance->NotifyControllerChangedAt(0);
		
		NotifyViewChanged();
	}
}



// Engine Specific
////////////////////

reRigTexture* reRig::GetComponentTextureAt(int index) const{
	if(index < 0 || index >= pComponentTextureCount) DETHROW(deeInvalidParam);
	
	return pComponentTextures[index];
}

void reRig::UpdateComponentTextures(){
	if(pEngComponent && pComponentTextureCount > 0){
		deSkin *engSkin = pEngComponent->GetSkin();
		deSkin *defaultSkin = NULL;
		int defaultTexture = 0;
		deSkin *useSkin;
		int useTexture;
		int t;
		
		defaultSkin = GetGameDefinition()->GetDefaultSkin();
		defaultTexture = 0;
		
		for(t=0; t<pComponentTextureCount; t++){
			deComponentTexture &componentTexture = pEngComponent->GetTextureAt(t);
			const decString &textureName = pComponentTextures[t]->GetName();
			
			useSkin = NULL;
			useTexture = 0;
			
			if(pComponentTextures[t]->GetEngineSkin()){
				useSkin = pComponentTextures[t]->GetEngineSkin();
				useTexture = 0;
			}
			
			if(!useSkin && (!engSkin || engSkin->IndexOfTextureNamed(textureName.GetString()) == -1)){
				useSkin = defaultSkin;
				useTexture = defaultTexture;
			}
			
			if(useSkin != componentTexture.GetSkin() || useTexture != componentTexture.GetTexture()){
				componentTexture.SetSkin(useSkin);
				componentTexture.SetTexture(useTexture);
				
				pEngComponent->NotifyTextureChanged(t);
			}
		}
	}
}



void reRig::UpdateWorld(float elapsed){
	int b, s, c;
	
	// apply slowmotion
	elapsed *= pSlowmotion;
	
	// update rig
	Rebuild();
	
	// update the animation
	if(pPlaybackMove){
		pEngAnimatorInstance->GetControllerAt(0).IncrementCurrentValue(elapsed);
		pEngAnimatorInstance->NotifyControllerChangedAt(0);
	}
	
	pEngAnimatorInstance->Apply();
	
	pEnvObject->Update(elapsed);
	
	// detect collisions
	pEngWorld->ProcessPhysics(elapsed);
	
	// update bones from pose
	for(b=0; b<pBoneCount; b++){
		pBones[b]->UpdateFromPose();
	}
	if(pSimulationRunning){
		for(c=0; c<pConstraintCount; c++){
			pConstraints[c]->InvalidatePositions();
		}
	}
	
	// update shapes and constraints
	if(pPoseChanged){
		for(s=0; s<pShapeCount; s++){
			pShapes[s]->Update();
		}
		pPoseChanged = false;
	}
	
	for(c=0; c<pConstraintCount; c++){
		pConstraints[c]->Update();
	}
	for(b=0; b<pBoneCount; b++){
		pBones[b]->UpdateShapesAndConstraints();
	}
	
	// update the scene
	pEngWorld->Update(elapsed);
	
	// update the camera
	pCamera->Update();
}

void reRig::Invalidate(){
	pDirtyRig = true;
}

void reRig::Rebuild(){
	if(!pDirtyRig){
		return;
	}
	
	int b;
	
	// free the old rig
	if(pEngRig){
		if(pEngComponent){
			pEngSimCollider->SetComponent(NULL);
			pEngAnimatorInstance->SetComponent(NULL);
			pEngComponent->SetRig(NULL);
		}
		
		pEngAnimator->SetRig(NULL);
		pEngRig = NULL;
		
	// if the rig does not exist we have to init the pose matrices from the
	// rest pose to avoid sick looking rig
	}else{
		UpdateBoneMatrices();
		SetPoseFromRest();
		for(b=0; b<pBoneCount; b++){
			pBones[b]->SetPoseFromRest();
		}
	}
	
	// build new rig. for this we use an empty name which creates an unnamed
	// rig. named rigs can only exist once but unnamed rigs can exist multiple
	// times. in contrary to named rigs the unnamed rigs can not be retrieved
	// using loading but this is okay for what we use them here.
	reRigBuilder builder(this);
	
	pEngRig = GetEngine()->GetRigManager()->CreateRig("", builder);
	
	// add to the component
	pEngAnimator->SetRig(pEngRig);
	
	if(pEngComponent){
		pEngComponent->SetRig(pEngRig);
		pEngAnimatorInstance->SetComponent(pEngComponent);
		pEngSimCollider->SetComponent(pEngComponent);
	}
	
	// no more dirty
	pDirtyRig = false;
}

void reRig::UpdateBoneMatrices(){
	int b, remaining, changeCount;
	reRigBone *parentBone;
	decMatrix matrix;
	
	// reset dirty flags of the bones
	for(b=0; b<pBoneCount; b++){
		pBones[b]->SetDirtyMatrix(true);
	}
	
	// keeps track of the count of bones in need of calculation
	remaining = pBoneCount;
	
	// loop until there are no more remaining bones
	while(remaining > 0){
		// set change count to 0 so we do not loop forever if for some strange
		// reason a loop occurs
		changeCount = 0;
		
		// loop over all bones and calculate those not done so yet
		for(b=0; b<pBoneCount; b++){
			if(pBones[b]->GetDirtyMatrix()){
				parentBone = pBones[b]->GetParentBone();
				
				// check if the parent if present is calculated
				if(parentBone && parentBone->GetDirtyMatrix()) continue;
				
				// calculate the matrix
				matrix = decMatrix::CreateRT(pBones[b]->GetOrientation() * DEG2RAD, pBones[b]->GetPosition());
				if(parentBone){
					matrix *= parentBone->GetMatrix();
				}
				pBones[b]->SetMatrices(matrix);
				
				// mark as no more dirty and decrease count of remaining bones by one
				pBones[b]->SetDirtyMatrix(false);
				remaining--;
				changeCount++;
			}
		}
		
		// it should never happen that nothing changed since then we ended up in
		// a bone-loop. not sure yet what to do but let's get angry on the console
		// and leave it to this
		if(changeCount == 0){
			GetLogger()->LogError(LOGSOURCE, "Bone loop detected while updating bone matrices");
			break;
		}
	}
}

void reRig::SetPoseFromRest(){
	pPoseMatrix.SetIdentity();
}

decVector reRig::GetLightPosition() const{
	return decVector(); //pEngLight->GetPosition().ToVector();
}

void reRig::SetLightPosition(const decVector &position){
	//pEngLight->SetPosition( position );
}

const decColor &reRig::GetAmbientLight() const{
	return pEngWorld->GetAmbientLight();
}

void reRig::SetAmbientLight(const decColor &ambientLight){
	pEngWorld->SetAmbientLight(ambientLight);
}

void reRig::InitDelegates(){
	GetEnvironment()->SetColliderDelegee(pEngSimCollider, &pCLSimulation);
}



// Editing
////////////

void reRig::SetElementMode(eElementModes mode){
	if(mode < eemBone || mode > eemPush){
		DETHROW(deeInvalidParam);
	}
	
	if(mode != pElementMode){
		pElementMode = mode;
		NotifyModeChanged();
	}
}

void reRig::SetWorkMode(eWorkModes mode){
	if(mode < ewmSelect || mode > ewm3DCursor){
		DETHROW(deeInvalidParam);
	}
	
	if(mode != pWorkMode){
		pWorkMode = mode;
		NotifyModeChanged();
	}
}

void reRig::SetLockAxisX(bool lock){
	pLockAxisX = lock;
}

void reRig::SetLockAxisY(bool lock){
	pLockAxisY = lock;
}

void reRig::SetLockAxisZ(bool lock){
	pLockAxisZ = lock;
}

void reRig::SetUseLocal(bool useLocal){
	pUseLocal = useLocal;
}

void reRig::SetSimulationRunning(bool simulationRunning){
	if(simulationRunning == pSimulationRunning){
		return;
	}
	
	pSimulationRunning = simulationRunning;
	
	// update animator
	pEngAnimatorInstance->SetProtectDynamicBones(simulationRunning);
	
	// reset pose matrix
	pPoseMatrix.SetIdentity();
	pPoseChanged = true;
	
	// mark constraints dirty
	int i;
	for(i=0; i<pConstraintCount; i++){
		pConstraints[i]->InvalidatePositions();
	}
	
	// enabled/disable simulation collider
	pEngSimCollider->SetEnabled(simulationRunning);
	
	// reset position of the simulation collider. for this the position of the
	// individual bones is copied from the current pose. velocities are all
	// set to zero
	pEngSimCollider->SetPosition(decVector());
	pEngSimCollider->SetOrientation(decQuaternion());
	pEngSimCollider->SetLinearVelocity(decVector());
	pEngSimCollider->SetAngularVelocity(decVector());
	
	if(pEngComponent){
		pEngComponent->SetPosition(decVector());
		pEngComponent->SetOrientation(decQuaternion());
		pEngComponent->PrepareBones();
		
		for(i=0; i<pBoneCount; i++){
			deComponentBone &compBone = pEngComponent->GetBoneAt(i);
			deColliderBone &colBone = pEngSimCollider->GetBoneAt(i);
			const decMatrix matrix = compBone.GetMatrix();
			
			colBone.SetPosition(matrix * pBones[i]->GetCentralMassPoint());
			colBone.SetOrientation(matrix.ToQuaternion());
			colBone.SetLinearVelocity(decVector());
			colBone.SetAngularVelocity(decVector());
			colBone.SetMass(pBones[i]->GetMass());
			colBone.SetDynamic(pBones[i]->GetDynamic());
			colBone.UpdateMatrix();
		}
	}
	
	// if simulation starts apply pushes
	if(simulationRunning){
		deBasePhysicsWorld * const phyWorld = pEngWorld->GetPeerPhysics();
		
		if(phyWorld){
			const float maxDistance = 10.0f;
			reCLApplyPush applyPush(this);
			
			applyPush.SetCollider(pEngSimCollider);
			
			for(i=0; i<pPushCount; i++){
				const decMatrix &rotmat = decMatrix::CreateRotation(pPushes[i]->GetOrientation() * DEG2RAD);
				const decVector view = rotmat.TransformView();
				
				if(pPushes[i]->GetType() == reRigPush::eptSimple){
					applyPush.SetRay(decDVector(pPushes[i]->GetPosition()), view * maxDistance);
					applyPush.SetPush(view * pPushes[i]->GetImpuls());
					
					applyPush.ApplyPushIn(*phyWorld, pEngSimCollider->GetCollisionFilter().GetCategory());
					
				}else if(pPushes[i]->GetType() == reRigPush::eptExplosion){
					const int rayCount = pPushes[i]->GetRayCount();
					if(rayCount == 0){
						continue;
					}
					
					const float coneAngle = pPushes[i]->GetConeAngle() * DEG2RAD;
					const decDVector pos = decDVector(pPushes[i]->GetPosition());
					const float rayImpulse = pPushes[i]->GetImpuls() / (float)rayCount;
					const float h = 1.0f - cosf(coneAngle);
					const float a = 2.0f * PI * h / (float)rayCount;
					const float d = sqrtf(a);
					const int Mrho = int(coneAngle / d);
					const float drho = coneAngle / (float)Mrho;
					const float dphi = a / drho;
					const float jrrho = drho * 0.1f; // 0.0;
					const float jrphi = dphi * 0.1f; // 0.0;
					int m, n;
					
					for(m=0; m<Mrho; m++){
						const float rho = coneAngle * ((float)m + 0.5f) / (float)Mrho;
						if(rho > coneAngle){
							break;
						}
						const float sinRho = sinf(rho);
						//const float cosRho = cosf( rho );
						const int Mphi = int(2.0f * PI * sinRho / dphi);
						for(n=0; n<Mphi; n++){
							const float phi = 2.0f * PI * (float)n / (float)Mphi;
							const float jrho = rho + (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * jrrho;
							const float jphi = phi + (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * jrphi;
							const float jsinRho = sinf(jrho);
							const float jcosRho = cosf(jrho);
							
							const decVector view2 = rotmat.Transform(jsinRho * cosf(jphi), jsinRho * sinf(jphi), jcosRho);
							
							applyPush.SetRay(pos, view2 * maxDistance);
							applyPush.SetPush(view2 * rayImpulse);
							
							applyPush.ApplyPushIn(*phyWorld, pEngSimCollider->GetCollisionFilter().GetCategory());
						}
					}
				}
			}
		}
	}
	
	NotifyViewChanged();
}

void reRig::SetGravity(const decVector &gravity){
	if(!gravity.IsEqualTo(pGravity)){
		pGravity = gravity;
		
		pEngWorld->SetGravity(gravity);
	}
}

void reRig::SetLocalGravity(const decVector &gravity){
	if(!gravity.IsEqualTo(pLocalGravity)){
		pLocalGravity = gravity;
		
		pEngSimCollider->SetGravity(gravity);
	}
}

void reRig::SetSlowmotion(float slowmotion){
	slowmotion = decMath::clamp(slowmotion, 0.0f, 1.0f);
	if(fabsf(slowmotion - pSlowmotion) > FLOAT_SAFE_EPSILON){
		pSlowmotion = slowmotion;
		NotifyViewChanged();
	}
}

void reRig::SetDynamic(bool dynamic){
	if(dynamic != pDynamic){
		pDynamic = dynamic;
		
		if(dynamic){
			pEngSimCollider->SetResponseType(deCollider::ertDynamic);
			pEngSimCollider->SetUseLocalGravity(false);
			
		}else{
			pEngSimCollider->SetResponseType(deCollider::ertKinematic);
			pEngSimCollider->SetUseLocalGravity(true);
		}
		
		NotifyRigChanged();
	}
}

void reRig::SetMass(float mass){
	mass = decMath::max(mass, 0.001f);
	if(fabsf(mass - pMass) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMass = mass;
	pEngSimCollider->SetMass(pMass);
	
	NotifyRigChanged();
}

void reRig::UpdateFromSimulation(){
	if(pEngComponent){
		decDMatrix matrix = decDMatrix::CreateFromQuaternion(pEngSimCollider->GetOrientation());
		
		pEngComponent->SetPosition(pEngSimCollider->GetPosition());
		pEngComponent->SetOrientation(matrix.ToQuaternion());
		
		pPoseMatrix = matrix * decDMatrix::CreateTranslation(pEngSimCollider->GetPosition());
		pPoseChanged = true;
	}
}



// Bones
//////////

reRigBone *reRig::GetBoneAt(int index) const{
	if(index < 0 || index >= pBoneCount) DETHROW(deeInvalidParam);
	
	return pBones[index];
}

reRigBone *reRig::GetBoneNamed(const char *name) const{
	if(!name) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(strcmp(name, pBones[i]->GetName()) == 0){
			return pBones[i];
		}
	}
	
	return NULL;
}

reRigBone *reRig::GetBoneWith(deColliderVolume *collider) const{
	if(!collider) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(collider == pBones[i]->GetCollider()){
			return pBones[i];
		}
	}
	
	return NULL;
}

reRigBone *reRig::GetBoneWithOrder(int order) const{
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(order == pBones[i]->GetOrder()){
			return pBones[i];
		}
	}
	
	return NULL;
}

int reRig::IndexOfBone(reRigBone::Ref bone) const{
	if(!bone) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(bone == pBones[i]) return i;
	}
	
	return -1;
}

bool reRig::HasBone(reRigBone::Ref bone) const{
	if(!bone) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(bone == pBones[i]) return true;
	}
	
	return false;
}

void reRig::AddBone(reRigBone::Ref bone){
	if(HasBone(bone)) DETHROW(deeInvalidParam);
	
	if(pBoneCount == pBoneSize){
		int newSize = pBoneSize * 3 / 2 + 1;
		reRigBone **newArray = new reRigBone*[newSize];
		if(pBones){
			memcpy(newArray, pBones, sizeof(reRigBone*) * pBoneSize);
			delete [] pBones;
		}
		pBones = newArray;
		pBoneSize = newSize;
	}
	
	pBones[pBoneCount] = bone;
	pBoneCount++;
	bone->SetRig(this);
	
	ReorderBones();
	
	UpdateBoneMatrices();
	NotifyBoneCountChanged();
}

void reRig::RemoveBone(reRigBone::Ref bone){
	int i, index = IndexOfBone(bone);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pBoneCount; i++){
		pBones[i - 1] = pBones[i];
	}
	pBoneCount--;
	
	bone->SetRig(NULL);
	ReorderBones();
	
	UpdateBoneMatrices();
	NotifyBoneCountChanged();
}

void reRig::RemoveAllBones(){
	while(pBoneCount > 0){
		pBoneCount--;
		pBones[pBoneCount]->SetRig(NULL);
		pBones[pBoneCount]->FreeReference();
	}
	
	UpdateBoneMatrices();
	NotifyBoneCountChanged();
}

void reRig::ReorderBones(){
	int b, nextOrder = 0;
	
	// first reset the order of all bones
	for(b=0; b<pBoneCount; b++){
		pBones[b]->SetOrder(-1);
	}
	
	// keep looping until all bones have an order assigned
	while(nextOrder < pBoneCount){
		for(b=0; b<pBoneCount; b++){
			// only operate on bones without an order set yet
			if(pBones[b]->GetOrder() == -1){
				// if the bone has no parent or the parent has an order assigned
				// assign the next order to this bone
				if(!pBones[b]->GetParentBone() || pBones[b]->GetParentBone()->GetOrder() != -1){
					pBones[b]->SetOrder(nextOrder);
					nextOrder++;
				}
			}
		}
	}
	/*
	GetLogger()->LogInfo(LOGSOURCE, "reorder:");
	for(b=0; b<pBoneCount; b++){
		GetLogger()->LogInfoFormat(LOGSOURCE, "- bone %i => order %i", b, pBones[b]->GetOrder());
	}
	*/
}

void reRig::SetAllBonesVisited(bool visited){
	int b;
	
	for(b=0; b<pBoneCount; b++){
		pBones[b]->SetVisited(visited);
	}
}

void reRig::SetRootBone(reRigBone::Ref rootBone){
	if(rootBone != pRootBone){
		pRootBone = rootBone;
		NotifyRigChanged();
	}
}



// Shapes
//////////

reRigShape *reRig::GetShapeAt(int index) const{
	if(index < 0 || index >= pShapeCount) DETHROW(deeInvalidParam);
	
	return pShapes[index];
}

reRigShape *reRig::GetShapeWith(deColliderVolume *collider) const{
	if(!collider) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(collider == pShapes[i]->GetCollider()){
			return pShapes[i];
		}
	}
	
	return NULL;
}

int reRig::IndexOfShape(reRigShape::Ref shape) const{
	if(!shape) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(shape == pShapes[i]) return i;
	}
	
	return -1;
}

bool reRig::HasShape(reRigShape::Ref shape) const{
	if(!shape) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(shape == pShapes[i]) return true;
	}
	
	return false;
}

void reRig::AddShape(reRigShape::Ref shape){
	if(HasShape(shape)) DETHROW(deeInvalidParam);
	
	if(pShapeCount == pShapeSize){
		int newSize = pShapeSize * 3 / 2 + 1;
		reRigShape **newArray = new reRigShape*[newSize];
		if(pShapes){
			memcpy(newArray, pShapes, sizeof(reRigShape*) * pShapeSize);
			delete [] pShapes;
		}
		pShapes = newArray;
		pShapeSize = newSize;
	}
	
	pShapes[pShapeCount] = shape;
	pShapeCount++;
	shape->SetRig(this);
	
	NotifyShapeCountChanged();
}

void reRig::RemoveShape(reRigShape::Ref shape){
	int i, index = IndexOfShape(shape);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pShapeCount; i++){
		pShapes[i - 1] = pShapes[i];
	}
	pShapeCount--;
	
	shape->SetRig(NULL);
	NotifyShapeCountChanged();
}

void reRig::RemoveAllShapes(){
	while(pShapeCount > 0){
		pShapeCount--;
		pShapes[pShapeCount]->SetRig(NULL);
		pShapes[pShapeCount]->FreeReference();
	}
	
	NotifyShapeCountChanged();
}



// Constraints
////////////////

reRigConstraint *reRig::GetConstraintAt(int index) const{
	if(index < 0 || index >= pConstraintCount) DETHROW(deeInvalidParam);
	
	return pConstraints[index];
}

reRigConstraint *reRig::GetConstraintWith(deColliderVolume *collider) const{
	if(!collider) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(collider == pConstraints[i]->GetCollider()){
			return pConstraints[i];
		}
	}
	
	return NULL;
}

int reRig::IndexOfConstraint(reRigConstraint::Ref constraint) const{
	if(!constraint) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(constraint == pConstraints[i]) return i;
	}
	
	return -1;
}

bool reRig::HasConstraint(reRigConstraint::Ref constraint) const{
	if(!constraint) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		if(constraint == pConstraints[i]) return true;
	}
	
	return false;
}

void reRig::AddConstraint(reRigConstraint::Ref constraint){
	if(HasConstraint(constraint)) DETHROW(deeInvalidParam);
	
	if(pConstraintCount == pConstraintSize){
		int newSize = pConstraintSize * 3 / 2 + 1;
		reRigConstraint **newArray = new reRigConstraint*[newSize];
		if(pConstraints){
			memcpy(newArray, pConstraints, sizeof(reRigConstraint*) * pConstraintSize);
			delete [] pConstraints;
		}
		pConstraints = newArray;
		pConstraintSize = newSize;
	}
	
	pConstraints[pConstraintCount] = constraint;
	pConstraintCount++;
	constraint->SetRig(this);
	
	NotifyConstraintCountChanged();
	
	deColliderConstraint *engConstraint = NULL;
	try{
		engConstraint = new deColliderConstraint;
		if(!engConstraint) DETHROW(deeOutOfMemory);
		
		pEngSimCollider->AddConstraint(engConstraint);
		
	}catch(const deException &){
		if(engConstraint) delete engConstraint;
		throw;
	}
	
	constraint->SetEngineConstraint(engConstraint);
}

void reRig::RemoveConstraint(reRigConstraint::Ref constraint){
	int i, index = IndexOfConstraint(constraint);
	if(index == -1) DETHROW(deeInvalidParam);
	
	deColliderConstraint *engConstraint = constraint->GetEngineConstraint();
	if(engConstraint && pEngSimCollider->HasConstraint(engConstraint)){
		pEngSimCollider->RemoveConstraint(engConstraint);
	}
	constraint->SetEngineConstraint(NULL);
	
	for(i=index+1; i<pConstraintCount; i++){
		pConstraints[i - 1] = pConstraints[i];
	}
	pConstraintCount--;
	
	constraint->SetRig(NULL);
	NotifyConstraintCountChanged();
}

void reRig::RemoveAllConstraints(){
	deColliderConstraint *engConstraint;
	
	while(pConstraintCount > 0){
		pConstraintCount--;
		
		engConstraint = pConstraints[pConstraintCount]->GetEngineConstraint();
		if(engConstraint && pEngSimCollider->HasConstraint(engConstraint)){
			pEngSimCollider->RemoveConstraint(engConstraint);
		}
		pConstraints[pConstraintCount]->SetEngineConstraint(NULL);
		
		pConstraints[pConstraintCount]->SetRig(NULL);
		pConstraints[pConstraintCount]->FreeReference();
	}
	
	NotifyConstraintCountChanged();
}



// Pushes
////////////////

reRigPush *reRig::GetPushAt(int index) const{
	if(index < 0 || index >= pPushCount) DETHROW(deeInvalidParam);
	
	return pPushes[index];
}

reRigPush *reRig::GetPushWith(deColliderVolume *collider) const{
	if(!collider) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(collider == pPushes[i]->GetCollider()){
			return pPushes[i];
		}
	}
	
	return NULL;
}

int reRig::IndexOfPush(reRigPush::Ref push) const{
	if(!push) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(push == pPushes[i]) return i;
	}
	
	return -1;
}

bool reRig::HasPush(reRigPush::Ref push) const{
	if(!push) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPushCount; i++){
		if(push == pPushes[i]) return true;
	}
	
	return false;
}

void reRig::AddPush(reRigPush::Ref push){
	if(HasPush(push)) DETHROW(deeInvalidParam);
	
	if(pPushCount == pPushSize){
		int newSize = pPushSize * 3 / 2 + 1;
		reRigPush **newArray = new reRigPush*[newSize];
		if(pPushes){
			memcpy(newArray, pPushes, sizeof(reRigPush*) * pPushSize);
			delete [] pPushes;
		}
		pPushes = newArray;
		pPushSize = newSize;
	}
	
	pPushes[pPushCount] = push;
	pPushCount++;
	push->SetRig(this);
	
	NotifyPushCountChanged();
}

void reRig::RemovePush(reRigPush::Ref push){
	int i, index = IndexOfPush(push);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pPushCount; i++){
		pPushes[i - 1] = pPushes[i];
	}
	pPushCount--;
	
	push->SetRig(NULL);
	NotifyPushCountChanged();
}

void reRig::RemoveAllPushes(){
	while(pPushCount > 0){
		pPushCount--;
		pPushes[pPushCount]->SetRig(NULL);
		pPushes[pPushCount]->FreeReference();
	}
	
	NotifyPushCountChanged();
}



// Notifiers
//////////////

reRigNotifier *reRig::GetNotifierAt(int index) const{
	if(index < 0 || index >= pNotifierCount) DETHROW(deeInvalidParam);
	
	return pNotifiers[index];
}

int reRig::IndexOfNotifier(reRigNotifier::Ref notifier) const{
	if(!notifier) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		if(notifier == pNotifiers[i]) return i;
	}
	
	return -1;
}

bool reRig::HasNotifier(reRigNotifier::Ref notifier) const{
	if(!notifier) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		if(notifier == pNotifiers[i]) return true;
	}
	
	return false;
}

void reRig::AddNotifier(reRigNotifier::Ref notifier){
	if(HasNotifier(notifier)) DETHROW(deeInvalidParam);
	
	if(pNotifierCount == pNotifierSize){
		int newSize = pNotifierSize * 3 / 2 + 1;
		reRigNotifier **newArray = new reRigNotifier*[newSize];
		if(pNotifiers){
			memcpy(newArray, pNotifiers, sizeof(reRigNotifier*) * pNotifierSize);
			delete [] pNotifiers;
		}
		pNotifiers = newArray;
		pNotifierSize = newSize;
	}
	
	pNotifiers[pNotifierCount] = notifier;
	pNotifierCount++;
}

void reRig::RemoveNotifier(reRigNotifier::Ref notifier){
	int i, index = IndexOfNotifier(notifier);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pNotifierCount; i++){
		pNotifiers[i - 1] = pNotifiers[i];
	}
	pNotifierCount--;
}

void reRig::RemoveAllNotifiers(){
	while(pNotifierCount > 0){
		pNotifierCount--;
		pNotifiers[pNotifierCount]->FreeReference();
	}
}



void reRig::NotifyModeChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ModeChanged(this);
	}
}

void reRig::NotifyStateChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->StateChanged(this);
	}
}

void reRig::NotifyUndoChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->UndoChanged(this);
	}
}



void reRig::NotifyAllStructureChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->StructureChanged(this);
	}
	
	ReorderBones();
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyRigChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->RigChanged(this);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyResourceChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ResourceChanged(this);
	}
}



void reRig::NotifySkyChanged(){
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->SkyChanged(this);
	}
}

void reRig::NotifyEnvObjectChanged(){
	int i;
	
	for(i=0; i<pNotifierCount; i++){
		pNotifiers[i]->EnvObjectChanged(this);
	}
}

void reRig::NotifyViewChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ViewChanged(this);
	}
}



void reRig::NotifyCameraChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->CameraChanged(this);
	}
}

void reRig::NotifyCameraViewChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->CameraViewChanged(this);
	}
}



void reRig::NotifyBoneCountChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->BoneCountChanged(this);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyBoneChanged(reRigBone::Ref bone){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->BoneChanged(this, bone);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyBoneSelectedChanged(reRigBone::Ref bone){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->BoneSelectedChanged(this, bone);
	}
}

void reRig::NotifyAllBonesDeselected(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->AllBonesDeselected(this);
	}
}

void reRig::NotifyActiveBoneChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ActiveBoneChanged(this);
	}
}

void reRig::NotifyAllBoneChanged(reRigBone::Ref bone){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->BoneChanged(this, bone);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}



void reRig::NotifyShapeCountChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ShapeCountChanged(this);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyShapeChanged(reRigShape::Ref shape){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ShapeChanged(this, shape);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyShapeSelectedChanged(reRigShape::Ref shape){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ShapeSelectedChanged(this, shape);
	}
}

void reRig::NotifyAllShapesDeselected(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->AllShapesDeselected(this);
	}
}

void reRig::NotifyActiveShapeChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ActiveShapeChanged(this);
	}
}

void reRig::NotifyAllShapeChanged(reRigShape::Ref shape){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ShapeChanged(this, shape);
	}
	
	pPoseChanged = true;
	pDirtyRig = true;
	SetChanged(true);
}



void reRig::NotifyConstraintCountChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ConstraintCountChanged(this);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyConstraintChanged(reRigConstraint::Ref constraint){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ConstraintChanged(this, constraint);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyConstraintSelectedChanged(reRigConstraint::Ref constraint){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ConstraintSelectedChanged(this, constraint);
	}
}

void reRig::NotifyAllConstraintsDeselected(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->AllConstraintsDeselected(this);
	}
}

void reRig::NotifyActiveConstraintChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ActiveConstraintChanged(this);
	}
}

void reRig::NotifyAllConstraintChanged(reRigConstraint::Ref constraint){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ConstraintChanged(this, constraint);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyAllConstraintDofChanged(reRigConstraint::Ref constraint, deColliderConstraint::eDegreesOfFreedom dof){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ConstraintDofChanged(this, constraint, dof);
	}
	
	pDirtyRig = true;
	SetChanged(true);
}



void reRig::NotifyPushCountChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PushCountChanged(this);
	}
}

void reRig::NotifyPushChanged(reRigPush::Ref push){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PushChanged(this, push);
	}
}

void reRig::NotifyPushSelectedChanged(reRigPush::Ref push){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->PushSelectedChanged(this, push);
	}
}

void reRig::NotifyAllPushesDeselected(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->AllPushesDeselected(this);
	}
}

void reRig::NotifyActivePushChanged(){
	int n;
	
	for(n=0; n<pNotifierCount; n++){
		pNotifiers[n]->ActivePushChanged(this);
	}
}



// Private Functions
//////////////////////

void reRig::pCleanUp(){
	SetSimulationRunning(false);
	
	if(pSky){
		delete pSky;
	}
	pEnvObject = nullptr;
	
	if(pCamera){
		delete pCamera;
	}
	
	if(pSelectionConstraints){
		delete pSelectionConstraints;
	}
	if(pSelectionPushes){
		delete pSelectionPushes;
	}
	if(pSelectionShapes){
		delete pSelectionShapes;
	}
	if(pSelectionBones){
		delete pSelectionBones;
	}
	RemoveAllNotifiers();
	if(pNotifiers){
		delete [] pNotifiers;
	}
	
	RemoveAllPushes();
	if(pPushes){
		delete [] pPushes;
	}
	
	RemoveAllConstraints();
	if(pConstraints){
		delete [] pConstraints;
	}
	
	RemoveAllShapes();
	if(pShapes){
		delete [] pShapes;
	}
	
	RemoveAllBones();
	if(pBones){
		delete [] pBones;
	}
	
	if(pComponentTextures){
		while(pComponentTextureCount > 0){
			pComponentTextureCount--;
			delete pComponentTextures[pComponentTextureCount];
		}
		delete [] pComponentTextures;
	}
	
	if(pEngSimCollider){
		pEngSimCollider->SetComponent(NULL);
	}
	if(pEngAnimatorInstance){
		pEngAnimatorInstance->SetComponent(NULL);
		pEngAnimatorInstance->SetAnimator(NULL);
	}
	if(pEngAnimator){
		pEngAnimator->SetRig(NULL);
	}
	if(pEngComponent){
		pEngComponent->SetRig(NULL);
	}
}



void reRig::pCreateWorld(){
	// create world
	pEngWorld = GetEngine()->GetWorldManager()->CreateWorld();
	//pEngWorld->SetSectorSize( decVector( 500.0f, 500.0f, 500.0f ) );
	pEngWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f)); //decColor(0.1f, 0.1f, 0.1f));
	pEngWorld->SetGravity(pGravity);
	pEngWorld->SetDisableLights(false);
}

void reRig::pCreateCamera(){
	pCamera = new reCamera(this, GetEngine());
	
	pCamera->SetName("Orbiting Camera");
	pCamera->SetEngineWorld(pEngWorld);
	
	pCamera->Reset();
}

void reRig::pUpdateComponent(){
	bool rebuildTextures = false;
	deModel::Ref model = NULL;
	deSkin::Ref skin = NULL;
	
	// try to load the model and skin if possible
	try{
		if(strlen(pModelPath) > 0){
			model = GetEngine()->GetModelManager()->LoadModel(pModelPath, "/");
		}
		
		if(strlen(pSkinPath) > 0){
			skin = GetEngine()->GetSkinManager()->LoadSkin(pSkinPath, "/");
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	// protect the loaded parts
	try{
		// if the skin is missing use the default one
		if(!skin){
			skin = GetGameDefinition()->GetDefaultSkin();
		}
		
		// reset the animator
		pEngAnimatorInstance->SetComponent(NULL); // otherwise the animator is not reset
		
		// update the component with the model and skin
		if(model && skin){
			if(pEngComponent){
				rebuildTextures = (model != pEngComponent->GetModel());
				pEngComponent->SetModelAndSkin(model, skin);
				
			}else{
				rebuildTextures = true;
				pEngComponent = GetEngine()->GetComponentManager()->CreateComponent(model, skin);
				pEngWorld->AddComponent(pEngComponent);
			}
			
		}else if(pEngComponent){
			pEngSimCollider->SetComponent(NULL);
			pEngWorld->RemoveComponent(pEngComponent);
			pEngComponent = NULL;
		}
		
		// update component with rig
		if(pEngComponent && pEngRig){
			pEngComponent->SetRig(pEngRig);
		}
		
		pEngAnimator->SetRig(pEngRig);
		
		// init component if existing
		if(pEngComponent){
			pEngComponent->SetVisible(true);
			pEngComponent->SetPosition(decDVector(0.0f, 0.0f, 0.0f));
			pEngComponent->SetOrientation(decQuaternion());
			
			pEngSimCollider->SetComponent(pEngComponent);
		}
		
		// free the reference we hold
		if(model) model->FreeReference();
		if(skin) skin->FreeReference();
		
	}catch(const deException &){
		if(model) model->FreeReference();
		if(skin) skin->FreeReference();
		throw;
	}
	
	// rebuild textures if required
	if(rebuildTextures){
		if(pComponentTextures){
			while(pComponentTextureCount > 0){
				pComponentTextureCount--;
				delete pComponentTextures[pComponentTextureCount];
			}
			delete [] pComponentTextures;
			pComponentTextures = NULL;
		}
		
		if(pEngComponent->GetModel()){
			deModel *engModel = pEngComponent->GetModel();
			int t, textureCount = engModel->GetTextureCount();
			
			if(textureCount > 0){
				pComponentTextures = new reRigTexture*[textureCount];
				if(!pComponentTextures) DETHROW(deeOutOfMemory);
				
				for(t=0; t<textureCount; t++){
					pComponentTextures[t] = new reRigTexture(GetEngine(), engModel->GetTextureAt(t)->GetName());
					if(!pComponentTextures[t]) DETHROW(deeOutOfMemory);
				}
				
				pComponentTextureCount = textureCount;
			}
		}
	}
	
	// update the component textures
	UpdateComponentTextures();
	
	// update the animator
	pEngAnimatorInstance->SetComponent(pEngComponent);
	pUpdateAnimator();
}

void reRig::pUpdateAnimator(){
	deAnimation::Ref animation = NULL;
	
	// try to load the animation if possible
	try{
		if(strlen(pAnimationPath) > 0){
			animation = GetEngine()->GetAnimationManager()->LoadAnimation(pAnimationPath, "/");
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	// protect the loaded parts
	try{
		// set animation
		pEngAnimator->SetAnimation(animation);
		
		// free the reference we hold
		if(animation) animation->FreeReference();
		
	}catch(const deException &){
		if(animation) animation->FreeReference();
		throw;
	}
	
	pUpdateAnimatorMove();
}

void reRig::pUpdateAnimatorMove(){
	deAnimationMove *move = NULL;
	deAnimation::Ref animation;
	int index;
	
	pEngAnimatorAnim->SetMoveName(pMoveName);
	pEngAnimator->NotifyRulesChanged();
	
	// try to locate the move
	animation = pEngAnimator->GetAnimation();
	if(animation){
		index = animation->FindMove(pMoveName);
		if(index != -1){
			move = animation->GetMove(index);
		}
	}
	
	// if there is a move get the playtime and adjust the controller
	if(move){
		pPlayTime = move->GetPlaytime();
		
	}else{
		pPlayTime = 0.0f;
	}
	
	pEngAnimatorInstance->GetControllerAt(0).SetValueRange(0.0f, pPlayTime);
	pEngAnimatorInstance->NotifyControllerChangedAt(0);
}

void reRig::pUpdateAnimatorTime(){
	pEngAnimatorInstance->GetControllerAt(0).SetCurrentValue(pMoveTime);
	pEngAnimatorInstance->NotifyControllerChangedAt(0);
}



void reRig::pBoneShowStateChanged(){
	int i;
	
	for(i=0; i<pBoneCount; i++){
		pBones[i]->ShowStateChanged();
	}
}

void reRig::pShapeShowStateChanged(){
	int i;
	
	for(i=0; i<pShapeCount; i++){
		pShapes[i]->ShowStateChanged();
	}
}

void reRig::pConstraintShowStateChanged(){
	int i;
	
	for(i=0; i<pConstraintCount; i++){
		pConstraints[i]->ShowStateChanged();
	}
}

void reRig::pPushShowStateChanged(){
	int i;
	
	for(i=0; i<pPushCount; i++){
		pPushes[i]->ShowStateChanged();
	}
}
