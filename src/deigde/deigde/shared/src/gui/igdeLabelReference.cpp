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

#include "igdeLabel.h"
#include "igdeLabelReference.h"



// Class igdeLabelReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLabelReference::igdeLabelReference(){
}

igdeLabelReference::igdeLabelReference( igdeLabel *label ) :
deObjectReference( label ){
}

igdeLabelReference::igdeLabelReference( const igdeLabelReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeLabelReference::TakeOver( igdeLabel *label ){
	deObjectReference::TakeOver( label );
}

bool igdeLabelReference::operator!() const{
	return deObjectReference::operator!();
}

igdeLabelReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeLabelReference::operator igdeLabel *() const{
	return ( igdeLabel* )operator deObject *();
}

igdeLabelReference::operator igdeLabel &() const{
	return ( igdeLabel& )operator deObject &();
}

igdeLabel *igdeLabelReference::operator->() const{
	return ( igdeLabel* )deObjectReference::operator->();
}

igdeLabelReference &igdeLabelReference::operator=( igdeLabel *label ){
	deObjectReference::operator=( label );
	return *this;
}

igdeLabelReference &igdeLabelReference::operator=( const igdeLabelReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeLabelReference::operator==( igdeLabel *label ) const{
	return deObjectReference::operator==( label );
}

bool igdeLabelReference::operator!=( igdeLabel *label ) const{
	return deObjectReference::operator!=( label );
}

bool igdeLabelReference::operator==( const igdeLabelReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeLabelReference::operator!=( const igdeLabelReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
