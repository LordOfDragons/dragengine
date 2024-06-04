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

#include "igdeSpinTextField.h"
#include "igdeSpinTextFieldReference.h"



// Class igdeSpinTextFieldReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpinTextFieldReference::igdeSpinTextFieldReference(){
}

igdeSpinTextFieldReference::igdeSpinTextFieldReference( igdeSpinTextField *textfield ) :
deObjectReference( textfield ){
}

igdeSpinTextFieldReference::igdeSpinTextFieldReference( const igdeSpinTextFieldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSpinTextFieldReference::TakeOver( igdeSpinTextField *textfield ){
	deObjectReference::TakeOver( textfield );
}

bool igdeSpinTextFieldReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSpinTextFieldReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSpinTextFieldReference::operator igdeSpinTextField *() const{
	return ( igdeSpinTextField* )operator deObject *();
}

igdeSpinTextFieldReference::operator igdeSpinTextField &() const{
	return ( igdeSpinTextField& )operator deObject &();
}

igdeSpinTextField *igdeSpinTextFieldReference::operator->() const{
	return ( igdeSpinTextField* )deObjectReference::operator->();
}

igdeSpinTextFieldReference &igdeSpinTextFieldReference::operator=( igdeSpinTextField *textfield ){
	deObjectReference::operator=( textfield );
	return *this;
}

igdeSpinTextFieldReference &igdeSpinTextFieldReference::operator=( const igdeSpinTextFieldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSpinTextFieldReference::operator==( igdeSpinTextField *textfield ) const{
	return deObjectReference::operator==( textfield );
}

bool igdeSpinTextFieldReference::operator!=( igdeSpinTextField *textfield ) const{
	return deObjectReference::operator!=( textfield );
}

bool igdeSpinTextFieldReference::operator==( const igdeSpinTextFieldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSpinTextFieldReference::operator!=( const igdeSpinTextFieldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
