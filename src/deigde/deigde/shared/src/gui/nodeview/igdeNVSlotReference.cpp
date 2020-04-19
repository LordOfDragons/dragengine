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

#include "igdeNVSlot.h"
#include "igdeNVSlotReference.h"



// Class igdeNVSlotReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVSlotReference::igdeNVSlotReference(){
}

igdeNVSlotReference::igdeNVSlotReference( igdeNVSlot *widget ) :
deObjectReference( widget ){
}

igdeNVSlotReference::igdeNVSlotReference( const igdeNVSlotReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVSlotReference::TakeOver( igdeNVSlot *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeNVSlotReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVSlotReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVSlotReference::operator igdeNVSlot *() const{
	return ( igdeNVSlot* )operator deObject *();
}

igdeNVSlotReference::operator igdeNVSlot &() const{
	return ( igdeNVSlot& )operator deObject &();
}

igdeNVSlot *igdeNVSlotReference::operator->() const{
	return ( igdeNVSlot* )deObjectReference::operator->();
}

igdeNVSlotReference &igdeNVSlotReference::operator=( igdeNVSlot *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeNVSlotReference &igdeNVSlotReference::operator=( const igdeNVSlotReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVSlotReference::operator==( igdeNVSlot *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeNVSlotReference::operator!=( igdeNVSlot *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeNVSlotReference::operator==( const igdeNVSlotReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVSlotReference::operator!=( const igdeNVSlotReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
