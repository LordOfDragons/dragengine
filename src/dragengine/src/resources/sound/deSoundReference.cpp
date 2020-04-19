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

#include "deSound.h"
#include "deSoundReference.h"



// Class deSoundReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deSoundReference::deSoundReference(){
}

deSoundReference::deSoundReference( deSound *object ) :
deObjectReference( object ){
}

deSoundReference::deSoundReference( const deSoundReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSoundReference::TakeOver( deSound *object ){
	deObjectReference::TakeOver( object );
}

bool deSoundReference::operator!() const{
	return deObjectReference::operator!();
}

deSoundReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSoundReference::operator deSound *() const{
	return ( deSound* )operator deObject *();
}

deSoundReference::operator deSound &() const{
	return ( deSound& )operator deObject &();
}

deSound *deSoundReference::operator->() const{
	return ( deSound* )deObjectReference::operator->();
}

deSoundReference &deSoundReference::operator=( deSound *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSoundReference &deSoundReference::operator=( const deSoundReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSoundReference::operator==( deSound *object ) const{
	return deObjectReference::operator==( object );
}

bool deSoundReference::operator!=( deSound *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSoundReference::operator==( const deSoundReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSoundReference::operator!=( const deSoundReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
