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
