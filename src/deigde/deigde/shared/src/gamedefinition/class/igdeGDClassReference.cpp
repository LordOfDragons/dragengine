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

#include "igdeGDClass.h"
#include "igdeGDClassReference.h"



// Class igdeGDClassReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDClassReference::igdeGDClassReference(){
}

igdeGDClassReference::igdeGDClassReference( igdeGDClass *gdclass ) :
deObjectReference( gdclass ){
}

igdeGDClassReference::igdeGDClassReference( const igdeGDClassReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDClassReference::TakeOver( igdeGDClass *gdclass ){
	deObjectReference::TakeOver( gdclass );
}

bool igdeGDClassReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDClassReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDClassReference::operator igdeGDClass *() const{
	return ( igdeGDClass* )operator deObject *();
}

igdeGDClassReference::operator igdeGDClass &() const{
	return ( igdeGDClass& )operator deObject &();
}

igdeGDClass *igdeGDClassReference::operator->() const{
	return ( igdeGDClass* )deObjectReference::operator->();
}

igdeGDClassReference &igdeGDClassReference::operator=( igdeGDClass *gdclass ){
	deObjectReference::operator=( gdclass );
	return *this;
}

igdeGDClassReference &igdeGDClassReference::operator=( const igdeGDClassReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDClassReference::operator==( igdeGDClass *gdclass ) const{
	return deObjectReference::operator==( gdclass );
}

bool igdeGDClassReference::operator!=( igdeGDClass *gdclass ) const{
	return deObjectReference::operator!=( gdclass );
}

bool igdeGDClassReference::operator==( const igdeGDClassReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDClassReference::operator!=( const igdeGDClassReference &reference ) const{
	return deObjectReference::operator!=( reference );
}

