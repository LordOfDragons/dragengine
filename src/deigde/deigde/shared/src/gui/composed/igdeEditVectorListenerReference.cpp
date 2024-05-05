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

#include "igdeEditVectorListener.h"
#include "igdeEditVectorListenerReference.h"



// Class igdeEditVectorListenerReference
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVectorListenerReference::igdeEditVectorListenerReference(){
}

igdeEditVectorListenerReference::igdeEditVectorListenerReference( igdeEditVectorListener *listener ) :
deObjectReference( listener ){
}

igdeEditVectorListenerReference::igdeEditVectorListenerReference( const igdeEditVectorListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVectorListenerReference::TakeOver( igdeEditVectorListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditVectorListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVectorListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVectorListenerReference::operator igdeEditVectorListener *() const{
	return ( igdeEditVectorListener* )operator deObject *();
}

igdeEditVectorListenerReference::operator igdeEditVectorListener &() const{
	return ( igdeEditVectorListener& )operator deObject &();
}

igdeEditVectorListener *igdeEditVectorListenerReference::operator->() const{
	return ( igdeEditVectorListener* )deObjectReference::operator->();
}

igdeEditVectorListenerReference &igdeEditVectorListenerReference::operator=( igdeEditVectorListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditVectorListenerReference &igdeEditVectorListenerReference::operator=( const igdeEditVectorListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVectorListenerReference::operator==( igdeEditVectorListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditVectorListenerReference::operator!=( igdeEditVectorListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditVectorListenerReference::operator==( const igdeEditVectorListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVectorListenerReference::operator!=( const igdeEditVectorListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
