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

#include "igdeWPWObject.h"
#include "igdeWPWObjectReference.h"



// Class igdeWPWObjectReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPWObjectReference::igdeWPWObjectReference(){
}

igdeWPWObjectReference::igdeWPWObjectReference( igdeWPWObject *widget ) :
deObjectReference( widget ){
}

igdeWPWObjectReference::igdeWPWObjectReference( const igdeWPWObjectReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPWObjectReference::TakeOver( igdeWPWObject *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPWObjectReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPWObjectReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPWObjectReference::operator igdeWPWObject *() const{
	return ( igdeWPWObject* )operator deObject *();
}

igdeWPWObjectReference::operator igdeWPWObject &() const{
	return ( igdeWPWObject& )operator deObject &();
}

igdeWPWObject *igdeWPWObjectReference::operator->() const{
	return ( igdeWPWObject* )deObjectReference::operator->();
}

igdeWPWObjectReference &igdeWPWObjectReference::operator=( igdeWPWObject *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPWObjectReference &igdeWPWObjectReference::operator=( const igdeWPWObjectReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPWObjectReference::operator==( igdeWPWObject *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPWObjectReference::operator!=( igdeWPWObject *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPWObjectReference::operator==( const igdeWPWObjectReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPWObjectReference::operator!=( const igdeWPWObjectReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
