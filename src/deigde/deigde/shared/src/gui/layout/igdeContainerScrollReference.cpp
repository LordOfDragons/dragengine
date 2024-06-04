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

#include "igdeContainerScroll.h"
#include "igdeContainerScrollReference.h"



// Class igdeContainerScrollReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerScrollReference::igdeContainerScrollReference(){
}

igdeContainerScrollReference::igdeContainerScrollReference( igdeContainerScroll *container ) :
deObjectReference( container ){
}

igdeContainerScrollReference::igdeContainerScrollReference( const igdeContainerScrollReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeContainerScrollReference::TakeOver( igdeContainerScroll *container ){
	deObjectReference::TakeOver( container );
}

bool igdeContainerScrollReference::operator!() const{
	return deObjectReference::operator!();
}

igdeContainerScrollReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeContainerScrollReference::operator igdeContainerScroll *() const{
	return ( igdeContainerScroll* )operator deObject *();
}

igdeContainerScrollReference::operator igdeContainerScroll &() const{
	return ( igdeContainerScroll& )operator deObject &();
}

igdeContainerScroll *igdeContainerScrollReference::operator->() const{
	return ( igdeContainerScroll* )deObjectReference::operator->();
}

igdeContainerScrollReference &igdeContainerScrollReference::operator=( igdeContainerScroll *container ){
	deObjectReference::operator=( container );
	return *this;
}

igdeContainerScrollReference &igdeContainerScrollReference::operator=( const igdeContainerScrollReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeContainerScrollReference::operator==( igdeContainerScroll *container ) const{
	return deObjectReference::operator==( container );
}

bool igdeContainerScrollReference::operator!=( igdeContainerScroll *container ) const{
	return deObjectReference::operator!=( container );
}

bool igdeContainerScrollReference::operator==( const igdeContainerScrollReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeContainerScrollReference::operator!=( const igdeContainerScrollReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
