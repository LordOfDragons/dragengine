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

#include "igdeActionSelectFile.h"
#include "igdeActionSelectFileReference.h"



// Class igdeActionSelectFileReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectFileReference::igdeActionSelectFileReference(){
}

igdeActionSelectFileReference::igdeActionSelectFileReference( igdeActionSelectFile *action ) :
deObjectReference( action ){
}

igdeActionSelectFileReference::igdeActionSelectFileReference( const igdeActionSelectFileReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionSelectFileReference::TakeOver( igdeActionSelectFile *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionSelectFileReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionSelectFileReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionSelectFileReference::operator igdeActionSelectFile *() const{
	return ( igdeActionSelectFile* )operator deObject *();
}

igdeActionSelectFileReference::operator igdeActionSelectFile &() const{
	return ( igdeActionSelectFile& )operator deObject &();
}

igdeActionSelectFile *igdeActionSelectFileReference::operator->() const{
	return ( igdeActionSelectFile* )deObjectReference::operator->();
}

igdeActionSelectFileReference &igdeActionSelectFileReference::operator=( igdeActionSelectFile *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionSelectFileReference &igdeActionSelectFileReference::operator=( const igdeActionSelectFileReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionSelectFileReference::operator==( igdeActionSelectFile *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionSelectFileReference::operator!=( igdeActionSelectFile *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionSelectFileReference::operator==( const igdeActionSelectFileReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionSelectFileReference::operator!=( const igdeActionSelectFileReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
