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

#include "decXmlDocument.h"
#include "decXmlDocumentReference.h"



// Class decXmlDocumentReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

decXmlDocumentReference::decXmlDocumentReference(){
}

decXmlDocumentReference::decXmlDocumentReference( decXmlDocument *object ) :
deObjectReference( object ){
}

decXmlDocumentReference::decXmlDocumentReference( const decXmlDocumentReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decXmlDocumentReference::TakeOver( decXmlDocument *object ){
	deObjectReference::TakeOver( object );
}

bool decXmlDocumentReference::operator!() const{
	return deObjectReference::operator!();
}

decXmlDocumentReference::operator bool() const{
	return deObjectReference::operator bool();
}

decXmlDocumentReference::operator decXmlDocument *() const{
	return ( decXmlDocument* )operator deObject *();
}

decXmlDocumentReference::operator decXmlDocument &() const{
	return ( decXmlDocument& )operator deObject &();
}

decXmlDocument *decXmlDocumentReference::operator->() const{
	return ( decXmlDocument* )deObjectReference::operator->();
}

decXmlDocumentReference &decXmlDocumentReference::operator=( decXmlDocument *object ){
	deObjectReference::operator=( object );
	return *this;
}

decXmlDocumentReference &decXmlDocumentReference::operator=( const decXmlDocumentReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decXmlDocumentReference::operator==( decXmlDocument *object ) const{
	return deObjectReference::operator==( object );
}

bool decXmlDocumentReference::operator!=( decXmlDocument *object ) const{
	return deObjectReference::operator!=( object );
}

bool decXmlDocumentReference::operator==( const decXmlDocumentReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decXmlDocumentReference::operator!=( const decXmlDocumentReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
