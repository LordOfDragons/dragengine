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

#include "igdeEditDirectoryListener.h"
#include "igdeEditDirectoryListenerReference.h"



// Class igdeEditDirectoryListenerReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDirectoryListenerReference::igdeEditDirectoryListenerReference(){
}

igdeEditDirectoryListenerReference::igdeEditDirectoryListenerReference( igdeEditDirectoryListener *listener ) :
deObjectReference( listener ){
}

igdeEditDirectoryListenerReference::igdeEditDirectoryListenerReference( const igdeEditDirectoryListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDirectoryListenerReference::TakeOver( igdeEditDirectoryListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditDirectoryListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDirectoryListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDirectoryListenerReference::operator igdeEditDirectoryListener *() const{
	return ( igdeEditDirectoryListener* )operator deObject *();
}

igdeEditDirectoryListenerReference::operator igdeEditDirectoryListener &() const{
	return ( igdeEditDirectoryListener& )operator deObject &();
}

igdeEditDirectoryListener *igdeEditDirectoryListenerReference::operator->() const{
	return ( igdeEditDirectoryListener* )deObjectReference::operator->();
}

igdeEditDirectoryListenerReference &igdeEditDirectoryListenerReference::operator=( igdeEditDirectoryListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditDirectoryListenerReference &igdeEditDirectoryListenerReference::operator=( const igdeEditDirectoryListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDirectoryListenerReference::operator==( igdeEditDirectoryListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditDirectoryListenerReference::operator!=( igdeEditDirectoryListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditDirectoryListenerReference::operator==( const igdeEditDirectoryListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDirectoryListenerReference::operator!=( const igdeEditDirectoryListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
