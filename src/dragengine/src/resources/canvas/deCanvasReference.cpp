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

#include "deCanvas.h"
#include "deCanvasReference.h"



// Class deCanvasReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasReference::deCanvasReference(){
}

deCanvasReference::deCanvasReference( deCanvas *object ) :
deObjectReference( object ){
}

deCanvasReference::deCanvasReference( const deCanvasReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasReference::TakeOver( deCanvas *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasReference::operator deCanvas *() const{
	return ( deCanvas* )operator deObject *();
}

deCanvasReference::operator deCanvas &() const{
	return ( deCanvas& )operator deObject &();
}

deCanvas *deCanvasReference::operator->() const{
	return ( deCanvas* )deObjectReference::operator->();
}

deCanvasReference &deCanvasReference::operator=( deCanvas *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasReference &deCanvasReference::operator=( const deCanvasReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasReference::operator==( deCanvas *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasReference::operator!=( deCanvas *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasReference::operator==( const deCanvasReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasReference::operator!=( const deCanvasReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
