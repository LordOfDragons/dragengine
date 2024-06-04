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

#include "igdeMouseCameraListener.h"
#include "igdeMouseCameraListenerReference.h"



// Class igdeMouseCameraListenerReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseCameraListenerReference::igdeMouseCameraListenerReference(){
}

igdeMouseCameraListenerReference::igdeMouseCameraListenerReference( igdeMouseCameraListener *listener ) :
deObjectReference( listener ){
}

igdeMouseCameraListenerReference::igdeMouseCameraListenerReference( const igdeMouseCameraListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMouseCameraListenerReference::TakeOver( igdeMouseCameraListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeMouseCameraListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMouseCameraListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMouseCameraListenerReference::operator igdeMouseCameraListener *() const{
	return ( igdeMouseCameraListener* )operator deObject *();
}

igdeMouseCameraListenerReference::operator igdeMouseCameraListener &() const{
	return ( igdeMouseCameraListener& )operator deObject &();
}

igdeMouseCameraListener *igdeMouseCameraListenerReference::operator->() const{
	return ( igdeMouseCameraListener* )deObjectReference::operator->();
}

igdeMouseCameraListenerReference &igdeMouseCameraListenerReference::operator=( igdeMouseCameraListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeMouseCameraListenerReference &igdeMouseCameraListenerReference::operator=( const igdeMouseCameraListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMouseCameraListenerReference::operator==( igdeMouseCameraListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeMouseCameraListenerReference::operator!=( igdeMouseCameraListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeMouseCameraListenerReference::operator==( const igdeMouseCameraListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMouseCameraListenerReference::operator!=( const igdeMouseCameraListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
