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

#include "debpCollider.h"
#include "debpColliderBone.h"
#include "debpColliderBones.h"
#include "debpColliderConstraint.h"
#include "../debpRig.h"
#include "../debpPhysicsBody.h"
#include "../dePhysicsBullet.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../component/debpComponent.h"
#include "../debug/debpDebugDrawerColors.h"
#include "../forcefield/debpForceField.h"
#include "../forcefield/debpFFVortex.h"
#include "../forcefield/debpForceFieldFluctuation.h"
#include "../shape/debpShape.h"
#include "../shape/debpCreateShape.h"
#include "../world/debpWorld.h"
#include "../world/debpCollisionWorld.h"
#include "../visitors/debpCreateBulletShape.h"
#include "../visitors/debpShapeSurface.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpColliderBones
////////////////////////////

// Constructor, destructor
////////////////////////////

debpColliderBones::debpColliderBones(debpCollider &collider, deColliderComponent *engColliderComponent) :
pCollider(collider),
pEngColliderRig(engColliderComponent),
pEngColliderComponent(engColliderComponent),
pRootBone(-1),
pRigMass(0.0f),
pRigInvMass(0.0f)
{
	pCreateBones();
}

debpColliderBones::debpColliderBones(debpCollider &collider, deColliderRig *engColliderRig) :
pCollider(collider),
pEngColliderRig(engColliderRig),
pEngColliderComponent(NULL),
pRootBone(-1),
pRigMass(0.0f),
pRigInvMass(0.0f)
{
	pCreateBones();
}

debpColliderBones::~debpColliderBones(){
	pCleanUp();
}



// Bones
//////////

debpColliderBone *debpColliderBones::GetBoneAt(int index) const{
	return pBones.GetAt(index);
}

bool debpColliderBones::HasBoneAt(int index) const{
	return pBones.GetAt(index).IsNotNull();
}

debpColliderBone *debpColliderBones::GetRootBone() const{
	return pRootBone != -1 ? pBones.GetAt(pRootBone).Pointer() : nullptr;
}

debpColliderBone &debpColliderBones::GetBonePhysicsAt(int index) const{
	return *pBonePhysics.GetAt(index);
}

bool debpColliderBones::HasConstraints() const{
	return pConstraints.IsNotEmpty();
}

bool debpColliderBones::HasBreakableConstraints() const{
	return pConstraints.HasMatching([](const deColliderConstraint &c){
		return c.GetBreakingThreshold() > 0.01f;
	});
}

deComponent *debpColliderBones::GetComponent() const{
	return pEngColliderComponent ? pEngColliderComponent->GetComponent().Pointer(): nullptr;
}

deRig *debpColliderBones::GetRig() const{
	if(pEngColliderComponent){
		if(pEngColliderComponent->GetComponent()){
			return pEngColliderComponent->GetComponent()->GetRig();
		}
		
	}else{
		return pEngColliderRig->GetRig();
	}
	return nullptr;
}



bool debpColliderBones::CanBonesCollide(int bone1, int bone2) const{
	if(bone1 == bone2){
		return false;
	}
	
	if(!pBones.GetAt(bone1) || !pBones.GetAt(bone2)){
		return false;
	}
	
	for(const auto &c : pBones.GetAt(bone1)->GetConstraints()){
		if(c->GetBpConstraint() && c->GetBpConstraint()->isEnabled() && c->GetConstraint().GetBone() == bone2){
			return false;
		}
	}
	
	for(const auto &c : pBones.GetAt(bone2)->GetConstraints()){
		if(c->GetBpConstraint() && c->GetBpConstraint()->isEnabled() && c->GetConstraint().GetBone() == bone1){
			return false;
		}
	}
	
	return true;
}

void debpColliderBones::SetDynamicWorld(debpCollisionWorld *dynWorld){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->GetPhysicsBody()->SetDynamicsWorld(dynWorld);
		b->GetConstraints().Visit([&](debpColliderConstraint &c){
			c.SetDynamicsWorld(dynWorld);
		});
	});
	
	if(dynWorld){
		// initialize bones. this is required for colliders to work properly if used
		// right before the very first time physics are processed. the initial update
		// requires to do the following steps:
		// - initialize deColliderBone from deComponentBone (otherwise 0-vectors by default)
		// - update bullet AABB
		PrepareForDetection(0.0f);
		
		pBonePhysics.Visit([&](debpColliderBone *b){
			btRigidBody * const rigidBody = b->GetPhysicsBody()->GetRigidBody();
			if(rigidBody){
				dynWorld->updateSingleAabb(rigidBody);
			}
		});
	}
}

