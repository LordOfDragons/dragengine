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

#include "deParticleEmitterInstance.h"
#include "deParticleEmitterInstanceReference.h"



// Class deParticleEmitterInstanceReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterInstanceReference::deParticleEmitterInstanceReference(){
}

deParticleEmitterInstanceReference::deParticleEmitterInstanceReference( deParticleEmitterInstance *object ) :
deObjectReference( object ){
}

deParticleEmitterInstanceReference::deParticleEmitterInstanceReference( const deParticleEmitterInstanceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deParticleEmitterInstanceReference::TakeOver( deParticleEmitterInstance *object ){
	deObjectReference::TakeOver( object );
}

bool deParticleEmitterInstanceReference::operator!() const{
	return deObjectReference::operator!();
}

deParticleEmitterInstanceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deParticleEmitterInstanceReference::operator deParticleEmitterInstance *() const{
	return ( deParticleEmitterInstance* )operator deObject *();
}

deParticleEmitterInstanceReference::operator deParticleEmitterInstance &() const{
	return ( deParticleEmitterInstance& )operator deObject &();
}

deParticleEmitterInstance *deParticleEmitterInstanceReference::operator->() const{
	return ( deParticleEmitterInstance* )deObjectReference::operator->();
}

deParticleEmitterInstanceReference &deParticleEmitterInstanceReference::operator=( deParticleEmitterInstance *object ){
	deObjectReference::operator=( object );
	return *this;
}

deParticleEmitterInstanceReference &deParticleEmitterInstanceReference::operator=( const deParticleEmitterInstanceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deParticleEmitterInstanceReference::operator==( deParticleEmitterInstance *object ) const{
	return deObjectReference::operator==( object );
}

bool deParticleEmitterInstanceReference::operator!=( deParticleEmitterInstance *object ) const{
	return deObjectReference::operator!=( object );
}

bool deParticleEmitterInstanceReference::operator==( const deParticleEmitterInstanceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deParticleEmitterInstanceReference::operator!=( const deParticleEmitterInstanceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
