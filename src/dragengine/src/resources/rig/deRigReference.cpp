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

#include "deRig.h"
#include "deRigReference.h"



// Class deRigReference
/////////////////////////

// Constructor, destructor
////////////////////////////

deRigReference::deRigReference(){
}

deRigReference::deRigReference( deRig *object ) :
deObjectReference( object ){
}

deRigReference::deRigReference( const deRigReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deRigReference::TakeOver( deRig *object ){
	deObjectReference::TakeOver( object );
}

bool deRigReference::operator!() const{
	return deObjectReference::operator!();
}

deRigReference::operator bool() const{
	return deObjectReference::operator bool();
}

deRigReference::operator deRig *() const{
	return ( deRig* )operator deObject *();
}

deRigReference::operator deRig &() const{
	return ( deRig& )operator deObject &();
}

deRig *deRigReference::operator->() const{
	return ( deRig* )deObjectReference::operator->();
}

deRigReference &deRigReference::operator=( deRig *object ){
	deObjectReference::operator=( object );
	return *this;
}

deRigReference &deRigReference::operator=( const deRigReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deRigReference::operator==( deRig *object ) const{
	return deObjectReference::operator==( object );
}

bool deRigReference::operator==( const deRigReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deRigReference::operator!=( deRig *object ) const{
	return deObjectReference::operator!=( object );
}

bool deRigReference::operator!=( const deRigReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
