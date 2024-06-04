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

#include "deCamera.h"
#include "deCameraReference.h"



// Class deCameraReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deCameraReference::deCameraReference(){
}

deCameraReference::deCameraReference( deCamera *object ) :
deObjectReference( object ){
}

deCameraReference::deCameraReference( const deCameraReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCameraReference::TakeOver( deCamera *object ){
	deObjectReference::TakeOver( object );
}

bool deCameraReference::operator!() const{
	return deObjectReference::operator!();
}

deCameraReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCameraReference::operator deCamera *() const{
	return ( deCamera* )operator deObject *();
}

deCameraReference::operator deCamera &() const{
	return ( deCamera& )operator deObject &();
}

deCamera *deCameraReference::operator->() const{
	return ( deCamera* )deObjectReference::operator->();
}

deCameraReference &deCameraReference::operator=( deCamera *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCameraReference &deCameraReference::operator=( const deCameraReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCameraReference::operator==( deCamera *object ) const{
	return deObjectReference::operator==( object );
}

bool deCameraReference::operator!=( deCamera *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCameraReference::operator==( const deCameraReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCameraReference::operator!=( const deCameraReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
