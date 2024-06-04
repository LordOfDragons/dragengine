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

#include "igdeTreeItem.h"
#include "igdeTreeItemReference.h"



// Class igdeTreeItemReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeItemReference::igdeTreeItemReference(){
}

igdeTreeItemReference::igdeTreeItemReference( igdeTreeItem *treeItem ) :
deObjectReference( treeItem ){
}

igdeTreeItemReference::igdeTreeItemReference( const igdeTreeItemReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeItemReference::TakeOver( igdeTreeItem *treeItem ){
	deObjectReference::TakeOver( treeItem );
}

bool igdeTreeItemReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeItemReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeItemReference::operator igdeTreeItem *() const{
	return ( igdeTreeItem* )operator deObject *();
}

igdeTreeItemReference::operator igdeTreeItem &() const{
	return ( igdeTreeItem& )operator deObject &();
}

igdeTreeItem *igdeTreeItemReference::operator->() const{
	return ( igdeTreeItem* )deObjectReference::operator->();
}

igdeTreeItemReference &igdeTreeItemReference::operator=( igdeTreeItem *treeItem ){
	deObjectReference::operator=( treeItem );
	return *this;
}

igdeTreeItemReference &igdeTreeItemReference::operator=( const igdeTreeItemReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeItemReference::operator==( igdeTreeItem *treeItem ) const{
	return deObjectReference::operator==( treeItem );
}

bool igdeTreeItemReference::operator!=( igdeTreeItem *treeItem ) const{
	return deObjectReference::operator!=( treeItem );
}

bool igdeTreeItemReference::operator==( const igdeTreeItemReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeItemReference::operator!=( const igdeTreeItemReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
