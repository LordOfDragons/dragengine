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
pEngAnimatorAnim(nullptr),
pEngAnimatorRestPose(nullptr),

pSky(nullptr),
pPoseChanged(true),

pModelCollision(false),

pMoveName("idle"),
pMoveTime(0.0f),
pPlayTime(0.0f),
pUseRestPose(true),
pPlaybackMove(false),

pCamera(nullptr),

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

pSelectionBones(nullptr),
pSelectionShapes(nullptr),
pSelectionPushes(nullptr),
pSelectionConstraints(nullptr),

pDirtyRig(true)
{
	deEngine * const engine = GetEngine();
	
	deAnimatorController *amController = nullptr;
	deAnimatorRuleAnimation::Ref amRuleAnim;
	deAnimatorRuleStateManipulator::Ref amRuleRestPose;
	deAnimatorRuleStateSnapshot::Ref amRuleStateSnapshot;
	deAnimatorLink *engLink = nullptr;
	
	try{
		SetFilePath("new.derig");
		
		// create animator
		pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
		
		amController = new deAnimatorController;
		amController->SetClamp(true);
		pEngAnimator->AddController(amController);
		amController = nullptr;
		
		engLink = new deAnimatorLink;
		engLink->SetController(0);
		pEngAnimator->AddLink(engLink);
		engLink = nullptr;
		
		amRuleStateSnapshot = deAnimatorRuleStateSnapshot::Ref::New();
		amRuleStateSnapshot->SetUseLastState(true);
		pEngAnimator->AddRule(amRuleStateSnapshot);
		amRuleStateSnapshot = nullptr;
		
		amRuleAnim = deAnimatorRuleAnimation::Ref::New();
		amRuleAnim->GetTargetMoveTime().AddLink(0);
		amRuleAnim->SetEnabled(!pUseRestPose);
		pEngAnimator->AddRule(amRuleAnim);
		pEngAnimatorAnim = amRuleAnim;
		amRuleAnim = nullptr;
		
		amRuleRestPose = deAnimatorRuleStateManipulator::Ref::New();
		amRuleRestPose->SetEnabled(true);
		amRuleRestPose->SetEnablePosition(true);
		amRuleRestPose->SetEnableRotation(true);
		amRuleRestPose->SetEnabled(pUseRestPose);
		pEngAnimator->AddRule(amRuleRestPose);
		pEngAnimatorRestPose = amRuleRestPose;
		amRuleRestPose = nullptr;
		
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
		pEnvObject = igdeWObject::Ref::New(*environment);
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

void reRig::UpdateComponentTextures(){
	if(pEngComponent && pComponentTextures.IsNotEmpty()){
		const deSkin::Ref &defaultSkin = GetGameDefinition()->GetDefaultSkin();
		const deSkin::Ref &engSkin = pEngComponent->GetSkin();
		int defaultTexture = 0;
		int i;
		
		defaultTexture = 0;
		
		for(i=0; i<pComponentTextures.GetCount(); i++){
			const reRigTexture &texture = pComponentTextures.GetAt(i);
			deComponentTexture &engTexture = pEngComponent->GetTextureAt(i);
			
			deSkin *useSkin = nullptr;
			int useTexture = 0;
			
			if(texture.GetEngineSkin()){
				useSkin = texture.GetEngineSkin();
				useTexture = 0;
			}
			
			if(!useSkin && (!engSkin || engSkin->IndexOfTextureNamed(texture.GetName()) == -1)){
				useSkin = defaultSkin;
				useTexture = defaultTexture;
			}
			
			if(useSkin != engTexture.GetSkin() || useTexture != engTexture.GetTexture()){
				engTexture.SetSkin(useSkin);
				engTexture.SetTexture(useTexture);
				
				pEngComponent->NotifyTextureChanged(i);
			}
		}
	}
}



void reRig::UpdateWorld(float elapsed){
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
	pBones.Visit([](reRigBone &b){
		b.UpdateFromPose();
	});
	if(pSimulationRunning){
		pConstraints.Visit([](reRigConstraint &c){
			c.InvalidatePositions();
		});
	}
	
	// update shapes and constraints
	if(pPoseChanged){
		pShapes.Visit([](reRigShape &s){
			s.Update();
		});
		pPoseChanged = false;
	}
	
	pConstraints.Visit([](reRigConstraint &c){
		c.Update();
	});
	pBones.Visit([](reRigBone &b){
		b.UpdateShapesAndConstraints();
	});
	
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
	
	// free the old rig
	if(pEngRig){
		if(pEngComponent){
			pEngSimCollider->SetComponent(nullptr);
			pEngAnimatorInstance->SetComponent(nullptr);
			pEngComponent->SetRig(nullptr);
		}
		
		pEngAnimator->SetRig(nullptr);
		pEngRig = nullptr;
		
	// if the rig does not exist we have to init the pose matrices from the
	// rest pose to avoid sick looking rig
	}else{
		UpdateBoneMatrices();
		SetPoseFromRest();
		pBones.Visit([](reRigBone &b){
			b.SetPoseFromRest();
		});
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
	
	pEngSimCollider->SetRig(pEngRig);
	
	// no more dirty
	pDirtyRig = false;
}

void reRig::UpdateBoneMatrices(){
	// reset dirty flags of the bones
	pBones.Visit([](reRigBone *bone){
		bone->SetDirtyMatrix(true);
	});
	
	// keeps track of the count of bones in need of calculation
	int remaining = pBones.GetCount();
	
	// loop until there are no more remaining bones
	while(remaining > 0){
		// set change count to 0 so we do not loop forever if for some strange
		// reason a loop occurs
		int changeCount = 0;
		
		// loop over all bones and calculate those not done so yet
		pBones.Visit([&](reRigBone &bone){
			if(!bone.GetDirtyMatrix()){
				return;
			}
			
			reRigBone *parentBone = bone.GetParentBone();
			// check if the parent if present is calculated
			if(parentBone && parentBone->GetDirtyMatrix()){
				return;
			}
			
			// calculate the matrix
			decMatrix matrix = decMatrix::CreateRT(bone.GetOrientation() * DEG2RAD, bone.GetPosition());
			if(parentBone){
				matrix *= parentBone->GetMatrix();
			}
			bone.SetMatrices(matrix);
			
			// mark as no more dirty and decrease count of remaining bones by one
			bone.SetDirtyMatrix(false);
			remaining--;
			changeCount++;
		});
		
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
	pConstraints.Visit([](reRigConstraint &c){
		c.InvalidatePositions();
	});
	
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
		
		int i;
		for(i=0; i<pBones.GetCount(); i++){
			const reRigBone &bone = pBones.GetAt(i);
			deComponentBone &compBone = pEngComponent->GetBoneAt(i);
			deColliderBone &colBone = pEngSimCollider->GetBoneAt(i);
			const decMatrix matrix = compBone.GetMatrix();
			
			colBone.SetPosition(matrix * bone.GetCentralMassPoint());
			colBone.SetOrientation(matrix.ToQuaternion());
			colBone.SetLinearVelocity(decVector());
			colBone.SetAngularVelocity(decVector());
			colBone.SetMass(bone.GetMass());
			colBone.SetDynamic(bone.GetDynamic());
			colBone.UpdateMatrix();
		}
	}
	
	// if simulation starts apply pushes
	if(simulationRunning){
		deBasePhysicsWorld * const phyWorld = pEngWorld->GetPeerPhysics();
		
		if(phyWorld){
			const float maxDistance = 10.0f;
			reCLApplyPush applyPush(*this);
			
			applyPush.SetCollider(pEngSimCollider);
			
			pPushes.Visit([&](const reRigPush &push){
				const decMatrix &rotmat = decMatrix::CreateRotation(push.GetOrientation() * DEG2RAD);
				const decVector view = rotmat.TransformView();
				
				if(push.GetType() == reRigPush::eptSimple){
					applyPush.SetRay(decDVector(push.GetPosition()), view * maxDistance);
					applyPush.SetPush(view * push.GetImpuls());
					
					applyPush.ApplyPushIn(*phyWorld, pEngSimCollider->GetCollisionFilter().GetCategory());
					
				}else if(push.GetType() == reRigPush::eptExplosion){
					const int rayCount = push.GetRayCount();
					if(rayCount == 0){
						return;
					}
					
					const float coneAngle = push.GetConeAngle() * DEG2RAD;
					const decDVector pos = decDVector(push.GetPosition());
					const float rayImpulse = push.GetImpuls() / (float)rayCount;
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
			});
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
reRigBone *reRig::GetBoneNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pBones.FindOrDefault([name](const reRigBone &b){
		return b.GetName() == name;
	});
}

reRigBone *reRig::GetBoneWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	return pBones.FindOrDefault([&](const reRigBone &b){
		return b.GetCollider() == collider;
	});
}

reRigBone *reRig::GetBoneWithOrder(int order) const{
	return pBones.FindOrDefault([&](const reRigBone &b){
		return b.GetOrder() == order;
	});
}

void reRig::AddBone(reRigBone *bone){
	pBones.Add(bone);
	bone->SetRig(this);
	
	ReorderBones();
	
	UpdateBoneMatrices();
	NotifyBoneCountChanged();
}

void reRig::RemoveBone(reRigBone *bone){
	const reRigBone::Ref guard(bone);
	pBones.Remove(bone);
	
	bone->SetRig(nullptr);
	ReorderBones();
	
	UpdateBoneMatrices();
	NotifyBoneCountChanged();
}

void reRig::RemoveAllBones(){
	pBones.Visit([](reRigBone *bone){
		bone->SetRig(nullptr);
	});
	pBones.RemoveAll();
	
	UpdateBoneMatrices();
	NotifyBoneCountChanged();
}

void reRig::ReorderBones(){
	const int count = pBones.GetCount();
	int nextOrder = 0;
	
	// first reset the order of all bones
	pBones.Visit([](reRigBone &bone){
		bone.SetOrder(-1);
	});
	
	// keep looping until all bones have an order assigned
	while(nextOrder < count){
		pBones.Visit([&](reRigBone &bone){
			// only operate on bones without an order set yet
			if(bone.GetOrder() != -1){
				return;
			}
			
			// if the bone has no parent or the parent has an order assigned
			// assign the next order to this bone
			reRigBone * const parent = bone.GetParentBone();
			if(!parent || parent->GetOrder() != -1){
				bone.SetOrder(nextOrder++);
			}
		});
	}
	/*
	GetLogger()->LogInfo(LOGSOURCE, "reorder:");
	for(b=0; b<count; b++){
		GetLogger()->LogInfoFormat(LOGSOURCE, "- bone %i => order %i", b, pBones.GetAt(b)->GetOrder());
	}
	*/
}

void reRig::SetAllBonesVisited(bool visited){
	pBones.Visit([visited](reRigBone *bone){
		bone->SetVisited(visited);
	});
}

void reRig::SetRootBone(reRigBone *rootBone){
	if(rootBone != pRootBone){
		pRootBone = rootBone;
		NotifyRigChanged();
	}
}



// Shapes
//////////
reRigShape *reRig::GetShapeWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	return pShapes.FindOrDefault([collider](const reRigShape &s){
		return s.GetCollider() == collider;
	});
}

void reRig::AddShape(reRigShape *shape){
	pShapes.Add(shape);
	shape->SetRig(this);
	
	NotifyShapeCountChanged();
}

void reRig::RemoveShape(reRigShape *shape){
	const reRigShape::Ref guard(shape);
	pShapes.Remove(shape);
	shape->SetRig(nullptr);
	NotifyShapeCountChanged();
}

void reRig::RemoveAllShapes(){
	pShapes.Visit([](reRigShape *shape){
		shape->SetRig(nullptr);
	});
	pShapes.RemoveAll();
	
	NotifyShapeCountChanged();
}



// Constraints
////////////////
reRigConstraint *reRig::GetConstraintWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	return pConstraints.FindOrDefault([collider](const reRigConstraint &c){
		return c.GetCollider() == collider;
	});
}

void reRig::AddConstraint(reRigConstraint *constraint){
	pConstraints.Add(constraint);
	constraint->SetRig(this);
	
	NotifyConstraintCountChanged();
	
	const deColliderConstraint::Ref engConstraint(deColliderConstraint::Ref::New());
	pEngSimCollider->AddConstraint(engConstraint);
	constraint->SetEngineConstraint(engConstraint);
}

void reRig::RemoveConstraint(reRigConstraint *constraint){
	const reRigConstraint::Ref guard(constraint);
	
	pConstraints.Remove(constraint);
	constraint->SetRig(nullptr);
	
	deColliderConstraint *engConstraint = constraint->GetEngineConstraint();
	if(engConstraint && pEngSimCollider->HasConstraint(engConstraint)){
		pEngSimCollider->RemoveConstraint(engConstraint);
	}
	constraint->SetEngineConstraint(nullptr);
	
	NotifyConstraintCountChanged();
}

void reRig::RemoveAllConstraints(){
	pConstraints.Visit([&](reRigConstraint *constraint){
		deColliderConstraint *engConstraint = constraint->GetEngineConstraint();
		if(engConstraint && pEngSimCollider->HasConstraint(engConstraint)){
			pEngSimCollider->RemoveConstraint(engConstraint);
		}
		constraint->SetEngineConstraint(nullptr);
		constraint->SetRig(nullptr);
	});
	pConstraints.RemoveAll();
	
	NotifyConstraintCountChanged();
}



// Pushes
////////////////

reRigPush *reRig::GetPushWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	return pPushes.FindOrDefault([collider](const reRigPush &p){
		return p.GetCollider() == collider;
	});
}

void reRig::AddPush(reRigPush *push){
	pPushes.Add(push);
	push->SetRig(this);
	
	NotifyPushCountChanged();
}

void reRig::RemovePush(reRigPush *push){
	const reRigPush::Ref guard(push);
	pPushes.Remove(push);
	push->SetRig(nullptr);
	NotifyPushCountChanged();
}

void reRig::RemoveAllPushes(){
	pPushes.Visit([](reRigPush *push){
		push->SetRig(nullptr);
	});
	pPushes.RemoveAll();
	
	NotifyPushCountChanged();
}



// Notifiers
//////////////

void reRig::AddNotifier(reRigNotifier *notifier){
	DEASSERT_NOTNULL(notifier)
	pNotifiers.Add(notifier);
}

void reRig::RemoveNotifier(reRigNotifier *notifier){
	pNotifiers.Remove(notifier);
}
void reRig::NotifyModeChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ModeChanged(this);
	});
}

