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

#include "deBaseGraphicWorld.h"



// Class deBaseGraphicWorld
///////////////////////////////

deBaseGraphicWorld::deBaseGraphicWorld(){
}

deBaseGraphicWorld::~deBaseGraphicWorld(){
}



// Notifications
//////////////////

void deBaseGraphicWorld::Update( float elapsed ){
}

void deBaseGraphicWorld::SizeChanged(){
}



void deBaseGraphicWorld::HeightTerrainChanged(){
}

void deBaseGraphicWorld::LightingChanged(){
}



void deBaseGraphicWorld::SkyAdded( deSkyInstance *sky ){
}

void deBaseGraphicWorld::SkyRemoved( deSkyInstance *sky ){
}

void deBaseGraphicWorld::AllSkiesRemoved(){
}



void deBaseGraphicWorld::BillboardAdded( deBillboard *billboard ){
}

void deBaseGraphicWorld::BillboardRemoved( deBillboard *billboard ){
}

void deBaseGraphicWorld::AllBillboardsRemoved(){
}



void deBaseGraphicWorld::CameraAdded( deCamera *camera ){
}

void deBaseGraphicWorld::CameraRemoved( deCamera *camera ){
}

void deBaseGraphicWorld::AllCamerasRemoved(){
}



void deBaseGraphicWorld::ComponentAdded( deComponent *component ){
}

void deBaseGraphicWorld::ComponentRemoved( deComponent *component ){
}

void deBaseGraphicWorld::AllComponentsRemoved(){
}



void deBaseGraphicWorld::DebugDrawerAdded( deDebugDrawer *debugDrawer ){
}

void deBaseGraphicWorld::DebugDrawerRemoved( deDebugDrawer *debugDrawer ){
}

void deBaseGraphicWorld::AllDebugDrawersRemoved(){
}



void deBaseGraphicWorld::EnvMapProbeAdded( deEnvMapProbe *envMapProbe ){
}

void deBaseGraphicWorld::EnvMapProbeRemoved( deEnvMapProbe *envMapProbe ){
}

void deBaseGraphicWorld::AllEnvMapProbesRemoved(){
}



void deBaseGraphicWorld::LightAdded( deLight *light ){
}

void deBaseGraphicWorld::LightRemoved( deLight *light ){
}

void deBaseGraphicWorld::AllLightsRemoved(){
}



void deBaseGraphicWorld::LumimeterAdded( deLumimeter *lumimeter ){
}

void deBaseGraphicWorld::LumimeterRemoved( deLumimeter *lumimeter ){
}

void deBaseGraphicWorld::AllLumimetersRemoved(){
}



void deBaseGraphicWorld::ParticleEmitterAdded( deParticleEmitterInstance *emitter ){
}

void deBaseGraphicWorld::ParticleEmitterRemoved( deParticleEmitterInstance *emitter ){
}

void deBaseGraphicWorld::AllParticleEmittersRemoved(){
}



void deBaseGraphicWorld::PropFieldAdded( dePropField *propField ){
}

void deBaseGraphicWorld::PropFieldRemoved( dePropField *propField ){
}

void deBaseGraphicWorld::AllPropFieldsRemoved(){
}



void deBaseGraphicWorld::SmokeEmitterAdded( deSmokeEmitter *smokeEmitter ){
}

void deBaseGraphicWorld::SmokeEmitterRemoved( deSmokeEmitter *smokeEmitter ){
}

void deBaseGraphicWorld::AllSmokeEmittersRemoved(){
}
