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

#include "deCanvasPaint.h"
#include "deCanvasPaintReference.h"



// Class deCanvasPaintReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasPaintReference::deCanvasPaintReference(){
}

deCanvasPaintReference::deCanvasPaintReference( deCanvasPaint *object ) :
deObjectReference( object ){
}

deCanvasPaintReference::deCanvasPaintReference( const deCanvasPaintReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasPaintReference::TakeOver( deCanvasPaint *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasPaintReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasPaintReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasPaintReference::operator deCanvasPaint *() const{
	return ( deCanvasPaint* )operator deObject *();
}

deCanvasPaintReference::operator deCanvasPaint &() const{
	return ( deCanvasPaint& )operator deObject &();
}

deCanvasPaint *deCanvasPaintReference::operator->() const{
	return ( deCanvasPaint* )deObjectReference::operator->();
}

deCanvasPaintReference &deCanvasPaintReference::operator=( deCanvasPaint *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasPaintReference &deCanvasPaintReference::operator=( const deCanvasPaintReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasPaintReference::operator==( deCanvasPaint *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasPaintReference::operator!=( deCanvasPaint *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasPaintReference::operator==( const deCanvasPaintReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasPaintReference::operator!=( const deCanvasPaintReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
