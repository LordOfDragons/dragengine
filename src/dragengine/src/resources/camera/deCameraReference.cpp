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

#include "deCamera.h"
#include "deCameraReference.h"



// Class deCameraReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deCameraReference::deCameraReference(){
}

deCameraReference::deCameraReference( deCamera *object ) :
deObjectReference( object ){
}

deCameraReference::deCameraReference( const deCameraReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCameraReference::TakeOver( deCamera *object ){
	deObjectReference::TakeOver( object );
}

bool deCameraReference::operator!() const{
	return deObjectReference::operator!();
}

deCameraReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCameraReference::operator deCamera *() const{
	return ( deCamera* )operator deObject *();
}

deCameraReference::operator deCamera &() const{
	return ( deCamera& )operator deObject &();
}

deCamera *deCameraReference::operator->() const{
	return ( deCamera* )deObjectReference::operator->();
}

deCameraReference &deCameraReference::operator=( deCamera *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCameraReference &deCameraReference::operator=( const deCameraReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCameraReference::operator==( deCamera *object ) const{
	return deObjectReference::operator==( object );
}

bool deCameraReference::operator!=( deCamera *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCameraReference::operator==( const deCameraReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCameraReference::operator!=( const deCameraReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
