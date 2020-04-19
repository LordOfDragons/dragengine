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

#include "igdeContainerSplitted.h"
#include "igdeContainerSplittedReference.h"



// Class igdeContainerSplittedReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerSplittedReference::igdeContainerSplittedReference(){
}

igdeContainerSplittedReference::igdeContainerSplittedReference( igdeContainerSplitted *widget ) :
deObjectReference( widget ){
}

igdeContainerSplittedReference::igdeContainerSplittedReference( const igdeContainerSplittedReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeContainerSplittedReference::TakeOver( igdeContainerSplitted *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeContainerSplittedReference::operator!() const{
	return deObjectReference::operator!();
}

igdeContainerSplittedReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeContainerSplittedReference::operator igdeContainerSplitted *() const{
	return ( igdeContainerSplitted* )operator deObject *();
}

igdeContainerSplittedReference::operator igdeContainerSplitted &() const{
	return ( igdeContainerSplitted& )operator deObject &();
}

igdeContainerSplitted *igdeContainerSplittedReference::operator->() const{
	return ( igdeContainerSplitted* )deObjectReference::operator->();
}

igdeContainerSplittedReference &igdeContainerSplittedReference::operator=( igdeContainerSplitted *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeContainerSplittedReference &igdeContainerSplittedReference::operator=( const igdeContainerSplittedReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeContainerSplittedReference::operator==( igdeContainerSplitted *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeContainerSplittedReference::operator!=( igdeContainerSplitted *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeContainerSplittedReference::operator==( const igdeContainerSplittedReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeContainerSplittedReference::operator!=( const igdeContainerSplittedReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
