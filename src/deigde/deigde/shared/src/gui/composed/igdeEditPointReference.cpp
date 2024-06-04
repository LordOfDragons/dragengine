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

#include "igdeEditPoint.h"
#include "igdeEditPointReference.h"



// Class igdeEditPointReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPointReference::igdeEditPointReference(){
}

igdeEditPointReference::igdeEditPointReference( igdeEditPoint *editPoint ) :
deObjectReference( editPoint ){
}

igdeEditPointReference::igdeEditPointReference( const igdeEditPointReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPointReference::TakeOver( igdeEditPoint *editPoint ){
	deObjectReference::TakeOver( editPoint );
}

bool igdeEditPointReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPointReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPointReference::operator igdeEditPoint *() const{
	return ( igdeEditPoint* )operator deObject *();
}

igdeEditPointReference::operator igdeEditPoint &() const{
	return ( igdeEditPoint& )operator deObject &();
}

igdeEditPoint *igdeEditPointReference::operator->() const{
	return ( igdeEditPoint* )deObjectReference::operator->();
}

igdeEditPointReference &igdeEditPointReference::operator=( igdeEditPoint *editPoint ){
	deObjectReference::operator=( editPoint );
	return *this;
}

igdeEditPointReference &igdeEditPointReference::operator=( const igdeEditPointReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPointReference::operator==( igdeEditPoint *editPoint ) const{
	return deObjectReference::operator==( editPoint );
}

bool igdeEditPointReference::operator!=( igdeEditPoint *editPoint ) const{
	return deObjectReference::operator!=( editPoint );
}

bool igdeEditPointReference::operator==( const igdeEditPointReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPointReference::operator!=( const igdeEditPointReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
