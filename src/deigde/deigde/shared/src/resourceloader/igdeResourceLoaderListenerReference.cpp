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

#include "igdeResourceLoaderListener.h"
#include "igdeResourceLoaderListenerReference.h"



// Class igdeResourceLoaderListenerReference
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeResourceLoaderListenerReference::igdeResourceLoaderListenerReference(){
}

igdeResourceLoaderListenerReference::igdeResourceLoaderListenerReference( igdeResourceLoaderListener *listener ) :
deObjectReference( listener ){
}

igdeResourceLoaderListenerReference::igdeResourceLoaderListenerReference( const igdeResourceLoaderListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeResourceLoaderListenerReference::TakeOver( igdeResourceLoaderListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeResourceLoaderListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeResourceLoaderListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeResourceLoaderListenerReference::operator igdeResourceLoaderListener *() const{
	return ( igdeResourceLoaderListener* )operator deObject *();
}

igdeResourceLoaderListenerReference::operator igdeResourceLoaderListener &() const{
	return ( igdeResourceLoaderListener& )operator deObject &();
}

igdeResourceLoaderListener *igdeResourceLoaderListenerReference::operator->() const{
	return ( igdeResourceLoaderListener* )deObjectReference::operator->();
}

igdeResourceLoaderListenerReference &igdeResourceLoaderListenerReference::operator=( igdeResourceLoaderListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeResourceLoaderListenerReference &igdeResourceLoaderListenerReference::operator=( const igdeResourceLoaderListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeResourceLoaderListenerReference::operator==( igdeResourceLoaderListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeResourceLoaderListenerReference::operator!=( igdeResourceLoaderListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeResourceLoaderListenerReference::operator==( const igdeResourceLoaderListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeResourceLoaderListenerReference::operator!=( const igdeResourceLoaderListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
