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

#include "igdeMouseKeyListener.h"
#include "igdeMouseKeyListenerReference.h"



// Class igdeMouseKeyListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseKeyListenerReference::igdeMouseKeyListenerReference(){
}

igdeMouseKeyListenerReference::igdeMouseKeyListenerReference( igdeMouseKeyListener *listener ) :
deObjectReference( listener ){
}

igdeMouseKeyListenerReference::igdeMouseKeyListenerReference( const igdeMouseKeyListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMouseKeyListenerReference::TakeOver( igdeMouseKeyListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeMouseKeyListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMouseKeyListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMouseKeyListenerReference::operator igdeMouseKeyListener *() const{
	return ( igdeMouseKeyListener* )operator deObject *();
}

igdeMouseKeyListenerReference::operator igdeMouseKeyListener &() const{
	return ( igdeMouseKeyListener& )operator deObject &();
}

igdeMouseKeyListener *igdeMouseKeyListenerReference::operator->() const{
	return ( igdeMouseKeyListener* )deObjectReference::operator->();
}

igdeMouseKeyListenerReference &igdeMouseKeyListenerReference::operator=( igdeMouseKeyListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeMouseKeyListenerReference &igdeMouseKeyListenerReference::operator=( const igdeMouseKeyListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMouseKeyListenerReference::operator==( igdeMouseKeyListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeMouseKeyListenerReference::operator!=( igdeMouseKeyListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeMouseKeyListenerReference::operator==( const igdeMouseKeyListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMouseKeyListenerReference::operator!=( const igdeMouseKeyListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
