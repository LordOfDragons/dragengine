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

#include "igdeColorBox.h"
#include "igdeColorBoxReference.h"



// Class igdeColorBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeColorBoxReference::igdeColorBoxReference(){
}

igdeColorBoxReference::igdeColorBoxReference( igdeColorBox *colorbox ) :
deObjectReference( colorbox ){
}

igdeColorBoxReference::igdeColorBoxReference( const igdeColorBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeColorBoxReference::TakeOver( igdeColorBox *colorbox ){
	deObjectReference::TakeOver( colorbox );
}

bool igdeColorBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeColorBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeColorBoxReference::operator igdeColorBox *() const{
	return ( igdeColorBox* )operator deObject *();
}

igdeColorBoxReference::operator igdeColorBox &() const{
	return ( igdeColorBox& )operator deObject &();
}

igdeColorBox *igdeColorBoxReference::operator->() const{
	return ( igdeColorBox* )deObjectReference::operator->();
}

igdeColorBoxReference &igdeColorBoxReference::operator=( igdeColorBox *colorbox ){
	deObjectReference::operator=( colorbox );
	return *this;
}

igdeColorBoxReference &igdeColorBoxReference::operator=( const igdeColorBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeColorBoxReference::operator==( igdeColorBox *colorbox ) const{
	return deObjectReference::operator==( colorbox );
}

bool igdeColorBoxReference::operator!=( igdeColorBox *colorbox ) const{
	return deObjectReference::operator!=( colorbox );
}

bool igdeColorBoxReference::operator==( const igdeColorBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeColorBoxReference::operator!=( const igdeColorBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
