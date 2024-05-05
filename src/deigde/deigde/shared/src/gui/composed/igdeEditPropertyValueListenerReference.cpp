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

#include "igdeEditPropertyValueListener.h"
#include "igdeEditPropertyValueListenerReference.h"



// Class igdeEditPropertyValueListenerReference
/////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPropertyValueListenerReference::igdeEditPropertyValueListenerReference(){
}

igdeEditPropertyValueListenerReference::igdeEditPropertyValueListenerReference( igdeEditPropertyValueListener *listener ) :
deObjectReference( listener ){
}

igdeEditPropertyValueListenerReference::igdeEditPropertyValueListenerReference( const igdeEditPropertyValueListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPropertyValueListenerReference::TakeOver( igdeEditPropertyValueListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditPropertyValueListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPropertyValueListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPropertyValueListenerReference::operator igdeEditPropertyValueListener *() const{
	return ( igdeEditPropertyValueListener* )operator deObject *();
}

igdeEditPropertyValueListenerReference::operator igdeEditPropertyValueListener &() const{
	return ( igdeEditPropertyValueListener& )operator deObject &();
}

igdeEditPropertyValueListener *igdeEditPropertyValueListenerReference::operator->() const{
	return ( igdeEditPropertyValueListener* )deObjectReference::operator->();
}

igdeEditPropertyValueListenerReference &igdeEditPropertyValueListenerReference::operator=( igdeEditPropertyValueListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditPropertyValueListenerReference &igdeEditPropertyValueListenerReference::operator=( const igdeEditPropertyValueListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPropertyValueListenerReference::operator==( igdeEditPropertyValueListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditPropertyValueListenerReference::operator!=( igdeEditPropertyValueListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditPropertyValueListenerReference::operator==( const igdeEditPropertyValueListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPropertyValueListenerReference::operator!=( const igdeEditPropertyValueListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
