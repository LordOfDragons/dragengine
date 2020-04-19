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

#include "igdeContainerBorder.h"
#include "igdeContainerBorderReference.h"



// Class igdeContainerBorderReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerBorderReference::igdeContainerBorderReference(){
}

igdeContainerBorderReference::igdeContainerBorderReference( igdeContainerBorder *widget ) :
deObjectReference( widget ){
}

igdeContainerBorderReference::igdeContainerBorderReference( const igdeContainerBorderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeContainerBorderReference::TakeOver( igdeContainerBorder *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeContainerBorderReference::operator!() const{
	return deObjectReference::operator!();
}

igdeContainerBorderReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeContainerBorderReference::operator igdeContainerBorder *() const{
	return ( igdeContainerBorder* )operator deObject *();
}

igdeContainerBorderReference::operator igdeContainerBorder &() const{
	return ( igdeContainerBorder& )operator deObject &();
}

igdeContainerBorder *igdeContainerBorderReference::operator->() const{
	return ( igdeContainerBorder* )deObjectReference::operator->();
}

igdeContainerBorderReference &igdeContainerBorderReference::operator=( igdeContainerBorder *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeContainerBorderReference &igdeContainerBorderReference::operator=( const igdeContainerBorderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeContainerBorderReference::operator==( igdeContainerBorder *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeContainerBorderReference::operator!=( igdeContainerBorder *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeContainerBorderReference::operator==( const igdeContainerBorderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeContainerBorderReference::operator!=( const igdeContainerBorderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
