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

#include "igdeScrollBar.h"
#include "igdeScrollBarReference.h"



// Class igdeScrollBarReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeScrollBarReference::igdeScrollBarReference(){
}

igdeScrollBarReference::igdeScrollBarReference( igdeScrollBar *scrollbar ) :
deObjectReference( scrollbar ){
}

igdeScrollBarReference::igdeScrollBarReference( const igdeScrollBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeScrollBarReference::TakeOver( igdeScrollBar *scrollbar ){
	deObjectReference::TakeOver( scrollbar );
}

bool igdeScrollBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeScrollBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeScrollBarReference::operator igdeScrollBar *() const{
	return ( igdeScrollBar* )operator deObject *();
}

igdeScrollBarReference::operator igdeScrollBar &() const{
	return ( igdeScrollBar& )operator deObject &();
}

igdeScrollBar *igdeScrollBarReference::operator->() const{
	return ( igdeScrollBar* )deObjectReference::operator->();
}

igdeScrollBarReference &igdeScrollBarReference::operator=( igdeScrollBar *scrollbar ){
	deObjectReference::operator=( scrollbar );
	return *this;
}

igdeScrollBarReference &igdeScrollBarReference::operator=( const igdeScrollBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeScrollBarReference::operator==( igdeScrollBar *scrollbar ) const{
	return deObjectReference::operator==( scrollbar );
}

bool igdeScrollBarReference::operator!=( igdeScrollBar *scrollbar ) const{
	return deObjectReference::operator!=( scrollbar );
}

bool igdeScrollBarReference::operator==( const igdeScrollBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeScrollBarReference::operator!=( const igdeScrollBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
