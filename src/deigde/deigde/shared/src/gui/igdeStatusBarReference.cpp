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

#include "igdeStatusBar.h"
#include "igdeStatusBarReference.h"



// Class igdeStatusBarReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeStatusBarReference::igdeStatusBarReference(){
}

igdeStatusBarReference::igdeStatusBarReference( igdeStatusBar *statusBar ) :
deObjectReference( statusBar ){
}

igdeStatusBarReference::igdeStatusBarReference( const igdeStatusBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeStatusBarReference::TakeOver( igdeStatusBar *statusBar ){
	deObjectReference::TakeOver( statusBar );
}

bool igdeStatusBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeStatusBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeStatusBarReference::operator igdeStatusBar *() const{
	return ( igdeStatusBar* )operator deObject *();
}

igdeStatusBarReference::operator igdeStatusBar &() const{
	return ( igdeStatusBar& )operator deObject &();
}

igdeStatusBar *igdeStatusBarReference::operator->() const{
	return ( igdeStatusBar* )deObjectReference::operator->();
}

igdeStatusBarReference &igdeStatusBarReference::operator=( igdeStatusBar *statusBar ){
	deObjectReference::operator=( statusBar );
	return *this;
}

igdeStatusBarReference &igdeStatusBarReference::operator=( const igdeStatusBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeStatusBarReference::operator==( igdeStatusBar *statusBar ) const{
	return deObjectReference::operator==( statusBar );
}

bool igdeStatusBarReference::operator!=( igdeStatusBar *statusBar ) const{
	return deObjectReference::operator!=( statusBar );
}

bool igdeStatusBarReference::operator==( const igdeStatusBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeStatusBarReference::operator!=( const igdeStatusBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
