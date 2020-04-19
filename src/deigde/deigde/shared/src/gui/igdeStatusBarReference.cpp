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

#include "igdeStatusBar.h"
#include "igdeStatusBarReference.h"



// Class igdeStatusBarReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeStatusBarReference::igdeStatusBarReference(){
}

igdeStatusBarReference::igdeStatusBarReference( igdeStatusBar *statusBar ) :
deObjectReference( statusBar ){
}

igdeStatusBarReference::igdeStatusBarReference( const igdeStatusBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeStatusBarReference::TakeOver( igdeStatusBar *statusBar ){
	deObjectReference::TakeOver( statusBar );
}

bool igdeStatusBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeStatusBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeStatusBarReference::operator igdeStatusBar *() const{
	return ( igdeStatusBar* )operator deObject *();
}

igdeStatusBarReference::operator igdeStatusBar &() const{
	return ( igdeStatusBar& )operator deObject &();
}

igdeStatusBar *igdeStatusBarReference::operator->() const{
	return ( igdeStatusBar* )deObjectReference::operator->();
}

igdeStatusBarReference &igdeStatusBarReference::operator=( igdeStatusBar *statusBar ){
	deObjectReference::operator=( statusBar );
	return *this;
}

igdeStatusBarReference &igdeStatusBarReference::operator=( const igdeStatusBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeStatusBarReference::operator==( igdeStatusBar *statusBar ) const{
	return deObjectReference::operator==( statusBar );
}

bool igdeStatusBarReference::operator!=( igdeStatusBar *statusBar ) const{
	return deObjectReference::operator!=( statusBar );
}

bool igdeStatusBarReference::operator==( const igdeStatusBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeStatusBarReference::operator!=( const igdeStatusBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