void reRig::NotifyStateChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.StateChanged(this);
	});
}

void reRig::NotifyUndoChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.UndoChanged(this);
	});
}

void reRig::NotifyAllStructureChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.StructureChanged(this);
	});
	
	ReorderBones();
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyRigChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.RigChanged(this);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyResourceChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ResourceChanged(this);
	});
}

void reRig::NotifySkyChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.SkyChanged(this);
	});
}

void reRig::NotifyEnvObjectChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.EnvObjectChanged(this);
	});
}

void reRig::NotifyViewChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ViewChanged(this);
	});
}

void reRig::NotifyCameraChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.CameraChanged(this);
	});
}

void reRig::NotifyCameraViewChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.CameraViewChanged(this);
	});
}

void reRig::NotifyBoneCountChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.BoneCountChanged(this);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyBoneChanged(reRigBone *bone){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.BoneChanged(this, bone);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyBoneSelectedChanged(reRigBone *bone){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.BoneSelectedChanged(this, bone);
	});
}

void reRig::NotifyAllBonesDeselected(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.AllBonesDeselected(this);
	});
}

void reRig::NotifyActiveBoneChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ActiveBoneChanged(this);
	});
}

void reRig::NotifyAllBoneChanged(reRigBone *bone){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.BoneChanged(this, bone);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyShapeCountChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ShapeCountChanged(this);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyShapeChanged(reRigShape *shape){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ShapeChanged(this, shape);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyShapeSelectedChanged(reRigShape *shape){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ShapeSelectedChanged(this, shape);
	});
}

