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

#include "deNetworkValue.h"
#include "deNetworkValueReference.h"



// Class deNetworkValueReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueReference::deNetworkValueReference(){
}

deNetworkValueReference::deNetworkValueReference( deNetworkValue *object ) :
deObjectReference( object ){
}

deNetworkValueReference::deNetworkValueReference( const deNetworkValueReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNetworkValueReference::TakeOver( deNetworkValue *object ){
	deObjectReference::TakeOver( object );
}

bool deNetworkValueReference::operator!() const{
	return deObjectReference::operator!();
}

deNetworkValueReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNetworkValueReference::operator deNetworkValue *() const{
	return ( deNetworkValue* )operator deObject *();
}

deNetworkValueReference::operator deNetworkValue &() const{
	return ( deNetworkValue& )operator deObject &();
}

deNetworkValue *deNetworkValueReference::operator->() const{
	return ( deNetworkValue* )deObjectReference::operator->();
}

deNetworkValueReference &deNetworkValueReference::operator=( deNetworkValue *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNetworkValueReference &deNetworkValueReference::operator=( const deNetworkValueReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNetworkValueReference::operator==( deNetworkValue *object ) const{
	return deObjectReference::operator==( object );
}

bool deNetworkValueReference::operator!=( deNetworkValue *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNetworkValueReference::operator==( const deNetworkValueReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNetworkValueReference::operator!=( const deNetworkValueReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
