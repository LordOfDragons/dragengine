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

#include "igdeMenuBar.h"
#include "igdeMenuBarReference.h"



// Class igdeMenuBarReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuBarReference::igdeMenuBarReference(){
}

igdeMenuBarReference::igdeMenuBarReference( igdeMenuBar *menubar ) :
deObjectReference( menubar ){
}

igdeMenuBarReference::igdeMenuBarReference( const igdeMenuBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMenuBarReference::TakeOver( igdeMenuBar *menubar ){
	deObjectReference::TakeOver( menubar );
}

bool igdeMenuBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMenuBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMenuBarReference::operator igdeMenuBar *() const{
	return ( igdeMenuBar* )operator deObject *();
}

igdeMenuBarReference::operator igdeMenuBar &() const{
	return ( igdeMenuBar& )operator deObject &();
}

igdeMenuBar *igdeMenuBarReference::operator->() const{
	return ( igdeMenuBar* )deObjectReference::operator->();
}

igdeMenuBarReference &igdeMenuBarReference::operator=( igdeMenuBar *menubar ){
	deObjectReference::operator=( menubar );
	return *this;
}

igdeMenuBarReference &igdeMenuBarReference::operator=( const igdeMenuBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMenuBarReference::operator==( igdeMenuBar *menubar ) const{
	return deObjectReference::operator==( menubar );
}

bool igdeMenuBarReference::operator!=( igdeMenuBar *menubar ) const{
	return deObjectReference::operator!=( menubar );
}

bool igdeMenuBarReference::operator==( const igdeMenuBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMenuBarReference::operator!=( const igdeMenuBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
