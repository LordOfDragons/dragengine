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

#include "igdeMouseDragListener.h"
#include "igdeMouseDragListenerReference.h"



// Class igdeMouseDragListenerReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseDragListenerReference::igdeMouseDragListenerReference(){
}

igdeMouseDragListenerReference::igdeMouseDragListenerReference( igdeMouseDragListener *listener ) :
deObjectReference( listener ){
}

igdeMouseDragListenerReference::igdeMouseDragListenerReference( const igdeMouseDragListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMouseDragListenerReference::TakeOver( igdeMouseDragListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeMouseDragListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMouseDragListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMouseDragListenerReference::operator igdeMouseDragListener *() const{
	return ( igdeMouseDragListener* )operator deObject *();
}

igdeMouseDragListenerReference::operator igdeMouseDragListener &() const{
	return ( igdeMouseDragListener& )operator deObject &();
}

igdeMouseDragListener *igdeMouseDragListenerReference::operator->() const{
	return ( igdeMouseDragListener* )deObjectReference::operator->();
}

igdeMouseDragListenerReference &igdeMouseDragListenerReference::operator=( igdeMouseDragListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeMouseDragListenerReference &igdeMouseDragListenerReference::operator=( const igdeMouseDragListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMouseDragListenerReference::operator==( igdeMouseDragListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeMouseDragListenerReference::operator!=( igdeMouseDragListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeMouseDragListenerReference::operator==( const igdeMouseDragListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMouseDragListenerReference::operator!=( const igdeMouseDragListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
