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

#include "igdeGroupBox.h"
#include "igdeGroupBoxReference.h"



// Class igdeGroupBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGroupBoxReference::igdeGroupBoxReference(){
}

igdeGroupBoxReference::igdeGroupBoxReference( igdeGroupBox *groupbox ) :
deObjectReference( groupbox ){
}

igdeGroupBoxReference::igdeGroupBoxReference( const igdeGroupBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGroupBoxReference::TakeOver( igdeGroupBox *groupbox ){
	deObjectReference::TakeOver( groupbox );
}

bool igdeGroupBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGroupBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGroupBoxReference::operator igdeGroupBox *() const{
	return ( igdeGroupBox* )operator deObject *();
}

igdeGroupBoxReference::operator igdeGroupBox &() const{
	return ( igdeGroupBox& )operator deObject &();
}

igdeGroupBox *igdeGroupBoxReference::operator->() const{
	return ( igdeGroupBox* )deObjectReference::operator->();
}

igdeGroupBoxReference &igdeGroupBoxReference::operator=( igdeGroupBox *groupbox ){
	deObjectReference::operator=( groupbox );
	return *this;
}

igdeGroupBoxReference &igdeGroupBoxReference::operator=( const igdeGroupBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGroupBoxReference::operator==( igdeGroupBox *groupbox ) const{
	return deObjectReference::operator==( groupbox );
}

bool igdeGroupBoxReference::operator!=( igdeGroupBox *groupbox ) const{
	return deObjectReference::operator!=( groupbox );
}

bool igdeGroupBoxReference::operator==( const igdeGroupBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGroupBoxReference::operator!=( const igdeGroupBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
