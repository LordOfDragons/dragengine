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

#include "igdeListItem.h"
#include "igdeListItemReference.h"



// Class igdeListItemReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListItemReference::igdeListItemReference(){
}

igdeListItemReference::igdeListItemReference( igdeListItem *listItem ) :
deObjectReference( listItem ){
}

igdeListItemReference::igdeListItemReference( const igdeListItemReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListItemReference::TakeOver( igdeListItem *listItem ){
	deObjectReference::TakeOver( listItem );
}

bool igdeListItemReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListItemReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListItemReference::operator igdeListItem *() const{
	return ( igdeListItem* )operator deObject *();
}

igdeListItemReference::operator igdeListItem &() const{
	return ( igdeListItem& )operator deObject &();
}

igdeListItem *igdeListItemReference::operator->() const{
	return ( igdeListItem* )deObjectReference::operator->();
}

igdeListItemReference &igdeListItemReference::operator=( igdeListItem *listItem ){
	deObjectReference::operator=( listItem );
	return *this;
}

igdeListItemReference &igdeListItemReference::operator=( const igdeListItemReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListItemReference::operator==( igdeListItem *listItem ) const{
	return deObjectReference::operator==( listItem );
}

bool igdeListItemReference::operator!=( igdeListItem *listItem ) const{
	return deObjectReference::operator!=( listItem );
}

bool igdeListItemReference::operator==( const igdeListItemReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListItemReference::operator!=( const igdeListItemReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
