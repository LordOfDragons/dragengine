/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
