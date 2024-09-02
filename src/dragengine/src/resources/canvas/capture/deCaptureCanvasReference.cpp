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

#include "deCaptureCanvas.h"
#include "deCaptureCanvasReference.h"



// Class deCaptureCanvasReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deCaptureCanvasReference::deCaptureCanvasReference(){
}

deCaptureCanvasReference::deCaptureCanvasReference( deCaptureCanvas *object ) :
deObjectReference( object ){
}

deCaptureCanvasReference::deCaptureCanvasReference( const deCaptureCanvasReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCaptureCanvasReference::TakeOver( deCaptureCanvas *object ){
	deObjectReference::TakeOver( object );
}

bool deCaptureCanvasReference::operator!() const{
	return deObjectReference::operator!();
}

deCaptureCanvasReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCaptureCanvasReference::operator deCaptureCanvas *() const{
	return ( deCaptureCanvas* )operator deObject *();
}

deCaptureCanvasReference::operator deCaptureCanvas &() const{
	return ( deCaptureCanvas& )operator deObject &();
}

deCaptureCanvas *deCaptureCanvasReference::operator->() const{
	return ( deCaptureCanvas* )deObjectReference::operator->();
}

deCaptureCanvasReference &deCaptureCanvasReference::operator=( deCaptureCanvas *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCaptureCanvasReference &deCaptureCanvasReference::operator=( const deCaptureCanvasReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCaptureCanvasReference::operator==( deCaptureCanvas *object ) const{
	return deObjectReference::operator==( object );
}

bool deCaptureCanvasReference::operator!=( deCaptureCanvas *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCaptureCanvasReference::operator==( const deCaptureCanvasReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCaptureCanvasReference::operator!=( const deCaptureCanvasReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
