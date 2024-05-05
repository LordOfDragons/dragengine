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

#include "igdeSliderListener.h"
#include "igdeSliderListenerReference.h"



// Class igdeSliderListenerReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSliderListenerReference::igdeSliderListenerReference(){
}

igdeSliderListenerReference::igdeSliderListenerReference( igdeSliderListener *listener ) :
deObjectReference( listener ){
}

igdeSliderListenerReference::igdeSliderListenerReference( const igdeSliderListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSliderListenerReference::TakeOver( igdeSliderListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeSliderListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSliderListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSliderListenerReference::operator igdeSliderListener *() const{
	return ( igdeSliderListener* )operator deObject *();
}

igdeSliderListenerReference::operator igdeSliderListener &() const{
	return ( igdeSliderListener& )operator deObject &();
}

igdeSliderListener *igdeSliderListenerReference::operator->() const{
	return ( igdeSliderListener* )deObjectReference::operator->();
}

igdeSliderListenerReference &igdeSliderListenerReference::operator=( igdeSliderListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeSliderListenerReference &igdeSliderListenerReference::operator=( const igdeSliderListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSliderListenerReference::operator==( igdeSliderListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeSliderListenerReference::operator!=( igdeSliderListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeSliderListenerReference::operator==( const igdeSliderListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSliderListenerReference::operator!=( const igdeSliderListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