bool debpColliderBones::UpdateFromBody(){
	deComponent * const component = GetComponent();
	bool rootBoneChanged = false;
	bool anyBoneChanged = false;
	
	// fetch the information from all bones and apply them to
	// either the collider if the root bone or the bone otherwise.
	
	pBonePhysics.Visit([&](const debpColliderBone *b){
		debpPhysicsBody &phyBody = *b->GetPhysicsBody();
		if(!phyBody.UpdateFromBody()){
			return;
		}
		
		const int boneIndex = b->GetIndex();
		
		deColliderBone &colBone = pEngColliderRig->GetBoneAt(boneIndex);
		colBone.SetPosition(phyBody.GetPosition());
		colBone.SetOrientation(phyBody.GetOrientation());
		colBone.SetLinearVelocity(phyBody.GetLinearVelocity());
		colBone.SetAngularVelocity(phyBody.GetAngularVelocity());
		colBone.UpdateMatrix();
		
		// if this is the root bone mark that it changed
		if(boneIndex == pRootBone){
			rootBoneChanged = true;
		}
		
		// something changed
		anyBoneChanged = true;
	});
	
	// update collider position with the position of the root bone
	if(rootBoneChanged && pRootBone != -1){
		deColliderBone &colBone = pEngColliderRig->GetBoneAt(pRootBone);
		
		//GetBullet()->LogInfoFormat( "root-bone: p=(%f,%f,%f) r=(%f,%f,%f)",
		//	colBone->GetMatrix().GetPosition().x, colBone->GetMatrix().GetPosition().y,
		//	colBone->GetMatrix().GetPosition().z, colBone->GetMatrix().GetEulerAngles().x,
		//	colBone->GetMatrix().GetEulerAngles().y, colBone->GetMatrix().GetEulerAngles().z );
		
		// pm = bcm * cm
		// bcm^-1 * pm = cm
		// NOTE: this only works if the root bone has no parent!
		const decDMatrix matrix(pBones.GetAt(pRootBone)->GetInverseColliderMatrix()
			.QuickMultiply(colBone.GetMatrix()));
		const decDVector newPosition(matrix.GetPosition());
		const decQuaternion newOrientation(matrix.ToQuaternion());
		
		const debpPhysicsBody &phyBody = *pBones.GetAt(pRootBone)->GetPhysicsBody();
		pEngColliderRig->SetPosition(newPosition);
		pEngColliderRig->SetOrientation(newOrientation);
		pEngColliderRig->SetLinearVelocity(phyBody.GetLinearVelocity());
		pEngColliderRig->SetAngularVelocity(phyBody.GetAngularVelocity());
		//GetBullet()->LogInfoFormat( "component: p=(%f,%f,%f) r=(%f,%f,%f)",
		//	pColliderComponent.GetPosition().x, pColliderComponent.GetPosition().y,
		//	pColliderComponent.GetPosition().z,
		//	decMatrix::CreateFromQuaternion( pColliderComponent.GetOrientation() ).GetEulerAngles().x,
		//	decMatrix::CreateFromQuaternion( pColliderComponent.GetOrientation() ).GetEulerAngles().y,
		//	decMatrix::CreateFromQuaternion( pColliderComponent.GetOrientation() ).GetEulerAngles().z );
		
		// root bone is always default
		if(component){
			deComponentBone &compBone = component->GetBoneAt(pRootBone);
			compBone.SetPosition(decVector());
			compBone.SetRotation(decQuaternion());
		}
	}
	
	// update bones only if anything changed
	if(anyBoneChanged){
		// retrieve the inverse matrix for all bones with no parent.
		const decDMatrix &invMatrix = pCollider.GetInverseMatrix();
		deRig * const rig = GetRig();
		
		// fetch the information for all non-root bones and
		// apply them to the component bones.
		pBones.VisitIndexed([&](int i, debpColliderBone *b){
			if(i != pRootBone && b && b->GetColBoneDynamic()){
				deColliderBone &colBone = pEngColliderRig->GetBoneAt(i);
				
			//if(pBones.GetAt(i)->GetDirty() ){
				const int parent = b->GetParent();
				decDMatrix matrix;
				
				if(parent == -1){
					// pm = cmpm * bm * lbm * cm
					// cmpm^-1 * pm = bm * lbm * cm
					// cmpm^-1 * pm * cm^-1 = bm * lbm
					// cmpm^-1 * pm * cm^-1 * lbm^-1 = bm
					matrix = colBone.GetMatrix().QuickMultiply(invMatrix)
						.QuickMultiply(b->GetInverseLocalMatrix());
					
				}else{
					// pm = cmpm * bm * lbm * pfm * cm
					// cmpm^-1 * pm = bm * lbm * pfm * cm
					// cmpm^-1 * pm * cm^-1 = bm * lbm * pfm
					// cmpm^-1 * pm * cm^-1 * pfm^-1 = bm * lbm
					// cmpm^-1 * pm * cm^-1 * pfm^-1 * lbm^-1 = bm
					matrix = colBone.GetMatrix().QuickMultiply(invMatrix);
					if(component){
						matrix = matrix.QuickMultiply(component->GetBoneAt(parent).GetInverseMatrix());
						
					}else if(rig){
						matrix = matrix.QuickMultiply(rig->GetBoneAt(parent)->GetInverseMatrix());
					}
					matrix = matrix.QuickMultiply(b->GetInverseLocalMatrix());
				}
				
				if(component){
					deComponentBone &compBone = component->GetBoneAt(i);
					matrix.Normalize();
					compBone.SetPosition((matrix * -b->GetOffset()).ToVector());
					compBone.SetRotation(matrix.ToQuaternion());
				}
			//}
			}
			
			// make sure the bone matrix is updated
			if(component){
				component->UpdateBoneAt(i);
			}
			/*
			if(i == pRootBone){
				decVector omp(component.GetBoneAt(pRootBone).GetOriginalMatrix().GetPosition());
				decVector omr(component.GetBoneAt(pRootBone).GetOriginalMatrix().GetEulerAngles());
				GetBullet()->LogInfoFormat("cbone-om: p=(%f,%f,%f) r=(%f,%f,%f)", omp.x, omp.y, omp.z, omr.x, omr.y, omr.z);
				decVector bmp(component.GetBoneAt(pRootBone).GetMatrix().GetPosition());
				decVector bmr(component.GetBoneAt(pRootBone).GetMatrix().GetEulerAngles());
				GetBullet()->LogInfoFormat("cbone-bm: p=(%f,%f,%f) r=(%f,%f,%f)", bmp.x, bmp.y, bmp.z, bmr.x, bmr.y, bmr.z);
				decDVector cmp(pBones.GetAt(pRootBone)->GetColliderMatrix().GetPosition());
				decDVector cmr(pBones.GetAt(pRootBone)->GetColliderMatrix().GetEulerAngles());
				GetBullet()->LogInfoFormat("cbone-cm: p=(%f,%f,%f) r=(%f,%f,%f)", cmp.x, cmp.y, cmp.z, cmr.x, cmr.y, cmr.z);
				decVector p1((component.GetMatrix() * component.GetBoneAt(pRootBone).GetMatrix()).GetPosition());
				decVector r1((component.GetMatrix() * component.GetBoneAt(pRootBone).GetMatrix()).GetEulerAngles());
				GetBullet()->LogInfoFormat("cbone1: p=(%f,%f,%f) r=(%f,%f,%f)", p1.x, p1.y, p1.z, r1.x, r1.y, r1.z);
				decVector ep(component.GetBoneAt(pRootBone).GetPosition());
				decVector er(component.GetBoneAt(pRootBone).GetRotation());
				GetBullet()->LogInfoFormat("cbone-err: p=(%f,%f,%f) r=(%f,%f,%f)", ep.x, ep.y, ep.z, er.x, er.y, er.z);
			}
			*/
		});
		
		if(component){
			// first invalidate then validate. this is required since invalidate marks
			// a couple of things dirty we do not know. validate just marks bones valid
			component->InvalidateBones();
			component->ValidateBones();
			
			// internally we know the bones are now valid. no need to prepare them again
			((debpComponent*)component->GetPeerPhysics())->ClearAllBoneDirty();
		}
	}
	
	// result
	return anyBoneChanged;
}
void debpColliderBones::SetAllBonesDirty(){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->SetDirty(true);
	});
}

