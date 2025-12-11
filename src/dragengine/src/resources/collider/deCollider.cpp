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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deCollider.h"
#include "deColliderManager.h"
#include "deColliderVisitor.h"
#include "deColliderAttachment.h"
#include "deColliderConstraint.h"
#include "deColliderCollisionTest.h"
#include "../../systems/modules/physics/deBasePhysicsCollider.h"
#include "../../systems/modules/scripting/deBaseScriptingCollider.h"
#include "../../common/exceptions.h"



// Class deCollider
/////////////////////

// Constructors and Destructors
/////////////////////////////////

deCollider::deCollider(deColliderManager *manager) :
deResource(manager),

pScale(1.0f, 1.0f, 1.0f),

pMass(1.0f),
pResponseType(ertStatic),
pEnabled(true),
pUseLocalGravity(false),

pForceFieldDirect(0.0f),
pForceFieldSurface(0.0f),
pForceFieldMass(0.0f),
pForceFieldSpeed(0.0f),

pAttachments(NULL),
pAttachmentCount(0),
pAttachmentSize(0),

pPeerPhysics(NULL),
pPeerScripting(NULL),

pParentWorld(NULL),
pLLWorldPrev(NULL),
pLLWorldNext(NULL){
}

deCollider::~deCollider(){
	pCleanUp();
}



// Management
///////////////

void deCollider::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pPeerPhysics){
		pPeerPhysics->PositionChanged();
	}
}

void deCollider::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pPeerPhysics){
		pPeerPhysics->OrientationChanged();
	}
}

