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

#include "igdeListBox.h"
#include "igdeListBoxReference.h"



// Class igdeListBoxReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListBoxReference::igdeListBoxReference(){
}

igdeListBoxReference::igdeListBoxReference( igdeListBox *listbox ) :
deObjectReference( listbox ){
}

igdeListBoxReference::igdeListBoxReference( const igdeListBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListBoxReference::TakeOver( igdeListBox *listbox ){
	deObjectReference::TakeOver( listbox );
}

bool igdeListBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListBoxReference::operator igdeListBox *() const{
	return ( igdeListBox* )operator deObject *();
}

igdeListBoxReference::operator igdeListBox &() const{
	return ( igdeListBox& )operator deObject &();
}

igdeListBox *igdeListBoxReference::operator->() const{
	return ( igdeListBox* )deObjectReference::operator->();
}

igdeListBoxReference &igdeListBoxReference::operator=( igdeListBox *listbox ){
	deObjectReference::operator=( listbox );
	return *this;
}

igdeListBoxReference &igdeListBoxReference::operator=( const igdeListBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListBoxReference::operator==( igdeListBox *listbox ) const{
	return deObjectReference::operator==( listbox );
}

bool igdeListBoxReference::operator!=( igdeListBox *listbox ) const{
	return deObjectReference::operator!=( listbox );
}

bool igdeListBoxReference::operator==( const igdeListBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListBoxReference::operator!=( const igdeListBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
