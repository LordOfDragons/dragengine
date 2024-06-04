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

#include "igdeTextFieldListener.h"
#include "igdeTextFieldListenerReference.h"



// Class igdeTextFieldListenerReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextFieldListenerReference::igdeTextFieldListenerReference(){
}

igdeTextFieldListenerReference::igdeTextFieldListenerReference( igdeTextFieldListener *listener ) :
deObjectReference( listener ){
}

igdeTextFieldListenerReference::igdeTextFieldListenerReference( const igdeTextFieldListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextFieldListenerReference::TakeOver( igdeTextFieldListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTextFieldListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextFieldListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextFieldListenerReference::operator igdeTextFieldListener *() const{
	return ( igdeTextFieldListener* )operator deObject *();
}

igdeTextFieldListenerReference::operator igdeTextFieldListener &() const{
	return ( igdeTextFieldListener& )operator deObject &();
}

igdeTextFieldListener *igdeTextFieldListenerReference::operator->() const{
	return ( igdeTextFieldListener* )deObjectReference::operator->();
}

igdeTextFieldListenerReference &igdeTextFieldListenerReference::operator=( igdeTextFieldListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTextFieldListenerReference &igdeTextFieldListenerReference::operator=( const igdeTextFieldListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextFieldListenerReference::operator==( igdeTextFieldListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTextFieldListenerReference::operator!=( igdeTextFieldListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTextFieldListenerReference::operator==( const igdeTextFieldListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextFieldListenerReference::operator!=( const igdeTextFieldListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
