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

#include "deSynthesizerInstance.h"
#include "deSynthesizerInstanceReference.h"



// Class deSynthesizerInstanceReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerInstanceReference::deSynthesizerInstanceReference(){
}

deSynthesizerInstanceReference::deSynthesizerInstanceReference( deSynthesizerInstance *object ) :
deObjectReference( object ){
}

deSynthesizerInstanceReference::deSynthesizerInstanceReference( const deSynthesizerInstanceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSynthesizerInstanceReference::TakeOver( deSynthesizerInstance *object ){
	deObjectReference::TakeOver( object );
}

bool deSynthesizerInstanceReference::operator!() const{
	return deObjectReference::operator!();
}

deSynthesizerInstanceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSynthesizerInstanceReference::operator deSynthesizerInstance *() const{
	return ( deSynthesizerInstance* )operator deObject *();
}

deSynthesizerInstanceReference::operator deSynthesizerInstance &() const{
	return ( deSynthesizerInstance& )operator deObject &();
}

deSynthesizerInstance *deSynthesizerInstanceReference::operator->() const{
	return ( deSynthesizerInstance* )deObjectReference::operator->();
}

deSynthesizerInstanceReference &deSynthesizerInstanceReference::operator=( deSynthesizerInstance *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSynthesizerInstanceReference &deSynthesizerInstanceReference::operator=( const deSynthesizerInstanceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSynthesizerInstanceReference::operator==( deSynthesizerInstance *object ) const{
	return deObjectReference::operator==( object );
}

bool deSynthesizerInstanceReference::operator!=( deSynthesizerInstance *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSynthesizerInstanceReference::operator==( const deSynthesizerInstanceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSynthesizerInstanceReference::operator!=( const deSynthesizerInstanceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
