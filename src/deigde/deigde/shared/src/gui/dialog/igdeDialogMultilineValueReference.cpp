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

#include "igdeDialogMultilineValue.h"
#include "igdeDialogMultilineValueReference.h"



// Class igdeDialogMultilineValueReference
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogMultilineValueReference::igdeDialogMultilineValueReference(){
}

igdeDialogMultilineValueReference::igdeDialogMultilineValueReference( igdeDialogMultilineValue *dialog ) :
deObjectReference( dialog ){
}

igdeDialogMultilineValueReference::igdeDialogMultilineValueReference( const igdeDialogMultilineValueReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeDialogMultilineValueReference::TakeOver( igdeDialogMultilineValue *dialog ){
	deObjectReference::TakeOver( dialog );
}

bool igdeDialogMultilineValueReference::operator!() const{
	return deObjectReference::operator!();
}

igdeDialogMultilineValueReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeDialogMultilineValueReference::operator igdeDialogMultilineValue *() const{
	return ( igdeDialogMultilineValue* )operator deObject *();
}

igdeDialogMultilineValueReference::operator igdeDialogMultilineValue &() const{
	return ( igdeDialogMultilineValue& )operator deObject &();
}

igdeDialogMultilineValue *igdeDialogMultilineValueReference::operator->() const{
	return ( igdeDialogMultilineValue* )deObjectReference::operator->();
}

igdeDialogMultilineValueReference &igdeDialogMultilineValueReference::operator=( igdeDialogMultilineValue *dialog ){
	deObjectReference::operator=( dialog );
	return *this;
}

igdeDialogMultilineValueReference &igdeDialogMultilineValueReference::operator=( const igdeDialogMultilineValueReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeDialogMultilineValueReference::operator==( igdeDialogMultilineValue *dialog ) const{
	return deObjectReference::operator==( dialog );
}

bool igdeDialogMultilineValueReference::operator!=( igdeDialogMultilineValue *dialog ) const{
	return deObjectReference::operator!=( dialog );
}

bool igdeDialogMultilineValueReference::operator==( const igdeDialogMultilineValueReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeDialogMultilineValueReference::operator!=( const igdeDialogMultilineValueReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
