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

#include "deNavigationSpace.h"
#include "deNavigationSpaceReference.h"



// Class deNavigationSpaceReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationSpaceReference::deNavigationSpaceReference(){
}

deNavigationSpaceReference::deNavigationSpaceReference( deNavigationSpace *object ) :
deObjectReference( object ){
}

deNavigationSpaceReference::deNavigationSpaceReference( const deNavigationSpaceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNavigationSpaceReference::TakeOver( deNavigationSpace *object ){
	deObjectReference::TakeOver( object );
}

bool deNavigationSpaceReference::operator!() const{
	return deObjectReference::operator!();
}

deNavigationSpaceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNavigationSpaceReference::operator deNavigationSpace *() const{
	return ( deNavigationSpace* )operator deObject *();
}

deNavigationSpaceReference::operator deNavigationSpace &() const{
	return ( deNavigationSpace& )operator deObject &();
}

deNavigationSpace *deNavigationSpaceReference::operator->() const{
	return ( deNavigationSpace* )deObjectReference::operator->();
}

deNavigationSpaceReference &deNavigationSpaceReference::operator=( deNavigationSpace *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNavigationSpaceReference &deNavigationSpaceReference::operator=( const deNavigationSpaceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNavigationSpaceReference::operator==( deNavigationSpace *object ) const{
	return deObjectReference::operator==( object );
}

bool deNavigationSpaceReference::operator!=( deNavigationSpace *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNavigationSpaceReference::operator==( const deNavigationSpaceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNavigationSpaceReference::operator!=( const deNavigationSpaceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
