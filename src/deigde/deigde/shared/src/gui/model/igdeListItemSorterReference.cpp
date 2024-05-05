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

#include "igdeListItemSorter.h"
#include "igdeListItemSorterReference.h"



// Class igdeListItemSorterReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListItemSorterReference::igdeListItemSorterReference(){
}

igdeListItemSorterReference::igdeListItemSorterReference( igdeListItemSorter *sorter ) :
deObjectReference( sorter ){
}

igdeListItemSorterReference::igdeListItemSorterReference( const igdeListItemSorterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListItemSorterReference::TakeOver( igdeListItemSorter *sorter ){
	deObjectReference::TakeOver( sorter );
}

bool igdeListItemSorterReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListItemSorterReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListItemSorterReference::operator igdeListItemSorter *() const{
	return ( igdeListItemSorter* )operator deObject *();
}

igdeListItemSorterReference::operator igdeListItemSorter &() const{
	return ( igdeListItemSorter& )operator deObject &();
}

igdeListItemSorter *igdeListItemSorterReference::operator->() const{
	return ( igdeListItemSorter* )deObjectReference::operator->();
}

igdeListItemSorterReference &igdeListItemSorterReference::operator=( igdeListItemSorter *sorter ){
	deObjectReference::operator=( sorter );
	return *this;
}

igdeListItemSorterReference &igdeListItemSorterReference::operator=( const igdeListItemSorterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListItemSorterReference::operator==( igdeListItemSorter *sorter ) const{
	return deObjectReference::operator==( sorter );
}

bool igdeListItemSorterReference::operator!=( igdeListItemSorter *sorter ) const{
	return deObjectReference::operator!=( sorter );
}

bool igdeListItemSorterReference::operator==( const igdeListItemSorterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListItemSorterReference::operator!=( const igdeListItemSorterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
