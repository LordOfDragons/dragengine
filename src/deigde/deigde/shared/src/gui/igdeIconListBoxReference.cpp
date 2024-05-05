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

#include "igdeIconListBox.h"
#include "igdeIconListBoxReference.h"



// Class igdeIconListBoxReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeIconListBoxReference::igdeIconListBoxReference(){
}

igdeIconListBoxReference::igdeIconListBoxReference( igdeIconListBox *listbox ) :
deObjectReference( listbox ){
}

igdeIconListBoxReference::igdeIconListBoxReference( const igdeIconListBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeIconListBoxReference::TakeOver( igdeIconListBox *listbox ){
	deObjectReference::TakeOver( listbox );
}

bool igdeIconListBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeIconListBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeIconListBoxReference::operator igdeIconListBox *() const{
	return ( igdeIconListBox* )operator deObject *();
}

igdeIconListBoxReference::operator igdeIconListBox &() const{
	return ( igdeIconListBox& )operator deObject &();
}

igdeIconListBox *igdeIconListBoxReference::operator->() const{
	return ( igdeIconListBox* )deObjectReference::operator->();
}

igdeIconListBoxReference &igdeIconListBoxReference::operator=( igdeIconListBox *listbox ){
	deObjectReference::operator=( listbox );
	return *this;
}

igdeIconListBoxReference &igdeIconListBoxReference::operator=( const igdeIconListBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeIconListBoxReference::operator==( igdeIconListBox *listbox ) const{
	return deObjectReference::operator==( listbox );
}

bool igdeIconListBoxReference::operator!=( igdeIconListBox *listbox ) const{
	return deObjectReference::operator!=( listbox );
}

bool igdeIconListBoxReference::operator==( const igdeIconListBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeIconListBoxReference::operator!=( const igdeIconListBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
