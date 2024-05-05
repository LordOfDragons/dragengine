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

#include "igdeContainerBorder.h"
#include "igdeContainerBorderReference.h"



// Class igdeContainerBorderReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerBorderReference::igdeContainerBorderReference(){
}

igdeContainerBorderReference::igdeContainerBorderReference( igdeContainerBorder *widget ) :
deObjectReference( widget ){
}

igdeContainerBorderReference::igdeContainerBorderReference( const igdeContainerBorderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeContainerBorderReference::TakeOver( igdeContainerBorder *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeContainerBorderReference::operator!() const{
	return deObjectReference::operator!();
}

igdeContainerBorderReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeContainerBorderReference::operator igdeContainerBorder *() const{
	return ( igdeContainerBorder* )operator deObject *();
}

igdeContainerBorderReference::operator igdeContainerBorder &() const{
	return ( igdeContainerBorder& )operator deObject &();
}

igdeContainerBorder *igdeContainerBorderReference::operator->() const{
	return ( igdeContainerBorder* )deObjectReference::operator->();
}

igdeContainerBorderReference &igdeContainerBorderReference::operator=( igdeContainerBorder *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeContainerBorderReference &igdeContainerBorderReference::operator=( const igdeContainerBorderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeContainerBorderReference::operator==( igdeContainerBorder *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeContainerBorderReference::operator!=( igdeContainerBorder *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeContainerBorderReference::operator==( const igdeContainerBorderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeContainerBorderReference::operator!=( const igdeContainerBorderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
