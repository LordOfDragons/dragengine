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

#include "igdeTreeList.h"
#include "igdeTreeListReference.h"



// Class igdeTreeListReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeListReference::igdeTreeListReference(){
}

igdeTreeListReference::igdeTreeListReference( igdeTreeList *treeList ) :
deObjectReference( treeList ){
}

igdeTreeListReference::igdeTreeListReference( const igdeTreeListReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeListReference::TakeOver( igdeTreeList *treeList ){
	deObjectReference::TakeOver( treeList );
}

bool igdeTreeListReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeListReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeListReference::operator igdeTreeList *() const{
	return ( igdeTreeList* )operator deObject *();
}

igdeTreeListReference::operator igdeTreeList &() const{
	return ( igdeTreeList& )operator deObject &();
}

igdeTreeList *igdeTreeListReference::operator->() const{
	return ( igdeTreeList* )deObjectReference::operator->();
}

igdeTreeListReference &igdeTreeListReference::operator=( igdeTreeList *treeList ){
	deObjectReference::operator=( treeList );
	return *this;
}

igdeTreeListReference &igdeTreeListReference::operator=( const igdeTreeListReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeListReference::operator==( igdeTreeList *treeList ) const{
	return deObjectReference::operator==( treeList );
}

bool igdeTreeListReference::operator!=( igdeTreeList *treeList ) const{
	return deObjectReference::operator!=( treeList );
}

bool igdeTreeListReference::operator==( const igdeTreeListReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeListReference::operator!=( const igdeTreeListReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
