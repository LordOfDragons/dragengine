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

#include "deLight.h"
#include "deLightReference.h"



// Class deLightReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deLightReference::deLightReference(){
}

deLightReference::deLightReference( deLight *object ) :
deObjectReference( object ){
}

deLightReference::deLightReference( const deLightReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deLightReference::TakeOver( deLight *object ){
	deObjectReference::TakeOver( object );
}

bool deLightReference::operator!() const{
	return deObjectReference::operator!();
}

deLightReference::operator bool() const{
	return deObjectReference::operator bool();
}

deLightReference::operator deLight *() const{
	return ( deLight* )operator deObject *();
}

deLightReference::operator deLight &() const{
	return ( deLight& )operator deObject &();
}

deLight *deLightReference::operator->() const{
	return ( deLight* )deObjectReference::operator->();
}

deLightReference &deLightReference::operator=( deLight *object ){
	deObjectReference::operator=( object );
	return *this;
}

deLightReference &deLightReference::operator=( const deLightReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deLightReference::operator==( deLight *object ) const{
	return deObjectReference::operator==( object );
}

bool deLightReference::operator!=( deLight *object ) const{
	return deObjectReference::operator!=( object );
}

bool deLightReference::operator==( const deLightReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deLightReference::operator!=( const deLightReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
