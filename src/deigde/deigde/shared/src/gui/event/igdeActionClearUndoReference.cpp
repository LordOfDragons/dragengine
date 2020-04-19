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

#include "igdeActionClearUndo.h"
#include "igdeActionClearUndoReference.h"



// Class igdeActionClearUndoReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionClearUndoReference::igdeActionClearUndoReference(){
}

igdeActionClearUndoReference::igdeActionClearUndoReference( igdeActionClearUndo *action ) :
deObjectReference( action ){
}

igdeActionClearUndoReference::igdeActionClearUndoReference( const igdeActionClearUndoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionClearUndoReference::TakeOver( igdeActionClearUndo *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionClearUndoReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionClearUndoReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionClearUndoReference::operator igdeActionClearUndo *() const{
	return ( igdeActionClearUndo* )operator deObject *();
}

igdeActionClearUndoReference::operator igdeActionClearUndo &() const{
	return ( igdeActionClearUndo& )operator deObject &();
}

igdeActionClearUndo *igdeActionClearUndoReference::operator->() const{
	return ( igdeActionClearUndo* )deObjectReference::operator->();
}

igdeActionClearUndoReference &igdeActionClearUndoReference::operator=( igdeActionClearUndo *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionClearUndoReference &igdeActionClearUndoReference::operator=( const igdeActionClearUndoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionClearUndoReference::operator==( igdeActionClearUndo *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionClearUndoReference::operator!=( igdeActionClearUndo *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionClearUndoReference::operator==( const igdeActionClearUndoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionClearUndoReference::operator!=( const igdeActionClearUndoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
