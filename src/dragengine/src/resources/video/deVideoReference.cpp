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

#include "deVideo.h"
#include "deVideoReference.h"



// Class deVideoReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deVideoReference::deVideoReference(){
}

deVideoReference::deVideoReference( deVideo *object ) :
deObjectReference( object ){
}

deVideoReference::deVideoReference( const deVideoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVideoReference::TakeOver( deVideo *object ){
	deObjectReference::TakeOver( object );
}

bool deVideoReference::operator!() const{
	return deObjectReference::operator!();
}

deVideoReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVideoReference::operator deVideo *() const{
	return ( deVideo* )operator deObject *();
}

deVideoReference::operator deVideo &() const{
	return ( deVideo& )operator deObject &();
}

deVideo *deVideoReference::operator->() const{
	return ( deVideo* )deObjectReference::operator->();
}

deVideoReference &deVideoReference::operator=( deVideo *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVideoReference &deVideoReference::operator=( const deVideoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVideoReference::operator==( deVideo *object ) const{
	return deObjectReference::operator==( object );
}

bool deVideoReference::operator!=( deVideo *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVideoReference::operator==( const deVideoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVideoReference::operator!=( const deVideoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
