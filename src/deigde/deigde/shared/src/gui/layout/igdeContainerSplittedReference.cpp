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

#include "igdeContainerSplitted.h"
#include "igdeContainerSplittedReference.h"



// Class igdeContainerSplittedReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerSplittedReference::igdeContainerSplittedReference(){
}

igdeContainerSplittedReference::igdeContainerSplittedReference( igdeContainerSplitted *widget ) :
deObjectReference( widget ){
}

igdeContainerSplittedReference::igdeContainerSplittedReference( const igdeContainerSplittedReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeContainerSplittedReference::TakeOver( igdeContainerSplitted *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeContainerSplittedReference::operator!() const{
	return deObjectReference::operator!();
}

igdeContainerSplittedReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeContainerSplittedReference::operator igdeContainerSplitted *() const{
	return ( igdeContainerSplitted* )operator deObject *();
}

igdeContainerSplittedReference::operator igdeContainerSplitted &() const{
	return ( igdeContainerSplitted& )operator deObject &();
}

igdeContainerSplitted *igdeContainerSplittedReference::operator->() const{
	return ( igdeContainerSplitted* )deObjectReference::operator->();
}

igdeContainerSplittedReference &igdeContainerSplittedReference::operator=( igdeContainerSplitted *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeContainerSplittedReference &igdeContainerSplittedReference::operator=( const igdeContainerSplittedReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeContainerSplittedReference::operator==( igdeContainerSplitted *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeContainerSplittedReference::operator!=( igdeContainerSplitted *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeContainerSplittedReference::operator==( const igdeContainerSplittedReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeContainerSplittedReference::operator!=( const igdeContainerSplittedReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
