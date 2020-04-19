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

#include "igdeListBox.h"
#include "igdeListBoxReference.h"



// Class igdeListBoxReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListBoxReference::igdeListBoxReference(){
}

igdeListBoxReference::igdeListBoxReference( igdeListBox *listbox ) :
deObjectReference( listbox ){
}

igdeListBoxReference::igdeListBoxReference( const igdeListBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListBoxReference::TakeOver( igdeListBox *listbox ){
	deObjectReference::TakeOver( listbox );
}

bool igdeListBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListBoxReference::operator igdeListBox *() const{
	return ( igdeListBox* )operator deObject *();
}

igdeListBoxReference::operator igdeListBox &() const{
	return ( igdeListBox& )operator deObject &();
}

igdeListBox *igdeListBoxReference::operator->() const{
	return ( igdeListBox* )deObjectReference::operator->();
}

igdeListBoxReference &igdeListBoxReference::operator=( igdeListBox *listbox ){
	deObjectReference::operator=( listbox );
	return *this;
}

igdeListBoxReference &igdeListBoxReference::operator=( const igdeListBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListBoxReference::operator==( igdeListBox *listbox ) const{
	return deObjectReference::operator==( listbox );
}

bool igdeListBoxReference::operator!=( igdeListBox *listbox ) const{
	return deObjectReference::operator!=( listbox );
}

bool igdeListBoxReference::operator==( const igdeListBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListBoxReference::operator!=( const igdeListBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