void debpColliderBones::ActivateDirtyPhysicsBodies(){
	pBonePhysics.Visit([&](debpColliderBone *b){
		if(b->GetDirty() || b->RequiresAutoDirty()){
			b->GetPhysicsBody()->Activate();
			b->SetDirty(false);
		}
	});
}

void debpColliderBones::SetGravity(const decVector &gravity){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->GetPhysicsBody()->SetGravity(gravity);
	});
}

void debpColliderBones::ApplyForceField(const debpForceField &forceField,
float fluctStrength, float fluctDirection){
	const deForceField &engForceField = forceField.GetForceField();
	// TODO check if the force field overlaps the component
	
	// apply acceleration to physics bodies
	const deForceField::eApplicationTypes ffApplyType = engForceField.GetApplicationType();
	const deForceField::eFieldTypes ffFieldType = engForceField.GetFieldType();
	const decDVector &ffpos = engForceField.GetPosition();
	const decVector &ffDir = forceField.GetDirection();
	const float ffFluctuationDirection = engForceField.GetFluctuationDirection();
	const float ffFluctuationForce = engForceField.GetFluctuationForce();
	const float ffRadius = engForceField.GetRadius();
	const float ffRadiusSquared = ffRadius * ffRadius;
	const float ffExponent = engForceField.GetExponent();
	const float ffForce = engForceField.GetForce();
	const float flucAngle = DEG2RAD * 180.0f;
	float distance;
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		debpPhysicsBody &phyBody = *b->GetPhysicsBody();
		
		decVector direction(phyBody.GetPosition() - ffpos);
		const float distanceSquared = direction.LengthSquared();
		if(distanceSquared >= ffRadiusSquared){
			return;
		}
		
		float forceFactor = 0.0f;
		
		switch(ffApplyType){
		case deForceField::eatDirect:
			forceFactor = pEngColliderRig->GetForceFieldDirect();
			break;
			
		case deForceField::eatSurface:
			// use half the surface area as estimation. this is a good enough approximation
			// since for convex objects half their surface faces a given direction
			forceFactor = pEngColliderRig->GetForceFieldSurface() * phyBody.GetShapeSurface() * 0.5f;
			break;
			
		case deForceField::eatMass:
			forceFactor = pEngColliderRig->GetForceFieldMass() * phyBody.GetMass();
			break;
			
		case deForceField::eatSpeed:
			forceFactor = pEngColliderRig->GetForceFieldSpeed() * phyBody.GetLinearVelocity().Length();
			break;
		}
		
		if(ffFieldType == deForceField::eftLinear){
			distance = sqrtf(distanceSquared);
			direction = ffDir;
			
		}else{
			if(distanceSquared < FLOAT_SAFE_EPSILON){
				return;
			}
			distance = sqrtf(distanceSquared);
			direction *= 1.0f - distance / ffRadius;
		}
		
		const float addForce = (ffForce + ffFluctuationForce * fluctStrength)
			* powf( 1.0f - distance / ffRadius, ffExponent ) * forceFactor;
		
		const decMatrix flucmat(decMatrix::CreateRotationY(
			ffFluctuationDirection * fluctDirection * flucAngle));
		
		phyBody.ApplyForce(flucmat.TransformNormal(direction) * addForce);
		//phyBody.ApplyTorque( addtorque ); // += direction * addtorque;
	});
}

void debpColliderBones::UpdateFromKinematic(bool resetInterpolation){
	const bool dynamic = pEngColliderRig->GetResponseType() == deCollider::ertDynamic;
	if(dynamic){
		return;
	}
	
	const decDMatrix &colMatrix = pCollider.GetMatrix();
	
	deComponent * const component = GetComponent();
	if(component){
		pBones.VisitIndexed([&](int i, debpColliderBone *b){
			if(!b){
				return;
			}
			
			const decDMatrix boneMatrix(component->GetBoneAt(i).GetMatrix().QuickMultiply(colMatrix));
			debpPhysicsBody &phybody = *b->GetPhysicsBody();
			phybody.SetPosition(boneMatrix.GetPosition());
			phybody.SetOrientation(boneMatrix.ToQuaternion());
			if(resetInterpolation){
				phybody.ResetKinematicInterpolation();
			}
		});
		
	}else{
		deRig * const rig = GetRig();
		if(rig){
			pBones.VisitIndexed([&](int i, debpColliderBone *b){
				if(!b){
					return;
				}
				
				const decDMatrix boneMatrix(rig->GetBoneAt(i)->GetMatrix().QuickMultiply(colMatrix));
				debpPhysicsBody &phybody = *b->GetPhysicsBody();
				phybody.SetPosition(boneMatrix.GetPosition());
				phybody.SetOrientation(boneMatrix.ToQuaternion());
				if(resetInterpolation){
					phybody.ResetKinematicInterpolation();
				}
			});
		}
	}
}

