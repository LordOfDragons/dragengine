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

#include "igdeComboBox.h"
#include "igdeComboBoxReference.h"



// Class igdeComboBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxReference::igdeComboBoxReference(){
}

igdeComboBoxReference::igdeComboBoxReference( igdeComboBox *combobox ) :
deObjectReference( combobox ){
}

igdeComboBoxReference::igdeComboBoxReference( const igdeComboBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeComboBoxReference::TakeOver( igdeComboBox *combobox ){
	deObjectReference::TakeOver( combobox );
}

bool igdeComboBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeComboBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeComboBoxReference::operator igdeComboBox *() const{
	return ( igdeComboBox* )operator deObject *();
}

igdeComboBoxReference::operator igdeComboBox &() const{
	return ( igdeComboBox& )operator deObject &();
}

igdeComboBox *igdeComboBoxReference::operator->() const{
	return ( igdeComboBox* )deObjectReference::operator->();
}

igdeComboBoxReference &igdeComboBoxReference::operator=( igdeComboBox *combobox ){
	deObjectReference::operator=( combobox );
	return *this;
}

igdeComboBoxReference &igdeComboBoxReference::operator=( const igdeComboBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeComboBoxReference::operator==( igdeComboBox *combobox ) const{
	return deObjectReference::operator==( combobox );
}

bool igdeComboBoxReference::operator!=( igdeComboBox *combobox ) const{
	return deObjectReference::operator!=( combobox );
}

bool igdeComboBoxReference::operator==( const igdeComboBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeComboBoxReference::operator!=( const igdeComboBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
