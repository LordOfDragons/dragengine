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

#include "igdeDialog.h"
#include "igdeDialogReference.h"



// Class igdeDialogReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogReference::igdeDialogReference(){
}

igdeDialogReference::igdeDialogReference( igdeDialog *dialog ) :
deObjectReference( dialog ){
}

igdeDialogReference::igdeDialogReference( const igdeDialogReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeDialogReference::TakeOver( igdeDialog *dialog ){
	deObjectReference::TakeOver( dialog );
}

bool igdeDialogReference::operator!() const{
	return deObjectReference::operator!();
}

igdeDialogReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeDialogReference::operator igdeDialog *() const{
	return ( igdeDialog* )operator deObject *();
}

igdeDialogReference::operator igdeDialog &() const{
	return ( igdeDialog& )operator deObject &();
}

igdeDialog *igdeDialogReference::operator->() const{
	return ( igdeDialog* )deObjectReference::operator->();
}

igdeDialogReference &igdeDialogReference::operator=( igdeDialog *dialog ){
	deObjectReference::operator=( dialog );
	return *this;
}

igdeDialogReference &igdeDialogReference::operator=( const igdeDialogReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeDialogReference::operator==( igdeDialog *dialog ) const{
	return deObjectReference::operator==( dialog );
}

bool igdeDialogReference::operator!=( igdeDialog *dialog ) const{
	return deObjectReference::operator!=( dialog );
}

bool igdeDialogReference::operator==( const igdeDialogReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeDialogReference::operator!=( const igdeDialogReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
