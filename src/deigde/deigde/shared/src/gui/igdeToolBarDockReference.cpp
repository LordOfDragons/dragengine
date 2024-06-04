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

#include "igdeToolBarDock.h"
#include "igdeToolBarDockReference.h"



// Class igdeToolBarDockReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToolBarDockReference::igdeToolBarDockReference(){
}

igdeToolBarDockReference::igdeToolBarDockReference( igdeToolBarDock *dock ) :
deObjectReference( dock ){
}

igdeToolBarDockReference::igdeToolBarDockReference( const igdeToolBarDockReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToolBarDockReference::TakeOver( igdeToolBarDock *dock ){
	deObjectReference::TakeOver( dock );
}

bool igdeToolBarDockReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToolBarDockReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToolBarDockReference::operator igdeToolBarDock *() const{
	return ( igdeToolBarDock* )operator deObject *();
}

igdeToolBarDockReference::operator igdeToolBarDock &() const{
	return ( igdeToolBarDock& )operator deObject &();
}

igdeToolBarDock *igdeToolBarDockReference::operator->() const{
	return ( igdeToolBarDock* )deObjectReference::operator->();
}

igdeToolBarDockReference &igdeToolBarDockReference::operator=( igdeToolBarDock *dock ){
	deObjectReference::operator=( dock );
	return *this;
}

igdeToolBarDockReference &igdeToolBarDockReference::operator=( const igdeToolBarDockReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToolBarDockReference::operator==( igdeToolBarDock *dock ) const{
	return deObjectReference::operator==( dock );
}

bool igdeToolBarDockReference::operator!=( igdeToolBarDock *dock ) const{
	return deObjectReference::operator!=( dock );
}

bool igdeToolBarDockReference::operator==( const igdeToolBarDockReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToolBarDockReference::operator!=( const igdeToolBarDockReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
