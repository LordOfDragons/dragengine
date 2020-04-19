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

#include "deForceField.h"
#include "deForceFieldReference.h"



// Class deForceFieldReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deForceFieldReference::deForceFieldReference(){
}

deForceFieldReference::deForceFieldReference( deForceField *object ) :
deObjectReference( object ){
}

deForceFieldReference::deForceFieldReference( const deForceFieldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deForceFieldReference::TakeOver( deForceField *object ){
	deObjectReference::TakeOver( object );
}

bool deForceFieldReference::operator!() const{
	return deObjectReference::operator!();
}

deForceFieldReference::operator bool() const{
	return deObjectReference::operator bool();
}

deForceFieldReference::operator deForceField *() const{
	return ( deForceField* )operator deObject *();
}

deForceFieldReference::operator deForceField &() const{
	return ( deForceField& )operator deObject &();
}

deForceField *deForceFieldReference::operator->() const{
	return ( deForceField* )deObjectReference::operator->();
}

deForceFieldReference &deForceFieldReference::operator=( deForceField *object ){
	deObjectReference::operator=( object );
	return *this;
}

deForceFieldReference &deForceFieldReference::operator=( const deForceFieldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deForceFieldReference::operator==( deForceField *object ) const{
	return deObjectReference::operator==( object );
}

bool deForceFieldReference::operator!=( deForceField *object ) const{
	return deObjectReference::operator!=( object );
}

bool deForceFieldReference::operator==( const deForceFieldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deForceFieldReference::operator!=( const deForceFieldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
