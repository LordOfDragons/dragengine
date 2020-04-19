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

#include "deNavigator.h"
#include "deNavigatorReference.h"



// Class deNavigatorReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigatorReference::deNavigatorReference(){
}

deNavigatorReference::deNavigatorReference( deNavigator *object ) :
deObjectReference( object ){
}

deNavigatorReference::deNavigatorReference( const deNavigatorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNavigatorReference::TakeOver( deNavigator *object ){
	deObjectReference::TakeOver( object );
}

bool deNavigatorReference::operator!() const{
	return deObjectReference::operator!();
}

deNavigatorReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNavigatorReference::operator deNavigator *() const{
	return ( deNavigator* )operator deObject *();
}

deNavigatorReference::operator deNavigator &() const{
	return ( deNavigator& )operator deObject &();
}

deNavigator *deNavigatorReference::operator->() const{
	return ( deNavigator* )deObjectReference::operator->();
}

deNavigatorReference &deNavigatorReference::operator=( deNavigator *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNavigatorReference &deNavigatorReference::operator=( const deNavigatorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNavigatorReference::operator==( deNavigator *object ) const{
	return deObjectReference::operator==( object );
}

bool deNavigatorReference::operator!=( deNavigator *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNavigatorReference::operator==( const deNavigatorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNavigatorReference::operator!=( const deNavigatorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
