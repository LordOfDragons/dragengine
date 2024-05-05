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

#include "igdeTextField.h"
#include "igdeTextFieldReference.h"



// Class igdeTextFieldReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextFieldReference::igdeTextFieldReference(){
}

igdeTextFieldReference::igdeTextFieldReference( igdeTextField *textfield ) :
deObjectReference( textfield ){
}

igdeTextFieldReference::igdeTextFieldReference( const igdeTextFieldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextFieldReference::TakeOver( igdeTextField *textfield ){
	deObjectReference::TakeOver( textfield );
}

bool igdeTextFieldReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextFieldReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextFieldReference::operator igdeTextField *() const{
	return ( igdeTextField* )operator deObject *();
}

igdeTextFieldReference::operator igdeTextField &() const{
	return ( igdeTextField& )operator deObject &();
}

igdeTextField *igdeTextFieldReference::operator->() const{
	return ( igdeTextField* )deObjectReference::operator->();
}

igdeTextFieldReference &igdeTextFieldReference::operator=( igdeTextField *textfield ){
	deObjectReference::operator=( textfield );
	return *this;
}

igdeTextFieldReference &igdeTextFieldReference::operator=( const igdeTextFieldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextFieldReference::operator==( igdeTextField *textfield ) const{
	return deObjectReference::operator==( textfield );
}

bool igdeTextFieldReference::operator!=( igdeTextField *textfield ) const{
	return deObjectReference::operator!=( textfield );
}

bool igdeTextFieldReference::operator==( const igdeTextFieldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextFieldReference::operator!=( const igdeTextFieldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
