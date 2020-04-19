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

#include "igdeToggleTags.h"
#include "igdeToggleTagsReference.h"



// Class igdeToggleTagsReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToggleTagsReference::igdeToggleTagsReference(){
}

igdeToggleTagsReference::igdeToggleTagsReference( igdeToggleTags *widget ) :
deObjectReference( widget ){
}

igdeToggleTagsReference::igdeToggleTagsReference( const igdeToggleTagsReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToggleTagsReference::TakeOver( igdeToggleTags *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeToggleTagsReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToggleTagsReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToggleTagsReference::operator igdeToggleTags *() const{
	return ( igdeToggleTags* )operator deObject *();
}

igdeToggleTagsReference::operator igdeToggleTags &() const{
	return ( igdeToggleTags& )operator deObject &();
}

igdeToggleTags *igdeToggleTagsReference::operator->() const{
	return ( igdeToggleTags* )deObjectReference::operator->();
}

igdeToggleTagsReference &igdeToggleTagsReference::operator=( igdeToggleTags *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeToggleTagsReference &igdeToggleTagsReference::operator=( const igdeToggleTagsReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToggleTagsReference::operator==( igdeToggleTags *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeToggleTagsReference::operator!=( igdeToggleTags *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeToggleTagsReference::operator==( const igdeToggleTagsReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToggleTagsReference::operator!=( const igdeToggleTagsReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
