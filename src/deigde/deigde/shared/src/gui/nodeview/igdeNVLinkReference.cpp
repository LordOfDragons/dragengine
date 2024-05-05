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

#include "igdeNVLink.h"
#include "igdeNVLinkReference.h"



// Class igdeNVLinkReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVLinkReference::igdeNVLinkReference(){
}

igdeNVLinkReference::igdeNVLinkReference( igdeNVLink *link ) :
deObjectReference( link ){
}

igdeNVLinkReference::igdeNVLinkReference( const igdeNVLinkReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVLinkReference::TakeOver( igdeNVLink *link ){
	deObjectReference::TakeOver( link );
}

bool igdeNVLinkReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVLinkReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVLinkReference::operator igdeNVLink *() const{
	return ( igdeNVLink* )operator deObject *();
}

igdeNVLinkReference::operator igdeNVLink &() const{
	return ( igdeNVLink& )operator deObject &();
}

igdeNVLink *igdeNVLinkReference::operator->() const{
	return ( igdeNVLink* )deObjectReference::operator->();
}

igdeNVLinkReference &igdeNVLinkReference::operator=( igdeNVLink *link ){
	deObjectReference::operator=( link );
	return *this;
}

igdeNVLinkReference &igdeNVLinkReference::operator=( const igdeNVLinkReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVLinkReference::operator==( igdeNVLink *link ) const{
	return deObjectReference::operator==( link );
}

bool igdeNVLinkReference::operator!=( igdeNVLink *link ) const{
	return deObjectReference::operator!=( link );
}

bool igdeNVLinkReference::operator==( const igdeNVLinkReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVLinkReference::operator!=( const igdeNVLinkReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
