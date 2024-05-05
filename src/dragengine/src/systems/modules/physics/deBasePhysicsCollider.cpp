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

#include "deBasePhysicsCollider.h"

#include "../../../common/exceptions.h"


// Class deBasePhysicsCollider
////////////////////////////////

// Constructres, destructors
//////////////////////////////

deBasePhysicsCollider::deBasePhysicsCollider(){
}

deBasePhysicsCollider::~deBasePhysicsCollider(){
}



// Notifications
//////////////////

void deBasePhysicsCollider::PositionChanged(){
}

void deBasePhysicsCollider::OrientationChanged(){
}

void deBasePhysicsCollider::GeometryChanged(){
}

void deBasePhysicsCollider::ScaleChanged(){
}

void deBasePhysicsCollider::LinearVelocityChanged(){
}

void deBasePhysicsCollider::AngularVelocityChanged(){
}

void deBasePhysicsCollider::EnabledChanged(){
}

void deBasePhysicsCollider::GravityChanged(){
}

void deBasePhysicsCollider::PropertiesChanged(){
}

void deBasePhysicsCollider::ResponseTypeChanged(){
}

void deBasePhysicsCollider::ForceFieldChanged(){
}



void deBasePhysicsCollider::CollisionFilterChanged(){
}

void deBasePhysicsCollider::IgnoreCollidersChanged(){
}



void deBasePhysicsCollider::CollisionTestAdded( int ){
}

void deBasePhysicsCollider::CollisionTestChanged( int ){
}

void deBasePhysicsCollider::CollisionTestEnabledChanged( int ){
}

void deBasePhysicsCollider::CollisionTestRemoved( int ){
}

void deBasePhysicsCollider::AllCollisionTestsRemoved(){
}



void deBasePhysicsCollider::CollisionVolumeChanged(){
}

void deBasePhysicsCollider::RigChanged(){
}

void deBasePhysicsCollider::ComponentChanged(){
}



// Bones
//////////

void deBasePhysicsCollider::BonePositionChanged( int ){
}

void deBasePhysicsCollider::BoneOrientationChanged( int ){
}

void deBasePhysicsCollider::BoneLinearVelocityChanged( int ){
}

void deBasePhysicsCollider::BoneAngularVelocityChanged( int ){
}

void deBasePhysicsCollider::BonePropertiesChanged( int ){
}

void deBasePhysicsCollider::BoneDynamicChanged( int ){
}



// Force and Impuls
/////////////////////

void deBasePhysicsCollider::ApplyImpuls( const decVector & ){
}

void deBasePhysicsCollider::ApplyImpulsAt( const decVector &, const decVector & ){
}

void deBasePhysicsCollider::ApplyTorqueImpuls( const decVector & ){
}

void deBasePhysicsCollider::ApplyForce( const decVector & ){
}

void deBasePhysicsCollider::ApplyForceAt( const decVector &, const decVector & ){
}

void deBasePhysicsCollider::ApplyTorque( const decVector & ){
}



void deBasePhysicsCollider::ApplyBoneImpuls( int, const decVector & ){
}

void deBasePhysicsCollider::ApplyBoneImpulsAt( int, const decVector &, const decVector & ){
}

void deBasePhysicsCollider::ApplyBoneTorqueImpuls( int, const decVector & ){
}

void deBasePhysicsCollider::ApplyBoneForce( int, const decVector & ){
}

void deBasePhysicsCollider::ApplyBoneForceAt( int, const decVector &, const decVector & ){
}

void deBasePhysicsCollider::ApplyBoneTorque( int, const decVector & ){
}



// Attachments
////////////////

void deBasePhysicsCollider::AttachmentAdded( int, deColliderAttachment* ){
}

void deBasePhysicsCollider::AttachmentChanged( int, deColliderAttachment* ){
}

void deBasePhysicsCollider::AttachmentRemoved( int, deColliderAttachment* ){
}

void deBasePhysicsCollider::AllAttachmentsRemoved(){
}

void deBasePhysicsCollider::AttachmentsForceUpdate(){
}

void deBasePhysicsCollider::InitWeightAttachment( deColliderAttachment&, int ){
	DETHROW( deeInvalidParam );
}



// Constraints
////////////////

void deBasePhysicsCollider::ConstraintAdded( int, deColliderConstraint* ){
}

void deBasePhysicsCollider::ConstraintChanged( int, deColliderConstraint* ){
}

void deBasePhysicsCollider::ConstraintRemoved( int, deColliderConstraint* ){
}

void deBasePhysicsCollider::AllConstraintsRemoved(){
}



void deBasePhysicsCollider::EnableBoneConstraint( int, int, bool ){
}

void deBasePhysicsCollider::ReplaceBoneConstraint( int, int, const deRigConstraint & ){
}



// Collision Detection
////////////////////////

bool deBasePhysicsCollider::PointInside( const decDVector & ){
	return false;
}

void deBasePhysicsCollider::RayHits( const decDVector &, const decVector &, deBaseScriptingCollider* ){
}

void deBasePhysicsCollider::ColliderHits( deCollider*, deBaseScriptingCollider* ){
}

void deBasePhysicsCollider::ColliderMoveHits( deCollider*, const decVector &, deBaseScriptingCollider* ){
}

void deBasePhysicsCollider::ColliderRotateHits( deCollider*, const decVector &, deBaseScriptingCollider* ){
}

void deBasePhysicsCollider::ColliderMoveRotateHits( deCollider*, const decVector &,
const decVector &, deBaseScriptingCollider* ){
}