void reRig::NotifyAllShapesDeselected(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.AllShapesDeselected(this);
	});
}

void reRig::NotifyActiveShapeChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ActiveShapeChanged(this);
	});
}

void reRig::NotifyAllShapeChanged(reRigShape *shape){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ShapeChanged(this, shape);
	});
	
	pPoseChanged = true;
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyConstraintCountChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ConstraintCountChanged(this);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyConstraintChanged(reRigConstraint *constraint){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ConstraintChanged(this, constraint);
	});
	
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyConstraintSelectedChanged(reRigConstraint *constraint){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ConstraintSelectedChanged(this, constraint);
	});
}

void reRig::NotifyAllConstraintsDeselected(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.AllConstraintsDeselected(this);
	});
}

void reRig::NotifyActiveConstraintChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ActiveConstraintChanged(this);
	});
}

void reRig::NotifyAllConstraintChanged(reRigConstraint *constraint){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ConstraintChanged(this, constraint);
	});
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyAllConstraintDofChanged(reRigConstraint *constraint, deColliderConstraint::eDegreesOfFreedom dof){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ConstraintDofChanged(this, constraint, dof);
	});
	pDirtyRig = true;
	SetChanged(true);
}

void reRig::NotifyPushCountChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.PushCountChanged(this);
	});
}

