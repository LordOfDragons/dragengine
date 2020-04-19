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

#include "igdeFont.h"
#include "igdeFontReference.h"



// Class igdeFontReference
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeFontReference::igdeFontReference(){
}

igdeFontReference::igdeFontReference( igdeFont *font ) :
deObjectReference( font ){
}

igdeFontReference::igdeFontReference( const igdeFontReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeFontReference::TakeOver( igdeFont *font ){
	deObjectReference::TakeOver( font );
}

bool igdeFontReference::operator!() const{
	return deObjectReference::operator!();
}

igdeFontReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeFontReference::operator igdeFont *() const{
	return ( igdeFont* )operator deObject *();
}

igdeFontReference::operator igdeFont &() const{
	return ( igdeFont& )operator deObject &();
}

igdeFont *igdeFontReference::operator->() const{
	return ( igdeFont* )deObjectReference::operator->();
}

igdeFontReference &igdeFontReference::operator=( igdeFont *font ){
	deObjectReference::operator=( font );
	return *this;
}

igdeFontReference &igdeFontReference::operator=( const igdeFontReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeFontReference::operator==( igdeFont *font ) const{
	return deObjectReference::operator==( font );
}

bool igdeFontReference::operator!=( igdeFont *font ) const{
	return deObjectReference::operator!=( font );
}

bool igdeFontReference::operator==( const igdeFontReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeFontReference::operator!=( const igdeFontReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
