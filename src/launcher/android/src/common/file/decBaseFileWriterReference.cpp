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

#include "decBaseFileWriter.h"
#include "decBaseFileWriterReference.h"



// Class decBaseFileWriterReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

decBaseFileWriterReference::decBaseFileWriterReference(){
}

decBaseFileWriterReference::decBaseFileWriterReference( decBaseFileWriter *object ) :
deObjectReference( object ){
}

decBaseFileWriterReference::decBaseFileWriterReference( const decBaseFileWriterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decBaseFileWriterReference::TakeOver( decBaseFileWriter *object ){
	deObjectReference::TakeOver( object );
}

bool decBaseFileWriterReference::operator!() const{
	return deObjectReference::operator!();
}

decBaseFileWriterReference::operator bool() const{
	return deObjectReference::operator bool();
}

decBaseFileWriterReference::operator decBaseFileWriter *() const{
	return ( decBaseFileWriter* )operator deObject *();
}

decBaseFileWriterReference::operator decBaseFileWriter &() const{
	return ( decBaseFileWriter& )operator deObject &();
}

decBaseFileWriter *decBaseFileWriterReference::operator->() const{
	return ( decBaseFileWriter* )deObjectReference::operator->();
}

decBaseFileWriterReference &decBaseFileWriterReference::operator=( decBaseFileWriter *object ){
	deObjectReference::operator=( object );
	return *this;
}

decBaseFileWriterReference &decBaseFileWriterReference::operator=( const decBaseFileWriterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decBaseFileWriterReference::operator==( decBaseFileWriter *object ) const{
	return deObjectReference::operator==( object );
}

bool decBaseFileWriterReference::operator!=( decBaseFileWriter *object ) const{
	return deObjectReference::operator!=( object );
}

bool decBaseFileWriterReference::operator==( const decBaseFileWriterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decBaseFileWriterReference::operator!=( const decBaseFileWriterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
