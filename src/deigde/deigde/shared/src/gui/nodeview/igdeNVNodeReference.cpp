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

#include "igdeNVNode.h"
#include "igdeNVNodeReference.h"



// Class igdeNVNodeReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVNodeReference::igdeNVNodeReference(){
}

igdeNVNodeReference::igdeNVNodeReference( igdeNVNode *widget ) :
deObjectReference( widget ){
}

igdeNVNodeReference::igdeNVNodeReference( const igdeNVNodeReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVNodeReference::TakeOver( igdeNVNode *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeNVNodeReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVNodeReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVNodeReference::operator igdeNVNode *() const{
	return ( igdeNVNode* )operator deObject *();
}

igdeNVNodeReference::operator igdeNVNode &() const{
	return ( igdeNVNode& )operator deObject &();
}

igdeNVNode *igdeNVNodeReference::operator->() const{
	return ( igdeNVNode* )deObjectReference::operator->();
}

igdeNVNodeReference &igdeNVNodeReference::operator=( igdeNVNode *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeNVNodeReference &igdeNVNodeReference::operator=( const igdeNVNodeReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVNodeReference::operator==( igdeNVNode *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeNVNodeReference::operator!=( igdeNVNode *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeNVNodeReference::operator==( const igdeNVNodeReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVNodeReference::operator!=( const igdeNVNodeReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
