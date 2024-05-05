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

#include "deModel.h"
#include "deModelReference.h"



// Class deModelReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deModelReference::deModelReference(){
}

deModelReference::deModelReference( deModel *object ) :
deObjectReference( object ){
}

deModelReference::deModelReference( const deModelReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deModelReference::TakeOver( deModel *object ){
	deObjectReference::TakeOver( object );
}

bool deModelReference::operator!() const{
	return deObjectReference::operator!();
}

deModelReference::operator bool() const{
	return deObjectReference::operator bool();
}

deModelReference::operator deModel *() const{
	return ( deModel* )operator deObject *();
}

deModelReference::operator deModel &() const{
	return ( deModel& )operator deObject &();
}

deModel *deModelReference::operator->() const{
	return ( deModel* )deObjectReference::operator->();
}

deModelReference &deModelReference::operator=( deModel *object ){
	deObjectReference::operator=( object );
	return *this;
}

deModelReference &deModelReference::operator=( const deModelReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deModelReference::operator==( deModel *object ) const{
	return deObjectReference::operator==( object );
}

bool deModelReference::operator!=( deModel *object ) const{
	return deObjectReference::operator!=( object );
}

bool deModelReference::operator==( const deModelReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deModelReference::operator!=( const deModelReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
