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

#include "igdeToolBar.h"
#include "igdeToolBarReference.h"



// Class igdeToolBarReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToolBarReference::igdeToolBarReference(){
}

igdeToolBarReference::igdeToolBarReference( igdeToolBar *toolbar ) :
deObjectReference( toolbar ){
}

igdeToolBarReference::igdeToolBarReference( const igdeToolBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToolBarReference::TakeOver( igdeToolBar *toolbar ){
	deObjectReference::TakeOver( toolbar );
}

bool igdeToolBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToolBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToolBarReference::operator igdeToolBar *() const{
	return ( igdeToolBar* )operator deObject *();
}

igdeToolBarReference::operator igdeToolBar &() const{
	return ( igdeToolBar& )operator deObject &();
}

igdeToolBar *igdeToolBarReference::operator->() const{
	return ( igdeToolBar* )deObjectReference::operator->();
}

igdeToolBarReference &igdeToolBarReference::operator=( igdeToolBar *toolbar ){
	deObjectReference::operator=( toolbar );
	return *this;
}

igdeToolBarReference &igdeToolBarReference::operator=( const igdeToolBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToolBarReference::operator==( igdeToolBar *toolbar ) const{
	return deObjectReference::operator==( toolbar );
}

bool igdeToolBarReference::operator!=( igdeToolBar *toolbar ) const{
	return deObjectReference::operator!=( toolbar );
}

bool igdeToolBarReference::operator==( const igdeToolBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToolBarReference::operator!=( const igdeToolBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
