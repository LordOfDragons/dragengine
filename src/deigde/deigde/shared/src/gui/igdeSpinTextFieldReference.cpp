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

#include "igdeSpinTextField.h"
#include "igdeSpinTextFieldReference.h"



// Class igdeSpinTextFieldReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpinTextFieldReference::igdeSpinTextFieldReference(){
}

igdeSpinTextFieldReference::igdeSpinTextFieldReference( igdeSpinTextField *textfield ) :
deObjectReference( textfield ){
}

igdeSpinTextFieldReference::igdeSpinTextFieldReference( const igdeSpinTextFieldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSpinTextFieldReference::TakeOver( igdeSpinTextField *textfield ){
	deObjectReference::TakeOver( textfield );
}

bool igdeSpinTextFieldReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSpinTextFieldReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSpinTextFieldReference::operator igdeSpinTextField *() const{
	return ( igdeSpinTextField* )operator deObject *();
}

igdeSpinTextFieldReference::operator igdeSpinTextField &() const{
	return ( igdeSpinTextField& )operator deObject &();
}

igdeSpinTextField *igdeSpinTextFieldReference::operator->() const{
	return ( igdeSpinTextField* )deObjectReference::operator->();
}

igdeSpinTextFieldReference &igdeSpinTextFieldReference::operator=( igdeSpinTextField *textfield ){
	deObjectReference::operator=( textfield );
	return *this;
}

igdeSpinTextFieldReference &igdeSpinTextFieldReference::operator=( const igdeSpinTextFieldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSpinTextFieldReference::operator==( igdeSpinTextField *textfield ) const{
	return deObjectReference::operator==( textfield );
}

bool igdeSpinTextFieldReference::operator!=( igdeSpinTextField *textfield ) const{
	return deObjectReference::operator!=( textfield );
}

bool igdeSpinTextFieldReference::operator==( const igdeSpinTextFieldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSpinTextFieldReference::operator!=( const igdeSpinTextFieldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
