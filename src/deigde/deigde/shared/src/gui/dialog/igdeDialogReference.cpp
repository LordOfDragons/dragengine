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

#include "igdeDialog.h"
#include "igdeDialogReference.h"



// Class igdeDialogReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogReference::igdeDialogReference(){
}

igdeDialogReference::igdeDialogReference( igdeDialog *dialog ) :
deObjectReference( dialog ){
}

igdeDialogReference::igdeDialogReference( const igdeDialogReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeDialogReference::TakeOver( igdeDialog *dialog ){
	deObjectReference::TakeOver( dialog );
}

bool igdeDialogReference::operator!() const{
	return deObjectReference::operator!();
}

igdeDialogReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeDialogReference::operator igdeDialog *() const{
	return ( igdeDialog* )operator deObject *();
}

igdeDialogReference::operator igdeDialog &() const{
	return ( igdeDialog& )operator deObject &();
}

igdeDialog *igdeDialogReference::operator->() const{
	return ( igdeDialog* )deObjectReference::operator->();
}

igdeDialogReference &igdeDialogReference::operator=( igdeDialog *dialog ){
	deObjectReference::operator=( dialog );
	return *this;
}

igdeDialogReference &igdeDialogReference::operator=( const igdeDialogReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeDialogReference::operator==( igdeDialog *dialog ) const{
	return deObjectReference::operator==( dialog );
}

bool igdeDialogReference::operator!=( igdeDialog *dialog ) const{
	return deObjectReference::operator!=( dialog );
}

bool igdeDialogReference::operator==( const igdeDialogReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeDialogReference::operator!=( const igdeDialogReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
