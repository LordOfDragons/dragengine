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

deCollider *deBasePhysicsTouchSensor::GetColliderAt(int collider){
	DETHROW(deeInvalidParam);
}



// Collision Detection
////////////////////////

bool deBasePhysicsTouchSensor::PointInside(const decDVector &point){
	return false;
}

void deBasePhysicsTouchSensor::AllHits(deBaseScriptingCollider *listener){
}

void deBasePhysicsTouchSensor::RayHits(const decDVector &rayOrigin,
const decVector &rayDirection, deBaseScriptingCollider *listener){
}

void deBasePhysicsTouchSensor::ColliderHits(deCollider *collider,
deBaseScriptingCollider *listener){
}

void deBasePhysicsTouchSensor::ColliderMoveHits(deCollider *collider,
const decVector &displacement, deBaseScriptingCollider *listener){
}

void deBasePhysicsTouchSensor::ColliderRotateHits(deCollider *collider,
const decVector &rotation, deBaseScriptingCollider *listener){
}

void deBasePhysicsTouchSensor::ColliderMoveRotateHits(deCollider *collider,
const decVector &displacement, const decVector &rotation, deBaseScriptingCollider *listener){
}
