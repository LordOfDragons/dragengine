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

#include "igdeWindowLogger.h"
#include "igdeWindowLoggerReference.h"



// Class igdeWindowLoggerReference
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWindowLoggerReference::igdeWindowLoggerReference(){
}

igdeWindowLoggerReference::igdeWindowLoggerReference( igdeWindowLogger *window ) :
deObjectReference( window ){
}

igdeWindowLoggerReference::igdeWindowLoggerReference( const igdeWindowLoggerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWindowLoggerReference::TakeOver( igdeWindowLogger *window ){
	deObjectReference::TakeOver( window );
}

bool igdeWindowLoggerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWindowLoggerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWindowLoggerReference::operator igdeWindowLogger *() const{
	return ( igdeWindowLogger* )operator deObject *();
}

igdeWindowLoggerReference::operator igdeWindowLogger &() const{
	return ( igdeWindowLogger& )operator deObject &();
}

igdeWindowLogger *igdeWindowLoggerReference::operator->() const{
	return ( igdeWindowLogger* )deObjectReference::operator->();
}

igdeWindowLoggerReference &igdeWindowLoggerReference::operator=( igdeWindowLogger *window ){
	deObjectReference::operator=( window );
	return *this;
}

igdeWindowLoggerReference &igdeWindowLoggerReference::operator=( const igdeWindowLoggerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWindowLoggerReference::operator==( igdeWindowLogger *window ) const{
	return deObjectReference::operator==( window );
}

bool igdeWindowLoggerReference::operator!=( igdeWindowLogger *window ) const{
	return deObjectReference::operator!=( window );
}

bool igdeWindowLoggerReference::operator==( const igdeWindowLoggerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWindowLoggerReference::operator!=( const igdeWindowLoggerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
