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

#include "igdeEditDVector.h"
#include "igdeEditDVectorReference.h"



// Class igdeEditDVectorReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDVectorReference::igdeEditDVectorReference(){
}

igdeEditDVectorReference::igdeEditDVectorReference( igdeEditDVector *editDVector ) :
deObjectReference( editDVector ){
}

igdeEditDVectorReference::igdeEditDVectorReference( const igdeEditDVectorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDVectorReference::TakeOver( igdeEditDVector *editDVector ){
	deObjectReference::TakeOver( editDVector );
}

bool igdeEditDVectorReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDVectorReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDVectorReference::operator igdeEditDVector *() const{
	return ( igdeEditDVector* )operator deObject *();
}

igdeEditDVectorReference::operator igdeEditDVector &() const{
	return ( igdeEditDVector& )operator deObject &();
}

igdeEditDVector *igdeEditDVectorReference::operator->() const{
	return ( igdeEditDVector* )deObjectReference::operator->();
}

igdeEditDVectorReference &igdeEditDVectorReference::operator=( igdeEditDVector *editDVector ){
	deObjectReference::operator=( editDVector );
	return *this;
}

igdeEditDVectorReference &igdeEditDVectorReference::operator=( const igdeEditDVectorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDVectorReference::operator==( igdeEditDVector *editDVector ) const{
	return deObjectReference::operator==( editDVector );
}

bool igdeEditDVectorReference::operator!=( igdeEditDVector *editDVector ) const{
	return deObjectReference::operator!=( editDVector );
}

bool igdeEditDVectorReference::operator==( const igdeEditDVectorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDVectorReference::operator!=( const igdeEditDVectorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
