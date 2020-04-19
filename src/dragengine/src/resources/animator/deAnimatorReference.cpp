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

#include "deAnimator.h"
#include "deAnimatorReference.h"



// Class deAnimatorReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorReference::deAnimatorReference(){
}

deAnimatorReference::deAnimatorReference( deAnimator *object ) :
deObjectReference( object ){
}

deAnimatorReference::deAnimatorReference( const deAnimatorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimatorReference::TakeOver( deAnimator *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimatorReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimatorReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimatorReference::operator deAnimator *() const{
	return ( deAnimator* )operator deObject *();
}

deAnimatorReference::operator deAnimator &() const{
	return ( deAnimator& )operator deObject &();
}

deAnimator *deAnimatorReference::operator->() const{
	return ( deAnimator* )deObjectReference::operator->();
}

deAnimatorReference &deAnimatorReference::operator=( deAnimator *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimatorReference &deAnimatorReference::operator=( const deAnimatorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimatorReference::operator==( deAnimator *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimatorReference::operator!=( deAnimator *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimatorReference::operator==( const deAnimatorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimatorReference::operator!=( const deAnimatorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
