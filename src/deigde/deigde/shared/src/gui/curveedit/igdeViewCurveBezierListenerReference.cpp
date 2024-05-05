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

#include "igdeViewCurveBezierListener.h"
#include "igdeViewCurveBezierListenerReference.h"



// Class igdeViewCurveBezierListenerReference
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewCurveBezierListenerReference::igdeViewCurveBezierListenerReference(){
}

igdeViewCurveBezierListenerReference::igdeViewCurveBezierListenerReference( igdeViewCurveBezierListener *listener ) :
deObjectReference( listener ){
}

igdeViewCurveBezierListenerReference::igdeViewCurveBezierListenerReference( const igdeViewCurveBezierListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeViewCurveBezierListenerReference::TakeOver( igdeViewCurveBezierListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeViewCurveBezierListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeViewCurveBezierListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeViewCurveBezierListenerReference::operator igdeViewCurveBezierListener *() const{
	return ( igdeViewCurveBezierListener* )operator deObject *();
}

igdeViewCurveBezierListenerReference::operator igdeViewCurveBezierListener &() const{
	return ( igdeViewCurveBezierListener& )operator deObject &();
}

igdeViewCurveBezierListener *igdeViewCurveBezierListenerReference::operator->() const{
	return ( igdeViewCurveBezierListener* )deObjectReference::operator->();
}

igdeViewCurveBezierListenerReference &igdeViewCurveBezierListenerReference::operator=( igdeViewCurveBezierListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeViewCurveBezierListenerReference &igdeViewCurveBezierListenerReference::operator=( const igdeViewCurveBezierListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeViewCurveBezierListenerReference::operator==( igdeViewCurveBezierListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeViewCurveBezierListenerReference::operator!=( igdeViewCurveBezierListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeViewCurveBezierListenerReference::operator==( const igdeViewCurveBezierListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeViewCurveBezierListenerReference::operator!=( const igdeViewCurveBezierListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
