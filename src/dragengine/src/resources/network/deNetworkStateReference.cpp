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

#include "deNetworkState.h"
#include "deNetworkStateReference.h"



// Class deNetworkStateReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkStateReference::deNetworkStateReference(){
}

deNetworkStateReference::deNetworkStateReference( deNetworkState *object ) :
deObjectReference( object ){
}

deNetworkStateReference::deNetworkStateReference( const deNetworkStateReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNetworkStateReference::TakeOver( deNetworkState *object ){
	deObjectReference::TakeOver( object );
}

bool deNetworkStateReference::operator!() const{
	return deObjectReference::operator!();
}

deNetworkStateReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNetworkStateReference::operator deNetworkState *() const{
	return ( deNetworkState* )operator deObject *();
}

deNetworkStateReference::operator deNetworkState &() const{
	return ( deNetworkState& )operator deObject &();
}

deNetworkState *deNetworkStateReference::operator->() const{
	return ( deNetworkState* )deObjectReference::operator->();
}

deNetworkStateReference &deNetworkStateReference::operator=( deNetworkState *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNetworkStateReference &deNetworkStateReference::operator=( const deNetworkStateReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNetworkStateReference::operator==( deNetworkState *object ) const{
	return deObjectReference::operator==( object );
}

bool deNetworkStateReference::operator!=( deNetworkState *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNetworkStateReference::operator==( const deNetworkStateReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNetworkStateReference::operator!=( const deNetworkStateReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
