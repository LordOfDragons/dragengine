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

#include "igdeViewRenderWindow.h"
#include "igdeViewRenderWindowReference.h"



// Class igdeViewRenderWindowReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewRenderWindowReference::igdeViewRenderWindowReference(){
}

igdeViewRenderWindowReference::igdeViewRenderWindowReference( igdeViewRenderWindow *view ) :
deObjectReference( view ){
}

igdeViewRenderWindowReference::igdeViewRenderWindowReference( const igdeViewRenderWindowReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeViewRenderWindowReference::TakeOver( igdeViewRenderWindow *view ){
	deObjectReference::TakeOver( view );
}

bool igdeViewRenderWindowReference::operator!() const{
	return deObjectReference::operator!();
}

igdeViewRenderWindowReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeViewRenderWindowReference::operator igdeViewRenderWindow *() const{
	return ( igdeViewRenderWindow* )operator deObject *();
}

igdeViewRenderWindowReference::operator igdeViewRenderWindow &() const{
	return ( igdeViewRenderWindow& )operator deObject &();
}

igdeViewRenderWindow *igdeViewRenderWindowReference::operator->() const{
	return ( igdeViewRenderWindow* )deObjectReference::operator->();
}

igdeViewRenderWindowReference &igdeViewRenderWindowReference::operator=( igdeViewRenderWindow *view ){
	deObjectReference::operator=( view );
	return *this;
}

igdeViewRenderWindowReference &igdeViewRenderWindowReference::operator=( const igdeViewRenderWindowReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeViewRenderWindowReference::operator==( igdeViewRenderWindow *view ) const{
	return deObjectReference::operator==( view );
}

bool igdeViewRenderWindowReference::operator!=( igdeViewRenderWindow *view ) const{
	return deObjectReference::operator!=( view );
}

bool igdeViewRenderWindowReference::operator==( const igdeViewRenderWindowReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeViewRenderWindowReference::operator!=( const igdeViewRenderWindowReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
