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

#include "igdeColorBox.h"
#include "igdeColorBoxReference.h"



// Class igdeColorBoxReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeColorBoxReference::igdeColorBoxReference(){
}

igdeColorBoxReference::igdeColorBoxReference( igdeColorBox *colorbox ) :
deObjectReference( colorbox ){
}

igdeColorBoxReference::igdeColorBoxReference( const igdeColorBoxReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeColorBoxReference::TakeOver( igdeColorBox *colorbox ){
	deObjectReference::TakeOver( colorbox );
}

bool igdeColorBoxReference::operator!() const{
	return deObjectReference::operator!();
}

igdeColorBoxReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeColorBoxReference::operator igdeColorBox *() const{
	return ( igdeColorBox* )operator deObject *();
}

igdeColorBoxReference::operator igdeColorBox &() const{
	return ( igdeColorBox& )operator deObject &();
}

igdeColorBox *igdeColorBoxReference::operator->() const{
	return ( igdeColorBox* )deObjectReference::operator->();
}

igdeColorBoxReference &igdeColorBoxReference::operator=( igdeColorBox *colorbox ){
	deObjectReference::operator=( colorbox );
	return *this;
}

igdeColorBoxReference &igdeColorBoxReference::operator=( const igdeColorBoxReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeColorBoxReference::operator==( igdeColorBox *colorbox ) const{
	return deObjectReference::operator==( colorbox );
}

bool igdeColorBoxReference::operator!=( igdeColorBox *colorbox ) const{
	return deObjectReference::operator!=( colorbox );
}

bool igdeColorBoxReference::operator==( const igdeColorBoxReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeColorBoxReference::operator!=( const igdeColorBoxReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
