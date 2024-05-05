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

#include "deCanvasCanvasView.h"
#include "deCanvasCanvasViewReference.h"



// Class deCanvasCanvasViewReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasCanvasViewReference::deCanvasCanvasViewReference(){
}

deCanvasCanvasViewReference::deCanvasCanvasViewReference( deCanvasCanvasView *object ) :
deObjectReference( object ){
}

deCanvasCanvasViewReference::deCanvasCanvasViewReference( const deCanvasCanvasViewReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasCanvasViewReference::TakeOver( deCanvasCanvasView *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasCanvasViewReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasCanvasViewReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasCanvasViewReference::operator deCanvasCanvasView *() const{
	return ( deCanvasCanvasView* )operator deObject *();
}

deCanvasCanvasViewReference::operator deCanvasCanvasView &() const{
	return ( deCanvasCanvasView& )operator deObject &();
}

deCanvasCanvasView *deCanvasCanvasViewReference::operator->() const{
	return ( deCanvasCanvasView* )deObjectReference::operator->();
}

deCanvasCanvasViewReference &deCanvasCanvasViewReference::operator=( deCanvasCanvasView *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasCanvasViewReference &deCanvasCanvasViewReference::operator=( const deCanvasCanvasViewReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasCanvasViewReference::operator==( deCanvasCanvasView *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasCanvasViewReference::operator!=( deCanvasCanvasView *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasCanvasViewReference::operator==( const deCanvasCanvasViewReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasCanvasViewReference::operator!=( const deCanvasCanvasViewReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