void debpColliderBones::PrepareForDetection(float elapsed){
	deComponent * const component = GetComponent();
	const bool dynamic = pEngColliderRig->GetResponseType() == deCollider::ertDynamic;
	const deRig * const rig = component ? component->GetRig() : pEngColliderRig->GetRig();
	const float factor = elapsed > 1e-6f ? 1.0f / elapsed : 0.0f;
	const decDMatrix &colMatrix = pCollider.GetMatrix();
	
	pPreparePhyBones();
	
	// deColliderBone of non-physics-body bones are never use. skip them, faster
	pBonePhysics.Visit([&](const debpColliderBone *b){
		if(dynamic && b->GetColBoneDynamic()){
			return;
		}
		
		const int boneIndex = b->GetIndex();
		deColliderBone &colbone = pEngColliderRig->GetBoneAt(boneIndex);
		
		decDMatrix goalMatrix;
		if(rig){
			goalMatrix.SetTranslation(rig->GetBoneAt(boneIndex)->GetCentralMassPoint());
		}
		if(component){
			goalMatrix = goalMatrix.QuickMultiply(component->GetBoneAt(boneIndex).GetMatrix());
		}else if(rig){
			goalMatrix = goalMatrix.QuickMultiply(rig->GetBoneAt(boneIndex)->GetMatrix());
		}
		goalMatrix = goalMatrix.QuickMultiply(colMatrix);
		
		const decDMatrix diffMatrix(goalMatrix.QuickMultiply(
			decDMatrix::CreateWorld(colbone.GetPosition(), colbone.GetOrientation()).QuickInvert()));
		
		colbone.SetPosition(goalMatrix.GetPosition());
		colbone.SetOrientation(goalMatrix.ToQuaternion());
		colbone.UpdateMatrix();
		
		colbone.SetLinearVelocity(diffMatrix.GetPosition().ToVector() * factor);
		colbone.SetAngularVelocity(diffMatrix.GetEulerAngles().ToVector() * factor);
	});
}

void debpColliderBones::PrepareConstraintsForStep(){
	pBonePhysics.Visit([&](const debpColliderBone *b){
		b->GetConstraints().Visit([&](debpColliderConstraint &c){
			c.PrepareForStep();
		});
	});
}
void debpColliderBones::CheckConstraintsBroke(){
	deRig * const rig = GetRig();
	if(!rig){
		return;
	}
	
	pBonePhysics.Visit([&](const debpColliderBone *b){
		b->GetConstraints().Visit([&](debpColliderConstraint &c){
			if(!c.CheckHasBroken()){
				return;
			}
			
			const int constraintIndex = c.GetRigBoneConstraintIndex();
			const int boneIndex = b->GetIndex();
			
			pEngColliderRig->GetPeerScripting()->RigConstraintBroke(
				pEngColliderRig, boneIndex, constraintIndex,
				rig->GetBoneAt(boneIndex)->GetConstraintAt(constraintIndex));
		});
	});
}


void debpColliderBones::UpdatePhyBodyAABBs(){
	pBonePhysics.Visit([&](const debpColliderBone *b){
		b->GetPhysicsBody()->UpdateAABB();
	});
}

void debpColliderBones::UpdateShapes(){
	const decDMatrix &matrix = pCollider.GetMatrix();
	
	// in the dynamic case the bone state is taken from the collider bones
	if(pEngColliderRig->GetResponseType() == deCollider::ertDynamic){
		pBonePhysics.Visit([&](debpColliderBone *b){
			b->GetShapes().UpdateWithMatrix(
				decDMatrix::CreateTranslation(b->GetOffset()).QuickMultiply(
					pEngColliderRig->GetBoneAt(b->GetIndex())->GetMatrix()));
		});
		
	// for the static and kinematic case taking the bone state from the collider
	// bones does not work. here we have to take the bone states from the component
	// bones themselves for the time being. this is not a proper solution yet since
	// this way no velocities are considered. for this we have to use the collider
	// bones as there the velocities can be stored. they are though in the kinematic
	// and static case not calculated by the scripts so we would have to do this on
	// our own most probably in the prepare detect call.
	}else{
		deComponent * const component = GetComponent();
		deRig * const rig = GetRig();
		if(!rig){
			return;
		}
		
		if(component){
			component->PrepareMatrix();
		}
		pPreparePhyBones();
		
		if(component){
			pBonePhysics.Visit([&](debpColliderBone *b){
				b->GetShapes().UpdateWithMatrix(
					decDMatrix(component->GetBoneAt(b->GetIndex()).GetMatrix())
					.QuickMultiply(matrix));
			});
			
		}else if(rig){
			pBonePhysics.Visit([&](debpColliderBone *b){
				b->GetShapes().UpdateWithMatrix(
					decDMatrix(rig->GetBoneAt(b->GetIndex())->GetMatrix())
					.QuickMultiply(matrix));
			});
		}
	}
}

void debpColliderBones::UpdateShapes(const decDMatrix &transformation){
	if(pBones.IsEmpty()){
		return;
	}
	
	// in the dynamic case the bone state is taken from the collider bones
	if(pCollider.GetCollider().GetResponseType() == deCollider::ertDynamic){
		const decDMatrix tmat(pCollider.GetInverseMatrix().QuickMultiply(transformation));
		
		pBonePhysics.Visit([&](debpColliderBone *b){
			b->GetShapes().UpdateWithMatrix(
				decDMatrix::CreateTranslation(b->GetOffset())
				.QuickMultiply(pEngColliderRig->GetBoneAt(b->GetIndex())->GetMatrix()
					.QuickMultiply(tmat)));
		});
		
	// for the static and kinematic case taking the bone state from the collider
	// bones does not work. here we have to take the bone states from the component
	// bones themselves for the time being. this is not a proper solution yet since
	// this way no velocities are considered. for this we have to use the collider
	// bones as there the velocities can be stored. they are though in the kinematic
	// and static case not calculated by the scripts so we would have to do this on
	// our own most probably in the prepate detect call.
	}else{
		deComponent * const component = GetComponent();
		deRig * const rig = GetRig();
		if(!rig){
			return;
		}
		
		if(component){
			component->PrepareMatrix();
		}
		
		pPreparePhyBones();
		
		if(component){
			pBonePhysics.Visit([&](debpColliderBone *b){
				b->GetShapes().UpdateWithMatrix(
					decDMatrix(component->GetBoneAt(b->GetIndex()).GetMatrix())
					.QuickMultiply(transformation));
			});
			
		}else if(rig){
			pBonePhysics.Visit([&](debpColliderBone *b){
				b->GetShapes().UpdateWithMatrix(
					decDMatrix(rig->GetBoneAt(b->GetIndex())->GetMatrix())
					.QuickMultiply(transformation));
			});
		}
	}
}



