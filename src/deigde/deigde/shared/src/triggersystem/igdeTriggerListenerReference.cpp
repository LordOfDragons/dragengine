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

#include "igdeTriggerListener.h"
#include "igdeTriggerListenerReference.h"



// Class igdeTriggerListenerReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerListenerReference::igdeTriggerListenerReference(){
}

igdeTriggerListenerReference::igdeTriggerListenerReference( igdeTriggerListener *listener ) :
deObjectReference( listener ){
}

igdeTriggerListenerReference::igdeTriggerListenerReference( const igdeTriggerListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTriggerListenerReference::TakeOver( igdeTriggerListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTriggerListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTriggerListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTriggerListenerReference::operator igdeTriggerListener *() const{
	return ( igdeTriggerListener* )operator deObject *();
}

igdeTriggerListenerReference::operator igdeTriggerListener &() const{
	return ( igdeTriggerListener& )operator deObject &();
}

igdeTriggerListener *igdeTriggerListenerReference::operator->() const{
	return ( igdeTriggerListener* )deObjectReference::operator->();
}

igdeTriggerListenerReference &igdeTriggerListenerReference::operator=( igdeTriggerListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTriggerListenerReference &igdeTriggerListenerReference::operator=( const igdeTriggerListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTriggerListenerReference::operator==( igdeTriggerListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTriggerListenerReference::operator!=( igdeTriggerListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTriggerListenerReference::operator==( const igdeTriggerListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTriggerListenerReference::operator!=( const igdeTriggerListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
