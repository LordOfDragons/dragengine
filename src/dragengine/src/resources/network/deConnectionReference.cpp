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

#include "deConnection.h"
#include "deConnectionReference.h"



// Class deConnectionReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deConnectionReference::deConnectionReference(){
}

deConnectionReference::deConnectionReference( deConnection *object ) :
deObjectReference( object ){
}

deConnectionReference::deConnectionReference( const deConnectionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deConnectionReference::TakeOver( deConnection *object ){
	deObjectReference::TakeOver( object );
}

bool deConnectionReference::operator!() const{
	return deObjectReference::operator!();
}

deConnectionReference::operator bool() const{
	return deObjectReference::operator bool();
}

deConnectionReference::operator deConnection *() const{
	return ( deConnection* )operator deObject *();
}

deConnectionReference::operator deConnection &() const{
	return ( deConnection& )operator deObject &();
}

deConnection *deConnectionReference::operator->() const{
	return ( deConnection* )deObjectReference::operator->();
}

deConnectionReference &deConnectionReference::operator=( deConnection *object ){
	deObjectReference::operator=( object );
	return *this;
}

deConnectionReference &deConnectionReference::operator=( const deConnectionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deConnectionReference::operator==( deConnection *object ) const{
	return deObjectReference::operator==( object );
}

bool deConnectionReference::operator!=( deConnection *object ) const{
	return deObjectReference::operator!=( object );
}

bool deConnectionReference::operator==( const deConnectionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deConnectionReference::operator!=( const deConnectionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
