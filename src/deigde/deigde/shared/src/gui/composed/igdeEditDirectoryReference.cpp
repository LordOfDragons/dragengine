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

#include "igdeEditDirectory.h"
#include "igdeEditDirectoryReference.h"



// Class igdeEditDirectoryReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDirectoryReference::igdeEditDirectoryReference(){
}

igdeEditDirectoryReference::igdeEditDirectoryReference( igdeEditDirectory *editEditDirectory ) :
deObjectReference( editEditDirectory ){
}

igdeEditDirectoryReference::igdeEditDirectoryReference( const igdeEditDirectoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDirectoryReference::TakeOver( igdeEditDirectory *editEditDirectory ){
	deObjectReference::TakeOver( editEditDirectory );
}

bool igdeEditDirectoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDirectoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDirectoryReference::operator igdeEditDirectory *() const{
	return ( igdeEditDirectory* )operator deObject *();
}

igdeEditDirectoryReference::operator igdeEditDirectory &() const{
	return ( igdeEditDirectory& )operator deObject &();
}

igdeEditDirectory *igdeEditDirectoryReference::operator->() const{
	return ( igdeEditDirectory* )deObjectReference::operator->();
}

igdeEditDirectoryReference &igdeEditDirectoryReference::operator=( igdeEditDirectory *editEditDirectory ){
	deObjectReference::operator=( editEditDirectory );
	return *this;
}

igdeEditDirectoryReference &igdeEditDirectoryReference::operator=( const igdeEditDirectoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDirectoryReference::operator==( igdeEditDirectory *editEditDirectory ) const{
	return deObjectReference::operator==( editEditDirectory );
}

bool igdeEditDirectoryReference::operator!=( igdeEditDirectory *editEditDirectory ) const{
	return deObjectReference::operator!=( editEditDirectory );
}

bool igdeEditDirectoryReference::operator==( const igdeEditDirectoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDirectoryReference::operator!=( const igdeEditDirectoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
