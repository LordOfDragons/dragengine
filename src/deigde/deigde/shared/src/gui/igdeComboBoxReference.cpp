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

#include "igdeComboBox.h"
#include "igdeComboBoxReference.h"



// Class igdeComboBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxReference::igdeComboBoxReference(){
}

igdeComboBoxReference::igdeComboBoxReference( igdeComboBox *combobox ) :
deObjectReference( combobox ){
}

igdeComboBoxReference::igdeComboBoxReference( const igdeComboBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeComboBoxReference::TakeOver( igdeComboBox *combobox ){
	deObjectReference::TakeOver( combobox );
}

bool igdeComboBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeComboBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeComboBoxReference::operator igdeComboBox *() const{
	return ( igdeComboBox* )operator deObject *();
}

igdeComboBoxReference::operator igdeComboBox &() const{
	return ( igdeComboBox& )operator deObject &();
}

igdeComboBox *igdeComboBoxReference::operator->() const{
	return ( igdeComboBox* )deObjectReference::operator->();
}

igdeComboBoxReference &igdeComboBoxReference::operator=( igdeComboBox *combobox ){
	deObjectReference::operator=( combobox );
	return *this;
}

igdeComboBoxReference &igdeComboBoxReference::operator=( const igdeComboBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeComboBoxReference::operator==( igdeComboBox *combobox ) const{
	return deObjectReference::operator==( combobox );
}

bool igdeComboBoxReference::operator!=( igdeComboBox *combobox ) const{
	return deObjectReference::operator!=( combobox );
}

bool igdeComboBoxReference::operator==( const igdeComboBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeComboBoxReference::operator!=( const igdeComboBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
