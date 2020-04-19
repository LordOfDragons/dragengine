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

#include "deLumimeter.h"
#include "deLumimeterReference.h"



// Class deLumimeterReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deLumimeterReference::deLumimeterReference(){
}

deLumimeterReference::deLumimeterReference( deLumimeter *object ) :
deObjectReference( object ){
}

deLumimeterReference::deLumimeterReference( const deLumimeterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deLumimeterReference::TakeOver( deLumimeter *object ){
	deObjectReference::TakeOver( object );
}

bool deLumimeterReference::operator!() const{
	return deObjectReference::operator!();
}

deLumimeterReference::operator bool() const{
	return deObjectReference::operator bool();
}

deLumimeterReference::operator deLumimeter *() const{
	return ( deLumimeter* )operator deObject *();
}

deLumimeterReference::operator deLumimeter &() const{
	return ( deLumimeter& )operator deObject &();
}

deLumimeter *deLumimeterReference::operator->() const{
	return ( deLumimeter* )deObjectReference::operator->();
}

deLumimeterReference &deLumimeterReference::operator=( deLumimeter *object ){
	deObjectReference::operator=( object );
	return *this;
}

deLumimeterReference &deLumimeterReference::operator=( const deLumimeterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deLumimeterReference::operator==( deLumimeter *object ) const{
	return deObjectReference::operator==( object );
}

bool deLumimeterReference::operator!=( deLumimeter *object ) const{
	return deObjectReference::operator!=( object );
}

bool deLumimeterReference::operator==( const deLumimeterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deLumimeterReference::operator!=( const deLumimeterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
