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

#include "deRenderWindow.h"
#include "deRenderWindowReference.h"



// Class deRenderWindowReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deRenderWindowReference::deRenderWindowReference(){
}

deRenderWindowReference::deRenderWindowReference( deRenderWindow *object ) :
deObjectReference( object ){
}

deRenderWindowReference::deRenderWindowReference( const deRenderWindowReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deRenderWindowReference::TakeOver( deRenderWindow *object ){
	deObjectReference::TakeOver( object );
}

bool deRenderWindowReference::operator!() const{
	return deObjectReference::operator!();
}

deRenderWindowReference::operator bool() const{
	return deObjectReference::operator bool();
}

deRenderWindowReference::operator deRenderWindow *() const{
	return ( deRenderWindow* )operator deObject *();
}

deRenderWindowReference::operator deRenderWindow &() const{
	return ( deRenderWindow& )operator deObject &();
}

deRenderWindow *deRenderWindowReference::operator->() const{
	return ( deRenderWindow* )deObjectReference::operator->();
}

deRenderWindowReference &deRenderWindowReference::operator=( deRenderWindow *object ){
	deObjectReference::operator=( object );
	return *this;
}

deRenderWindowReference &deRenderWindowReference::operator=( const deRenderWindowReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deRenderWindowReference::operator==( deRenderWindow *object ) const{
	return deObjectReference::operator==( object );
}

bool deRenderWindowReference::operator!=( deRenderWindow *object ) const{
	return deObjectReference::operator!=( object );
}

bool deRenderWindowReference::operator==( const deRenderWindowReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deRenderWindowReference::operator!=( const deRenderWindowReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
