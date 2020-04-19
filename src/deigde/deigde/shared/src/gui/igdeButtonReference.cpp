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

#include "igdeButton.h"
#include "igdeButtonReference.h"



// Class igdeButtonReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeButtonReference::igdeButtonReference(){
}

igdeButtonReference::igdeButtonReference( igdeButton *button ) :
deObjectReference( button ){
}

igdeButtonReference::igdeButtonReference( const igdeButtonReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeButtonReference::TakeOver( igdeButton *button ){
	deObjectReference::TakeOver( button );
}

bool igdeButtonReference::operator!() const{
	return deObjectReference::operator!();
}

igdeButtonReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeButtonReference::operator igdeButton *() const{
	return ( igdeButton* )operator deObject *();
}

igdeButtonReference::operator igdeButton &() const{
	return ( igdeButton& )operator deObject &();
}

igdeButton *igdeButtonReference::operator->() const{
	return ( igdeButton* )deObjectReference::operator->();
}

igdeButtonReference &igdeButtonReference::operator=( igdeButton *button ){
	deObjectReference::operator=( button );
	return *this;
}

igdeButtonReference &igdeButtonReference::operator=( const igdeButtonReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeButtonReference::operator==( igdeButton *button ) const{
	return deObjectReference::operator==( button );
}

bool igdeButtonReference::operator!=( igdeButton *button ) const{
	return deObjectReference::operator!=( button );
}

bool igdeButtonReference::operator==( const igdeButtonReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeButtonReference::operator!=( const igdeButtonReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
