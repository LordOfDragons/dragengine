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

#include "deCanvasImage.h"
#include "deCanvasImageReference.h"



// Class deCanvasImageReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasImageReference::deCanvasImageReference(){
}

deCanvasImageReference::deCanvasImageReference( deCanvasImage *object ) :
deObjectReference( object ){
}

deCanvasImageReference::deCanvasImageReference( const deCanvasImageReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasImageReference::TakeOver( deCanvasImage *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasImageReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasImageReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasImageReference::operator deCanvasImage *() const{
	return ( deCanvasImage* )operator deObject *();
}

deCanvasImageReference::operator deCanvasImage &() const{
	return ( deCanvasImage& )operator deObject &();
}

deCanvasImage *deCanvasImageReference::operator->() const{
	return ( deCanvasImage* )deObjectReference::operator->();
}

deCanvasImageReference &deCanvasImageReference::operator=( deCanvasImage *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasImageReference &deCanvasImageReference::operator=( const deCanvasImageReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasImageReference::operator==( deCanvasImage *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasImageReference::operator!=( deCanvasImage *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasImageReference::operator==( const deCanvasImageReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasImageReference::operator!=( const deCanvasImageReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
