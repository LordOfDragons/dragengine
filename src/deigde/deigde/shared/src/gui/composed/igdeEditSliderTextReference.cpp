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

#include "igdeEditSliderText.h"
#include "igdeEditSliderTextReference.h"



// Class igdeEditSliderTextReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditSliderTextReference::igdeEditSliderTextReference(){
}

igdeEditSliderTextReference::igdeEditSliderTextReference( igdeEditSliderText *editSliderText ) :
deObjectReference( editSliderText ){
}

igdeEditSliderTextReference::igdeEditSliderTextReference( const igdeEditSliderTextReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditSliderTextReference::TakeOver( igdeEditSliderText *editSliderText ){
	deObjectReference::TakeOver( editSliderText );
}

bool igdeEditSliderTextReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditSliderTextReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditSliderTextReference::operator igdeEditSliderText *() const{
	return ( igdeEditSliderText* )operator deObject *();
}

igdeEditSliderTextReference::operator igdeEditSliderText &() const{
	return ( igdeEditSliderText& )operator deObject &();
}

igdeEditSliderText *igdeEditSliderTextReference::operator->() const{
	return ( igdeEditSliderText* )deObjectReference::operator->();
}

igdeEditSliderTextReference &igdeEditSliderTextReference::operator=( igdeEditSliderText *editSliderText ){
	deObjectReference::operator=( editSliderText );
	return *this;
}

igdeEditSliderTextReference &igdeEditSliderTextReference::operator=( const igdeEditSliderTextReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditSliderTextReference::operator==( igdeEditSliderText *editSliderText ) const{
	return deObjectReference::operator==( editSliderText );
}

bool igdeEditSliderTextReference::operator!=( igdeEditSliderText *editSliderText ) const{
	return deObjectReference::operator!=( editSliderText );
}

bool igdeEditSliderTextReference::operator==( const igdeEditSliderTextReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditSliderTextReference::operator!=( const igdeEditSliderTextReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
