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

#include "igdeSpacer.h"
#include "igdeSpacerReference.h"



// Class igdeSpacerReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpacerReference::igdeSpacerReference(){
}

igdeSpacerReference::igdeSpacerReference( igdeSpacer *spacer ) :
deObjectReference( spacer ){
}

igdeSpacerReference::igdeSpacerReference( const igdeSpacerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSpacerReference::TakeOver( igdeSpacer *spacer ){
	deObjectReference::TakeOver( spacer );
}

bool igdeSpacerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSpacerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSpacerReference::operator igdeSpacer *() const{
	return ( igdeSpacer* )operator deObject *();
}

igdeSpacerReference::operator igdeSpacer &() const{
	return ( igdeSpacer& )operator deObject &();
}

igdeSpacer *igdeSpacerReference::operator->() const{
	return ( igdeSpacer* )deObjectReference::operator->();
}

igdeSpacerReference &igdeSpacerReference::operator=( igdeSpacer *spacer ){
	deObjectReference::operator=( spacer );
	return *this;
}

igdeSpacerReference &igdeSpacerReference::operator=( const igdeSpacerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSpacerReference::operator==( igdeSpacer *spacer ) const{
	return deObjectReference::operator==( spacer );
}

bool igdeSpacerReference::operator!=( igdeSpacer *spacer ) const{
	return deObjectReference::operator!=( spacer );
}

bool igdeSpacerReference::operator==( const igdeSpacerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSpacerReference::operator!=( const igdeSpacerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
