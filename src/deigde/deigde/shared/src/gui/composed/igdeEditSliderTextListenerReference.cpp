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

#include "igdeEditSliderTextListener.h"
#include "igdeEditSliderTextListenerReference.h"



// Class igdeEditSliderTextListenerReference
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditSliderTextListenerReference::igdeEditSliderTextListenerReference(){
}

igdeEditSliderTextListenerReference::igdeEditSliderTextListenerReference( igdeEditSliderTextListener *listener ) :
deObjectReference( listener ){
}

igdeEditSliderTextListenerReference::igdeEditSliderTextListenerReference( const igdeEditSliderTextListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditSliderTextListenerReference::TakeOver( igdeEditSliderTextListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditSliderTextListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditSliderTextListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditSliderTextListenerReference::operator igdeEditSliderTextListener *() const{
	return ( igdeEditSliderTextListener* )operator deObject *();
}

igdeEditSliderTextListenerReference::operator igdeEditSliderTextListener &() const{
	return ( igdeEditSliderTextListener& )operator deObject &();
}

igdeEditSliderTextListener *igdeEditSliderTextListenerReference::operator->() const{
	return ( igdeEditSliderTextListener* )deObjectReference::operator->();
}

igdeEditSliderTextListenerReference &igdeEditSliderTextListenerReference::operator=( igdeEditSliderTextListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditSliderTextListenerReference &igdeEditSliderTextListenerReference::operator=( const igdeEditSliderTextListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditSliderTextListenerReference::operator==( igdeEditSliderTextListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditSliderTextListenerReference::operator!=( igdeEditSliderTextListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditSliderTextListenerReference::operator==( const igdeEditSliderTextListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditSliderTextListenerReference::operator!=( const igdeEditSliderTextListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
