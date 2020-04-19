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

#include "igdeGuiTheme.h"
#include "igdeGuiThemeReference.h"



// Class igdeGuiThemeReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGuiThemeReference::igdeGuiThemeReference(){
}

igdeGuiThemeReference::igdeGuiThemeReference( igdeGuiTheme *guitheme ) :
deObjectReference( guitheme ){
}

igdeGuiThemeReference::igdeGuiThemeReference( const igdeGuiThemeReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGuiThemeReference::TakeOver( igdeGuiTheme *guitheme ){
	deObjectReference::TakeOver( guitheme );
}

bool igdeGuiThemeReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGuiThemeReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGuiThemeReference::operator igdeGuiTheme *() const{
	return ( igdeGuiTheme* )operator deObject *();
}

igdeGuiThemeReference::operator igdeGuiTheme &() const{
	return ( igdeGuiTheme& )operator deObject &();
}

igdeGuiTheme *igdeGuiThemeReference::operator->() const{
	return ( igdeGuiTheme* )deObjectReference::operator->();
}

igdeGuiThemeReference &igdeGuiThemeReference::operator=( igdeGuiTheme *guitheme ){
	deObjectReference::operator=( guitheme );
	return *this;
}

igdeGuiThemeReference &igdeGuiThemeReference::operator=( const igdeGuiThemeReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGuiThemeReference::operator==( igdeGuiTheme *guitheme ) const{
	return deObjectReference::operator==( guitheme );
}

bool igdeGuiThemeReference::operator!=( igdeGuiTheme *guitheme ) const{
	return deObjectReference::operator!=( guitheme );
}

bool igdeGuiThemeReference::operator==( const igdeGuiThemeReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGuiThemeReference::operator!=( const igdeGuiThemeReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
