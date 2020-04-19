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

#include "deSynthesizerInstance.h"
#include "deSynthesizerInstanceReference.h"



// Class deSynthesizerInstanceReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerInstanceReference::deSynthesizerInstanceReference(){
}

deSynthesizerInstanceReference::deSynthesizerInstanceReference( deSynthesizerInstance *object ) :
deObjectReference( object ){
}

deSynthesizerInstanceReference::deSynthesizerInstanceReference( const deSynthesizerInstanceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSynthesizerInstanceReference::TakeOver( deSynthesizerInstance *object ){
	deObjectReference::TakeOver( object );
}

bool deSynthesizerInstanceReference::operator!() const{
	return deObjectReference::operator!();
}

deSynthesizerInstanceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSynthesizerInstanceReference::operator deSynthesizerInstance *() const{
	return ( deSynthesizerInstance* )operator deObject *();
}

deSynthesizerInstanceReference::operator deSynthesizerInstance &() const{
	return ( deSynthesizerInstance& )operator deObject &();
}

deSynthesizerInstance *deSynthesizerInstanceReference::operator->() const{
	return ( deSynthesizerInstance* )deObjectReference::operator->();
}

deSynthesizerInstanceReference &deSynthesizerInstanceReference::operator=( deSynthesizerInstance *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSynthesizerInstanceReference &deSynthesizerInstanceReference::operator=( const deSynthesizerInstanceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSynthesizerInstanceReference::operator==( deSynthesizerInstance *object ) const{
	return deObjectReference::operator==( object );
}

bool deSynthesizerInstanceReference::operator!=( deSynthesizerInstance *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSynthesizerInstanceReference::operator==( const deSynthesizerInstanceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSynthesizerInstanceReference::operator!=( const deSynthesizerInstanceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
