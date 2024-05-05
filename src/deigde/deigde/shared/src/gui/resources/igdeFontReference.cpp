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

#include "igdeFont.h"
#include "igdeFontReference.h"



// Class igdeFontReference
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeFontReference::igdeFontReference(){
}

igdeFontReference::igdeFontReference( igdeFont *font ) :
deObjectReference( font ){
}

igdeFontReference::igdeFontReference( const igdeFontReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeFontReference::TakeOver( igdeFont *font ){
	deObjectReference::TakeOver( font );
}

bool igdeFontReference::operator!() const{
	return deObjectReference::operator!();
}

igdeFontReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeFontReference::operator igdeFont *() const{
	return ( igdeFont* )operator deObject *();
}

igdeFontReference::operator igdeFont &() const{
	return ( igdeFont& )operator deObject &();
}

igdeFont *igdeFontReference::operator->() const{
	return ( igdeFont* )deObjectReference::operator->();
}

igdeFontReference &igdeFontReference::operator=( igdeFont *font ){
	deObjectReference::operator=( font );
	return *this;
}

igdeFontReference &igdeFontReference::operator=( const igdeFontReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeFontReference::operator==( igdeFont *font ) const{
	return deObjectReference::operator==( font );
}

bool igdeFontReference::operator!=( igdeFont *font ) const{
	return deObjectReference::operator!=( font );
}

bool igdeFontReference::operator==( const igdeFontReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeFontReference::operator!=( const igdeFontReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
