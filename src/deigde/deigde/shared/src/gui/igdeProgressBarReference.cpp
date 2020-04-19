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

#include "igdeProgressBar.h"
#include "igdeProgressBarReference.h"



// Class igdeProgressBarReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeProgressBarReference::igdeProgressBarReference(){
}

igdeProgressBarReference::igdeProgressBarReference( igdeProgressBar *textfield ) :
deObjectReference( textfield ){
}

igdeProgressBarReference::igdeProgressBarReference( const igdeProgressBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeProgressBarReference::TakeOver( igdeProgressBar *textfield ){
	deObjectReference::TakeOver( textfield );
}

bool igdeProgressBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeProgressBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeProgressBarReference::operator igdeProgressBar *() const{
	return ( igdeProgressBar* )operator deObject *();
}

igdeProgressBarReference::operator igdeProgressBar &() const{
	return ( igdeProgressBar& )operator deObject &();
}

igdeProgressBar *igdeProgressBarReference::operator->() const{
	return ( igdeProgressBar* )deObjectReference::operator->();
}

igdeProgressBarReference &igdeProgressBarReference::operator=( igdeProgressBar *textfield ){
	deObjectReference::operator=( textfield );
	return *this;
}

igdeProgressBarReference &igdeProgressBarReference::operator=( const igdeProgressBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeProgressBarReference::operator==( igdeProgressBar *textfield ) const{
	return deObjectReference::operator==( textfield );
}

bool igdeProgressBarReference::operator!=( igdeProgressBar *textfield ) const{
	return deObjectReference::operator!=( textfield );
}

bool igdeProgressBarReference::operator==( const igdeProgressBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeProgressBarReference::operator!=( const igdeProgressBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
