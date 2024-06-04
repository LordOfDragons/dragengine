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

#include "igdeTextAreaListener.h"
#include "igdeTextAreaListenerReference.h"



// Class igdeTextAreaListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextAreaListenerReference::igdeTextAreaListenerReference(){
}

igdeTextAreaListenerReference::igdeTextAreaListenerReference( igdeTextAreaListener *listener ) :
deObjectReference( listener ){
}

igdeTextAreaListenerReference::igdeTextAreaListenerReference( const igdeTextAreaListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextAreaListenerReference::TakeOver( igdeTextAreaListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTextAreaListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextAreaListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextAreaListenerReference::operator igdeTextAreaListener *() const{
	return ( igdeTextAreaListener* )operator deObject *();
}

igdeTextAreaListenerReference::operator igdeTextAreaListener &() const{
	return ( igdeTextAreaListener& )operator deObject &();
}

igdeTextAreaListener *igdeTextAreaListenerReference::operator->() const{
	return ( igdeTextAreaListener* )deObjectReference::operator->();
}

igdeTextAreaListenerReference &igdeTextAreaListenerReference::operator=( igdeTextAreaListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTextAreaListenerReference &igdeTextAreaListenerReference::operator=( const igdeTextAreaListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextAreaListenerReference::operator==( igdeTextAreaListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTextAreaListenerReference::operator!=( igdeTextAreaListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTextAreaListenerReference::operator==( const igdeTextAreaListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextAreaListenerReference::operator!=( const igdeTextAreaListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
