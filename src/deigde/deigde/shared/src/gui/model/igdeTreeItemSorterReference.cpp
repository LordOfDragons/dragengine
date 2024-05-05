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

#include "igdeTreeItemSorter.h"
#include "igdeTreeItemSorterReference.h"



// Class igdeTreeItemSorterReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeItemSorterReference::igdeTreeItemSorterReference(){
}

igdeTreeItemSorterReference::igdeTreeItemSorterReference( igdeTreeItemSorter *sorter ) :
deObjectReference( sorter ){
}

igdeTreeItemSorterReference::igdeTreeItemSorterReference( const igdeTreeItemSorterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeItemSorterReference::TakeOver( igdeTreeItemSorter *sorter ){
	deObjectReference::TakeOver( sorter );
}

bool igdeTreeItemSorterReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeItemSorterReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeItemSorterReference::operator igdeTreeItemSorter *() const{
	return ( igdeTreeItemSorter* )operator deObject *();
}

igdeTreeItemSorterReference::operator igdeTreeItemSorter &() const{
	return ( igdeTreeItemSorter& )operator deObject &();
}

igdeTreeItemSorter *igdeTreeItemSorterReference::operator->() const{
	return ( igdeTreeItemSorter* )deObjectReference::operator->();
}

igdeTreeItemSorterReference &igdeTreeItemSorterReference::operator=( igdeTreeItemSorter *sorter ){
	deObjectReference::operator=( sorter );
	return *this;
}

igdeTreeItemSorterReference &igdeTreeItemSorterReference::operator=( const igdeTreeItemSorterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeItemSorterReference::operator==( igdeTreeItemSorter *sorter ) const{
	return deObjectReference::operator==( sorter );
}

bool igdeTreeItemSorterReference::operator!=( igdeTreeItemSorter *sorter ) const{
	return deObjectReference::operator!=( sorter );
}

bool igdeTreeItemSorterReference::operator==( const igdeTreeItemSorterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeItemSorterReference::operator!=( const igdeTreeItemSorterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
