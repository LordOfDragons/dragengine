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

#include "igdeGroupBox.h"
#include "igdeGroupBoxReference.h"



// Class igdeGroupBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGroupBoxReference::igdeGroupBoxReference(){
}

igdeGroupBoxReference::igdeGroupBoxReference( igdeGroupBox *groupbox ) :
deObjectReference( groupbox ){
}

igdeGroupBoxReference::igdeGroupBoxReference( const igdeGroupBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGroupBoxReference::TakeOver( igdeGroupBox *groupbox ){
	deObjectReference::TakeOver( groupbox );
}

bool igdeGroupBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGroupBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGroupBoxReference::operator igdeGroupBox *() const{
	return ( igdeGroupBox* )operator deObject *();
}

igdeGroupBoxReference::operator igdeGroupBox &() const{
	return ( igdeGroupBox& )operator deObject &();
}

igdeGroupBox *igdeGroupBoxReference::operator->() const{
	return ( igdeGroupBox* )deObjectReference::operator->();
}

igdeGroupBoxReference &igdeGroupBoxReference::operator=( igdeGroupBox *groupbox ){
	deObjectReference::operator=( groupbox );
	return *this;
}

igdeGroupBoxReference &igdeGroupBoxReference::operator=( const igdeGroupBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGroupBoxReference::operator==( igdeGroupBox *groupbox ) const{
	return deObjectReference::operator==( groupbox );
}

bool igdeGroupBoxReference::operator!=( igdeGroupBox *groupbox ) const{
	return deObjectReference::operator!=( groupbox );
}

bool igdeGroupBoxReference::operator==( const igdeGroupBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGroupBoxReference::operator!=( const igdeGroupBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
