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

#include "igdeScrollBar.h"
#include "igdeScrollBarReference.h"



// Class igdeScrollBarReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeScrollBarReference::igdeScrollBarReference(){
}

igdeScrollBarReference::igdeScrollBarReference( igdeScrollBar *scrollbar ) :
deObjectReference( scrollbar ){
}

igdeScrollBarReference::igdeScrollBarReference( const igdeScrollBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeScrollBarReference::TakeOver( igdeScrollBar *scrollbar ){
	deObjectReference::TakeOver( scrollbar );
}

bool igdeScrollBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeScrollBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeScrollBarReference::operator igdeScrollBar *() const{
	return ( igdeScrollBar* )operator deObject *();
}

igdeScrollBarReference::operator igdeScrollBar &() const{
	return ( igdeScrollBar& )operator deObject &();
}

igdeScrollBar *igdeScrollBarReference::operator->() const{
	return ( igdeScrollBar* )deObjectReference::operator->();
}

igdeScrollBarReference &igdeScrollBarReference::operator=( igdeScrollBar *scrollbar ){
	deObjectReference::operator=( scrollbar );
	return *this;
}

igdeScrollBarReference &igdeScrollBarReference::operator=( const igdeScrollBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeScrollBarReference::operator==( igdeScrollBar *scrollbar ) const{
	return deObjectReference::operator==( scrollbar );
}

bool igdeScrollBarReference::operator!=( igdeScrollBar *scrollbar ) const{
	return deObjectReference::operator!=( scrollbar );
}

bool igdeScrollBarReference::operator==( const igdeScrollBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeScrollBarReference::operator!=( const igdeScrollBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
