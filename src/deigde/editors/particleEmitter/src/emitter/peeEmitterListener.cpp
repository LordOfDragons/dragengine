/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

void peeEmitterListener::StateChanged( peeEmitter* ){
}

void peeEmitterListener::UndoChanged( peeEmitter* ){
}

void peeEmitterListener::EmitterChanged( peeEmitter* ){
}

void peeEmitterListener::SkyChanged( peeEmitter* ){
}

void peeEmitterListener::EnvObjectChanged( peeEmitter* ){
}

void peeEmitterListener::ViewChanged( peeEmitter* ){
}

void peeEmitterListener::CameraChanged( peeEmitter* ){
}



void peeEmitterListener::ControllerStructureChanged( peeEmitter* ){
}

void peeEmitterListener::ControllerNameChanged( peeEmitter*, peeController* ){
}

void peeEmitterListener::ControllerChanged( peeEmitter*, peeController* ){
}

void peeEmitterListener::ControllerValueChanged( peeEmitter*, peeController* ){
}

void peeEmitterListener::ActiveControllerChanged( peeEmitter* ){
}



void peeEmitterListener::TypeStructureChanged( peeEmitter* ){
}

void peeEmitterListener::TypeChanged( peeEmitter*, peeType *type ){
}

void peeEmitterListener::TypeActiveTrailControllerChanged( peeEmitter*, peeType *type ){
}

void peeEmitterListener::TypeActiveEmitControllerChanged( peeEmitter*, peeType *type ){
}

void peeEmitterListener::TypeParameterChanged( peeEmitter*, peeType *type, peeParameter *parameter ){
}

void peeEmitterListener::ActiveTypeParameterChanged( peeEmitter*, peeType *type ){
}

void peeEmitterListener::ActiveTypeChanged( peeEmitter* ){
}
