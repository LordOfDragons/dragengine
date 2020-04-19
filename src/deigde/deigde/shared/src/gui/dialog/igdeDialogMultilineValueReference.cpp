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

#include "igdeDialogMultilineValue.h"
#include "igdeDialogMultilineValueReference.h"



// Class igdeDialogMultilineValueReference
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogMultilineValueReference::igdeDialogMultilineValueReference(){
}

igdeDialogMultilineValueReference::igdeDialogMultilineValueReference( igdeDialogMultilineValue *dialog ) :
deObjectReference( dialog ){
}

igdeDialogMultilineValueReference::igdeDialogMultilineValueReference( const igdeDialogMultilineValueReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeDialogMultilineValueReference::TakeOver( igdeDialogMultilineValue *dialog ){
	deObjectReference::TakeOver( dialog );
}

bool igdeDialogMultilineValueReference::operator!() const{
	return deObjectReference::operator!();
}

igdeDialogMultilineValueReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeDialogMultilineValueReference::operator igdeDialogMultilineValue *() const{
	return ( igdeDialogMultilineValue* )operator deObject *();
}

igdeDialogMultilineValueReference::operator igdeDialogMultilineValue &() const{
	return ( igdeDialogMultilineValue& )operator deObject &();
}

igdeDialogMultilineValue *igdeDialogMultilineValueReference::operator->() const{
	return ( igdeDialogMultilineValue* )deObjectReference::operator->();
}

igdeDialogMultilineValueReference &igdeDialogMultilineValueReference::operator=( igdeDialogMultilineValue *dialog ){
	deObjectReference::operator=( dialog );
	return *this;
}

igdeDialogMultilineValueReference &igdeDialogMultilineValueReference::operator=( const igdeDialogMultilineValueReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeDialogMultilineValueReference::operator==( igdeDialogMultilineValue *dialog ) const{
	return deObjectReference::operator==( dialog );
}

bool igdeDialogMultilineValueReference::operator!=( igdeDialogMultilineValue *dialog ) const{
	return deObjectReference::operator!=( dialog );
}

bool igdeDialogMultilineValueReference::operator==( const igdeDialogMultilineValueReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeDialogMultilineValueReference::operator!=( const igdeDialogMultilineValueReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
