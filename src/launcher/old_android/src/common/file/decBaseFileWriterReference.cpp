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

#include "decBaseFileWriter.h"
#include "decBaseFileWriterReference.h"



// Class decBaseFileWriterReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

decBaseFileWriterReference::decBaseFileWriterReference(){
}

decBaseFileWriterReference::decBaseFileWriterReference( decBaseFileWriter *object ) :
deObjectReference( object ){
}

decBaseFileWriterReference::decBaseFileWriterReference( const decBaseFileWriterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void decBaseFileWriterReference::TakeOver( decBaseFileWriter *object ){
	deObjectReference::TakeOver( object );
}

bool decBaseFileWriterReference::operator!() const{
	return deObjectReference::operator!();
}

decBaseFileWriterReference::operator bool() const{
	return deObjectReference::operator bool();
}

decBaseFileWriterReference::operator decBaseFileWriter *() const{
	return ( decBaseFileWriter* )operator deObject *();
}

decBaseFileWriterReference::operator decBaseFileWriter &() const{
	return ( decBaseFileWriter& )operator deObject &();
}

decBaseFileWriter *decBaseFileWriterReference::operator->() const{
	return ( decBaseFileWriter* )deObjectReference::operator->();
}

decBaseFileWriterReference &decBaseFileWriterReference::operator=( decBaseFileWriter *object ){
	deObjectReference::operator=( object );
	return *this;
}

decBaseFileWriterReference &decBaseFileWriterReference::operator=( const decBaseFileWriterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool decBaseFileWriterReference::operator==( decBaseFileWriter *object ) const{
	return deObjectReference::operator==( object );
}

bool decBaseFileWriterReference::operator!=( decBaseFileWriter *object ) const{
	return deObjectReference::operator!=( object );
}

bool decBaseFileWriterReference::operator==( const decBaseFileWriterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool decBaseFileWriterReference::operator!=( const decBaseFileWriterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
