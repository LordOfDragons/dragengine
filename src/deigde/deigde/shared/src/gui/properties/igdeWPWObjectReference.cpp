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

#include "igdeWPWObject.h"
#include "igdeWPWObjectReference.h"



// Class igdeWPWObjectReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPWObjectReference::igdeWPWObjectReference(){
}

igdeWPWObjectReference::igdeWPWObjectReference( igdeWPWObject *widget ) :
deObjectReference( widget ){
}

igdeWPWObjectReference::igdeWPWObjectReference( const igdeWPWObjectReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPWObjectReference::TakeOver( igdeWPWObject *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPWObjectReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPWObjectReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPWObjectReference::operator igdeWPWObject *() const{
	return ( igdeWPWObject* )operator deObject *();
}

igdeWPWObjectReference::operator igdeWPWObject &() const{
	return ( igdeWPWObject& )operator deObject &();
}

igdeWPWObject *igdeWPWObjectReference::operator->() const{
	return ( igdeWPWObject* )deObjectReference::operator->();
}

igdeWPWObjectReference &igdeWPWObjectReference::operator=( igdeWPWObject *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPWObjectReference &igdeWPWObjectReference::operator=( const igdeWPWObjectReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPWObjectReference::operator==( igdeWPWObject *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPWObjectReference::operator!=( igdeWPWObject *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPWObjectReference::operator==( const igdeWPWObjectReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPWObjectReference::operator!=( const igdeWPWObjectReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
