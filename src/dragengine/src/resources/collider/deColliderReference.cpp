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

#include "deCollider.h"
#include "deColliderReference.h"



// Class deColliderReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

deColliderReference::deColliderReference(){
}

deColliderReference::deColliderReference( deCollider *object ) :
deObjectReference( object ){
}

deColliderReference::deColliderReference( const deColliderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deColliderReference::TakeOver( deCollider *object ){
	deObjectReference::TakeOver( object );
}

bool deColliderReference::operator!() const{
	return deObjectReference::operator!();
}

deColliderReference::operator bool() const{
	return deObjectReference::operator bool();
}

deColliderReference::operator deCollider *() const{
	return ( deCollider* )operator deObject *();
}

deColliderReference::operator deCollider &() const{
	return ( deCollider& )operator deObject &();
}

deCollider *deColliderReference::operator->() const{
	return ( deCollider* )deObjectReference::operator->();
}

deColliderReference &deColliderReference::operator=( deCollider *object ){
	deObjectReference::operator=( object );
	return *this;
}

deColliderReference &deColliderReference::operator=( const deColliderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deColliderReference::operator==( deCollider *object ) const{
	return deObjectReference::operator==( object );
}

bool deColliderReference::operator!=( deCollider *object ) const{
	return deObjectReference::operator!=( object );
}

bool deColliderReference::operator==( const deColliderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deColliderReference::operator!=( const deColliderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
