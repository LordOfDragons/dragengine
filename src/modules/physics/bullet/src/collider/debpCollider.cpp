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

#include "debpColliderAttachment.h"
#include "debpColliderConstraint.h"
#include "debpCollider.h"
#include "debpColliderCollisionTest.h"
#include "../dePhysicsBullet.h"
#include "../coldet/debpCollisionDetection.h"
#include "../coldet/debpConvexResultCallback.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../debug/debpDebugDrawerColors.h"
#include "../devmode/debpDeveloperMode.h"
#include "../touchsensor/debpTouchSensor.h"
#include "../world/debpWorld.h"

#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderCollisionTest.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/deEngine.h>



// Class debpCollider
///////////////////////

// Constructor, destructor
////////////////////////////

debpCollider::debpCollider(dePhysicsBullet *bullet, deCollider &collider, int type) :
pCollider(collider)
{
	if(!bullet || type < ectVolume || type > ectRig) DETHROW(deeInvalidParam);
	int i, count;
	
	pType = type;
	pBullet = bullet;
	
	pParentWorld = NULL;
	pIndex = -1;
	
	pAttachments = NULL;
	pAttachmentCount = 0;
	pAttachmentSize = 0;
	
	pConstraints = NULL;
	pConstraintCount = 0;
	pConstraintSize = 0;
	
	pIsMoving = false;
	pDirtyMatrix = true;
	pDirtyOctree = true;
	pRequiresUpdate = false;
	pUseKinematicSim = false;
	pIsPrepared = false;
	
	pMarked = false;
	pTouchSensorMarked = false;
	
	pColDetPrepareIndex = -1;
	pAutoColDetPrepare = false;
	pColDetFinishIndex = -1;
	pAutoColDetFinish = false;
	pPPCTColliderIndex = -1;
	pUpdateOctreeIndex = -1;
	
	pDebugDrawer = NULL;
	pDDSShape = NULL;
	
	try{
		count = pCollider.GetAttachmentCount();
		for(i=0; i<count; i++){
			AttachmentAdded(i, pCollider.GetAttachmentAt(i));
		}
		
		count = pCollider.GetConstraintCount();
		for(i=0; i<count; i++){
			ConstraintAdded(i, pCollider.GetConstraintAt(i));
		}
		
		count = pCollider.GetCollisionTestCount();
		for(i=0; i<count; i++){
			CollisionTestAdded(i);
		}
		
		CollisionFilterChanged();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

debpCollider::~debpCollider(){
	pCleanUp();
}



// Physics management
///////////////////////

debpColliderVolume *debpCollider::CastToVolume() const{
	if(pType != ectVolume) DETHROW(deeInvalidParam);
	return (debpColliderVolume*)this;
}

debpColliderComponent *debpCollider::CastToComponent() const{
	if(pType != ectComponent) DETHROW(deeInvalidParam);
	return (debpColliderComponent*)this;
}

debpColliderRig *debpCollider::CastToRigged() const{
	if(pType != ectRig) DETHROW(deeInvalidParam);
	return (debpColliderRig*)this;
}

void debpCollider::SetIsMoving(bool isMoving){
	pIsMoving = isMoving;
	
	SetAutoColDetPrepare(CalcAutoColDetPrepare());
	SetAutoColDetFinish(CalcAutoColDetFinish());
}

const decDMatrix &debpCollider::GetMatrix(){
	if(pDirtyMatrix){
		UpdateMatrix();
		pDirtyMatrix = false;
	}
	return pMatrix;
}

const decDMatrix &debpCollider::GetInverseMatrix(){
	if(pDirtyMatrix){
		UpdateMatrix();
		pDirtyMatrix = false;
	}
	return pInvMatrix;
}

const decDMatrix &debpCollider::GetMatrixNormal(){
	if(pDirtyMatrix){
		UpdateMatrix();
		pDirtyMatrix = false;
	}
	return pMatrixNormal;
}

void debpCollider::SetIndex(int index){
	pIndex = index;
}

void debpCollider::SetParentWorld(debpWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	if(pParentWorld){
		UnregisterUpdateOctree();
		UnregisterPPCProcessing();
		UnregisterColDetFinish();
		UnregisterColDetPrepare();
		
		if(pDebugDrawer){
			pParentWorld->GetWorld().RemoveDebugDrawer(pDebugDrawer);
		}
		
		// tell all touch sensors we are leaving. we can not delay this like we do if the
		// collider moves because removing a collider from the world often happens right
		// before disposing of game elements. not telling leaving immediately to the game
		// scripts can cause difficult bugs
		/*
		const int tttCount = pTrackingTouchSensors.GetCount();
		int i;
		for(i=0; i<tttCount; i++){
			debpTouchSensor &touchSensor = *((debpTouchSensor*)pTrackingTouchSensors.GetAt(i));
			if(touchSensor.GetTouchingColliders().Has(this)){
				touchSensor.GetTouchingColliders().Remove(this);
				touchSensor.GetLeavingColliders().AddIfAbsent(this);
			}
		}
		*/
		pRemoveFromAllTrackingTouchSensors();
	}
	
	pParentWorld = parentWorld;
	
	if(parentWorld){
		if(pDebugDrawer){
			parentWorld->GetWorld().AddDebugDrawer(pDebugDrawer);
		}
	}
	
	ParentWorldChanged();
	MarkDirtyOctree();
	if(pCollisionTests.GetCount() > 0){
		RegisterPPCProcessing();
	}
	
	// always register once
	RegisterColDetPrepare();
	RegisterColDetFinish();
}

void debpCollider::ParentWorldChanged(){
	if(pParentWorld){
		CreateBody();
		
	}else{
		DestroyBody();
	}
}

void debpCollider::SetAddToOctree(bool doAdd){
	MarkDirtyOctree();
}

void debpCollider::SetExtends(const decDVector &minExtend, const decDVector &maxExtend){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void debpCollider::MarkMatrixDirty(){
	pDirtyMatrix = true;
}

void debpCollider::MarkDirtyOctree(){
	pDirtyOctree = true;
	RegisterUpdateOctree();
	
	if(pParentWorld){
		pParentWorld->MarkOctreeDirty();
	}
}

void debpCollider::RequiresUpdate(){
	pRequiresUpdate = true;
	
	// NOTE RequiresUpdate can be called outside a prepare/finish cycle. if registered
	//      outside this cycle we do a superfluous finish call. while not optimal
	//      performance wise it is better to do it to not miss a notification.
	RegisterColDetFinish();
}

void debpCollider::ClearRequiresUpdate(){
	pRequiresUpdate = false;
}

bool debpCollider::Collides(const debpCollider &collider) const{
	deCollider &engCollider = collider.GetCollider();
	return pCollider.GetCollisionFilter().Collides(engCollider.GetCollisionFilter())
		&& !pCollider.HasIgnoreCollider(&engCollider)
		&& !engCollider.HasIgnoreCollider(&pCollider);
}

bool debpCollider::CollidesNot(const debpCollider &collider) const{
	deCollider &engCollider = collider.GetCollider();
	return pCollider.GetCollisionFilter().CollidesNot(engCollider.GetCollisionFilter())
		|| pCollider.HasIgnoreCollider(&engCollider)
		|| engCollider.HasIgnoreCollider(&pCollider);
}

void debpCollider::CreateBody(){
}

void debpCollider::DestroyBody(){
}

void debpCollider::UpdateFromBody(){
}

void debpCollider::UpdateExtends(){
}

void debpCollider::UpdateMatrix(){
	pMatrixNormal.SetFromQuaternion(pCollider.GetOrientation());
	pMatrix = decDMatrix::CreateScale(pCollider.GetScale()).QuickMultiply(pMatrixNormal)
		.QuickMultiply(decDMatrix::CreateTranslation(pCollider.GetPosition()));
	pInvMatrix = pMatrix.QuickInvert();
}

void debpCollider::PrepareForStep(){
	PrepareConstraintsForStep();
}

void debpCollider::DetectCustomCollision(float elapsed){
}



void debpCollider::PrepareDetection(float elapsed){
	pRequiresUpdate = false;
	pIsPrepared = true;
}

void debpCollider::FinishDetection(){
	if(pIsPrepared){
		CheckColliderConstraintsBroke();
	}
	
	if(pRequiresUpdate){
		pCollider.GetPeerScripting()->ColliderChanged(&pCollider);
		pRequiresUpdate = false;
	}
	
	UpdateDebugDrawer();
	pIsPrepared = false;
}



void debpCollider::UpdateCollisionObjectAABBs(){
}



void debpCollider::UpdateOctreePosition(){
	if(pDirtyOctree && pParentWorld){
		UpdateExtends();
		pDirtyOctree = false;
	}
}

debpCollisionWorld *debpCollider::GetDynamicsWorld() const{
	if(pParentWorld){
		return pParentWorld->GetDynamicsWorld();
	}
	
	return NULL;
}

void debpCollider::SetUseKinematicSimulation(bool useKinematicSimulation){
	pUseKinematicSim = useKinematicSimulation;
}



void debpCollider::UpdateShapes(){
}

void debpCollider::UpdateShapesWithMatrix(const decDMatrix &transformation){
}

void debpCollider::SetShapeExtends(const decDVector &minExtend, const decDVector &maxExtend){
	pShapeMinExtend = minExtend;
	pShapeMaxExtend = maxExtend;
}



void debpCollider::PrepareConstraintsForStep(){
	int i;
	for(i=0; i<pConstraintCount; i++){
		pConstraints[i]->PrepareForStep();
	}
}

void debpCollider::CheckColliderConstraintsBroke(){
	// NOTE optimizing this is tricky due to bullet not providing the required functions
	//      virtual so we can not actively track the breaking but have to find it afterwards.
	//      this requires checking constraints for having broken in FinishDetection().
	//      
	//      the only possible optimization is to check if constraints are used at all.
	//      constraints exist only if added by the user. so we use the rule that
	//      FinishDetection is required if there are any constraints. furthermore we can
	//      rule out any constraint which can not break.
	int i;
	for(i=0; i<pConstraintCount; i++){
		if(pConstraints[i]->CheckHasBroken()){
			pCollider.GetPeerScripting()->ColliderConstraintBroke(
				&pCollider, i, &pConstraints[i]->GetConstraint());
		}
	}
}



void debpCollider::ProcessColliderCollisionTests(){
	const int count = pCollisionTests.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((debpColliderCollisionTest*)pCollisionTests.GetAt(i))->Update();
	}
}



void debpCollider::RegisterColDetPrepare(){
	if(pColDetPrepareIndex != -1 || !pParentWorld){
		return;
	}
	pParentWorld->pColDetPrepareColliderAdd(this);
}

void debpCollider::UnregisterColDetPrepare(){
	if(pColDetPrepareIndex == -1 || !pParentWorld){
		return;
	}
	pParentWorld->pColDetPrepareColliderRemove(this);
}

void debpCollider::SetColDetPrepareIndex(int index){
	pColDetPrepareIndex = index;
}

void debpCollider::SetAutoColDetPrepare(bool autoColDetPrepare){
	pAutoColDetPrepare = autoColDetPrepare;
	
	if(autoColDetPrepare){
		RegisterColDetPrepare();
	}
}

bool debpCollider::CalcAutoColDetPrepare(){
	if(pIsMoving){
		return true;
	}
	
	// see CheckColliderConstraintsBroke()
	int i;
	for(i=0; i<pConstraintCount; i++){
		if(pConstraints[i]->IsBreakable()){
			return true;
		}
	}
	
	return false;
}



void debpCollider::RegisterColDetFinish(){
	if(pColDetFinishIndex != -1 || !pParentWorld){
		return;
	}
	pParentWorld->pColDetFinishColliderAdd(this);
	
	// we need to reset pRequiresUpdate if the collider is not registered for prepare.
	// this is due to PrepareDetection() reseting pRequiresUpdate. if the collider is
	// not registered for prepare this reset would be missed so we do it here
	// 
	// NOTE this resulted in problems with attached colliders not getting ColliderChanged()
	//      properly called. this is now solved by using pIsPrepared which indicates if
	//      the PrepareDetection() has been called or not. thus below lines are now disabled
// 	if( pColDetPrepareIndex == -1 ){
// 		pRequiresUpdate = false;
// 	}
}

void debpCollider::UnregisterColDetFinish(){
	if(pColDetFinishIndex == -1 || !pParentWorld){
		return;
	}
	pParentWorld->pColDetFinishColliderRemove(this);
}

void debpCollider::SetColDetFinishIndex(int index){
	pColDetFinishIndex = index;
}

void debpCollider::SetAutoColDetFinish(bool autoColDetFinish){
	pAutoColDetFinish = autoColDetFinish;
	
	if(autoColDetFinish){
		RegisterColDetFinish();
	}
}

bool debpCollider::CalcAutoColDetFinish(){
	if(pIsMoving){
		return true;
	}
	
	// see CheckColliderConstraintsBroke()
	int i;
	for(i=0; i<pConstraintCount; i++){
		if(pConstraints[i]->IsBreakable()){
			return true;
		}
	}
	
	return false;
}



void debpCollider::RegisterPPCProcessing(){
	if(pPPCTColliderIndex != -1 || !pParentWorld){
		return;
	}
	pParentWorld->pPPCTColliderAdd(this);
}

void debpCollider::UnregisterPPCProcessing(){
	if(pPPCTColliderIndex == -1 || !pParentWorld){
		return;
	}
	pParentWorld->pPPCTColliderRemove(this);
}

void debpCollider::SetPPCProcessingIndex(int index){
	pPPCTColliderIndex = index;
}



void debpCollider::RegisterUpdateOctree(){
	if(pUpdateOctreeIndex != -1 || !pParentWorld){
		return;
	}
	pParentWorld->pUpdateOctreeColliderAdd(this);
}

void debpCollider::UnregisterUpdateOctree(){
	if(pUpdateOctreeIndex == -1 || !pParentWorld){
		return;
	}
	pParentWorld->pUpdateOctreeColliderRemove(this);
}

void debpCollider::SetUpdateOctreeIndex(int index){
	pUpdateOctreeIndex = index;
}



// Attachments
////////////////

debpColliderAttachment *debpCollider::GetAttachmentAt(int index) const{
	if(index < 0 || index >= pAttachmentCount){
		DETHROW(deeInvalidParam);
	}
	
	return pAttachments[index];
}



// Constraints
////////////////

debpColliderConstraint *debpCollider::GetConstraintAt(int index) const{
	if(index < 0 || index >= pConstraintCount) DETHROW(deeInvalidParam);
	
	return pConstraints[index];
}



// Attachments
////////////////

int debpCollider::GetCollisionTestCount() const{
	return pCollisionTests.GetCount();
}

debpColliderCollisionTest *debpCollider::GetCollisionTestAt(int index) const{
	return (debpColliderCollisionTest*)pCollisionTests.GetAt(index);
}



// Debugging
//////////////

void debpCollider::UpdateDebugDrawer(){
	const debpDeveloperMode &devmode = pBullet->GetDeveloperMode();
	
	if(devmode.GetEnabled()
	&& devmode.GetShowCategory().IsNotEmpty()
	&& devmode.GetShowCategory().Matches(pCollider.GetCollisionFilter().GetCategory())){
		// ensure the debug drawer exists
		if(!pDebugDrawer){
			pDebugDrawer = pBullet->GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer();
			pDebugDrawer->SetXRay(true);
			pDebugDrawer->SetPosition(pCollider.GetPosition());
			pDebugDrawer->SetOrientation(pCollider.GetOrientation());
			pDebugDrawer->SetScale(pCollider.GetScale());
			
			if(pParentWorld){
				pParentWorld->GetWorld().AddDebugDrawer(pDebugDrawer);
			}
		}
		
		// show shapes if layer mask matches
		if(!pDDSShape){
			pDDSShape = new deDebugDrawerShape;
			pDebugDrawer->AddShape(pDDSShape);
			UpdateDDSShape();
		}
		
		// update color
		decColor colorFill(pDDSShape->GetFillColor());
		decColor colorEdge(pDDSShape->GetEdgeColor());
		
		if(devmode.GetHighlightResponseType() != -1){
			if(pCollider.GetResponseType() == (deCollider::eResponseType)devmode.GetHighlightResponseType()){
				colorFill = debpDebugDrawerColors::colliderFill;
				colorEdge = debpDebugDrawerColors::colliderEdge;
				
			}else{
				colorFill = debpDebugDrawerColors::colliderLowFill;
				colorEdge = debpDebugDrawerColors::colliderLowEdge;
			}
			
		}else if(devmode.GetHighlightDeactivation()){
			if(GetRigidBodyDeactivated()){
				colorFill = debpDebugDrawerColors::colliderLowFill;
				colorEdge = debpDebugDrawerColors::colliderLowEdge;
				
			}else{
				colorFill = debpDebugDrawerColors::colliderFill;
				colorEdge = debpDebugDrawerColors::colliderEdge;
			}
			
		}else{
			colorFill = debpDebugDrawerColors::colliderFill;
			colorEdge = debpDebugDrawerColors::colliderEdge;
		}
		
		if(!colorFill.IsEqualTo(pDDSShape->GetFillColor()) || !colorEdge.IsEqualTo(pDDSShape->GetEdgeColor())){
			pDDSShape->SetFillColor(colorFill);
			pDDSShape->SetEdgeColor(colorEdge);
			pDebugDrawer->NotifyShapeColorChanged();
		}
		
	}else{
		// if the debug drawer exists remove it
		if(pDebugDrawer){
			if(pParentWorld){
				pParentWorld->GetWorld().RemoveDebugDrawer(pDebugDrawer);
			}
			
			pDDSShape = NULL;
			
			pDebugDrawer = NULL;
		}
	}
}

void debpCollider::UpdateDDSShape(){
}

bool debpCollider::GetRigidBodyDeactivated() const{
	return true;
}



// Notifications
//////////////////

void debpCollider::PositionChanged(){
	if(pDebugDrawer){
		pDebugDrawer->SetPosition(pCollider.GetPosition());
	}
}

void debpCollider::OrientationChanged(){
	if(pDebugDrawer){
		pDebugDrawer->SetOrientation(pCollider.GetOrientation());
	}
}

void debpCollider::ScaleChanged(){
	if(pDebugDrawer){
		pDebugDrawer->SetScale(pCollider.GetScale());
	}
}

void debpCollider::GeometryChanged(){
	if(pDebugDrawer){
		pDebugDrawer->SetPosition(pCollider.GetPosition());
		pDebugDrawer->SetOrientation(pCollider.GetOrientation());
		pDebugDrawer->SetScale(pCollider.GetScale());
	}
}

void debpCollider::LinearVelocityChanged(){}
void debpCollider::AngularVelocityChanged(){}
void debpCollider::GravityChanged(){}
void debpCollider::EnabledChanged(){}
void debpCollider::PropertiesChanged(){}
void debpCollider::ResponseTypeChanged(){}

void debpCollider::CollisionFilterChanged(){
}

void debpCollider::IgnoreCollidersChanged(){
}

void debpCollider::ForceFieldChanged(){
}

void debpCollider::AttachmentAdded(int index, deColliderAttachment *attachment){
	if(pAttachmentCount == pAttachmentSize){
		int newSize = pAttachmentSize * 3 / 2 + 1;
		debpColliderAttachment **newArray = new debpColliderAttachment*[newSize];
		if(pAttachments){
			memcpy(newArray, pAttachments, sizeof(debpColliderAttachment*) * pAttachmentSize);
			delete [] pAttachments;
		}
		pAttachments = newArray;
		pAttachmentSize = newSize;
	}
	
	pAttachments[pAttachmentCount] = new debpColliderAttachment(attachment);
	pAttachmentCount++;
	
	deResource * const resource = attachment->GetResource();
	if(resource->GetResourceManager()->GetResourceType() == deResourceManager::ertCollider){
		((debpCollider*)(((deCollider*)resource)->GetPeerPhysics()))->GetAttachedToList().Add(this);
	}
}

void debpCollider::AttachmentChanged(int index, deColliderAttachment *attachment){
	pAttachments[index]->AttachmentChanged();
}

void debpCollider::AttachmentRemoved(int index, deColliderAttachment *attachment){
	int i;
	
	deResource * const resource = attachment->GetResource();
	if(resource->GetResourceManager()->GetResourceType() == deResourceManager::ertCollider){
		debpCollider * const collider = (debpCollider*)(((deCollider*)resource)->GetPeerPhysics());
		if(collider){
			collider->GetAttachedToList().Remove(this);
		}
	}
	
	delete pAttachments[index];
	
	for(i=index+1; i<pAttachmentCount; i++){
		pAttachments[i - 1] = pAttachments[i];
	}
	
	pAttachmentCount--;
}

void debpCollider::AllAttachmentsRemoved(){
	while(pAttachmentCount > 0){
		pAttachmentCount--;
		
		deResource * const resource = pAttachments[pAttachmentCount]->GetAttachment()->GetResource();
		if(resource->GetResourceManager()->GetResourceType() == deResourceManager::ertCollider){
			debpCollider * const collider = (debpCollider*)(((deCollider*)resource)->GetPeerPhysics());
			if(collider){
				collider->GetAttachedToList().Remove(this);
			}
		}
		
		delete pAttachments[pAttachmentCount];
	}
}



void debpCollider::ConstraintAdded(int index, deColliderConstraint *constraint){
	if(pConstraintCount == pConstraintSize){
		int newSize = pConstraintSize * 3 / 2 + 1;
		debpColliderConstraint **newArray = new debpColliderConstraint*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pConstraints){
			memcpy(newArray, pConstraints, sizeof(debpColliderConstraint*) * pConstraintSize);
			delete [] pConstraints;
		}
		pConstraints = newArray;
		pConstraintSize = newSize;
	}
	
	pConstraints[pConstraintCount] = new debpColliderConstraint(*pBullet, *constraint);
	if(!pConstraints[pConstraintCount]) DETHROW(deeOutOfMemory);
	pConstraintCount++;
	
	// see CheckColliderConstraintsBroke() for the reason to register
	SetAutoColDetPrepare(CalcAutoColDetPrepare());
	SetAutoColDetFinish(CalcAutoColDetFinish());
}

void debpCollider::ConstraintChanged(int index, deColliderConstraint *constraint){
	//pConstraints[ index ]->ConstraintChanged();
}

void debpCollider::ConstraintRemoved(int index, deColliderConstraint *constraint){
	int i;
	
	delete pConstraints[index];
	for(i=index+1; i<pConstraintCount; i++){
		pConstraints[i - 1] = pConstraints[i];
	}
	pConstraintCount--;
}

void debpCollider::AllConstraintsRemoved(){
	while(pConstraintCount > 0){
		pConstraintCount--;
		delete pConstraints[pConstraintCount];
	}
}



void debpCollider::CollisionTestChanged(int index){
	((debpColliderCollisionTest*)pCollisionTests.GetAt(index))->CollisionTestChanged();
}

void debpCollider::CollisionTestEnabledChanged(int index){
	// not interesting for us. we check this during each update
}

void debpCollider::CollisionTestAdded(int index){
	pCollisionTests.Add(new debpColliderCollisionTest(*this, *pCollider.GetCollisionTestAt(index)));
	RegisterPPCProcessing();
}

void debpCollider::CollisionTestRemoved(int index){
	debpColliderCollisionTest * const cct = (debpColliderCollisionTest*)pCollisionTests.GetAt(index);
	pCollisionTests.RemoveFrom(index);
	delete cct;
	
	if(pCollisionTests.GetCount() == 0){
		UnregisterPPCProcessing();
	}
}

void debpCollider::AllCollisionTestsRemoved(){
	while(pCollisionTests.GetCount() > 0){
		delete (debpColliderCollisionTest*)pCollisionTests.GetAt(pCollisionTests.GetCount() - 1);
		pCollisionTests.RemoveFrom(pCollisionTests.GetCount() - 1);
	}
	
	UnregisterPPCProcessing();
}



// private functions
//////////////////////

void debpCollider::pCleanUp(){
	pRemoveFromAllTrackingTouchSensors();
	
	AllCollisionTestsRemoved();
	
	if(pConstraints){
		AllConstraintsRemoved();
		delete [] pConstraints;
	}
	
	if(pAttachments){
		AllAttachmentsRemoved();
		delete [] pAttachments;
	}
	
}

void debpCollider::pRemoveFromAllTrackingTouchSensors(){
	const int count = pTrackingTouchSensors.GetCount();
	int i;
	
	for(i=0; i< count; i++){
		((debpTouchSensor*)pTrackingTouchSensors.GetAt(i))->RemoveColliderImmediately(this);
	}
	
	pTrackingTouchSensors.RemoveAll();
}
