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

#include "igdeCheckBox.h"
#include "igdeCheckBoxReference.h"



// Class igdeCheckBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeCheckBoxReference::igdeCheckBoxReference(){
}

igdeCheckBoxReference::igdeCheckBoxReference( igdeCheckBox *checkbox ) :
deObjectReference( checkbox ){
}

igdeCheckBoxReference::igdeCheckBoxReference( const igdeCheckBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeCheckBoxReference::TakeOver( igdeCheckBox *checkbox ){
	deObjectReference::TakeOver( checkbox );
}

bool igdeCheckBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeCheckBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeCheckBoxReference::operator igdeCheckBox *() const{
	return ( igdeCheckBox* )operator deObject *();
}

igdeCheckBoxReference::operator igdeCheckBox &() const{
	return ( igdeCheckBox& )operator deObject &();
}

igdeCheckBox *igdeCheckBoxReference::operator->() const{
	return ( igdeCheckBox* )deObjectReference::operator->();
}

igdeCheckBoxReference &igdeCheckBoxReference::operator=( igdeCheckBox *checkbox ){
	deObjectReference::operator=( checkbox );
	return *this;
}

igdeCheckBoxReference &igdeCheckBoxReference::operator=( const igdeCheckBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeCheckBoxReference::operator==( igdeCheckBox *checkbox ) const{
	return deObjectReference::operator==( checkbox );
}

bool igdeCheckBoxReference::operator!=( igdeCheckBox *checkbox ) const{
	return deObjectReference::operator!=( checkbox );
}

bool igdeCheckBoxReference::operator==( const igdeCheckBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeCheckBoxReference::operator!=( const igdeCheckBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
