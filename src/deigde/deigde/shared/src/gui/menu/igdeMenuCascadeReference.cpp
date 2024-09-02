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

#include "igdeMenuCascade.h"
#include "igdeMenuCascadeReference.h"



// Class igdeMenuCascadeReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCascadeReference::igdeMenuCascadeReference(){
}

igdeMenuCascadeReference::igdeMenuCascadeReference( igdeMenuCascade *menuCascade ) :
deObjectReference( menuCascade ){
}

igdeMenuCascadeReference::igdeMenuCascadeReference( const igdeMenuCascadeReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMenuCascadeReference::TakeOver( igdeMenuCascade *menuCascade ){
	deObjectReference::TakeOver( menuCascade );
}

bool igdeMenuCascadeReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMenuCascadeReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMenuCascadeReference::operator igdeMenuCascade *() const{
	return ( igdeMenuCascade* )operator deObject *();
}

igdeMenuCascadeReference::operator igdeMenuCascade &() const{
	return ( igdeMenuCascade& )operator deObject &();
}

igdeMenuCascade *igdeMenuCascadeReference::operator->() const{
	return ( igdeMenuCascade* )deObjectReference::operator->();
}

igdeMenuCascadeReference &igdeMenuCascadeReference::operator=( igdeMenuCascade *menuCascade ){
	deObjectReference::operator=( menuCascade );
	return *this;
}

igdeMenuCascadeReference &igdeMenuCascadeReference::operator=( const igdeMenuCascadeReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMenuCascadeReference::operator==( igdeMenuCascade *menuCascade ) const{
	return deObjectReference::operator==( menuCascade );
}

bool igdeMenuCascadeReference::operator!=( igdeMenuCascade *menuCascade ) const{
	return deObjectReference::operator!=( menuCascade );
}

bool igdeMenuCascadeReference::operator==( const igdeMenuCascadeReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMenuCascadeReference::operator!=( const igdeMenuCascadeReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
