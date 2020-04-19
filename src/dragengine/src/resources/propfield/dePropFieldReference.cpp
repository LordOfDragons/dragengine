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

#include "dePropField.h"
#include "dePropFieldReference.h"



// Class dePropFieldReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldReference::dePropFieldReference(){
}

dePropFieldReference::dePropFieldReference( dePropField *object ) :
deObjectReference( object ){
}

dePropFieldReference::dePropFieldReference( const dePropFieldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void dePropFieldReference::TakeOver( dePropField *object ){
	deObjectReference::TakeOver( object );
}

bool dePropFieldReference::operator!() const{
	return deObjectReference::operator!();
}

dePropFieldReference::operator bool() const{
	return deObjectReference::operator bool();
}

dePropFieldReference::operator dePropField *() const{
	return ( dePropField* )operator deObject *();
}

dePropFieldReference::operator dePropField &() const{
	return ( dePropField& )operator deObject &();
}

dePropField *dePropFieldReference::operator->() const{
	return ( dePropField* )deObjectReference::operator->();
}

dePropFieldReference &dePropFieldReference::operator=( dePropField *object ){
	deObjectReference::operator=( object );
	return *this;
}

dePropFieldReference &dePropFieldReference::operator=( const dePropFieldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool dePropFieldReference::operator==( dePropField *object ) const{
	return deObjectReference::operator==( object );
}

bool dePropFieldReference::operator!=( dePropField *object ) const{
	return deObjectReference::operator!=( object );
}

bool dePropFieldReference::operator==( const dePropFieldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool dePropFieldReference::operator!=( const dePropFieldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
