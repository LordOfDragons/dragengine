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

#include "deSkin.h"
#include "deSkinReference.h"



// Class deSkinReference
//////////////////////////

// Constructor, destructor
////////////////////////////

deSkinReference::deSkinReference(){
}

deSkinReference::deSkinReference( deSkin *object ) :
deObjectReference( object ){
}

deSkinReference::deSkinReference( const deSkinReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSkinReference::TakeOver( deSkin *object ){
	deObjectReference::TakeOver( object );
}

bool deSkinReference::operator!() const{
	return deObjectReference::operator!();
}

deSkinReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSkinReference::operator deSkin *() const{
	return ( deSkin* )operator deObject *();
}

deSkinReference::operator deSkin &() const{
	return ( deSkin& )operator deObject &();
}

deSkin *deSkinReference::operator->() const{
	return ( deSkin* )deObjectReference::operator->();
}

deSkinReference &deSkinReference::operator=( deSkin *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSkinReference &deSkinReference::operator=( const deSkinReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSkinReference::operator==( deSkin *object ) const{
	return deObjectReference::operator==( object );
}

bool deSkinReference::operator!=( deSkin *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSkinReference::operator==( const deSkinReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSkinReference::operator!=( const deSkinReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
