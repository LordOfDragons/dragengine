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

#include "igdeNVNodeListener.h"
#include "igdeNVNodeListenerReference.h"



// Class igdeNVNodeListenerReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVNodeListenerReference::igdeNVNodeListenerReference(){
}

igdeNVNodeListenerReference::igdeNVNodeListenerReference( igdeNVNodeListener *listener ) :
deObjectReference( listener ){
}

igdeNVNodeListenerReference::igdeNVNodeListenerReference( const igdeNVNodeListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVNodeListenerReference::TakeOver( igdeNVNodeListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeNVNodeListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVNodeListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVNodeListenerReference::operator igdeNVNodeListener *() const{
	return ( igdeNVNodeListener* )operator deObject *();
}

igdeNVNodeListenerReference::operator igdeNVNodeListener &() const{
	return ( igdeNVNodeListener& )operator deObject &();
}

igdeNVNodeListener *igdeNVNodeListenerReference::operator->() const{
	return ( igdeNVNodeListener* )deObjectReference::operator->();
}

igdeNVNodeListenerReference &igdeNVNodeListenerReference::operator=( igdeNVNodeListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeNVNodeListenerReference &igdeNVNodeListenerReference::operator=( const igdeNVNodeListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVNodeListenerReference::operator==( igdeNVNodeListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeNVNodeListenerReference::operator!=( igdeNVNodeListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeNVNodeListenerReference::operator==( const igdeNVNodeListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVNodeListenerReference::operator!=( const igdeNVNodeListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