void debpColliderBones::CreateDebugDrawers(){
	const debpWorld * const world = pCollider.GetParentWorld();
	
	deDebugDrawerManager &ddmanager = *pCollider.GetBullet()->GetGameEngine()->GetDebugDrawerManager();
	
	// ensure the debug drawers exists
	pBonePhysics.Visit([&](debpColliderBone *b){
		if(b->GetDebugDrawer()){
			return;
		}
		
		const deColliderBone &colbone = pEngColliderRig->GetBoneAt(b->GetIndex());
		deDebugDrawer::Ref boneDebugDrawer(ddmanager.CreateDebugDrawer());
		boneDebugDrawer->SetXRay(true);
		boneDebugDrawer->SetPosition(colbone.GetPosition());
		boneDebugDrawer->SetOrientation(colbone.GetOrientation());
		boneDebugDrawer->SetScale(pGetColliderScale());
		b->SetDebugDrawer(boneDebugDrawer);
		if(world){
			world->GetWorld().AddDebugDrawer(boneDebugDrawer);
		}
	});
	
	// show shapes if layer mask matches
	bool requiresUpdateShapes = false;
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		if(b->GetDDSShape()){
			return;
		}
		
		auto boneDDSShape = deDebugDrawerShape::Ref::New();
		b->SetDDSShape(boneDDSShape);
		b->GetDebugDrawer()->AddShape(std::move(boneDDSShape));
		requiresUpdateShapes = true;
	});
	
	if(requiresUpdateShapes){
		UpdateDebugDrawersShape();
	}
}
void debpColliderBones::DestroyDebugDrawers(){
	const debpWorld * const world = pCollider.GetParentWorld();
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		if(!b->GetDebugDrawer()){
			return;
		}
		
		if(world){
			world->GetWorld().RemoveDebugDrawer(b->GetDebugDrawer());
		}
		b->SetDDSShape(nullptr);
		b->SetDebugDrawer(nullptr);
	});
}

void debpColliderBones::UpdateDebugDrawersShape(){
	deDebugDrawerShape * const ddshape = pCollider.GetDDSShape();
	if(!ddshape){
		return;
	}
	
	const deRig * const rig = GetRig();
	if(!rig){
		return;
	}
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		if(b->GetDDSShape()){
			b->GetDDSShape()->GetShapeList() = rig->GetBoneAt(b->GetIndex())->GetShapes();
		}
	});
}

void debpColliderBones::UpdateDebugDrawers(){
	pBonePhysics.Visit([&](debpColliderBone *b){
		deDebugDrawer * const ddrawer = b->GetDebugDrawer();
		if(!ddrawer){
			return;
		}
		
		const deColliderBone &colbone = pEngColliderRig->GetBoneAt(b->GetIndex());
		const decDMatrix matrix(decDMatrix::CreateFromQuaternion(colbone.GetOrientation()));
		const decVector scale(pGetColliderScale());
		
		ddrawer->SetPosition(colbone.GetPosition() - (matrix * b->GetOffset()).Multiply(scale));
		ddrawer->SetOrientation(colbone.GetOrientation());
		ddrawer->SetScale(scale);
		
		const int highlightResponseType = pCollider.GetBullet()->GetDeveloperMode().GetHighlightResponseType();
		deDebugDrawerShape &ddshape = *b->GetDDSShape();
		decColor colorFill(ddshape.GetFillColor());
		decColor colorEdge(ddshape.GetEdgeColor());
		
		if(highlightResponseType != -1){
			if(pEngColliderRig->GetResponseType() == (deCollider::eResponseType)highlightResponseType){
				colorFill = debpDebugDrawerColors::colliderBoneFill;
				colorEdge = debpDebugDrawerColors::colliderBoneEdge;
				
			}else{
				colorFill = debpDebugDrawerColors::colliderBoneLowFill;
				colorEdge = debpDebugDrawerColors::colliderBoneLowEdge;
			}
			
		}else{
			colorFill = debpDebugDrawerColors::colliderBoneFill;
			colorEdge = debpDebugDrawerColors::colliderBoneEdge;
		}
		
		if(!colorFill.IsEqualTo(ddshape.GetFillColor()) || !colorEdge.IsEqualTo(ddshape.GetEdgeColor())){
			ddshape.SetFillColor(colorFill);
			ddshape.SetEdgeColor(colorEdge);
			ddrawer->NotifyShapeColorChanged();
		}
	});
}


void debpColliderBones::EnableConstraint(int bone, int constraint, bool enable){
	if(bone < 0 || bone >= pBones.GetCount() || !pBones.GetAt(bone)){
		return;
	}
	
	deRig * const rig = GetRig();
	if(!rig || constraint < 0 || constraint >= rig->GetBoneAt(bone)->GetConstraintCount()){
		return;
	}
	
	// the rig constraints are not always existing. if they are invalid for some reason they
	// are skipped. the index thus can not be used directly to access the constraint. instead
	// it has to be searched
	const int count = pBones.GetAt(bone)->GetConstraintCount();
	int i;
	
	for(i=0; i<count; i++){
		const debpColliderConstraint &cc = *pBones.GetAt(bone)->GetConstraintAt(i);
		if(cc.GetRigBoneConstraintIndex() == i && cc.GetBpConstraint()){
			cc.GetBpConstraint()->setEnabled(enable);
			break;
		}
	}
}



