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

#include "peeEmitterListener.h"



// Class peeEmitterListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

peeEmitterListener::peeEmitterListener(){
}

peeEmitterListener::~peeEmitterListener(){
}



// Notifications
//////////////////

void peeEmitterListener::StateChanged(peeEmitter*){
}

void peeEmitterListener::UndoChanged(peeEmitter*){
}

void peeEmitterListener::EmitterChanged(peeEmitter*){
}

void peeEmitterListener::SkyChanged(peeEmitter*){
}

void peeEmitterListener::EnvObjectChanged(peeEmitter*){
}

void peeEmitterListener::ViewChanged(peeEmitter*){
}

void peeEmitterListener::CameraChanged(peeEmitter*){
}



void peeEmitterListener::ControllerStructureChanged(peeEmitter*){
}

void peeEmitterListener::ControllerNameChanged(peeEmitter*, peeController*){
}

void peeEmitterListener::ControllerChanged(peeEmitter*, peeController*){
}

void peeEmitterListener::ControllerValueChanged(peeEmitter*, peeController*){
}

void peeEmitterListener::ActiveControllerChanged(peeEmitter*){
}



void peeEmitterListener::TypeStructureChanged(peeEmitter*){
}

void peeEmitterListener::TypeChanged(peeEmitter*, peeType *type){
}

void peeEmitterListener::TypeActiveTrailControllerChanged(peeEmitter*, peeType *type){
}

void peeEmitterListener::TypeActiveEmitControllerChanged(peeEmitter*, peeType *type){
}

void peeEmitterListener::TypeParameterChanged(peeEmitter*, peeType *type, peeParameter *parameter){
}

void peeEmitterListener::ActiveTypeParameterChanged(peeEmitter*, peeType *type){
}

void peeEmitterListener::ActiveTypeChanged(peeEmitter*){
}
