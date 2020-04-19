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

#include "igdeMenuBar.h"
#include "igdeMenuBarReference.h"



// Class igdeMenuBarReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuBarReference::igdeMenuBarReference(){
}

igdeMenuBarReference::igdeMenuBarReference( igdeMenuBar *menubar ) :
deObjectReference( menubar ){
}

igdeMenuBarReference::igdeMenuBarReference( const igdeMenuBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMenuBarReference::TakeOver( igdeMenuBar *menubar ){
	deObjectReference::TakeOver( menubar );
}

bool igdeMenuBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMenuBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMenuBarReference::operator igdeMenuBar *() const{
	return ( igdeMenuBar* )operator deObject *();
}

igdeMenuBarReference::operator igdeMenuBar &() const{
	return ( igdeMenuBar& )operator deObject &();
}

igdeMenuBar *igdeMenuBarReference::operator->() const{
	return ( igdeMenuBar* )deObjectReference::operator->();
}

igdeMenuBarReference &igdeMenuBarReference::operator=( igdeMenuBar *menubar ){
	deObjectReference::operator=( menubar );
	return *this;
}

igdeMenuBarReference &igdeMenuBarReference::operator=( const igdeMenuBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMenuBarReference::operator==( igdeMenuBar *menubar ) const{
	return deObjectReference::operator==( menubar );
}

bool igdeMenuBarReference::operator!=( igdeMenuBar *menubar ) const{
	return deObjectReference::operator!=( menubar );
}

bool igdeMenuBarReference::operator==( const igdeMenuBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMenuBarReference::operator!=( const igdeMenuBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