void debpColliderBones::ApplyImpuls(const decVector &impuls){
	if(impuls.IsZero()){
		return;
	}
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		debpPhysicsBody &phybody = *b->GetPhysicsBody();
		phybody.ApplyImpuls(impuls * (pRigInvMass * phybody.GetMass()));
	});
}

void debpColliderBones::ApplyImpulsAt(const decVector &impuls, const decVector &position){
	if(impuls.IsZero()){
		return;
	}
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		debpPhysicsBody &phybody = *b->GetPhysicsBody();
		phybody.ApplyImpulsAt(impuls * (pRigInvMass * phybody.GetMass()), position);
	});
}

void debpColliderBones::ApplyTorqueImpuls(const decVector &torqueImpuls){
	if(torqueImpuls.IsZero()){
		return;
	}
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		debpPhysicsBody &phybody = *b->GetPhysicsBody();
		phybody.ApplyTorqueImpuls(torqueImpuls * (pRigInvMass * phybody.GetMass()));
	});
}

void debpColliderBones::ApplyForce(const decVector &force){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->GetPhysicsBody()->ApplyForce(force);
	});
}

void debpColliderBones::ApplyForceAt(const decVector &force, const decVector &point){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->GetPhysicsBody()->ApplyForceAt(force, point);
	});
}

void debpColliderBones::ApplyTorque(const decVector &torque){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->GetPhysicsBody()->ApplyTorque(torque);
	});
}



void debpColliderBones::LinearVelocityChanged(const decVector &velocity){
	pBonePhysics.Visit([&](debpColliderBone *b){
		if(!b->GetColBoneDynamic()){
			b->GetPhysicsBody()->SetLinearVelocity(velocity);
		}
	});
}

void debpColliderBones::AngularVelocityChanged(const decVector &velocity){
	pBonePhysics.Visit([&](debpColliderBone *b){
		if(!b->GetColBoneDynamic()){
			b->GetPhysicsBody()->SetAngularVelocity(velocity);
		}
	});
}

void debpColliderBones::EnablePhysicsBodies(bool enabled){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->GetPhysicsBody()->SetEnabled(enabled);
		b->GetConstraints().Visit([&](debpColliderConstraint &c){
			c.SetEnabled(enabled);
		});
	});
}

void debpColliderBones::UpdatePhysicsType(deCollider::eResponseType responseType){
	// mark dirty and update dynamic flags first
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->SetDirty(true);
		b->SetColBoneDynamic(pEngColliderRig->GetBoneAt(b->GetIndex())->GetDynamic());
	});
	
	switch(responseType){
	case deCollider::ertStatic:
		pBonePhysics.Visit([&](debpColliderBone *b){
			b->GetPhysicsBody()->SetResponseType(debpPhysicsBody::ertStatic);
		});
		break;
		
	case deCollider::ertKinematic:
		pBonePhysics.Visit([&](debpColliderBone *b){
			b->GetPhysicsBody()->SetResponseType(debpPhysicsBody::ertKinematic);
		});
		break;
		
	case deCollider::ertDynamic:
	default:
		pBonePhysics.Visit([&](debpColliderBone *b){
			b->GetPhysicsBody()->SetResponseType(b->GetColBoneDynamic()
				? debpPhysicsBody::ertDynamic : debpPhysicsBody::ertKinematic);
		});
	}
}


void debpColliderBones::UpdatePhysicsType(deCollider::eResponseType responseType, int bone){
	debpColliderBone * const b = pBones.GetAt(bone);
	if(!b){
		return;
	}
	
	b->SetDirty(true);
	b->SetColBoneDynamic(pEngColliderRig->GetBoneAt(bone)->GetDynamic());
	
	switch(responseType){
	case deCollider::ertStatic:
		b->GetPhysicsBody()->SetResponseType(debpPhysicsBody::ertStatic);
		break;
		
	case deCollider::ertKinematic:
		b->GetPhysicsBody()->SetResponseType(debpPhysicsBody::ertKinematic);
		break;
		
	case deCollider::ertDynamic:
	default:
		b->GetPhysicsBody()->SetResponseType(b->GetColBoneDynamic()
			? debpPhysicsBody::ertDynamic : debpPhysicsBody::ertKinematic);
	}
}

void debpColliderBones::CollisionFilteringChanged(){
	pBonePhysics.Visit([&](debpColliderBone *b){
		b->GetPhysicsBody()->CollisionFilteringChanged();
	});
}



bool debpColliderBones::PointInside(const decDVector &point){
	return pBonePhysics.HasMatching([&](const debpColliderBone *b){
		return b->GetShapes().HasMatching([&](const debpShape &s){
			return s.GetCollisionVolume()->IsPointInside(point);
		});
	});
}

void debpColliderBones::CalcShapeExtends(decDVector &minExtend, decDVector &maxExtend){
	debpDCollisionBox colBox;
	bool hasExtend = false;

	pBonePhysics.Visit([&](debpColliderBone *b){
		const debpShape::List &shapes = b->GetShapes();
		if(shapes.IsEmpty()){
			return;
		}

		shapes.Visit([&](debpShape &s){
			s.GetCollisionVolume()->GetEnclosingBox(&colBox);

			if(hasExtend){
				minExtend.SetSmallest(colBox.GetCenter() - colBox.GetHalfSize());
				maxExtend.SetLargest(colBox.GetCenter() + colBox.GetHalfSize());
				
			}else{
				minExtend = colBox.GetCenter() - colBox.GetHalfSize();
				maxExtend = colBox.GetCenter() + colBox.GetHalfSize();
				hasExtend = true;
			}
		});
	});
	
	if(!hasExtend){
		minExtend.SetZero();
		maxExtend.SetZero();
	}
}



bool debpColliderBones::UpdateStaticCollisionTests(){
	bool hasCShapes = false;
	pBonePhysics.Visit([&](debpColliderBone *b){
		hasCShapes |= b->GetStaticCollisionTestPrepare() != nullptr;
	});
	return hasCShapes;
}


