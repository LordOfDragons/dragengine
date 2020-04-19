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

#include "deColliderCollisionTest.h"
#include "deColliderCollisionTestReference.h"



// Class deColliderCollisionTestReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deColliderCollisionTestReference::deColliderCollisionTestReference(){
}

deColliderCollisionTestReference::deColliderCollisionTestReference( deColliderCollisionTest *object ) :
deObjectReference( object ){
}

deColliderCollisionTestReference::deColliderCollisionTestReference( const deColliderCollisionTestReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deColliderCollisionTestReference::TakeOver( deColliderCollisionTest *object ){
	deObjectReference::TakeOver( object );
}

bool deColliderCollisionTestReference::operator!() const{
	return deObjectReference::operator!();
}

deColliderCollisionTestReference::operator bool() const{
	return deObjectReference::operator bool();
}

deColliderCollisionTestReference::operator deColliderCollisionTest *() const{
	return ( deColliderCollisionTest* )operator deObject *();
}

deColliderCollisionTestReference::operator deColliderCollisionTest &() const{
	return ( deColliderCollisionTest& )operator deObject &();
}

deColliderCollisionTest *deColliderCollisionTestReference::operator->() const{
	return ( deColliderCollisionTest* )deObjectReference::operator->();
}

deColliderCollisionTestReference &deColliderCollisionTestReference::operator=( deColliderCollisionTest *object ){
	deObjectReference::operator=( object );
	return *this;
}

deColliderCollisionTestReference &deColliderCollisionTestReference::operator=( const deColliderCollisionTestReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deColliderCollisionTestReference::operator==( deColliderCollisionTest *object ) const{
	return deObjectReference::operator==( object );
}

bool deColliderCollisionTestReference::operator!=( deColliderCollisionTest *object ) const{
	return deObjectReference::operator!=( object );
}

bool deColliderCollisionTestReference::operator==( const deColliderCollisionTestReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deColliderCollisionTestReference::operator!=( const deColliderCollisionTestReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
