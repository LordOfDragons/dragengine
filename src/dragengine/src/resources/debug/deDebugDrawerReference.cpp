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

#include "deDebugDrawer.h"
#include "deDebugDrawerReference.h"



// Class deDebugDrawerReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deDebugDrawerReference::deDebugDrawerReference(){
}

deDebugDrawerReference::deDebugDrawerReference( deDebugDrawer *object ) :
deObjectReference( object ){
}

deDebugDrawerReference::deDebugDrawerReference( const deDebugDrawerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deDebugDrawerReference::TakeOver( deDebugDrawer *object ){
	deObjectReference::TakeOver( object );
}

bool deDebugDrawerReference::operator!() const{
	return deObjectReference::operator!();
}

deDebugDrawerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deDebugDrawerReference::operator deDebugDrawer *() const{
	return ( deDebugDrawer* )operator deObject *();
}

deDebugDrawerReference::operator deDebugDrawer &() const{
	return ( deDebugDrawer& )operator deObject &();
}

deDebugDrawer *deDebugDrawerReference::operator->() const{
	return ( deDebugDrawer* )deObjectReference::operator->();
}

deDebugDrawerReference &deDebugDrawerReference::operator=( deDebugDrawer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deDebugDrawerReference &deDebugDrawerReference::operator=( const deDebugDrawerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deDebugDrawerReference::operator==( deDebugDrawer *object ) const{
	return deObjectReference::operator==( object );
}

bool deDebugDrawerReference::operator!=( deDebugDrawer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deDebugDrawerReference::operator==( const deDebugDrawerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deDebugDrawerReference::operator!=( const deDebugDrawerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