// Private functions
//////////////////////
void debpColliderBones::pCleanUp(){
	debpWorld * const world = pCollider.GetParentWorld();
	if(world){
		pBonePhysics.Visit([&](const debpColliderBone *b){
			if(b->GetDebugDrawer()){
				world->GetWorld().RemoveDebugDrawer(b->GetDebugDrawer());
			}
		});
	}
}

void debpColliderBones::pCreateBones(){
	const deCollider::eResponseType responseType = pEngColliderRig->GetResponseType();
	debpCollisionWorld *dynWorld = pCollider.GetDynamicsWorld();
	const bool enabled = pEngColliderRig->GetEnabled();
	deRig * const rig = GetRig();
	int boneCount = 0;
	
	// check if there is a component and a rig
	if(rig){
		boneCount = rig->GetBoneCount();
		
		const debpRig &bprig = *((debpRig*)rig->GetPeerPhysics());
		pRigMass = bprig.GetMass();
		pRigInvMass = bprig.GetInverseMass();
	}
	
	if(boneCount == 0){
		return;
	}
	
	// scaling
	decVector scale(pGetColliderScale());
	
	// prepare bones
// 	if( component ){
// 		component->PrepareBones(); // useless. we use collider bones not component bones
// 	}
	
	// create bones array and set all bones to NULL
	pBones.EnlargeCapacity(boneCount);
	int i;
	for(i=0; i<boneCount; i++){
		pBones.Add({});
	}
	
	// create the individual bones if they have physics properties
	debpCreateShape createShape;
	
	for(i=0; i<boneCount; i++){
		deColliderBone &colBone = pEngColliderRig->GetBoneAt(i);
		deRigBone &rigBone = rig->GetBoneAt(i);
		
		/*
		if(b == 0){
			colBone.SetDynamic(responseType == deCollider::ertDynamic);
			
		}else{
			colBone.SetDynamic(responseType == deCollider::ertDynamic && rigBone->GetDynamic());
		}
		*/
		colBone.SetDynamic(rigBone.GetDynamic()); // WARNING! this calls BoneDynamicChanged()!
		
		// only create a bone if it has a shape definition. bones without a
		// shape definition are most probably controller bones or used for
		// rendering only and should not clog up the physics system with
		// unused rigid bodies
		if(rigBone.GetShapes().GetCount() == 0){
			continue;
		}
		
		// create collider bone
		pBones.SetAt(i, deTUniqueReference<debpColliderBone>::New(&pCollider, i));
		debpColliderBone * const b = pBones.GetAt(i);
		pBonePhysics.Add(b);
		
		// set the properties of the physics body
		debpPhysicsBody &phyBody = *b->GetPhysicsBody();
		phyBody.SetDynamicsWorld(dynWorld);
		
		phyBody.SetLinearVelocity(colBone.GetLinearVelocity());
		phyBody.SetAngularVelocity(colBone.GetAngularVelocity());
		
		if(pEngColliderRig->GetUseLocalGravity()){
			phyBody.SetGravity(pEngColliderRig->GetGravity());
			
		}else{
			debpWorld * const bpWorld = pCollider.GetParentWorld();
			
			if(bpWorld){
				phyBody.SetGravity(bpWorld->GetWorld().GetGravity());
				
			}else{
				phyBody.SetGravity(decVector());
			}
		}
		
		phyBody.SetEnabled(enabled);
		phyBody.SetMass(rigBone.GetMass());
		
		switch(responseType){
		case deCollider::ertStatic:
			phyBody.SetResponseType(debpPhysicsBody::ertStatic);
			break;
			
		case deCollider::ertKinematic:
			phyBody.SetResponseType(debpPhysicsBody::ertKinematic);
			break;
			
		case deCollider::ertDynamic:
		default:
			if(colBone.GetDynamic()){
				phyBody.SetResponseType(debpPhysicsBody::ertDynamic);
				
			}else{
				phyBody.SetResponseType(debpPhysicsBody::ertKinematic);
			}
		}
		
		phyBody.SetPosition(colBone.GetPosition());
		phyBody.SetOrientation(colBone.GetOrientation());
		
		// init some parameters from the rig bone
		b->SetFromRigBone(rigBone);
		
		// set dynamic flag
		b->SetColBoneDynamic(colBone.GetDynamic());
		
		// initialize from the bone shape if possible
		pSetBoneShape(i, rigBone, scale);
		
		// create the collision shapes
		createShape.Reset();
		rigBone.GetShapes().Visit([&](decShape &s){
			s.Visit(createShape);
			if(createShape.GetCreatedShape()){
				b->GetShapes().Add(createShape.GetCreatedShape());
				createShape.SetCreatedShape(nullptr);
			}
		});
	}
	
	if(rig){
		pCreateConstraints(*rig, scale);
	}
	
	// check if the root bone exists
	if(rig && rig->GetRootBone() != -1 && pBones[rig->GetRootBone()]){
		pRootBone = rig->GetRootBone();
	}
	
	UpdateDebugDrawersShape(); // debug if enabled
}

void debpColliderBones::pSetBoneShape(int index, deRigBone &bone, const decVector &scale){
	if(bone.GetShapes().IsEmpty()){
		return;
	}
	
	debpPhysicsBody *phyBody = pBones[index]->GetPhysicsBody();
	debpCreateBulletShape createBulletShape;
	debpShapeSurface shapeSurface;
	
	createBulletShape.SetOffset(-bone.GetCentralMassPoint().Multiply(scale));
	createBulletShape.SetScale(scale);
	
	bone.GetShapes().VisitIndexed([&](int i, decShape &s){
		createBulletShape.SetShapeIndex(i);
		s.Visit(createBulletShape);
		s.Visit(shapeSurface);
	});
	createBulletShape.Finish();
	
	phyBody->SetShape(createBulletShape.GetBulletShape());
	phyBody->SetShapeSurface(shapeSurface.GetSurface());
	
	//phyBody->SetCcdParameters( createBulletShape.GetCcdThreshold(), createBulletShape.GetCcdRadius() );
}

