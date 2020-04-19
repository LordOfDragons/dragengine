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

#include "deAnimation.h"
#include "deAnimationReference.h"



// Class deAnimationReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimationReference::deAnimationReference(){
}

deAnimationReference::deAnimationReference( deAnimation *object ) :
deObjectReference( object ){
}

deAnimationReference::deAnimationReference( const deAnimationReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimationReference::TakeOver( deAnimation *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimationReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimationReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimationReference::operator deAnimation *() const{
	return ( deAnimation* )operator deObject *();
}

deAnimationReference::operator deAnimation &() const{
	return ( deAnimation& )operator deObject &();
}

deAnimation *deAnimationReference::operator->() const{
	return ( deAnimation* )deObjectReference::operator->();
}

deAnimationReference &deAnimationReference::operator=( deAnimation *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimationReference &deAnimationReference::operator=( const deAnimationReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimationReference::operator==( deAnimation *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimationReference::operator!=( deAnimation *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimationReference::operator==( const deAnimationReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimationReference::operator!=( const deAnimationReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
