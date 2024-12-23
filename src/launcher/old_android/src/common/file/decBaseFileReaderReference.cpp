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

#include "decBaseFileReader.h"
#include "decBaseFileReaderReference.h"



// Class decBaseFileReaderReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

decBaseFileReaderReference::decBaseFileReaderReference(){
}

decBaseFileReaderReference::decBaseFileReaderReference( decBaseFileReader *object ) :
deObjectReference( object ){
}

decBaseFileReaderReference::decBaseFileReaderReference( const decBaseFileReaderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decBaseFileReaderReference::TakeOver( decBaseFileReader *object ){
	deObjectReference::TakeOver( object );
}

bool decBaseFileReaderReference::operator!() const{
	return deObjectReference::operator!();
}

decBaseFileReaderReference::operator bool() const{
	return deObjectReference::operator bool();
}

decBaseFileReaderReference::operator decBaseFileReader *() const{
	return ( decBaseFileReader* )operator deObject *();
}

decBaseFileReaderReference::operator decBaseFileReader &() const{
	return ( decBaseFileReader& )operator deObject &();
}

decBaseFileReader *decBaseFileReaderReference::operator->() const{
	return ( decBaseFileReader* )deObjectReference::operator->();
}

decBaseFileReaderReference &decBaseFileReaderReference::operator=( decBaseFileReader *object ){
	deObjectReference::operator=( object );
	return *this;
}

decBaseFileReaderReference &decBaseFileReaderReference::operator=( const decBaseFileReaderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decBaseFileReaderReference::operator==( decBaseFileReader *object ) const{
	return deObjectReference::operator==( object );
}

bool decBaseFileReaderReference::operator!=( decBaseFileReader *object ) const{
	return deObjectReference::operator!=( object );
}

bool decBaseFileReaderReference::operator==( const decBaseFileReaderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decBaseFileReaderReference::operator!=( const decBaseFileReaderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
