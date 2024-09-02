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

#include "igdeViewRenderWindow.h"
#include "igdeViewRenderWindowReference.h"



// Class igdeViewRenderWindowReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewRenderWindowReference::igdeViewRenderWindowReference(){
}

igdeViewRenderWindowReference::igdeViewRenderWindowReference( igdeViewRenderWindow *view ) :
deObjectReference( view ){
}

igdeViewRenderWindowReference::igdeViewRenderWindowReference( const igdeViewRenderWindowReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeViewRenderWindowReference::TakeOver( igdeViewRenderWindow *view ){
	deObjectReference::TakeOver( view );
}

bool igdeViewRenderWindowReference::operator!() const{
	return deObjectReference::operator!();
}

igdeViewRenderWindowReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeViewRenderWindowReference::operator igdeViewRenderWindow *() const{
	return ( igdeViewRenderWindow* )operator deObject *();
}

igdeViewRenderWindowReference::operator igdeViewRenderWindow &() const{
	return ( igdeViewRenderWindow& )operator deObject &();
}

igdeViewRenderWindow *igdeViewRenderWindowReference::operator->() const{
	return ( igdeViewRenderWindow* )deObjectReference::operator->();
}

igdeViewRenderWindowReference &igdeViewRenderWindowReference::operator=( igdeViewRenderWindow *view ){
	deObjectReference::operator=( view );
	return *this;
}

igdeViewRenderWindowReference &igdeViewRenderWindowReference::operator=( const igdeViewRenderWindowReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeViewRenderWindowReference::operator==( igdeViewRenderWindow *view ) const{
	return deObjectReference::operator==( view );
}

bool igdeViewRenderWindowReference::operator!=( igdeViewRenderWindow *view ) const{
	return deObjectReference::operator!=( view );
}

bool igdeViewRenderWindowReference::operator==( const igdeViewRenderWindowReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeViewRenderWindowReference::operator!=( const igdeViewRenderWindowReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
