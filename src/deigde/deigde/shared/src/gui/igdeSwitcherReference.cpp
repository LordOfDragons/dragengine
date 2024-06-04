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

#include "igdeSwitcher.h"
#include "igdeSwitcherReference.h"



// Class igdeSwitcherReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSwitcherReference::igdeSwitcherReference(){
}

igdeSwitcherReference::igdeSwitcherReference( igdeSwitcher *switcher ) :
deObjectReference( switcher ){
}

igdeSwitcherReference::igdeSwitcherReference( const igdeSwitcherReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSwitcherReference::TakeOver( igdeSwitcher *switcher ){
	deObjectReference::TakeOver( switcher );
}

bool igdeSwitcherReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSwitcherReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSwitcherReference::operator igdeSwitcher *() const{
	return ( igdeSwitcher* )operator deObject *();
}

igdeSwitcherReference::operator igdeSwitcher &() const{
	return ( igdeSwitcher& )operator deObject &();
}

igdeSwitcher *igdeSwitcherReference::operator->() const{
	return ( igdeSwitcher* )deObjectReference::operator->();
}

igdeSwitcherReference &igdeSwitcherReference::operator=( igdeSwitcher *switcher ){
	deObjectReference::operator=( switcher );
	return *this;
}

igdeSwitcherReference &igdeSwitcherReference::operator=( const igdeSwitcherReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSwitcherReference::operator==( igdeSwitcher *switcher ) const{
	return deObjectReference::operator==( switcher );
}

bool igdeSwitcherReference::operator!=( igdeSwitcher *switcher ) const{
	return deObjectReference::operator!=( switcher );
}

bool igdeSwitcherReference::operator==( const igdeSwitcherReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSwitcherReference::operator!=( const igdeSwitcherReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
