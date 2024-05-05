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

#include "deVideo.h"
#include "deVideoReference.h"



// Class deVideoReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deVideoReference::deVideoReference(){
}

deVideoReference::deVideoReference( deVideo *object ) :
deObjectReference( object ){
}

deVideoReference::deVideoReference( const deVideoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVideoReference::TakeOver( deVideo *object ){
	deObjectReference::TakeOver( object );
}

bool deVideoReference::operator!() const{
	return deObjectReference::operator!();
}

deVideoReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVideoReference::operator deVideo *() const{
	return ( deVideo* )operator deObject *();
}

deVideoReference::operator deVideo &() const{
	return ( deVideo& )operator deObject &();
}

deVideo *deVideoReference::operator->() const{
	return ( deVideo* )deObjectReference::operator->();
}

deVideoReference &deVideoReference::operator=( deVideo *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVideoReference &deVideoReference::operator=( const deVideoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVideoReference::operator==( deVideo *object ) const{
	return deObjectReference::operator==( object );
}

bool deVideoReference::operator!=( deVideo *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVideoReference::operator==( const deVideoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVideoReference::operator!=( const deVideoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
