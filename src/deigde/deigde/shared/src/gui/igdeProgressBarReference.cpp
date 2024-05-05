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

#include "igdeProgressBar.h"
#include "igdeProgressBarReference.h"



// Class igdeProgressBarReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeProgressBarReference::igdeProgressBarReference(){
}

igdeProgressBarReference::igdeProgressBarReference( igdeProgressBar *textfield ) :
deObjectReference( textfield ){
}

igdeProgressBarReference::igdeProgressBarReference( const igdeProgressBarReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeProgressBarReference::TakeOver( igdeProgressBar *textfield ){
	deObjectReference::TakeOver( textfield );
}

bool igdeProgressBarReference::operator!() const{
	return deObjectReference::operator!();
}

igdeProgressBarReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeProgressBarReference::operator igdeProgressBar *() const{
	return ( igdeProgressBar* )operator deObject *();
}

igdeProgressBarReference::operator igdeProgressBar &() const{
	return ( igdeProgressBar& )operator deObject &();
}

igdeProgressBar *igdeProgressBarReference::operator->() const{
	return ( igdeProgressBar* )deObjectReference::operator->();
}

igdeProgressBarReference &igdeProgressBarReference::operator=( igdeProgressBar *textfield ){
	deObjectReference::operator=( textfield );
	return *this;
}

igdeProgressBarReference &igdeProgressBarReference::operator=( const igdeProgressBarReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeProgressBarReference::operator==( igdeProgressBar *textfield ) const{
	return deObjectReference::operator==( textfield );
}

bool igdeProgressBarReference::operator!=( igdeProgressBar *textfield ) const{
	return deObjectReference::operator!=( textfield );
}

bool igdeProgressBarReference::operator==( const igdeProgressBarReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeProgressBarReference::operator!=( const igdeProgressBarReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
