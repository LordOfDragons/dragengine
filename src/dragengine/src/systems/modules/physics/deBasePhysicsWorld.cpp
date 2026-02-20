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

#include "deBasePhysicsWorld.h"



// Class deBasePhysicsWorld
/////////////////////////////

// Constructure, destructors
//////////////////////////////

deBasePhysicsWorld::deBasePhysicsWorld(){
}

deBasePhysicsWorld::~deBasePhysicsWorld(){
}



// Management
///////////////

void deBasePhysicsWorld::Update(float elapsed){
}

void deBasePhysicsWorld::ProcessPhysics(float elapsed){
}



// Notifications
//////////////////

void deBasePhysicsWorld::SizeChanged(){
}

void deBasePhysicsWorld::PhysicsChanged(){
}



void deBasePhysicsWorld::HeightTerrainChanged(){
}



void deBasePhysicsWorld::ComponentAdded(deComponent *component){
}

void deBasePhysicsWorld::ComponentRemoved(deComponent *component){
}

void deBasePhysicsWorld::AllComponentsRemoved(){
}



void deBasePhysicsWorld::ColliderAdded(deCollider *collider){
}

void deBasePhysicsWorld::ColliderRemoved(deCollider *collider){
}

void deBasePhysicsWorld::AllCollidersRemoved(){
}



void deBasePhysicsWorld::TouchSensorAdded(deTouchSensor *touchSensor){
}

void deBasePhysicsWorld::TouchSensorRemoved(deTouchSensor *touchSensor){
}

void deBasePhysicsWorld::AllTouchSensorsRemoved(){
}



void deBasePhysicsWorld::PropFieldAdded(dePropField *propField){
}

void deBasePhysicsWorld::PropFieldRemoved(dePropField *propField){
}

void deBasePhysicsWorld::AllPropFieldsRemoved(){
}



void deBasePhysicsWorld::ForceFieldAdded(deForceField *forceField){
}

void deBasePhysicsWorld::ForceFieldRemoved(deForceField *forceField){
}

void deBasePhysicsWorld::AllForceFieldsRemoved(){
}



void deBasePhysicsWorld::ParticleEmitterAdded(deParticleEmitterInstance *emitter){
}

void deBasePhysicsWorld::ParticleEmitterRemoved(deParticleEmitterInstance *emitter){
}

void deBasePhysicsWorld::AllParticleEmittersRemoved(){
}



void deBasePhysicsWorld::SmokeEmitterAdded(deSmokeEmitter *smokeEmitter){
}

void deBasePhysicsWorld::SmokeEmitterRemoved(deSmokeEmitter *smokeEmitter){
}

void deBasePhysicsWorld::AllSmokeEmittersRemoved(){
}



// Collision Detection
////////////////////////

void deBasePhysicsWorld::PointHits(const decDVector &point, deBaseScriptingCollider *listener,
const decCollisionFilter &collisionFilter){
}

void deBasePhysicsWorld::RayHits(const decDVector &rayOrigin, const decVector &rayDirection,
deBaseScriptingCollider *listener, const decCollisionFilter &collisionFilter){
}

void deBasePhysicsWorld::ColliderHits(deCollider *collider, deBaseScriptingCollider *listener){
}

void deBasePhysicsWorld::ColliderMoveHits(deCollider *collider, const decVector &displacement,
deBaseScriptingCollider *listener){
}

void deBasePhysicsWorld::ColliderRotateHits(deCollider *collider, const decVector &rotation,
deBaseScriptingCollider *listener){
}

void deBasePhysicsWorld::ColliderMoveRotateHits(deCollider *collider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener){
}
