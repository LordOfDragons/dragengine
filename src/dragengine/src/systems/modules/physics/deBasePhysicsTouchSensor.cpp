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

#include "deBasePhysicsTouchSensor.h"
#include "../../../common/exceptions.h"



// Class deBasePhysicsTouchSensor
///////////////////////////////////

// Constructres, destructors
//////////////////////////////

deBasePhysicsTouchSensor::deBasePhysicsTouchSensor(){
}

deBasePhysicsTouchSensor::~deBasePhysicsTouchSensor(){
}



// Notifications
//////////////////

void deBasePhysicsTouchSensor::PositionChanged(){
}

void deBasePhysicsTouchSensor::OrientationChanged(){
}



void deBasePhysicsTouchSensor::CollisionFilterChanged(){
}

void deBasePhysicsTouchSensor::IgnoreCollidersChanged(){
}

void deBasePhysicsTouchSensor::TrackEnterLeaveChanged(){
}

void deBasePhysicsTouchSensor::EnabledChanged(){
}

void deBasePhysicsTouchSensor::ShapeChanged(){
}



bool deBasePhysicsTouchSensor::IsEmpty(){
	return true;
}

int deBasePhysicsTouchSensor::GetColliderCount(){
	return 0;
}

deCollider *deBasePhysicsTouchSensor::GetColliderAt( int collider ){
	DETHROW( deeInvalidParam );
}



// Collision Detection
////////////////////////

bool deBasePhysicsTouchSensor::PointInside( const decDVector &point ){
	return false;
}

void deBasePhysicsTouchSensor::AllHits( deBaseScriptingCollider *listener ){
}

void deBasePhysicsTouchSensor::RayHits( const decDVector &rayOrigin,
const decVector &rayDirection, deBaseScriptingCollider *listener ){
}

void deBasePhysicsTouchSensor::ColliderHits( deCollider *collider,
deBaseScriptingCollider *listener ){
}

void deBasePhysicsTouchSensor::ColliderMoveHits( deCollider *collider,
const decVector &displacement, deBaseScriptingCollider *listener ){
}

void deBasePhysicsTouchSensor::ColliderRotateHits( deCollider *collider,
const decVector &rotation, deBaseScriptingCollider *listener ){
}

void deBasePhysicsTouchSensor::ColliderMoveRotateHits( deCollider *collider,
const decVector &displacement, const decVector &rotation, deBaseScriptingCollider *listener ){
}
