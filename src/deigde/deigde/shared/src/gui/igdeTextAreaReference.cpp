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

#include "igdeTextArea.h"
#include "igdeTextAreaReference.h"



// Class igdeTextAreaReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextAreaReference::igdeTextAreaReference(){
}

igdeTextAreaReference::igdeTextAreaReference( igdeTextArea *textarea ) :
deObjectReference( textarea ){
}

igdeTextAreaReference::igdeTextAreaReference( const igdeTextAreaReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextAreaReference::TakeOver( igdeTextArea *textarea ){
	deObjectReference::TakeOver( textarea );
}

bool igdeTextAreaReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextAreaReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextAreaReference::operator igdeTextArea *() const{
	return ( igdeTextArea* )operator deObject *();
}

igdeTextAreaReference::operator igdeTextArea &() const{
	return ( igdeTextArea& )operator deObject &();
}

igdeTextArea *igdeTextAreaReference::operator->() const{
	return ( igdeTextArea* )deObjectReference::operator->();
}

igdeTextAreaReference &igdeTextAreaReference::operator=( igdeTextArea *textarea ){
	deObjectReference::operator=( textarea );
	return *this;
}

igdeTextAreaReference &igdeTextAreaReference::operator=( const igdeTextAreaReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextAreaReference::operator==( igdeTextArea *textarea ) const{
	return deObjectReference::operator==( textarea );
}

bool igdeTextAreaReference::operator!=( igdeTextArea *textarea ) const{
	return deObjectReference::operator!=( textarea );
}

bool igdeTextAreaReference::operator==( const igdeTextAreaReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextAreaReference::operator!=( const igdeTextAreaReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
