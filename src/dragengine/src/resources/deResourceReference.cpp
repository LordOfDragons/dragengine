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

#include "deResource.h"
#include "deResourceReference.h"



// Class deResourceReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

deResourceReference::deResourceReference(){
}

deResourceReference::deResourceReference( deResource *object ) :
deObjectReference( object ){
}

deResourceReference::deResourceReference( const deResourceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deResourceReference::TakeOver( deResource *object ){
	deObjectReference::TakeOver( object );
}

bool deResourceReference::operator!() const{
	return deObjectReference::operator!();
}

deResourceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deResourceReference::operator deResource *() const{
	return ( deResource* )operator deObject *();
}

deResourceReference::operator deResource &() const{
	return ( deResource& )operator deObject &();
}

deResource *deResourceReference::operator->() const{
	return ( deResource* )deObjectReference::operator->();
}

deResourceReference &deResourceReference::operator=( deResource *object ){
	deObjectReference::operator=( object );
	return *this;
}

deResourceReference &deResourceReference::operator=( const deResourceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deResourceReference::operator==( deResource *object ) const{
	return deObjectReference::operator==( object );
}

bool deResourceReference::operator!=( deResource *object ) const{
	return deObjectReference::operator!=( object );
}

bool deResourceReference::operator==( const deResourceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deResourceReference::operator!=( const deResourceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
