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

#include "deParticleEmitter.h"
#include "deParticleEmitterReference.h"



// Class deParticleEmitterReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterReference::deParticleEmitterReference(){
}

deParticleEmitterReference::deParticleEmitterReference( deParticleEmitter *object ) :
deObjectReference( object ){
}

deParticleEmitterReference::deParticleEmitterReference( const deParticleEmitterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deParticleEmitterReference::TakeOver( deParticleEmitter *object ){
	deObjectReference::TakeOver( object );
}

bool deParticleEmitterReference::operator!() const{
	return deObjectReference::operator!();
}

deParticleEmitterReference::operator bool() const{
	return deObjectReference::operator bool();
}

deParticleEmitterReference::operator deParticleEmitter *() const{
	return ( deParticleEmitter* )operator deObject *();
}

deParticleEmitterReference::operator deParticleEmitter &() const{
	return ( deParticleEmitter& )operator deObject &();
}

deParticleEmitter *deParticleEmitterReference::operator->() const{
	return ( deParticleEmitter* )deObjectReference::operator->();
}

deParticleEmitterReference &deParticleEmitterReference::operator=( deParticleEmitter *object ){
	deObjectReference::operator=( object );
	return *this;
}

deParticleEmitterReference &deParticleEmitterReference::operator=( const deParticleEmitterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deParticleEmitterReference::operator==( deParticleEmitter *object ) const{
	return deObjectReference::operator==( object );
}

bool deParticleEmitterReference::operator!=( deParticleEmitter *object ) const{
	return deObjectReference::operator!=( object );
}

bool deParticleEmitterReference::operator==( const deParticleEmitterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deParticleEmitterReference::operator!=( const deParticleEmitterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
