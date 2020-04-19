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

#include "deBillboard.h"
#include "deBillboardReference.h"



// Class deBillboardReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deBillboardReference::deBillboardReference(){
}

deBillboardReference::deBillboardReference( deBillboard *object ) :
deObjectReference( object ){
}

deBillboardReference::deBillboardReference( const deBillboardReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deBillboardReference::TakeOver( deBillboard *object ){
	deObjectReference::TakeOver( object );
}

bool deBillboardReference::operator!() const{
	return deObjectReference::operator!();
}

deBillboardReference::operator bool() const{
	return deObjectReference::operator bool();
}

deBillboardReference::operator deBillboard *() const{
	return ( deBillboard* )operator deObject *();
}

deBillboardReference::operator deBillboard &() const{
	return ( deBillboard& )operator deObject &();
}

deBillboard *deBillboardReference::operator->() const{
	return ( deBillboard* )deObjectReference::operator->();
}

deBillboardReference &deBillboardReference::operator=( deBillboard *object ){
	deObjectReference::operator=( object );
	return *this;
}

deBillboardReference &deBillboardReference::operator=( const deBillboardReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deBillboardReference::operator==( deBillboard *object ) const{
	return deObjectReference::operator==( object );
}

bool deBillboardReference::operator!=( deBillboard *object ) const{
	return deObjectReference::operator!=( object );
}

bool deBillboardReference::operator==( const deBillboardReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deBillboardReference::operator!=( const deBillboardReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
