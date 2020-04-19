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

#include "igdeSlider.h"
#include "igdeSliderReference.h"



// Class igdeSliderReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSliderReference::igdeSliderReference(){
}

igdeSliderReference::igdeSliderReference( igdeSlider *slider ) :
deObjectReference( slider ){
}

igdeSliderReference::igdeSliderReference( const igdeSliderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSliderReference::TakeOver( igdeSlider *slider ){
	deObjectReference::TakeOver( slider );
}

bool igdeSliderReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSliderReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSliderReference::operator igdeSlider *() const{
	return ( igdeSlider* )operator deObject *();
}

igdeSliderReference::operator igdeSlider &() const{
	return ( igdeSlider& )operator deObject &();
}

igdeSlider *igdeSliderReference::operator->() const{
	return ( igdeSlider* )deObjectReference::operator->();
}

igdeSliderReference &igdeSliderReference::operator=( igdeSlider *slider ){
	deObjectReference::operator=( slider );
	return *this;
}

igdeSliderReference &igdeSliderReference::operator=( const igdeSliderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSliderReference::operator==( igdeSlider *slider ) const{
	return deObjectReference::operator==( slider );
}

bool igdeSliderReference::operator!=( igdeSlider *slider ) const{
	return deObjectReference::operator!=( slider );
}

bool igdeSliderReference::operator==( const igdeSliderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSliderReference::operator!=( const igdeSliderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
