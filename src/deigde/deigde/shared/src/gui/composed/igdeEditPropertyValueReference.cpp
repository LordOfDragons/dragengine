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

#include "igdeEditPropertyValue.h"
#include "igdeEditPropertyValueReference.h"



// Class igdeEditPropertyValueReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPropertyValueReference::igdeEditPropertyValueReference(){
}

igdeEditPropertyValueReference::igdeEditPropertyValueReference( igdeEditPropertyValue *widget ) :
deObjectReference( widget ){
}

igdeEditPropertyValueReference::igdeEditPropertyValueReference( const igdeEditPropertyValueReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPropertyValueReference::TakeOver( igdeEditPropertyValue *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeEditPropertyValueReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPropertyValueReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPropertyValueReference::operator igdeEditPropertyValue *() const{
	return ( igdeEditPropertyValue* )operator deObject *();
}

igdeEditPropertyValueReference::operator igdeEditPropertyValue &() const{
	return ( igdeEditPropertyValue& )operator deObject &();
}

igdeEditPropertyValue *igdeEditPropertyValueReference::operator->() const{
	return ( igdeEditPropertyValue* )deObjectReference::operator->();
}

igdeEditPropertyValueReference &igdeEditPropertyValueReference::operator=( igdeEditPropertyValue *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeEditPropertyValueReference &igdeEditPropertyValueReference::operator=( const igdeEditPropertyValueReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPropertyValueReference::operator==( igdeEditPropertyValue *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeEditPropertyValueReference::operator!=( igdeEditPropertyValue *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeEditPropertyValueReference::operator==( const igdeEditPropertyValueReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPropertyValueReference::operator!=( const igdeEditPropertyValueReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
