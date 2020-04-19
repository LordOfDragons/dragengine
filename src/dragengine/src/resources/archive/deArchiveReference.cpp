/* 
 * Drag[en]gine Game Engine
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

#include "deArchive.h"
#include "deArchiveReference.h"



// Class deArchiveReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

deArchiveReference::deArchiveReference(){
}

deArchiveReference::deArchiveReference( deArchive *object ) :
deObjectReference( object ){
}

deArchiveReference::deArchiveReference( const deArchiveReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deArchiveReference::TakeOver( deArchive *object ){
	deObjectReference::TakeOver( object );
}

bool deArchiveReference::operator!() const{
	return deObjectReference::operator!();
}

deArchiveReference::operator bool() const{
	return deObjectReference::operator bool();
}

deArchiveReference::operator deArchive *() const{
	return ( deArchive* )operator deObject *();
}

deArchiveReference::operator deArchive &() const{
	return ( deArchive& )operator deObject &();
}

deArchive *deArchiveReference::operator->() const{
	return ( deArchive* )deObjectReference::operator->();
}

deArchiveReference &deArchiveReference::operator=( deArchive *object ){
	deObjectReference::operator=( object );
	return *this;
}

deArchiveReference &deArchiveReference::operator=( const deArchiveReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deArchiveReference::operator==( deArchive *object ) const{
	return deObjectReference::operator==( object );
}

bool deArchiveReference::operator!=( deArchive *object ) const{
	return deObjectReference::operator!=( object );
}

bool deArchiveReference::operator==( const deArchiveReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deArchiveReference::operator!=( const deArchiveReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
