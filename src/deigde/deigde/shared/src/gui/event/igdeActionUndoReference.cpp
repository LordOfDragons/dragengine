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

#include "igdeActionUndo.h"
#include "igdeActionUndoReference.h"



// Class igdeActionUndoReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionUndoReference::igdeActionUndoReference(){
}

igdeActionUndoReference::igdeActionUndoReference( igdeActionUndo *action ) :
deObjectReference( action ){
}

igdeActionUndoReference::igdeActionUndoReference( const igdeActionUndoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionUndoReference::TakeOver( igdeActionUndo *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionUndoReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionUndoReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionUndoReference::operator igdeActionUndo *() const{
	return ( igdeActionUndo* )operator deObject *();
}

igdeActionUndoReference::operator igdeActionUndo &() const{
	return ( igdeActionUndo& )operator deObject &();
}

igdeActionUndo *igdeActionUndoReference::operator->() const{
	return ( igdeActionUndo* )deObjectReference::operator->();
}

igdeActionUndoReference &igdeActionUndoReference::operator=( igdeActionUndo *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionUndoReference &igdeActionUndoReference::operator=( const igdeActionUndoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionUndoReference::operator==( igdeActionUndo *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionUndoReference::operator!=( igdeActionUndo *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionUndoReference::operator==( const igdeActionUndoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionUndoReference::operator!=( const igdeActionUndoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
