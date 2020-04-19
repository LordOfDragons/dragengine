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

#include "igdeUndo.h"
#include "igdeUndoReference.h"



// Class igdeUndoReference
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeUndoReference::igdeUndoReference(){
}

igdeUndoReference::igdeUndoReference( igdeUndo *undo ) :
deObjectReference( undo ){
}

igdeUndoReference::igdeUndoReference( const igdeUndoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeUndoReference::TakeOver( igdeUndo *undo ){
	deObjectReference::TakeOver( undo );
}

bool igdeUndoReference::operator!() const{
	return deObjectReference::operator!();
}

igdeUndoReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeUndoReference::operator igdeUndo *() const{
	return ( igdeUndo* )operator deObject *();
}

igdeUndoReference::operator igdeUndo &() const{
	return ( igdeUndo& )operator deObject &();
}

igdeUndo *igdeUndoReference::operator->() const{
	return ( igdeUndo* )deObjectReference::operator->();
}

igdeUndoReference &igdeUndoReference::operator=( igdeUndo *undo ){
	deObjectReference::operator=( undo );
	return *this;
}

igdeUndoReference &igdeUndoReference::operator=( const igdeUndoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeUndoReference::operator==( igdeUndo *undo ) const{
	return deObjectReference::operator==( undo );
}

bool igdeUndoReference::operator!=( igdeUndo *undo ) const{
	return deObjectReference::operator!=( undo );
}

bool igdeUndoReference::operator==( const igdeUndoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeUndoReference::operator!=( const igdeUndoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
