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

#include "deWorld.h"
#include "deWorldReference.h"



// Class deWorldReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deWorldReference::deWorldReference(){
}

deWorldReference::deWorldReference( deWorld *object ) :
deObjectReference( object ){
}

deWorldReference::deWorldReference( const deWorldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deWorldReference::TakeOver( deWorld *object ){
	deObjectReference::TakeOver( object );
}

bool deWorldReference::operator!() const{
	return deObjectReference::operator!();
}

deWorldReference::operator bool() const{
	return deObjectReference::operator bool();
}

deWorldReference::operator deWorld *() const{
	return ( deWorld* )operator deObject *();
}

deWorldReference::operator deWorld &() const{
	return ( deWorld& )operator deObject &();
}

deWorld *deWorldReference::operator->() const{
	return ( deWorld* )deObjectReference::operator->();
}

deWorldReference &deWorldReference::operator=( deWorld *object ){
	deObjectReference::operator=( object );
	return *this;
}

deWorldReference &deWorldReference::operator=( const deWorldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deWorldReference::operator==( deWorld *object ) const{
	return deObjectReference::operator==( object );
}

bool deWorldReference::operator!=( deWorld *object ) const{
	return deObjectReference::operator!=( object );
}

bool deWorldReference::operator==( const deWorldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deWorldReference::operator!=( const deWorldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
