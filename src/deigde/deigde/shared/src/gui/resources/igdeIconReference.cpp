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

#include "igdeIcon.h"
#include "igdeIconReference.h"



// Class igdeIconReference
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeIconReference::igdeIconReference(){
}

igdeIconReference::igdeIconReference( igdeIcon *icon ) :
deObjectReference( icon ){
}

igdeIconReference::igdeIconReference( const igdeIconReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeIconReference::TakeOver( igdeIcon *icon ){
	deObjectReference::TakeOver( icon );
}

bool igdeIconReference::operator!() const{
	return deObjectReference::operator!();
}

igdeIconReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeIconReference::operator igdeIcon *() const{
	return ( igdeIcon* )operator deObject *();
}

igdeIconReference::operator igdeIcon &() const{
	return ( igdeIcon& )operator deObject &();
}

igdeIcon *igdeIconReference::operator->() const{
	return ( igdeIcon* )deObjectReference::operator->();
}

igdeIconReference &igdeIconReference::operator=( igdeIcon *icon ){
	deObjectReference::operator=( icon );
	return *this;
}

igdeIconReference &igdeIconReference::operator=( const igdeIconReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeIconReference::operator==( igdeIcon *icon ) const{
	return deObjectReference::operator==( icon );
}

bool igdeIconReference::operator!=( igdeIcon *icon ) const{
	return deObjectReference::operator!=( icon );
}

bool igdeIconReference::operator==( const igdeIconReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeIconReference::operator!=( const igdeIconReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
