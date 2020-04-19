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

#include "decBaseFileReader.h"
#include "decBaseFileReaderReference.h"



// Class decBaseFileReaderReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

decBaseFileReaderReference::decBaseFileReaderReference(){
}

decBaseFileReaderReference::decBaseFileReaderReference( decBaseFileReader *object ) :
deObjectReference( object ){
}

decBaseFileReaderReference::decBaseFileReaderReference( const decBaseFileReaderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decBaseFileReaderReference::TakeOver( decBaseFileReader *object ){
	deObjectReference::TakeOver( object );
}

bool decBaseFileReaderReference::operator!() const{
	return deObjectReference::operator!();
}

decBaseFileReaderReference::operator bool() const{
	return deObjectReference::operator bool();
}

decBaseFileReaderReference::operator decBaseFileReader *() const{
	return ( decBaseFileReader* )operator deObject *();
}

decBaseFileReaderReference::operator decBaseFileReader &() const{
	return ( decBaseFileReader& )operator deObject &();
}

decBaseFileReader *decBaseFileReaderReference::operator->() const{
	return ( decBaseFileReader* )deObjectReference::operator->();
}

decBaseFileReaderReference &decBaseFileReaderReference::operator=( decBaseFileReader *object ){
	deObjectReference::operator=( object );
	return *this;
}

decBaseFileReaderReference &decBaseFileReaderReference::operator=( const decBaseFileReaderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decBaseFileReaderReference::operator==( decBaseFileReader *object ) const{
	return deObjectReference::operator==( object );
}

bool decBaseFileReaderReference::operator!=( decBaseFileReader *object ) const{
	return deObjectReference::operator!=( object );
}

bool decBaseFileReaderReference::operator==( const decBaseFileReaderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decBaseFileReaderReference::operator!=( const decBaseFileReaderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
