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

#include "igdeWPTriggerTable.h"
#include "igdeWPTriggerTableReference.h"



// Class igdeWPTriggerTableReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPTriggerTableReference::igdeWPTriggerTableReference(){
}

igdeWPTriggerTableReference::igdeWPTriggerTableReference( igdeWPTriggerTable *widget ) :
deObjectReference( widget ){
}

igdeWPTriggerTableReference::igdeWPTriggerTableReference( const igdeWPTriggerTableReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPTriggerTableReference::TakeOver( igdeWPTriggerTable *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPTriggerTableReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPTriggerTableReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPTriggerTableReference::operator igdeWPTriggerTable *() const{
	return ( igdeWPTriggerTable* )operator deObject *();
}

igdeWPTriggerTableReference::operator igdeWPTriggerTable &() const{
	return ( igdeWPTriggerTable& )operator deObject &();
}

igdeWPTriggerTable *igdeWPTriggerTableReference::operator->() const{
	return ( igdeWPTriggerTable* )deObjectReference::operator->();
}

igdeWPTriggerTableReference &igdeWPTriggerTableReference::operator=( igdeWPTriggerTable *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPTriggerTableReference &igdeWPTriggerTableReference::operator=( const igdeWPTriggerTableReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPTriggerTableReference::operator==( igdeWPTriggerTable *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPTriggerTableReference::operator!=( igdeWPTriggerTable *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPTriggerTableReference::operator==( const igdeWPTriggerTableReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPTriggerTableReference::operator!=( const igdeWPTriggerTableReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
