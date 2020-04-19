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

#include "igdeTimer.h"
#include "igdeTimerReference.h"



// Class igdeTimerReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTimerReference::igdeTimerReference(){
}

igdeTimerReference::igdeTimerReference( igdeTimer *timer ) :
deObjectReference( timer ){
}

igdeTimerReference::igdeTimerReference( const igdeTimerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTimerReference::TakeOver( igdeTimer *timer ){
	deObjectReference::TakeOver( timer );
}

bool igdeTimerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTimerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTimerReference::operator igdeTimer *() const{
	return ( igdeTimer* )operator deObject *();
}

igdeTimerReference::operator igdeTimer &() const{
	return ( igdeTimer& )operator deObject &();
}

igdeTimer *igdeTimerReference::operator->() const{
	return ( igdeTimer* )deObjectReference::operator->();
}

igdeTimerReference &igdeTimerReference::operator=( igdeTimer *timer ){
	deObjectReference::operator=( timer );
	return *this;
}

igdeTimerReference &igdeTimerReference::operator=( const igdeTimerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTimerReference::operator==( igdeTimer *timer ) const{
	return deObjectReference::operator==( timer );
}

bool igdeTimerReference::operator!=( igdeTimer *timer ) const{
	return deObjectReference::operator!=( timer );
}

bool igdeTimerReference::operator==( const igdeTimerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTimerReference::operator!=( const igdeTimerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
