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

#include "igdeWPUndoHistory.h"
#include "igdeWPUndoHistoryReference.h"



// Class igdeWPUndoHistoryReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPUndoHistoryReference::igdeWPUndoHistoryReference(){
}

igdeWPUndoHistoryReference::igdeWPUndoHistoryReference( igdeWPUndoHistory *widget ) :
deObjectReference( widget ){
}

igdeWPUndoHistoryReference::igdeWPUndoHistoryReference( const igdeWPUndoHistoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPUndoHistoryReference::TakeOver( igdeWPUndoHistory *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPUndoHistoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPUndoHistoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPUndoHistoryReference::operator igdeWPUndoHistory *() const{
	return ( igdeWPUndoHistory* )operator deObject *();
}

igdeWPUndoHistoryReference::operator igdeWPUndoHistory &() const{
	return ( igdeWPUndoHistory& )operator deObject &();
}

igdeWPUndoHistory *igdeWPUndoHistoryReference::operator->() const{
	return ( igdeWPUndoHistory* )deObjectReference::operator->();
}

igdeWPUndoHistoryReference &igdeWPUndoHistoryReference::operator=( igdeWPUndoHistory *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPUndoHistoryReference &igdeWPUndoHistoryReference::operator=( const igdeWPUndoHistoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPUndoHistoryReference::operator==( igdeWPUndoHistory *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPUndoHistoryReference::operator!=( igdeWPUndoHistory *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPUndoHistoryReference::operator==( const igdeWPUndoHistoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPUndoHistoryReference::operator!=( const igdeWPUndoHistoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
