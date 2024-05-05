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

#include "deCanvasText.h"
#include "deCanvasTextReference.h"



// Class deCanvasTextReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasTextReference::deCanvasTextReference(){
}

deCanvasTextReference::deCanvasTextReference( deCanvasText *object ) :
deObjectReference( object ){
}

deCanvasTextReference::deCanvasTextReference( const deCanvasTextReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasTextReference::TakeOver( deCanvasText *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasTextReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasTextReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasTextReference::operator deCanvasText *() const{
	return ( deCanvasText* )operator deObject *();
}

deCanvasTextReference::operator deCanvasText &() const{
	return ( deCanvasText& )operator deObject &();
}

deCanvasText *deCanvasTextReference::operator->() const{
	return ( deCanvasText* )deObjectReference::operator->();
}

deCanvasTextReference &deCanvasTextReference::operator=( deCanvasText *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasTextReference &deCanvasTextReference::operator=( const deCanvasTextReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasTextReference::operator==( deCanvasText *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasTextReference::operator!=( deCanvasText *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasTextReference::operator==( const deCanvasTextReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasTextReference::operator!=( const deCanvasTextReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
