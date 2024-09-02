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

#include "igdeEditPathListener.h"
#include "igdeEditPathListenerReference.h"



// Class igdeEditPathListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPathListenerReference::igdeEditPathListenerReference(){
}

igdeEditPathListenerReference::igdeEditPathListenerReference( igdeEditPathListener *listener ) :
deObjectReference( listener ){
}

igdeEditPathListenerReference::igdeEditPathListenerReference( const igdeEditPathListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPathListenerReference::TakeOver( igdeEditPathListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditPathListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPathListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPathListenerReference::operator igdeEditPathListener *() const{
	return ( igdeEditPathListener* )operator deObject *();
}

igdeEditPathListenerReference::operator igdeEditPathListener &() const{
	return ( igdeEditPathListener& )operator deObject &();
}

igdeEditPathListener *igdeEditPathListenerReference::operator->() const{
	return ( igdeEditPathListener* )deObjectReference::operator->();
}

igdeEditPathListenerReference &igdeEditPathListenerReference::operator=( igdeEditPathListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditPathListenerReference &igdeEditPathListenerReference::operator=( const igdeEditPathListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPathListenerReference::operator==( igdeEditPathListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditPathListenerReference::operator!=( igdeEditPathListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditPathListenerReference::operator==( const igdeEditPathListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPathListenerReference::operator!=( const igdeEditPathListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
