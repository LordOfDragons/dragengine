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

#include "deBaseGraphicWorld.h"



// Class deBaseGraphicWorld
///////////////////////////////

deBaseGraphicWorld::deBaseGraphicWorld(){
}

deBaseGraphicWorld::~deBaseGraphicWorld(){
}



// Notifications
//////////////////

void deBaseGraphicWorld::Update(float elapsed){
}

void deBaseGraphicWorld::SizeChanged(){
}



void deBaseGraphicWorld::HeightTerrainChanged(){
}

void deBaseGraphicWorld::LightingChanged(){
}



void deBaseGraphicWorld::SkyAdded(deSkyInstance *sky){
}

void deBaseGraphicWorld::SkyRemoved(deSkyInstance *sky){
}

void deBaseGraphicWorld::AllSkiesRemoved(){
}



void deBaseGraphicWorld::BillboardAdded(deBillboard *billboard){
}

void deBaseGraphicWorld::BillboardRemoved(deBillboard *billboard){
}

void deBaseGraphicWorld::AllBillboardsRemoved(){
}



void deBaseGraphicWorld::CameraAdded(deCamera *camera){
}

void deBaseGraphicWorld::CameraRemoved(deCamera *camera){
}

void deBaseGraphicWorld::AllCamerasRemoved(){
}



void deBaseGraphicWorld::ComponentAdded(deComponent *component){
}

void deBaseGraphicWorld::ComponentRemoved(deComponent *component){
}

void deBaseGraphicWorld::AllComponentsRemoved(){
}



void deBaseGraphicWorld::DebugDrawerAdded(deDebugDrawer *debugDrawer){
}

void deBaseGraphicWorld::DebugDrawerRemoved(deDebugDrawer *debugDrawer){
}

void deBaseGraphicWorld::AllDebugDrawersRemoved(){
}



void deBaseGraphicWorld::EnvMapProbeAdded(deEnvMapProbe *envMapProbe){
}

void deBaseGraphicWorld::EnvMapProbeRemoved(deEnvMapProbe *envMapProbe){
}

void deBaseGraphicWorld::AllEnvMapProbesRemoved(){
}



void deBaseGraphicWorld::LightAdded(deLight *light){
}

void deBaseGraphicWorld::LightRemoved(deLight *light){
}

void deBaseGraphicWorld::AllLightsRemoved(){
}



void deBaseGraphicWorld::LumimeterAdded(deLumimeter *lumimeter){
}

void deBaseGraphicWorld::LumimeterRemoved(deLumimeter *lumimeter){
}

void deBaseGraphicWorld::AllLumimetersRemoved(){
}



void deBaseGraphicWorld::ParticleEmitterAdded(deParticleEmitterInstance *emitter){
}

void deBaseGraphicWorld::ParticleEmitterRemoved(deParticleEmitterInstance *emitter){
}

void deBaseGraphicWorld::AllParticleEmittersRemoved(){
}



void deBaseGraphicWorld::PropFieldAdded(dePropField *propField){
}

void deBaseGraphicWorld::PropFieldRemoved(dePropField *propField){
}

void deBaseGraphicWorld::AllPropFieldsRemoved(){
}



void deBaseGraphicWorld::SmokeEmitterAdded(deSmokeEmitter *smokeEmitter){
}

void deBaseGraphicWorld::SmokeEmitterRemoved(deSmokeEmitter *smokeEmitter){
}

void deBaseGraphicWorld::AllSmokeEmittersRemoved(){
}
