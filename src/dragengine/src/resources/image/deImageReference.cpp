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

#include "deImage.h"
#include "deImageReference.h"



// Class deImageReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deImageReference::deImageReference(){
}

deImageReference::deImageReference( deImage *object ) :
deObjectReference( object ){
}

deImageReference::deImageReference( const deImageReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deImageReference::TakeOver( deImage *object ){
	deObjectReference::TakeOver( object );
}

bool deImageReference::operator!() const{
	return deObjectReference::operator!();
}

deImageReference::operator bool() const{
	return deObjectReference::operator bool();
}

deImageReference::operator deImage *() const{
	return ( deImage* )operator deObject *();
}

deImageReference::operator deImage &() const{
	return ( deImage& )operator deObject &();
}

deImage *deImageReference::operator->() const{
	return ( deImage* )deObjectReference::operator->();
}

deImageReference &deImageReference::operator=( deImage *object ){
	deObjectReference::operator=( object );
	return *this;
}

deImageReference &deImageReference::operator=( const deImageReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deImageReference::operator==( deImage *object ) const{
	return deObjectReference::operator==( object );
}

bool deImageReference::operator!=( deImage *object ) const{
	return deObjectReference::operator!=( object );
}

bool deImageReference::operator==( const deImageReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deImageReference::operator!=( const deImageReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
