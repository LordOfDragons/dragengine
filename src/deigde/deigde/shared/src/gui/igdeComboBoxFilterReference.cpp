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

#include "igdeComboBoxFilter.h"
#include "igdeComboBoxFilterReference.h"



// Class igdeComboBoxFilterReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxFilterReference::igdeComboBoxFilterReference(){
}

igdeComboBoxFilterReference::igdeComboBoxFilterReference( igdeComboBoxFilter *combobox ) :
deObjectReference( combobox ){
}

igdeComboBoxFilterReference::igdeComboBoxFilterReference( const igdeComboBoxFilterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeComboBoxFilterReference::TakeOver( igdeComboBoxFilter *combobox ){
	deObjectReference::TakeOver( combobox );
}

bool igdeComboBoxFilterReference::operator!() const{
	return deObjectReference::operator!();
}

igdeComboBoxFilterReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeComboBoxFilterReference::operator igdeComboBoxFilter *() const{
	return ( igdeComboBoxFilter* )operator deObject *();
}

igdeComboBoxFilterReference::operator igdeComboBoxFilter &() const{
	return ( igdeComboBoxFilter& )operator deObject &();
}

igdeComboBoxFilter *igdeComboBoxFilterReference::operator->() const{
	return ( igdeComboBoxFilter* )deObjectReference::operator->();
}

igdeComboBoxFilterReference &igdeComboBoxFilterReference::operator=( igdeComboBoxFilter *combobox ){
	deObjectReference::operator=( combobox );
	return *this;
}

igdeComboBoxFilterReference &igdeComboBoxFilterReference::operator=( const igdeComboBoxFilterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeComboBoxFilterReference::operator==( igdeComboBoxFilter *combobox ) const{
	return deObjectReference::operator==( combobox );
}

bool igdeComboBoxFilterReference::operator!=( igdeComboBoxFilter *combobox ) const{
	return deObjectReference::operator!=( combobox );
}

bool igdeComboBoxFilterReference::operator==( const igdeComboBoxFilterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeComboBoxFilterReference::operator!=( const igdeComboBoxFilterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
