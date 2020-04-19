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

#include "igdeGDCategory.h"
#include "igdeGDCategoryReference.h"



// Class igdeGDCategoryReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCategoryReference::igdeGDCategoryReference(){
}

igdeGDCategoryReference::igdeGDCategoryReference( igdeGDCategory *object ) :
deObjectReference( object ){
}

igdeGDCategoryReference::igdeGDCategoryReference( const igdeGDCategoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDCategoryReference::TakeOver( igdeGDCategory *object ){
	deObjectReference::TakeOver( object );
}

bool igdeGDCategoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDCategoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDCategoryReference::operator igdeGDCategory *() const{
	return ( igdeGDCategory* )operator deObject *();
}

igdeGDCategoryReference::operator igdeGDCategory &() const{
	return ( igdeGDCategory& )operator deObject &();
}

igdeGDCategory *igdeGDCategoryReference::operator->() const{
	return ( igdeGDCategory* )deObjectReference::operator->();
}

igdeGDCategoryReference &igdeGDCategoryReference::operator=( igdeGDCategory *object ){
	deObjectReference::operator=( object );
	return *this;
}

igdeGDCategoryReference &igdeGDCategoryReference::operator=( const igdeGDCategoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDCategoryReference::operator==( igdeGDCategory *object ) const{
	return deObjectReference::operator==( object );
}

bool igdeGDCategoryReference::operator!=( igdeGDCategory *object ) const{
	return deObjectReference::operator!=( object );
}

bool igdeGDCategoryReference::operator==( const igdeGDCategoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDCategoryReference::operator!=( const igdeGDCategoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
