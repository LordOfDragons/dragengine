/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
