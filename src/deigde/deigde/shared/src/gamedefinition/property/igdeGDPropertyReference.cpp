/* 
 * Drag[en]gine IGDE
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

#include "igdeGDProperty.h"
#include "igdeGDPropertyReference.h"



// Class igdeGDPropertyReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPropertyReference::igdeGDPropertyReference(){
}

igdeGDPropertyReference::igdeGDPropertyReference( igdeGDProperty *property ) :
deObjectReference( property ){
}

igdeGDPropertyReference::igdeGDPropertyReference( const igdeGDPropertyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDPropertyReference::TakeOver( igdeGDProperty *property ){
	deObjectReference::TakeOver( property );
}

bool igdeGDPropertyReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDPropertyReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDPropertyReference::operator igdeGDProperty *() const{
	return ( igdeGDProperty* )operator deObject *();
}

igdeGDPropertyReference::operator igdeGDProperty &() const{
	return ( igdeGDProperty& )operator deObject &();
}

igdeGDProperty *igdeGDPropertyReference::operator->() const{
	return ( igdeGDProperty* )deObjectReference::operator->();
}

igdeGDPropertyReference &igdeGDPropertyReference::operator=( igdeGDProperty *property ){
	deObjectReference::operator=( property );
	return *this;
}

igdeGDPropertyReference &igdeGDPropertyReference::operator=( const igdeGDPropertyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDPropertyReference::operator==( igdeGDProperty *property ) const{
	return deObjectReference::operator==( property );
}

bool igdeGDPropertyReference::operator!=( igdeGDProperty *property ) const{
	return deObjectReference::operator!=( property );
}

bool igdeGDPropertyReference::operator==( const igdeGDPropertyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDPropertyReference::operator!=( const igdeGDPropertyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
