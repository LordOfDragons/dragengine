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

#include "igdeMenuCascade.h"
#include "igdeMenuCascadeReference.h"



// Class igdeMenuCascadeReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCascadeReference::igdeMenuCascadeReference(){
}

igdeMenuCascadeReference::igdeMenuCascadeReference( igdeMenuCascade *menuCascade ) :
deObjectReference( menuCascade ){
}

igdeMenuCascadeReference::igdeMenuCascadeReference( const igdeMenuCascadeReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMenuCascadeReference::TakeOver( igdeMenuCascade *menuCascade ){
	deObjectReference::TakeOver( menuCascade );
}

bool igdeMenuCascadeReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMenuCascadeReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMenuCascadeReference::operator igdeMenuCascade *() const{
	return ( igdeMenuCascade* )operator deObject *();
}

igdeMenuCascadeReference::operator igdeMenuCascade &() const{
	return ( igdeMenuCascade& )operator deObject &();
}

igdeMenuCascade *igdeMenuCascadeReference::operator->() const{
	return ( igdeMenuCascade* )deObjectReference::operator->();
}

igdeMenuCascadeReference &igdeMenuCascadeReference::operator=( igdeMenuCascade *menuCascade ){
	deObjectReference::operator=( menuCascade );
	return *this;
}

igdeMenuCascadeReference &igdeMenuCascadeReference::operator=( const igdeMenuCascadeReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMenuCascadeReference::operator==( igdeMenuCascade *menuCascade ) const{
	return deObjectReference::operator==( menuCascade );
}

bool igdeMenuCascadeReference::operator!=( igdeMenuCascade *menuCascade ) const{
	return deObjectReference::operator!=( menuCascade );
}

bool igdeMenuCascadeReference::operator==( const igdeMenuCascadeReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMenuCascadeReference::operator!=( const igdeMenuCascadeReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
