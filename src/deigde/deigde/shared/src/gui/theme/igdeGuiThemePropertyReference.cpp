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

#include "igdeGuiThemeProperty.h"
#include "igdeGuiThemePropertyReference.h"



// Class igdeGuiThemePropertyReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGuiThemePropertyReference::igdeGuiThemePropertyReference(){
}

igdeGuiThemePropertyReference::igdeGuiThemePropertyReference( igdeGuiThemeProperty *property ) :
deObjectReference( property ){
}

igdeGuiThemePropertyReference::igdeGuiThemePropertyReference( const igdeGuiThemePropertyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGuiThemePropertyReference::TakeOver( igdeGuiThemeProperty *property ){
	deObjectReference::TakeOver( property );
}

bool igdeGuiThemePropertyReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGuiThemePropertyReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGuiThemePropertyReference::operator igdeGuiThemeProperty *() const{
	return ( igdeGuiThemeProperty* )operator deObject *();
}

igdeGuiThemePropertyReference::operator igdeGuiThemeProperty &() const{
	return ( igdeGuiThemeProperty& )operator deObject &();
}

igdeGuiThemeProperty *igdeGuiThemePropertyReference::operator->() const{
	return ( igdeGuiThemeProperty* )deObjectReference::operator->();
}

igdeGuiThemePropertyReference &igdeGuiThemePropertyReference::operator=( igdeGuiThemeProperty *property ){
	deObjectReference::operator=( property );
	return *this;
}

igdeGuiThemePropertyReference &igdeGuiThemePropertyReference::operator=( const igdeGuiThemePropertyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGuiThemePropertyReference::operator==( igdeGuiThemeProperty *property ) const{
	return deObjectReference::operator==( property );
}

bool igdeGuiThemePropertyReference::operator!=( igdeGuiThemeProperty *property ) const{
	return deObjectReference::operator!=( property );
}

bool igdeGuiThemePropertyReference::operator==( const igdeGuiThemePropertyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGuiThemePropertyReference::operator!=( const igdeGuiThemePropertyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
