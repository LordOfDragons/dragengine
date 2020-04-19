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

#include "deAnimatorInstance.h"
#include "deAnimatorInstanceReference.h"



// Class deAnimatorInstanceReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorInstanceReference::deAnimatorInstanceReference(){
}

deAnimatorInstanceReference::deAnimatorInstanceReference( deAnimatorInstance *object ) :
deObjectReference( object ){
}

deAnimatorInstanceReference::deAnimatorInstanceReference( const deAnimatorInstanceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimatorInstanceReference::TakeOver( deAnimatorInstance *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimatorInstanceReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimatorInstanceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimatorInstanceReference::operator deAnimatorInstance *() const{
	return ( deAnimatorInstance* )operator deObject *();
}

deAnimatorInstanceReference::operator deAnimatorInstance &() const{
	return ( deAnimatorInstance& )operator deObject &();
}

deAnimatorInstance *deAnimatorInstanceReference::operator->() const{
	return ( deAnimatorInstance* )deObjectReference::operator->();
}

deAnimatorInstanceReference &deAnimatorInstanceReference::operator=( deAnimatorInstance *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimatorInstanceReference &deAnimatorInstanceReference::operator=( const deAnimatorInstanceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimatorInstanceReference::operator==( deAnimatorInstance *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimatorInstanceReference::operator!=( deAnimatorInstance *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimatorInstanceReference::operator==( const deAnimatorInstanceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimatorInstanceReference::operator!=( const deAnimatorInstanceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