void reRig::NotifyPushChanged(reRigPush *push){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.PushChanged(this, push);
	});
}

void reRig::NotifyPushSelectedChanged(reRigPush *push){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.PushSelectedChanged(this, push);
	});
}

void reRig::NotifyAllPushesDeselected(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.AllPushesDeselected(this);
	});
}

void reRig::NotifyActivePushChanged(){
	pNotifiers.Visit([&](reRigNotifier &n){
		n.ActivePushChanged(this);
	});
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
	pNotifiers.RemoveAll();
	RemoveAllPushes();
	RemoveAllConstraints();
	RemoveAllShapes();
	RemoveAllBones();
	
	if(pEngSimCollider){
		pEngSimCollider->SetComponent(nullptr);
	}
	if(pEngAnimatorInstance){
		pEngAnimatorInstance->SetComponent(nullptr);
		pEngAnimatorInstance->SetAnimator(nullptr);
	}
	if(pEngAnimator){
		pEngAnimator->SetRig(nullptr);
	}
	if(pEngComponent){
		pEngComponent->SetRig(nullptr);
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
	deModel::Ref model;
	deSkin::Ref skin;
	
	// try to load the model and skin if possible
	try{
		if(!pModelPath.IsEmpty()){
			model = GetEngine()->GetModelManager()->LoadModel(pModelPath, "/");
		}
		
		if(!pSkinPath.IsEmpty()){
			skin = GetEngine()->GetSkinManager()->LoadSkin(pSkinPath, "/");
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	// if the skin is missing use the default one
	if(!skin){
		skin = GetGameDefinition()->GetDefaultSkin();
	}
	
	// reset the animator
	pEngAnimatorInstance->SetComponent(nullptr); // otherwise the animator is not reset
	
	// update the component with the model and skin
	if(model && skin){
		if(pEngComponent){
			rebuildTextures = model != pEngComponent->GetModel();
			pEngComponent->SetModelAndSkin(model, skin);
			
		}else{
			rebuildTextures = true;
			pEngComponent = GetEngine()->GetComponentManager()->CreateComponent(model, skin);
			pEngWorld->AddComponent(pEngComponent);
		}
		
	}else if(pEngComponent){
		pEngSimCollider->SetComponent(nullptr);
		pEngWorld->RemoveComponent(pEngComponent);
		pEngComponent = nullptr;
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
	
	// rebuild textures if required
	if(rebuildTextures){
		pComponentTextures.RemoveAll();
		
		if(pEngComponent->GetModel()){
			const deModel &engModel = pEngComponent->GetModel();
			const int count = engModel.GetTextureCount();
			int i;
			for(i=0; i<count; i++){
				pComponentTextures.Add(reRigTexture::Ref::New(GetEngine(),
					engModel.GetTextureAt(i)->GetName()));
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
	deAnimation::Ref animation;
	
	// try to load the animation if possible
	try{
		if(!pAnimationPath.IsEmpty()){
			animation = GetEngine()->GetAnimationManager()->LoadAnimation(pAnimationPath, "/");
		}
		
	}catch(const deException &e){
		GetLogger()->LogException(LOGSOURCE, e);
	}
	
	// set animation
	pEngAnimator->SetAnimation(animation);
	
	pUpdateAnimatorMove();
}

void reRig::pUpdateAnimatorMove(){
	pEngAnimatorAnim->SetMoveName(pMoveName);
	pEngAnimator->NotifyRulesChanged();
	
	// try to locate the move
	const deAnimation *animation = pEngAnimator->GetAnimation();
	deAnimationMove *move = nullptr;
	
	if(animation){
		const int index = animation->FindMove(pMoveName);
		if(index != -1){
			move = animation->GetMove(index);
		}
	}
	
	// if there is a move get the playtime and adjust the controller
	pPlayTime = move ? move->GetPlaytime() : 0.0f;
	
	pEngAnimatorInstance->GetControllerAt(0).SetValueRange(0.0f, pPlayTime);
	pEngAnimatorInstance->NotifyControllerChangedAt(0);
}

void reRig::pUpdateAnimatorTime(){
	pEngAnimatorInstance->GetControllerAt(0).SetCurrentValue(pMoveTime);
	pEngAnimatorInstance->NotifyControllerChangedAt(0);
}


void reRig::pBoneShowStateChanged(){
	pBones.Visit([](reRigBone &b){
		b.ShowStateChanged();
	});
}

void reRig::pShapeShowStateChanged(){
	pShapes.Visit([](reRigShape &s){
		s.ShowStateChanged();
	});
}

void reRig::pConstraintShowStateChanged(){
	pConstraints.Visit([](reRigConstraint &c){
		c.ShowStateChanged();
	});
}

void reRig::pPushShowStateChanged(){
	pPushes.Visit([](reRigPush &p){
		p.ShowStateChanged();
	});
}
