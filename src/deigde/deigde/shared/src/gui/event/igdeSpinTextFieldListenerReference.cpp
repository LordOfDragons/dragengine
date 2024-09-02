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

#include "igdeSpinTextFieldListener.h"
#include "igdeSpinTextFieldListenerReference.h"



// Class igdeSpinTextFieldListenerReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpinTextFieldListenerReference::igdeSpinTextFieldListenerReference(){
}

igdeSpinTextFieldListenerReference::igdeSpinTextFieldListenerReference( igdeSpinTextFieldListener *listener ) :
deObjectReference( listener ){
}

igdeSpinTextFieldListenerReference::igdeSpinTextFieldListenerReference( const igdeSpinTextFieldListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSpinTextFieldListenerReference::TakeOver( igdeSpinTextFieldListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeSpinTextFieldListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSpinTextFieldListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSpinTextFieldListenerReference::operator igdeSpinTextFieldListener *() const{
	return ( igdeSpinTextFieldListener* )operator deObject *();
}

igdeSpinTextFieldListenerReference::operator igdeSpinTextFieldListener &() const{
	return ( igdeSpinTextFieldListener& )operator deObject &();
}

igdeSpinTextFieldListener *igdeSpinTextFieldListenerReference::operator->() const{
	return ( igdeSpinTextFieldListener* )deObjectReference::operator->();
}

igdeSpinTextFieldListenerReference &igdeSpinTextFieldListenerReference::operator=( igdeSpinTextFieldListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeSpinTextFieldListenerReference &igdeSpinTextFieldListenerReference::operator=( const igdeSpinTextFieldListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSpinTextFieldListenerReference::operator==( igdeSpinTextFieldListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeSpinTextFieldListenerReference::operator!=( igdeSpinTextFieldListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeSpinTextFieldListenerReference::operator==( const igdeSpinTextFieldListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSpinTextFieldListenerReference::operator!=( const igdeSpinTextFieldListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
