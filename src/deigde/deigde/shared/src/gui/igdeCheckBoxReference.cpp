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

#include "igdeCheckBox.h"
#include "igdeCheckBoxReference.h"



// Class igdeCheckBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeCheckBoxReference::igdeCheckBoxReference(){
}

igdeCheckBoxReference::igdeCheckBoxReference( igdeCheckBox *checkbox ) :
deObjectReference( checkbox ){
}

igdeCheckBoxReference::igdeCheckBoxReference( const igdeCheckBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeCheckBoxReference::TakeOver( igdeCheckBox *checkbox ){
	deObjectReference::TakeOver( checkbox );
}

bool igdeCheckBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeCheckBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeCheckBoxReference::operator igdeCheckBox *() const{
	return ( igdeCheckBox* )operator deObject *();
}

igdeCheckBoxReference::operator igdeCheckBox &() const{
	return ( igdeCheckBox& )operator deObject &();
}

igdeCheckBox *igdeCheckBoxReference::operator->() const{
	return ( igdeCheckBox* )deObjectReference::operator->();
}

igdeCheckBoxReference &igdeCheckBoxReference::operator=( igdeCheckBox *checkbox ){
	deObjectReference::operator=( checkbox );
	return *this;
}

igdeCheckBoxReference &igdeCheckBoxReference::operator=( const igdeCheckBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeCheckBoxReference::operator==( igdeCheckBox *checkbox ) const{
	return deObjectReference::operator==( checkbox );
}

bool igdeCheckBoxReference::operator!=( igdeCheckBox *checkbox ) const{
	return deObjectReference::operator!=( checkbox );
}

bool igdeCheckBoxReference::operator==( const igdeCheckBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeCheckBoxReference::operator!=( const igdeCheckBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
