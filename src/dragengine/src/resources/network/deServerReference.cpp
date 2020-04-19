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

#include "deServer.h"
#include "deServerReference.h"



// Class deServerReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deServerReference::deServerReference(){
}

deServerReference::deServerReference( deServer *object ) :
deObjectReference( object ){
}

deServerReference::deServerReference( const deServerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deServerReference::TakeOver( deServer *object ){
	deObjectReference::TakeOver( object );
}

bool deServerReference::operator!() const{
	return deObjectReference::operator!();
}

deServerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deServerReference::operator deServer *() const{
	return ( deServer* )operator deObject *();
}

deServerReference::operator deServer &() const{
	return ( deServer& )operator deObject &();
}

deServer *deServerReference::operator->() const{
	return ( deServer* )deObjectReference::operator->();
}

deServerReference &deServerReference::operator=( deServer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deServerReference &deServerReference::operator=( const deServerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deServerReference::operator==( deServer *object ) const{
	return deObjectReference::operator==( object );
}

bool deServerReference::operator!=( deServer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deServerReference::operator==( const deServerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deServerReference::operator!=( const deServerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
