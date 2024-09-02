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

#include "igdeTreeListListener.h"
#include "igdeTreeListListenerReference.h"



// Class igdeTreeListListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeListListenerReference::igdeTreeListListenerReference(){
}

igdeTreeListListenerReference::igdeTreeListListenerReference( igdeTreeListListener *listener ) :
deObjectReference( listener ){
}

igdeTreeListListenerReference::igdeTreeListListenerReference( const igdeTreeListListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeListListenerReference::TakeOver( igdeTreeListListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTreeListListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeListListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeListListenerReference::operator igdeTreeListListener *() const{
	return ( igdeTreeListListener* )operator deObject *();
}

igdeTreeListListenerReference::operator igdeTreeListListener &() const{
	return ( igdeTreeListListener& )operator deObject &();
}

igdeTreeListListener *igdeTreeListListenerReference::operator->() const{
	return ( igdeTreeListListener* )deObjectReference::operator->();
}

igdeTreeListListenerReference &igdeTreeListListenerReference::operator=( igdeTreeListListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTreeListListenerReference &igdeTreeListListenerReference::operator=( const igdeTreeListListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeListListenerReference::operator==( igdeTreeListListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTreeListListenerReference::operator!=( igdeTreeListListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTreeListListenerReference::operator==( const igdeTreeListListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeListListenerReference::operator!=( const igdeTreeListListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