void debpColliderBones::pCreateConstraints(const deRig &rig, const decVector &scale){
	if(pBonePhysics.IsEmpty()){
		return;
	}
	
	debpCollisionWorld *dynWorld = pCollider.GetDynamicsWorld();
	
	pBonePhysics.Visit([&](debpColliderBone *b){
		deRigBone &rigBone = rig.GetBoneAt(b->GetIndex());
		
		rigBone.GetConstraints().VisitIndexed([&](int rci, const deRigConstraint &rc){
			const int partner = rc.GetParentBone();
			if(partner < 0 || partner >= pBones.GetCount() || !pBones.GetAt(partner)){
				return;
			}
			
			const debpColliderBone &bp = pBones.GetAt(partner);
			
			// add a new temporary bone constraint
			const auto cc = deColliderConstraint::Ref::New();
			pConstraints.Add(cc);
			
			// set the temporary bone constraint from the rig constraint
			decDMatrix bcRotMatrix(decDMatrix::CreateFromQuaternion(rc.GetReferenceOrientation()));
			
			cc->SetPosition1((rc.GetReferencePosition() + bcRotMatrix * rc.GetBoneOffset()));
			cc->SetOrientation1(decQuaternion()/*rigConstraint.GetReferenceOrientation()*/);
			
			decDMatrix bcMatrix = b->GetBoneMatrix().QuickMultiply(bp.GetInverseBoneMatrix());
			
			cc->SetPosition2((bcMatrix * rc.GetReferencePosition()));
			cc->SetOrientation2(bcRotMatrix.QuickMultiply(bcMatrix).ToQuaternion());
			
			cc->GetDofLinearX() = rc.GetDofLinearX();
			if(cc->GetDofLinearX().GetLowerLimit() <= cc->GetDofLinearX().GetUpperLimit()){
				cc->GetDofLinearX().SetLowerLimit(cc->GetDofLinearX().GetLowerLimit() * scale.x);
				cc->GetDofLinearX().SetUpperLimit(cc->GetDofLinearX().GetUpperLimit() * scale.x);
			}
			
			cc->GetDofLinearY() = rc.GetDofLinearY();
			if(cc->GetDofLinearY().GetLowerLimit() <= cc->GetDofLinearY().GetUpperLimit()){
				cc->GetDofLinearY().SetLowerLimit(cc->GetDofLinearY().GetLowerLimit() * scale.y);
				cc->GetDofLinearY().SetUpperLimit(cc->GetDofLinearY().GetUpperLimit() * scale.y);
			}
			
			cc->GetDofLinearZ() = rc.GetDofLinearZ();
			if(cc->GetDofLinearZ().GetLowerLimit() <= cc->GetDofLinearZ().GetUpperLimit()){
				cc->GetDofLinearZ().SetLowerLimit(cc->GetDofLinearZ().GetLowerLimit() * scale.z);
				cc->GetDofLinearZ().SetUpperLimit(cc->GetDofLinearZ().GetUpperLimit() * scale.z);
			}
			
			cc->GetDofAngularX() = rc.GetDofAngularX();
			cc->GetDofAngularY() = rc.GetDofAngularY();
			cc->GetDofAngularZ() = rc.GetDofAngularZ();
			
			cc->SetLinearDamping(rc.GetLinearDamping());
			cc->SetAngularDamping(rc.GetAngularDamping());
			cc->SetSpringDamping(rc.GetSpringDamping());
			
			cc->SetIsRope(rc.GetIsRope());
			cc->SetBreakingThreshold(rc.GetBreakingThreshold());
			
			cc->SetBone(partner);
			
			// create bullet constraint wrapper
			/*
			GetBullet()->LogInfoFormat("collider component create constraint: bone=%s i=%i\n", rigBone->GetName(), c);
			const decDVector &rp1 = b.GetPhysicsBody()->GetPosition();
			GetBullet()->LogInfoFormat(">> rp1 (%g,%g,%g)\n", rp1.x, rp1.y, rp1.z);
			const decDVector &rp2 = pBones[partner]->GetPhysicsBody()->GetPosition();
			GetBullet()->LogInfoFormat(">> rp2 (%g,%g,%g)\n", rp2.x, rp2.y, rp2.z);
			*/
			
			auto bc = deTUniqueReference<debpColliderConstraint>::New(*pCollider.GetBullet(), cc);
			bc->SetRigBoneConstraintIndex(rci);
			
			bc->SetDynamicsWorld(dynWorld);
			bc->SetEnabled(pEngColliderRig->GetEnabled());
			
			bc->SetFirstBody(b->GetPhysicsBody());
			bc->SetFirstOffset(b->GetOffset().ToVector());
			bc->SetSecondBody(bp.GetPhysicsBody());
			bc->SetSecondOffset(bp.GetOffset().ToVector());
			bc->SetScale(scale);
			
			b->AddConstraint(std::move(bc));
		});
	});
}

void debpColliderBones::pPreparePhyBones(){
	// TODO this is very slow. having a bunch of complex models with many
	// bones updated each frame costs up to 10ms and more. a better solution
	// has to be found to handle this situation. most probably requires a
	// hack of some bullet classes to add a late-update call so these
	// kinematic updates are only done if they are really required.
	deComponent * const engComponent = GetComponent();
	if(!engComponent){
		return;
	}
	
// 	engComponent->PrepareBones();
	
	debpComponent &component = *((debpComponent*)engComponent->GetPeerPhysics());
	
	pBonePhysics.Visit([&](const debpColliderBone *b){
		component.PrepareBone(b->GetIndex());
	});
}

decVector debpColliderBones::pGetColliderScale() const{
	decVector scale(pEngColliderRig->GetScale());
	/* collider scale is implcitly applied during physics simulation
	deComponent * const component = GetComponent();
	if(component){
		scale.x *= component->GetScaling().x;
		scale.y *= component->GetScaling().y;
		scale.z *= component->GetScaling().z;
	}
	*/
	return scale;
}