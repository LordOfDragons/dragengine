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

#include "igdeTabBook.h"
#include "igdeTabBookReference.h"



// Class igdeTabBookReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTabBookReference::igdeTabBookReference(){
}

igdeTabBookReference::igdeTabBookReference( igdeTabBook *tabbook ) :
deObjectReference( tabbook ){
}

igdeTabBookReference::igdeTabBookReference( const igdeTabBookReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTabBookReference::TakeOver( igdeTabBook *tabbook ){
	deObjectReference::TakeOver( tabbook );
}

bool igdeTabBookReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTabBookReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTabBookReference::operator igdeTabBook *() const{
	return ( igdeTabBook* )operator deObject *();
}

igdeTabBookReference::operator igdeTabBook &() const{
	return ( igdeTabBook& )operator deObject &();
}

igdeTabBook *igdeTabBookReference::operator->() const{
	return ( igdeTabBook* )deObjectReference::operator->();
}

igdeTabBookReference &igdeTabBookReference::operator=( igdeTabBook *tabbook ){
	deObjectReference::operator=( tabbook );
	return *this;
}

igdeTabBookReference &igdeTabBookReference::operator=( const igdeTabBookReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTabBookReference::operator==( igdeTabBook *tabbook ) const{
	return deObjectReference::operator==( tabbook );
}

bool igdeTabBookReference::operator!=( igdeTabBook *tabbook ) const{
	return deObjectReference::operator!=( tabbook );
}

bool igdeTabBookReference::operator==( const igdeTabBookReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTabBookReference::operator!=( const igdeTabBookReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
