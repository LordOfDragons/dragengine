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

#include "igdeEditDVectorListener.h"
#include "igdeEditDVectorListenerReference.h"



// Class igdeEditDVectorListenerReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDVectorListenerReference::igdeEditDVectorListenerReference(){
}

igdeEditDVectorListenerReference::igdeEditDVectorListenerReference( igdeEditDVectorListener *listener ) :
deObjectReference( listener ){
}

igdeEditDVectorListenerReference::igdeEditDVectorListenerReference( const igdeEditDVectorListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDVectorListenerReference::TakeOver( igdeEditDVectorListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditDVectorListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDVectorListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDVectorListenerReference::operator igdeEditDVectorListener *() const{
	return ( igdeEditDVectorListener* )operator deObject *();
}

igdeEditDVectorListenerReference::operator igdeEditDVectorListener &() const{
	return ( igdeEditDVectorListener& )operator deObject &();
}

igdeEditDVectorListener *igdeEditDVectorListenerReference::operator->() const{
	return ( igdeEditDVectorListener* )deObjectReference::operator->();
}

igdeEditDVectorListenerReference &igdeEditDVectorListenerReference::operator=( igdeEditDVectorListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditDVectorListenerReference &igdeEditDVectorListenerReference::operator=( const igdeEditDVectorListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDVectorListenerReference::operator==( igdeEditDVectorListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditDVectorListenerReference::operator!=( igdeEditDVectorListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditDVectorListenerReference::operator==( const igdeEditDVectorListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDVectorListenerReference::operator!=( const igdeEditDVectorListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
