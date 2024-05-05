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

#include "deCanvasView.h"
#include "deCanvasViewReference.h"



// Class deCanvasViewReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasViewReference::deCanvasViewReference(){
}

deCanvasViewReference::deCanvasViewReference( deCanvasView *object ) :
deObjectReference( object ){
}

deCanvasViewReference::deCanvasViewReference( const deCanvasViewReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasViewReference::TakeOver( deCanvasView *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasViewReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasViewReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasViewReference::operator deCanvasView *() const{
	return ( deCanvasView* )operator deObject *();
}

deCanvasViewReference::operator deCanvasView &() const{
	return ( deCanvasView& )operator deObject &();
}

deCanvasView *deCanvasViewReference::operator->() const{
	return ( deCanvasView* )deObjectReference::operator->();
}

deCanvasViewReference &deCanvasViewReference::operator=( deCanvasView *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasViewReference &deCanvasViewReference::operator=( const deCanvasViewReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasViewReference::operator==( deCanvasView *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasViewReference::operator!=( deCanvasView *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasViewReference::operator==( const deCanvasViewReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasViewReference::operator!=( const deCanvasViewReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
