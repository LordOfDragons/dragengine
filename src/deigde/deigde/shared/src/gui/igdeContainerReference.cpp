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

#include "igdeContainer.h"
#include "igdeContainerReference.h"



// Class igdeContainerReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerReference::igdeContainerReference(){
}

igdeContainerReference::igdeContainerReference( igdeContainer *container ) :
deObjectReference( container ){
}

igdeContainerReference::igdeContainerReference( const igdeContainerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeContainerReference::TakeOver( igdeContainer *container ){
	deObjectReference::TakeOver( container );
}

bool igdeContainerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeContainerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeContainerReference::operator igdeContainer *() const{
	return ( igdeContainer* )operator deObject *();
}

igdeContainerReference::operator igdeContainer &() const{
	return ( igdeContainer& )operator deObject &();
}

igdeContainer *igdeContainerReference::operator->() const{
	return ( igdeContainer* )deObjectReference::operator->();
}

igdeContainerReference &igdeContainerReference::operator=( igdeContainer *container ){
	deObjectReference::operator=( container );
	return *this;
}

igdeContainerReference &igdeContainerReference::operator=( const igdeContainerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeContainerReference::operator==( igdeContainer *container ) const{
	return deObjectReference::operator==( container );
}

bool igdeContainerReference::operator!=( igdeContainer *container ) const{
	return deObjectReference::operator!=( container );
}

bool igdeContainerReference::operator==( const igdeContainerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeContainerReference::operator!=( const igdeContainerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
