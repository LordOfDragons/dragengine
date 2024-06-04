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

#include "igdeEditVector2Listener.h"
#include "igdeEditVector2ListenerReference.h"



// Class igdeEditVector2ListenerReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVector2ListenerReference::igdeEditVector2ListenerReference(){
}

igdeEditVector2ListenerReference::igdeEditVector2ListenerReference( igdeEditVector2Listener *listener ) :
deObjectReference( listener ){
}

igdeEditVector2ListenerReference::igdeEditVector2ListenerReference( const igdeEditVector2ListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVector2ListenerReference::TakeOver( igdeEditVector2Listener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditVector2ListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVector2ListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVector2ListenerReference::operator igdeEditVector2Listener *() const{
	return ( igdeEditVector2Listener* )operator deObject *();
}

igdeEditVector2ListenerReference::operator igdeEditVector2Listener &() const{
	return ( igdeEditVector2Listener& )operator deObject &();
}

igdeEditVector2Listener *igdeEditVector2ListenerReference::operator->() const{
	return ( igdeEditVector2Listener* )deObjectReference::operator->();
}

igdeEditVector2ListenerReference &igdeEditVector2ListenerReference::operator=( igdeEditVector2Listener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditVector2ListenerReference &igdeEditVector2ListenerReference::operator=( const igdeEditVector2ListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVector2ListenerReference::operator==( igdeEditVector2Listener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditVector2ListenerReference::operator!=( igdeEditVector2Listener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditVector2ListenerReference::operator==( const igdeEditVector2ListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVector2ListenerReference::operator!=( const igdeEditVector2ListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
