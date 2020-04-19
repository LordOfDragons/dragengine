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

#include "deSky.h"
#include "deSkyReference.h"



// Class deSkyReference
/////////////////////////

// Constructor, destructor
////////////////////////////

deSkyReference::deSkyReference(){
}

deSkyReference::deSkyReference( deSky *object ) :
deObjectReference( object ){
}

deSkyReference::deSkyReference( const deSkyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSkyReference::TakeOver( deSky *object ){
	deObjectReference::TakeOver( object );
}

bool deSkyReference::operator!() const{
	return deObjectReference::operator!();
}

deSkyReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSkyReference::operator deSky *() const{
	return ( deSky* )operator deObject *();
}

deSkyReference::operator deSky &() const{
	return ( deSky& )operator deObject &();
}

deSky *deSkyReference::operator->() const{
	return ( deSky* )deObjectReference::operator->();
}

deSkyReference &deSkyReference::operator=( deSky *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSkyReference &deSkyReference::operator=( const deSkyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSkyReference::operator==( deSky *object ) const{
	return deObjectReference::operator==( object );
}

bool deSkyReference::operator!=( deSky *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSkyReference::operator==( const deSkyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSkyReference::operator!=( const deSkyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
