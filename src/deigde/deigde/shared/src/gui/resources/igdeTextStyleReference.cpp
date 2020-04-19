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

#include "igdeTextStyle.h"
#include "igdeTextStyleReference.h"



// Class igdeTextStyleReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextStyleReference::igdeTextStyleReference(){
}

igdeTextStyleReference::igdeTextStyleReference( igdeTextStyle *textStyle ) :
deObjectReference( textStyle ){
}

igdeTextStyleReference::igdeTextStyleReference( const igdeTextStyleReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextStyleReference::TakeOver( igdeTextStyle *textStyle ){
	deObjectReference::TakeOver( textStyle );
}

bool igdeTextStyleReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextStyleReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextStyleReference::operator igdeTextStyle *() const{
	return ( igdeTextStyle* )operator deObject *();
}

igdeTextStyleReference::operator igdeTextStyle &() const{
	return ( igdeTextStyle& )operator deObject &();
}

igdeTextStyle *igdeTextStyleReference::operator->() const{
	return ( igdeTextStyle* )deObjectReference::operator->();
}

igdeTextStyleReference &igdeTextStyleReference::operator=( igdeTextStyle *textStyle ){
	deObjectReference::operator=( textStyle );
	return *this;
}

igdeTextStyleReference &igdeTextStyleReference::operator=( const igdeTextStyleReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextStyleReference::operator==( igdeTextStyle *textStyle ) const{
	return deObjectReference::operator==( textStyle );
}

bool igdeTextStyleReference::operator!=( igdeTextStyle *textStyle ) const{
	return deObjectReference::operator!=( textStyle );
}

bool igdeTextStyleReference::operator==( const igdeTextStyleReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextStyleReference::operator!=( const igdeTextStyleReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
