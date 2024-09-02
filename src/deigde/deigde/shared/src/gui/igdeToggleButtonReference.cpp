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

#include "igdeToggleButton.h"
#include "igdeToggleButtonReference.h"



// Class igdeToggleButtonReference
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToggleButtonReference::igdeToggleButtonReference(){
}

igdeToggleButtonReference::igdeToggleButtonReference( igdeToggleButton *button ) :
deObjectReference( button ){
}

igdeToggleButtonReference::igdeToggleButtonReference( const igdeToggleButtonReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToggleButtonReference::TakeOver( igdeToggleButton *button ){
	deObjectReference::TakeOver( button );
}

bool igdeToggleButtonReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToggleButtonReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToggleButtonReference::operator igdeToggleButton *() const{
	return ( igdeToggleButton* )operator deObject *();
}

igdeToggleButtonReference::operator igdeToggleButton &() const{
	return ( igdeToggleButton& )operator deObject &();
}

igdeToggleButton *igdeToggleButtonReference::operator->() const{
	return ( igdeToggleButton* )deObjectReference::operator->();
}

igdeToggleButtonReference &igdeToggleButtonReference::operator=( igdeToggleButton *button ){
	deObjectReference::operator=( button );
	return *this;
}

igdeToggleButtonReference &igdeToggleButtonReference::operator=( const igdeToggleButtonReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToggleButtonReference::operator==( igdeToggleButton *button ) const{
	return deObjectReference::operator==( button );
}

bool igdeToggleButtonReference::operator!=( igdeToggleButton *button ) const{
	return deObjectReference::operator!=( button );
}

bool igdeToggleButtonReference::operator==( const igdeToggleButtonReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToggleButtonReference::operator!=( const igdeToggleButtonReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
