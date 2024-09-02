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

#include "igdeGDProperty.h"
#include "igdeGDPropertyReference.h"



// Class igdeGDPropertyReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPropertyReference::igdeGDPropertyReference(){
}

igdeGDPropertyReference::igdeGDPropertyReference( igdeGDProperty *property ) :
deObjectReference( property ){
}

igdeGDPropertyReference::igdeGDPropertyReference( const igdeGDPropertyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDPropertyReference::TakeOver( igdeGDProperty *property ){
	deObjectReference::TakeOver( property );
}

bool igdeGDPropertyReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDPropertyReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDPropertyReference::operator igdeGDProperty *() const{
	return ( igdeGDProperty* )operator deObject *();
}

igdeGDPropertyReference::operator igdeGDProperty &() const{
	return ( igdeGDProperty& )operator deObject &();
}

igdeGDProperty *igdeGDPropertyReference::operator->() const{
	return ( igdeGDProperty* )deObjectReference::operator->();
}

igdeGDPropertyReference &igdeGDPropertyReference::operator=( igdeGDProperty *property ){
	deObjectReference::operator=( property );
	return *this;
}

igdeGDPropertyReference &igdeGDPropertyReference::operator=( const igdeGDPropertyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDPropertyReference::operator==( igdeGDProperty *property ) const{
	return deObjectReference::operator==( property );
}

bool igdeGDPropertyReference::operator!=( igdeGDProperty *property ) const{
	return deObjectReference::operator!=( property );
}

bool igdeGDPropertyReference::operator==( const igdeGDPropertyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDPropertyReference::operator!=( const igdeGDPropertyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
