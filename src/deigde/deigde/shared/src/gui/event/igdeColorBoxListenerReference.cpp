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

#include "igdeColorBoxListener.h"
#include "igdeColorBoxListenerReference.h"



// Class igdeColorBoxListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeColorBoxListenerReference::igdeColorBoxListenerReference(){
}

igdeColorBoxListenerReference::igdeColorBoxListenerReference( igdeColorBoxListener *listener ) :
deObjectReference( listener ){
}

igdeColorBoxListenerReference::igdeColorBoxListenerReference( const igdeColorBoxListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeColorBoxListenerReference::TakeOver( igdeColorBoxListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeColorBoxListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeColorBoxListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeColorBoxListenerReference::operator igdeColorBoxListener *() const{
	return ( igdeColorBoxListener* )operator deObject *();
}

igdeColorBoxListenerReference::operator igdeColorBoxListener &() const{
	return ( igdeColorBoxListener& )operator deObject &();
}

igdeColorBoxListener *igdeColorBoxListenerReference::operator->() const{
	return ( igdeColorBoxListener* )deObjectReference::operator->();
}

igdeColorBoxListenerReference &igdeColorBoxListenerReference::operator=( igdeColorBoxListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeColorBoxListenerReference &igdeColorBoxListenerReference::operator=( const igdeColorBoxListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeColorBoxListenerReference::operator==( igdeColorBoxListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeColorBoxListenerReference::operator!=( igdeColorBoxListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeColorBoxListenerReference::operator==( const igdeColorBoxListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeColorBoxListenerReference::operator!=( const igdeColorBoxListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
