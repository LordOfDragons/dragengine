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

#include "deNavigationBlocker.h"
#include "deNavigationBlockerReference.h"



// Class deNavigationBlockerReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationBlockerReference::deNavigationBlockerReference(){
}

deNavigationBlockerReference::deNavigationBlockerReference( deNavigationBlocker *object ) :
deObjectReference( object ){
}

deNavigationBlockerReference::deNavigationBlockerReference( const deNavigationBlockerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNavigationBlockerReference::TakeOver( deNavigationBlocker *object ){
	deObjectReference::TakeOver( object );
}

bool deNavigationBlockerReference::operator!() const{
	return deObjectReference::operator!();
}

deNavigationBlockerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNavigationBlockerReference::operator deNavigationBlocker *() const{
	return ( deNavigationBlocker* )operator deObject *();
}

deNavigationBlockerReference::operator deNavigationBlocker &() const{
	return ( deNavigationBlocker& )operator deObject &();
}

deNavigationBlocker *deNavigationBlockerReference::operator->() const{
	return ( deNavigationBlocker* )deObjectReference::operator->();
}

deNavigationBlockerReference &deNavigationBlockerReference::operator=( deNavigationBlocker *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNavigationBlockerReference &deNavigationBlockerReference::operator=( const deNavigationBlockerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNavigationBlockerReference::operator==( deNavigationBlocker *object ) const{
	return deObjectReference::operator==( object );
}

bool deNavigationBlockerReference::operator!=( deNavigationBlocker *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNavigationBlockerReference::operator==( const deNavigationBlockerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNavigationBlockerReference::operator!=( const deNavigationBlockerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
