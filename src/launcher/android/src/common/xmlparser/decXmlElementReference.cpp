/* 
 * Drag[en]gine Android Launcher
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

#include "decXmlElement.h"
#include "decXmlElementReference.h"



// Class decXmlElementReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

decXmlElementReference::decXmlElementReference(){
}

decXmlElementReference::decXmlElementReference( decXmlElement *object ) :
deObjectReference( object ){
}

decXmlElementReference::decXmlElementReference( const decXmlElementReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decXmlElementReference::TakeOver( decXmlElement *object ){
	deObjectReference::TakeOver( object );
}

bool decXmlElementReference::operator!() const{
	return deObjectReference::operator!();
}

decXmlElementReference::operator bool() const{
	return deObjectReference::operator bool();
}

decXmlElementReference::operator decXmlElement *() const{
	return ( decXmlElement* )operator deObject *();
}

decXmlElementReference::operator decXmlElement &() const{
	return ( decXmlElement& )operator deObject &();
}

decXmlElement *decXmlElementReference::operator->() const{
	return ( decXmlElement* )deObjectReference::operator->();
}

decXmlElementReference &decXmlElementReference::operator=( decXmlElement *object ){
	deObjectReference::operator=( object );
	return *this;
}

decXmlElementReference &decXmlElementReference::operator=( const decXmlElementReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decXmlElementReference::operator==( decXmlElement *object ) const{
	return deObjectReference::operator==( object );
}

bool decXmlElementReference::operator!=( decXmlElement *object ) const{
	return deObjectReference::operator!=( object );
}

bool decXmlElementReference::operator==( const decXmlElementReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decXmlElementReference::operator!=( const decXmlElementReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
