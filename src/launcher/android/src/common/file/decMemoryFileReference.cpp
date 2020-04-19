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

#include "decMemoryFile.h"
#include "decMemoryFileReference.h"



// Class decMemoryFileReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

decMemoryFileReference::decMemoryFileReference(){
}

decMemoryFileReference::decMemoryFileReference( decMemoryFile *object ) :
deObjectReference( object ){
}

decMemoryFileReference::decMemoryFileReference( const decMemoryFileReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decMemoryFileReference::TakeOver( decMemoryFile *object ){
	deObjectReference::TakeOver( object );
}

bool decMemoryFileReference::operator!() const{
	return deObjectReference::operator!();
}

decMemoryFileReference::operator bool() const{
	return deObjectReference::operator bool();
}

decMemoryFileReference::operator decMemoryFile *() const{
	return ( decMemoryFile* )operator deObject *();
}

decMemoryFileReference::operator decMemoryFile &() const{
	return ( decMemoryFile& )operator deObject &();
}

decMemoryFile *decMemoryFileReference::operator->() const{
	return ( decMemoryFile* )deObjectReference::operator->();
}

decMemoryFileReference &decMemoryFileReference::operator=( decMemoryFile *object ){
	deObjectReference::operator=( object );
	return *this;
}

decMemoryFileReference &decMemoryFileReference::operator=( const decMemoryFileReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decMemoryFileReference::operator==( decMemoryFile *object ) const{
	return deObjectReference::operator==( object );
}

bool decMemoryFileReference::operator!=( decMemoryFile *object ) const{
	return deObjectReference::operator!=( object );
}

bool decMemoryFileReference::operator==( const decMemoryFileReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decMemoryFileReference::operator!=( const decMemoryFileReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
