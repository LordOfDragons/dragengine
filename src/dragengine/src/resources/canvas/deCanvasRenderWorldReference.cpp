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

#include "deCanvasRenderWorld.h"
#include "deCanvasRenderWorldReference.h"



// Class deCanvasRenderWorldReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasRenderWorldReference::deCanvasRenderWorldReference(){
}

deCanvasRenderWorldReference::deCanvasRenderWorldReference( deCanvasRenderWorld *object ) :
deObjectReference( object ){
}

deCanvasRenderWorldReference::deCanvasRenderWorldReference( const deCanvasRenderWorldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasRenderWorldReference::TakeOver( deCanvasRenderWorld *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasRenderWorldReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasRenderWorldReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasRenderWorldReference::operator deCanvasRenderWorld *() const{
	return ( deCanvasRenderWorld* )operator deObject *();
}

deCanvasRenderWorldReference::operator deCanvasRenderWorld &() const{
	return ( deCanvasRenderWorld& )operator deObject &();
}

deCanvasRenderWorld *deCanvasRenderWorldReference::operator->() const{
	return ( deCanvasRenderWorld* )deObjectReference::operator->();
}

deCanvasRenderWorldReference &deCanvasRenderWorldReference::operator=( deCanvasRenderWorld *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasRenderWorldReference &deCanvasRenderWorldReference::operator=( const deCanvasRenderWorldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasRenderWorldReference::operator==( deCanvasRenderWorld *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasRenderWorldReference::operator!=( deCanvasRenderWorld *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasRenderWorldReference::operator==( const deCanvasRenderWorldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasRenderWorldReference::operator!=( const deCanvasRenderWorldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
