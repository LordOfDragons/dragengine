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

#include "decMemoryFile.h"
#include "decMemoryFileReference.h"



// Class decMemoryFileReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

decMemoryFileReference::decMemoryFileReference(){
}

decMemoryFileReference::decMemoryFileReference( decMemoryFile *object ) :
deObjectReference( object ){
}

decMemoryFileReference::decMemoryFileReference( const decMemoryFileReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decMemoryFileReference::TakeOver( decMemoryFile *object ){
	deObjectReference::TakeOver( object );
}

bool decMemoryFileReference::operator!() const{
	return deObjectReference::operator!();
}

decMemoryFileReference::operator bool() const{
	return deObjectReference::operator bool();
}

decMemoryFileReference::operator decMemoryFile *() const{
	return ( decMemoryFile* )operator deObject *();
}

decMemoryFileReference::operator decMemoryFile &() const{
	return ( decMemoryFile& )operator deObject &();
}

decMemoryFile *decMemoryFileReference::operator->() const{
	return ( decMemoryFile* )deObjectReference::operator->();
}

decMemoryFileReference &decMemoryFileReference::operator=( decMemoryFile *object ){
	deObjectReference::operator=( object );
	return *this;
}

decMemoryFileReference &decMemoryFileReference::operator=( const decMemoryFileReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decMemoryFileReference::operator==( decMemoryFile *object ) const{
	return deObjectReference::operator==( object );
}

bool decMemoryFileReference::operator!=( decMemoryFile *object ) const{
	return deObjectReference::operator!=( object );
}

bool decMemoryFileReference::operator==( const decMemoryFileReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decMemoryFileReference::operator!=( const decMemoryFileReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
