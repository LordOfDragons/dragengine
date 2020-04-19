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

void deBasePhysicsWorld::Update( float elapsed ){
}

void deBasePhysicsWorld::ProcessPhysics( float elapsed ){
}



// Notifications
//////////////////

void deBasePhysicsWorld::SizeChanged(){
}

void deBasePhysicsWorld::PhysicsChanged(){
}



void deBasePhysicsWorld::HeightTerrainChanged(){
}



void deBasePhysicsWorld::ComponentAdded( deComponent *component ){
}

void deBasePhysicsWorld::ComponentRemoved( deComponent *component ){
}

void deBasePhysicsWorld::AllComponentsRemoved(){
}



void deBasePhysicsWorld::ColliderAdded( deCollider *collider ){
}

void deBasePhysicsWorld::ColliderRemoved( deCollider *collider ){
}

void deBasePhysicsWorld::AllCollidersRemoved(){
}



void deBasePhysicsWorld::TouchSensorAdded( deTouchSensor *touchSensor ){
}

void deBasePhysicsWorld::TouchSensorRemoved( deTouchSensor *touchSensor ){
}

void deBasePhysicsWorld::AllTouchSensorsRemoved(){
}



void deBasePhysicsWorld::PropFieldAdded( dePropField *propField ){
}

void deBasePhysicsWorld::PropFieldRemoved( dePropField *propField ){
}

void deBasePhysicsWorld::AllPropFieldsRemoved(){
}



void deBasePhysicsWorld::ForceFieldAdded( deForceField *forceField ){
}

void deBasePhysicsWorld::ForceFieldRemoved( deForceField *forceField ){
}

void deBasePhysicsWorld::AllForceFieldsRemoved(){
}



void deBasePhysicsWorld::ParticleEmitterAdded( deParticleEmitterInstance *emitter ){
}

void deBasePhysicsWorld::ParticleEmitterRemoved( deParticleEmitterInstance *emitter ){
}

void deBasePhysicsWorld::AllParticleEmittersRemoved(){
}



void deBasePhysicsWorld::SmokeEmitterAdded( deSmokeEmitter *smokeEmitter ){
}

void deBasePhysicsWorld::SmokeEmitterRemoved( deSmokeEmitter *smokeEmitter ){
}

void deBasePhysicsWorld::AllSmokeEmittersRemoved(){
}



// Collision Detection
////////////////////////

void deBasePhysicsWorld::PointHits( const decDVector &point, deBaseScriptingCollider *listener,
const decCollisionFilter &collisionFilter ){
}

void deBasePhysicsWorld::RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
deBaseScriptingCollider *listener, const decCollisionFilter &collisionFilter ){
}

void deBasePhysicsWorld::ColliderHits( deCollider *collider, deBaseScriptingCollider *listener ){
}

void deBasePhysicsWorld::ColliderMoveHits( deCollider *collider, const decVector &displacement,
deBaseScriptingCollider *listener ){
}

void deBasePhysicsWorld::ColliderRotateHits( deCollider *collider, const decVector &rotation,
deBaseScriptingCollider *listener ){
}

void deBasePhysicsWorld::ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener ){
}
