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

#include "igdeToolBar.h"
#include "igdeToolBarReference.h"



// Class igdeToolBarReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToolBarReference::igdeToolBarReference(){
}

igdeToolBarReference::igdeToolBarReference( igdeToolBar *toolbar ) :
deObjectReference( toolbar ){
}

igdeToolBarReference::igdeToolBarReference( const igdeToolBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToolBarReference::TakeOver( igdeToolBar *toolbar ){
	deObjectReference::TakeOver( toolbar );
}

bool igdeToolBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToolBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToolBarReference::operator igdeToolBar *() const{
	return ( igdeToolBar* )operator deObject *();
}

igdeToolBarReference::operator igdeToolBar &() const{
	return ( igdeToolBar& )operator deObject &();
}

igdeToolBar *igdeToolBarReference::operator->() const{
	return ( igdeToolBar* )deObjectReference::operator->();
}

igdeToolBarReference &igdeToolBarReference::operator=( igdeToolBar *toolbar ){
	deObjectReference::operator=( toolbar );
	return *this;
}

igdeToolBarReference &igdeToolBarReference::operator=( const igdeToolBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToolBarReference::operator==( igdeToolBar *toolbar ) const{
	return deObjectReference::operator==( toolbar );
}

bool igdeToolBarReference::operator!=( igdeToolBar *toolbar ) const{
	return deObjectReference::operator!=( toolbar );
}

bool igdeToolBarReference::operator==( const igdeToolBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToolBarReference::operator!=( const igdeToolBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
