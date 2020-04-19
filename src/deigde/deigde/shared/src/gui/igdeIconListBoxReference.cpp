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

#include "igdeIconListBox.h"
#include "igdeIconListBoxReference.h"



// Class igdeIconListBoxReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeIconListBoxReference::igdeIconListBoxReference(){
}

igdeIconListBoxReference::igdeIconListBoxReference( igdeIconListBox *listbox ) :
deObjectReference( listbox ){
}

igdeIconListBoxReference::igdeIconListBoxReference( const igdeIconListBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeIconListBoxReference::TakeOver( igdeIconListBox *listbox ){
	deObjectReference::TakeOver( listbox );
}

bool igdeIconListBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeIconListBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeIconListBoxReference::operator igdeIconListBox *() const{
	return ( igdeIconListBox* )operator deObject *();
}

igdeIconListBoxReference::operator igdeIconListBox &() const{
	return ( igdeIconListBox& )operator deObject &();
}

igdeIconListBox *igdeIconListBoxReference::operator->() const{
	return ( igdeIconListBox* )deObjectReference::operator->();
}

igdeIconListBoxReference &igdeIconListBoxReference::operator=( igdeIconListBox *listbox ){
	deObjectReference::operator=( listbox );
	return *this;
}

igdeIconListBoxReference &igdeIconListBoxReference::operator=( const igdeIconListBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeIconListBoxReference::operator==( igdeIconListBox *listbox ) const{
	return deObjectReference::operator==( listbox );
}

bool igdeIconListBoxReference::operator!=( igdeIconListBox *listbox ) const{
	return deObjectReference::operator!=( listbox );
}

bool igdeIconListBoxReference::operator==( const igdeIconListBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeIconListBoxReference::operator!=( const igdeIconListBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
