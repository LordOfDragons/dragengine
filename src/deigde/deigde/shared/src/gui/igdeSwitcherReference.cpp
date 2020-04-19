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

#include "igdeSwitcher.h"
#include "igdeSwitcherReference.h"



// Class igdeSwitcherReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSwitcherReference::igdeSwitcherReference(){
}

igdeSwitcherReference::igdeSwitcherReference( igdeSwitcher *switcher ) :
deObjectReference( switcher ){
}

igdeSwitcherReference::igdeSwitcherReference( const igdeSwitcherReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSwitcherReference::TakeOver( igdeSwitcher *switcher ){
	deObjectReference::TakeOver( switcher );
}

bool igdeSwitcherReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSwitcherReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSwitcherReference::operator igdeSwitcher *() const{
	return ( igdeSwitcher* )operator deObject *();
}

igdeSwitcherReference::operator igdeSwitcher &() const{
	return ( igdeSwitcher& )operator deObject &();
}

igdeSwitcher *igdeSwitcherReference::operator->() const{
	return ( igdeSwitcher* )deObjectReference::operator->();
}

igdeSwitcherReference &igdeSwitcherReference::operator=( igdeSwitcher *switcher ){
	deObjectReference::operator=( switcher );
	return *this;
}

igdeSwitcherReference &igdeSwitcherReference::operator=( const igdeSwitcherReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSwitcherReference::operator==( igdeSwitcher *switcher ) const{
	return deObjectReference::operator==( switcher );
}

bool igdeSwitcherReference::operator!=( igdeSwitcher *switcher ) const{
	return deObjectReference::operator!=( switcher );
}

bool igdeSwitcherReference::operator==( const igdeSwitcherReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSwitcherReference::operator!=( const igdeSwitcherReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
