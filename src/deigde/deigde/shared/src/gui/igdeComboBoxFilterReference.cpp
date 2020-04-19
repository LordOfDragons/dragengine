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

#include "igdeComboBoxFilter.h"
#include "igdeComboBoxFilterReference.h"



// Class igdeComboBoxFilterReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxFilterReference::igdeComboBoxFilterReference(){
}

igdeComboBoxFilterReference::igdeComboBoxFilterReference( igdeComboBoxFilter *combobox ) :
deObjectReference( combobox ){
}

igdeComboBoxFilterReference::igdeComboBoxFilterReference( const igdeComboBoxFilterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeComboBoxFilterReference::TakeOver( igdeComboBoxFilter *combobox ){
	deObjectReference::TakeOver( combobox );
}

bool igdeComboBoxFilterReference::operator!() const{
	return deObjectReference::operator!();
}

igdeComboBoxFilterReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeComboBoxFilterReference::operator igdeComboBoxFilter *() const{
	return ( igdeComboBoxFilter* )operator deObject *();
}

igdeComboBoxFilterReference::operator igdeComboBoxFilter &() const{
	return ( igdeComboBoxFilter& )operator deObject &();
}

igdeComboBoxFilter *igdeComboBoxFilterReference::operator->() const{
	return ( igdeComboBoxFilter* )deObjectReference::operator->();
}

igdeComboBoxFilterReference &igdeComboBoxFilterReference::operator=( igdeComboBoxFilter *combobox ){
	deObjectReference::operator=( combobox );
	return *this;
}

igdeComboBoxFilterReference &igdeComboBoxFilterReference::operator=( const igdeComboBoxFilterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeComboBoxFilterReference::operator==( igdeComboBoxFilter *combobox ) const{
	return deObjectReference::operator==( combobox );
}

bool igdeComboBoxFilterReference::operator!=( igdeComboBoxFilter *combobox ) const{
	return deObjectReference::operator!=( combobox );
}

bool igdeComboBoxFilterReference::operator==( const igdeComboBoxFilterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeComboBoxFilterReference::operator!=( const igdeComboBoxFilterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
