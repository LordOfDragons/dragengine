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

#include "igdeListHeader.h"
#include "igdeListHeaderReference.h"



// Class igdeListHeaderReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListHeaderReference::igdeListHeaderReference(){
}

igdeListHeaderReference::igdeListHeaderReference( igdeListHeader *header ) :
deObjectReference( header ){
}

igdeListHeaderReference::igdeListHeaderReference( const igdeListHeaderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListHeaderReference::TakeOver( igdeListHeader *header ){
	deObjectReference::TakeOver( header );
}

bool igdeListHeaderReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListHeaderReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListHeaderReference::operator igdeListHeader *() const{
	return ( igdeListHeader* )operator deObject *();
}

igdeListHeaderReference::operator igdeListHeader &() const{
	return ( igdeListHeader& )operator deObject &();
}

igdeListHeader *igdeListHeaderReference::operator->() const{
	return ( igdeListHeader* )deObjectReference::operator->();
}

igdeListHeaderReference &igdeListHeaderReference::operator=( igdeListHeader *header ){
	deObjectReference::operator=( header );
	return *this;
}

igdeListHeaderReference &igdeListHeaderReference::operator=( const igdeListHeaderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListHeaderReference::operator==( igdeListHeader *header ) const{
	return deObjectReference::operator==( header );
}

bool igdeListHeaderReference::operator!=( igdeListHeader *header ) const{
	return deObjectReference::operator!=( header );
}

bool igdeListHeaderReference::operator==( const igdeListHeaderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListHeaderReference::operator!=( const igdeListHeaderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
