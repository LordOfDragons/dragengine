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

#include "deSkyInstance.h"
#include "deSkyInstanceReference.h"



// Class deSkyInstanceReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkyInstanceReference::deSkyInstanceReference(){
}

deSkyInstanceReference::deSkyInstanceReference( deSkyInstance *object ) :
deObjectReference( object ){
}

deSkyInstanceReference::deSkyInstanceReference( const deSkyInstanceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSkyInstanceReference::TakeOver( deSkyInstance *object ){
	deObjectReference::TakeOver( object );
}

bool deSkyInstanceReference::operator!() const{
	return deObjectReference::operator!();
}

deSkyInstanceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSkyInstanceReference::operator deSkyInstance *() const{
	return ( deSkyInstance* )operator deObject *();
}

deSkyInstanceReference::operator deSkyInstance &() const{
	return ( deSkyInstance& )operator deObject &();
}

deSkyInstance *deSkyInstanceReference::operator->() const{
	return ( deSkyInstance* )deObjectReference::operator->();
}

deSkyInstanceReference &deSkyInstanceReference::operator=( deSkyInstance *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSkyInstanceReference &deSkyInstanceReference::operator=( const deSkyInstanceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSkyInstanceReference::operator==( deSkyInstance *object ) const{
	return deObjectReference::operator==( object );
}

bool deSkyInstanceReference::operator!=( deSkyInstance *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSkyInstanceReference::operator==( const deSkyInstanceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSkyInstanceReference::operator!=( const deSkyInstanceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
