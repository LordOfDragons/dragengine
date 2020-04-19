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

#include "deDecal.h"
#include "deDecalReference.h"



// Class deDecalReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deDecalReference::deDecalReference(){
}

deDecalReference::deDecalReference( deDecal *object ) :
deObjectReference( object ){
}

deDecalReference::deDecalReference( const deDecalReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deDecalReference::TakeOver( deDecal *object ){
	deObjectReference::TakeOver( object );
}

bool deDecalReference::operator!() const{
	return deObjectReference::operator!();
}

deDecalReference::operator bool() const{
	return deObjectReference::operator bool();
}

deDecalReference::operator deDecal *() const{
	return ( deDecal* )operator deObject *();
}

deDecalReference::operator deDecal &() const{
	return ( deDecal& )operator deObject &();
}

deDecal *deDecalReference::operator->() const{
	return ( deDecal* )deObjectReference::operator->();
}

deDecalReference &deDecalReference::operator=( deDecal *object ){
	deObjectReference::operator=( object );
	return *this;
}

deDecalReference &deDecalReference::operator=( const deDecalReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deDecalReference::operator==( deDecal *object ) const{
	return deObjectReference::operator==( object );
}

bool deDecalReference::operator!=( deDecal *object ) const{
	return deObjectReference::operator!=( object );
}

bool deDecalReference::operator==( const deDecalReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deDecalReference::operator!=( const deDecalReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
