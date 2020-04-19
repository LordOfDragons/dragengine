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

#include "deComponent.h"
#include "deComponentReference.h"



// Class deComponentReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deComponentReference::deComponentReference(){
}

deComponentReference::deComponentReference( deComponent *object ) :
deObjectReference( object ){
}

deComponentReference::deComponentReference( const deComponentReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deComponentReference::TakeOver( deComponent *object ){
	deObjectReference::TakeOver( object );
}

bool deComponentReference::operator!() const{
	return deObjectReference::operator!();
}

deComponentReference::operator bool() const{
	return deObjectReference::operator bool();
}

deComponentReference::operator deComponent *() const{
	return ( deComponent* )operator deObject *();
}

deComponentReference::operator deComponent &() const{
	return ( deComponent& )operator deObject &();
}

deComponent *deComponentReference::operator->() const{
	return ( deComponent* )deObjectReference::operator->();
}

deComponentReference &deComponentReference::operator=( deComponent *object ){
	deObjectReference::operator=( object );
	return *this;
}

deComponentReference &deComponentReference::operator=( const deComponentReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deComponentReference::operator==( deComponent *object ) const{
	return deObjectReference::operator==( object );
}

bool deComponentReference::operator!=( deComponent *object ) const{
	return deObjectReference::operator!=( object );
}

bool deComponentReference::operator==( const deComponentReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deComponentReference::operator!=( const deComponentReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
