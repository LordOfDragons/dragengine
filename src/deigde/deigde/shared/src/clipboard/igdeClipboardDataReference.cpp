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

#include "igdeClipboardData.h"
#include "igdeClipboardDataReference.h"



// Class igdeClipboardDataReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeClipboardDataReference::igdeClipboardDataReference(){
}

igdeClipboardDataReference::igdeClipboardDataReference( igdeClipboardData *clipboardData ) :
deObjectReference( clipboardData ){
}

igdeClipboardDataReference::igdeClipboardDataReference( const igdeClipboardDataReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeClipboardDataReference::TakeOver( igdeClipboardData *clipboardData ){
	deObjectReference::TakeOver( clipboardData );
}

bool igdeClipboardDataReference::operator!() const{
	return deObjectReference::operator!();
}

igdeClipboardDataReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeClipboardDataReference::operator igdeClipboardData *() const{
	return ( igdeClipboardData* )operator deObject *();
}

igdeClipboardDataReference::operator igdeClipboardData &() const{
	return ( igdeClipboardData& )operator deObject &();
}

igdeClipboardData *igdeClipboardDataReference::operator->() const{
	return ( igdeClipboardData* )deObjectReference::operator->();
}

igdeClipboardDataReference &igdeClipboardDataReference::operator=( igdeClipboardData *clipboardData ){
	deObjectReference::operator=( clipboardData );
	return *this;
}

igdeClipboardDataReference &igdeClipboardDataReference::operator=( const igdeClipboardDataReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeClipboardDataReference::operator==( igdeClipboardData *clipboardData ) const{
	return deObjectReference::operator==( clipboardData );
}

bool igdeClipboardDataReference::operator!=( igdeClipboardData *clipboardData ) const{
	return deObjectReference::operator!=( clipboardData );
}

bool igdeClipboardDataReference::operator==( const igdeClipboardDataReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeClipboardDataReference::operator!=( const igdeClipboardDataReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