void deCollider::SetGeometry(const decDVector &position, const decQuaternion &orientation){
	if(position.IsEqualTo(pPosition) && orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pPosition = position;
	pOrientation = orientation;
	
	if(pPeerPhysics){
		pPeerPhysics->GeometryChanged();
	}
}

void deCollider::SetGeometry(const decDVector &position, const decQuaternion &orientation,
const decVector &scale){
	if(position.IsEqualTo(pPosition) && orientation.IsEqualTo(pOrientation)
	&& scale.IsEqualTo(pScale)){
		return;
	}
	
	pPosition = position;
	pOrientation = orientation;
	pScale = scale;
	
	if(pPeerPhysics){
		pPeerPhysics->GeometryChanged();
	}
}

void deCollider::SetScale(const decVector &scale){
	if(scale.IsEqualTo(pScale)){
		return;
	}
	
	pScale = scale;
	
	if(pPeerPhysics){
		pPeerPhysics->ScaleChanged();
	}
}

void deCollider::SetLinearVelocity(const decVector &velocity){
	if(velocity.IsEqualTo(pLinearVelocity)){
		return;
	}
	
	pLinearVelocity = velocity;
	
	if(pPeerPhysics){
		pPeerPhysics->LinearVelocityChanged();
	}
}

void deCollider::SetAngularVelocity(const decVector &momentum){
	if(momentum.IsEqualTo(pAngularVelocity)){
		return;
	}
	
	pAngularVelocity = momentum;
	
	if(pPeerPhysics){
		pPeerPhysics->AngularVelocityChanged();
	}
}

void deCollider::SetMass(float mass){
	mass = decMath::max(mass, 0.0f);
	if(fabsf(mass - pMass) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMass = mass;
	
	if(pPeerPhysics){
		pPeerPhysics->PropertiesChanged();
	}
}

void deCollider::SetGravity(const decVector &force){
	if(force.IsEqualTo(pGravity)){
		return;
	}
	
	pGravity = force;
	
	if(pPeerPhysics){
		pPeerPhysics->GravityChanged();
	}
}

void deCollider::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(pPeerPhysics){
		pPeerPhysics->EnabledChanged();
	}
}

void deCollider::SetResponseType(eResponseType responseType){
	if(responseType < ertStatic || responseType > ertKinematic){
		DETHROW(deeInvalidParam);
	}
	
	if(pResponseType == responseType){
		return;
	}
	
	pResponseType = responseType;
	
	if(pPeerPhysics){
		pPeerPhysics->ResponseTypeChanged();
	}
}

void deCollider::SetUseLocalGravity(bool useLocalGravity){
	if(pUseLocalGravity == useLocalGravity){
		return;
	}
	
	pUseLocalGravity = useLocalGravity;
	
	if(pPeerPhysics){
		pPeerPhysics->GravityChanged();
	}
}

void deCollider::SetForceFieldDirect(float factor){
	if(fabsf(factor - pForceFieldDirect) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pForceFieldDirect = factor;
	
	if(pPeerPhysics){
		pPeerPhysics->ForceFieldChanged();
	}
}

void deCollider::SetForceFieldSurface(float factor){
	if(fabsf(factor - pForceFieldSurface) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pForceFieldSurface = factor;
	
	if(pPeerPhysics){
		pPeerPhysics->ForceFieldChanged();
	}
}

void deCollider::SetForceFieldMass(float factor){
	if(fabsf(factor - pForceFieldMass) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pForceFieldMass = factor;
	
	if(pPeerPhysics){
		pPeerPhysics->ForceFieldChanged();
	}
}

void deCollider::SetForceFieldSpeed(float factor){
	if(fabsf(factor - pForceFieldSpeed) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pForceFieldSpeed = factor;
	
	if(pPeerPhysics){
		pPeerPhysics->ForceFieldChanged();
	}
}

void deCollider::SetCollisionFilter(const decCollisionFilter &filter){
	if(filter == pCollisionFilter){
		return;
	}
	
	pCollisionFilter = filter;
	
	if(pPeerPhysics){
		pPeerPhysics->CollisionFilterChanged();
	}
}



// Attachments
////////////////

deColliderAttachment *deCollider::GetAttachmentAt(int index) const{
	if(index < 0 || index >= pAttachmentCount){
		DETHROW(deeInvalidParam);
	}
	
	return pAttachments[index];
}

bool deCollider::HasAttachmentWith(deResource *resource) const{
	if(!resource){
		DETHROW(deeInvalidParam);
	}
	int i;
	
	for(i=0; i<pAttachmentCount; i++){
		if(pAttachments[i]->GetResource() == resource){
			return true;
		}
	}
	
	return false;
}

deColliderAttachment *deCollider::GetAttachmentWith(deResource *resource) const{
	if(!resource){
		DETHROW(deeInvalidParam);
	}
	int i;
	
	for(i=0; i<pAttachmentCount; i++){
		if(pAttachments[i]->GetResource() == resource){ 
			return pAttachments[i];
		}
	}
	return NULL;
}

void deCollider::AddAttachment(deColliderAttachment *attachment){
	if(!attachment || HasAttachmentWith(attachment->GetResource())){
		DETHROW(deeInvalidParam);
	}
	
	if(pAttachmentCount == pAttachmentSize){
		int i, newSize = pAttachmentSize * 3 / 2 + 1;
		deColliderAttachment **newArray = new deColliderAttachment*[newSize];
		
		if(pAttachments){
			for(i=0; i<pAttachmentSize; i++) newArray[i] = pAttachments[i];
			delete [] pAttachments;
		}
		pAttachments = newArray;
		pAttachmentSize = newSize;
	}
	
	pAttachments[pAttachmentCount] = attachment;
	pAttachmentCount++;
	
	if(pPeerPhysics){
		pPeerPhysics->AttachmentAdded(pAttachmentCount - 1, attachment);
	}
}

void deCollider::RemoveAttachment(deColliderAttachment *attachment){
	int i, index = pFindAttachment(attachment);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	for(i=index+1; i<pAttachmentCount; i++){
		pAttachments[i - 1] = pAttachments[i];
	}
	pAttachmentCount--;
	
	if(pPeerPhysics){
		pPeerPhysics->AttachmentRemoved(index, attachment);
	}
	
	delete attachment;
}

void deCollider::RemoveAllAttachments(){
	if(pPeerPhysics){
		pPeerPhysics->AllAttachmentsRemoved();
	}
	
	while(pAttachmentCount > 0){
		delete pAttachments[pAttachmentCount - 1];
		pAttachmentCount--;
	}
}

void deCollider::NotifyAttachmentChanged(int index){
	if(index < 0 || index >= pAttachmentCount){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerPhysics){
		pPeerPhysics->AttachmentChanged(index, pAttachments[index]);
	}
}

void deCollider::AttachmentsForceUpdate(){
	if(pPeerPhysics){
		pPeerPhysics->AttachmentsForceUpdate();
	}
}



// Forces
///////////

void deCollider::ApplyImpuls(const decVector &impuls){
	if(pPeerPhysics){
		pPeerPhysics->ApplyImpuls(impuls);
	}
}

void deCollider::ApplyImpulsAt(const decVector &impuls, const decVector &point){
	if(pPeerPhysics){
		pPeerPhysics->ApplyImpulsAt(impuls, point);
	}
}

void deCollider::ApplyTorqueImpuls(const decVector &torqueImpuls){
	if(pPeerPhysics){
		pPeerPhysics->ApplyTorqueImpuls(torqueImpuls);
	}
}

void deCollider::ApplyForce(const decVector &force){
	if(pPeerPhysics){
		pPeerPhysics->ApplyForce(force);
	}
}

void deCollider::ApplyForceAt(const decVector &force, const decVector &point){
	if(pPeerPhysics){
		pPeerPhysics->ApplyForceAt(force, point);
	}
}

void deCollider::ApplyTorque(const decVector &torque){
	if(pPeerPhysics){
		pPeerPhysics->ApplyTorque(torque);
	}
}



// Constraints
////////////////

int deCollider::GetConstraintCount() const{
	return pConstraints.GetCount();
}

deColliderConstraint *deCollider::GetConstraintAt(int index) const{
	return (deColliderConstraint*)pConstraints.GetAt(index);
}

int deCollider::IndexOfConstraint(deColliderConstraint *constraint) const{
	return pConstraints.IndexOf(constraint);
}

bool deCollider::HasConstraint(deColliderConstraint *constraint) const{
	DEASSERT_NOTNULL(constraint)
	return pConstraints.Has(constraint);
}

void deCollider::AddConstraint(deColliderConstraint *constraint){
	pConstraints.Add(constraint);
	
	if(pPeerPhysics){
		pPeerPhysics->ConstraintAdded(pConstraints.GetCount() - 1, constraint);
	}
}

void deCollider::RemoveConstraint(deColliderConstraint *constraint){
	int index = pConstraints.IndexOf(constraint);
	DEASSERT_FALSE(index == -1)
	
	if(pPeerPhysics){
		pPeerPhysics->ConstraintRemoved(index, constraint);
	}
	
	pConstraints.RemoveFrom(index);
}

void deCollider::RemoveAllConstraints(){
	if(pConstraints.GetCount() == 0){
		return;
	}
	
	if(pPeerPhysics){
		pPeerPhysics->AllConstraintsRemoved();
	}
	
	pConstraints.RemoveAll();
}

void deCollider::NotifyConstraintChanged(int index){
	deColliderConstraint * const constraint =
		static_cast<deColliderConstraint*>(pConstraints.GetAt(index));
	
	if(pPeerPhysics){
		pPeerPhysics->ConstraintChanged(index, constraint);
	}
}



// Colliders to ignore
////////////////////////

int deCollider::GetIgnoreColliderCount() const{
	return pIgnoreColliders.GetCount();
}

deCollider *deCollider::GetIgnoreColliderAt(int index) const{
	return (deCollider*)pIgnoreColliders.GetAt(index);
}

bool deCollider::HasIgnoreCollider(deCollider *collider) const{
	return pIgnoreColliders.Has(collider);
}

void deCollider::AddIgnoreCollider(deCollider *collider){
	pIgnoreColliders.Add(collider);
	
	if(pPeerPhysics){
		pPeerPhysics->IgnoreCollidersChanged();
	}
}

void deCollider::RemoveIgnoreCollider(deCollider *collider){
	pIgnoreColliders.Remove(collider);
	
	if(pPeerPhysics){
		pPeerPhysics->IgnoreCollidersChanged();
	}
}

void deCollider::RemoveAllIgnoreColliders(){
	pIgnoreColliders.RemoveAll();
	
	if(pPeerPhysics){
		pPeerPhysics->IgnoreCollidersChanged();
	}
}



// Post physics collision tests
/////////////////////////////////

int deCollider::GetCollisionTestCount() const{
	return pCollisionTests.GetCount();
}

deColliderCollisionTest *deCollider::GetCollisionTestAt(int index) const{
	return (deColliderCollisionTest*)pCollisionTests.GetAt(index);
}

int deCollider::IndexOfCollisionTest(deColliderCollisionTest *collisionTest) const{
	return pCollisionTests.IndexOf(collisionTest);
}

bool deCollider::HasCollisionTest(deColliderCollisionTest *collisionTest) const{
	return pCollisionTests.Has(collisionTest);
}

void deCollider::AddCollisionTest(deColliderCollisionTest *collisionTest){
	pCollisionTests.Add(collisionTest);
	
	if(pPeerPhysics){
		pPeerPhysics->CollisionTestAdded(pCollisionTests.GetCount() - 1);
	}
}

void deCollider::RemoveCollisionTest(deColliderCollisionTest *collisionTest){
	const int index = pCollisionTests.IndexOf(collisionTest);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	pCollisionTests.Remove(collisionTest);
	
	if(pPeerPhysics){
		pPeerPhysics->CollisionTestRemoved(index);
	}
}

void deCollider::RemoveAllCollisionTests(){
	if(pPeerPhysics){
		pPeerPhysics->AllCollisionTestsRemoved();
	}
	pCollisionTests.RemoveAll();
}

void deCollider::NotifyCollisionTestChanged(int index){
	if(index < 0 || index >= pCollisionTests.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerPhysics){
		pPeerPhysics->CollisionTestChanged(index);
	}
}

void deCollider::NotifyCollisionTestEnableChanged(int index){
	if(index < 0 || index >= pCollisionTests.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerPhysics){
		pPeerPhysics->CollisionTestEnabledChanged(index);
	}
}



// Collision Detection
////////////////////////

bool deCollider::PointInside(const decDVector &point){
	if(!pPeerPhysics){
		return false;
	}
	return pPeerPhysics->PointInside(point);
}

void deCollider::RayHits(const decDVector &rayOrigin, const decVector &rayDirection, deBaseScriptingCollider *listener){
	if(!pPeerPhysics){
		return;
	}
	pPeerPhysics->RayHits(rayOrigin, rayDirection, listener);
}

void deCollider::ColliderHits(deCollider *collider, deBaseScriptingCollider *listener){
	if(!pPeerPhysics){
		return;
	}
	pPeerPhysics->ColliderHits(collider, listener);
}

void deCollider::ColliderMoveHits(deCollider *collider, const decVector &displacement, deBaseScriptingCollider *listener){
	if(!pPeerPhysics){
		return;
	}
	pPeerPhysics->ColliderMoveHits(collider, displacement, listener);
}

void deCollider::ColliderRotateHits(deCollider *collider, const decVector &rotation, deBaseScriptingCollider *listener){
	if(!pPeerPhysics){
		return;
	}
	pPeerPhysics->ColliderRotateHits(collider, rotation, listener);
}

void deCollider::ColliderMoveRotateHits(deCollider *collider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener){
	if(!pPeerPhysics){
		return;
	}
	pPeerPhysics->ColliderMoveRotateHits(collider, displacement, rotation, listener);
}



// System Peers
/////////////////

void deCollider::SetPeerPhysics(deBasePhysicsCollider *peer){
	if(peer == pPeerPhysics){
		return;
	}
	
	if(pPeerPhysics){
		delete pPeerPhysics;
	}
	
	pPeerPhysics = peer;
}

void deCollider::SetPeerScripting(deBaseScriptingCollider *peer){
	if(peer == pPeerScripting){
		return;
	}
	
	if(pPeerScripting){
		delete pPeerScripting;
	}
	
	pPeerScripting = peer;
}



// Visiting
/////////////

void deCollider::Visit(deColliderVisitor &visitor){
	visitor.VisitCollider(*this);
}



// Linked List
////////////////

void deCollider::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deCollider::SetLLWorldPrev(deCollider *collider){
	pLLWorldPrev = collider;
}

void deCollider::SetLLWorldNext(deCollider *collider){
	pLLWorldNext = collider;
}



// Private Functions
//////////////////////

void deCollider::pCleanUp(){
	if(pPeerScripting){
		delete pPeerScripting;
		pPeerScripting = NULL;
	}
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	
	RemoveAllCollisionTests();
	RemoveAllConstraints();
	
	RemoveAllAttachments();
	if(pAttachments){
		delete [] pAttachments;
	}
}

int deCollider::pFindAttachment(deColliderAttachment *attachment){
	int i;
	for(i=0; i<pAttachmentCount; i++){
		if(pAttachments[i] == attachment) return i;
	}
	return -1;
}



void deCollider::pNotifyCollisionVolumeChanged(){
	if(pPeerPhysics){
		pPeerPhysics->CollisionVolumeChanged();
	}
}



void deCollider::pNotifyRigChanged(){
	if(pPeerPhysics){
		pPeerPhysics->RigChanged();
	}
}



void deCollider::pNotifyComponentChanged(){
	if(pPeerPhysics){
		pPeerPhysics->ComponentChanged();
	}
}



void deCollider::pNotifyBonePositionChanged(int index){
	if(pPeerPhysics){
		pPeerPhysics->BonePositionChanged(index);
	}
}

void deCollider::pNotifyBoneOrientationChanged(int index){
	if(pPeerPhysics){
		pPeerPhysics->BoneOrientationChanged(index);
	}
}

void deCollider::pNotifyBoneLinearVelocityChanged(int index){
	if(pPeerPhysics){
		pPeerPhysics->BoneLinearVelocityChanged(index);
	}
}

void deCollider::pNotifyBoneAngularVelocityChanged(int index){
	if(pPeerPhysics){
		pPeerPhysics->BoneAngularVelocityChanged(index);
	}
}

void deCollider::pNotifyBonePropertiesChanged(int index){
	if(pPeerPhysics){
		pPeerPhysics->BonePropertiesChanged(index);
	}
}

void deCollider::pNotifyBoneDynamicChanged(int index){
	if(pPeerPhysics){
		pPeerPhysics->BoneDynamicChanged(index);
	}
}
