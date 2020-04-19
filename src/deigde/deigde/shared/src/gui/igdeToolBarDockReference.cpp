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

#include "igdeToolBarDock.h"
#include "igdeToolBarDockReference.h"



// Class igdeToolBarDockReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToolBarDockReference::igdeToolBarDockReference(){
}

igdeToolBarDockReference::igdeToolBarDockReference( igdeToolBarDock *dock ) :
deObjectReference( dock ){
}

igdeToolBarDockReference::igdeToolBarDockReference( const igdeToolBarDockReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToolBarDockReference::TakeOver( igdeToolBarDock *dock ){
	deObjectReference::TakeOver( dock );
}

bool igdeToolBarDockReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToolBarDockReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToolBarDockReference::operator igdeToolBarDock *() const{
	return ( igdeToolBarDock* )operator deObject *();
}

igdeToolBarDockReference::operator igdeToolBarDock &() const{
	return ( igdeToolBarDock& )operator deObject &();
}

igdeToolBarDock *igdeToolBarDockReference::operator->() const{
	return ( igdeToolBarDock* )deObjectReference::operator->();
}

igdeToolBarDockReference &igdeToolBarDockReference::operator=( igdeToolBarDock *dock ){
	deObjectReference::operator=( dock );
	return *this;
}

igdeToolBarDockReference &igdeToolBarDockReference::operator=( const igdeToolBarDockReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToolBarDockReference::operator==( igdeToolBarDock *dock ) const{
	return deObjectReference::operator==( dock );
}

bool igdeToolBarDockReference::operator!=( igdeToolBarDock *dock ) const{
	return deObjectReference::operator!=( dock );
}

bool igdeToolBarDockReference::operator==( const igdeToolBarDockReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToolBarDockReference::operator!=( const igdeToolBarDockReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
