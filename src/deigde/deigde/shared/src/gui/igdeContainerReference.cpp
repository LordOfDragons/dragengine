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

#include "igdeContainer.h"
#include "igdeContainerReference.h"



// Class igdeContainerReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerReference::igdeContainerReference(){
}

igdeContainerReference::igdeContainerReference( igdeContainer *container ) :
deObjectReference( container ){
}

igdeContainerReference::igdeContainerReference( const igdeContainerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeContainerReference::TakeOver( igdeContainer *container ){
	deObjectReference::TakeOver( container );
}

bool igdeContainerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeContainerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeContainerReference::operator igdeContainer *() const{
	return ( igdeContainer* )operator deObject *();
}

igdeContainerReference::operator igdeContainer &() const{
	return ( igdeContainer& )operator deObject &();
}

igdeContainer *igdeContainerReference::operator->() const{
	return ( igdeContainer* )deObjectReference::operator->();
}

igdeContainerReference &igdeContainerReference::operator=( igdeContainer *container ){
	deObjectReference::operator=( container );
	return *this;
}

igdeContainerReference &igdeContainerReference::operator=( const igdeContainerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeContainerReference::operator==( igdeContainer *container ) const{
	return deObjectReference::operator==( container );
}

bool igdeContainerReference::operator!=( igdeContainer *container ) const{
	return deObjectReference::operator!=( container );
}

bool igdeContainerReference::operator==( const igdeContainerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeContainerReference::operator!=( const igdeContainerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
