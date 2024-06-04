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

#include "igdeScrollBarListener.h"
#include "igdeScrollBarListenerReference.h"



// Class igdeScrollBarListenerReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeScrollBarListenerReference::igdeScrollBarListenerReference(){
}

igdeScrollBarListenerReference::igdeScrollBarListenerReference( igdeScrollBarListener *listener ) :
deObjectReference( listener ){
}

igdeScrollBarListenerReference::igdeScrollBarListenerReference( const igdeScrollBarListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeScrollBarListenerReference::TakeOver( igdeScrollBarListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeScrollBarListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeScrollBarListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeScrollBarListenerReference::operator igdeScrollBarListener *() const{
	return ( igdeScrollBarListener* )operator deObject *();
}

igdeScrollBarListenerReference::operator igdeScrollBarListener &() const{
	return ( igdeScrollBarListener& )operator deObject &();
}

igdeScrollBarListener *igdeScrollBarListenerReference::operator->() const{
	return ( igdeScrollBarListener* )deObjectReference::operator->();
}

igdeScrollBarListenerReference &igdeScrollBarListenerReference::operator=( igdeScrollBarListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeScrollBarListenerReference &igdeScrollBarListenerReference::operator=( const igdeScrollBarListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeScrollBarListenerReference::operator==( igdeScrollBarListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeScrollBarListenerReference::operator!=( igdeScrollBarListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeScrollBarListenerReference::operator==( const igdeScrollBarListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeScrollBarListenerReference::operator!=( const igdeScrollBarListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
