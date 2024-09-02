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

#include "igdeEditPath.h"
#include "igdeEditPathReference.h"



// Class igdeEditPathReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPathReference::igdeEditPathReference(){
}

igdeEditPathReference::igdeEditPathReference( igdeEditPath *editEditPath ) :
deObjectReference( editEditPath ){
}

igdeEditPathReference::igdeEditPathReference( const igdeEditPathReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPathReference::TakeOver( igdeEditPath *editEditPath ){
	deObjectReference::TakeOver( editEditPath );
}

bool igdeEditPathReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPathReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPathReference::operator igdeEditPath *() const{
	return ( igdeEditPath* )operator deObject *();
}

igdeEditPathReference::operator igdeEditPath &() const{
	return ( igdeEditPath& )operator deObject &();
}

igdeEditPath *igdeEditPathReference::operator->() const{
	return ( igdeEditPath* )deObjectReference::operator->();
}

igdeEditPathReference &igdeEditPathReference::operator=( igdeEditPath *editEditPath ){
	deObjectReference::operator=( editEditPath );
	return *this;
}

igdeEditPathReference &igdeEditPathReference::operator=( const igdeEditPathReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPathReference::operator==( igdeEditPath *editEditPath ) const{
	return deObjectReference::operator==( editEditPath );
}

bool igdeEditPathReference::operator!=( igdeEditPath *editEditPath ) const{
	return deObjectReference::operator!=( editEditPath );
}

bool igdeEditPathReference::operator==( const igdeEditPathReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPathReference::operator!=( const igdeEditPathReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
