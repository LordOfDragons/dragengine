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

#include "igdeNVBoard.h"
#include "igdeNVBoardReference.h"



// Class igdeNVBoardReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVBoardReference::igdeNVBoardReference(){
}

igdeNVBoardReference::igdeNVBoardReference( igdeNVBoard *widget ) :
deObjectReference( widget ){
}

igdeNVBoardReference::igdeNVBoardReference( const igdeNVBoardReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVBoardReference::TakeOver( igdeNVBoard *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeNVBoardReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVBoardReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVBoardReference::operator igdeNVBoard *() const{
	return ( igdeNVBoard* )operator deObject *();
}

igdeNVBoardReference::operator igdeNVBoard &() const{
	return ( igdeNVBoard& )operator deObject &();
}

igdeNVBoard *igdeNVBoardReference::operator->() const{
	return ( igdeNVBoard* )deObjectReference::operator->();
}

igdeNVBoardReference &igdeNVBoardReference::operator=( igdeNVBoard *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeNVBoardReference &igdeNVBoardReference::operator=( const igdeNVBoardReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVBoardReference::operator==( igdeNVBoard *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeNVBoardReference::operator!=( igdeNVBoard *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeNVBoardReference::operator==( const igdeNVBoardReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVBoardReference::operator!=( const igdeNVBoardReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
