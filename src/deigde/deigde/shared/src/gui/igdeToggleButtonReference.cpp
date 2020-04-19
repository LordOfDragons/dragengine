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

#include "igdeToggleButton.h"
#include "igdeToggleButtonReference.h"



// Class igdeToggleButtonReference
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToggleButtonReference::igdeToggleButtonReference(){
}

igdeToggleButtonReference::igdeToggleButtonReference( igdeToggleButton *button ) :
deObjectReference( button ){
}

igdeToggleButtonReference::igdeToggleButtonReference( const igdeToggleButtonReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToggleButtonReference::TakeOver( igdeToggleButton *button ){
	deObjectReference::TakeOver( button );
}

bool igdeToggleButtonReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToggleButtonReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToggleButtonReference::operator igdeToggleButton *() const{
	return ( igdeToggleButton* )operator deObject *();
}

igdeToggleButtonReference::operator igdeToggleButton &() const{
	return ( igdeToggleButton& )operator deObject &();
}

igdeToggleButton *igdeToggleButtonReference::operator->() const{
	return ( igdeToggleButton* )deObjectReference::operator->();
}

igdeToggleButtonReference &igdeToggleButtonReference::operator=( igdeToggleButton *button ){
	deObjectReference::operator=( button );
	return *this;
}

igdeToggleButtonReference &igdeToggleButtonReference::operator=( const igdeToggleButtonReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToggleButtonReference::operator==( igdeToggleButton *button ) const{
	return deObjectReference::operator==( button );
}

bool igdeToggleButtonReference::operator!=( igdeToggleButton *button ) const{
	return deObjectReference::operator!=( button );
}

bool igdeToggleButtonReference::operator==( const igdeToggleButtonReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToggleButtonReference::operator!=( const igdeToggleButtonReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
