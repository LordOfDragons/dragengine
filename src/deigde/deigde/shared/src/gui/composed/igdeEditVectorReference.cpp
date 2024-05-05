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

#include "igdeEditVector.h"
#include "igdeEditVectorReference.h"



// Class igdeEditVectorReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVectorReference::igdeEditVectorReference(){
}

igdeEditVectorReference::igdeEditVectorReference( igdeEditVector *editVector ) :
deObjectReference( editVector ){
}

igdeEditVectorReference::igdeEditVectorReference( const igdeEditVectorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVectorReference::TakeOver( igdeEditVector *editVector ){
	deObjectReference::TakeOver( editVector );
}

bool igdeEditVectorReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVectorReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVectorReference::operator igdeEditVector *() const{
	return ( igdeEditVector* )operator deObject *();
}

igdeEditVectorReference::operator igdeEditVector &() const{
	return ( igdeEditVector& )operator deObject &();
}

igdeEditVector *igdeEditVectorReference::operator->() const{
	return ( igdeEditVector* )deObjectReference::operator->();
}

igdeEditVectorReference &igdeEditVectorReference::operator=( igdeEditVector *editVector ){
	deObjectReference::operator=( editVector );
	return *this;
}

igdeEditVectorReference &igdeEditVectorReference::operator=( const igdeEditVectorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVectorReference::operator==( igdeEditVector *editVector ) const{
	return deObjectReference::operator==( editVector );
}

bool igdeEditVectorReference::operator!=( igdeEditVector *editVector ) const{
	return deObjectReference::operator!=( editVector );
}

bool igdeEditVectorReference::operator==( const igdeEditVectorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVectorReference::operator!=( const igdeEditVectorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
