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

#include "igdeIconListBoxListener.h"
#include "igdeIconListBoxListenerReference.h"



// Class igdeIconListBoxListenerReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeIconListBoxListenerReference::igdeIconListBoxListenerReference(){
}

igdeIconListBoxListenerReference::igdeIconListBoxListenerReference( igdeIconListBoxListener *listener ) :
deObjectReference( listener ){
}

igdeIconListBoxListenerReference::igdeIconListBoxListenerReference( const igdeIconListBoxListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeIconListBoxListenerReference::TakeOver( igdeIconListBoxListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeIconListBoxListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeIconListBoxListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeIconListBoxListenerReference::operator igdeIconListBoxListener *() const{
	return ( igdeIconListBoxListener* )operator deObject *();
}

igdeIconListBoxListenerReference::operator igdeIconListBoxListener &() const{
	return ( igdeIconListBoxListener& )operator deObject &();
}

igdeIconListBoxListener *igdeIconListBoxListenerReference::operator->() const{
	return ( igdeIconListBoxListener* )deObjectReference::operator->();
}

igdeIconListBoxListenerReference &igdeIconListBoxListenerReference::operator=( igdeIconListBoxListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeIconListBoxListenerReference &igdeIconListBoxListenerReference::operator=( const igdeIconListBoxListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeIconListBoxListenerReference::operator==( igdeIconListBoxListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeIconListBoxListenerReference::operator!=( igdeIconListBoxListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeIconListBoxListenerReference::operator==( const igdeIconListBoxListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeIconListBoxListenerReference::operator!=( const igdeIconListBoxListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
