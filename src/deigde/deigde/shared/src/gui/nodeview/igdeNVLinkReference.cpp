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

#include "igdeNVLink.h"
#include "igdeNVLinkReference.h"



// Class igdeNVLinkReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVLinkReference::igdeNVLinkReference(){
}

igdeNVLinkReference::igdeNVLinkReference( igdeNVLink *link ) :
deObjectReference( link ){
}

igdeNVLinkReference::igdeNVLinkReference( const igdeNVLinkReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVLinkReference::TakeOver( igdeNVLink *link ){
	deObjectReference::TakeOver( link );
}

bool igdeNVLinkReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVLinkReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVLinkReference::operator igdeNVLink *() const{
	return ( igdeNVLink* )operator deObject *();
}

igdeNVLinkReference::operator igdeNVLink &() const{
	return ( igdeNVLink& )operator deObject &();
}

igdeNVLink *igdeNVLinkReference::operator->() const{
	return ( igdeNVLink* )deObjectReference::operator->();
}

igdeNVLinkReference &igdeNVLinkReference::operator=( igdeNVLink *link ){
	deObjectReference::operator=( link );
	return *this;
}

igdeNVLinkReference &igdeNVLinkReference::operator=( const igdeNVLinkReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVLinkReference::operator==( igdeNVLink *link ) const{
	return deObjectReference::operator==( link );
}

bool igdeNVLinkReference::operator!=( igdeNVLink *link ) const{
	return deObjectReference::operator!=( link );
}

bool igdeNVLinkReference::operator==( const igdeNVLinkReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVLinkReference::operator!=( const igdeNVLinkReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
