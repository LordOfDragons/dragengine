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

#include "igdeEditPoint3Listener.h"
#include "igdeEditPoint3ListenerReference.h"



// Class igdeEditPoint3ListenerReference
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPoint3ListenerReference::igdeEditPoint3ListenerReference(){
}

igdeEditPoint3ListenerReference::igdeEditPoint3ListenerReference( igdeEditPoint3Listener *listener ) :
deObjectReference( listener ){
}

igdeEditPoint3ListenerReference::igdeEditPoint3ListenerReference( const igdeEditPoint3ListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPoint3ListenerReference::TakeOver( igdeEditPoint3Listener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditPoint3ListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPoint3ListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPoint3ListenerReference::operator igdeEditPoint3Listener *() const{
	return ( igdeEditPoint3Listener* )operator deObject *();
}

igdeEditPoint3ListenerReference::operator igdeEditPoint3Listener &() const{
	return ( igdeEditPoint3Listener& )operator deObject &();
}

igdeEditPoint3Listener *igdeEditPoint3ListenerReference::operator->() const{
	return ( igdeEditPoint3Listener* )deObjectReference::operator->();
}

igdeEditPoint3ListenerReference &igdeEditPoint3ListenerReference::operator=( igdeEditPoint3Listener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditPoint3ListenerReference &igdeEditPoint3ListenerReference::operator=( const igdeEditPoint3ListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPoint3ListenerReference::operator==( igdeEditPoint3Listener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditPoint3ListenerReference::operator!=( igdeEditPoint3Listener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditPoint3ListenerReference::operator==( const igdeEditPoint3ListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPoint3ListenerReference::operator!=( const igdeEditPoint3ListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
