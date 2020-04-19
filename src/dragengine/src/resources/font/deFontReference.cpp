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

#include "deFont.h"
#include "deFontReference.h"



// Class deFontReference
//////////////////////////

// Constructor, destructor
////////////////////////////

deFontReference::deFontReference(){
}

deFontReference::deFontReference( deFont *object ) :
deObjectReference( object ){
}

deFontReference::deFontReference( const deFontReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deFontReference::TakeOver( deFont *object ){
	deObjectReference::TakeOver( object );
}

bool deFontReference::operator!() const{
	return deObjectReference::operator!();
}

deFontReference::operator bool() const{
	return deObjectReference::operator bool();
}

deFontReference::operator deFont *() const{
	return ( deFont* )operator deObject *();
}

deFontReference::operator deFont &() const{
	return ( deFont& )operator deObject &();
}

deFont *deFontReference::operator->() const{
	return ( deFont* )deObjectReference::operator->();
}

deFontReference &deFontReference::operator=( deFont *object ){
	deObjectReference::operator=( object );
	return *this;
}

deFontReference &deFontReference::operator=( const deFontReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deFontReference::operator==( deFont *object ) const{
	return deObjectReference::operator==( object );
}

bool deFontReference::operator!=( deFont *object ) const{
	return deObjectReference::operator!=( object );
}

bool deFontReference::operator==( const deFontReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deFontReference::operator!=( const deFontReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
