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

#include "deServer.h"
#include "deServerReference.h"



// Class deServerReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deServerReference::deServerReference(){
}

deServerReference::deServerReference( deServer *object ) :
deObjectReference( object ){
}

deServerReference::deServerReference( const deServerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deServerReference::TakeOver( deServer *object ){
	deObjectReference::TakeOver( object );
}

bool deServerReference::operator!() const{
	return deObjectReference::operator!();
}

deServerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deServerReference::operator deServer *() const{
	return ( deServer* )operator deObject *();
}

deServerReference::operator deServer &() const{
	return ( deServer& )operator deObject &();
}

deServer *deServerReference::operator->() const{
	return ( deServer* )deObjectReference::operator->();
}

deServerReference &deServerReference::operator=( deServer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deServerReference &deServerReference::operator=( const deServerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deServerReference::operator==( deServer *object ) const{
	return deObjectReference::operator==( object );
}

bool deServerReference::operator!=( deServer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deServerReference::operator==( const deServerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deServerReference::operator!=( const deServerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
